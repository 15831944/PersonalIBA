
#include "stdafx.h"
#include "IBA.h"
#include "MainFrm.h"
#include "IBADoc.h"
#include "IBALoginDlg.h"
#include "IBAHelpper.h"
#include "MsgBoxDlg.h"
#include "DialogManager.h"
#include "RealName\IBARealName.h"
#include "dal\UpdateDBThread.h"
#include "localcashier.h"
#include <string>
#include "CurrentNetBarUser.h"
#include ".\iba.h"
#include "Process.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CIBAApp


CIBAApp theApp;
DWORD CIBAGlobal::s_dwStartUpTickcount = 0;
HWND CIBAGlobal::m_hCurRegisterDlg = NULL;
HWND CIBAGlobal::m_hCurCreditDlg = NULL;

UINT CIBAApp::m_nTerminateSeconds = 10;

CIBAApp::CIBAApp() 
: CBCGPWorkspace(TRUE)
, m_hinstBCGCBRes(NULL) 
, m_lpCurCashier(NULL)
, m_lpIBAView(NULL)
, m_IBAInstance(_T("Global\\SUNLIKE_IBA")) //�����Ըı��ʵ������
, m_hModuleSAI(NULL), m_bInitAuditInterface(false)
, m_hRes(NULL)
{
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("_IBA_SUNLIKE_MUTEX_"));
	if(hMutex == NULL)
		return ;
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//IBA_LOG(_T("�Ѿ�����һ��ʵ��"));
		TerminateProcess(GetCurrentProcess(), 0);
		
	}
	CIBAGlobal::s_dwStartUpTickcount = GetTickCount();
}

CString CIBAApp::GetWorkPath() const
{
	return m_strWorkPath;
}

CCashier* CIBAApp::GetCurCashier()
{
	if (m_lpCurCashier == NULL)
	{
		/*if (CNetBarConfig::GetInstance()->GetLocalMode())
		{
			m_lpCurCashier = new CLocalCashier;
		} 
		else*/
		{
			m_lpCurCashier = new CCashier;
		}
	}

	ASSERT(m_lpCurCashier);

	return m_lpCurCashier;
}

CIBARealName* CIBAApp::GetRealName()
{
	if (m_lpRealName == NULL)
	{
		m_lpRealName = new CIBARealName;
	}

	ASSERT(m_lpRealName);

	return m_lpRealName;
}

CDialogManager* CIBAApp::GetDialogManager()
{
	if (m_lpDialogManager == NULL)
	{
		m_lpDialogManager = new CDialogManager;
	}

	ASSERT(m_lpDialogManager);

	return m_lpDialogManager;
}

/////////////////////////////////////////////////////////////////////////////
// CIBAApp initialization

BOOL CIBAApp::InitInstance()
{
	UNIT_AUTO_TRACE("CIBAApp::InitInstance()--��ʼ��ʵ��");

	if (!InitIBAEnv())
	{
		return FALSE;
	}
	
	InitIBA();
	UpdateIBA();
	
	
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIBADoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CIBAView));
	AddDocTemplate(pDocTemplate);
	
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (!ProcessShellCommand(cmdInfo)) return FALSE;

	IBA_TRACE("�ȴ���¼...");

	CBrokenNetwork::LoadLocalModeStatus();

	CIBALoginDlg dlg;
	if (IDOK != dlg.DoModal())
	{
		IBA_TRACE("��¼ʧ��!");

		return FALSE;
	}

	IBA_TRACE("��¼�ɹ�.");

	OnLoginOK();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIBAApp message handlers

