// CreditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "MainFrm.h"
#include "Cashier.h"
#include "IBAPrint.h"
#include "IBAHelpper.h"
#include "ServletHellper.h"
#include "dal\IBADAL.h"
#include "Socket\LocalServer.h"
#include "RealName\IBARealName.h"
#include "tts\IBATTS.h"
#include ".\creditdlg.h"
#include <math.h>
#include "CheckMemberDlg.h"
#include "BundleTimeListDlg.h"
#include "ComputerListView.h"
#include "ActiveMemberView.h"
#include "CashierHelper.h"

// CCreditDlg �Ի���

HWND CCreditDlg::m_StaticPrehWnd = NULL;

IMPLEMENT_DYNAMIC(CCreditDlg, CNDDialog)
CCreditDlg::CCreditDlg(CWnd* pParent /*=NULL*/)
: CNDDialog(CCreditDlg::IDD, pParent)
, m_bNoPresent(FALSE), m_bUserHasCard(FALSE)
, m_bPrint(FALSE), m_nAllMoney(0)
, m_bFromCard(FALSE), m_nCreditMoney(0), m_bForReturn(FALSE)
, m_nBundleTimeId(0), m_nPresentMoney(0), m_bAlreadyBundTime(FALSE)
, m_nBundTimeClass(0)
{
	m_bHasLocalQuery = FALSE;
}

CCreditDlg::~CCreditDlg()
{
}

BEGIN_MESSAGE_MAP(CCreditDlg, CNDDialog)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REFUNDMENT, OnBnClickedButtonRefundment)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CHECKOUT, OnBnClickedButtonCheckout)
	ON_MESSAGE(WM_RECEIVECARD, OnRealName)
	ON_BN_CLICKED(IDC_BUTTON_CONSUMELIST, OnBnClickedButtonConsumelist)
	ON_BN_CLICKED(IDC_BUTTON_REPRINT, OnBnClickedButtonReprint)
	ON_BN_CLICKED(IDC_BUTTON_BUNDTIME, OnBnClickedButtonBundtime)
	ON_EN_CHANGE(IDC_EDIT_IDCARDSNO, OnEnChangeEditIdcardsno)
	ON_BN_CLICKED(IDC_BUTTON_CANCELACTIVE, OnBnClickedButtonCancelactive)
	ON_BN_CLICKED(IDC_BUTTON_CREDITPRINT, OnBnClickedButtonCreditprint)
END_MESSAGE_MAP()

void CCreditDlg::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CREDIT, m_strCreditMoney);
	DDX_Text(pDX, IDC_STATIC_USERNAME, m_strName);
	DDX_Text(pDX, IDC_STATIC_MEMBERID, m_strMemberID);
	DDX_Text(pDX, IDC_STATIC_NETBAR, m_strNetBar);
	DDX_Text(pDX, IDC_STATIC_PERSONALID, m_strPersonalID);
	DDX_Text(pDX, IDC_STATIC_CLASS, m_strUserClass);
	DDX_Text(pDX, IDC_STATIC_YSCZ, m_strBalanceY);
	DDX_Text(pDX, IDC_STATIC_WSCZ, m_strBalanceW);
	DDX_Text(pDX, IDC_STATIC_ZSZH, m_strBalanceZ);
	DDX_Text(pDX, IDC_STATIC_POINT, m_strPoint);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_Text(pDX, IDC_STATIC_ZTJE, m_strGiveBack);
	DDX_Text(pDX, IDC_STATIC_RESULT, m_strResult);
	DDX_Text(pDX, IDC_STATIC_DEPOSIT, m_strDeposit);
	DDX_Check(pDX, IDC_CHECK_PRESENT, m_bNoPresent);
	DDX_Check(pDX, IDC_CHECK_USERHASCARD, m_bUserHasCard);
	DDX_Control(pDX, IDC_EDIT_CREDIT, m_edtCreditMoney);
	DDX_Control(pDX, IDC_EDIT_IDCARDSNO, m_edtNetId);
	DDX_Text(pDX, IDC_STATIC_BUNDTINELIMIT, m_strBundTimeLimit);
	DDX_Text(pDX, IDC_STATIC_BUNDTIMEMONEY, m_strBundTimeMoney);
	DDX_Text(pDX, IDC_STATIC_BTREMAINTIME, m_strBTRemainTime);
	DDX_Text(pDX, IDC_STATIC_ONLINEAMOUNT, m_strOnlineAmount);
	DDX_Control(pDX, IDC_BUTTON_CONSUMELIST, m_btnConsumeList);
	DDX_Control(pDX, IDC_BUTTON_CANCELACTIVE, m_btnCancelActivation);
}

BOOL CCreditDlg::OnInitDialog()
{
	CNDDialog::OnInitDialog();

	InitControls();

	//{ 2011/05/24-gxx: �Զ���ѯ��ˢ����ɨ��֤��������²���
	if (m_bAutoQuery)
	{
		SetTimer(TimeIdOfAutoQuery, 100, NULL);//ֻ��ѯһ��
	}
	else
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_IDCARDSNO);
		ASSERT(pWnd);
		if (pWnd)
		{
			pWnd->SetFocus();
		}
	}
	//}

	UpdateData(FALSE);
	//�����Ѿ�����
	m_StaticPrehWnd = GetSafeHwnd();

	return FALSE; 
}

//�˿�
void CCreditDlg::OnBnClickedButtonRefundment()
{
	//���Ĳ�ѯ���жϲ������˿����������Ѿ���ֹ������
 	if (!GetDlgItem(IDC_BUTTON_REFUNDMENT)->IsWindowEnabled())
 	{
 		return;
 	}

	CIBALog::GetInstance()->Write(_T("CCreditDlg::OnBnClickedButtonRefundment"));

	CWaitCursor WaitCursor;

	UpdateData();

	// 2011/07/28-8201-gxx: 
	if (m_bHasLocalQuery)
	{
		if (m_LocalActiveMember.GetIsLocalUser())
		{
			CCashierHelper::ReturnLocalUser(m_LocalActiveMember);
		}
		else
		{
			CCashierHelper::ReturnLocalCenterUser(m_LocalActiveMember);
		}
	}
	else
	{
		//���Դ��ڡ��û����ߡ�״̬����ǿ��Զ���»�
		if (m_bOnLine)
		{
			OnBnClickedButtonCheckout();
		}
		//��֤�Ѿ��ɹ��»�
		if (!MakeSureDReg()) return;
		/////////////////////////////ע��//////////////////////
		if (m_bOnLine)
		{	//ע��ǰ��״̬����
			CCheckout2Method Checkout2;
			Checkout2.SetSafeLogout(0); //�۵���ǰʱ��
			Checkout2.SetMemberId(m_nMemberId);
			theApp.GetCurCashier()->DoCheckout2(Checkout2);	
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
		}
		//ע���û�
		DeregisterUser();
	}

	
}

