// NDReturnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "MainFrm.h"
#include "Cashier.h"
#include "IBAPrint.h"
#include "IBAHelpper.h"
#include "ServletHellper.h"
#include "dal\IBADAL.h"
#include "tts\IBATTS.h"
#include "Socket\LocalServer.h"
#include "RealName\IBARealName.h"
#include "ComputerListView.h"
#include "ActiveMemberView.h"
#include "DialogManager.h"
#include "NDCreditDlg.h"
#include "BundleTimeDlg.h"
#include ".\ndreturndlg.h"
#include "ActiveMemberList.h"
#include ".\Socket\LocalServer.h"
#include "CashierHelper.h"


// CNDReturnDlg �Ի���

IMPLEMENT_DYNAMIC(CNDReturnDlg, CNDDialog)
CNDReturnDlg::CNDReturnDlg(CWnd* pParent /*=NULL*/)
: CNDDialog(CNDReturnDlg::IDD, pParent)
, m_bAutoQuery(FALSE), m_bUserHasCard(FALSE)
, m_bPrintTicket(FALSE)
{
	m_nConsumeMoney = 0;
}

CNDReturnDlg::~CNDReturnDlg()
{
}

void CNDReturnDlg::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_USERNAME, m_strUserInfo);
	DDX_Text(pDX, IDC_STATIC_ALL, m_strAllBalance);
	DDX_Text(pDX, IDC_STATIC_NETBAR, m_strOpenNetBar);
	DDX_Text(pDX, IDC_STATIC_TERMID, m_strTermId);
	DDX_Text(pDX, IDC_STATIC_BTENDTIME, m_strBundleTimeInfo);
	DDX_Control(pDX, IDC_BUTTON_CONSUMELIST, m_btnConsumeList);
	DDX_Control(pDX, IDC_BUTTON_CREDIT, m_btnCredit);
	DDX_Control(pDX, IDC_BUTTON_RETURN2, m_btnReturn2);
	DDX_Control(pDX, IDC_BUTTON_BUNDLETIME, m_btnBundleTime);
	DDX_Control(pDX, IDC_BUTTON_CONSUMELIST2, m_btnZSBConsumeList);
	DDX_Control(pDX, IDC_BUTTON_CANCELACTIVE, m_btnCancelActivation);

	DDX_Check(pDX, IDC_CHECK_TICKET, m_bPrintTicket);
}


BEGIN_MESSAGE_MAP(CNDReturnDlg, CNDDialog)
	ON_BN_CLICKED(IDC_BUTTON_CREDIT, OnBnClickedButtonCredit)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CONSUMELIST, OnBnClickedButtonConsumelist)
	ON_BN_CLICKED(IDC_BUTTON_RETURN2, OnBnClickedButtonReturn2)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_BUNDLETIME, OnBnClickedButtonBundletime)
	ON_BN_CLICKED(IDC_BUTTON_CONSUMELIST2, OnBnClickedButtonConsumelist2)
	ON_BN_CLICKED(IDC_BUTTON_CANCELACTIVE, OnBnClickedButtonCancelactive)
END_MESSAGE_MAP()


// CNDReturnDlg ��Ϣ�������

BOOL CNDReturnDlg::OnInitDialog()
{
	CNDDialog::OnInitDialog();

	InitControls();
	
	if (m_bAutoQuery)
	{
		SetTimer(TimeIdOfAutoQuery, 100, NULL);
	}
	else
	{
		m_nMemberId = CCurrentNetBarUser::GetInstance()->Balance.GetMemberId();
		m_nCardId = CCurrentNetBarUser::GetInstance()->Balance.GetCardId(); 
		OnGetBalanceOK(CCurrentNetBarUser::GetInstance()->Balance);

		SetTimer(TimeIdOfAutoQuery, 800, NULL);
	}

	

	GetDlgItem(IDOK)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNDReturnDlg::OnBnClickedButtonCredit()
{
	CBCGPDialog::OnCancel();

	CNDCreditDlg dlg;
	dlg.SetNetId(m_strNetId);
	dlg.DoModal();
}

void CNDReturnDlg::OnBnClickedCancel()
{
	AutoClose();
}

void CNDReturnDlg::OnBnClickedOk()
{
	IBA_TRACE("Click.......................................................");
	CWaitCursor WaitCursor;

	UpdateData();

	//{ 2011/04/29-gxx: ��Ӷ�������
	if (m_bCenterDisconnect)
	{
		BOOL bRet = DoLocalModeOperate();
		if (bRet)
		{
			AutoClose();
		}
		else
		{
			CIBALog::GetInstance()->Write(_T("�������ĶϿ�������£����ж�������ʧ��!"));
			SetToolTipPos(IDOK);
			if (m_LocalActiveMember.GetIsLocalUser())
			{
				ShowToolTip(LOAD_STRING(IDS_RETURN_FAILURE),TRUE); // �˿�ʧ��
			}
			else
			{
				ShowToolTip(LOAD_STRING(IDS_JZ_FAILURE),TRUE); // ����ʧ��
			}		
		}
	}
	//}
	else
	{
		if (m_bIsMember)
		{
			if (PayoutMember())
			{
				CBCGPDialog::OnOK();
			}
		}
		else
		{
			if (m_bOnLine)
			{
				INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

				if (nIndex >= 0)
				{	
					CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId);

					Sleep(500);
				}

				CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
			}

			//ע���û�
			DeregisterUser();
		}
	}

	
	
}

