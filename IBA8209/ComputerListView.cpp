
#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "MainFrm.h"
#include "Socket\LocalServer.h"
#include "ShortMsgDlg.h"
#include "MsgPopupWindow.h"
#include "CreditDlg.h"
#include "MsgBoxDlg.h"
#include "IBAHelpper.h"
#include "BundleTimeDlg.h"
#include "selcomputerdlg.h"
#include "UnlockScreenDlg.h"
#include "NDCreditDlg.h"
#include "NDReturnDlg.h"
#include "NDCheckInDlg.h"
#include ".\JXC\JXCMainDlg.h"
#include "ActiveMemberView.h"
#include ".\computerlistview.h"
#include "Tts\IBATTS.h"
#include "DdnPcpInc.h"
#include "CashierHelper.h"
#include "BossPwdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CComputerListView
//ע�⣬���ڱ���Ŀؼ������´����������Ϣ��������
//�ű����ڶ��㴰�巢����Ϣ��CComputerListView
IMPLEMENT_DYNCREATE(CComputerListView, CBCGPGridView)

BEGIN_MESSAGE_MAP(CComputerListView, CBCGPGridView)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()

	ON_UPDATE_COMMAND_UI(IDM_SNAPSCREEN, OnCheckClient)
	ON_UPDATE_COMMAND_UI(IDM_GETLOG, OnCheckClient)
	ON_UPDATE_COMMAND_UI(IDM_POWEROFF, OnCheckClient)
	ON_UPDATE_COMMAND_UI(IDM_UNUSUAL, OnCheckUnusal)
	ON_UPDATE_COMMAND_UI(IDM_PAYOUT, OnCheckPayOut)
	ON_UPDATE_COMMAND_UI(IDM_CREDITONLINE, OnCheckCredit)
	ON_UPDATE_COMMAND_UI(IDM_REBOOT, OnCheckClient)
	ON_UPDATE_COMMAND_UI(IDM_POWERON, OnCheckNotClient)
	ON_UPDATE_COMMAND_UI(IDM_COLUMN_SET, OnUpdateColumnSet)
	ON_UPDATE_COMMAND_UI(IDM_CONSUMELIST2, OnUpdateConsumelist)
	ON_UPDATE_COMMAND_UI(IDM_SENDSHORTMESSAGE, OnUpdateSendShortMessage)
	ON_UPDATE_COMMAND_UI(IDM_BUNDTIMEONLINE, OnUpdateBundtimeonline)
	ON_UPDATE_COMMAND_UI(IDM_TERMCONSUME2, OnUpdateTermConsume)

	ON_COMMAND_RANGE(IDM_MINCOLUMNCMD, IDM_MAXCOLUMNCMD, OnShowColumn)//�Զ�����ӵ��в˵���Ϣ

	ON_COMMAND(IDM_CREDITONLINE, OnCreditOnline)
	ON_COMMAND(IDM_COLUMN_SET, OnColumnSet)
	ON_COMMAND(IDM_PAYOUT, OnPayout)
	ON_COMMAND(IDM_UNUSUAL, OnChangeComputer)
	ON_COMMAND(IDM_REBOOT, OnReboot)
	ON_COMMAND(IDM_SNAPSCREEN, OnSnapScreen)
	ON_COMMAND(IDM_GETLOG, OnGetLog)
	ON_COMMAND(IDM_POWEROFF, OnPowerOff)
	ON_COMMAND(IDM_POWERON, OnPowerOn)
	ON_COMMAND(IDM_SENDSHORTMESSAGE, OnSendShortMessage)
	ON_COMMAND(IDM_MYREFRESH, OnRefresh)
	ON_COMMAND(IDM_CONSUMELIST2, OnConsumelist)
	ON_COMMAND(IDM_TERMCONSUME2, OnTermConsume)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_BUNDTIMEONLINE, OnBundtimeonline)
	
	ON_COMMAND(IDM_RETAILONLINE, OnRetailonline)
	ON_UPDATE_COMMAND_UI(IDM_RETAILONLINE, OnUpdateRetailonline)
	ON_COMMAND(IDM_PING, OnPing)
	ON_COMMAND(IDM_UNLOCKSCREEN, OnUnlockScreen)
	ON_UPDATE_COMMAND_UI(IDM_UNLOCKSCREEN, OnUpdateUnlockScreen)
	ON_MESSAGE(WM_COMPVIEW_REFRESH, OnCompRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComputerListView construction/destruction

CComputerListView::CComputerListView()
: m_lpComputerGird(NULL)
{
	CLocalServer::GetInstance()->SetComputerListView(this);//����ָ�룬���ⲿ����
}

CComputerListView::~CComputerListView()
{
	//L2_SAFE_DELETE(m_lpComputerGird)
	if (m_lpDataXMReceiver != NULL)
	{
		delete m_lpDataXMReceiver;
		m_lpDataXMReceiver = NULL;
	}
}

INT_PTR CComputerListView::MsgBox(LPCTSTR lpszText, UINT nType /*= MB_OK*/, LPCTSTR lpszCaption /*= NULL*/)
{
	CMsgBoxDlg dlg(this);
	dlg.SetCaption(lpszCaption);
	dlg.SetText(lpszText);
	dlg.SetType(nType);

	return dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CComputerListView message handlers

int CComputerListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1) return -1;
	//�������������,���Ҵ���ָ��
	m_lpComputerGird = new CComputerGrid(&CLocalServer::GetInstance()->ComputerList);

	m_pWndGridCtrl = m_lpComputerGird;
	//�����ؼ�,ʵ���ϣ�����GRIDView�ڲ����еĲ���
	if (!m_lpComputerGird->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))//����
	{
		return -1;
	}

	SetTimer(TimeIdOfPing, IntervalOfPing, NULL);//���ӳ�ʱ����

	m_lpDataXMReceiver = new CDataXMReceiverT<CComputerListView>
		(this, OnDataArrival);

	BOOL bRet = m_lpDataXMReceiver->Create(_T("DdnPcp-0ED70A77-233A-43c0-9490-962283436DF5"));
	if (!bRet)
	{
		IBA_LOG0(_T("CComputerListView-�������̼�ͨ�Ŷ���ʧ��"));
	}

	return 0;
}