//��ֵ
void CCreditDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (CheckReturnAction())  return;//����س�����������

	// 2011/06/29-gxx: �Ƿ���Գ�ֵ, �һ��в�������
	if (!CLocalServer::GetInstance()->ActiveMemberList.IsAllowCredit(m_nMemberId))
	{
		return;
	}

	
	//û�а�ʱ
	if (0 == m_nBundleTimeId)
	{
		DoCredit();
	}
	else
	{
		
		//UINT nCreditMoney = _tstof(m_strCreditMoney) * 100, nBundTimeMoney = _tstof(m_strBundTimeMoney) * 100;
		UINT nCreditMoney = CIBAHelpper::DecT2Int(m_strCreditMoney), nBundTimeMoney = CIBAHelpper::DecT2Int(m_strBundTimeMoney);
		
		//�����ȳ�ֵʱ����ֵ�������ڰ�ʱ���������ʱ
		if (nCreditMoney >= nBundTimeMoney)
		{
			//ִ�г�ֵ����
			if (DoCredit())
			{//�ȳ�ֵ�ɹ��󣬺��ʱ
				DoBundleTime();
			}
		}
		else
		{
			SetToolTipPos(IDC_EDIT_CREDIT);
			ShowToolTip(IDS_CREDITBEFOREBT);
		}
	}
}

BOOL CCreditDlg::CheckInput()
{
	//��֤��Ǯ����
	UpdateData();

	//INT nMoney = _tstof(m_strCreditMoney) * 100;
	INT nMoney = CIBAHelpper::DecT2Int(m_strCreditMoney);

	if (nMoney <= 0)
	{
		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(IDS_INPUTERROR);
		GetDlgItem(IDC_EDIT_PWD)->SetFocus();
		return FALSE;
	}

	// 2012/03/05-8238-gxx: 
	if (CIBAConfig::GetInstance()->GetMaxCreditMoney() != 0)
	{
		INT nMax = CIBAConfig::GetInstance()->GetMaxCreditMoney()*100;
		if (nMoney > nMax)
		{
			ShowToolTip(IDS_OVERFLOW_MAXMONEY, IDC_EDIT_CREDIT, TRUE);
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CREDIT);
			if (pEdit)
			{
				pEdit->SetFocus();
				pEdit->SetSel(0, m_strCreditMoney.GetLength());
			}

			return FALSE;
		}
	}
	// --end

	return TRUE;
}

void CCreditDlg::InitControls()
{
	GetDlgItem(IDC_STATIC_RESULT)->SetFont(&m_Font);

	CIBAHelpper::CreateFont(m_BigFont, 22, 900);
	m_edtCreditMoney.SetFont(&m_BigFont);
	m_edtCreditMoney.DisableMask();
	m_edtCreditMoney.SetValidChars(_T("0123456789."));
	m_edtCreditMoney.SetLimitText(4);

	GetDlgItem(IDC_STATIC_ZTJE)->SetFont(&m_BigFont);
	GetDlgItem(IDC_EDIT_PWD)->SetFont(&m_BigFont);
	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFont(&m_BigFont);
	GetDlgItem(IDC_BUTTON_QUERY)->ShowWindow(SW_SHOWNOACTIVATE);
	GetDlgItem(IDC_CHECK_USERHASCARD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CHECKOUT)->ShowWindow(SW_HIDE);
	//��ֵ�˿�
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REFUNDMENT)->EnableWindow(!m_strMemberID.IsEmpty());
	//��ֵ��ӡ
	GetDlgItem(IDC_BUTTON_CREDITPRINT)->EnableWindow(!m_strMemberID.IsEmpty());

	AddFocusEnter(IDC_EDIT_CREDIT, IDOK);
	AddFocusEnter(IDC_EDIT_PWD, IDC_BUTTON_REFUNDMENT);
	//�ظ���ӡ��ť����
	GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	//��ѯ�û����Ѱ�ť����
	GetDlgItem(IDC_BUTTON_CONSUMELIST)->ShowWindow(SW_HIDE);
	// ȡ������
	GetDlgItem(IDC_BUTTON_CANCELACTIVE)->ShowWindow(SW_HIDE);
	//��ʱ��ť����
	GetDlgItem(IDC_BUTTON_BUNDTIME)->EnableWindow(FALSE);

}

void CCreditDlg::ClearData()
{
	//////////////��ʱ����////////////
	m_bAlreadyBundTime = FALSE;
	m_nBundleTimeId = 0;
	m_nBundTimeClass = 0;
	m_strBundleTimeArea.Empty();
	m_strBundleTimeEndTime.Empty();
	m_strBundleTimeStartTime.Empty();

	m_strBundTimeMoney.Empty();
	m_strBundTimeLimit.Empty();
	m_strBTRemainTime.Empty();
	////////////////////////////
	m_strCreditMoney.Empty();
	m_strName.Empty();
	m_strMemberID.Empty();
	m_strNetBar.Empty();
	m_strPersonalID.Empty();
	m_strUserClass.Empty();
	m_strAllBalance.Empty();
	m_strBalanceY.Empty();
	m_strBalanceW.Empty();
	m_strBalanceZ.Empty();
	m_strOnlineAmount.Empty();
	m_strPwd.Empty();
	m_strGiveBack.Empty();
	m_strDeposit.Empty();
	m_bNoPresent = FALSE;
	m_bUserHasCard = FALSE;
	m_strResult.Empty();
	m_strPoint.Empty();
	m_bOnLine = FALSE;
	m_strPwd.Empty();
	m_strTmpUserPwd.Empty();

	//���²�ѯ���ظ���ӡ���ܽ�ֹ
	m_nCreditMoney = 0;
	//��ӡ���ָܻ�
	m_bPrint = FALSE;

	GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CREDITPRINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BUNDTIME)->EnableWindow(FALSE);
	//�˿ťÿ�β�ѯ�����½�ֹ
	GetDlgItem(IDC_BUTTON_REFUNDMENT)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