int CIBAApp::ExitInstance() 
{
	SetTerminateProcess(1);

	if (CNetBarConfig::GetInstance()->GetMonitorAuditProxy() == 1 && CIBAConfig::GetInstance()->GetCashRegisterID() == 1)
	{
		CNetBarConfig::GetInstance()->SetAllowOpenUser(FALSE);
	}
	L2_SAFE_DELETE(m_lpCurCashier);
	L2_SAFE_DELETE(m_lpRealName);
	L2_SAFE_DELETE(m_lpDialogManager);

	//CleanState();

	if (m_hinstBCGCBRes != NULL)
	{
		FreeLibrary(m_hinstBCGCBRes);
	}
	if(m_hRes != NULL)
	{
		FreeLibrary(m_hRes);
	}
	
	BCGCBProCleanUp();

	CL2Process MyProcess;
	MyProcess.Attach(_T("IBAHotKey.exe"));
	MyProcess.Terminate();

	WCHAR wchName[1024] = {0};
	GetModuleFileName(theApp.m_hInstance, wchName,1024);
	int nRes = CWinApp::ExitInstance();

	//if (m_bAutoRestart)
	//{
	//	ShellExecuteW(NULL,_T("open"),wchName,0,NULL,SW_SHOW);	
	//	//WinExec(".\\IBABin\\NBX.exe", SW_HIDE);
	//}	

	CL2Process id2Reader;	id2Reader.Attach(_T("ID2Reader.exe"));	if (id2Reader.IsRunning())	{		id2Reader.Terminate();	}	
	UnInitSunlikeAuditInterface();
	return nRes;
}

void CIBAApp::PreLoadState()
{
	//
}

BOOL CIBAApp::LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd)
{
	BOOL bRet = CBCGPWorkspace::LoadWindowPlacement(rectNormalPosition, nFflags, nShowCmd);

	nShowCmd = SW_HIDE; //Ϊ������������

	return bRet;
}

INT CIBAApp::IBAMsgBox(LPCTSTR lpszPrompt, UINT nType/* = MB_OK | MB_ICONEXCLAMATION*/, BOOL bFormat)
{
	CMsgBoxDlg dlg(AfxGetMainWnd());
	dlg.SetText(lpszPrompt, bFormat);
	dlg.SetType(nType);

	return dlg.DoModal();
}

// QSCADD 2013-6-17 14:49:03 ��ʾ�����Լ��ĶԻ�����ʵ�ֺ�AfxMessageBoxһ����Ч��--�����
INT CIBAApp::IBAMsgBox2(LPCTSTR lpszPrompt, UINT nType/* = MB_OK | MB_ICONEXCLAMATION*/, BOOL bFormat)
{
	// disable windows for modal dialog
	EnableModeless(FALSE);
	HWND hWndTop;
	HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);

	// WINBUG: re-enable the parent window, so that focus is restored 
	// correctly when the dialog is dismissed.
	if (hWnd != hWndTop)
		EnableWindow(hWnd, TRUE);

	CMsgBoxDlg dlg(CWnd::FromHandle(hWnd));
	dlg.SetText(lpszPrompt, bFormat);
	dlg.SetType(nType);
	
	int nResult = dlg.DoModal();
	// re-enable windows
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	EnableModeless(TRUE);

	return nResult;
}

INT CIBAApp::IBAMsgBox(UINT nResID, UINT nType /*= MB_OK | MB_ICONEXCLAMATION*/, BOOL bFormat)
{
	CIBAString strTmp;

	strTmp.LoadString(nResID);

	return IBAMsgBox(strTmp, nType, bFormat);
}

void CIBAApp::InitIBA()
{
	UNIT_AUTO_TRACE("CIBAApp::InitIBA()--");

	m_hinstBCGCBRes = LoadLibrary(_T("BCGCBProResChs.dll"));    // *** - language
	m_hRes = LoadLibrary(GetWorkPath() + _T("\\IBABin\\IBARes.dll"));

	if (m_hinstBCGCBRes != NULL)
	{
		BCGCBProSetResourceHandle(m_hinstBCGCBRes);
	}


	SetRegistryKey(_T("Sunlike"));
	SetRegistryBase(_T("BCGUI"));
	
	WriteProfileString(_T("Setting"), _T("WorkPath"), GetWorkPath());

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager(); //��ݼ�
	
	NS_DAL::CUpdateDBThread UpdateDBThread;
	UpdateDBThread.Start();
	UpdateDBThread.Wait();

	CIBAConfig::GetInstance();
	CNetBarConfig::GetInstance();
	CIBAConfig::GetInstance()->LoadConfig();

	// QSCT1
	// 2014-4-25 qsc
	// ����sfregĿ¼
	CString strSFREGPath = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\");
	CIBAHelpper::MakeSurePathExists(strSFREGPath);

	strSFREGPath = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\sfreg.exe");
	CFileVersionInfo fvi;
	if (!fvi.Create(strSFREGPath))
	{
		IBA_LOG0(_T("error: ��ȡsfreg.exe�İ汾��ʧ��"));
	}
	else
	{
		IBA_LOG(_T("sfreg.exe version :%s"), fvi.GetProductVersion());
	}
}