void CNDReturnDlg::OnBnClickedButtonConsumelist()
{
	GetDlgItem(IDOK)->SetFocus();

	COperationInfo OperationInfo;

	OperationInfo.SetMemberId(m_nMemberId);
	OperationInfo.SetUserName(m_strName);
	OperationInfo.SetNetId(m_strNetId);

	OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
	OperationInfo.SetIsMember(m_ActiveMember.IsMember());

	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, MAKELONG(2,0), (LPARAM)&OperationInfo);
}

void CNDReturnDlg::OnBnClickedButtonConsumelist2()
{
	GetDlgItem(IDOK)->SetFocus();

	COperationInfo OperationInfo;

	OperationInfo.SetMemberId(m_nMemberId);
	OperationInfo.SetUserName(m_strName);
	OperationInfo.SetNetId(m_strNetId);

	OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
	OperationInfo.SetIsMember(m_ActiveMember.IsMember());

	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, MAKELONG(2,4), (LPARAM)&OperationInfo);
}

BOOL CNDReturnDlg::PayoutMember()
{
	CWaitCursor WaitCursor;

	CActiveMember ActiveMember;
	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId,ActiveMember);

	if (m_bOnLine) //����
	{
		CCheckout2Method Checkout2;
		Checkout2.SetSafeLogout(0); //�۵���ǰʱ��
		Checkout2.SetMemberId(m_nMemberId);
		theApp.GetCurCashier()->DoCheckout2(Checkout2);	

		if (Checkout2.GetStatusCode() == 0)
		{
			INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

			if (nIndex >= 0) //Զ���»�
			{
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId,2);
				Sleep(2000);

				CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
				computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
				computer.ClearUserInfo();
				CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);

				CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
			}

			if (m_bPrintTicket)
			{
				CString strTicket;
				CIBAPrint::GetRefundTicket(strTicket,m_bIsMember);

				strTicket.Replace(_T("[name]"), m_strName);
				strTicket.Replace(_T("[idnumber]"), m_strPersonalID);
				strTicket.Replace(_T("[memberclass]"), m_strUserClass);
				//CString strTmp;
				//strTmp.Format(_T("%.2f"), m_nConsumeMoney/100.0);
				//strCreditMoney.Format(_T("%.2f"), m_nCreditMoney/100.0);
				//strTicket.Replace(_T("[Consume]"), strTmp); // ����
				
				strTicket.Replace(_T("[return]"), _T("0.00")); // ��Ա�û��˿���Ϊ0

				strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);

				strTicket.Replace(_T("[netid]"), m_strNetId.Right(8));
				CString strMemberID;
				strMemberID.Format(_T("%d"), m_nMemberId);
				strTicket.Replace(_T("[memberid]"), strMemberID);
				strTicket.Replace(_T("[totalmoney]"), m_strAllBalance);

				CIBAPrint::Print(strTicket);
			}
		}
		else if( Checkout2.GetStatusCode() != -1)
		{
			INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

			if (nIndex >= 0) //Զ���»�
			{
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId,0);
				Sleep(2000);

				CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
				computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
				computer.ClearUserInfo();
				CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);

				CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
			}
		}
	}
	else
	{
		IBA_LOG0(_T("��Ա����ʱ,ȡ������"));
		CActivationMethod Activation;
		Activation.SetMemberId(m_nMemberId);
		Activation.SetMode(CActivationMethod::ACTIVATION_NO);
		theApp.GetCurCashier()->DoActivation(Activation);

		// 2011/10/27-8210-gxx: 
		if (Activation.GetStatusCode() == 0)
		{
			INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
			if (nIndex >= 0)
			{
				CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
				theApp.GetRealName()->AuditUserCheckOut(
					computer.GetTerminalID(), computer.GetComputerIP(), m_nMemberId);	
			}
		}
		

		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(m_nMemberId);
	}

	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		// �������ǰ����ʱͨ�ģ��������ʱ����������Ͽ���Ϊ�˷�ֹżȻ����Ӵ���
		OnBnClickedButtonQuery();
		return FALSE;
	}
	else
	{
		// Checkout2ִ�гɹ���������û���ͼ��,ֻ��ˢ��
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);	
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeVipReturn);
		OperationInfo.SetNetId(m_strNetId);
		OperationInfo.SetUserName(m_strName);
		OperationInfo.SetDescription(m_strTermId);

		// 2011/10/18-8208-gxx: 
		OperationInfo.SetClassId(ActiveMember.GetClassId());
		OperationInfo.SetTermId(ActiveMember.GetTerminalID());
		OperationInfo.SetCheckInTime(ActiveMember.GetCheckInTime());
		OperationInfo.SetIdNum(ActiveMember.GetPersonalID());
		
		// 2014-10-10 - qsc
		if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
		{
			NS_REALNAME::CheckoutInfo coi;
			ZeroMemory(&coi, sizeof(coi));
			_tcsncpy(coi.szName, m_strName, 100);
			_tcsncpy(coi.szNetId, m_strNetId, 100);
			_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

			CString strIP;
			CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
			_tcsncpy(coi.szIP, strIP, 50);
			coi.nCheckoutType = CIBAGlobal::cot_MemLogout;

			theApp.GetRealName()->CheckOut(&coi);
		}
		//CLocalServer::GetInstance()->GetActiveMemberView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

		return TRUE;
	}
}


