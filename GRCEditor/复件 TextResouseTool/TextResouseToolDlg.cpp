// TextResouseToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TextResouseTool.h"
#include "TextResouseToolDlg.h"
#include ".\textresousetooldlg.h"

#include "FileAnalyzer.h"
#include <afx.h>
#include "DlgFindWnd.h"
#include "DlgSetting.h"

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


// CTextResouseToolDlg �Ի���



CTextResouseToolDlg::CTextResouseToolDlg(CWnd* pParent /*=NULL*/)
	: CStringLoaderDlg<CDialog>(CTextResouseToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextResouseToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STRING, m_listStrings);
}

BEGIN_MESSAGE_MAP(CTextResouseToolDlg, CDialog)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_LOADRC, OnBnClickedLoadrc)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS, OnBnClickedBtnSaveAs)
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_MESSAGE(WM_ITEMCHANGE, OnItemChanged)
	ON_WM_CLOSE()
	ON_COMMAND(ID_FIND, OnSearch)
	ON_COMMAND(ID_FIND_NEXT, OnSearchNext)
	ON_COMMAND(ID_OPEN, OnBnClickedOpen)
	ON_COMMAND(ID_OPEN_RC, OnBnClickedLoadrc)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_SAVE, OnBnClickedSave)
	ON_COMMAND(ID_SAVE_AS, OnBnClickedBtnSaveAs)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_INSERT, OnInsert)
	ON_COMMAND(ID_ADD, OnAdd)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(ID_SAVE, OnBnClickedSave)
	ON_COMMAND(ID_SETTING, OnSetting)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STRING, OnNMRclickListString)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(ID_SAVE, OnUpdateSave)
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()


// CTextResouseToolDlg ��Ϣ�������

BOOL CTextResouseToolDlg::OnInitDialog()
{
	//SetStringID(IDC_BTN_SAVE_AS);
	////SetStringID(IDC_OPEN);
	//SetStringID(IDC_LOADRC);
	//SetStringID(ID_SAVE);
	//SetStringID(IDCANCEL);
	//SetStringID(IDC_OPEN);//, UINT(IDC_OPEN) |(UINT(IDD_TEXTRESOUSETOOL_DIALOG) << 16));


	CStringLoaderDlg<CDialog>::OnInitDialog();

	WINDOWPLACEMENT wp;
	m_listStrings.GetWindowPlacement(&wp);
	m_nDefaultTop = wp.rcNormalPosition.top;
	if (m_nDefaultTop<=5)
	{
		m_nDefaultTop = 30;
	}

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

	m_listStrings.InitList();

	GetDlgItem(ID_SAVE)->EnableWindow(FALSE);

	m_menuMain.LoadMenu(IDR_MAINMENU);
	SetMenu(&m_menuMain);
	m_menuMain.EnableMenuItem(ID_SAVE, MF_DISABLED|MF_GRAYED);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTextResouseToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTextResouseToolDlg::OnPaint() 
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
HCURSOR CTextResouseToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTextResouseToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (IsWindow(m_listStrings.GetSafeHwnd()))
	{
		CRect rc;
		GetClientRect(rc);
		rc.DeflateRect(8,m_nDefaultTop,8,5);
		m_listStrings.MoveWindow(rc);
	}
}

