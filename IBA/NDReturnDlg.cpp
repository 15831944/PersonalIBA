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
#include "GlobalFunctions.h"
using namespace IBAGlobal;
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
/*************************************************
Function:       // CNDReturnDlg::OnInitDialog
Description:    // ���г�ʼ���á����ݵ�ǰʵ�����ã���ʾ/����ScanReco(ɨ��)��ť
				// �����localActiveMember��������ʱ����0.1s��
					//����ģʽ:ִ��OnBnClickedButtonQuery��
					//����ģʽ��ѯ�����ð�ť��ʾ�����ݻ�Ա���͵���������ʾ���������ȡ�������m_bNDHasLocalQuery��
					//1.��m_localActiveMember���Dialog,��ʾNetId�ڹ����������m_bNDHasLocalQuery��
					//2.���ð�ʱ,���Ѽ�¼,���ձ���¼��ť
					//3.�����û��������ô��ڱ���,���˰�ť����
					//4.�����û���Ϣ,�ܽ��(��������50ȡ��)(ͬʱ����m_localActiveMember��AvailableBalance)-�����ϼ���Ѻ��
					//5.ˢ����ʾ
				// �粻����localActiveMember������OnGetBalanceOK,���ô��ڵ�m_nMemberId��m_nCardId from CCurrentNetBarUser::Balance����CCurrentNetBarUser��Ϊ�˿�Ի������ø���Աֵ����ˢ����ʾ
					//����ģʽ
					//ΪDialog���ø���Աֵ,���ܽ��(�˻����н��)��ʽ��Ϊ�̶���ʽ������ʾ,(�ô����������������Ľ��е�)
					//���Ѻ��,�ǳ�ʱ��,�û���Ϣ,��������̨�����Ƿ��ӡ,
					//Ϊ��ʱ����ʱ�û����ð�ʱ��Ϣ��ʾ���˿ť��ʾ
					//ˢ�´�����ʾ(��������ֵ) 
Calls:          // OnGetBalanceOK
Table Accessed: // None
Table Updated:  // None
Input:          // None
Output:         // None
Return:         // FALSE. ��ֻҪ����Ҫ���ý�����ĳ���ؼ���ʱ�ŷ���TRUE��
Others:         // None
*************************************************/
BOOL CNDReturnDlg::OnInitDialog()
{
	CNDDialog::OnInitDialog(); //���г�ʼ���á����ݵ�ǰʵ�����ã���ʾ/����ScanReco(ɨ��)��ť

	InitControls(); //��ʼ���ؼ�����

	//2015-0828 liyajun
	if (m_ActiveMember.GetMemberID() > 0) //�����localActiveMember��������ʱ������AutoQuery,m_bAutoQuery=TRUE
	{
		m_bAutoQuery = TRUE;
	}	
	if (m_bAutoQuery)
	{
		SetTimer(TimeIdOfAutoQuery, 100, NULL); //0.1s�󣬱���ģʽ:ִ��OnBnClickedButtonQuery��m_localActiveMember��ֵ��䵽Dialog����Ա��.
	}
	else                                       //�粻����localActiveMember�����ô��ڵ�m_nMemberId��m_nCardId from CCurrentNetBarUser::Balance
	{
		m_nMemberId = CCurrentNetBarUser::GetInstance()->Balance.GetMemberId();
		m_nCardId = CCurrentNetBarUser::GetInstance()->Balance.GetCardId(); 
		OnGetBalanceOK(CCurrentNetBarUser::GetInstance()->Balance); //��CCurrentNetBarUser��Ϊ�˿�Ի������ø���Աֵ����ˢ����ʾ

		SetTimer(TimeIdOfAutoQuery, 800, NULL); //����ģʽ����ʱ��ֻ��һ�������ö��������ý��㶯��
	}	

	GetDlgItem(IDOK)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNDReturnDlg::OnBnClickedButtonCredit()
{
	CBCGPDialog::OnCancel();

	CNDCreditDlg dlg;
	//if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	//{
	//	dlg.SetLocalActiveMember(m_LocalActiveMember);
	//}
	//else
	//{
	dlg.SetNetId(m_strNetId);
	//}
	dlg.DoModal();
}

void CNDReturnDlg::OnBnClickedCancel()
{
	AutoClose();
}

/*************************************************
Function:       // OnBnClickedOk
Description:    // IBA���н��ˣ���������£����б��ؽ��ˣ����˲��ɹ�(��Ҫ��localMember��������ɹ�)�򵯳���ʾ�� 
				// ��������£���Ա����PayoutMember����ʱ�û�֪ͨ�ͻ����»�������DeregisterUserע��
Calls:          // DoLocalModeOperate��PayoutMember��DeregisterUser
Table Accessed: // None
Table Updated: //  None
Input:          // None
Output:         // None
Return:         // None
Others:         // �˿�������˿�
*************************************************/
void CNDReturnDlg::OnBnClickedOk()
{
	CWaitCursor WaitCursor;
	UpdateData();

	CString strErrMsg;
	BOOL bSuccess(FALSE);
	if (m_bIsMember)//��Ա����
	{
		bSuccess = PayoutMember(strErrMsg);
	}
	else//��ʱ�û��˿�
	{
		bSuccess = DeregisterUser(strErrMsg);
	}

	if(bSuccess)//�ɹ�,�˳�
	{
		CBCGPDialog::OnOK();
	}
	else//ʧ��,��ʾʧ����Ϣ
	{
		SetToolTipPos(IDOK);
		ShowToolTip(strErrMsg,TRUE); 
		OnBnClickedButtonQuery();
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

/*************************************************
Function:       //PayoutMember
Description:    //��Ա����.����DocheckOut2,������DoActivation(ACTIVATION_NO).
Calls:          //DoCheckout2 or DoActivation
Return:         //����ʧ�ܣ�����FALSE.
*************************************************/
BOOL CNDReturnDlg::PayoutMember(CString &ErrMsg)
{
	ErrMsg.Empty();

	CWaitCursor WaitCursor;
	CActiveMember ActiveMember;
	if(FALSE == CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId,ActiveMember))
	{
		ErrMsg = _T("�û��ѽ���");
		IBA_LOG(_T("�û��ѽ���"));
		return FALSE;
	}

	//��Ա����
	if (ActiveMember.GetIsOnLine())
	{
		//�˹��ǳ���Ա
		CCheckout2Method Checkout2;
		Checkout2.SetSafeLogout(0); 
		Checkout2.SetMemberId(m_nMemberId);
		BOOL bRes = theApp.GetCurCashier()->DoCheckout2(Checkout2);	

		//�˿�ʧ��,���ش�����Ϣ(����ģʽ��,ֻ�з���-1����ʧ��)
		if(FALSE == bRes)
		{
			if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
			{
				ErrMsg = LOAD_STRING(IDS_ERROR_DBOPERATION);//�����¾������ݿ����ʧ��
				return FALSE;
			}
			else
			{
				if(-1 == Checkout2.GetStatusCode())
				{
					ErrMsg = Checkout2.GetStatusMessage();
					return FALSE;
				}				
			}			
		}

		//ֻҪ���ӳɹ�
		if(TRUE == bRes || -1!= Checkout2.GetStatusCode())
		{
			//�����ն���Ϣ�������ն���ͼ(����û���Ϣ,״̬��Ϊ����)
			INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
			if (nIndex >= 0) 
			{
				if(0 == Checkout2.GetStatusCode() || TRUE == bRes )
				{
					CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId,2);//����Ϊ2,��Ա�»�
				}
				else
				{
					//���û��ѵǳ��������������,��UDO˵���ǻ���.
					CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId,0);//����Ϊ0,����/���㱻�߳�
				}				
				Sleep(1000);
				CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
				computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
				computer.ClearUserInfo();
				CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);
				CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
			}
			//��ӡСƱ
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
		else//����ʧ��,����-1.���ش�����Ϣ
		{
			ErrMsg = Checkout2.GetStatusMessage();
			return FALSE;
		}
	}
	else//��Ա������.ȡ������
	{
		BOOL bSuccess(TRUE);
		//ȡ������,�Ӽ����б���ɾ��,������ͼ
		CActivationMethod Activation;
		Activation.SetMemberId(m_nMemberId);
		Activation.SetMode(CActivationMethod::ACTIVATION_NO);

		//����ģʽ
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			//��ѯLocalMember�Ƿ��м�¼
			CLocalMember lMember;
			BOOL bHasRecord = CIBADAL::GetInstance()->GetLocalMemberByMemberID(m_nMemberId,lMember);
			if(bHasRecord)//�м�¼,�����˿���Ϣ
			{
				COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
				if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(m_nMemberId,CheckoutTime,TRUE))
				{
					IBA_LOG(_T("Error.���±��ؽ��˼�¼ʧ��."));
					bSuccess = FALSE;
				}
			}
			else//�޼�¼,������¼,�����¼,���¼�¼
			{
				GxxDictionaryPtr lmPrt = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(m_nMemberId);  //������¼
				if(lmPrt)
				{
					if(CIBADAL::GetInstance()->InsertNewLocalMember(lmPrt))//�����¼
					{
						COleDateTime CheckoutTime(0,0,0,0,0,0);//ȡ������ʱ,�ǳ�ʱ����Ϊ��Ч
						if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(m_nMemberId,CheckoutTime,TRUE))//���¼�¼
						{
							IBA_LOG(_T("Error.���±��ؽ��˼�¼ʧ��."));
							//����ɹ�������ʧ�ܵģ��Ե����ɹ�
						}	
					}
					else
					{
						IBA_LOG(_T("Error.Ϊ���˲��뱾�ؼ�¼ʧ��."));
						bSuccess =  FALSE;
					}
				}
				else
				{
					IBA_LOG(_T("Error.Ϊ���˴������ؼ�¼ʧ��."));
					bSuccess = FALSE;
				}
			}
		}
		else//����ģʽ
		{
			//ȡ������
			theApp.GetCurCashier()->DoActivation(Activation);
		}

		//if(FALSE == theApp.GetCurCashier()->DoActivation(Activation))
		//{
		//	ErrMsg = Activation.GetStatusMessage();
		//	return FALSE;
		//}

		//����ʵ��������Ҳ����ʵ���»� - ����Ҫ��201160114
		//if(bSuccess)
		//{
		//	if(CIBAGlobal::emRealnameJQ == CNetBarConfig::GetInstance()->GetAuditType())
		//	{
		//		IBA_LOG(_T("����ʵ���»� - �û�������,TermId��IPAddressΪ��"));
		//		CString strTerminalID(_T(""));
		//		CString strComputerIP(_T(""));
		//		theApp.GetRealName()->AuditUserCheckOut(strTerminalID,strComputerIP, m_nMemberId);
		//	}
		//}

		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(m_nMemberId);
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);
	}

	//���������־
	COperationInfo OperationInfo;
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)//����ģʽ - ���ؽ���
	{
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeBrokenReturnVip);
	}
	else
	{
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeVipReturn);
	}		
	OperationInfo.SetNetId(m_strNetId);
	OperationInfo.SetUserName(m_strName);
	OperationInfo.SetDescription(m_strTermId);
	OperationInfo.SetClassId(ActiveMember.GetClassId());
	OperationInfo.SetCheckInTime(ActiveMember.GetCheckInTime());
	OperationInfo.SetIdNum(ActiveMember.GetPersonalID());
	OperationInfo.SetTermId(ActiveMember.GetTerminalID());

	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

	return TRUE;
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