void CNDReturnDlg::InitControls()
{
	CIBAHelpper::CreateFont(m_BigFont, 22, 900);

	GetDlgItem(IDC_STATIC_ALL)->SetFont(&m_BigFont);
	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFont(&m_BigFont);
	GetDlgItem(IDC_STATIC_TERMID)->SetFont(&m_Font);

	// 2014-11-5 - qsc �Ƕ��²��ܡ�ȡ�������ֹȡ������ͽ��˵��·��˶�ν�����Ϣ��ʵ��
	if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType() )
	{
		GetDlgItem(IDC_BUTTON_CANCELACTIVE)->ShowWindow(SW_HIDE);
	}
}

void CNDReturnDlg::OnGetBalanceOK(CBalanceMethod & Balance)
{
	CNDDialog::OnGetBalanceOK(Balance);

	m_strDeposit.Format(_T("%.2f"), Balance.GetGuarantyMoney() / 100.0);//Ѻ��

	if (m_bIsMember)
	{
		m_nAllMoney = Balance.GetTotalBalacne();

		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(IDS_VIP_RETURN)); //��ʾΪ����Ա����(&R)
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_BALANCE));
		SetWindowText(LOAD_STRING(IDS_PAYOUTMEMBER)); // ��ʾΪ: ��Ա����
	}
	else
	{
		m_nAllMoney = Balance.GetReturnAmount() + Balance.GetGuarantyMoney();
	}

	m_nConsumeMoney = Balance.GetLastConsumeAmount();
	
#ifdef __USING_INTER_RESOURCE
	m_strAllBalance.Format(_T("��%.2f"), m_nAllMoney / 100.0);
#else
	m_strAllBalance.Format(_T("%.2f"), m_nAllMoney / 100.0);
#endif

	m_strLastLogoutTime = Balance.GetLogoutTime();

	m_strUserInfo.Format(_T("%s, %s"), m_strName, m_strUserClass);

	ShowBundleTimeInfo(Balance);

	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, m_ActiveMember);

	m_bPrintTicket = CIBAConfig::GetInstance()->GetPrintReturnTicket(m_bIsMember);

	//{ 2011/10/26-8230-gxx: 
	if (m_ActiveMember.GetRoomMemberId() == m_nMemberId)
	{
		UINT nID = m_bIsMember?IDS_ROOM_CHECKOUT:IDS_ROOM_RETURN;
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(nID)); //��ʾΪ����������(&R)
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(nID));
		SetWindowText(LOAD_STRING(nID)); 
	}
	
	//}
	

	UpdateData(FALSE);

	RedrawWindow();

	//COleDateTimeSpan span = COleDateTime::GetCurrentTime() - m_ActiveMember.GetActivationTime();
	//if (span.GetTotalSeconds() <= 5.0)
	//{
	//	// �������ڼ����5���ڵ����˿����, �ڴ˵�����ֵ����
	//	CNDReturnDlg::OnBnClickedButtonCredit();
	//	return;
	//}

}


