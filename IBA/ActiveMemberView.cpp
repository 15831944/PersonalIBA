// ActiveMemberView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "MainFrm.h"
#include "Cashier.h"
#include "CreditDlg.h"
#include "NDCheckInDlg.h"
#include "NDReturnDlg.h"
#include "NDCreditDlg.h"
#include "BundleTimeDlg.h"
#include ".\JXC\JXCMainDlg.h"
#include "ComputerListView.h"
#include "Socket\LocalServer.h"
#include "ActiveMemberGrid.h"
#include ".\ActiveMemberView.h"
#include "ShortMsgDlg.h"

#include "DlgInputParam.h"
#include "IBAHelpper.h"
#include "CreditReverseDlg.h"
#include "NetbarRoom.h"
#include "Dal\RightsManagement.h"
#include "RealName\RealNameBase.h"
#include "RealName\IBARealName.h"

static const UINT TIMER_KILL_PROCESS = 101;

// CActiveMemberView

IMPLEMENT_DYNCREATE(CActiveMemberView, CBCGPGridView)

CActiveMemberView::CActiveMemberView()
{
	CLocalServer::GetInstance()->SetActiveMemberView(this);
}

CActiveMemberView::~CActiveMemberView()
{
}

BEGIN_MESSAGE_MAP(CActiveMemberView, CBCGPGridView)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_CREDITONLINE, OnCredit)
	ON_COMMAND(IDM_PAYOUT, OnPayout)
	ON_COMMAND(IDM_CONSUMELIST2, OnConsumelist2)
	ON_UPDATE_COMMAND_UI(IDM_CONSUMELIST2, OnUpdateConsumelist2)
	ON_COMMAND(IDM_MYREFRESH, OnRefresh)
	ON_COMMAND(IDM_BUNDTIMEONLINE, OnBundtimeonline)
	ON_COMMAND(IDM_RETAILONLINE, OnRetailonline)
	ON_COMMAND(IDM_CHECKIN, OnCheckin)
	ON_COMMAND(IDM_CHANGECOMPUTER, OnChangeComputer)
	ON_COMMAND(IDM_SENDSHORTMESSAGE, OnSendShortMessage)
	ON_COMMAND(ID_ACTIVE_REMARK, OnModifyRemark)
	ON_COMMAND(ID_CREDITMODIFY, OnCreditModify)
	ON_COMMAND(ID_RE_BILLING, OnRebilling)
	ON_UPDATE_COMMAND_UI(IDM_CREDITONLINE, OnUpdateCredit)
	ON_UPDATE_COMMAND_UI(IDM_BUNDTIMEONLINE, OnUpdateBundtimeonline)
	ON_UPDATE_COMMAND_UI(IDM_PAYOUT, OnUpdatePayout)
	ON_UPDATE_COMMAND_UI(IDM_CHANGECOMPUTER, OnUpdateChangeComputer)
	ON_UPDATE_COMMAND_UI(IDM_RETAILONLINE, OnUpdateRetailonline)
	ON_UPDATE_COMMAND_UI(IDM_CHECKIN, OnUpdateCheckin)
	ON_UPDATE_COMMAND_UI(ID_CREDITMODIFY, OnUpdateCreditReverse)
	ON_UPDATE_COMMAND_UI(ID_RE_BILLING, OnUpdateRebilling)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CActiveMemberView ��Ϣ�������

int CActiveMemberView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pWndGridCtrl = new CActiveMemberGrid();

	//�����ؼ�,ʵ���ϣ�����GRIDView�ڲ����еĲ���
	if (!m_pWndGridCtrl->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))//����
	{
		return -1;
	}

	SetTimer(TIMER_KILL_PROCESS,3000,NULL);

	return 0;
}

void CActiveMemberView::Refresh(BOOL bRedrawAll)
{
	IBA_TRACE("CActiveMemberView::Refresh(BOOL bRedrawAll)");

	((CActiveMemberGrid*)m_pWndGridCtrl)->Refresh(bRedrawAll);
}