INT_PTR CCreditDlg::DoModal()
{	
	INT_PTR nRet = FALSE;
	
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		//��д������յ�����־
		nRet = CNDDialog::DoModal();
		m_StaticPrehWnd = NULL;
	}

	return nRet;
}
//ѯ���Ƿ񵯳���ʾ��ֵ
BOOL CCreditDlg::MakeSureCredit(UINT nMoney)
{
	BOOL bRet = TRUE;
	//ѯ�ʱ����Ƿ񵯳���ֵ�Ի���
	if (!CIBAConfig::GetInstance()->GetPopMsgBox())
	{
		return bRet;
	}

	CString strTmp;

	strTmp.Format(IDS_CREDITQUESTION, m_strName, nMoney / 100.0);

	if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
	{
		bRet = FALSE;
	}

	return bRet;
}
//��֤�˿�
BOOL CCreditDlg::MakeSureDReg()
{
	BOOL bRet = TRUE;

	if (m_bOnLine) //�����߾ͱ�ʾԶ���»�ʧ��
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_FORCERETURN);

		if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
		{
			bRet = FALSE;
		}
	}
	else
	{
		if (!CIBAConfig::GetInstance()->GetPopMsgBox())
		{
			return bRet;
		}

		CString strTmp;
		strTmp.Format(IDS_DREGQUESTION, m_strName);

		if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
		{
			bRet = FALSE;
		}
	}

	return bRet;
}

void CCreditDlg::PrintTicket(CString strCreditMoney)
{	
	CString strTicket;
	CIBAPrint::GetCreditTicket(strTicket);

	strTicket.Replace(_T("[name]"), m_strName);
	strTicket.Replace(_T("[idnumber]"), m_strPersonalID);
	strTicket.Replace(_T("[memberclass]"), m_strUserClass);
	strTicket.Replace(_T("[inmoney]"), strCreditMoney);
	strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);
	//ʵ��Ҫ��ǿ�����룬�򲻴�ӡ����
	if (CNetBarConfig::GetInstance()->GetRealNamePassword() >= 2)
	{
		strTicket.Replace(_T("[password]"), _T("*****"));
	}
	else
	{
		strTicket.Replace(_T("[password]"), m_strTmpUserPwd);
	}

	strTicket.Replace(_T("[netid]"), m_strNetId.Right(8));
	strTicket.Replace(_T("[memberid]"), m_strMemberID);
	strTicket.Replace(_T("[totalmoney]"), m_strGiveBack);

	CIBAPrint::Print(strTicket);
}

void CCreditDlg::OnOldCard(UINT nCardId, UINT nMemberId)
{	
	QueryBalance(nMemberId, nCardId);
	
	UpdateData(FALSE);
}

void CCreditDlg::OnGetBalanceOK(CBalanceMethod & Balance)
{
	m_bHasLocalQuery = FALSE; // 2011/07/28-8201-gxx: Ϊ�Ǳ��ز�ѯ

	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId,m_ActiveMember);

	ShowUserInfo(Balance);
	ShowBundleTimeInfo(Balance);
	ShowControls(Balance);
	AutoDoRefundment(Balance);
	
	if (m_bForReturn) //���˿�ǳ�ֵ�����㲻һ��
	{
		m_bForReturn = FALSE;

		GetDlgItem(IDC_EDIT_PWD)->SetFocus();
	}
	else
	{
		m_edtCreditMoney.SetFocus();
	}
}

//�ǳ�
void CCreditDlg::OnBnClickedButtonCheckout()
{
	//Ѱ�Ҽ����
	INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

	if (nIndex >= 0)
	{	
		CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId);

		if (!GetDlgItem(IDC_BUTTON_CHECKOUT)->IsWindowVisible())  return;//��ʹ�ܣ���ִ������

		Sleep(2000);

		m_bOnLine = FALSE;
		ReQuery();//����ͳ�ƣ�����״̬
		UpdateData(FALSE);
	}
	else
	{
		if (!GetDlgItem(IDC_BUTTON_CHECKOUT)->IsWindowVisible())  return;//��ʹ�ܣ���ִ������
	}

	if (m_bOnLine) //�����߾ͱ�ʾԶ���»�ʧ��
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_REMOTECHECKOUTFIALED);
		
		if (MsgBox(strTmp, MB_ICONINFORMATION | MB_OKCANCEL) == IDOK)
		{	
			//ִ���쳣�»�
			CWaitCursor WaitCursor;
			UpdateData();
			/////////////////////////////ע��//////////////////////
			//ע��ǰ��״̬����
			CCheckout2Method Checkout2;
			Checkout2.SetSafeLogout(0); //�۵���ǰʱ��
			Checkout2.SetMemberId(m_nMemberId);
			theApp.GetCurCashier()->DoCheckout2(Checkout2);		
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

			//���²�ѯ״̬
			m_bOnLine = FALSE;
			ReQuery();//����ͳ�ƣ�����״̬
			UpdateData(FALSE);
		}
	}
}

void CCreditDlg::OnTimer(UINT nIDEvent)
{//�Զ�����һ�β�ѯ
	if (TimeIdOfAutoQuery == nIDEvent)
	{
		KillTimer(nIDEvent);//ִֻ��һ��
		m_bAutoQuery = FALSE;
		
		if (m_bFromCard)
		{
			m_nMemberId = CCurrentNetBarUser::GetInstance()->Balance.GetMemberId();
			m_nCardId = CCurrentNetBarUser::GetInstance()->Balance.GetCardId();
			
			OnGetBalanceOK(CCurrentNetBarUser::GetInstance()->Balance);
			UpdateData(FALSE);
			return;
		}
		
		//�ӿ��Ż������
		if (!m_strNetId.IsEmpty())//�п��ţ�����ˢ�������
		{
			CheckCard(m_strNetId);
			return;
		}
	}

	CNDDialog::OnTimer(nIDEvent);
}