HBRUSH CNDReturnDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNDDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_ALL)
		{
			if (m_bIsMember)
			{
				pDC->SetTextColor(RGB(0, 0, 240)); // ��Ա����ɫ��ʾ
			}
			else
			{
				pDC->SetTextColor(RGB(255, 0, 0)); // ��ʱ�û��ú�ɫ��ʾ
			}
		}
		
	}

	return hbr;
}

void CNDReturnDlg::DeregisterUser()
{
	CDeregisterMethod Deregister;

	Deregister.SetMemberId(m_nMemberId);
	Deregister.SetPersonalId(m_strPersonalID);
	Deregister.SetMemberName(m_strMemberName);
	Deregister.SetCardId(m_nCardId);
	Deregister.SetPassword(m_strPwd);
	Deregister.SetEndUsingTime(m_strLastLogoutTime);

	theApp.GetCurCashier()->DoDeregister(Deregister);

	if (Deregister.GetStatusCode() == 0)
	{	
		UINT nAll = Deregister.GetBalance() + Deregister.GetGuarantyMoney();//�ϼ�

		// 2011/09/07-8201-gxx: 
		if (m_bPrintTicket)
		{
			CString strTicket;
			CIBAPrint::GetRefundTicket(strTicket,m_bIsMember);

			strTicket.Replace(_T("[name]"), m_strName);
			strTicket.Replace(_T("[idnumber]"), m_strPersonalID);
			strTicket.Replace(_T("[memberclass]"), m_strUserClass);
			//CString strTmp;
			//strTmp.Format(_T("%.2f"), nAll/100.0);
			
			//strTicket.Replace(_T("[Consume]"), strTmp); // ����

			strTicket.Replace(_T("[return]"), m_strAllBalance);

			strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);

			strTicket.Replace(_T("[netid]"), m_strNetId.Right(8));
			CString strMemberID;
			strMemberID.Format(_T("%d"), m_nMemberId);
			strTicket.Replace(_T("[memberid]"), strMemberID);
			strTicket.Replace(_T("[totalmoney]"), _T("0.00")); // ��ʱ�û��˿���Ϊ0

			CIBAPrint::Print(strTicket);
		}
		

		CIBATTS::GetInstance()->SpeakReturnResult(nAll);//����

		{//д������־
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(2);
		OperationInfo.SetIsBundTime(FALSE);
		OperationInfo.SetUserName(m_strName);
		OperationInfo.SetNetId(m_strNetId);
		OperationInfo.SetMemberId(m_nMemberId);
		OperationInfo.SetOperationAmount(Deregister.GetBalance());
		OperationInfo.SetAllAmount(nAll);
		OperationInfo.SetDeposit(Deregister.GetGuarantyMoney());
		OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
		OperationInfo.SetDescription(m_strTermId);
		OperationInfo.SetClassId(m_ActiveMember.GetClassId());
		OperationInfo.SetTermId(m_ActiveMember.GetTerminalID());
		OperationInfo.SetIdNum(m_ActiveMember.GetPersonalID());
		OperationInfo.SetCheckInTime(m_ActiveMember.GetCheckInTime());
		OperationInfo.SetConsumeMoney(m_ActiveMember.GetAdvancePayment()-nAll);

		// 2014-10-10 - qsc
		if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
		{
			NS_REALNAME::CheckoutInfo coi;
			ZeroMemory(&coi, sizeof(coi));
			_tcsncpy(coi.szName, m_strName, 100);
			_tcsncpy(coi.szNetId, m_strNetId, 100);
			_tcsncpy(coi.szTermId, m_ActiveMember.GetTerminalID(), 50);

			CString strIP;
			CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
			_tcsncpy(coi.szIP, strIP, 50);
			coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

			theApp.GetRealName()->CheckOut(&coi);
		}
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
		}

		{//д���ݿ�
		CReturnedInfo ReturnedInfo;

		ReturnedInfo.SetCardId(m_nCardId);
		ReturnedInfo.SetReturnDate(CIBAHelpper::FormatCenterTime(Deregister.GetTranTime()));
		ReturnedInfo.SetRefNo(Deregister.GetRefNo());
		ReturnedInfo.SetDeposit(Deregister.GetGuarantyMoney());
		ReturnedInfo.SetOperator(theApp.GetCurCashier()->GetName());
		ReturnedInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
		ReturnedInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
		ReturnedInfo.SetMemberId(m_nMemberId);
		ReturnedInfo.SetSerialNo(Deregister.GetLocalSerialNo());
		ReturnedInfo.SetReturnAmount(nAll);
		ReturnedInfo.SetClassId(m_nUserClassID);

		CIBADAL::GetInstance()->AddReturnedRecord(ReturnedInfo);
		}

		m_btnConsumeList.ShowWindow(SW_HIDE);

		//ʵ������ʱ��
		if (CNetBarConfig::GetInstance()->GetReturnOnlyClassId() != m_nUserClassID)
		{
			theApp.GetRealName()->InvokeRealName(100, (LPVOID)m_strNetId.GetString());
		}

		

		//��ռ���б���Ϣ

		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
		
		// 2011/11/03-8210-gxx: ������������
		if (m_ActiveMember.GetIsOnLine() && m_nMemberId == m_ActiveMember.GetRoomMemberId())
		{
			if (nIndex >= 0)
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId, 2);
		}

		if (nIndex >= 0)
		{
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			computer.ClearUserInfo();
			CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);
			CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
		}

		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

		//AutoClose();
		CDialog::OnOK();
	}
	else
	{	
		SetToolTipPos(IDOK);
		ShowToolTip(Deregister.GetStatusMessage(), TRUE);

		OnBnClickedButtonQuery();
	}
}