/*************************************************
Function:       // CNDReturnDlg::OnGetBalanceOK
Description:    // ΪDialog���ø���Աֵ,���ܽ��(�˻����н��)��ʽ��Ϊ�̶���ʽ������ʾ(������Ҫ��ʾ��ͬ���),
				// ���Ѻ��,�ǳ�ʱ��,�û���Ϣ,��������̨�����Ƿ��ӡ,
				// Ϊ��ʱ����ʱ�û����ð�ʱ��Ϣ��ʾ���˿ť��ʾ
				// ˢ�´�����ʾ(��������ֵ)
Others:         // Virtual function. 
*************************************************/
void CNDReturnDlg::OnGetBalanceOK(CBalanceMethod & Balance)
{
	CNDDialog::OnGetBalanceOK(Balance); //ΪDialog����Ա��ֵ. ����m_ActiveMember

	if (m_nUserClassID == 1) //��ʱ�û�
	{
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(IDS_NONVIP_RETURN)); //��ʾΪ����ͨ������(&R)		
		SetWindowText(LOAD_STRING(IDS_PAYOUTNONMEMBER)); // ��ʾΪ: ��ͨ������
	}
	else //VIP��Ա
	{		
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(IDS_VIP_RETURN)); //��ʾΪ����Ա������(&R)
		SetWindowText(LOAD_STRING(IDS_PAYOUTMEMBER)); // ��ʾΪ: ��Ա������
	}	
	//����ǰ����û�����ʾΪ�������� 
	if (m_ActiveMember.GetRoomMemberId() == m_nMemberId)
	{
		UINT nID = m_bIsMember?IDS_ROOM_CHECKOUT:IDS_ROOM_RETURN;
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(nID)); //��ʾΪ����������(&R)
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(nID));
		SetWindowText(LOAD_STRING(nID)); 
	}

	ShowBundleTimeInfo(Balance); //Ϊ��ʱ����ʱ�û����ð�ʱ��Ϣ��ʾ���˿ť��ʾ

	m_strUserInfo.Format(_T("%s, %s"), m_strName, m_strUserClass); //�û���Ϣ="�û�����, �û�����"

	m_strDeposit.Format(_T("%.2f"), (double)m_nMoneyGuaranty / 100.0); //Ѻ���ַ���

	m_strLastLogoutTime = Balance.GetLogoutTime(); //�ǳ�ʱ��,yyyymmddhhmmss

	m_nConsumeMoney = Balance.GetLastConsumeAmount(); //�ϴ����ѽ��(��)������ֵΪ0

	//��Ա�������˲������˿��ʾ������Ѻ��
	if(Balance.GetIsMember() || !Balance.GetAllowReturn())
	{
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_BALANCE)); //��ʾΪ�����
		m_strAllBalance.Format(_T("��%.2f"), (double)m_nMoneyAllInAccount / 100.0); //������Ѻ��
	}
	else //�����˿�
	{	
		if(Balance.GetAllowReturn()==2)//ֻ��Ѻ��
		{
			GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_REAL_RETURN)); //��ʾΪ��ʵ��
			m_strAllBalance.Format(_T("��%.2f"), (double)m_nMoneyGuaranty / 100.0); //ֻ��Ѻ��
		}
		else
		{
			GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_REAL_RETURN)); //��ʾΪ��ʵ��
			//m_strAllBalance.Format(_T("��%.2f"), (double)(m_nMoneyAllInAccount + m_nMoneyGuaranty) / 100.0); //����Ѻ��
			//��Ϊ��ʾBalance��returnAmount
			m_strAllBalance.Format(_T("��%.2f"), (double)(Balance.GetReturnAmount() + m_nMoneyGuaranty) / 100.0); //����Ѻ��
		}
	}	

