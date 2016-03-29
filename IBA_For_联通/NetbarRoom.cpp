// ModifyPwdDlg.cpp : ʵ���ļ�
//�޸�����Ի���

#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "NetbarRoom.h"
#include "Socket\LocalServer.h"
#include "ActiveMemberList.h"
#include "IBAHelpper.h"
#include "Encrypt.h"
#include ".\netbarroom.h"
#include "Dal\IBADAL.h"
#include "NDCreditDlg.h"
#include "NDReturnDlg.h"
#include "MainFrm.h"

// CNetbarRoomDlg �Ի���

IMPLEMENT_DYNAMIC(CNetbarRoomDlg, CNDDialog)
CNetbarRoomDlg::CNetbarRoomDlg(CWnd* pParent /*=NULL*/)
: CNDDialog(CNetbarRoomDlg::IDD, pParent)
{
	m_strIdCardSN = theApp.GetLastNetID();
	m_nAllMoney = 0;
}

CNetbarRoomDlg::~CNetbarRoomDlg()
{
}

void CNetbarRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_CREDIT,m_btnCredit);
	DDX_Text(pDX, IDC_STATIC_USERNAME, m_strName);
	DDX_Text(pDX, IDC_STATIC_NETBAR, m_strNetBar);
	DDX_Text(pDX, IDC_STATIC_PERSONALID, m_strPersonalID);
	DDX_Text(pDX, IDC_STATIC_CLASS, m_strUserClass);
	DDX_Text(pDX, IDC_EDIT_IDCARDSNO, m_strIdCardSN);
	DDX_Text(pDX, IDC_STATIC_ALL, m_strAllBalance);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CNetbarRoomDlg, CNDDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBnClickedBtnQuery)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CREDIT, OnBnClickedBtnCredit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnItemchangedList1)
	ON_NOTIFY(GLCN_CHILD_CONTROL, IDC_LIST1, OnGlcNotify)
END_MESSAGE_MAP()


// CNetbarRoomDlg ��Ϣ�������

void CNetbarRoomDlg::ShowListCheckOutBtn(INT nRow , BOOL bShow)
{
	CBCGPButton *pWnd = (CBCGPButton*)m_listCtrl.GetSubItemWnd(nRow,6);
	if (pWnd)
	{
		pWnd->ShowWindow(bShow);
		return;
	}
	
	if (bShow)
	{
		pWnd = new CBCGPButton;
		pWnd->m_bVisualManagerStyle = TRUE;
		pWnd->Create(LOAD_STRING(IDS_ROOM_REFUND),WS_CHILD|BS_PUSHBUTTON,
			CRect(0,0,0,0),&m_listCtrl,m_listCtrl.RegeisterCtrlID());
		m_listCtrl.SetItem(nRow,6,LOAD_STRING(IDS_ROOM_REFUND),0,pWnd,NULL);

		m_listCtrl.ShowAllSubWindows();
	}
}
void CNetbarRoomDlg::ShowListOpenBtn(INT nRow, BOOL bShow, BOOL bOpened, BOOL bEanble)
{
	CBCGPButton *pWnd = (CBCGPButton*)m_listCtrl.GetSubItemWnd(nRow,5);
	if (!bShow)
	{	
		if (pWnd)
		{
            pWnd->ShowWindow(FALSE);
		}
		return;
	}
	if (!pWnd)
	{
		pWnd = new CBCGPButton;
		pWnd->m_bVisualManagerStyle = TRUE;
		pWnd->Create(_T(""),WS_CHILD|BS_PUSHBUTTON,
			CRect(0,0,0,0),&m_listCtrl,m_listCtrl.RegeisterCtrlID());

		m_listCtrl.SetItem(nRow,5,_T(""),0,pWnd,NULL);
		m_listCtrl.ShowAllSubWindows();
		
	}
	if (bOpened)
	{
		pWnd->SetWindowText(LOAD_STRING(IDS_ROOM_OPENED));
	}
	else
	{
		pWnd->SetWindowText(LOAD_STRING(IDS_ROOM_OPEN));
	}
	pWnd->EnableWindow(bEanble);
}
void CNetbarRoomDlg::OnBnClickedOk()
{
	UpdateData();

	CWaitCursor Wait;
	INT iSel = m_listCtrl.GetSelectionMark();
	if (iSel < 0)
	{
		return;
	}
	
	CRoomCheckIn RoomCheckIn;

	RoomCheckIn.SetMemberId(m_nMemberId);	
	RoomCheckIn.SetPCClassID(m_arrRoomInfos.GetAt(iSel).GetRoomID());

	theApp.GetCurCashier()->DoRoomCheckIn(RoomCheckIn);

	GetDlgItem(IDOK)->GetWindowRect(m_ToolTipRect);
	SetToolTipPos(IDOK);

	if (RoomCheckIn.GetStatusCode() == 0)
	{
		ShowToolTip(RoomCheckIn.GetStatusMessage());

		CActiveMember ActiveMember;
		if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId,ActiveMember))
		{
			CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(CCurrentNetBarUser::GetInstance()->Balance);
		}
		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId,ActiveMember))
		{
			ActiveMember.SetPCClassID(m_arrRoomInfos.GetAt(iSel).GetRoomID());
			ActiveMember.SetRoomMemberId(m_nMemberId);
			CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember,TRUE);
		}

		GetDlgItem(IDOK)->EnableWindow(FALSE);

		ShowListOpenBtn(iSel, TRUE, TRUE, FALSE);  // ��ʾ�ѿ�ͨ��ť�����
		ShowListCheckOutBtn(iSel, TRUE);           // ��ʾ���˰�ť

		CRoomInfo& Info = m_arrRoomInfos.GetAt(iSel);
		Info.SetMemberID(m_nMemberId);
		Info.SetNetId(m_strNetId);
		Info.SetOpenTime(COleDateTime::GetCurrentTime());

		m_listCtrl.SetItemText(iSel,1, LOAD_STRING(IDS_ROOM_WHOLE)); // ��Ϊ�����Ʒ�
		m_listCtrl.SetItemText(iSel,2, m_strNetId); // ��ʾ���˺�
		m_listCtrl.SetSubItemTxtColor(iSel,1, RGB(0,0,0));

		m_bAutoQuery = TRUE;
		SetTimer(TimeIdOfAutoQuery,1000,NULL);
	}
	else
	{	
		ShowToolTip(RoomCheckIn.GetStatusMessage(),TRUE);
	}

	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();

	//AutoClose();
}