void CNDReturnDlg::SetNetId(CString newVal)
{
	m_strNetId = newVal;

	m_bAutoQuery = TRUE;
}

void CNDReturnDlg::OnTimer(UINT nIDEvent)
{
	if (TimeIdOfAutoQuery == nIDEvent)
	{
		KillTimer(nIDEvent);

		if (m_bAutoQuery)
		{
			m_bAutoQuery = FALSE;
			OnBnClickedButtonQuery();
		}
		else
		{
			if (theApp.GetRealName()->RobFocusAfterSwipingCard())
			{
				SimulateClickMe();	
			}
		}
	}

	CNDDialog::OnTimer(nIDEvent);
}

BOOL CNDReturnDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_F3 == pMsg->wParam)
		{
			OnBnClickedButtonCredit();
			return TRUE;
		}
	}

	return CNDDialog::PreTranslateMessage(pMsg);
}

void CNDReturnDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if ( int(m_nMemberId) > 0)
	{
		m_Images.Draw(&dc, m_bIsMember ? 1 : 2, CPoint(30, 25), ILD_NORMAL);
	}
	else if( int(m_nMemberId) < 0 )
	{
		m_Images.Draw(&dc, 5, CPoint(30, 25),ILD_NORMAL);
	}
}

void CNDReturnDlg::ShowBundleTimeInfo(CBalanceMethod & Balance)
{
	if (Balance.GetPayType() > 0)
	{
		COleDateTime StartTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetStartTime());
		COleDateTime EndTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime());

		COleDateTimeSpan dtSpan;

		if (COleDateTime::GetCurrentTime() < StartTime)
		{
			dtSpan = EndTime - StartTime;
		}
		else
		{
			dtSpan = EndTime - COleDateTime::GetCurrentTime();
		}

		m_strBundleTimeInfo.Format(_T("%s �� %s��ʣ�� %d:%.2d\r\n\r\n���(%.1fԪ)"),
			StartTime.Format(_T("%H:%M")),
			EndTime.Format(_T("%H:%M")),
			(LONG)dtSpan.GetTotalHours(), dtSpan.GetMinutes(), 
			Balance.GetPrice() / 100.0
			);

		//��ʾ����ť

		if (!m_bIsMember && m_nAllMoney - Balance.GetPrice() > 0)
		{
			m_btnReturn2.ShowWindow(SW_NORMAL);
		}
		else
		{
			m_btnReturn2.ShowWindow(SW_HIDE);
		}
	}
}