//����ˢ����Ϣ
LRESULT CCreditDlg::OnRealName(WPARAM wp, LPARAM lp)
{
	CString strTmp;
	m_edtCreditMoney.GetWindowText(strTmp);//�Ի������������

	if (strTmp.IsEmpty())
	{
		strTmp = (LPCTSTR)lp;//����������

		if (0 != m_strNetId.CompareNoCase(strTmp))//����ͬ
		{	
			m_strNetId = strTmp;//���¶Ի����е�������
			SetFormCard();//���ñ�־
			CheckCard(m_strNetId);//��ѯ

			return TRUE;
		}
	}

	return FALSE;
}
//������ɫ
HBRUSH CCreditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNDDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT)
		{//��ʱ������£���ɫ��ʾ�����������������ʾ
			if (m_bAlreadyBundTime)
			{
				pDC->SetTextColor(RGB(255, 0, 0));
			} 
			else
			{
				pDC->SetTextColor(RGB(0, 0, 255));
			}
		}
		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_ZTJE)
		{
			if (m_bIsMember)
			{
				pDC->SetTextColor(RGB(0, 0, 255));
			}
			else
			{
				pDC->SetTextColor(RGB(255, 0, 0));
			}
		}
	}
	else if (CTLCOLOR_EDIT == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT_CREDIT)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}		
	}

	return hbr;
}


//�Զ��˿��
void CCreditDlg::AutoDoRefundment(CBalanceMethod & Balance)
{	
	if (!CDeregisterHelpper::AllowAutoRefund(Balance))
	{
		return;
	}
	UpdateData(FALSE);

	//ִ���˿����
	DeregisterUser(TRUE);
}
//���Ѳ�ѯ��ť
void CCreditDlg::OnBnClickedButtonConsumelist()
{
	AutoClose();

	if (!m_strNetId.IsEmpty())
	{
		theApp.GetIBAView()->QueryUserComsumeList(m_strNetId);
	}
}

void CCreditDlg::DeregisterUser(BOOL bAutoRefund)
{
	// 2011/10/21-8208-gxx: 
	CActiveMember& ActiveMember = m_ActiveMember;

	CDeregisterMethod Deregister;

	Deregister.SetMemberId(m_nMemberId);
	Deregister.SetPersonalId(m_strPersonalID);
	Deregister.SetMemberName(m_strMemberName);
	Deregister.SetCardId(m_nCardId);
	Deregister.SetPassword(m_strPwd);
	Deregister.SetReturnMode(m_bUserHasCard ? 1 : 0);

	theApp.GetCurCashier()->DoDeregister(Deregister);

	if (Deregister.GetStatusCode() == 0)
	{	
		m_bAlreadyBundTime = FALSE;

		UINT nAll = Deregister.GetBalance() + Deregister.GetGuarantyMoney();//�ϼ�
		m_strResult.Format(IDS_DREGRESULT, nAll / 100.0);

		if (!bAutoRefund || CNetBarConfig::GetInstance()->GetReminderAutoReturn())
		{
			CIBATTS::GetInstance()->SpeakReturnResult(nAll);//����
		}
		

		//д������־
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeReturn);
		OperationInfo.SetIsBundTime(FALSE);
		OperationInfo.SetUserName(m_strName);
		OperationInfo.SetNetId(m_strNetId);
		OperationInfo.SetMemberId(m_nMemberId);
		OperationInfo.SetOperationAmount(Deregister.GetBalance());
		OperationInfo.SetAllAmount(nAll);
		OperationInfo.SetDeposit(Deregister.GetGuarantyMoney());

		if (m_ActiveMember.GetMemberID() > 0)
		{
			OperationInfo.SetCheckInTime(ActiveMember.GetCheckInTime());
			OperationInfo.SetConsumeMoney(ActiveMember.GetAdvancePayment()-nAll);
			OperationInfo.SetClassId(ActiveMember.GetClassId());
			OperationInfo.SetIdNum(ActiveMember.GetPersonalID());
			OperationInfo.SetTermId(ActiveMember.GetTerminalID());
		}
		
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);

		//������Ϣд�����ݿ�
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
		//��ʼ������״̬
		m_strAllBalance.Empty();
		m_strBalanceZ.Empty();
		m_strOnlineAmount.Empty();
		m_strBalanceW.Empty();
		m_strBalanceY.Empty();
		m_strGiveBack.Empty();
		m_strDeposit.Empty();
		m_strPoint.Empty();

		UpdateData(FALSE);

		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REFUNDMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CREDITPRINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BUNDTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CREDIT)->SetFocus();
		GetDlgItem(IDC_BUTTON_CHECKOUT)->ShowWindow(SW_HIDE);//�ǳ���ť��ֹ
		m_btnCancelActivation.ShowWindow(SW_HIDE);
		m_btnConsumeList.ShowWindow(SW_HIDE);

		//ʵ������ʱ��
		if (CNetBarConfig::GetInstance()->GetReturnOnlyClassId() != m_nUserClassID)
		{
			theApp.GetRealName()->InvokeRealName(100, (LPVOID)m_strNetId.GetString());
		}

		//��ռ���б���Ϣ

		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

		if (nIndex >= 0)
		{
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			computer.ClearUserInfo();
			CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);
			CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
		}

		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
	}
	else
	{	
		SetToolTipPos(IDC_BUTTON_REFUNDMENT);
		ShowToolTip(Deregister.GetStatusMessage(), TRUE);
	}
}

void CCreditDlg::OnBnClickedButtonReprint()
{
	//��ʱ�ɹ�ͬʱ��ǰѡ���˰�ʱ����
	if (m_bAlreadyBundTime && m_nBundleTimeId > 0)
	{
		PrintCreditBTimeTicket();
	} 
	else
	{
		//��ӡСƱ
		CString strTmp;
		strTmp.Format(_T("%.2f"), m_nCreditMoney / 100.0 );
		PrintTicket(strTmp);
	}
	
}

void CCreditDlg::DoBundTimeOnLine()
{
	CDlgBundleTimeList BundleTimeListDlg;
	BundleTimeListDlg.SetSelectStyle(1);

	if (BundleTimeListDlg.DoModal() == IDOK)
	{   
		//���
		INT nBundleTimeId = BundleTimeListDlg.GetBundleTimeId();//��ʱ���

		NS_ISERVER::CLocalServer::GetInstance()->LocalBundleTime(m_nMemberId, nBundleTimeId);
		Sleep(2000);

		//��ʱִ�в�ѯ���ȴ��ͻ��˰�ʱ���
		OnBnClickedButtonQuery();//���²�ѯ

		if (m_bAlreadyBundTime)
		{
			CString strTmp;
			strTmp.Format(IDS_ONLINEBUNDTIME);
			//MsgBox(L"���߰�ʱ�ɹ���");
			MsgBox(strTmp);
		}
		else
		{
			CString strTmp;
			strTmp.Format(IDS_ONLINEBTFAILE);
			//MsgBox(L"����ʧ�ܣ�");
			MsgBox(strTmp);
		}
	}
}

