// LockDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Lock.h"
#include "LockDlg.h"
#include ".\lockdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLockDlg �Ի���



CLockDlg::CLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLockDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_plockScreendlg = NULL;
}

void CLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_LOCKBTN, m_LockBtn);
	DDX_Control(pDX, IDCANCEL, m_CancleBtn);
	DDX_Control(pDX, IDC_PASSWORDEDIT, m_PassWordEdit);
	DDX_Control(pDX, IDC_RPASSWORDEDIT, m_RPassWordEdit);
}

BEGIN_MESSAGE_MAP(CLockDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CHECKPASSWORD, OnCheckPassword)
	ON_BN_CLICKED(IDB_LOCKBTN, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLockDlg ��Ϣ�������

BOOL CLockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SetWindowText(_T("��ţ������"));

	m_ImageBg.LoadFromOtherResource(AfxGetResourceHandle(), IDP_PASSWORDBKPNG, _T("PNG"));

	if (!m_ImageBg.IsNull())
	{
		m_ImageBg.DoForPNG();
		SetWindowPos(this, 0, 0, m_ImageBg.GetWidth(), m_ImageBg.GetHeight(), SWP_NOMOVE | SWP_NOZORDER );
	}

	CRgn MyRgn;
	RECT rectClient;
	GetClientRect(&rectClient);
	rectClient.right += 1;
	rectClient.bottom += 2;

	MoveWindow(&rectClient, TRUE);
	MyRgn.CreateRoundRectRgn(0, 0, rectClient.right, rectClient.bottom, 5, 5);
	SetWindowRgn(MyRgn, TRUE);

	CRect rect(5, 20, 10, 10);

	m_LockBtn.ModifyStyle(0, BS_OWNERDRAW);
	m_LockBtn.LoadPng(IDP_LOCKPNG, 2);

	m_CancleBtn.ModifyStyle(0, BS_OWNERDRAW);
	m_CancleBtn.LoadPng(IDR_CANCLEPNG, 2);

	GetDlgItem(IDC_PASSWORDEDIT)->SetWindowPos(NULL, 85, 54, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

	GetDlgItem(IDC_RPASSWORDEDIT)->SetWindowPos(NULL, 85, 98, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

	CreateFont();

	((CEdit*)GetDlgItem(IDC_PASSWORDEDIT))->SetLimitText(20);
	GetDlgItem(IDC_PASSWORDEDIT)->SetFont(&m_Font);
	//GetDlgItem(IDC_PASSWORDEDIT)->SetFocus();

	((CEdit*)GetDlgItem(IDC_RPASSWORDEDIT))->SetLimitText(20);
	GetDlgItem(IDC_RPASSWORDEDIT)->SetFont(&m_Font);

	SetWindowPos(&wndTopMost, 0, 0, 0, 0,  SWP_NOMOVE | SWP_NOSIZE);

	return FALSE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLockDlg::OnPaint() 
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
HCURSOR CLockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CLockDlg::OnEraseBkgnd(CDC* pDC)
{
	
	pDC->SelectObject(m_Font2);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	if (!m_ImageBg.IsNull())
	{
		CRect rect;
		GetWindowRect(rect);
		m_ImageBg.Draw(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height());

		pDC->DrawText(_T("�������룺"), CRect(20,56,100,79), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		pDC->DrawText(_T("ȷ�����룺"), CRect(20,98,100,121), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	return TRUE;
}

void CLockDlg::CreateFont()
{

	m_Font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����"));  


	m_Font2.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����"));
	

}

BOOL CLockDlg::CreateLockDlg()
{
	BOOL bl = FALSE;
	if (!m_plockScreendlg)
	{
		m_plockScreendlg = new CLockScreenDlg(AfxGetMainWnd());
		bl = m_plockScreendlg->Create(IDD_LOCKSCREENDLG, AfxGetMainWnd());

	}

	return bl;
}

void CLockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); 

	CDialog::OnLButtonDown(nFlags, point);
}

void CLockDlg::OnBnClickedOk()
{
	CString strPassWord;

	GetDlgItem(IDC_PASSWORDEDIT)->GetWindowText(strPassWord);

	CString strRPassWord;

	GetDlgItem(IDC_RPASSWORDEDIT)->GetWindowText(strRPassWord);
	
	if (strPassWord.IsEmpty())
	{
		MessageBox("������������룡", "������", MB_ICONINFORMATION);

		return;
	}

	if (strRPassWord.IsEmpty())
	{
		//MessageBox("��������֤����!", "������", MB_ICONINFORMATION);

		GetDlgItem(IDC_RPASSWORDEDIT)->SetFocus();

		return;
	}

	if (strRPassWord != strPassWord)
	{
		MessageBox("������������벻һ����", "������", MB_ICONINFORMATION);

		return;
	}

	m_strPassWord = strPassWord;

	if (CreateLockDlg())
	{
		ShowWindow(SW_HIDE);
	}

}

void CLockDlg::OnBnClickedCancel()
{
	DestroyWindow();
}

LRESULT CLockDlg::OnCheckPassword( WPARAM wParam, LPARAM lParam )
{
	CString* pwd = (CString*)lParam;

	if (*pwd != m_strPassWord)
	{
		return -1;
	}

	return 0;
}
void CLockDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_plockScreendlg)
	{
		delete m_plockScreendlg;

		m_plockScreendlg = NULL;
	}
}

void CLockDlg::PostNcDestroy()
{
	delete this;

	CDialog::PostNcDestroy();
}

void CLockDlg::OnClose()
{
	return;
}
