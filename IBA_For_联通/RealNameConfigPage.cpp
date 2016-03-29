// RealNameConfigPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "RealNameConfigPage.h"
#include ".\realnameconfigpage.h"


// CConfigOfRealName �Ի���

IMPLEMENT_DYNAMIC(CConfigOfRealName, CIBADialog)
CConfigOfRealName::CConfigOfRealName(CWnd* pParent /*=NULL*/)
: CIBADialog(CConfigOfRealName::IDD, pParent)
{
}

CConfigOfRealName::~CConfigOfRealName()
{
}

void CConfigOfRealName::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STATIC_WWMIP, m_strWWMIPTIP);
	DDX_Text(pDX, IDC_EDIT_PROXYIP, m_ProxyIP);
	DDX_Text(pDX, IDC_EDIT_WWMIP, m_WWMIP);
	DDX_Check(pDX, IDC_CHECK_REAALNAME, m_bNetIdentityManage);
}


BEGIN_MESSAGE_MAP(CConfigOfRealName, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigOfRealName ��Ϣ�������

void CConfigOfRealName::OnBnClickedOk()
{
	UpdateData();

	if (m_bNetIdentityManage != m_NetBarConfig2.GetNetIdentityManage())
	{
		m_NetBarConfig2.SetNetIdentityManage(m_bNetIdentityManage);
	}

	m_WWMIP.Trim();

	if (m_WWMIP != m_NetBarConfig2.GetWWMIP())
	{
		m_NetBarConfig2.SetWWMIP(m_WWMIP);
	}

	m_ProxyIP.Trim();

	//{ 2011/04/19-gxx: �޸Ĳ���IP��ȵ��жϣ�ֱ�ӱ���
	CNetBarConfig::GetInstance()->SetProxyIP(m_ProxyIP);
	//}

	CBCGPDialog::OnOK();
}

BOOL CConfigOfRealName::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	SetEditFont();

	m_NetBarConfig2.LoadFromDB();

	m_ProxyIP = CNetBarConfig::GetInstance()->GetProxyIP();
	m_WWMIP = m_NetBarConfig2.GetWWMIP();
	m_bNetIdentityManage = m_NetBarConfig2.GetNetIdentityManage();

	UpdateData(FALSE);

	switch (CNetBarConfig::GetInstance()->GetAuditType())
	{
	case 1:
	case 6:
		m_strWWMIPTIP = LOAD_STRING(IDS_RZX_NAME_IP);//_T("���������IP��");
		break;

	case 2:
		m_strWWMIPTIP = LOAD_STRING(IDS_GLV_NAME_IP);//_T("����������IP��");

		GetDlgItem(IDC_EDIT_PROXYIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WWMIP)->EnableWindow(FALSE);

		break;

	case 5:
		m_strWWMIPTIP = LOAD_STRING(IDS_WD_NAME_IP);//_T("����ʵ��������IP��");
		break;

	default:
		IBA_ASSERT2(0,"�޷�ʶ��ĵ�����ʵ��,������Ҫ����...");
		break;
	}

	if (!m_strWWMIPTIP.IsEmpty())
	{
		GetDlgItem(IDC_STATIC_WWMIP)->SetWindowText(m_strWWMIPTIP);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