void CComputerListView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
}

//ȡ�õ�ǰ��ѡ�ն���Ϣ
BOOL CComputerListView::GetCurComputerInfo(CComputerInfo & ComputerInfo)
{
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow != NULL) 
	{
		if (!CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(pRow->GetData()))
		{
			ComputerInfo = CLocalServer::GetInstance()->ComputerList.GetComputer(pRow->GetData());

			return TRUE;
		}
	}

	return FALSE;
}

//ͨ������ŵõ��ն���Ϣ
BOOL CComputerListView::GetComputerInfoFromRow(CComputerInfo & ComputerInfo, INT RowIndex)
{
	CBCGPGridRow* pRow = GetGridCtrl()->GetRow(RowIndex);

	if (pRow != NULL) 
	{
		if (!CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(pRow->GetData()))
		{
			ComputerInfo = CLocalServer::GetInstance()->ComputerList.GetComputer(pRow->GetData());

			return TRUE;
		}
	}

	return FALSE;
}

//�Ƿ��пͻ���
void CComputerListView::OnCheckClient(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		bEnable = ComputerInfo.HasClient();
	}

	pCmdUI->Enable(bEnable);
}
//�Ƿ����û�
void CComputerListView::OnCheckUser(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		bEnable = ComputerInfo.HasUser();
	}

	pCmdUI->Enable(bEnable);
}
//�Ƿ�û�пͻ���
void CComputerListView::OnCheckNotClient(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		if (ComputerInfo.GetComputerStatus() != CComputerInfo::ECS_THIEF)
		{
			bEnable = !ComputerInfo.HasClient();
		}
	}

	pCmdUI->Enable(bEnable);
}
//����Ƿ���г�ֵȨ��ͬʱ�����ߡ����𡢺ڻ���VIP�������״̬
void CComputerListView::OnCheckCredit(CCmdUI *pCmdUI)
{
	// 2011/08/04-8201-gxx: �����½��ù���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		CComputerInfo ComputerInfo;
		if (GetCurComputerInfo(ComputerInfo) && ComputerInfo.HasUser())
		{
			switch(CLocalServer::GetInstance()->ActiveMemberList.GetUserStateByNetId(ComputerInfo.GetNetId()))
			{
			case CIBAGlobal::emUserStateCenter:
			case CIBAGlobal::emUserStateNULL:
				pCmdUI->Enable(FALSE);
				return;
			}
		}
	}

	BOOL bEnable = FALSE;

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		if (theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CREDIT")))
		{
			CComputerInfo ComputerInfo;

			if (GetCurComputerInfo(ComputerInfo))
			{
				bEnable = ComputerInfo.HasUser();
			}
		}
	}

	pCmdUI->Enable(bEnable);
}
//����Ȩ�ޣ���ֵȨ��+�û����ڣ�
void CComputerListView::OnCheckPayOut(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		if (theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CREDIT")))
		{
			CComputerInfo ComputerInfo;

			if (GetCurComputerInfo(ComputerInfo))
			{
				bEnable = (ComputerInfo.GetMemberID() > 0);
			}
		}
	}

	pCmdUI->Enable(bEnable);
}
//�쳣����Ȩ��
void CComputerListView::OnCheckUnusal(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		if (theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_UNUSUAL")))
		{
			CComputerInfo ComputerInfo;

			if (GetCurComputerInfo(ComputerInfo))
			{
				bEnable = ComputerInfo.HasUser();
				//���2��״̬���ֱ������ߺ���
				if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_OFFLINE)
				{
					bEnable = TRUE;
				}
				//�һ�״̬���ܻ���
				if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_SUSPEND)
				{
					bEnable = FALSE;
				}
			}
		}
	}

	pCmdUI->Enable(bEnable);
}
//��ʾ��
void CComputerListView::OnShowColumn(UINT id)
{
	INT nIndex = id - IDM_MINCOLUMNCMD;//�к�

	if (nIndex >= 0 && nIndex < GetGridCtrl()->GetColumnCount())
	{//�����е���ʾ
		GetGridCtrl()->SetColumnVisible(nIndex, !GetGridCtrl()->GetColumnVisible(nIndex));
		GetGridCtrl()->AdjustLayout();//����
	}
}
//�е����ò˵�
void CComputerListView::OnColumnSet()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, IDM_IBASET);
}
//�Ҽ���������
void CComputerListView::OnContextMenu(CWnd*, CPoint point)
{
	m_SelectRows.RemoveAll();

	CMenu menu;

	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow != NULL)//����ѡ�е�����
	{
		CList<CBCGPGridItem*, CBCGPGridItem*> lstSelected;
		GetGridCtrl()->GetSelectedItems(lstSelected);//����ѡ�����

		POSITION pos = lstSelected.GetHeadPosition();//��һ��

		while (pos != NULL)
		{
			CBCGPGridItem* pItem = lstSelected.GetNext(pos);//��һ��			
			m_SelectRows.Add((UINT)pItem->GetParentRow()->GetData());//����ѡ�е����			
		}

		SendMessage(WM_LBUTTONUP);//����

		VERIFY(menu.LoadMenu(IDR_COMPUTER_MENU));//����
		CMenu* pPopup = menu.GetSubMenu(0);//��һ���Ӳ˵�
		ASSERT(pPopup != NULL);

#ifndef __USING_INTER_RESOURCE
		HMENU hTmp = pPopup->GetSubMenu(6)->GetSafeHmenu();
		pPopup->ModifyMenu((UINT_PTR)hTmp,MF_POPUP,(UINT_PTR)hTmp,LOAD_STRING(IDS_REMOTE_OPERATION));
#endif

		//�����˵�
		theApp.GetContextMenuManager()->ShowPopupMenu(pPopup->Detach(), point.x, point.y, this, TRUE);
	}
	else
	{
		//ɨ�����λ�ã�����������ϣ��ŵ����˲˵�
		CRect rect;
		m_pWndGridCtrl->GetClientRect(rect);
		rect.bottom = 21; //���߶ȴ��Ϊ21������
		ClientToScreen(rect);

		if (rect.PtInRect(point))//���������
		{
			VERIFY(menu.LoadMenu(IDR_MENU_COLUMN));//�����в˵�

			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup != NULL);

			for (INT i = 0; i < GetGridCtrl()->GetColumnCount(); i++)//��ö��
			{//��̬�����������йصĲ˵�
				pPopup->InsertMenu(i, MF_BYPOSITION, IDM_MINCOLUMNCMD + i, GetGridCtrl()->GetColumnName(i));
				pPopup->CheckMenuItem(i, MF_BYPOSITION | (GetGridCtrl()->GetColumnVisible(i) ? MF_CHECKED : MF_UNCHECKED));
			}

			theApp.GetContextMenuManager()->ShowPopupMenu(pPopup->Detach(), point.x, point.y, this, TRUE);
		}
	}
}