void CCreditDlg::OnBnClickedButtonBundtime()
{
	if (m_bOnLine)//����
	{
		if (!m_bAlreadyBundTime)
		{
			DoBundTimeOnLine();//���߰�ʱ
		} 
		return;
	}

	if (m_bAlreadyBundTime)
	{
		DoCancelBundleTime(m_strMemberID);
	} 
	else
	{
		SelectBundTime();
	}

}

void CCreditDlg::PrintCreditBTimeTicket()
{
	CString strTicket, strTmp;

	CIBAPrint::GetCreditAndBundleTimeTicket(strTicket);
	//��ֵ��Ϣ���û���Ϣ
	strTicket.Replace(_T("[name]"), m_strName);
	strTmp.Format(_T("%d"), m_nMemberId);
	strTicket.Replace(_T("[memberid]"), strTmp);
	strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);
	strTicket.Replace(_T("[memberclass]"), m_strUserClass);
	strTmp.Format(_T("%.2f"),  m_nCreditMoney / 100.0 );
	strTicket.Replace(_T("[inmoney]"), strTmp);
	//��ʱ��Ϣ
	strTicket.Replace(_T("[bundletimemoney]"), m_strBundTimeMoney);
	strTicket.Replace(_T("[starttime]"), m_strBundleTimeStartTime);
	strTicket.Replace(_T("[endtime]"), m_strBundleTimeEndTime);
	strTicket.Replace(_T("[pcclass]"), m_strBundleTimeArea);
	strTmp.Empty();
	strTmp.Format(_T("%.2f"), m_nAllMoney / 100.0);
	strTicket.Replace(_T("[totalmoney]"), strTmp);
	//ʵ��Ҫ��ǿ�����룬�򲻴�ӡ����
	if (CNetBarConfig::GetInstance()->GetRealNamePassword() >= 2)
	{
		strTicket.Replace(_T("[password]"), _T("*****"));
	}
	else
	{
		strTicket.Replace(_T("[password]"), m_strTmpUserPwd);
	}

	CIBAPrint::Print(strTicket);

}

BOOL CCreditDlg::DoBundleTime()
{	//û���Ѿ���ʱ��û�а�ʱ��ûѡ���ʱ��Ϣ������
	if (m_nBundleTimeId == 0 || m_bAlreadyBundTime || m_bOnLine)
	{
		return FALSE;
	}

	CBundleTimeMethod bundleTime;
	bundleTime.SetMemberId(m_nMemberId);
	bundleTime.SetReserve(m_nBundTimeClass);
	bundleTime.SetTimeId(m_nBundleTimeId);
	theApp.GetCurCashier()->DoBundleTime(bundleTime);

	if (bundleTime.GetStatusCode() == 0)
	{	
		CBundleTimeActionMethod BundleTimeAction;
		BundleTimeAction.SetMemberId(m_nMemberId);

		theApp.GetCurCashier()->DoBundleTimeAction(BundleTimeAction);

		if (BundleTimeAction.GetStatusCode() == 0 && BundleTimeAction.GetNetBarId() > 0)
		{
			if (m_nCreditMoney > 0)
			{
				CIBATTS::GetInstance()->SpeakCreditBundTimeResult(m_nCreditMoney, m_nPresentMoney, 
					m_nCreditMoney + m_nPresentMoney - BundleTimeAction.GetAmount1() + m_nAllMoney,
					BundleTimeAction.GetAmount1());
			} 
			else
			{
				CIBATTS::GetInstance()->SpeakBundleTimeResult(BundleTimeAction.GetAmount1());
			}
			
			//���²�ѯ��Ϣ
			CheckCard(m_strNetId);

			if (m_bPrint)//��ʱ��ӡ
			{
				PrintCreditBTimeTicket();
			}
		}

		m_nBundleTimeId = 0;
	}
	else
	{//��ʱʧ��
		SetToolTipPos(IDOK);
		ShowToolTip(bundleTime.GetStatusMessage(), TRUE);
	}

	return TRUE;
}

void CCreditDlg::OnEnChangeEditIdcardsno()
{//����Ϣ��BCGP�޸ģ�����������ؼ�������Ż���յ�
	GetDlgItem(IDC_BUTTON_BUNDTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CONSUMELIST)->ShowWindow(SW_HIDE);
	// ȡ������
	GetDlgItem(IDC_BUTTON_CANCELACTIVE)->ShowWindow(SW_HIDE);
	m_nCreditMoney = 0;
}

void CCreditDlg::OnBnClickedButtonCancelactive()
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
			CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(m_nMemberId);
		}
		else
		{
			CActiveMember ActiveMember;

			if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, ActiveMember))
			{
				ActiveMember.SetActivationTime() = COleDateTime(1970, 1, 1, 8, 0, 0);
				CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
			}
		}

		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

		// 2011/10/27-8210-gxx: 
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);
		if (nIndex >= 0)
		{
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			theApp.GetRealName()->AuditUserCheckOut(
				computer.GetTerminalID(), computer.GetComputerIP(),m_nMemberId);	
		}
	}
	else
	{
		SetToolTipPos(IDC_BUTTON_CANCELACTIVE);
		ShowToolTip(Activation.GetStatusMessage(), TRUE);
	}
}