//#ifdef __USING_INTER_RESOURCE
//	m_strAllBalance.Format(_T("��%.2f"), m_nMoneyAllInAccount / 100.0);    //��ʽ�������ʾ
//#else
//	m_strAllBalance.Format(_T("%.2f"), m_nMoneyAllInAccount / 100.0); 
//#endif	

	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, m_ActiveMember); //��ȡ�����û���Ϣ

	//m_bPrintTicket = CIBAConfig::GetInstance()->GetPrintReturnTicket(m_bIsMember); m_nUserClassID == 2
	m_bPrintTicket = CIBAConfig::GetInstance()->GetPrintReturnTicket(m_nUserClassID == 2); 
	
	UpdateData(FALSE); //���ݱ���ֵ�����´�����ʾ

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

/*************************************************
Function:       //DeregisterUser
Description:    //��ʱ�û��˿�.
Return:			//�ɹ�����TRUE,ʧ�ܷ���FALSE
*************************************************/
BOOL CNDReturnDlg::DeregisterUser(CString &ErrMsg,UINT nReturnMode /*= 0*/)
{
	ErrMsg.Empty();
	//�û�����,֪ͨUDO�»�
	if(m_ActiveMember.GetIsOnLine())
	{
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
		if(nIndex >= 0)
		{
			if (m_nMemberId == m_ActiveMember.GetRoomMemberId()) //�����û�,nCheckoutType = 2
			{
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId, 2);
			}
			else												//�ǰ����û�,nCheckoutType = 0
			{
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId, 0);
			}
			Sleep(500);
		}
	}
	//��ʱ�û��˿�
	CDeregisterMethod Deregister;

	Deregister.SetMemberId(m_nMemberId);
	Deregister.SetPersonalId(m_strPersonalID);
	Deregister.SetMemberName(m_strMemberName);
	Deregister.SetCardId(m_nCardId);
	Deregister.SetPassword(m_strPwd);
	Deregister.SetEndUsingTime(m_strLastLogoutTime);

	BOOL bRes = theApp.GetCurCashier()->DoDeregister(Deregister,nReturnMode); 

	//�˿�ʧ��,���ش�����Ϣ
	if(FALSE == bRes)
	{
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			ErrMsg = LOAD_STRING(IDS_ERROR_DBOPERATION);//�����¾������ݿ����ʧ��
		}
		else
		{
			ErrMsg = Deregister.GetStatusMessage();
		}		
		return FALSE;
	}
	else//�ɹ�-���������־
	{
		UINT nAll = 0;
		//����ģʽ,�����˿�
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			nAll = m_nMoneyAllInAccount + m_nMoneyGuaranty;
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeBrokenReturn);//�����˿�
			OperationInfo.SetUserName(m_strName);
			OperationInfo.SetNetId(m_strNetId);
			OperationInfo.SetAllAmount(nAll);
			OperationInfo.SetOperationAmount(m_nMoneyAllInAccount);
			OperationInfo.SetDeposit(m_nMoneyGuaranty);
			OperationInfo.SetDescription(m_strTermId);
			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
		}
		else//����ģʽ,�˿�
		{
			nAll = Deregister.GetBalance() + Deregister.GetGuarantyMoney();//�ϼ�
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeReturn);//�˿�
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

			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
		}

		//����
		CIBATTS::GetInstance()->SpeakReturnResult(nAll);

		//�������Ѵ���
		m_btnConsumeList.ShowWindow(SW_HIDE);                                        

		//��ӡСƱ
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
		//�����ն���Ϣ(����û���Ϣ,ˢ����ͼ) 
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
		if (nIndex >= 0)                                                                      
		{
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			computer.ClearUserInfo();
			CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);
			CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
		}
	}
	return TRUE;
}

