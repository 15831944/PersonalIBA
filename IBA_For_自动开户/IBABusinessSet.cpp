// IBABusinessSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include ".\ibabusinessset.h"


// CIBABusinessSet �Ի���

IMPLEMENT_DYNAMIC(CSetPageOfRegister, CIBAPage)
CSetPageOfRegister::CSetPageOfRegister()
: CIBAPage(CSetPageOfRegister::IDD)
, m_bCheckIDCardNumber(TRUE)
, m_bHasCard(FALSE)
{
}

CSetPageOfRegister::~CSetPageOfRegister()
{
}

void CSetPageOfRegister::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_USERCLASS, m_cboUserClass);
	DDX_Control(pDX, IDC_COMBO_IDTYPE, m_cboIDType);

	DDX_Control(pDX, IDC_COMBO_PASSWORDTYPE, m_cboPwdType);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strDefPwd);
	DDX_Control(pDX, IDC_COMBO_PASSWORDTYPE2, m_cboTempPwdType);
	DDX_Text(pDX, IDC_EDIT_PWD2, m_strTempDefPwd);

	DDX_Control(pDX, IDC_COMBO_REGISTERAGE, m_cboRegisterAge);
	DDX_Text(pDX, IDC_EDIT_REGISTERMONEY, m_strRegisterMoney);
	DDX_Check(pDX, IDC_CHECK_HASCARD, m_bHasCard);
	DDX_Check(pDX, IDC_CHECK_CHECKIDCARDNUMBER, m_bCheckIDCardNumber);

}


BEGIN_MESSAGE_MAP(CSetPageOfRegister, CIBAPage)
	ON_CBN_SELCHANGE(IDC_COMBO_PASSWORDTYPE, OnCbnSelchangeComboPasswordtype)
	ON_CBN_SELCHANGE(IDC_COMBO_PASSWORDTYPE2, OnCbnSelchangeComboPasswordtype2)
END_MESSAGE_MAP()


// CIBABusinessSet ��Ϣ�������

BOOL CSetPageOfRegister::OnInitDialog()
{
	CIBAPage::OnInitDialog();

	int nTmp = 0;

	//�û�����
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
	{
		int nIdx = m_cboUserClass.AddString(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName());

		UINT nClassID = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID();

		m_cboUserClass.SetItemData(nIdx, nClassID);

		if (CIBAConfig::GetInstance()->GetDefUserClassID() == nClassID)
		{
			nTmp = nIdx;
		}
	}

	m_cboUserClass.SetCurSel(nTmp);


	//֤������
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetCount(); i++)
	{
		int nIdx = m_cboIDType.AddString(CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(i).GetIDTypeName());

		UINT nClassID = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(i).GetIDTypeID();

		m_cboIDType.SetItemData(nIdx, nClassID);

		if (CIBAConfig::GetInstance()->GetDefIDTypeId() == nClassID)
		{
			nTmp = nIdx;
		}
	}

	m_cboIDType.SetCurSel(nTmp);
	//�������뷽ʽ
	m_cboPwdType.SetCurSel(CIBAConfig::GetInstance()->GetPasswordType());
	OnCbnSelchangeComboPasswordtype();

	m_cboTempPwdType.SetCurSel(CIBAConfig::GetInstance()->GetTempPasswordType());
	OnCbnSelchangeComboPasswordtype2();

	//{ 2011/06/21-gxx: �������ж�
	if (CIBAConfig::GetInstance()->GetPasswordType() == CIBAGlobal::emPwdTypeFixed)
	{
		m_strDefPwd = CIBAConfig::GetInstance()->GetDefPassword();
	}
	if (CIBAConfig::GetInstance()->GetTempPasswordType() == CIBAGlobal::emPwdTypeFixed)
	{
		m_strTempDefPwd = CIBAConfig::GetInstance()->GetTempDefPassword();
	}
	//}

	((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetLimitText(8);
	((CEdit*)GetDlgItem(IDC_EDIT_PWD2))->SetLimitText(8);

	//��������
	if (CIBAConfig::GetInstance()->GetRegisterAge() == 0)
	{
		m_cboRegisterAge.SetCurSel(0);
	}
	else
	{
		CString strTxt;
		strTxt.Format(_T("%d"), CIBAConfig::GetInstance()->GetRegisterAge());
		m_cboRegisterAge.SelectString(-1, strTxt);
	}
	//�������
	m_strRegisterMoney.Format(_T("%.0f"), CIBAConfig::GetInstance()->GetRegisterMoney() / 100.0);
	
	//�û��Դ���
	m_bHasCard = CIBAConfig::GetInstance()->GetHasCard();

	m_bCheckIDCardNumber = CIBAConfig::GetInstance()->GetCheckIDCardNumber();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSetPageOfRegister::OnOK()
{
	UpdateData();

	CIBAConfig::GetInstance()->SetTempDefPassword(m_strTempDefPwd);
	CIBAConfig::GetInstance()->SetTempPasswordType(m_cboTempPwdType.GetCurSel());
	CIBAConfig::GetInstance()->SetDefPassword(m_strDefPwd);
	CIBAConfig::GetInstance()->SetPasswordType(m_cboPwdType.GetCurSel());

	CIBAConfig::GetInstance()->SetDefUserClassID(m_cboUserClass.GetItemData(m_cboUserClass.GetCurSel()));
	CIBAConfig::GetInstance()->SetDefIDTypeId(m_cboIDType.GetItemData(m_cboIDType.GetCurSel()));

	/////////////////������������///////////////////
	INT CurIndex = m_cboRegisterAge.GetCurSel();
	if (CurIndex > -1)
	{
		if (CurIndex == 0)
		{
			CIBAConfig::GetInstance()->SetRegisterAge(0);
		}
		else
		{
			CString strAge;
			m_cboRegisterAge.GetLBText(CurIndex, strAge);
			CIBAConfig::GetInstance()->SetRegisterAge(_tstoi(strAge));
		}
	}
	//////////�������///////////
	CIBAConfig::GetInstance()->SetRegisterMoney(_tstof(m_strRegisterMoney) * 100);
	//�û��Դ���
	CIBAConfig::GetInstance()->SetHasCard(m_bHasCard);

	CIBAConfig::GetInstance()->SetCheckIDCardNumber(m_bCheckIDCardNumber);

	CIBAPage::OnOK();
}

void CSetPageOfRegister::OnCbnSelchangeComboPasswordtype()
{
	BOOL bEnableEdit = (m_cboPwdType.GetCurSel() == CIBAGlobal::emPwdTypeFixed);
	GetDlgItem(IDC_EDIT_PWD)->EnableWindow(bEnableEdit);
	if (bEnableEdit)
	{
		GetDlgItem(IDC_EDIT_PWD)->SetWindowText(CIBAConfig::GetInstance()->GetDefPassword());
	}
	else
	{
		GetDlgItem(IDC_EDIT_PWD)->SetWindowText(_T(""));
	}
}

void CSetPageOfRegister::OnCbnSelchangeComboPasswordtype2()
{

	BOOL bEnableEdit = (m_cboTempPwdType.GetCurSel() == CIBAGlobal::emPwdTypeFixed);
	GetDlgItem(IDC_EDIT_PWD2)->EnableWindow(bEnableEdit);
	if (bEnableEdit)
	{
		GetDlgItem(IDC_EDIT_PWD2)->SetWindowText(CIBAConfig::GetInstance()->GetTempDefPassword());
	}
	else
	{
		GetDlgItem(IDC_EDIT_PWD2)->SetWindowText(_T(""));
	}
}