BOOL CNetbarRoomDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	DWORD dwEx = m_listCtrl.GetExtendedStyle();
	dwEx = dwEx | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listCtrl.SetExtendedStyle(dwEx);
	m_listCtrl.ModifyStyle(0,LVS_SINGLESEL);
	m_listCtrl.SetItemsHeight(24);

	CIBAString strHeader;
	strHeader.LoadString(IDS_ROOM_LIST_HEAD);//��������|����״̬|���˺�|�ϻ�����|��������
	CStringArray strArrs;
	CIBAHelpper::SplitLine(strHeader, strArrs);
	if (strArrs.GetCount() != 5)
	{
		CIBAHelpper::SplitLine(CString(_T("��������|����״̬|���˺�|�ϻ�����|��������")), strArrs);
	}

	UINT nWidths[5] = {80,90,140,80,80};
	for(int i=0; i < 5; i++)
	{
		m_listCtrl.InsertColumn(i,strArrs.GetAt(i),LVCFMT_LEFT,nWidths[i]);
	}

	m_listCtrl.InsertColumn(5,_T(""),LVCFMT_LEFT,60,TRUE,FALSE,TRUE,NULL,0,NULL);  // ��ͨ�İ�ť��
	m_listCtrl.InsertColumn(6,_T(""),LVCFMT_LEFT,60,TRUE,FALSE,TRUE,NULL,0,NULL);  // ���˵İ�ť��

	m_strIdCardSN = m_strNetId;

	UpdateData(FALSE);

	//CIBAHelpper::CreateFont(m_BigFont, 22, 900);
	//�Զ���ѯ��ˢ����ɨ��֤��������²���
	if (m_bAutoQuery)
	{
		SetTimer(TimeIdOfAutoQuery, 100, NULL);//ֻ��ѯһ��
	}
	else
	{
		if (m_strNetId.IsEmpty())
		{
			GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
		}
		else
		{
			GetDlgItem(IDC_BTN_QUERY)->SetFocus();
		}
	}

	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFont(&m_BigFont);

	GetDlgItem(IDC_STATIC_ALL)->SetFont(&m_BigFont);

	GetDlgItem(IDOK)->EnableWindow(FALSE);

	FillRoomList();



	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNetbarRoomDlg::ClearData()
{
	m_strPwd.Empty();
	m_strName.Empty();
	m_strMemberID.Empty();
	m_strNetBar.Empty();
	m_strPersonalID.Empty();
	m_strUserClass.Empty();
	m_strAllBalance.Empty();
}