void CIBAApp::UpdateIBA()
{
	UNIT_TRACE("CIBAApp::UpdateIBA()");

	WritePrivateProfileString(_T("INFO"), _T("IP"), CNetBarConfig::GetInstance()->GetLocalServerIP(), _T(".\\IBABin\\NBX.ini"));
	WritePrivateProfileString(_T("INFO"), _T("LOCATION"), _T("2"), _T(".\\IBABin\\NBX.ini"));
	WinExec(".\\IBABin\\NBX.exe", SW_HIDE);
	WinExec(".\\IBABin\\IBAHotKey.exe", SW_HIDE);
}

BOOL CIBAApp::InitIBAEnv()
{
	UNIT_AUTO_TRACE("CIBAApp::InitIBAEnv()--��ʼ��IBA����");

	CWinApp::InitInstance();

	if (!m_IBAInstance.CreateInstance())
	{	
		m_IBAInstance.ActiveMainWnd();
		return FALSE;
	}

	m_strWorkPath = CIBAConfig::GetAppPath();
	m_strWorkPath = m_strWorkPath.Left(m_strWorkPath.ReverseFind('\\'));
	VERIFY(SetCurrentDirectory(GetWorkPath()));

	CIBAHelpper::AddEnvironmentVariable(GetWorkPath() + _T("\\IBAScan"));
	CIBAHelpper::AddEnvironmentVariable(GetWorkPath() + _T("\\IBARealName"));

	//CIBALog::GetInstance(); //ʵ������־

	InitCommonControls();

	AfxEnableControlContainer();

	if (!(AfxSocketInit() && AfxOleInit()))
	{
		CIBALog::GetInstance()->Write(_T("IBA Start Error!"));

		return FALSE;
	}

#ifndef __USING_INTER_RESOURCE
	// 2011/08/17-8201-gxx: �����ַ�����Դ
	CString strPathDll = m_strWorkPath + _T("\\IBABin\\StrProvider.dll");
	CString strPathGrc = m_strWorkPath + _T("\\IBAConfig\\IBA.grc");
	if (!CStrProviderLoader::GetInstance()->Loading(strPathDll, strPathGrc))
	{
		MessageBox(NULL,_T("Loading IBA.grc error!"),NULL,MB_OK|MB_ICONERROR);
		return FALSE;
	}
#endif
	
	return TRUE;
}

void CIBAApp::OnLoginOK()
{
	CString strTmp;
	m_pMainWnd->GetWindowText(strTmp);
	strTmp.Insert(0, _T(" "));
	free((void*)m_pszAppName);
	m_pszAppName = _tcsdup(strTmp);

	m_IBAInstance.SetMainWnd(m_pMainWnd->GetSafeHwnd());
	m_IBAInstance.UpdateInstanceData();
	
	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		//ȡ������̨�汾��
		theApp.GetCurCashier()->DoAssetStatus();
	}

	// 2014-4-28 - qsc ������ʵ�� QSCT1
	//if(CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameRZX)
	if(1 == CNetBarConfig::GetInstance()->GetDomainId())
	{
		if(!PathFileExists(theApp.GetWorkPath() + _T("\\IBABin\\AuditInterface.dll")))
		{
			IBA_LOG0(_T("AuditInterface.dll�����ڣ��ᵼ���޷��ϴ�ע����Ϣ�������У�"));
			IBAMsgBox2(_T("AuditInterface.dll�����ڣ��ᵼ���޷��ϴ�ע����Ϣ�������У�"));
		}
	}

	if(0 != CNetBarConfig::GetInstance()->GetAuditInterface())
	{
		InitSunlikeAuditInterface();	
	}
	
	((CMainFrame*)AfxGetMainWnd())->StartIBA();

}