void CActiveMemberView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_ACTIVEMEMBER_MENU));//����
	
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow != NULL)//����ѡ�е�����
	{
		SendMessage(WM_LBUTTONUP);

		CMenu* pPopup = menu.GetSubMenu(0);//��һ���Ӳ˵�

		if (pPopup)
		{
			// 2012/03/21-8238-gxx: 
			if (CNetBarConfig::GetInstance()->GetRealNamePassword() == 4 )
			{
				pPopup->RemoveMenu(IDM_CHECKIN, MF_BYCOMMAND);
			}

			theApp.GetContextMenuManager()->ShowPopupMenu(pPopup->Detach(), point.x, point.y, this, TRUE);
		}

		
	}
	else
	{
		CMenu* pPopup = menu.GetSubMenu(1);//��2���Ӳ˵�
		theApp.GetContextMenuManager()->ShowPopupMenu(pPopup->Detach(), point.x, point.y, this, TRUE);
	}
}

void CActiveMemberView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBCGPGridView::OnLButtonUp(nFlags, point);

	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTaskPane.ShowUserInfo(ActiveMember);
	/*	CCurrentNetBarUser::GetInstance()->m_strNetId = ActiveMember.GetNetId();*/
		CCurrentNetBarUser::GetInstance()->SetNetId(ActiveMember.GetNetId());
	}
	else
	{
		//CCurrentNetBarUser::GetInstance()->m_strNetId.Empty();
		CCurrentNetBarUser::GetInstance()->SetNetId(_T(""));
	}
}

void CActiveMemberView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CBCGPGridView::OnLButtonDblClk(nFlags, point);

	if (!theApp.GetCurCashier()->IsOnDuty()) return;

	CActiveMember ActiveMember;

	if (!GetCurActiveMember(ActiveMember)) return;

	// 2015-0828 liyajun
	//if (ActiveMember.GetIsLocalUser() || CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		switch(CIBAConfig::GetInstance()->GetOperationAfterDClick())
		{
		case CIBAGlobal::emOperationAfterCredit:
			{
				CNDCreditDlg dlg;
				dlg.SetNetId(ActiveMember.GetNetId());
				dlg.DoModal();
			}
			break;
		case CIBAGlobal::emOperationAfterRefund:
			{
				//2015-0828 liyajun
				//CNDReturnDlg dlg;
				//dlg.SetNetId(ActiveMember.GetNetId());
				//dlg.DoModal();
				this->OnPayout();
			}
			break;
		default:
			break;
		}	
		return;	
	}

	if (CCurrentNetBarUser::GetInstance()->QueryBalance(ActiveMember.GetNetId()))
	{
		switch(CIBAConfig::GetInstance()->GetOperationAfterDClick())
		{
		case CIBAGlobal::emOperationAfterCredit:
			{
				CNDCreditDlg dlg;
				dlg.DoModal();
			}
			break;
		case CIBAGlobal::emOperationAfterRefund:
			{
				// 2011/10/31-8210-gxx: 
				if (ActiveMember.GetMemberID() == ActiveMember.GetRoomMemberId())
				{
					// �������˺ţ��Զ����������������
					CNetbarRoomDlg dlg;
					dlg.SetNetId(ActiveMember.GetNetId(),TRUE);
					dlg.DoModal();
				}
				else
				{
					CNDReturnDlg dlg;
					dlg.DoModal();
				}
				
			}
			break;
		default:
			break;
		}	
		return;	

		//CNDReturnDlg dlg;
		////dlg.SetNetId(ActiveMember.GetNetId());
		//dlg.DoModal();
	}
	else
	{
		//�û��Ѿ�ע����
		if (CCurrentNetBarUser::GetInstance()->GetMemberId() == 0
			&& CCurrentNetBarUser::GetInstance()->GetLastStatusCode() >= 0)
		{
			// 2014-10-28 - qsc
			if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
			{
				NS_REALNAME::CheckoutInfo coi;
				ZeroMemory(&coi, sizeof(coi));
				_tcsncpy(coi.szName, ActiveMember.GetUserName(), 100);
				_tcsncpy(coi.szNetId, ActiveMember.GetNetId(), 100);
				_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

				CString strIP;
				CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
				_tcsncpy(coi.szIP, strIP, 50);

				if(ActiveMember.IsMember())
				{
					coi.nCheckoutType = CIBAGlobal::cot_TempLogout;
				}
				else
				{
					coi.nCheckoutType = CIBAGlobal::cot_MemLogout;
				}


				theApp.GetRealName()->CheckOut(&coi);
			}

			((CActiveMemberGrid*)m_pWndGridCtrl)->GetActiveMemberList()->RemoveActiveMember(ActiveMember.GetMemberID());
			Refresh(TRUE);

			CIBAString strInfo;
			strInfo.LoadString(IDS_HASCHECKOUT);
			CString str;
			str.Format(strInfo, ActiveMember.GetNetId());
			theApp.IBAMsgBox(str);

			CIBALog::GetInstance()->Write(str);

			// %s ��ע��
		}
	}
}