void CNetbarRoomDlg::OnOldCard(UINT nCardId, UINT nMemberId)
{
	m_strMemberID.Format(_T("%d"), nMemberId);
	
	QueryBalance(nMemberId, nCardId);

	UpdateData(FALSE);

}

void CNetbarRoomDlg::OnNewCard(UINT nCardId)
{
	CNDDialog::OnNewCard( nCardId );
}

void CNetbarRoomDlg::OnGetBalanceOK(CBalanceMethod & Balance)
{
	m_strName = Balance.GetUserName();
	m_strPersonalID = Balance.GetPersonalId();
	m_strUserClass = CNetBarConfig::GetInstance()->GetUserClassName(Balance.GetUserClass());
	m_strNetBar = CNetBarConfig::GetInstance()->GetNetBarName(Balance.GetOpenNetbarId());

	//�ϼ� = �����˻�+�����˻�+�����˻�+Ԥ��������
	m_nAllMoney = Balance.GetCashBalance() + Balance.GetUsePresent() + Balance.GetCreditPresent() + Balance.GetOnlineAmount();
	m_strAllBalance.Format(_T("%.2f"), m_nAllMoney / 100.0);

	for(int i=0; i<m_arrRoomInfos.GetCount();i++)
	{
		CRoomInfo& info = m_arrRoomInfos.GetAt(i);
		CWnd* pWnd = m_listCtrl.GetSubItemWnd(i,5);
		if (pWnd
			&& info.GetMemberID() == 0
			&& info.GetOnlineCount() == 0 
			&& info.GetComputerCount() > 0)
		{
			pWnd->EnableWindow(TRUE); // ʹ�ÿ�ͨ��ť����
		}

		if (info.GetMemberID() == m_nMemberId)
		{
			m_listCtrl.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,  LVIS_SELECTED | LVIS_FOCUSED);
			pWnd = m_listCtrl.GetSubItemWnd(i,6);
			if (pWnd)
			{
				pWnd->SetFocus();
			}
		}
	}

}

void CNetbarRoomDlg::OnTimer(UINT nIDEvent)
{
	if (TimeIdOfAutoQuery == nIDEvent)
	{
		KillTimer(nIDEvent);

		if (m_bAutoQuery)
		{
			m_bAutoQuery = FALSE;
			OnBnClickedBtnQuery();
		}
	}
	if (13 == nIDEvent)
	{
		FillRoomList();
		KillTimer(13);
	}
	CNDDialog::OnTimer(nIDEvent);
}

BOOL CNetbarRoomDlg::DoLocalModeQuery()
{
	BOOL bRet = CNDDialog::DoLocalModeQuery();
	/*if (bRet)
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_strNetBar = CNetBarConfig::GetInstance()->GetNetBarName(CNetBarConfig::GetInstance()->GetNetBarId());
		m_strMemberID.Format(_T("%d,�����û�"), (int)m_nMemberId);
		UpdateData(FALSE);
	}*/
	return bRet;
}

BOOL CNetbarRoomDlg::DoLocalModeOperate()
{
	IBA_ASSERT2(0,"no code");
	return TRUE;
}


void CNetbarRoomDlg::OnBnClickedBtnQuery()
{
	//FillRoomList();
	CNDDialog::OnBnClickedButtonQuery();
}

HBRUSH CNetbarRoomDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNDDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ALL)
	{
		pDC->SetTextColor(RGB(0,0,255));
	}
	return hbr;
}

void CNetbarRoomDlg::OnBnClickedBtnCredit()
{
	CNDCreditDlg dlg;
	dlg.SetNetId(m_strIdCardSN);
	if (dlg.DoModal() == IDOK)
	{
		OnBnClickedBtnQuery();
	}
}

void CNetbarRoomDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if (pNMLV->iItem != -1 && pNMLV->iSubItem != -1 && m_arrRoomInfos.GetCount() > 0)
	{
		CRoomInfo& room = m_arrRoomInfos.GetAt(pNMLV->iItem);
		if (room.GetMemberID()==0 && room.GetOnlineCount()==0 && m_nMemberId > 0)
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}