void CIBAApp::UpdateToolbarLastID(const CString& str,BOOL bError)
{
	CWnd* pWnd = GetMainWnd();
	pWnd->SendMessage(WM_TOOLBAR_LABEL_TEXT,(WPARAM)(LPCTSTR)str,(LPARAM)bError);
}

CActiveMemberView* CIBAApp::GetActiveMemberView()
{
	CIBAView *pIBAView = theApp.GetIBAView();
	ASSERT(pIBAView);

	CActiveMemberView *pMemberView = (CActiveMemberView*)pIBAView->GetTabControl()
		.GetTabWnd(1);

	ASSERT(pMemberView);
	ASSERT( pMemberView->IsKindOf( RUNTIME_CLASS(CActiveMemberView) ));

	return pMemberView;
}

void CIBAApp::AutoRestart()
{
	CFile file;

	CString strWorkPath = theApp.GetWorkPath();
	CString strPath = strWorkPath;
	strPath += _T("\\IBATemp\\autoiba.bat");
	char chIBA[1024] = {0};
	GetModuleFileNameA(m_hInstance, chIBA, 1024);

	if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite))
	{
		std::string str;
		str += "@echo off\n ping -n 1 127.0.0.1\n start ";
		str += chIBA;
		str += "\n";

		file.Write(str.c_str(), str.size());
		
		file.Close();

		CIBALog::GetInstance()->Write(_T("Auto retart IBA.exe"));

		ShellExecute(NULL, _T("open"), _T("cmd.exe"), _T("/C ") + strPath, NULL/*strWorkPath+_T("\\IBABin\\")*/, SW_HIDE);
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

CString CIBAApp::GetLastNetID() const
{
	CString strNetID;

	if ( CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		strNetID = CCurrentNetBarUser::GetInstance()->GetNetId();
	}
	else
	{
		strNetID = ((CMainFrame*)theApp.GetMainWnd())->m_strLastIdInToolbar;
	}

	return strNetID;
}


BOOL CIBAApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	
#ifndef __USING_INTER_RESOURCE
	
	// 2011/08/22-8201-gxx:
	// ָ���汾��������Դ������grc 
	//
	if (nCode == CN_UPDATE_COMMAND_UI)
	{
		if (nID == ID_LABEL)
		{
			return FALSE;
		}

		CCmdUI *pCmdUI = (CCmdUI*)pExtra;
		if (pCmdUI->m_pOther != NULL && m_pMainWnd != NULL && pCmdUI->m_pOther == &((CMainFrame*)m_pMainWnd)->m_wndToolBar)
		{
			// ���Թ�����������
		}
		else
		{
			pCmdUI->SetText(LOAD_STRING(nID));
		}
	}
#endif

	return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CIBAApp::UpdateIBAStatus(BOOL bTryingConnection/*=FALSE*/)
{
	if (bTryingConnection)
	{
		GetMainWnd()->SendMessage(WM_UPDATE_IBA_STATUS,3,0);
	}
	else
	{
		GetMainWnd()->SendMessage(WM_UPDATE_IBA_STATUS, CIBAGlobal::NetworkStatus,0);
	}
}

bool CIBAApp::InitSunlikeAuditInterface()
{
	CString strDllSunlikeAuditName = theApp.GetWorkPath() + _T("\\IBABin\\") + _T("SunlikeAudit.dll");
	if(PathFileExists(strDllSunlikeAuditName))
	{
		// ��ʼ��ǰǰ�ر�SunlikeAuditProxy.exe,��Ȼ�Ļ�����Ϊ��̬�������ʼ��ʱ������һ����SunlikeAuditProxy.exe
		// һ���Ļ��������ᴴ��ʧ��
		CProcess proxy;
		proxy.Attach(_T("SunlikeAuditProxy.exe"));
		proxy.Terminate(3000);		// 2014-7-28 - qsc ���ӵȴ������˳�ʱ��

		// 2014-7-24 - qsc
		// ɾ��ϣ֮��ʵ���ӿ�������
		LPITEMIDLIST pidlDesktop = NULL;
		HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &pidlDesktop);
		if (SUCCEEDED(hr))
		{
			TCHAR szPath[256] = {0};
			hr = SHGetPathFromIDList(pidlDesktop, szPath);
			if (SUCCEEDED(hr))
			{
				CString strLnkPath;
				strLnkPath.Format(_T("%s%s"), szPath, _T("\\ϣ֮��ʵ���ӿ�.lnk"));
				if(PathFileExists(strLnkPath))
				{
					DeleteFile(strLnkPath);
				}
			}
		}	
		
		m_hModuleSAI = ::LoadLibrary(strDllSunlikeAuditName);
		if(m_hModuleSAI)
		{	
			typedef int (__cdecl* FNInit)();
			FNInit fnInit = (FNInit)GetProcAddress(m_hModuleSAI, "Init");
			if(fnInit)
			{
				if(0 == fnInit())
				{
					m_bInitAuditInterface = true;
					return true;
				}
				else
				{
					IBA_LOG(_T("fnInit ����ʧ��"), strDllSunlikeAuditName);
				}
			}
			else
			{
				IBA_LOG(_T("��ȡ %s ��ʼ������ʧ��"), strDllSunlikeAuditName);
			}
		}
		else
		{
			IBA_LOG(_T("��ȡ %s ʧ��"), strDllSunlikeAuditName);
		}
	}
	else
	{
		IBA_LOG(_T("�ļ� %s ������"), strDllSunlikeAuditName);
	}
	IBAMsgBox(_T("����SunlikeAudit.dllʧ�ܣ����ȷ���˳�����"));
	//TerminateProcess(GetCurrentProcess(), 0);

	return false;
}