void CComputerListView::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case TimeIdOfPing : 
		{
		if (!CLocalServer::GetInstance()->LocalPing())//��������
		{
			Refresh(FALSE);
		}

		break;

		}
	case TimeIdOfSetFocus ://��ʱ���ս��㣬��Ҫ��������ֹ���ֻس�����

		KillTimer(TimeIdOfSetFocus);
		SetFocus();

		break;
	}

	CBCGPGridView::OnTimer(nIDEvent);
}

void CComputerListView::AdjustLayout()
{//���¸���λ��
	if (GetGridCtrl()->GetSafeHwnd() != NULL)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		GetGridCtrl()->SetWindowPos (NULL, 0, 0,  rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}
//����״̬��
void CComputerListView::Refresh(BOOL bRedrawAll)
{
	m_lpComputerGird->Refresh(bRedrawAll);

	CIBAString strTmp;

	if (CLocalServer::GetInstance()->IsConnected())
	{
		strTmp = MakeStatText();
	}
	else
	{
		strTmp.LoadString(IDS_DISCONNECT);
	}

	((CMainFrame*)AfxGetMainWnd())->UpdateStatusText(strTmp);
}
//����
void CComputerListView::OnSnapScreen()
{
	if (CNetBarConfig::GetInstance()->GetForbidCaptureScreen())
	{
		CDlgBossPwd dlg;
		if (dlg.DoModal() != IDOK)
			return;
	}
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();
	if (pRow != NULL) CLocalServer::GetInstance()->LocalCaptureScreen(pRow->GetData());
}

void CComputerListView::OnGetLog()
{
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();
	if (pRow != NULL) CLocalServer::GetInstance()->LocalGetLog(pRow->GetData());
}

void CComputerListView::OnPowerOff()
{
	//��ѡ�ر�
	for (INT ii = 0; ii <  GetGridCtrl()->GetRowCount(); ii++)
	{
		if (GetGridCtrl()->IsRowSelected(ii))
		{
			CBCGPGridRow* pRow = GetGridCtrl()->GetRow(ii);
			if (pRow != NULL)
			{
				CComputerInfo ComputerInfo;
				GetComputerInfoFromRow(ComputerInfo, ii);
				if (ComputerInfo.HasClient())
				{
					INT_PTR nIndex = pRow->GetData();

					CLocalServer::GetInstance()->LocalShutdownComputer(pRow->GetData(), FALSE);
				}
			}
		}
	}
}

void CComputerListView::OnPowerOn()
{
	//��ѡ����
	for (INT ii = 0; ii <  GetGridCtrl()->GetRowCount(); ii++)
	{
		if (GetGridCtrl()->IsRowSelected(ii))
		{
			CComputerInfo ComputerInfo;
			GetComputerInfoFromRow(ComputerInfo, ii);
			if (!ComputerInfo.HasClient())
			{
				CBCGPGridRow* pRow = GetGridCtrl()->GetRow(ii);
				CLocalServer::GetInstance()->LocalStartupComputer(pRow->GetData());
			}
		}
	}
}
//Ⱥ����ն���
void CComputerListView::OnSendShortMessage()
{
	CShortMsgDlg dlg;
	dlg.SetReceivers(m_SelectRows);
	dlg.DoModal();
}

void CComputerListView::OnClientShortMessage(SHORT_MESSAGE_INFO& shortMsgInfo)
{	
	//{ 2011/11/15-8210-gxx: 

	CString str = CA2T(shortMsgInfo.shortMsg.message);

	int nSpl = str.Find(_T('|'));
	if (nSpl > 0 && str.Left(nSpl).CompareNoCase(_T("p-ddnparam")) == 0)
	{
		// ����㿨����Ϣ
		CString strTmp(CA2T(shortMsgInfo.shortMsg.message));
		strTmp = strTmp.Mid(11);

		CString strTerm(CA2T(shortMsgInfo.shortMsg.termID));
		if (strTmp.Find(_T('?')) >= 0)
		{
			strTmp.AppendFormat(_T("&TermId=%s"), strTerm);
		}
		else
		{
			strTmp.AppendFormat(_T("?TermId=%s"), strTerm);
		}

		CActiveMember ActiveMember;
		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(shortMsgInfo.shortMsg.memberID,ActiveMember))
		{
			strTmp.AppendFormat(_T("&UserName=%s"), ActiveMember.GetUserName());
		}
		else
		{
			strTmp += _T("&UserName=δ֪����");
		}

		IBA_LOG(_T("�ͻ���������㿨,URL=%s"), strTmp);

		CCashierHelper::CashSellDianka(strTmp);
	}

	//}
	else if (shortMsgInfo.shortMsg.messageType !=2 ) // ��������Ϣ
	{
		//����ʽ
		CBCGPPopupWindow* pPopup = new CMsgPopupWindow; //���ﲻ��Ҫɾ������ܻ��Զ�ɾ����

		// Create indirect:
		CBCGPPopupWndParams params;//���ô��ڲ���

		CString strMsg, strTermId, strTermIP, strTmp;

		strTermId = CA2W(shortMsgInfo.shortMsg.termID);//�ն�ID

		// 2011/06/17-gxx: �򲹶���IP���ȵ���15ʱ�������ַ���������
		char IP[LEN_IPADDR+1];
		memcpy(IP, shortMsgInfo.shortMsg.computerIP, LEN_IPADDR);
		IP[LEN_IPADDR] = '\0';
		strTermIP = CA2W(IP);//�ն�IP

		strTermId.Trim();
		strTermIP.Trim();

		if (!strTermIP.IsEmpty())//IP ���ǿ� ��ʾ�ǿͻ�����Ϣ
		{
			strTmp = LOAD_STRING(IDS_CLIENTINFO);
			strMsg.Format(LOAD_STRING(IDS_FORMTERMIP), strTermId, strTermIP);//�����ն˵�URL
			params.m_nURLCmdID = (UINT)CLocalServer::GetInstance()->ComputerList.Find(strTermId, strTermIP);
			params.m_hIcon = AfxGetApp()->LoadIcon(IDR_UDO);
		}
		else
		{
			params.m_nURLCmdID = 10000;
			strTmp = LOAD_STRING(IDS_CASHIERINFO);
			params.m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		}

		//���ܿͻ�����Ϣ
		strMsg.Append(CA2W(shortMsgInfo.shortMsg.message));

		params.m_strText = strMsg;

		RECT rt = {-1, -1, -1, -1};

		if (CIBAConfig::GetInstance()->GetMsgLeftBottom())// �����½ǵ���
		{
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		}

		if (!CIBAConfig::GetInstance()->GetMsgAutoHide())
		{
			pPopup->SetAutoCloseTime(0);
		}

		pPopup->Create(NULL, params, 0, CPoint(rt.left, rt.bottom));//��������

		pPopup->SetWindowText(strTmp);//��ʾ��Ϣ

	}

	CClientShortMessage csm;
	csm.SetMemberId( shortMsgInfo.shortMsg.memberID);
	csm.SetMsgBody( (LPCTSTR)CA2T(shortMsgInfo.shortMsg.message));
	csm.SetMsgID( shortMsgInfo.shortMsg.ID);
	csm.SetMsgType( shortMsgInfo.shortMsg.messageType);
	csm.SetRevTime( (LPCTSTR)CA2T(shortMsgInfo.shortMsg.sendTime));
	csm.SetTermID( (LPCTSTR)CA2T(shortMsgInfo.shortMsg.termID));
	csm.SetTermIP( (LPCTSTR)CA2T(shortMsgInfo.shortMsg.computerIP));

	if (csm.GetMsgType() == 2)
	{
		if (CNetBarConfig::GetInstance()->GetReminderBalance())
		{
			static DWORD nTimeCount = GetTickCount() - 10000;
			if (GetTickCount()-nTimeCount > 10000)// ��ֹ��10����������
			{
				NS_TTS::CIBATTS::GetInstance()->SpeakReminderBalance(csm.GetTermID());
				nTimeCount = GetTickCount();
			}
			
		}
		csm.SetMsgBody(LOAD_STRING(IDS_BALANCE_REMINDER)); // ����
	}

	//д����Ϣ�Ի�����
	//((CMainFrame*)AfxGetMainWnd())->m_wndOutput.UpdateMsgLog();
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.AddMsgLog(csm);
}

