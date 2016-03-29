// ID2MonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ID2Monitor.h"
#include "ID2MonitorDlg.h"
#include ".\id2monitordlg.h"

//#include "..\\DataX\\G2XDataX.h"
#include <atlbase.h>
#include "wndfinder.h"

#include "G2XDestructorRunner.h"
#include "capturebase.h"
#include "capturehengxin.h"
#include "DlgAdvance.h"

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


//-------------------------------------------------------------------------
// CID2MonitorDlg �Ի���
const UINT MSG_SCAN_WORK = WM_USER + 101;

HWND g_hWnd = NULL;

extern "C"
VOID CALLBACK ScanCallBack(int idType)
{
	::SendMessage(g_hWnd, MSG_SCAN_WORK, 0, 0);
}

#define DataX_RN2DDN_CARDINFO			101	//ˢ��

#pragma pack(1)

struct DataX_UserInfo    //�û���Ϣ
{
	DWORD cbLen;		 //�ṹ��С
	DWORD dwVer;		 //�汾
	
	char  realNameType;  //ʵ������

	char  cardNo[31];	 //����
	int   cardType;		 //�����ͣ��μ���1��������
	
	char  idNo[31];		 //֤������
	int   idType;        //֤�����ͣ��μ���2��֤������
	
	char  password[31];  //����
	int   passFlag;      //�����־��0��ʾ�������룻1��ʾָ������
	
	char  name[31];      //�û�����
	char  sex;           //�Ա�

	char termId[MAX_COMPUTERNAME_LENGTH + 1]; //��������
	char termIp[16];//����IP

	DWORD dwReserve1;
	DWORD dwReserve2;

	char  Reserve1[128]; //�����ռ�
};

#pragma pack()


CID2MonitorDlg::CID2MonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CID2MonitorDlg::IDD, pParent)
	, m_nTimerGap(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bDeviceOpen = FALSE;

	m_hIBAWnd = NULL;

	CString strPath;
	GetModuleFileName(NULL,strPath.GetBuffer(255),255);
	strPath.ReleaseBuffer();

	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	m_strIniFile = strPath + "\\ScanReco.ini";
	m_nScanType = (INT)::GetPrivateProfileInt("SCAN_LOAD", "LOAD_ID",0, m_strIniFile);

	m_nTimerGap = (UINT)::GetPrivateProfileInt("MonitorConfig", "MonitorTime", 1, m_strIniFile);



	LOGFONT lf = {0};
	m_boldFont.CreateStockObject(DEFAULT_GUI_FONT);
	m_boldFont.GetLogFont(&lf);
	m_boldFont.DeleteObject();
	lf.lfWidth = 0;
	lf.lfHeight = 0 - 20;
	lf.lfWeight = 600;

	m_boldFont.CreateFontIndirect(&lf);

	m_bScanFinish = TRUE;
}

void CID2MonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combTypes);
	DDX_Text(pDX, IDC_EDIT1, m_nTimerGap);
	DDX_Control(pDX, IDC_COMBO2, m_combSystem);
}

BEGIN_MESSAGE_MAP(CID2MonitorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_MESSAGE(MSG_SCAN_WORK, OnScanWork)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SCAN, OnBnClickedBtnScan)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_COMMAND(ID_ACCELERATOR_SCAN, OnBnClickedBtnScan)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_READ_OTHER, OnBnClickedBtnReadOther)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BTN_ADVANCE, OnBnClickedBtnAdvance)
END_MESSAGE_MAP()


// CID2MonitorDlg ��Ϣ�������

BOOL CID2MonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!RegisterHotKey(m_hWnd, IDC_BTN_SCAN, MOD_CONTROL|MOD_ALT, 'Q'))
	{
		MessageBox(_T("ע���ȼ� Ctrl+Atl+Q ʧ��!"));
	}
	if (!RegisterHotKey(m_hWnd, IDC_BTN_READ_OTHER, MOD_CONTROL|MOD_ALT, 'W'))
	{
		MessageBox(_T("ע���ȼ� Ctrl+Atl+W ʧ��!"));
	}

	const char *str_types[9] = {
		"û��ɨ����",
        "E��ͨ",
		"��ʶ",
		"�廪��ͨ",
		"����",
		"���ס�����𿨶���֤",
		"����",
		"����ɨ����",
		"���Ӷ���֤"
	};
	for (int i=0; i < 9; i++)
	{
		m_combTypes.AddString(str_types[i]);
	}
	
	m_combTypes.SetCurSel(m_nScanType);

	g_hWnd = m_hWnd;
	
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

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	
	CWnd* pWnd = GetDlgItem(IDC_BTN_SCAN);
	pWnd->SetFont(&m_boldFont);
	pWnd = GetDlgItem(IDC_BTN_READ_OTHER);
	pWnd->SetFont(&m_boldFont);

	int nCX = ::GetSystemMetrics(SM_CXSCREEN);
	int nCY = ::GetSystemMetrics(SM_CYSCREEN);
	CRect rcClient;
	GetClientRect(rcClient);
	int x = nCX - rcClient.Width() - 4;
	int y = nCY - rcClient.Height() - 100;
	SetWindowPos(NULL, x,y,rcClient.Width(),rcClient.Width(), SWP_NOSIZE);

	UpdateData(FALSE);

	m_captureManager.ReadFromConfig();

	for (int i=0; i < m_captureManager.GetCount(); i++)
	{
		CaptureItem* pItem = &m_captureManager.GetItem(i);
		m_combSystem.AddString(pItem->strName);
		m_combSystem.SetItemData(i, (DWORD_PTR)pItem);
	}

	int nSel = theApp.GetProfileInt("Config","SystemType", 0);
	if (nSel < m_combSystem.GetCount())
	{
		m_combSystem.SetCurSel(nSel);
	}

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CID2MonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CID2MonitorDlg::OnPaint() 
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
HCURSOR CID2MonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CID2MonitorDlg::OnBnClickedBtnStart()
{
	UpdateData(TRUE);
	if (m_nTimerGap == 0)
	{
		m_nTimerGap = 1;
	}
	CString str;
	str.Format("%d", m_nTimerGap);
	(UINT)::WritePrivateProfileString("MonitorConfig", "MonitorTime", str, m_strIniFile);

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);

	GetDlgItem(IDC_BTN_SCAN)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

	SetTimer(100, m_nTimerGap*1000, NULL);
}

void CID2MonitorDlg::OnBnClickedBtnStop()
{
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_SCAN)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	
	KillTimer(100);
}