void CNDReturnDlg::OnBnClickedButtonReturn2()
{
	if (IDOK != MsgBox(IDS_RETURN2QUESTION, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}

	CDeregisterMethod Deregister;

	Deregister.SetMemberId(m_nMemberId);
	Deregister.SetPersonalId(m_strPersonalID);
	Deregister.SetMemberName(m_strMemberName);
	Deregister.SetCardId(m_nCardId);
	Deregister.SetPassword(m_strPwd);
	Deregister.SetEndUsingTime(m_strLastLogoutTime);
	Deregister.SetReturnMode(2);

	theApp.GetCurCashier()->DoDeregister(Deregister);

	if (Deregister.GetStatusCode() == 0)
	{	
		UINT nAll = Deregister.GetBalance();

		//{ 2011/06/30-gxx: 
		CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(m_nMemberId,0,0,nAll);
		
		//}
		
		CIBATTS::GetInstance()->SpeakReturn2Result(nAll);//����

		//д������־
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(8);
		OperationInfo.SetIsBundTime(FALSE);
		OperationInfo.SetUserName(m_strName);
		OperationInfo.SetNetId(m_strNetId);
		OperationInfo.SetMemberId(m_nMemberId);
		OperationInfo.SetOperationAmount(0);
		OperationInfo.SetAllAmount(nAll);
		OperationInfo.SetDeposit(Deregister.GetGuarantyMoney());
		OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
		OperationInfo.SetDescription(m_strTermId);

		// 2014-10-10 - qsc
		if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
		{
			NS_REALNAME::CheckoutInfo coi;
			ZeroMemory(&coi, sizeof(coi));
			_tcsncpy(coi.szName, m_strName, 100);
			_tcsncpy(coi.szNetId, m_strNetId, 100);
			_tcsncpy(coi.szTermId, m_ActiveMember.GetTerminalID(), 50);

			CString strIP;
			CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
			_tcsncpy(coi.szIP, strIP, 50);
			coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

			theApp.GetRealName()->CheckOut(&coi);
		}
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
	
		ReQuery();

		SetToolTipPos(IDC_BUTTON_RETURN2);
		ShowToolTipFormat(IDS_RETURN2INFO, nAll / 100.0);
	}
	else
	{	
		SetToolTipPos(IDOK);
		ShowToolTip(Deregister.GetStatusMessage(), TRUE);
	}
}

void CNDReturnDlg::OnCenterDisconnect()
{
	m_bCenterDisconnect = TRUE;

	CActiveMember ActiveMember;

	BOOL bRes = CLocalServer::GetInstance()
		->ActiveMemberList.GetActiveMemberNetID(m_strNetId,ActiveMember);

	m_strName.Format(_T("%s(%s)"), ActiveMember.GetUserName(), CIBAHelpper::GetSexAsString(ActiveMember.GetSex()));
	m_strPersonalID = ActiveMember.GetPersonalID();

	m_strMemberName = ActiveMember.GetUserName();

#ifdef __USING_INTER_RESOURCE
	m_strAllBalance.Format(_T("��%.2f"), ActiveMember.GetAvailavleBalance() / 100.0);
#else
	m_strAllBalance.Format(_T("%.2f"), ActiveMember.GetAvailavleBalance() / 100.0);
#endif

	m_strOpenNetBar = CNetBarConfig::GetInstance()->GetNetBarName(ActiveMember.GetNetbarID());

	m_strTermId = ActiveMember.GetTerminalID();

	m_strUserInfo = m_strMemberName;
	
	if (ActiveMember.IsMember())
	{
		GetDlgItem(IDC_STATIC0)->SetWindowText(_T("���(Ԫ)��"));
	}

	UpdateData(FALSE);
}

BOOL CNDReturnDlg::DoDisconnectReturn()
{
	if (m_strNetId.IsEmpty())
	{
		IBA_ASSERT(0);
		return FALSE;
	}
	
	return CCashierHelper::ReturnLocalCenterUser(m_LocalActiveMember);

}
void CNDReturnDlg::OnBnClickedButtonBundletime()
{
	CBCGPDialog::OnCancel();

	CDlgBundleTime dlg;
	dlg.SetNetId(m_strNetId, TRUE);
	dlg.DoModal();
}

BOOL CNDReturnDlg::DoLocalModeQuery()
{
	m_bCenterDisconnect = TRUE;
	if (!CNDDialog::DoLocalModeQuery())
	{
		return FALSE;
	}

	// 2011/07/29-8201-gxx: ���������߱����û�
	m_btnBundleTime.ShowWindow(FALSE);     // ���ð�ʱ����
	m_btnConsumeList.ShowWindow(FALSE);    // �������Ѽ�¼
	m_btnZSBConsumeList.ShowWindow(FALSE); // �������ձ���¼

	if (!m_LocalActiveMember.GetIsLocalUser())
	{
		m_btnCredit.ShowWindow(FALSE); // ���ó�ֵ����

		if (m_LocalActiveMember.IsMember())
		{
			GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(IDS_VIP_RETURN)); //��ʾΪ����Ա����(&R)
			GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_BALANCE));
			SetWindowText(LOAD_STRING(IDS_PAYOUTMEMBER)); // ��ʾΪ: ��Ա����
		}
	}

	m_strUserInfo = m_strName;

	if (m_LocalActiveMember.GetIsLocalUser())
	{
		UINT nExpense = m_LocalActiveMember.GetLocalExpense();
		if (nExpense > (UINT)m_LocalActiveMember.GetAdvancePayment())
		{
			IBA_ASSERT(FALSE);
			m_nAllMoney = 0;
		}
		else
		{
			m_nAllMoney = m_LocalActiveMember.GetAdvancePayment() - nExpense;
		}
	}
	else
	{
		m_nAllMoney = m_LocalActiveMember.GetAvailavleBalance();
	}
	
	m_LocalActiveMember.SetAvailavleBalance(m_nAllMoney);

