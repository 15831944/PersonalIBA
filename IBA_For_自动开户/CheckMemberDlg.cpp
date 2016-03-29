// CheckMember.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IdentityNumber.h"
#include "Servlet\ModifyMemberMethod.h"
#include "Cashier.h"
#include ".\checkmemberdlg.h"
#include "IBAHelpper.h"

// CCheckMemberDlg �Ի���

IMPLEMENT_DYNAMIC(CCheckMemberDlg, CIBADialog)
CCheckMemberDlg::CCheckMemberDlg(CWnd* pParent /*=NULL*/)
: CIBADialog(CCheckMemberDlg::IDD, pParent)
,m_nRNCertificateType(0) ,m_nNetbarID(0)
,m_nSLCertificateType(0)
{

}

CCheckMemberDlg::~CCheckMemberDlg()
{
}

void CCheckMemberDlg::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_RNCARDID, m_strRNCardID);
	DDX_Text(pDX, IDC_STATIC_SLCARDID, m_strSLCardID);
	DDX_Text(pDX, IDC_STATIC_RNNAME, m_strRNName);
	DDX_Text(pDX, IDC_STATIC_SLNAME, m_strSLName);
	DDX_Text(pDX, IDC_STATIC_RNCERTIFICATECLASS, m_strRNCertificateType);
	DDX_Text(pDX, IDC_STATIC_SLCERTIFICATECLASS, m_strSLCertificateType);
	DDX_Text(pDX, IDC_STATIC_RNCERTIFICATEID, m_strRNCertificateID);
	DDX_Text(pDX, IDC_STATIC_SLCERTIFICATEID, m_strSLCertificateID);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPwd);
	DDX_Control(pDX, IDC_STATIC_TIP, m_STTip);
}


BEGIN_MESSAGE_MAP(CCheckMemberDlg, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CCheckMemberDlg ��Ϣ�������
BOOL CCheckMemberDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	//CenterWindow();
	//ShowWindow(SW_SHOW);

	CIBAHelpper::CreateFont(m_BigFont, 16, 220);
	m_STTip.SetFont(&m_BigFont);

	DoModify();

	return FALSE;// ��������棬������TABΪ1�Ľ��㣬���򣬰����趨�Ľ���
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCheckMemberDlg::SetRealName(CNetBarUser& NetBarUser)
{
	//�û���
	m_strRNName = NetBarUser.GetUserName();

	//֤������
	m_nRNCertificateType = NetBarUser.GetCertificateType();
	for (INT j = 0; j < CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetCount(); j++)
	{
		UINT nCertificateType = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(j).GetIDTypeID();

		if (nCertificateType == NetBarUser.GetCertificateType())
		{
			m_strRNCertificateType = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(j).GetIDTypeName();
			break;
		}
	}
	//֤����
	m_strRNCertificateID = NetBarUser.GetCertificateID();
}

void CCheckMemberDlg::SetServlet(NS_SERVLET::CBalanceMethod& BalanceMethod)
{
	//�û���
 	m_strSLName = BalanceMethod.GetUserName();

	//֤������
	m_nSLCertificateType = BalanceMethod.GetIdType();//Ϊ�����޸ı������
	for (INT j = 0; j < CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetCount(); j++)
	{
		UINT nCertificateType = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(j).GetIDTypeID();

		UINT tmp = BalanceMethod.GetIdType();
		if (nCertificateType == BalanceMethod.GetIdType())
		{
			m_strSLCertificateType = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(j).GetIDTypeName();
			break;
		}
	}
	//֤����
 	m_strSLCertificateID = BalanceMethod.GetPersonalId();

	//���ɺ�
	m_nNetbarID = BalanceMethod.GetOpenNetbarId();

	//�û���¼�������û�������
	m_strMemberName = BalanceMethod.GetMemberName();
	
}

void CCheckMemberDlg::SetCardID(CString strCardID )
{
	//��������
	m_strRNCardID = strCardID;

	m_strSLCardID = strCardID;
}

void CCheckMemberDlg::DoModify()
{
	CWaitCursor Wait;

	UpdateData();

	NS_SERVLET::CModifyMemberMethod ModifyMember;//ֻ����ʵ�����ڵ�����

	ModifyMember.SetUsername(m_strRNName);
	ModifyMember.SetOpenNetBarId(m_nNetbarID);
	ModifyMember.SetIdType(m_nRNCertificateType);//ʵ������
	ModifyMember.SetPersonalId(m_strRNCertificateID);//ʵ��֤����
	ModifyMember.SetOldPersonalId(m_strSLCertificateID);//����ԭ֤����
	ModifyMember.SetPassword(_T("PASSWORDPASSWORD"));
	ModifyMember.SetMemberName(m_strMemberName);
	ModifyMember.SetCountryId(_T("CN"));

	theApp.GetCurCashier()->DoModifyMember(ModifyMember);

	if (ModifyMember.GetStatusCode() == 0)
	{
		GetDlgItem(IDOK)->SetFocus();

		//CString strTmp;
		//strTmp.LoadString(IDS_MODIFYMEMBERINFOSUCCESS);
		//MsgBox(strTmp);
	}
	else
	{
		//SetToolTipPos(IDOK);
		//ShowToolTip(ModifyMember.GetStatusMessage());
	}
}

void CCheckMemberDlg::OnBnClickedOk()
{
	CBCGPDialog::OnOK();
}

void CCheckMemberDlg::OnCancel()
{
	//CBCGPDialog::OnCancel();
}