void CComputerListView::OnCreditOnline()
{
	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		CString strTmp = ComputerInfo.GetNetId();

		if (!strTmp.IsEmpty() && (ComputerInfo.HasUser() 
			|| ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_OFFLINE))
		{	
			CNDCreditDlg dlg;
			dlg.SetNetId(strTmp);
			dlg.DoModal();
		}
	}
}

CString CComputerListView::MakeStatText()
{//ȡ��ص�ͳ������
	CComputerList::CStatInfo StatInfo;
	CLocalServer::GetInstance()->ComputerList.GetStatInfo(StatInfo);

	CString strRet;

	UINT nTmp = StatInfo.GetAllCount();//�û�������Ŀ�������쳣�û�

	if (StatInfo.GetAllCount() == 0)
	{
		nTmp = 1;//��ֹ���ֳ���Ϊ0
	}

	strRet.Format(LOAD_STRING(IDS_STATTEXT), StatInfo.GetAllCount(), 
		StatInfo.GetOnline(), StatInfo.GetOnline() * 100.0 / nTmp,//�ٷ���
		StatInfo.GetLock(), StatInfo.GetLock() * 100.0 / nTmp,
		StatInfo.GetUnLock(), StatInfo.GetUnLock() * 100.0 / nTmp,
		StatInfo.GetThiefCount(),StatInfo.GetThiefCount() * 100.0 / nTmp,
		StatInfo.GetCommonUserCount(), StatInfo.GetCommonUserCount() * 100.0 / nTmp,
		StatInfo.GetMemberUserCount(), StatInfo.GetMemberUserCount() * 100.0 / nTmp,
		StatInfo.GetBundTimePayCount(), StatInfo.GetBundTimePayCount() * 100.0 / nTmp
		);

	return strRet;
}