bool CIBAApp::UnInitSunlikeAuditInterface()
{
	bool bFlag = false;
	if(m_bInitAuditInterface)
	{
		if(m_hModuleSAI)
		{
			typedef int (__cdecl* FNUnInitQuick)();
			FNUnInitQuick fnUnInitQuick = (FNUnInitQuick)GetProcAddress(m_hModuleSAI, "UnInitQuick");
			if(fnUnInitQuick)
			{
				if(0 == fnUnInitQuick())
				{
					bFlag = true;
				}
			}
			FreeLibrary(m_hModuleSAI);
		}
	}
	return bFlag;
}

DWORD CIBAApp::ThreadTerminateProcess( LPVOID pParam )
{
	IBA_LOG0(_T("����ǿ���˳�1"));
	Sleep(m_nTerminateSeconds * 1000);
	IBA_LOG0(_T("����ǿ���˳�2"));
	TerminateProcess(GetCurrentProcess(), 0);
	return 0;
}

void CIBAApp::SetTerminateProcess( UINT nSeconds )
{
	m_nTerminateSeconds = nSeconds;
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTerminateProcess, NULL, 0, 0);
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
CIBAGlobal::emNetworkStatus CIBAGlobal::NetworkStatus = CIBAGlobal::emNetworkNormal;

CString CIBAGlobal::GetOperationName(int nOptType)
{
	//(NULL)|��ֵ|�˿�|����|����|�Զ��˿�|��Ա����|��ʱ|�����|��ֵ����|��ֵ����|���ؿ���|�����û��˿�|���س�ֵ|�����˿�|��������
	CIBAString strNames;
	CStringArray strArr;
	strNames.LoadString(IDS_OPT_TYPE_NAMES);
	CIBAHelpper::SplitLine(strNames,strArr);
	IBA_ASSERT2(strArr.GetCount()==emOpTypeCount,"���������ַ�����ʽ�иĶ�");

	if (nOptType<0 || nOptType>=emOpTypeCount)
	{
		IBA_ASSERT2(0,"��Ч�Ĳ�������");
		return _T("");
	}

	return strArr.GetAt(nOptType);

}