/*************************************************
Function:       // GetCurActiveMember
Description:    // ��ActiveMemberMap�в��ҵ�ǰѡ���е��û�����.
Calls:          // GetActiveMember(nMemberId, ActiveMember)
Output:         // �ҵ��򱣴���ActiveMember��
Return:         // �ҵ�����TRUE�����򷵻�FALSE
Others:         // ������ͨ����ǰѡ���е�MemberId. MemberId�Ǻ�ʱ�����浽Data�еģ�
*************************************************/

BOOL CActiveMemberView::GetCurActiveMember(CActiveMember& ActiveMember)
{
	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();

	if (pRow != NULL)
	{
		UINT nMemberId = pRow->GetData();

		return ((CActiveMemberGrid*)m_pWndGridCtrl)->GetActiveMemberList()->GetActiveMember(nMemberId, ActiveMember);
	}

	return FALSE;
}

void CActiveMemberView::OnCredit()
{
	//CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);

	//CActiveMember ActiveMember;

	//if (GetCurActiveMember(ActiveMember))
	//{
	//	// ����״̬�£������������û���ֵ
	//	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken && ActiveMember.GetIsLocalUser() == FALSE)
	//	{
	//		theApp.IBAMsgBox(IDS_FORBID_CREDIT,MB_OK|MB_ICONEXCLAMATION); 
	//		return;
	//	}
	//
	//	CNDCreditDlg dlg;
	//	dlg.SetNetId(ActiveMember.GetNetId());
	//	dlg.DoModal();
	//}
	CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);

	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		// ����״̬�£������������û���ֵ
		//if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken && ActiveMember.GetIsLocalUser() == FALSE)
		//{
		//	theApp.IBAMsgBox(IDS_FORBID_CREDIT,MB_OK|MB_ICONEXCLAMATION); 
		//	return;
		//}

		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal) 
		{
			CNDCreditDlg dlg;
			dlg.SetNetId(ActiveMember.GetNetId());
			dlg.DoModal();
		}
		else 
		{
			CNDCreditDlg dlg;
			//dlg.SetLocalActiveMember(ActiveMember);
			dlg.SetNetId(ActiveMember.GetNetId());
			//2015-0907 liyajun �ظ�UpdateActiveMember�ˣ�ɾ��
			//if (dlg.DoModal() == IDOK) {
			//	CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember.GetMemberID(), dlg.m_nCreditMoney, 0, 0);
			//}
			dlg.DoModal();

		}

	}
}

void CActiveMemberView::OnUpdateCredit(CCmdUI *pCmdUI)
{
	BOOL bEnable = theApp.GetCurCashier()->IsOnDuty();
	
	pCmdUI->Enable(bEnable);
}