void CComputerListView::OnPayout()
{
	CComputerInfo ComputerInfo;

	if (!GetCurComputerInfo(ComputerInfo))
	{
		return;
	}

	CActiveMember ActiveMember;

	if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(ComputerInfo.GetMemberID(), ActiveMember))
	{
		return;
	}

	CNDReturnDlg dlg;
	dlg.SetNetId(ActiveMember.GetNetId());
	dlg.DoModal();
}

//����
void CComputerListView::OnChangeComputer()
{	////////////////��ѯ�Ƿ����û�////////////
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow == NULL) return;

	INT_PTR nIndex = pRow->GetData();

	if (CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(nIndex)) return;

	CComputerInfo &ComputerInfo = CLocalServer::GetInstance()->ComputerList.GetComputer(pRow->GetData());

	CNDCheckInDlg dlg;
	dlg.SetCheckIn(FALSE);
	dlg.SetNetId(ComputerInfo.GetNetId());
	dlg.DoModal();

}
 
void CComputerListView::OnReboot()
{//��������
	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		for (INT ii = 0; ii <  GetGridCtrl()->GetRowCount(); ii++)
		{
			if (GetGridCtrl()->IsRowSelected(ii))
			{
				CComputerInfo ComputerInfo;
				GetComputerInfoFromRow(ComputerInfo, ii);
				if (ComputerInfo.HasClient())
				{
					CBCGPGridRow* pRow = GetGridCtrl()->GetRow(ii);
					CLocalServer::GetInstance()->LocalShutdownComputer(pRow->GetData(), TRUE);
				}
			}
		}
	}
}