BOOL CTextResouseToolDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN)
		{
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTextResouseToolDlg::OnUpdateDelete(CCmdUI* pCmdUI)
{
	int nRow = m_listStrings.GetSelectionMark();
	pCmdUI->Enable(nRow>=0);
}
void CTextResouseToolDlg::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{
	CStringLoaderDlg<CDialog>::OnInitMenuPopup(pMenu, nIndex, bSysMenu);

	UpdateMenu(pMenu);
}

void CTextResouseToolDlg::UpdateMenu(CMenu* pMenu)
{
	CCmdUI cmdUI; 
	cmdUI.m_nIndexMax = pMenu-> GetMenuItemCount(); 
	for(UINT n = 0; n < cmdUI.m_nIndexMax; ++n) 
	{ 
		CMenu*   pSubMenu   =   pMenu-> GetSubMenu(n); 
		if(pSubMenu   ==   NULL) 
		{ 
			cmdUI.m_nIndex = n; 
			cmdUI.m_nID = pMenu-> GetMenuItemID(n); 
			cmdUI.m_pMenu = pMenu; 
			cmdUI.DoUpdate(this, FALSE);
		} 
		else 
		{ 
			UpdateMenu(pSubMenu); 
		} 
	} 
}


void CTextResouseToolDlg::OnBnClickedLoadrc()
{
	CString strRC;

	CFileDialog fileDlg1(TRUE, _T("*.rc"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("(*.rc)|*.rc||"));
	if (fileDlg1.DoModal() == IDOK)
	{
		strRC = fileDlg1.GetPathName();
		OpenRCFile(strRC);
	}	
	else
	{
		return;
	}

}

void CTextResouseToolDlg::OnBnClickedOpen()
{
	CFileDialog fileDlg(TRUE, _T("*.grc"), NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.grc)|*.grc||"));
	if (fileDlg.DoModal() == IDOK)
	{
		OpenGrcFile(fileDlg.GetPathName().GetString());
	}
	
}

BOOL CTextResouseToolDlg::AddItem(StringItem& it)
{
	return m_listStrings.AddItem(it);
}
void CTextResouseToolDlg::OnBnClickedBtnSaveAs()
{
	CFileDialog fileDlg(FALSE, _T("*.grc"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.grc)|*.grc||"));
	if (fileDlg.DoModal() == IDOK)
	{
		m_listStrings.SaveAs( fileDlg.GetPathName() );

		if (m_strOpenedPath.IsEmpty())
		{
			m_strOpenedPath = fileDlg.GetPathName();
			CString str = _T("grc�ļ��༭���� ");
			str += m_strOpenedPath;
			SetWindowText(str);
		}
	}
}

LRESULT CTextResouseToolDlg::OnItemChanged(WPARAM wParam, LPARAM lParam)
{
	if (wParam == TRUE)
	{
		GetDlgItem(ID_SAVE)->EnableWindow(TRUE);
		m_menuMain.EnableMenuItem(ID_SAVE, MF_ENABLED);
	}
	else
	{
		GetDlgItem(ID_SAVE)->EnableWindow(FALSE);
		m_menuMain.EnableMenuItem(ID_SAVE, MF_DISABLED|MF_GRAYED);

	}
	return 0;
}

static CDlgFindWnd wndFind;
void CTextResouseToolDlg::OnSearch()
{
	if (!IsWindow(wndFind.m_hWnd))
	{
		wndFind.Create(IDD_DLG_FIND,this);
		wndFind.SetOwner(this);
	}
	wndFind.ShowWindow(TRUE);
}
void CTextResouseToolDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTextResouseToolDlg::OnExit()
{
	if (m_listStrings.GetChanged())
	{
		if (MessageBox(_T("�Ƿ񱣴浱ǰ�޸ĵ�����"),NULL,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
		{
			if (m_strOpenedPath.IsEmpty())
			{
				OnBnClickedBtnSaveAs();
			}
			else
			{
				m_listStrings.SaveAs(m_strOpenedPath);
			}
		}
	}
	CDialog::OnCancel();
}
void CTextResouseToolDlg::OnCancel()
{
	OnExit();
}
void CTextResouseToolDlg::OnClose()
{
	OnExit();
}

void CTextResouseToolDlg::OnDelete()
{
	int nRow = m_listStrings.GetSelectionMark();
	if (nRow>=0)
	{
		m_listStrings.DeleteString(nRow);
	}
}

void CTextResouseToolDlg::OnInsert()
{
	int nRow = m_listStrings.GetSelectionMark();
	if (nRow<0)
	{
		nRow = 0;
	}
	m_listStrings.InsertString(nRow);
}

void CTextResouseToolDlg::OnAdd()
{
	m_listStrings.AddString();
}
void CTextResouseToolDlg::OnBnClickedSave()
{
	// ����
	if (m_strOpenedPath.IsEmpty())
	{
		OnBnClickedBtnSaveAs();
		m_listStrings.SetChanged(FALSE);
	}
	else
	{
		m_listStrings.SaveAs(m_strOpenedPath);
		m_listStrings.SetChanged(FALSE);
	}
}

BOOL CTextResouseToolDlg::Search(CString strKeyword, BOOL bCase, BOOL bWholeMatch,int nLastResult, int& nNewResult)
{
	return m_listStrings.Search(strKeyword, bCase, bWholeMatch,nLastResult, nNewResult);
}

void CTextResouseToolDlg::OnSetting()
{
	CDlgSetting ds;
	ds.DoModal();
}

void CTextResouseToolDlg::OnSearchNext()
{
	wndFind.SearchNext();
}
void CTextResouseToolDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR *lpszFileName=new TCHAR[MAX_PATH];
	int nCharactersLen = 0;
	int nFileNum = 0;

	//����ļ����ĳ���
	nCharactersLen = ::DragQueryFile(hDropInfo,0,NULL,100);

	//�����ļ�����
	nFileNum = ::DragQueryFile(hDropInfo,0xFFFFFFFF,lpszFileName,nCharactersLen+1);
	//���ļ���д������
	::DragQueryFile(hDropInfo,0,lpszFileName,nCharactersLen+1);

	CMyString strFile = lpszFileName;
	delete lpszFileName;
	
	CMyString strExt = strFile.Mid(strFile.ReverseFind(_T('.')));
	strExt.ToLower();
	if (strExt == _T(".grc"))
	{
		OpenGrcFile(strFile);
	}
	else if(strExt == _T(".rc"))
	{
		OpenRCFile(strFile.GetData());
	}
	else
	{
		MessageBox(_T("�޷�ʶ����ļ���ʽ")+strExt,NULL,MB_OK|MB_ICONWARNING);
	}


	CStringLoaderDlg<CDialog>::OnDropFiles(hDropInfo);
}

void CTextResouseToolDlg::OpenGrcFile(CMyString strFile)
{
	vector<StringItem> vecItems;
	CFileAnalyzer::LoadFromGrcFile(strFile.GetData(), vecItems);

	if (m_listStrings.GetItemCount() > 0)
	{
		if (IDYES == MessageBox(_T("�Ƿ�������е��ַ�����Դ?\n���ѡ�����ô���е��ַ�����ԴID�ظ����ᱻ����"),NULL,MB_YESNO|MB_ICONQUESTION))
		{
			m_listStrings.DeleteAllStrings();
		}
	}

	if (m_strOpenedPath.IsEmpty())
	{
		m_strOpenedPath = strFile;
		CString str = _T("grc�ļ��༭���� ");
		str += m_strOpenedPath;
		SetWindowText(str);
	}

	m_listStrings.SetChanged(m_listStrings.AddItems(vecItems));
}

void CTextResouseToolDlg::OpenRCFile(CString strRC)
{
	CMyString strTmp;
	strTmp = strRC.GetString();
	strTmp.CutTail(_T('\\'));
	strTmp += _T("\\resource.h");
	CString strResource;
	CFileDialog fileDlg2(TRUE, _T("*.h"), strTmp, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("(*.h)|*.h||"));
	if (fileDlg2.DoModal() == IDOK)
	{
		strResource = fileDlg2.GetPathName();
	}
	else
	{
		return;
	}

	vector<StringItem> vecItems;
	CFileAnalyzer::LoadFromSource(strRC, strResource, vecItems,CGlobalParam::GetInstance()->GetMenuBefore());

	if (m_listStrings.GetItemCount() > 0)
	{
		if (IDYES == MessageBox(_T("�Ƿ�������е��ַ�����Դ?\n\n���ѡ�����ô���е��ַ�����ԴID�ظ����ᱻ����"),NULL,MB_YESNO|MB_ICONQUESTION))
		{
			m_listStrings.DeleteAllStrings();
		}
	}

	m_listStrings.SetChanged(m_listStrings.AddItems(vecItems));
}
void CTextResouseToolDlg::OnNMRclickListString(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu menu;
	menu.LoadMenu(IDR_MAINMENU);
	CMenu* pPopMenu = menu.GetSubMenu(1);
	if (pPopMenu)
	{
		POINT pt;
		::GetCursorPos(&pt);
		pPopMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	}
	*pResult = 0;
}

void CTextResouseToolDlg::OnUpdateSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_listStrings.m_bHaveChanged);
}

void CTextResouseToolDlg::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}