BOOL CCreditDlg::DoCredit()
{
	CWaitCursor WaitCursor;

	if (!CheckInput()) return FALSE;//�����Ǯ����

	//UINT nMoney = _tstof(m_strCreditMoney) * 100;
	UINT nMoney = CIBAHelpper::DecT2Int(m_strCreditMoney);
	
	
	if (!MakeSureCredit(nMoney)) return FALSE;//ѯ���Ƿ�Ҫ��ֵ

	if (m_bHasLocalQuery && m_LocalActiveMember.GetIsLocalUser())
	{
		return DoLocalModeCredit(nMoney);
	}

	///////////////////////�˺ų�ֵ/////////////////////
	CCreditMethod Credit;

	Credit.SetMemberId(m_nMemberId);
	Credit.SetTermTime(CIBAHelpper::GetCurrentTimeAsString());
	Credit.SetActivityId(0);
	Credit.SetNoPresent(m_bNoPresent);
	Credit.SetAmount(nMoney);

	theApp.GetCurCashier()->DoCredit(Credit);
	///////////////////////��ֵ�ɹ������ø���״̬///////////////////////
	if (Credit.GetStatusCode() == 0)
	{	
		//������ǰ�ĳ�ֵ���
		//m_nCreditMoney = _tstof(m_strCreditMoney) * 100.0;
		m_nCreditMoney = CIBAHelpper::DecT2Int(m_strCreditMoney);
		//���ͽ��
		if (m_bNoPresent)
		{
			m_nPresentMoney = 0;
		} 
		else
		{
			m_nPresentMoney = Credit.GetCreditPresent();
		}

		//�ظ���ӡ��ť����
		if (m_bPrint)
		{
			GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_NORMAL);
		}      

		if (m_bOnLine) //���ͳ�ֵ��Ϣ
		{			
			CString strTmp;
			strTmp.Format(IDS_ONLINECREDIT, nMoney / 100.0, Credit.GetCreditPresent() / 100.0);
			CLocalServer::GetInstance()->LocalSendShortMessageByMemberID(m_nMemberId, strTmp);
		}

		{//д����ֵ��Ϣ��־��¼��
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(1);
			OperationInfo.SetIsBundTime(FALSE);
			OperationInfo.SetUserName(m_strName);
			OperationInfo.SetNetId(m_strNetId);
			OperationInfo.SetMemberId(m_nMemberId);
			OperationInfo.SetOperationAmount(nMoney);
			OperationInfo.SetDeposit(_ttoi(m_strDeposit) * 100);
			OperationInfo.SetPassWord(m_strTmpUserPwd);
			OperationInfo.SetPresentAmount(Credit.GetCreditPresent());
			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));;
		}

		{//д�����ݿ�

			CCreditInfoDB CreditInfo;

			CreditInfo.SetCardSerial(m_strNetId);
			CreditInfo.SetCreditDate(CIBAHelpper::FormatCenterTime(Credit.GetTrantime()));
			CreditInfo.SetRefNo(Credit.GetRefNo());
			CreditInfo.SetCreditPresent(Credit.GetCreditPresent());
			CreditInfo.SetCreditAmount(nMoney);
			CreditInfo.SetOperator(theApp.GetCurCashier()->GetName());
			CreditInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			CreditInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
			CreditInfo.SetMemberId(m_nMemberId);
			CreditInfo.SetSerialNo(Credit.GetLocalSerialNo());
			CreditInfo.SetCassId(m_nUserClassID);

			CIBADAL::GetInstance()->AddCreditRecord(CreditInfo);
		}

		m_strResult.Format(IDS_CREDITRESULT, nMoney / 100.0, Credit.GetCreditPresent() / 100.0);
		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(m_strResult);

		//��ֵ�ɹ����ѯ����
		if (m_bAlreadyBundTime || m_nBundleTimeId == 0 || m_bOnLine)//�Ѿ���ʱ��(û�а�ʱ��ûѡ���ʱ��Ϣ��
		{
			//����
			CIBATTS::GetInstance()->SpeakCreditResult(nMoney, Credit.GetCreditPresent(), Credit.GetAvailableBalance());

			CheckCard(m_strNetId);

			//��ӡСƱ
			if (m_bPrint)
			{
				CString strTmp;
				strTmp.Format(_T("%.2f"), nMoney / 100.0);
				PrintTicket(strTmp);
			}
		}

		//��ճ�ֵ�ı༭��
		m_strCreditMoney.Empty();
		m_edtCreditMoney.SetWindowText(_T(""));
		m_edtCreditMoney.SetFocus();

		return TRUE;
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(Credit.GetStatusMessage(), TRUE);

		return FALSE;
	}
}

//ȡ����ʱ����
BOOL CCreditDlg::DoCancelBundleTime(CString strMemberID)
{
	if (strMemberID.IsEmpty())//��ִ���Լ���
	{
		return FALSE;
	}

	CWaitCursor Wait;

	CCancelBundleTimeMethod CancelBundleTime;

	CancelBundleTime.SetMemberId(_ttoi(strMemberID));
	//ִ��ȡ����ʱ
	theApp.GetCurCashier()->DoCancelBundleTime(CancelBundleTime);

	CString strRet;

	if (CancelBundleTime.GetStatusCode() == 0)
	{
		ClearData();
		//���²�ѯ��Ϣ
		CheckCard(m_strNetId);

		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(CancelBundleTime.GetStatusMessage());

		return TRUE;
	}
	else
	{
		CIBALog::GetInstance()->WriteFormat(_T("MemberID:%s CancelBundleTime failed!"), strMemberID);

		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(CancelBundleTime.GetStatusMessage(), TRUE);
		
		return FALSE;
	}

}

void CCreditDlg::SelectBundTime()
{
	CDlgBundleTimeList dlg;

	dlg.SetUserClassId(m_nUserClassID);

	if (dlg.DoModal() == IDOK)
	{
		//���
		m_nBundleTimeId = dlg.GetBundleTimeId();//��ʱ���

		//��ֵ��ֵ
		m_edtCreditMoney.SetWindowText(dlg.GetBundleTimeMoneyString());

		//if (!CIBAConfig::GetInstance()->GetCreditBeforeBundtime()) 
		{
			if (m_nAllMoney < (INT)dlg.GetBundleTimeMoney())
			{
				SetToolTipPos(IDC_EDIT_CREDIT);
				ShowToolTip(IDS_CREDITBEFOREBT);
			} 
			else
			{
				m_edtCreditMoney.SetWindowText(_T(""));
				m_strCreditMoney.Empty();

				DoBundleTime();
			}
		}

		//��ʱ���
		m_strBundTimeMoney = dlg.GetBundleTimeMoneyString();
		m_strBundTimeLimit.Format(_T("%s-%s"), dlg.GetStartTime(), dlg.GetEndTime());
		m_strBTRemainTime = dlg.GetRemainTime();

		//ѡ����Ϻ󽹵����ŵ��༭������
		m_edtCreditMoney.SetFocus();

		UpdateData(FALSE);
	}
}