void CNetbarRoomDlg::FillRoomList()
{
	m_listCtrl.DeleteAllItems();

	if (!theApp.GetCurCashier()->DoNetbarRoom(m_roomMethod))
	{
		return;
	}

	m_arrRoomInfos.RemoveAll();

	if (m_roomMethod.GetStatusCode() == NO_ERROR)
	{
		for(UINT i=0; i < m_roomMethod.GetRecordCount(); i++)
		{
			CRoomInfo info;
			info.SetRoomID() = m_roomMethod.GetRoomID(i);
			info.SetRoomName() = m_roomMethod.GetRoomName(i);
			info.SetMemberID() = m_roomMethod.GetMemberID(i);
			info.SetOpenTime() = CIBAHelpper::CenterTimeToOleDateTime(m_roomMethod.GetOpenTime(i));
			CIBADAL::GetInstance()->GetRoomsComputerCount(info);
			CLocalServer::GetInstance()->ActiveMemberList.FillRoomInfo(info);

			m_listCtrl.InsertItem(i, info.GetRoomName());
			BOOL bIdle = FALSE;
			if (info.GetMemberID() == 0)
			{
				if (info.GetOnlineCount() == 0)
				{
					m_listCtrl.SetItemText(i,1, LOAD_STRING(IDS_ROOM_IDLE)); // ��������
					bIdle = TRUE;
				}
				else
				{
					m_listCtrl.SetItemText(i,1, LOAD_STRING(IDS_ROOM_STANDALONE)); // �����Ʒ�
				}
			}
			else
			{
				m_listCtrl.SetItemText(i,1, LOAD_STRING(IDS_ROOM_WHOLE)); // �����Ʒ�
			}

			m_listCtrl.SetItemText(i, 2, info.GetNetId());

			CString strTmp;
			strTmp.Format(_T("%d"), info.GetOnlineCount());
			m_listCtrl.SetItemText(i, 3, strTmp);

			strTmp.Format(_T("%d"), info.GetComputerCount());
			m_listCtrl.SetItemText(i, 4, strTmp);
			m_arrRoomInfos.Add(info);

			if (bIdle)
			{
				m_listCtrl.SetSubItemTxtColor(i,1,RGB(255,0,0)); // ���е���ɫ��Ϊ��ɫ

				// ��ʾ�����ÿ�ͨ��ť
				ShowListOpenBtn(i, TRUE, FALSE, FALSE);
			}
			else
			{
				if (info.GetMemberID() > 0)
				{
					ShowListOpenBtn(i, TRUE, TRUE, FALSE); // �ѿ�ͨ��ť������
				}
			}

			if (info.GetMemberID()  > 0)
			{
				// ��ʾ���˰�ť
				ShowListCheckOutBtn(i, TRUE);
			}
		}

		m_listCtrl.ShowAllSubWindows();
	}
}

BOOL CNetbarRoomDlg::DoRoomCheckOut(CRoomInfo& Info)
{
	ASSERT(Info.GetMemberID() == m_nMemberId);

	CRoomCheckOut roomCheckOut;
	roomCheckOut.SetMemberId(Info.GetMemberID());
	roomCheckOut.SetPCClass(Info.GetRoomID());

	theApp.GetCurCashier()->DoRoomCheckOut(roomCheckOut);

	if (roomCheckOut.GetStatusCode() == 0)
	{
		SetToolTipPos(IDOK);
		ShowToolTip(roomCheckOut.GetStatusMessage());

		CActiveMember ActiveMember;
		BOOL bRead = CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(Info.GetMemberID(),ActiveMember);
		CArray<UINT> arrMembers;
		CLocalServer::GetInstance()->ActiveMemberList.GetRoomSMembers(Info.GetMemberID(), arrMembers);

		// ���������־
		{//д������־
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeRoomReturn);
			OperationInfo.SetIsBundTime(FALSE);

			if (bRead)
			{
				OperationInfo.SetUserName(ActiveMember.GetUserName());
				OperationInfo.SetNetId(ActiveMember.GetNetId());
				OperationInfo.SetMemberId(ActiveMember.GetMemberID());
				OperationInfo.SetTermId(ActiveMember.GetTerminalID());
				OperationInfo.SetIdNum(ActiveMember.GetNetId());
			}

			CString strTmp;
			strTmp += Info.GetRoomName();
			strTmp += _T(": ");
			strTmp.AppendFormat(LOAD_STRING(IDS_ROOM_BODY), arrMembers.GetCount());
			OperationInfo.SetDescription(strTmp);

			OperationInfo.SetCheckInTime(Info.GetOpenTime()); // �����Ŀ�ͨʱ��

			OperationInfo.SetClassId(ActiveMember.GetClassId());
		
			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
		}
		return TRUE;
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(roomCheckOut.GetStatusMessage(), TRUE);
		return FALSE;
	}
}
void CNetbarRoomDlg::AutoQueryRoom()
{
	SetTimer(13, 200,NULL);
}
void CNetbarRoomDlg::OnGlcNotify(NMHDR *pNMHDR, LRESULT *pResult)
{
	using namespace NS_COMMON;
	LPNMHDR_GLC_CONTROL pGlc = (LPNMHDR_GLC_CONTROL)pNMHDR;

	if (pGlc->pCtrlHdr->code == BN_CLICKED)
	{
		m_listCtrl.SetItemState(pGlc->nRowID, LVIS_SELECTED | LVIS_FOCUSED,  LVIS_SELECTED | LVIS_FOCUSED);
		m_listCtrl.SetSelectionMark(pGlc->nRowID);
		if (pGlc->nColID == 5)
		{
			OnBnClickedOk();
		}
		else
		{
			CRoomInfo& info = m_arrRoomInfos[pGlc->nRowID];

			if (DoRoomCheckOut(info))
			{
				ClearActiveMember(info.GetMemberID());

				if (info.GetMemberID() == m_nMemberId)
				{
					// ������˵��ǵ�ǰ�Ĳ�ѯ�û�
					// ����û���Ϣ
					m_strIdCardSN.Empty();
					m_nMemberId = 0;
					m_strMemberID.Empty();
					m_strUserClass.Empty();
					m_strNetBar.Empty();
					m_strPersonalID.Empty();
					m_strAllBalance.Empty();
					m_strName.Empty();

					UpdateData(FALSE);

				}
				/*info.SetMemberID(0);
				info.SetOnlineCount(0);
				m_listCtrl.SetItemText(pGlc->nRowID,3,_T("0"));*/

				

				AutoQueryRoom(); // ���¶�ȡ�����б�
			}
		}
	}
}