/*************************************************
Function:       // OnPayout
Description:    // �û���ͼ���"����"����鵱ǰ�����û���Ϣ��
				// �����û� - Popup NetbarRoomDlg
				// �������� - Popup NDReturnDlg
				// ����ģʽ - Set localActiveMember, Popup NDReturnDlg 
Calls:          // DoModal
Table Accessed: // None
Table Updated:  // None
Input:          // None
Output:         // None
Return:         // None
Others:         // None
*************************************************/
void CActiveMemberView::OnPayout()
{
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember)) //��ȡ��ǰ�����û���Ϣ
	{
		// 2011/10/31-8210-gxx: 
		if (ActiveMember.GetMemberID() == ActiveMember.GetRoomMemberId()) //�����û�������������
		{
			// �������˺ţ��Զ����������������
			CNetbarRoomDlg dlg;
			dlg.SetNetId(ActiveMember.GetNetId(),TRUE);
			dlg.DoModal();
		}
		else
		{
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal) //�������������˿����
			{
				CNDReturnDlg dlg;
				dlg.SetNetId(ActiveMember.GetNetId());
				dlg.DoModal();
			}
			else                                                          //����ģʽ���ñ����û������˿����
			{
				CNDReturnDlg dlg;
				//dlg.SetLocalActiveMember(ActiveMember);
				dlg.SetNetId(ActiveMember.GetNetId());
				dlg.DoModal();
			}
		}
	}
}

void CActiveMemberView::OnConsumelist2()
{
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		COperationInfo OperationInfo;
		OperationInfo.SetMemberId(ActiveMember.GetMemberID());
		OperationInfo.SetUserName(ActiveMember.GetUserName());
		OperationInfo.SetNetId(ActiveMember.GetNetId());
		OperationInfo.SetOperationTime(ActiveMember.GetActivationTime());
		OperationInfo.SetIsMember(ActiveMember.IsMember());
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 2, (LPARAM)&OperationInfo);
	}
}

void CActiveMemberView::OnRefresh()
{
	((CActiveMemberGrid*)m_pWndGridCtrl)->GetActiveMemberList()->LoadFromDB();

	Refresh(TRUE);
}

void CActiveMemberView::OnBundtimeonline()
{
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		// ����״̬�£��������ʱ
		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken && ActiveMember.GetIsLocalUser() == FALSE)
		{
			theApp.IBAMsgBox(IDS_FORBID_BUNDTIME,MB_OK|MB_ICONEXCLAMATION); 
			return;
		}
		CDlgBundleTime BundleTimeDlg;
		BundleTimeDlg.SetNetId(ActiveMember.GetNetId(), TRUE);
		BundleTimeDlg.DoModal();
	}
}

void CActiveMemberView::OnUpdateBundtimeonline(CCmdUI *pCmdUI)
{
	OnUpdateCredit(pCmdUI);
}

void CActiveMemberView::OnUpdatePayout(CCmdUI *pCmdUI)
{
	BOOL bEnable = theApp.GetCurCashier()->IsOnDuty();
	pCmdUI->Enable(bEnable);
}

void CActiveMemberView::OnDestroy()
{
	((CActiveMemberGrid*)m_pWndGridCtrl)->GetActiveMemberList()->Terminate();

	CBCGPGridView::OnDestroy();
}

void CActiveMemberView::OnChangeComputer()
{
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		CNDCheckInDlg dlg;
		dlg.SetCheckIn(FALSE);
		dlg.SetNetId(ActiveMember.GetNetId());
		dlg.DoModal();
	}
}

void CActiveMemberView::OnUpdateChangeComputer(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);

	//����ģʽ��������
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal)
	{
		return;
	}

	if (!theApp.GetCurCashier()->IsOnDuty())
	{
		return;
	}

	if (!theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_UNUSUAL")))
	{
		return;
	}

	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		pCmdUI->Enable(!ActiveMember.GetTerminalID().IsEmpty());
	}
}


void CActiveMemberView::OnRetailonline()
{
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		if (ActiveMember.GetIsOnLine())
		{
			if (ActiveMember.GetIsLocalUser())
			{
				theApp.IBAMsgBox(IDS_FORBID_LOCAL_SOLD,MB_OK|MB_ICONEXCLAMATION);
				return;
			}
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
			{
				theApp.IBAMsgBox(IDS_FORBID_SOLD,MB_OK|MB_ICONEXCLAMATION);
				return;
			}
			CJXCMainDlg::RetailOnline(ActiveMember.GetTerminalID(), ActiveMember.GetNetId());
		}
	}
}

