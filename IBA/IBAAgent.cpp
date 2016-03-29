// IBAAgent.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include ".\ibaagent.h"


// CDlgIBAAgent �Ի���

IMPLEMENT_DYNAMIC(CDlgIBAAgent, CBCGPDialog)
CDlgIBAAgent::CDlgIBAAgent(CWnd* pParent /*=NULL*/)
	: CStringLoaderDlg<CBCGPDialog>(CDlgIBAAgent::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgIBAAgent::~CDlgIBAAgent()
{
}

void CDlgIBAAgent::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_AGENTNAME, m_strAgentName);
	DDX_Text(pDX, IDC_STATIC_MOBILEPHONE, m_strMobilePhone);
	DDX_Text(pDX, IDC_STATIC_REGULARTEL, m_strRegularTel);

	DDX_Control(pDX, IDC_BUTTON_EMAIL, m_btnEmail);
	DDX_Control(pDX, IDC_BUTTON_ONLINE, m_btnOnline);
	DDX_Control(pDX, IDC_BUTTON_BBS, m_btnBBS);
}


BEGIN_MESSAGE_MAP(CDlgIBAAgent, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_EMAIL, OnBnClickedButtonEmail)
	ON_BN_CLICKED(IDC_BUTTON_BBS, OnBnClickedButtonBbs)
	ON_BN_CLICKED(IDC_BUTTON_ONLINE, OnBnClickedButtonOnline)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgIBAAgent ��Ϣ�������

BOOL CDlgIBAAgent::OnInitDialog()
{
	InitData();

	CStringLoaderDlg<CBCGPDialog>::OnInitDialog();

	m_btnBBS.SizeToContent();
	m_btnEmail.SizeToContent();
	m_btnOnline.SizeToContent();
	
	m_btnEmail.SetURLPrefix(_T("mailto:"));
	m_btnOnline.SetURL(_T("http://live.webeye.net.cn/v8x/liveIcon.aspx?w=4664"));

	CIBAHelpper::CreateFont(m_font, 20 ,600);

	GetDlgItem(IDC_STATIC_400TEL)->SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ��ʼ������������ֵ
void CDlgIBAAgent::InitData()
{
	m_strAgentName = CNetBarConfig::GetInstance()->GetAgentName();

	m_strMobilePhone = CNetBarConfig::GetInstance()->GetAgentMobilePhone();

	m_strRegularTel = CNetBarConfig::GetInstance()->GetAgentregularTel();

}
	
void CDlgIBAAgent::OnBnClickedButtonEmail()
{
	m_btnEmail.OpenURL();
}

void CDlgIBAAgent::OnBnClickedButtonBbs()
{
	m_btnBBS.OpenURL();
}

void CDlgIBAAgent::OnBnClickedButtonOnline()
{
	m_btnOnline.OpenURL();
}

HBRUSH CDlgIBAAgent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_400TEL)
		{
			pDC->SetTextColor(RGB(0, 0, 255));		
		}
	}

	return hbr;
}