void CComputerListView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBCGPGridView::OnLButtonUp(nFlags, point);

	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTaskPane.ShowUserInfo(ComputerInfo);
	}
}

void CComputerListView::OnUpdateColumnSet(CCmdUI *pCmdUI)
{
	// 2011/08/04-8201-gxx: �����½��ù���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	BOOL bRet = theApp.GetCurCashier()->IsOnDuty(); 

	if (bRet)
	{
		bRet = theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CONFIG"));
	}

	pCmdUI->Enable(bRet);
}

void CComputerListView::OnRefresh()
{
	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		//ȡϵͳʱ��
		CGetTimeMethod GetTimeMethod;
		GetTimeMethod.Execute();

		//�Ƕ���ʱʹ��
		if (GetTimeMethod.GetStatusCode() == -1)
		{
			return;
		}
		else if (GetTimeMethod.GetStatusCode() == 0)
		{
			GetTimeMethod.SyncLocalTime();
		}
	}

	CLocalServer::GetInstance()->LocalGetComputerList();
}

void CComputerListView::OnUpdateConsumelist(CCmdUI *pCmdUI)
{
	// 2011/08/04-8201-gxx: �����½��ù���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	BOOL bEnable = FALSE;

	if (theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CONSUMELIST")))
	{
		CComputerInfo ComputerInfo;

		if (GetCurComputerInfo(ComputerInfo))
		{
			bEnable = ComputerInfo.HasUserInfo();
		}
	}

	pCmdUI->Enable(bEnable);
}

void CComputerListView::OnConsumelist()
{
	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		CActiveMember ActiveMember;

		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(ComputerInfo.GetMemberID(), ActiveMember))
		{
			COperationInfo OperationInfo;
			OperationInfo.SetMemberId(ActiveMember.GetMemberID());
			OperationInfo.SetUserName(ActiveMember.GetUserName());
			OperationInfo.SetNetId(ActiveMember.GetNetId());
			OperationInfo.SetOperationTime(ActiveMember.GetActivationTime());
			OperationInfo.SetIsMember(ActiveMember.IsMember());

			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 2, (LPARAM)&OperationInfo);
		}
		
		//theApp.GetIBAView()->QueryUserComsumeList(ComputerInfo.GetNetId());
	}
}