void CCreditDlg::OnBnClickedButtonCreditprint()
{
	m_bPrint = TRUE;

	OnBnClickedOk();
}

BOOL CCreditDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (CIBAHelpper::IsCtrlPressed())
		{
			return PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_CREDITPRINT, BN_CLICKED));
		}

		if (!GetDlgItem(IDOK)->IsWindowEnabled())
		{
			if (!m_strResult.IsEmpty())
			{
				AutoClose();
			}
		}
	}
	
	return CNDDialog::PreTranslateMessage(pMsg);
}

BOOL CCreditDlg::CheckReturnAction()
{
	LPMSG pMsg = AfxGetCurrentMessage();
	//�رնԻ���
	if (m_strCreditMoney.IsEmpty() && m_nBundleTimeId == 0)
	{
		if (CIBAHelpper::IsAltPressed())
		{
			return FALSE;
		}

		if (WM_LBUTTONUP == pMsg->message)
		{
			return FALSE;
		}

		if (CIBAConfig::GetInstance()->GetReturnAction() == 1)//����Ҫ���˿�
		{
			OnBnClickedButtonRefundment();
		} 
		else//�س��ر�
		{
			AutoClose();
		}

		return TRUE;
	}

	return FALSE;
}

void CCreditDlg::ShowUserInfo(CBalanceMethod & Balance)
{
	m_bOnLine = (Balance.GetOnline() == 1); //1 ��ʾ�û�����

	m_strNetId = Balance.GetSerailNum();
	m_strMemberID.Format(_T("%d"), Balance.GetMemberId());
	m_strName.Format(_T("%s(%s)"), Balance.GetUserName(), CIBAHelpper::GetSexAsString(Balance.GetSex()));
	m_strPoint.Format(_T("%d"), Balance.GetCurrentPoint());
	m_strMemberName = Balance.GetMemberName();
	m_strPersonalID = Balance.GetPersonalId();
	m_nUserClassID = Balance.GetUserClass();
	m_strUserClass = CNetBarConfig::GetInstance()->GetUserClassName(Balance.GetUserClass());
	m_strNetBar = CNetBarConfig::GetInstance()->GetNetBarName(Balance.GetOpenNetbarId());
	m_strDeposit.Format(_T("%.2f"), Balance.GetGuarantyMoney() / 100.0);//Ѻ��
	m_strBalanceW.Format(_T("%.2f"), Balance.GetCashBalance() / 100.0);//����
	m_strBalanceY.Format(_T("%.2f"), Balance.GetUsePresent() / 100.0);//����
	m_strBalanceZ.Format(_T("%.2f"), Balance.GetCreditPresent() / 100.0);//����
	m_strOnlineAmount.Format(_T("%.2f"), Balance.GetOnlineAmount() / 100.0);//����Ԥ��
	m_nAllMoney = Balance.GetTotalBalacne();

	if (Balance.GetAllowReturn() > 0) 
	{
		m_strGiveBack.Format(_T("%.2f"), (Balance.GetReturnAmount() + Balance.GetGuarantyMoney()) / 100.0);//�˿�
	}
	else
	{
		m_strGiveBack.Format(_T("%.2f"), m_nAllMoney / 100.0);//�ϼ�
	}

	if (Balance.GetIsMember() == 0) //��ʱ�û�����
	{
		m_strTmpUserPwd = Balance.GetPassword();
	}
};

void CCreditDlg::ShowBundleTimeInfo(CBalanceMethod & Balance)
{
	m_bAlreadyBundTime = (Balance.GetPayType() > 0);

	if (m_bAlreadyBundTime)
	{
		m_strBundTimeMoney.Format(_T("%.2f"), Balance.GetPrice() / 100.0);
		m_strBundleTimeStartTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetStartTime()).Format(_T("%H:%M"));
		m_strBundleTimeEndTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime()).Format(_T("%H:%M"));
		m_strBundTimeLimit.Format(_T("%s-%s"), m_strBundleTimeStartTime, m_strBundleTimeEndTime);
		
		///////////////////��ʱʣ��ʱ��/////////////////
		CString strCurTime = CIBAHelpper::GetCurrentTimeAsString();

		if (strCurTime < Balance.GetStartTime())
		{
			COleDateTime StartTime, EndTime;
			StartTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetStartTime());
			EndTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime());
			COleDateTimeSpan RemainTime = EndTime - StartTime;
			m_strBTRemainTime = RemainTime.Format(_T("%H:%M:%S"));
		}
		else if (strCurTime > Balance.GetStartTime() && strCurTime < Balance.GetEndTime())
		{
			//���ڴ˴���ʱ�������������ķ��صģ���˲���Ҫ�������䳤�ȵ��ж���
			COleDateTime CurTime, EndTime;
			CurTime = COleDateTime::GetCurrentTime();
			EndTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime());
			COleDateTimeSpan RemainTime = EndTime - CurTime;
			m_strBTRemainTime = RemainTime.Format(_T("%H:%M:%S"));
		} 
		else if (strCurTime > Balance.GetEndTime())
		{
			m_strBTRemainTime = _T("00:00:00");
		}
	
		if (!m_bOnLine)//��������ߣ�����ʾ��ʱ��Ϣ
		{
			CString strBTInfo = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetStartTime()).Format(_T("%H:%M"));
			strBTInfo.Append(_T(" - "));
			strBTInfo.Append(CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime()).Format(_T("%H:%M")));
			CIBAString strTmp;
			strTmp.LoadString(Balance.GetPayType() == 1 ? IDS_PAYTYPE1 : IDS_PAYTYPE2);
			m_strResult.Format(LOAD_STRING(IDS_BUNDLETIMEINFO), strTmp, strBTInfo);
		}
	}
}