void CNDReturnDlg::SetNetId(CString newVal)
{
	m_strNetId = newVal;

	m_bAutoQuery = TRUE;
}

/*************************************************
Function:       //OnTimer
Description:    //ִ��ָ������.ִ���꣬���ٶ�ʱ��.
				//����ģʽ:ִ��OnBnClickedButtonQuery��
				-			//����ģʽ��ѯ�����ð�ť��ʾ�����ݻ�Ա���͵���������ʾ���������ȡ�������m_bNDHasLocalQuery��
							//1.��m_localActiveMember���Dialog,��ʾNetId�ڹ����������m_bNDHasLocalQuery��
							//2.���ð�ʱ,���Ѽ�¼,���ձ���¼��ť
							//3.�����û��������ô��ڱ���,���˰�ť����
							//4.�����û���Ϣ,�ܽ��(��������50ȡ��)(ͬʱ����m_localActiveMember��AvailableBalance)-�����ϼ���Ѻ��
							//5.ˢ����ʾ
				//����ģʽ:�Ӿ���ʵ���������ý���
Calls:          //OnBnClickedButtonQuery
Table Accessed: //None
Table Updated:  //None
Input:          //nIDEvent - ��ʱ��ID��ֻ����TimeIdOfAutoQuery���Ҵ����������
Output:         //None
Return:         //None
Others:         //None
*************************************************/
void CNDReturnDlg::OnTimer(UINT nIDEvent)
{
	if (TimeIdOfAutoQuery == nIDEvent) //�Զ���ѯ��ʱ��
	{
		KillTimer(nIDEvent);

		if (m_bAutoQuery)    //����ģʽ-����ģʽ��OnInitDialog������
		{
			m_bAutoQuery = FALSE;
			OnBnClickedButtonQuery();  
			//����ģʽ��ѯ�����ð�ť��ʾ�����ݻ�Ա���͵���������ʾ���������ȡ�������m_bNDHasLocalQuery��
			//1.��m_localActiveMember���Dialog,��ʾNetId�ڹ����������m_bNDHasLocalQuery��
			//2.���ð�ʱ,���Ѽ�¼,���ձ���¼��ť
			//3.�����û��������ô��ڱ���,���˰�ť����
			//4.�����û���Ϣ,�ܽ��(��������50ȡ��)(ͬʱ����m_localActiveMember��AvailableBalance)-�����ϼ���Ѻ��
			//5.ˢ����ʾ
		}
		else
		{
			if (theApp.GetRealName()->RobFocusAfterSwipingCard()) //��ʹ�õ�ʵ����������ˢ�����Զ����ý��㣬�Զ��ƶ�����Լ����ý���
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

/*************************************************
Function:       // ShowBundleTimeInfo
Description:    // Ϊ��ʱ�û�����"��ʱ��Ϣ"��ʾ��������ʱ�û�ʱ"�˰�ʱ�����"��ť��ʾ������(��ʱ�û����+Ѻ��>��ʱ���ü���ʾ)
Calls:          // ..
Table Accessed: // None
Table Updated:  // None
Input:          // Balance - ���е���Ϣ�������ð�ʱ��Ϣ,�жϽ��
Output:         // ..
Return:         // None
Others:         // None
*************************************************/
void CNDReturnDlg::ShowBundleTimeInfo(CBalanceMethod & Balance)
{
	if (Balance.GetPayType() > 0)  //���������ͨ�Ʒ�,i.e.�ۼƷⶥ�����Ԥ��ʱ�Ž�����������
	{
		COleDateTime StartTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetStartTime());
		COleDateTime EndTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime());

		COleDateTimeSpan dtSpan;

		if (COleDateTime::GetCurrentTime() < StartTime)   //��û����ʱ��ʼʱ��,ʣ��ʱ��Ϊ��ʱʱ��
		{
			dtSpan = EndTime - StartTime;
		} 
		else                                              //�Ѿ���ʼ�˰�ʱ,ʣ��ʱ��Ϊ��ʱ����ʱ��-��ǰʱ��
		{
			dtSpan = EndTime - COleDateTime::GetCurrentTime();
		}

		m_strBundleTimeInfo.Format(_T("%s �� %s��ʣ�� %d:%.2d\r\n\r\n���(%.1fԪ)"),   //���ð�ʱʱ����Ϣ������ʾ��IDC_STATIC_BTENDTIME(��ʱ��Ϣ��)
			StartTime.Format(_T("%H:%M")),    
			EndTime.Format(_T("%H:%M")),
			(LONG)dtSpan.GetTotalHours(), dtSpan.GetMinutes(), 
			Balance.GetPrice() / 100.0       //��ʱ���
			);

		//��ʾ����ť
		//if (!m_bIsMember && (m_nAllMoney - Balance.GetPrice() > 0)) 
		if ((m_nUserClassID==1) && (m_nMoneyAllInAccount - Balance.GetPrice() > 0)) //��ʱ�û����ܽ��>��ʱ����ʾ"�˰�ʱ�����"��ť
		{
			m_btnReturn2.ShowWindow(SW_NORMAL);
		}
		else                                                        //��������"�˰�ʱ�����"��ť
		{
			m_btnReturn2.ShowWindow(SW_HIDE);
		}
	}
}

//�˰�ʱ�����
void CNDReturnDlg::OnBnClickedButtonReturn2()
{
	if (IDOK != MsgBox(IDS_RETURN2QUESTION, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}
	UINT nReturnMode = 2;//0:�����˿� 1:�˿���� 2:�˰�ʱ�����

	//CDeregisterMethod Deregister;

	//Deregister.SetMemberId(m_nMemberId);
	//Deregister.SetPersonalId(m_strPersonalID);
	//Deregister.SetMemberName(m_strMemberName);
	//Deregister.SetCardId(m_nCardId);
	//Deregister.SetPassword(m_strPwd);
	//Deregister.SetEndUsingTime(m_strLastLogoutTime);
	//Deregister.SetReturnMode(2);

	//theApp.GetCurCashier()->DoDeregister(Deregister,nReturnMode);

	//if (Deregister.GetStatusCode() == 0)
	//{	
	//	UINT nAll = Deregister.GetBalance();

	//	//{ 2011/06/30-gxx: 
	//	CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(m_nMemberId,0,0,nAll);
	//	
	//	//}
	//	
	//	CIBATTS::GetInstance()->SpeakReturn2Result(nAll);//����

	//	//д������־
	//	COperationInfo OperationInfo;
	//	OperationInfo.SetOperationType(8);
	//	OperationInfo.SetIsBundTime(FALSE);
	//	OperationInfo.SetUserName(m_strName);
	//	OperationInfo.SetNetId(m_strNetId);
	//	OperationInfo.SetMemberId(m_nMemberId);
	//	OperationInfo.SetOperationAmount(0);
	//	OperationInfo.SetAllAmount(nAll);
	//	OperationInfo.SetDeposit(Deregister.GetGuarantyMoney());
	//	OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
	//	OperationInfo.SetDescription(m_strTermId);
	//	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);

	//	// 2014-10-10 - qsc
	///*	if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
	//	{
	//		NS_REALNAME::CheckoutInfo coi;
	//		ZeroMemory(&coi, sizeof(coi));
	//		_tcsncpy(coi.szName, m_strName, 100);
	//		_tcsncpy(coi.szNetId, m_strNetId, 100);
	//		_tcsncpy(coi.szTermId, m_ActiveMember.GetTerminalID(), 50);

	//		CString strIP;
	//		CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
	//		_tcsncpy(coi.szIP, strIP, 50);
	//		coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

	//		theApp.GetRealName()->CheckOut(&coi);
	//	}*/	
	CString strErrMsg;
	if(DeregisterUser(strErrMsg,nReturnMode))
	{
		UINT nAll = 0;
		nAll = m_nMoneyAllInAccount + m_nMoneyGuaranty;
		ReQuery();		

		SetToolTipPos(IDC_BUTTON_RETURN2);
		ShowToolTipFormat(IDS_RETURN2INFO, nAll / 100.0);
	}
	else
	{	
		SetToolTipPos(IDOK);
		ShowToolTip(strErrMsg, TRUE);
	}
}

//void CNDReturnDlg::OnCenterDisconnect()
//{
//	//m_bCenterDisconnect = TRUE;
//
//	CActiveMember ActiveMember;
//
//	BOOL bRes = CLocalServer::GetInstance()
//		->ActiveMemberList.GetActiveMemberNetID(m_strNetId,ActiveMember);
//
//	m_strName.Format(_T("%s(%s)"), ActiveMember.GetUserName(), CIBAHelpper::GetSexAsString(ActiveMember.GetSex()));
//	m_strPersonalID = ActiveMember.GetPersonalID();
//
//	m_strMemberName = ActiveMember.GetUserName();
//
//#ifdef __USING_INTER_RESOURCE
//	m_strAllBalance.Format(_T("��%.2f"), (double)ActiveMember.GetAvailavleBalance() / 100.0);
//#else
//	m_strAllBalance.Format(_T("%.2f"), (double)ActiveMember.GetAvailavleBalance() / 100.0);
//#endif
//
//	m_strOpenNetBar = CNetBarConfig::GetInstance()->GetNetBarName(ActiveMember.GetNetbarID());
//
//	m_strTermId = ActiveMember.GetTerminalID();
//
//	m_strUserInfo = m_strMemberName;
//	
//	if (ActiveMember.IsMember())
//	{
//		GetDlgItem(IDC_STATIC0)->SetWindowText(_T("���(Ԫ)��"));
//	}
//
//	UpdateData(FALSE);
//}

//2015-0828 liyajun
/*************************************************
Function:       // DoLocalModeReturn
Description:    // ���ؽ���.��ӡСƱ
Calls:          // CCashierHelper::ReturnLocalCenterUser
Return:         // CCashierHelper::ReturnLocalCenterUser
*************************************************/
//BOOL CNDReturnDlg::DoLocalModeReturn()
//{
//	if (m_strNetId.IsEmpty())
//	{
//		IBA_LOG(_T("Abnormal .CNDReturnDlg::DoLocalModeReturn: NetId is Empty!"));
//		return FALSE;
//	}
//
//	BOOL bRet = CCashierHelper::ReturnLocalCenterUser(m_LocalActiveMember);	
//
//	//��ӡСƱ
//	//if(bRet)
//	//{
//	//	PrintTicket(m_ActiveMember);		
//	//}
//	return bRet;
//}

void CNDReturnDlg::OnBnClickedButtonBundletime()
{
	CBCGPDialog::OnCancel();

	CDlgBundleTime dlg;
	dlg.SetNetId(m_strNetId, TRUE);
	dlg.DoModal();
}

/*************************************************
Function:       //CNDReturnDlg::DoLocalModeQuery
Description:    //����ģʽ��ѯ�����ð�ť��ʾ�����ݻ�Ա���͵���������ʾ���������ȡ�������m_bNDHasLocalQuery��
				//1.��m_localActiveMember���Dialog,��ʾNetId�ڹ����������m_bNDHasLocalQuery��
				//2.���ð�ʱ,���Ѽ�¼,���ձ���¼��ť
				//3.�����û��������ô��ڱ���,���˰�ť����
				//4.�����û���Ϣ,�ܽ��(��������50ȡ��)(������Ҫ��������)
				//5.ˢ����ʾ
Calls:          //DoLocalModeQuery,UpdateDate(FALSE),�ػ洰��.
Table Accessed: //None
Table Updated:  //None
Input:          //None
Output:         //None
Return:         //
Others:         //��GetBalanceOk���ܳ�ͻ����������GetBalanceOk��������ģʽ��ִ�С�
*************************************************/
BOOL CNDReturnDlg::DoLocalModeQuery()
{
	//m_bCenterDisconnect = TRUE;  //���øú���������Ϊ�������ѶϿ����ӡ�

	//CActiveMember ActiveMember;
	//����ģʽ��ȡm_LocalActiveMember�ĸ�ֵ,����ģʽor�ָ�ģʽ��,ȡ��ǰNetId��ActiveMember
	//if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	//{
	//	ActiveMember = m_LocalActiveMember;
	//}
	//else
	//{
	//	UINT nMemberId;
	//	CIBADAL::GetInstance()->GetMemberId(m_strNetId,nMemberId);
	//	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
	//}

	if (!CNDDialog::DoLocalModeQuery()) //��m_localActiveMember��ֵ��䵽Dialog����Ա.��NetId��ʾ�ڹ������С����m_bNDHasLocalQuery=TRUE��
	{
		return FALSE;
	}
	
	if(FALSE == CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId,m_ActiveMember)){return FALSE;}
	// ���ð�ʱ,���Ѽ�¼,���ձ���¼��ť - ����ʱ
	m_btnBundleTime.ShowWindow(FALSE);     // ���ð�ʱ����
	m_btnConsumeList.ShowWindow(FALSE);    // �������Ѽ�¼
	m_btnZSBConsumeList.ShowWindow(FALSE); // �������ձ���¼

	//�����û��������ô��ڱ���,���˰�ť����
	if (m_ActiveMember.IsMember())
	{
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(IDS_VIP_RETURN)); //��ʾΪ����Ա����(&R)
		SetWindowText(LOAD_STRING(IDS_PAYOUTMEMBER)); // ��ʾΪ: ��Ա����
	}
	else 
	{
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(IDS_NONVIP_RETURN)); //��ʾΪ����ͨ������(&R)
		SetWindowText(LOAD_STRING(IDS_PAYOUTNONMEMBER)); // ��ʾΪ: ��ͨ������
	}
	//����ǰ����û�����ʾΪ�������� 
	if (m_ActiveMember.GetRoomMemberId() == m_nMemberId)
	{
		UINT nID = m_bIsMember?IDS_ROOM_CHECKOUT:IDS_ROOM_RETURN;
		GetDlgItem(IDOK)->SetWindowText(LOAD_STRING(nID)); //��ʾΪ����������(&R)
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(nID));
		SetWindowText(LOAD_STRING(nID)); 
	}

	//ShowBundleTimeInfo(Balance); //Ϊ��ʱ����ʱ�û����ð�ʱ��Ϣ��ʾ���˿ť��ʾ

	m_strUserInfo.Format(_T("%s, %s"), m_strName, m_strUserClass); //�û���Ϣ="�û�����, �û�����"

	//m_strLastLogoutTime = Balance.GetLogoutTime(); //�ǳ�ʱ��,yyyymmddhhmmss
	//m_nConsumeMoney = Balance.GetLastConsumeAmount(); //�ϴ����ѽ��(��)������ֵΪ0

	m_strDeposit.Format(_T("%.2f"), (double)m_nMoneyGuaranty / 100.0); //Ѻ���ַ���

	//ȡ�Ƿ������˿������
	UINT nAllowReturn;
	CString sql;
	sql.Format(_T("select allowReturn from member where memberId=%d"),m_ActiveMember.GetMemberID());
	GxxDictionaryPtr MemberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);
	if(!MemberPtr)
	{
		IBA_LOG(_T("Error! CNDReturnDlg::DoLocalModeQuery Desp:Access DB Error! Proc: Read allowReturn from Member for MemberId = %d"),m_ActiveMember.GetMemberID());
		GXX_ASSERT(0,"CNDReturnDlg::DoLocalModeQuery��Member�������ݿ��ȡ�˿�����ʧ�ܣ�");
		nAllowReturn = 1;
	}
	else
	{
		nAllowReturn = MemberPtr->valueForKey("allowReturn")->intValue();
	}	

	UINT uAvailableBalance = m_ActiveMember.GetAvailavleBalance();//ȡ�������°�50ȡ������ֻ��ʾ������
	uAvailableBalance -= uAvailableBalance % 50;
	if (uAvailableBalance < 0)
	{
		uAvailableBalance = 0;
	}
	//��Ա�������˲������˿��ʾ������Ѻ��
	if(m_bIsMember || nAllowReturn == 0)
	{
		GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_BALANCE));// ��ʾΪ�����:
		m_strAllBalance.Format(_T("��%.2f"), (double)uAvailableBalance / 100.0); //������Ѻ��
	}
	else //�����˿�
	{	
		if(nAllowReturn==2)//ֻ��Ѻ��
		{
			GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_REAL_RETURN)); //��ʾΪ��ʵ��
			m_strAllBalance.Format(_T("��%.2f"), (double)m_nMoneyGuaranty / 100.0); //ֻ��Ѻ��
		}
		else
		{
			GetDlgItem(IDC_STATIC0)->SetWindowText(LOAD_STRING(IDS_REAL_RETURN)); //��ʾΪ��ʵ��
			m_strAllBalance.Format(_T("��%.2f"), (double)(uAvailableBalance + m_nMoneyGuaranty) / 100.0); //����Ѻ��
		}
	}

	m_bPrintTicket = CIBAConfig::GetInstance()->GetPrintReturnTicket(m_nUserClassID == 2); 