void CActiveMemberView::OnCheckin()
{
	// ����״̬�£������������û��ϻ�
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		theApp.IBAMsgBox(IDS_FORBID_CHECKIN,MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	CActiveMember ActiveMember;
	if (GetCurActiveMember(ActiveMember))
	{
		CNDCheckInDlg dlg;
		dlg.SetNetId(ActiveMember.GetNetId());
		dlg.DoModal();
	}
}

void CActiveMemberView::OnUpdateCheckin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);

	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		BOOL bEnable = ActiveMember.GetTerminalID().IsEmpty();
		if (bEnable && !CNetBarConfig::GetInstance()->GetUserRCheckIn())
		{
			pCmdUI->Enable(TRUE);
		}
		
	}
}

void CActiveMemberView::PayoutMember(CActiveMember &ActiveMember)
{
	if (!CCurrentNetBarUser::GetInstance()->QueryBalance(ActiveMember.GetNetId())) return;

	CString strEndTime = CCurrentNetBarUser::GetInstance()->Balance.GetEndTime();

	CString strTmp;

	strTmp.Format(strEndTime.IsEmpty() ? IDS_PAYOUT2 : IDS_PAYOUT3, ActiveMember.GetNetId());

	if (theApp.IBAMsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION) != IDOK) return;

	CWaitCursor WaitCursor;

	INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(ActiveMember.GetMemberID());

	if (nIndex >= 0) //Զ���»�
	{
		CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, ActiveMember.GetMemberID());
		Sleep(500);

		CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
		computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
		computer.ClearUserInfo();
		CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);

		CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
	}

	CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(ActiveMember.GetMemberID());

	CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);	//ˢ��

	// ���Ļ��Զ�ȡ������
	/*CActivationMethod Activation;
	Activation.SetMemberId(ActiveMember.GetMemberID());
	Activation.SetMode(CActivationMethod::ACTIVATION_NO );
	theApp.GetCurCashier()->DoActivation(Activation);*/

	CCheckout2Method Checkout2;
	Checkout2.SetSafeLogout(0); //�۵���ǰʱ��
	Checkout2.SetMemberId(ActiveMember.GetMemberID());
	theApp.GetCurCashier()->DoCheckout2(Checkout2);	

	COperationInfo OperationInfo;
	OperationInfo.SetOperationType(6);
	OperationInfo.SetNetId(ActiveMember.GetNetId());
	OperationInfo.SetUserName(ActiveMember.GetUserName());
	OperationInfo.SetDescription(ActiveMember.GetTerminalID());
	
	// 2014-10-10 - qsc
	if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
	{
		NS_REALNAME::CheckoutInfo coi;
		ZeroMemory(&coi, sizeof(coi));
		_tcsncpy(coi.szName, ActiveMember.GetUserName(), 100);
		_tcsncpy(coi.szNetId, ActiveMember.GetNetId(), 100);
		_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

		CString strIP;
		CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
		_tcsncpy(coi.szIP, strIP, 50);
		coi.nCheckoutType = CIBAGlobal::cot_MemLogout;

		theApp.GetRealName()->CheckOut(&coi);
	}

	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

	/*strTmp.Format(IDS_PAYOUTMEMBER, ActiveMember.GetNetId());

	theApp.IBAMsgBox(strTmp, MB_ICONINFORMATION);*/
}