void CComputerListView::OnUpdateTermConsume(CCmdUI *pCmdUI)
{
	// 2011/08/04-8201-gxx: �����½��ù���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_TERMCONSUME")));
}

void CComputerListView::OnTermConsume()
{
	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		theApp.GetIBAView()->QueryTermConsumeList(ComputerInfo.GetTerminalID());
	}
}

void CComputerListView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CBCGPGridView::OnLButtonDblClk(nFlags, point);

	if (theApp.GetCurCashier()->IsOnDuty() && theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CREDIT")))
	{
		CComputerInfo ComputerInfo;

		if (GetCurComputerInfo(ComputerInfo))
		{
			CString strTmp = ComputerInfo.GetNetId();

			if (!strTmp.IsEmpty() && (ComputerInfo.HasUser() 
				|| ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_OFFLINE))
			{	
				CNDReturnDlg dlg;
				dlg.SetNetId(strTmp);
				dlg.DoModal();
			}
		}
	}
}

BOOL CComputerListView::FindComputer(UINT nKeyType, LPCTSTR lpszKeyValue)
{
	INT_PTR nIndex = m_lpComputerGird->GetComputerList()->LookFor(nKeyType, lpszKeyValue);

	if (nIndex < 0)
	{
		return FALSE;
	}

	CBCGPGridRow* pRow = m_lpComputerGird->FindRowByData(nIndex);

	if (pRow == NULL)
	{
		return FALSE;
	}

	m_lpComputerGird->EnsureVisible(pRow);
	m_lpComputerGird->SetCurSel(pRow);

	//���ﲻ��ֱ�����ý��㣬Ҫ������ʱ�Ļ��ƣ����������س��¼���ԭ��δ֪
	SetTimer(TimeIdOfSetFocus, 200, NULL);

	return TRUE;
}

void CComputerListView::OnBundtimeonline()
{
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow != NULL) 
	{
		if (!CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(pRow->GetData()))
		{
			CComputerInfo ComputerInfo = CLocalServer::GetInstance()->ComputerList.GetComputer(pRow->GetData());
			
			CDlgBundleTime BundleTimeDlg;
			BundleTimeDlg.SetNetId(ComputerInfo.GetNetId(), TRUE);
			BundleTimeDlg.DoModal();
		}
	}
}