LRESULT CID2MonitorDlg::OnScanWork(WPARAM wParam, LPARAM lParam)
{
	GXX_LOG_HIT("�յ�ˢ������");
	return 0;
}
void CID2MonitorDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 100)
	{
		if (m_bScanFinish)
		{
			OnBnClickedBtnScan();
		}
		else
		{
			return;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CID2MonitorDlg::ScanInfo()
{
	// ��ȡ����Ϣ
	IDENTITY_INFO idInfo;
	ZeroMemory(&idInfo, sizeof IDENTITY_INFO);

	CString strPath;
	GetModuleFileName(NULL,strPath.GetBuffer(255),255);
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath += _T("\\ID.jpg");

	BOOL bReadOK = FALSE;

	//int iRet = ::ScanImage(11, (LPSTR)strPath.GetString());
	//GXX_LOG_HIT("ScanImage return %d", iRet);


	if (!IsSupportRecognise(11))
	{
		GXX_LOG_HIT("device does not support recognise IdType");

		GXX_LOG_HIT("Try device does support Reader IdType?");
		if (!IsSupportReader())
		{
			GXX_LOG_HIT("device does not support Reader IdType");

			MessageBox("��������ɨ���ǲ�֧�ֶ���","����",MB_OK|MB_ICONWARNING);
		}
		else
		{


			int iRes = ReadCard(idInfo, (char*)strPath.GetString());

			GXX_LOG_HIT("ReadCard return %d", iRes);

			if (iRes > 0)
			{
				bReadOK = TRUE;
			}
		}
	}
	else
	{
		GXX_LOG_HIT("device support recognise IdType");

		int iRes = ScanImage(11, (char*)strPath.GetString());
		if (iRes == 1)
		{
			RecogniseImage(11, idInfo, "");
			bReadOK = TRUE;
		}
		else
		{
			GXX_LOG_HIT("ScanImage return %d", iRes);
		}
		
	}




	if (bReadOK)
	{
		// ��ˢ����Ϣ,������Ϣ������̨

		GXX_LOG_HIT("name:%s, number:%s, sex:%s", idInfo.name, idInfo.number, idInfo.sex);


		CString strNetId;             // �����˺�
		CString strIdNumber;          // ֤������
		int		nIdType = 12;         // ����֤
		CString strName;              // ����
		CString strPassword = "123";  // ����

//#ifdef DEBUG
//		strNetId = "420114198607072210";
//		strIdNumber = "420114198607072210";
//		strName = "С��";
//#else

		strIdNumber = idInfo.number;
		strNetId = strIdNumber;
		strName = idInfo.name;
//#endif
		//static CString s_strIdNumber("");
		//static DWORD s_Tick = 0;

		//// ���ʱ����5����֮�ڣ���֤��������ϴ���ͬ�����ٷ�������
		//if((GetTickCount() - s_Tick < 5000) 
		//	&& (s_strIdNumber == strIdNumber))
		//{
		//	
		//}
		//else
		//{
		//	SendCardInfo(m_hIBAWnd, strNetId, strIdNumber, strName);
		//	s_Tick = GetTickCount();		// ��������˲�Ҫ����ʱ��
		//}
		//s_strIdNumber = strIdNumber;

		// qsc-2013-3-22 14:29:41
		// �������ɱ���ʹ���Զ�ˢ�����ܵ�ʱ��ˢһ�ο�A������ˢ����ʱ�򣬻�һֱ���Ϳ�A�������Ϣ
		// ������һЩ�������Ļ���û�����

		// ����������İ취ʵ�֣����ˢ��һ�ο�A��֪��ˢ������һ�ſ�B֮ǰ����A������û����ˢ�����ᷢ�������Ϣ
		// �������汨������İ취���ֵ�Ч��ȴ�ǣ�ˢ��һ�ο�A���ᷢ�������Ϣ����ˢ����ʱ�򲻻��ٷ��������Ϣ��
		// ���ǣ���������ˢ��A�����û���
		static CString s_strIdNumber("");

		// ֤��������ϴβ���ͬ���ŷ�������
		if(0 != s_strIdNumber.CompareNoCase(strIdNumber))
		{
			SendCardInfo(m_hIBAWnd, strNetId, strIdNumber, strName);
			s_strIdNumber = strIdNumber;
		}	
		
	}
	else
	{
		GXX_LOG_WARNING("û��ɨ�����");
	}
}
void CID2MonitorDlg::OnBnClickedBtnScan()
{
	m_bScanFinish = FALSE; // ��δ��ɹ����ı�־
	CG2XDestructorRunner<CID2MonitorDlg,void,void> autRun(
		&CID2MonitorDlg::SetScanFinish,	this);

	try{

	int iRes = OpenDevice();
	GXX_LOG_HIT("OpenDevice return %d", iRes);
	if (iRes == 1)
	{
		m_bDeviceOpen = TRUE;
		GXX_LOG_HIT("���豸�ɹ�");
	}
	else
	{
		m_bDeviceOpen = FALSE;
		

		char err[1000] = {0};
		GetErrorMessage(iRes, err);
		MessageBox(err, "���豸����", MB_OK|MB_ICONWARNING);

		GXX_LOG_WARNING("���豸ʧ��, error:%s", err);
		return;
	}

	if (m_bDeviceOpen)
	{
		if (CheckIBARunning())
		{
			ScanInfo();
		}
		
	}
	
	if (m_bDeviceOpen)
	{
		iRes = CloseDevice();
		GXX_LOG_HIT("CloseDevice return %d", iRes);
		if (iRes > 0)
		{
			GXX_LOG_HIT("�ر��豸�ɹ�");
		}
		else
		{
			GXX_LOG_WARNING("�ر��豸ʧ��");
		}
	}

	}// end try
	catch(...)
	{
		GXX_LOG_HIT("����ʱ�������쳣");
		MessageBox(_T("����ʱ�������쳣"));
	}
}

BOOL CID2MonitorDlg::CheckIBARunning()
{
	HWND hWnd = ::FindWindow("IBA_MAINWINDOW_CLASS", NULL);
	if (hWnd == NULL)
	{
		MessageBox("����̨δ�������������в���½��IBA","����",MB_OK|MB_ICONWARNING);
	}
	m_hIBAWnd = hWnd;
	return m_hIBAWnd != NULL;
}
void CID2MonitorDlg::OnCbnSelchangeCombo1()
{
	INT iSel = m_combTypes.GetCurSel();

	m_nScanType = iSel;

	CString strTmp;
	strTmp.Format("%d", iSel);
	::WritePrivateProfileString("SCAN_LOAD", "LOAD_ID",strTmp,m_strIniFile);
}

BOOL CID2MonitorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedBtnScan();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CID2MonitorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	/*if (pWnd->GetDlgCtrlID() == IDC_BTN_SCAN)
	{
		

	}*/
	return hbr;
}

LRESULT CID2MonitorDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (m_bScanFinish)
	{
		if (wParam == IDC_BTN_SCAN)
		{
			OnBnClickedBtnScan();
		}
		else if(wParam == IDC_BTN_READ_OTHER)
		{
			OnBnClickedBtnReadOther();
		}
	}
	

	return 0;
}
void CID2MonitorDlg::OnDestroy()
{
	UnregisterHotKey(m_hWnd, IDC_BTN_SCAN);
	CDialog::OnDestroy();

	
}