/*************************************************
Function:       // FindUserAt
Description:    // ���û���ͼ�б��в��Ҷ�ӦstrKeyword(�����˺�)���û�������������.
Calls:          // None
Table Accessed: // None
Table Updated:  // None
Input:          // nKeyType - 0 -��ʾƥ��ID����󼸸��ַ���1 - ��ʾ����ȫ�ַ�ƥ��
                // strKeyword - ��ƥ����˺�
Output:         // None
Return:         // ƥ����û���������
Others:         // None
*************************************************/
int CActiveMemberView::FindUserAt(UINT nKeyType,CString strKeyword)
{
	switch( nKeyType )
	{
	case 0:  // ƥ��ID����󼸸��ַ�
		{
			int nRCount = m_pWndGridCtrl->GetRowCount();
			int nLeft = 0;
			int nRight = strKeyword.GetLength();
			//�����û���ͼ�е��û���Ѱ����termId�����λ�Ƿ���strKeywordһ�£�һ���򷵻������б��е�������
			for(int i = 0 ; i < nRCount ; i++ )
			{
				CString strID = m_pWndGridCtrl->GetRow(i)->GetItem(0)->GetLabel();

				nLeft = strID.GetLength();

				if( nLeft < nRight )
				{
					continue;
				}

				if( strID.Right(nRight).CompareNoCase(strKeyword) == 0 )
				{
					return i;
				}

			}
		}
		break;
	case 1:  // ƥ������ID
		{
			int nRCount = m_pWndGridCtrl->GetRowCount();
			for(int i = 0 ; i < nRCount ; i++ )
			{
				CString strID = m_pWndGridCtrl->GetRow(i)->GetItem(0)->GetLabel();

				if( strID.CompareNoCase(strKeyword) == 0 )
				{
					return i;
				}

			}
		}
		break;
	default:
		{
			IBA_LOG(_T("Abnormal.int CActiveMemberView::FindUserAt:δָ���Ĳ�������"));
			GXX_ASSERT(0,"δָ���Ĳ�������");
			return -1;
		}
	}

	return -1;
}

/*************************************************
Function:       // SetSelUser
Description:    // �����û���ͼ�ĵ�ǰѡ���У���Map�в��Ҷ�Ӧ�����ҵ�������������ʾ���û���Ϣ��������CCurrentNetBarUser��NetId
Calls:          // SetCurSel, ShowUserInfo, CCurrentNetBarUser::GetInstance()->SetNetId
Table Accessed: // None
Table Updated:  // None
Input:          // nIndex - �û���ͼ�б��������
                // bSetFocus - �Ƿ񽫽������õ�Grid�ؼ���
Output:         // None
Return:         // None
Others:         // None
*************************************************/

void CActiveMemberView::SetSelUser(int nIndex, BOOL bSetFocus)
{
	if( nIndex < 0 || nIndex >= m_pWndGridCtrl->GetRowCount() )
	{
		//GXX_ASSERT(0,"��Чλ��"); 
		return ;

	}
	m_pWndGridCtrl->SetCurSel( m_pWndGridCtrl->GetRow(nIndex) );

	// ��ʾ�û���Ϣ
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTaskPane.ShowUserInfo(ActiveMember);  //��������ʾ�û���Ϣ
		CCurrentNetBarUser::GetInstance()->SetNetId(ActiveMember.SetNetId());      //����CCurrentNetBarUser��NetId
	}

	if (bSetFocus)
	{
		m_pWndGridCtrl->SetFocus();
	}
	
}

CActiveMemberList* CActiveMemberView::GetActiveMemberList()
{
	return &(CLocalServer::GetInstance()->ActiveMemberList);
}
	
void CActiveMemberView::OnTimer(UINT nIDEvent)
{
	CBCGPGridView::OnTimer(nIDEvent);

	if (nIDEvent == TIMER_KILL_PROCESS )
	{
		CString strKillCaptions;
    	strKillCaptions = CNetBarConfig::GetInstance()->GetKillCaptions();

		CStringArray strArray;
		CIBAHelpper::SplitLine(strKillCaptions,strArray);
		for (int i=0; i<strArray.GetCount(); i++)
		{
			if (strArray.GetAt(i).IsEmpty())
			{
				continue;
			}
			HWND hWnd = ::FindWindow(NULL,strArray.GetAt(i).GetString());
			while(hWnd)
			{
				::PostMessage(hWnd,WM_CLOSE,0,0);
				hWnd = ::FindWindow(NULL,strArray.GetAt(i).GetString());
			}
		}
	}
}
void CActiveMemberView::OnRebilling()
{
	CIBAString str;
	str.LoadString(IDS_ENSURE_REBILLING);
	if (theApp.IBAMsgBox(str,MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)
	{
		return;
	}
	
	CList <CBCGPGridItem*,CBCGPGridItem*> arrMember;
	GetGridCtrl()->GetSelectedItems(arrMember);

	if( arrMember.GetCount() > 0)
	{
		CShortMsgDlg dlg;
		int nValid = 0;

		POSITION pos = arrMember.GetHeadPosition();
		while(pos!=NULL)
		{
			const CBCGPGridItem* &pItem = arrMember.GetNext(pos);
			CBCGPGridRow *pRow = pItem->GetParentRow();


			UINT nMemberId = pRow->GetData();
			INT_PTR nIn = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);
			if ( nIn!=-1 )
			{
				CActiveMember Act;
				if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,Act))
				{
					// ���͸�ָ���ͻ���
					CLocalServer::GetInstance()->LocalCheckOutUser(nIn, nMemberId, 3);
					Sleep(500);
					CIBALog::GetInstance()->WriteFormat(_T("�ͻ������¼Ʒ�->PC=%s, memberId=%d"),Act.GetTerminalID(),nMemberId);

					Act.SetActivationTime(COleDateTime::GetCurrentTime());
					CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(Act,TRUE);
					Refresh(FALSE);
				}
				
			}


		}
	}
}

