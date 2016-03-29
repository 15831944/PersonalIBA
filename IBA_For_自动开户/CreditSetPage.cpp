// CreditSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "CreditSetPage.h"
#include ".\creditsetpage.h"


// CSetPageOfCredit �Ի���

IMPLEMENT_DYNAMIC(CSetPageOfCredit, CIBAPage)
CSetPageOfCredit::CSetPageOfCredit()
: CIBAPage(CSetPageOfCredit::IDD)
, m_bAutoRefund(FALSE)
, m_nAutoRefundTime(0)
, m_bPopMsgBox(FALSE)
, m_nMaxCreditMoney(0)
{
	EnableVisualManagerStyle();
}

CSetPageOfCredit::~CSetPageOfCredit()
{
}

void CSetPageOfCredit::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);
	//DDX_Check(pDX, IDC_CHECK_REFUNDMENT, m_bAutoRefund);
	DDX_Text(pDX, IDC_EDIT_REFUNDTIME, m_nAutoRefundTime);
	DDX_Control(pDX, IDC_COMBO_RETURNACTION, m_cboReturnAction);
	DDX_Check(pDX, IDC_CHECK_POPMSGBOX, m_bPopMsgBox);
	DDX_Text(pDX, IDC_EDIT_MAX_MONEY, m_nMaxCreditMoney);
}


BEGIN_MESSAGE_MAP(CSetPageOfCredit, CIBAPage)
	ON_BN_CLICKED(IDC_CHECK_REFUNDMENT, OnBnClickedCheckRefundment)
END_MESSAGE_MAP()


// CSetPageOfCredit ��Ϣ�������

void CSetPageOfCredit::OnBnClickedCheckRefundment()
{
	UpdateData();

	if (m_bAutoRefund)
	{
		GetDlgItem(IDC_EDIT_REFUNDTIME)->SetFocus();
	} 
}

void CSetPageOfCredit::OnOK()
{
	UpdateData();

	// 2011/05/12-gxx: ����û������ǿ���˿�ʱ��Ϊ0, ����Ҫ����ʱ�������
	if (CNetBarConfig::GetInstance()->GetMustAutoReturn()!=1)
	{
		CIBAConfig::GetInstance()->SetRefundmentTime(m_nAutoRefundTime);
	}	

	// �س�����
	if (m_cboReturnAction.GetCurSel() != -1)
	{
		CIBAConfig::GetInstance()->SetReturnAction(m_cboReturnAction.GetCurSel());
	}

	// 2012/03/05-8238-gxx:
	CIBAConfig::GetInstance()->SetMaxCreditMoney((INT)m_nMaxCreditMoney);

	//���浯����Ϣ
	CIBAConfig::GetInstance()->SetPopMsgBox(m_bPopMsgBox);

	CIBAPage::OnOK();
}

BOOL CSetPageOfCredit::OnInitDialog()
{
	CIBAPage::OnInitDialog();

	// 2011/04/27-gxx: ����ʱ���������λ��
	((CEdit*)GetDlgItem(IDC_EDIT_REFUNDTIME))->SetLimitText( CIBAConfig::MAX_TIME_LIMIT ); 

	//�Զ��˿������
	m_nAutoRefundTime = CIBAConfig::GetInstance()->GetRefundmentTime();
	
	// 2011/05/12-gxx: ���������ǿ���Զ��˿�ʱ��Ϊ0�� ��ô�ͽ����Զ��Կ�ʱ��ı༭��
	if (CNetBarConfig::GetInstance()->GetMustAutoReturn()==1)
	{
		GetDlgItem(IDC_EDIT_REFUNDTIME)->EnableWindow(FALSE);
		m_nAutoRefundTime = 0;
	}

	
	//�س�����
	m_cboReturnAction.SetCurSel(CIBAConfig::GetInstance()->GetReturnAction());

	//�Ƿ񵯳��Ի���
	m_bPopMsgBox = CIBAConfig::GetInstance()->GetPopMsgBox();

	// 2012/03/05-8238-gxx:
	m_nMaxCreditMoney = CIBAConfig::GetInstance()->GetMaxCreditMoney();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