#ifdef __USING_INTER_RESOURCE
	m_strAllBalance.Format(_T("��%.2f"), m_nAllMoney / 100.0);
#else
	m_strAllBalance.Format(_T("%.2f"), m_nAllMoney / 100.0);
#endif

	if (m_LocalActiveMember.IsMember())
	{
		// �����û����л�Ա�������û��޻�Ա
		ASSERT(!m_LocalActiveMember.GetIsLocalUser());
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_BALANCE)); // ��ʾΪ�����:
	}

	UpdateData(FALSE);

	RedrawWindow();
	return TRUE;
}

BOOL CNDReturnDlg::DoLocalModeOperate()
{
	// CNDDialog::DoLocalModeOperate//---do nothing
	if (m_strNetId.IsEmpty() || m_LocalActiveMember.GetMemberID() == 0)
	{
		IBA_ASSERT(0);
		return FALSE;
	}

	if (!m_LocalActiveMember.GetIsLocalUser())
	{
		return DoDisconnectReturn();
	}
	else
	{
		return CCashierHelper::ReturnLocalUser(m_LocalActiveMember);
	}
	
	return FALSE;
}

BOOL CNDReturnDlg::ReturnRoomUser()
{
	CRoomCheckOut roomCheckOut;
	roomCheckOut.SetMemberId(m_nMemberId);
	roomCheckOut.SetPCClass(m_ActiveMember.GetPCClassID());

	theApp.GetCurCashier()->DoRoomCheckOut(roomCheckOut);

	if (roomCheckOut.GetStatusCode() == 0)
	{
		CArray<UINT> arrMembers;
		if (!CLocalServer::GetInstance()->ActiveMemberList.GetRoomSMembers(m_nMemberId, arrMembers))
		{
			IBA_LOG0(_T("��ȡ�����ڵ������û�ʧ��"));
			return FALSE;
		}

		for(int i=0; i < arrMembers.GetCount(); i++)
		{
			UINT nMemberId = arrMembers.GetAt(i);
			CActiveMember ActiveMember;
			CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
			INT_PTR nIn = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);
			
			IBA_LOG0(_T("�������˺Ž��ˣ����˺Ÿ��ű��ǳ������"));
			if ( nIn!=-1 )
			{
				// ���͸�ָ���ͻ���
				CLocalServer::GetInstance()->LocalCheckOutUser(nIn, nMemberId, 2);
			}
			CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(nMemberId);

			if (!m_bIsMember)
			{
				CDeregisterMethod Deregister;

				Deregister.SetMemberId(nMemberId);
				Deregister.SetPersonalId(ActiveMember.GetPersonalID());
				Deregister.SetMemberName(ActiveMember.GetUserName());
				Deregister.SetCardId(0);
				Deregister.SetPassword(_T(""));
				Deregister.SetEndUsingTime(m_strLastLogoutTime);

				theApp.GetCurCashier()->DoDeregister(Deregister);

				if (Deregister.GetStatusCode() == 0)
				{	
					IBA_LOG0(_T("�������û��ɹ��˿�"));
				}
				else
				{
					IBA_LOG(_T("�������û��˿�ʧ��, ����ID=%d, netId=%s"), m_ActiveMember.GetPCClassID(), m_ActiveMember.GetNetId());
					SetToolTipPos(IDOK);
					ShowToolTip(Deregister.GetStatusMessage(), TRUE);
					return FALSE;
				}
			}
		}

		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);

		SetToolTipPos(IDOK);
		ShowToolTip(roomCheckOut.GetStatusMessage(), FALSE);

		// ���������־
		{//д������־
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeRoomReturn);
			OperationInfo.SetIsBundTime(FALSE);
			OperationInfo.SetUserName(m_strName);
			OperationInfo.SetNetId(m_strNetId);
			OperationInfo.SetMemberId(m_nMemberId);
			OperationInfo.SetOperationAmount(m_ActiveMember.GetAvailavleBalance());
			OperationInfo.SetAllAmount(m_nAllMoney);
			OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
			CString strTmp;
			strTmp.Format(LOAD_STRING(IDS_ROOM_BODY), arrMembers.GetCount());
			OperationInfo.SetDescription(strTmp);
			OperationInfo.SetClassId(m_ActiveMember.GetClassId());
			OperationInfo.SetTermId(m_ActiveMember.GetTerminalID());
			OperationInfo.SetIdNum(m_ActiveMember.GetPersonalID());
			OperationInfo.SetCheckInTime(m_ActiveMember.GetCheckInTime());
			OperationInfo.SetConsumeMoney(m_ActiveMember.GetAdvancePayment()-m_nAllMoney);

			// 2014-10-10 - qsc
			if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
			{
				NS_REALNAME::CheckoutInfo coi;
				ZeroMemory(&coi, sizeof(coi));
				_tcsncpy(coi.szName, m_strName, 100);
				_tcsncpy(coi.szNetId, m_strNetId, 100);
				_tcsncpy(coi.szTermId, m_ActiveMember.GetTerminalID(), 50);

				CString strIP;
				CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
				_tcsncpy(coi.szIP, strIP, 50);
				coi.nCheckoutType = m_ActiveMember.IsMember() ? CIBAGlobal::cot_MemLogout : CIBAGlobal::cot_TempLogout;

				theApp.GetRealName()->CheckOut(&coi);
			}
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