void CActiveMemberView::OnUpdateRebilling(CCmdUI *pCmdUI)
{
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CList <CBCGPGridItem*,CBCGPGridItem*> arrMember;
	GetGridCtrl()->GetSelectedItems(arrMember);

	pCmdUI->Enable(arrMember.GetCount() > 0);
}


void CActiveMemberView::OnSendShortMessage()
{
	CList <CBCGPGridItem*,CBCGPGridItem*> arrMember;
	GetGridCtrl()->GetSelectedItems(arrMember);

	if( arrMember.GetCount() > 0)
	{
		CShortMsgDlg dlg;
		int nValid = 0;

		POSITION pos = arrMember.GetHeadPosition();
		while(pos!=NULL)
		{
			const CBCGPGridItem* &pItem = arrMember.GetNext(pos);
			CBCGPGridRow *pRow = pItem->GetParentRow();
			

			UINT nMemberId = pRow->GetData();
			INT_PTR nIn = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);
			if ( nIn!=-1 )
			{
				// ���͸�ָ���ͻ���
				nValid++;		
				dlg.AddReceiver( (UINT)nIn );
			}

			
		}
		dlg.SetSendType(1);
		dlg.DoModal();
	}
	else
	{
		CShortMsgDlg dlg;
		dlg.DoModal();
	}
}

void CActiveMemberView::OnModifyRemark()
{

	CBCGPGridRow* pRow = GetGridCtrl()->GetCurSel();
	if (!pRow)
	{
		return;
	}

	UINT nMemberID = pRow->GetData();

	CActiveMember ActiveMember;
	if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberID, ActiveMember))
	{
		return;
	}
	
	CDlgInputParam dlgInput(this);
	dlgInput.InitParam(LOAD_STRING(IDS_MODIFY_REMARK),LOAD_STRING(IDS_REMARK),ActiveMember.GetRemark());
	if (dlgInput.DoModal() == IDOK)
	{
		ActiveMember.SetRemark( dlgInput.m_strParam );
		CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember,TRUE);
		pRow->GetItem(14)->SetValue(ActiveMember.GetRemark().GetString(), TRUE);
	}
}