void CID2MonitorDlg::OnBnClickedBtnReadOther()
{
	m_bScanFinish = FALSE; // ��δ��ɹ����ı�־
	CG2XDestructorRunner<CID2MonitorDlg,void,void> autRun(
		&CID2MonitorDlg::SetScanFinish,	this);

	if (!CheckIBARunning())
	{
		return ;
	}

	
	int iSel = m_combSystem.GetCurSel();
	if (iSel < 0)
	{
		return;
	}

	CaptureItem* pItem = (CaptureItem*)m_combSystem.GetItemData(iSel);
	CCaptureBase cap;
	if (cap.CreateFrom(*pItem))
	{

	//}

	//CWndFinder wF;
	////wF.Init(_T("IBA_MAINWINDOW_CLASS"),NULL);
	//if (wF.Init(_T("ThunderRT6FormDC"), _T("���ٲټ�")))
	//{	
		/*CString strNetId;
		CString strName;
		CString strIdNumber;
		wF.GetChildCtrlText(0x1A,strNetId);
		wF.GetChildCtrlText(0x413,strName);
		wF.GetChildCtrlText(0x423,strIdNumber);

		
		CString strFo;
		strFo.Format(_T("�����˺�:%s\r\n����:%s\r\n֤������:%s"), strNetId, strName, strIdNumber);


		MessageBox(strFo);*/

		cap.CaptureAndSendEx(m_hIBAWnd);
	}
	else
	{
		MessageBox("��Ϣ����δ��!");
	}
}

void CID2MonitorDlg::OnClose()
{
	if (MessageBox(_T("ȷ���˳���?"), NULL, MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}

	CDialog::OnClose();
}

void CID2MonitorDlg::SetScanFinish()
{
	m_bScanFinish = TRUE;
}
void CID2MonitorDlg::OnCbnSelchangeCombo2()
{
	int iSel = m_combSystem.GetCurSel();
	if (iSel >= 0)
	{
		theApp.WriteProfileInt("Config","SystemType", iSel);
	}
}

void CID2MonitorDlg::OnBnClickedBtnAdvance()
{
	CDlgAdvance dlg;
	if (dlg.DoModal() == IDOK)
	{

	}
}	
