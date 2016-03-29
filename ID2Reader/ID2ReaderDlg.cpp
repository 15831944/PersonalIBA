// ID2ReaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ID2Reader.h"
#include "ID2ReaderDlg.h"
#include ".\id2readerdlg.h"
#include <AtlBase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CID2ReaderDlg �Ի���



CID2ReaderDlg::CID2ReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CID2ReaderDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strNation(_T(""))
	, m_strBirthday(_T(""))
	, m_strAddress(_T(""))
	, m_strIdNumber(_T(""))
	, m_strOrg(_T(""))
	, m_strOutOfDate(_T(""))
	, m_strDate(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bReadOK = FALSE;
	m_bDeviceOpened = FALSE;

	char chModulePath[256] = {0};
	::GetModuleFileName(NULL, chModulePath, 256);
	CString strTmp = chModulePath;
	m_strImgPath = strTmp.Left(strTmp.ReverseFind('\\'));
	m_strImgPath.Append("\\temp.jpg");
	GXX_LOG_HIT("imagePath: %s", m_strImgPath);
}


CID2ReaderDlg::~CID2ReaderDlg()
{
	m_thread.SafeStop();
}

void CID2ReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SEX, m_strSex);
	DDX_Text(pDX, IDC_NATION, m_strNation);
	DDX_Text(pDX, IDC_BIRTHDAY, m_strBirthday);
	DDX_Text(pDX, IDC_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_IDNUMBER, m_strIdNumber);
	DDX_Text(pDX, IDC_ORGAN, m_strOrg);
	DDX_Text(pDX, IDC_OUTOFDATE, m_strOutOfDate);
	DDX_Text(pDX, IDC_DATE, m_strDate);
	DDX_Control(pDX, IDC_STATIC_PIC, m_staticPic);
}

BEGIN_MESSAGE_MAP(CID2ReaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_UPDATE_DATA, OnUIUpdateData)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CID2ReaderDlg ��Ϣ�������

BOOL CID2ReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText("Dodonew Reader");

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//SetTimer(1, 1000, NULL);
	m_thread.Create(this, false, true);

	SetTimer(1, 1, NULL);
	
	// ɾ�������������־�ļ�
	m_threadDelFile.SetDelDay(7);

	CString strPath("");
	::GetModuleFileName(NULL, strPath.GetBuffer(512), 512);
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);

	m_threadDelFile.SetDelDir(strPath);	
	m_threadDelFile.Start();

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CID2ReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CID2ReaderDlg::OnPaint() 
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
HCURSOR CID2ReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef int(__stdcall *CVR_InitComm)(int nPort);
typedef int(__stdcall *CVR_Authenticate)();
typedef int(__stdcall *CVR_Read_Content)(int Active);
typedef int(__stdcall *CVR_CloseComm)();

void CID2ReaderDlg::OnBnClickedOk()
{
	HWND hWnd = ::FindWindow("IBA_MAINWINDOW_CLASS", NULL);

	CRegKey reg;

	if(0 == reg.Create(HKEY_CURRENT_USER, "SoftWare\\Sunlike\\IBA\\RealName\\Base\\NewUser"))
	{
		reg.SetStringValue("IDNo", m_strIdNumber);
		reg.SetStringValue("CertificateID", m_strIdNumber);
		reg.SetDWORDValue("CertificateType", 12);
		reg.SetStringValue("Name", m_strName);
		reg.SetStringValue("Password", "");

		// 2013-12-18 18:18:55-qsc
		reg.SetStringValue("Nation", m_strNation);	//����
		reg.SetStringValue("Sex", m_strSex);
		reg.SetStringValue("Birthday", m_strBirthday);	//��������
		reg.SetStringValue("Address", m_strAddress);	//��ַ
		reg.SetStringValue("Department", m_strOrg);	 //��֤����
		reg.SetStringValue("Signdate", m_strDate);	 //ǩ������
		reg.SetStringValue("OutOfDate", m_strOutOfDate);	 //��Ч��
		/*time_t tm = time(NULL);
		CString strTm;
		strTm.Format("%ld", tm);
		reg.SetStringValue(_T("tValue1"), strTm);
		CString strOrgCode;
		strOrgCode += lpszNetid;
		strOrgCode.AppendFormat("%d", tm);
		INT nLen = strOrgCode.GetLength();
		BYTE *pBuff = new BYTE[nLen+8];
		memcpy(pBuff,strOrgCode.GetString(), nLen);
		BYTE byCheck[8] = {0x03,0xF1,0x00,0x0A,0x0B,0xED,0x02,0xC1};
		memcpy(pBuff+nLen, byCheck, 8);
		char pCode[33] = {0};
		G2XUtility::CG2XMD5::CalculateMD5(pBuff, nLen+8, pCode);
		reg.SetStringValue("tValue2", pCode);
		delete []pBuff;*/

		reg.Close();
	}
	else
	{
		GXX_LOG_HIT("����ʧ��!");
	}

	if( hWnd )
	{
		::SendMessage(hWnd, WM_USER+100, 999, 0);
		//::SendMessage(hIBAWnd, WM_REALNAME, 0, 100);

		ShowWindow(SW_HIDE);
	}
	else
	{
		::MessageBox(NULL, "��ţ����̨û������!", "��ʾ", MB_OK);
		GXX_LOG_HIT("��ţ����̨û������!");
		return ;
	}


	return;
}


HBRUSH CID2ReaderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() != IDC_STATIC)
		{
			pDC->SetTextColor(RGB(0,0,255));
		}
	}
	return hbr;
}