void CComputerListView::OnUpdateBundtimeonline(CCmdUI *pCmdUI)
{
	// 2011/08/04-8201-gxx: �����½��ù���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�ϰ�����
	BOOL bRet = theApp.GetCurCashier()->IsOnDuty(); 
	//����̨Ȩ�޿���
	bRet = bRet && theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_BUNDLETIME"));
	//�û�״̬����
	CComputerInfo ComputerInfo;
	GetCurComputerInfo(ComputerInfo);
	//ֻ�����գ��ڻ���VIP���һ������࣬����״̬����
	bRet = bRet && ComputerInfo.HasUser();

	pCmdUI->Enable(bRet);
}

void CComputerListView::OnUpdateSendShortMessage(CCmdUI *pCmdUI)
{
	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_THIEF)
		{
			pCmdUI->Enable(FALSE);
		}
	}
}

void CComputerListView::OnRetailonline()
{
	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_ONLINE)
		{
			CJXCMainDlg::RetailOnline(ComputerInfo.GetTerminalID(), ComputerInfo.GetNetId());
		}
	}
}

void CComputerListView::OnUpdateRetailonline(CCmdUI *pCmdUI)
{
	// 2011/08/04-8201-gxx: �����½��ù���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(FALSE);

	CComputerInfo ComputerInfo;

	if (GetCurComputerInfo(ComputerInfo))
	{
		if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_ONLINE)
		{
			pCmdUI->Enable(TRUE);
		}
	}
}

void CComputerListView::OnPing()
{
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow != NULL) 
	{
		if (!CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(pRow->GetData()))
		{
			CComputerInfo ComputerInfo;

			if (GetCurComputerInfo(ComputerInfo))
			{
				if (ComputerInfo.HasUser()) //���û����ϱ����
				{
					CLocalServer::GetInstance()->LocalForceUpdateData(pRow->GetData());

					return ;
				}
			}

			CLocalServer::GetInstance()->LocalPingClient(pRow->GetData());
		}
	}
}

void CComputerListView::OnUnlockScreen()
{
	CUnlockScreenDlg dlg;
	
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	for (INT i = 0; i < GetGridCtrl()->GetRowCount(); i++)
	{
		if (!GetGridCtrl()->IsRowSelected(i))
		{
			continue;
		}

		CBCGPGridRow* pRow = GetGridCtrl()->GetRow(i);

		if (!CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(pRow->GetData()))
		{
			CComputerInfo& ComputerInfo = CLocalServer::GetInstance()->ComputerList.GetComputer(pRow->GetData());

			if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
			{
				CLocalServer::GetInstance()->LocalUnlockScreen(pRow->GetData(), dlg.GetPwd());
			}
		}
	}
}

void CComputerListView::OnUpdateUnlockScreen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
	
	if(2 != CNetBarConfig::GetInstance()->GetDisallowUnlock())
	{
		CComputerInfo ComputerInfo;

		if (GetCurComputerInfo(ComputerInfo))
		{
			if (ComputerInfo.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
			{
				pCmdUI->Enable(TRUE);
			}
		}
	}	
}

INT CComputerListView::OnDataArrival(INT nAppId, INT nMsg, INT nParam1, INT nParam2, LPVOID lpBuf, size_t cbLen)
{
	if (nMsg == 1)
	{
		// ����㿨�ɹ���
		UINT nMemberID = (UINT)nParam1;
		char chInfo[1024] = {0};
		strncpy(chInfo,(const char*)lpBuf,cbLen);
		CString strInfo = CA2T(chInfo);

		CLocalServer::GetInstance()->LocalSendShortMessageByMemberID(nMemberID, strInfo);

		IBA_LOG(_T("���͵㿨���۳ɹ�����Ϣ���ͻ���,MemberID=%d,msg=%s"),nMemberID,strInfo);

	}
	return 0;
}

void CComputerListView::SetCurSel(int nIndex)
{
	if(m_lpComputerGird)
	{
		m_lpComputerGird->SetCurSel(-1);
	}
}

void CComputerListView::SetNewUserDefaultTerm(LPCTSTR szTermID)
{
	m_strDefaultTermID = szTermID;
}

LRESULT CComputerListView::OnCompRefresh( WPARAM wParam, LPARAM lParam )
{
	Refresh(TRUE);
	return 0;
}