BOOL CNetbarRoomDlg::ClearActiveMember(UINT nRoomMemberID)
{
	CArray<UINT> arrMembers;
	if (!CLocalServer::GetInstance()->ActiveMemberList.GetRoomSMembers(nRoomMemberID, arrMembers))
	{
		IBA_LOG0(_T("�������û����ж�ȡ�����ڵ������û���Ϣʧ��"));
		//return FALSE;
	}

	IBA_LOG(_T("֪ͨ�����ڵ����е�UDO�»�,������MemberID=%d"), nRoomMemberID);

	for(int i=0; i < arrMembers.GetCount(); i++)
	{
		UINT nMemberId = arrMembers.GetAt(i);
		CActiveMember ActiveMember;
		CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);

		BOOL bClear = TRUE;
		if (ActiveMember.IsMember())
		{
			if (ActiveMember.GetPayType() > 0)
			{
				bClear = FALSE;
			}
		}
		else
		{
			// ���˺�����а�ʱ״̬������������ô���������
			if (ActiveMember.GetPayType() > 0 || ActiveMember.GetAvailavleBalance() > 0)
			{
				if (nRoomMemberID == nMemberId)
				{
					CNDReturnDlg dlg;
					dlg.SetNetId(ActiveMember.GetNetId());
					if (dlg.DoModal()!=IDOK)
					{
						
					}
					else
					{
						bClear = FALSE;
					}
				}
				else
				{
					bClear = FALSE;
				}
				
				
			}
		}
		INT_PTR nIn = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);

		if ( nIn!=-1 )
		{
			// ���͸�ָ���ͻ���, �ǳ�����������
			CLocalServer::GetInstance()->LocalCheckOutUser(nIn, nMemberId, 2);

			IBA_LOG(_T("�������ˣ�֪ͨUDO�»��� TermID=%s MemberID=%d"), ActiveMember.GetTerminalID(), ActiveMember.GetMemberID());
		}

		if (nMemberId == nRoomMemberID)
		{
			continue;
		}
		
		if (bClear)
		{
			CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(nMemberId);

			if (!ActiveMember.IsMember())
			{
				CDeregisterMethod Deregister;

				Deregister.SetMemberId(nMemberId);
				Deregister.SetPersonalId(ActiveMember.GetPersonalID());
				Deregister.SetMemberName(ActiveMember.GetUserName());
				Deregister.SetCardId(0);
				Deregister.SetPassword(_T(""));
				Deregister.SetEndUsingTime(_T(""));

				theApp.GetCurCashier()->DoDeregister(Deregister);

				if (Deregister.GetStatusCode() == 0)
				{	

				}
				else
				{
					SetToolTipPos(IDOK);
					ShowToolTip(Deregister.GetStatusMessage(), TRUE);
					return FALSE;
				}
			}
		}
	}

	CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);

	return TRUE;
}