void CID2ReaderDlg::Clear()
{
	m_bReadOK = FALSE;

	m_strName.Empty();
	m_strSex.Empty();
	m_strNation.Empty();
	m_strBirthday.Empty();
	m_strAddress.Empty();
	m_strIdNumber.Empty();
	m_strOrg.Empty();
	m_strDate.Empty();
	m_strOutOfDate.Empty();

	m_staticPic.ShowPic(NULL);

	UpdateData(FALSE);
}
void CID2ReaderDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent = 1)
	{
		ShowWindow(SW_HIDE);
		KillTimer(1);
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

void CID2ReaderDlg::didRun(CG2XThread* pThread)
{
	while(1)
	{
		if (pThread->CanSafeExit())
		{
			return;
		}
		CString strImgPath = m_strImgPath;//"C:\\123.jpg";

		if (m_bDeviceOpened != 1)
		{
			m_bDeviceOpened = OpenDevice();
		}
		int nRes = m_bDeviceOpened;
		if (nRes != 1)
		{
			static DWORD s_Tick = GetTickCount() + 60 * 1000;	// ������һ�β��ܴ�ӡ����
			// �����ϴδ�ӡ����1һ���Ӳ��ٴδ�ӡ��־
			if(GetTickCount() - s_Tick >= 60 * 1000)
			{
				GXX_LOG_HIT("error: �򿪶�ȡ���豸ʧ��!");
				s_Tick = GetTickCount();		// ��ӡ�˲�Ҫ����ʱ��
			}

			
			if (IsWindow(m_hWnd))
			{
				SendMessage(WM_UPDATE_DATA, 0);
			}
			Sleep(500);
			continue;
		}
		if (!IsSupportReader())
		{
			static DWORD s_Tick = GetTickCount() + 60 * 1000;	// ������һ�β��ܴ�ӡ����
			// �����ϴδ�ӡ����1һ���Ӳ��ٴδ�ӡ��־
			if(GetTickCount() - s_Tick >= 60 * 1000)
			{
				GXX_LOG_HIT("error: �豸��֧�ֶ���!");
				s_Tick = GetTickCount();		// ��ӡ�˲�Ҫ����ʱ��
			}

			
			CloseDevice();
			if (IsWindow(m_hWnd))
			{
				SendMessage(WM_UPDATE_DATA, 0);
			}
			Sleep(500);
			continue;
		}
		IDENTITY_INFO inf;
		ZeroMemory(&inf, sizeof(inf));
		nRes = ReadCard(inf, (char*)strImgPath.GetString());
		if (nRes)
		{
			m_strName  = inf.name;
			m_strSex = (inf.sex[0] == '0' ? "��" : "Ů");
			m_strNation = inf.nation;
			m_strBirthday = inf.birthday;
			m_strAddress = inf.address;
			m_strIdNumber = inf.number;
			m_strOrg = inf.department;
			m_strDate = inf.signdate;
			m_strDate.Insert(4, '.');
			m_strDate.Insert(7, '.');
			m_strOutOfDate = inf.validterm;

			if (!m_strIdNumber.IsEmpty())
			{
				// GXX_LOG_HIT("��ȡ�ɹ�: %s %s", m_strIdNumber, m_strName);
				if (IsWindow(m_hWnd))
				{
					SendMessage(WM_UPDATE_DATA, 1);
				}
			}
			else
			{
				if (IsWindow(m_hWnd))
				{
					SendMessage(WM_UPDATE_DATA, 0);
				}
			}
		}
		else
		{
			static DWORD s_Tick = GetTickCount() + 60 * 1000;	// ������һ�β��ܴ�ӡ����
			// �����ϴδ�ӡ����1һ���Ӳ��ٴδ�ӡ��־
			if(GetTickCount() - s_Tick >= 60 * 1000)
			{
				GXX_LOG_HIT("error: ��ȡʧ��! ��Ҫ����ˢ֤");
				s_Tick = GetTickCount();		// ��ӡ�˲�Ҫ����ʱ��
			}	
		}

		

		Sleep(500);
	}
}

bool CID2ReaderDlg::didCanExitThread(CG2XThread* pThread)
{
	bool bRes = CG2XThreadDelegate::didCanExitThread(pThread);
	if (bRes)
	{
		return true;
	}
	if (m_hWnd == 0 || !::IsWindow(m_hWnd))
	{
		return true;
	}
	return false;
}

LRESULT CID2ReaderDlg::OnUIUpdateData(WPARAM wParam, LPARAM lParam)
{
	CString strImgPath = m_strImgPath;
	if (wParam == 1)
	{	
		m_bReadOK = TRUE;
		UpdateData(FALSE);
		m_staticPic.ShowPic(strImgPath);
		//ShowWindow(SW_SHOW);
		SetWindowPos(&CWnd::wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
		OnBnClickedOk();
	}
	else
	{
		Clear();
		ShowWindow(SW_HIDE);
	}
	return 0;
}

void CID2ReaderDlg::didInitInstance(CG2XThread* pThread)
{
	GXX_LOG_HIT("thread init instance");
	m_bDeviceOpened = OpenDevice();
	GXX_LOG_HIT("open device return %d", m_bDeviceOpened);
}

int CID2ReaderDlg::didExitInstance(CG2XThread* pThread)
{
	if (m_bDeviceOpened == 1)
	{
		if (!CloseDevice())
		{
			GXX_LOG_HIT("error: �ر��豸ʧ��!");
		}
	}
	GXX_LOG_HIT("thread exit instance");
	return 0;
}
void CID2ReaderDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CID2ReaderDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}