void CActiveMemberView::OnCreditModify()
{
	CActiveMember ActiveMember;

	if (GetCurActiveMember(ActiveMember))
	{
		if (ActiveMember.GetIsLocalUser())
		{
			theApp.IBAMsgBox(IDS_FORBID_LOCAL_REVERSE,MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		else
		{
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
			{
				theApp.IBAMsgBox(IDS_FORBID_REVERSE,MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}
		if (CRightsManagement::GetInstance()->Take_IBAP_CREDITREVERSE())
		{
			CCreditReverseDlg dlg;
			dlg.SetNetId(ActiveMember.GetNetId(), TRUE);
			dlg.DoModal();
		}
	}	
}

void CActiveMemberView::OnUpdateCreditReverse(CCmdUI *pCmdUI)
{
	CRightsManagement::GetInstance()->SetUI_IBAP_CREDITREVERSE(pCmdUI);
}

CActiveMemberGrid* CActiveMemberView::GetActiveMemberGrid()
{
	return (CActiveMemberGrid*)m_pWndGridCtrl;
}

void CActiveMemberView::OnUpdateRetailonline(CCmdUI *pCmdUI)
{
	OnUpdateChangeComputer(pCmdUI);
}

void CActiveMemberView::OnUpdateConsumelist2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		CActiveMember ActiveMember;

		if (GetCurActiveMember(ActiveMember))
		{
			if (ActiveMember.GetIsLocalUser())
			{
				pCmdUI->Enable(FALSE);
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}
}


BOOL CActiveMemberView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN)
	{
		if (EnterActiveMember())
		{
			return TRUE;
		}
		return CBCGPGridView::PreTranslateMessage(pMsg);
	}

	return CBCGPGridView::PreTranslateMessage(pMsg);
}

BOOL CActiveMemberView::EnterActiveMember()
{
	if (!theApp.GetCurCashier()->IsOnDuty()) return FALSE;

	CActiveMember ActiveMember;

	if (!GetCurActiveMember(ActiveMember)) return FALSE;

	// 2011/07/11-8201-gxx: 
	if (ActiveMember.GetIsLocalUser() || CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		switch(CIBAConfig::GetInstance()->GetOperationAfterEnter())
		{
		case CIBAGlobal::emOperationAfterCredit:
			{
				CNDCreditDlg dlg;
				dlg.SetNetId(ActiveMember.GetNetId());
				dlg.DoModal();
			}
			break;
		case CIBAGlobal::emOperationAfterRefund:
			{
				CNDReturnDlg dlg;
				dlg.SetNetId(ActiveMember.GetNetId());
				dlg.DoModal();
			}
			break;
		case CIBAGlobal::emOperationAfterNull:
			{
				return TRUE;
			}
		default:
			return FALSE;
		}	
		return TRUE;	
	}

	if (CCurrentNetBarUser::GetInstance()->QueryBalance(ActiveMember.GetNetId()))
	{
		switch(CIBAConfig::GetInstance()->GetOperationAfterEnter())
		{
		case CIBAGlobal::emOperationAfterCredit:
			{
				CNDCreditDlg dlg;
				dlg.DoModal();
			}
			break;
		case CIBAGlobal::emOperationAfterRefund:
			{
				// 2011/10/31-8210-gxx: 
				if (ActiveMember.GetMemberID() == ActiveMember.GetRoomMemberId())
				{
					// �������˺ţ��Զ����������������
					CNetbarRoomDlg dlg;
					dlg.SetNetId(ActiveMember.GetNetId(),TRUE);
					dlg.DoModal();
				}
				else
				{
					CNDReturnDlg dlg;
					dlg.DoModal();
				}
			}
			break;
		case CIBAGlobal::emOperationAfterNull:
			{
				return TRUE;
			}
		default:
			return FALSE;
		}	
		return TRUE;	
	}
	else
	{
		//�û��Ѿ�ע����
		if (CCurrentNetBarUser::GetInstance()->GetMemberId() == 0
			&& CCurrentNetBarUser::GetInstance()->GetLastStatusCode() >= 0)
		{
			// 2014-10-28 - qsc
			if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
			{
				NS_REALNAME::CheckoutInfo coi;
				ZeroMemory(&coi, sizeof(coi));
				_tcsncpy(coi.szName, ActiveMember.GetUserName(), 100);
				_tcsncpy(coi.szNetId, ActiveMember.GetNetId(), 100);
				_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

				CString strIP;
				CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
				_tcsncpy(coi.szIP, strIP, 50);

				if(ActiveMember.IsMember())
				{
					coi.nCheckoutType = CIBAGlobal::cot_TempLogout;
				}
				else
				{
					coi.nCheckoutType = CIBAGlobal::cot_MemLogout;
				}
				

				theApp.GetRealName()->CheckOut(&coi);
			}

			((CActiveMemberGrid*)m_pWndGridCtrl)->GetActiveMemberList()->RemoveActiveMember(ActiveMember.GetMemberID());
			Refresh(TRUE);

			CIBAString strInfo;
			strInfo.LoadString(IDS_HASCHECKOUT);
			CString str;
			str.Format(strInfo, ActiveMember.GetNetId());
			theApp.IBAMsgBox(str);

			CIBALog::GetInstance()->Write(str);

			// %s ��ע��
		}
	}
	return FALSE;
}