void CCreditDlg::ShowControls(CBalanceMethod & Balance)
{
	GetDlgItem(IDOK)->EnableWindow(TRUE); //��ʾ��ֵ��ť
	GetDlgItem(IDC_BUTTON_CREDITPRINT)->EnableWindow(TRUE); //��ֵ��ӡ
	GetDlgItem(IDC_BUTTON_CONSUMELIST)->ShowWindow(SW_NORMAL); //�û����Ѳ�ѯ

	if (!m_bOnLine && CNetBarConfig::GetInstance()->GetNeedActivation() == 1) //��ʾȡ������
	{	
		GetDlgItem(IDC_BUTTON_CANCELACTIVE)->ShowWindow(SW_NORMAL);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CANCELACTIVE)->ShowWindow(SW_HIDE);
	}

	if (m_bOnLine) //����
	{
		if (Balance.GetDisallowLogout() == 0) //���������»�
		{
			GetDlgItem(IDC_BUTTON_CHECKOUT)->ShowWindow(SW_SHOW);
		} 

		if (!m_bAlreadyBundTime) //û�а�ʱ
		{
			m_strResult.Format(IDS_USERONLINE, Balance.GetTermId());//�ն˺�
		}
	}
	else
	{
		m_strResult.Empty();
		//������ʱ��ʾ���˽��
		GetDlgItem(IDC_STATIC_ZTJE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_BUTTON_CHECKOUT)->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_BUTTON_REFUNDMENT)->EnableWindow(Balance.GetAllowReturn() > 0);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(Balance.GetAllowReturn() > 0);

	if (Balance.GetAllowReturn() > 0) //�����˿�
	{
		GetDlgItem(IDC_STATICZTJE)->SetWindowText(LOAD_STRING(IDS_CAN_RETURN_MONEY)); // ���˽��(Ԫ)��
	}
	else
	{
		GetDlgItem(IDC_STATICZTJE)->SetWindowText(LOAD_STRING(IDS_ACCOUNT_TOTAL)); // �˻��ϼ�(Ԫ)��
	}

	//��ʱ��ť����
	GetDlgItem(IDC_BUTTON_BUNDTIME)->EnableWindow(TRUE);

	if (m_bAlreadyBundTime)
	{
		GetDlgItem(IDC_BUTTON_BUNDTIME)->SetWindowText(LOAD_STRING(IDS_CANCEL_BT));
		
		//��������£�ȡ����ʱ���ܽ�ֹ
		if (m_bOnLine)
		{
			GetDlgItem(IDC_BUTTON_BUNDTIME)->EnableWindow(FALSE);
		}
	} 
	else
	{
		GetDlgItem(IDC_BUTTON_BUNDTIME)->SetWindowText(LOAD_STRING(IDS_SELECT_BT));
	}
}

BOOL CCreditDlg::DoLocalModeQuery()
{
	m_bHasLocalQuery = TRUE;
	BOOL bRet = CNDDialog::DoLocalModeQuery();
	if (!bRet)
	{
		return FALSE;
	}

	//m_bOnLine = m_LocalActiveMember.GetIsOnLine();
	//m_strNetId = m_LocalActiveMember.GetNetId();

	m_strMemberID.Format(_T("%ud"), m_nMemberId);
	m_strMemberName = m_strName;

	m_strNetBar = m_strOpenNetBar;
	m_nAllMoney = m_LocalActiveMember.GetAvailavleBalance();
	m_strDeposit.Format(_T("%.2f"), 0);//Ѻ��
	m_strBalanceW.Format(_T("%.2f"), 0);//����
	m_strBalanceY.Format(_T("%.2f"), 0);//����
	m_strBalanceZ.Format(_T("%.2f"), 0);//����
	m_strOnlineAmount.Format(_T("%.2f"), m_nAllMoney/100.0);//����Ԥ��
	
	if (m_LocalActiveMember.GetIsLocalUser())
	{	
		m_strGiveBack.Format(_T("%.2f"),(m_LocalActiveMember.GetAdvancePayment()-m_LocalActiveMember.GetLocalExpense())/100.0);
	}
	else
	{
		m_strGiveBack.Format(_T("%.2f"),(m_LocalActiveMember.GetAvailavleBalance())/100.0);
	}

	m_strTmpUserPwd = m_LocalActiveMember.GetPassword();

	UpdateData(FALSE);

	// ֻ�������û����Գ�ֵ
	if (m_LocalActiveMember.GetIsLocalUser())
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE); //��ʾ��ֵ��ť
		GetDlgItem(IDC_BUTTON_CREDITPRINT)->EnableWindow(TRUE); //��ֵ��ӡ
	}
	
	
	if (m_bOnLine) //����
	{
		m_strResult.Format(IDS_USERONLINE, m_LocalActiveMember.GetTerminalID());//�ն˺�
	}
	else
	{
		m_strResult.Empty();
		//������ʱ��ʾ���˽��
		GetDlgItem(IDC_STATIC_ZTJE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_BUTTON_CHECKOUT)->ShowWindow(SW_HIDE);
	}

	if (!m_LocalActiveMember.GetIsLocalUser() && m_LocalActiveMember.IsMember())
	{
		GetDlgItem(IDC_BUTTON_REFUNDMENT)->SetWindowText(LOAD_STRING(IDS_VIP_RETURN));
		GetDlgItem(IDC_BUTTON_REFUNDMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATICZTJE)->SetWindowText(LOAD_STRING(IDS_ACCOUNT_TOTAL));
	}
	else
	{
		// ��ʱ�û� �� �����û� �˿�
		GetDlgItem(IDC_BUTTON_REFUNDMENT)->SetWindowText(LOAD_STRING(IDS_RETURN_EX));
		GetDlgItem(IDC_BUTTON_REFUNDMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATICZTJE)->SetWindowText(LOAD_STRING(IDS_CAN_RETURN_MONEY));
	}

	return TRUE;
}

BOOL CCreditDlg::DoLocalModeCredit(UINT nCreditMoney)
{
	BOOL bOK = CCashierHelper::CreditLocalUser(m_LocalActiveMember, nCreditMoney);
	if (bOK)
	{
		//�ظ���ӡ��ť����
		if (m_bPrint)
		{
			GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_NORMAL);
		}  

		m_strResult.Format(IDS_CREDITRESULT, nCreditMoney / 100.0, 0 / 100.0);
		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(m_strResult);
		
		//��ӡСƱ
		if (m_bPrint)
		{
			CString strTmp;
			strTmp.Format(_T("%.2f"), nCreditMoney / 100.0);
			PrintTicket(strTmp);
		}

		//��ճ�ֵ�ı༭��
		m_strCreditMoney.Empty();
		m_edtCreditMoney.SetWindowText(_T(""));
		m_edtCreditMoney.SetFocus();
		return TRUE;
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(LOAD_STRING(IDS_LOCAL_CREDIT_FAULT), TRUE);  // ���س�ֵʧ��

		return FALSE;
	}

	
}
