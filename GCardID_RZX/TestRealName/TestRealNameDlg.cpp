// TestRealNameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestRealName.h"
#include "..\RealName.h"
#include ".\testrealnamedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL SendMessageToIBA(UINT nType)
{
	BOOL bRet = FALSE;

	HWND hIBA = FindWindow(_T("IBA_MAINWINDOW_CLASS"), NULL);

	if (hIBA != NULL)
	{
		bRet = PostMessage(hIBA, WM_REALNAME, nType, NULL);
	}
	else
	{
		AfxMessageBox(_T("����������ţ����̨��"));
	}

	return bRet ? 1 : 0;
}


CTestRealNameDlg::CTestRealNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestRealNameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strIDType = "15";
	m_strPeronId = "123456789";
	m_strPwd = "123";
	m_strName = "�����û�";
}

void CTestRealNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ED_NETID, m_strIDNumber);
	DDX_Text(pDX, IDC_ED_IDTYPE, m_strIDType);
	DDX_Text(pDX, IDC_ED_PERSONID, m_strPeronId);
	DDX_Text(pDX, IDC_ED_PWD, m_strPwd);
	DDX_Text(pDX, IDC_ED_NAME, m_strName);

}

BEGIN_MESSAGE_MAP(CTestRealNameDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CTestRealNameDlg ��Ϣ�������

BOOL CTestRealNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestRealNameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CTestRealNameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestRealNameDlg::OnBnClickedButton1()
{
	UpdateData();

	REQ_REGISTER_INFO REGISTER_INFO = {0};

	lstrcpy(REGISTER_INFO.szCertificateID, m_strPeronId) ;
	lstrcpy(REGISTER_INFO.szName , m_strName) ;
	lstrcpy(REGISTER_INFO.szCompany , "sunlike") ;
	lstrcpy(REGISTER_INFO.szCertificateType , m_strIDType) ;
	lstrcpy(REGISTER_INFO.szCountry , "�й�") ;
	lstrcpy(REGISTER_INFO.szDetail , "ģ��ˢ��") ;

	//IDUser(m_strIDNumber.GetBuffer(), m_strPeronId.GetBuffer(), m_strPwd.GetBuffer(), 1, 1, REGISTER_INFO);
	IDUser(m_strIDNumber.GetBuffer(), m_strPeronId.GetBuffer(), m_strPwd.GetBuffer(), 3, 1, REGISTER_INFO);
}

void CTestRealNameDlg::OnBnClickedButton2()
{
	UpdateData();

	CRegKey regIBA;

	if (ERROR_SUCCESS == regIBA.Create(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\RealName\\GLW\\OpenUser")))
	{
		regIBA.SetStringValue(_T("IDNo"), m_strIDNumber);
		
		regIBA.SetStringValue(_T("Password"), m_strPwd);
		
		//regIBA.SetDWORDValue(_T("IDCardType"), OpenUserInfo.nType);
		//regIBA.SetDWORDValue(_T("Validate"), OpenUserInfo.bValidate);

		regIBA.SetStringValue(_T("Name"), m_strName);
		regIBA.SetDWORDValue(_T("CertificateType"), _ttoi(m_strIDType));
		regIBA.SetStringValue(_T("CertificateID"), m_strPeronId);
		
		//regIBA.SetStringValue(_T("AccreditID"), OpenUserInfo.szAccreditID);
		//regIBA.SetDWORDValue(_T("PswdFlag"), OpenUserInfo.bPswdFlag);
		//regIBA.SetDWORDValue(_T("InfoFlag"), OpenUserInfo.nInfoFlag);

		regIBA.Close();
		
		SendMessageToIBA(1);
	}
}

void CTestRealNameDlg::OnBnClickedOk()
{
	OnOK();
}

void CTestRealNameDlg::OnBnClickedButton3()
{
	UpdateData();

	struct ID_CARD_INFO
	{
		char szISN[20]; //����  ��������Ϊ׼��
		char szPSN[20]; //ӡˢ�루��ʱ��ʹ�ã�
	};

	ID_CARD_INFO id_card_info = {0};

	lstrcpyn(id_card_info.szISN, m_strIDNumber, 20);

	COPYDATASTRUCT copydatastruct = {0};

	copydatastruct.cbData = sizeof ID_CARD_INFO;
	copydatastruct.dwData = 0;
	copydatastruct.lpData = &id_card_info;

	HWND hIBA = ::FindWindow(_T("IBA_MAINWINDOW_CLASS"), NULL);

	if (hIBA != NULL)
	{
		::SendMessage(hIBA, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)(LPVOID)&copydatastruct);
	}
	else
	{
		AfxMessageBox(_T("����������ţ����̨��"));
	}
}