void CNDReturnDlg::OnBnClickedButtonCancelactive()
{
	CActivationMethod Activation;

	Activation.SetMemberId(m_nMemberId);
	Activation.SetMode( CActivationMethod::ACTIVATION_NO );

	theApp.GetCurCashier()->DoActivation(Activation);

	if (Activation.GetStatusCode() == 0)
	{	
		SetToolTipPos(IDC_BUTTON_CANCELACTIVE);
		ShowToolTip(Activation.GetStatusMessage());

		//����ǻ�Ա�Ŵӵ�ǰ�û���ɾ����������ʱ�û�
		if (CCurrentNetBarUser::GetInstance()->Balance.GetIsMember() == 1)
		{
			// 2014-11-5 - qsc ��Ա�û�����ȡ��������˲���
			// 2014-10-14 - qsc
			if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
			{
				NS_REALNAME::CheckoutInfo coi;
				ZeroMemory(&coi, sizeof(coi));
				_tcsncpy(coi.szName, m_strName, 100);
				_tcsncpy(coi.szNetId, m_strNetId, 100);
				_tcsncpy(coi.szTermId, m_ActiveMember.GetTerminalID(), 50);

				CString strIP;
				CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
				_tcsncpy(coi.szIP, strIP, 50);
				coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

				theApp.GetRealName()->CheckOut(&coi);
			}

			CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(m_nMemberId);
		}
		else
		{
			CActiveMember ActiveMember;

			if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, ActiveMember))
			{
				//2011-03-14-gxx-�޸�-����ʱ�û�Ϊ����ʱҲ���û���ɾ��
				//
				if (CCurrentNetBarUser::GetInstance()->Balance.GetOpenNetbarId()
					!= CNetBarConfig::GetInstance()->GetNetBarId())
				{
					// ����ɾ��
					CLocalServer::GetInstance()->
						ActiveMemberList.RemoveActiveMember(m_nMemberId);
				}
				else
				{
					ActiveMember.SetActive(FALSE);
					CLocalServer::GetInstance()->
						ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
				}
			}
		}

		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

		OnBnClickedButtonQuery();

		// 2011/10/27-8210-gxx: 
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
		if (nIndex >= 0)
		{
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			theApp.GetRealName()->AuditUserCheckOut(
				computer.GetTerminalID(), computer.GetComputerIP(), m_nMemberId);	
		}

	}
	else
	{
		SetToolTipPos(IDC_BUTTON_CANCELACTIVE);
		ShowToolTip(Activation.GetStatusMessage(), TRUE);
	}
}