//#ifdef __USING_INTER_RESOURCE
//	m_strAllBalance.Format(_T("��%.2f"), nTmp / 100.0);
//#else
//	m_strAllBalance.Format(_T("%.2f"), nTmp / 100.0);
//#endif

	UpdateData(FALSE); //���´���������ʾ

	RedrawWindow();
	return TRUE;
}

/*************************************************
Function:       //DoLocalModeOperate
Description:    //���NetId��m_localActiveMember�����ؽ���m_localActiveMember,���»����localMember��¼(isReturned),֪ͨ�ն��»��������û���ͼ���ն���ͼ
Calls:          //DoLocalModeReturn 
Return:         //���ؽ��˳ɹ�����TRUE�����򷵻�FALSE 
*************************************************/
//BOOL CNDReturnDlg::DoLocalModeOperate()
//{
//	if (m_strNetId.IsEmpty() || m_LocalActiveMember.GetMemberID() == 0)
//	{
//		IBA_ASSERT2(0, "��Ч�ı����û�");
//		IBA_LOG(_T("Abnormal.CNDReturnDlg::DoLocalModeOperate:NoNetId or no MemberId=0 in m_localActiveMember!"));
//		return FALSE;
//	}
//
//	return DoLocalModeReturn(); //���ؽ���m_localActiveMember,���»����localMember��¼(isReturned),֪ͨ�ն��»��������û���ͼ���ն���ͼ
//}

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
				Sleep(500);
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
			OperationInfo.SetAllAmount(m_nMoneyAllInAccount);
			OperationInfo.SetOperationTime(m_ActiveMember.GetActivationTime());
			CString strTmp;
			strTmp.Format(LOAD_STRING(IDS_ROOM_BODY), arrMembers.GetCount());
			OperationInfo.SetDescription(strTmp);
			OperationInfo.SetClassId(m_ActiveMember.GetClassId());
			OperationInfo.SetTermId(m_ActiveMember.GetTerminalID());
			OperationInfo.SetIdNum(m_ActiveMember.GetPersonalID());
			OperationInfo.SetCheckInTime(m_ActiveMember.GetCheckInTime());
			OperationInfo.SetConsumeMoney(m_ActiveMember.GetAdvancePayment()-m_nMoneyAllInAccount);

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

void CNDReturnDlg::PrintTicket(CActiveMember& ActiveMember)
{
	if (m_bPrintTicket)                            
	{
		if(ActiveMember.GetClassId() == 1)  //��ʱ�û�
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
			strTicket.Replace(_T("[totalmoney]"), _T("0.00")); // ��ʱ�û����Ϊ0

			CIBAPrint::Print(strTicket);
		}
		else if(ActiveMember.GetClassId() == 2)
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
}
