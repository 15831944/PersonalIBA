// PcpThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DdnPayCard.h"
#include "PcpThread.h"
#include "PayCardDlg.h"
#include "MainWnd.h"
#include "hooks.h"

CPcpThread* CPcpThread::m_lpThis = NULL;

HWND CPcpThread::m_hMainWnd = NULL;

// CPcpThread

IMPLEMENT_DYNCREATE(CPcpThread, CWinThread)

CPcpThread::CPcpThread()
{
	m_lpMainWnd = NULL;
}

CPcpThread::~CPcpThread()
{
}


BEGIN_MESSAGE_MAP(CPcpThread, CWinThread)
END_MESSAGE_MAP()

BOOL CPcpThread::InitInstance()
{
	OleInitialize(NULL); //����Ҫʹ��OleInitialize����Ȼ��ҳ�����治�ܸ���

	m_lpMainWnd = new CMainWnd;

	m_pMainWnd = m_lpMainWnd;

	LPCSTR szClassName = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));

	if (!m_pMainWnd->Create(szClassName, _T("DdnPcp"), WS_OVERLAPPEDWINDOW,
		CRect(0, 0, 10, 10), CWnd::GetDesktopWindow(), NULL))
	{
		delete m_pMainWnd;
		m_pMainWnd = NULL;

		return FALSE;
	}

	((CMainWnd*)m_pMainWnd)->m_strParams = m_strParams;

	m_hMainWnd = m_pMainWnd->GetSafeHwnd();

	m_pMainWnd->ShowWindow(SW_HIDE);

	CHooks::gModuleHandle = AfxGetInstanceHandle();
	CHooks::Hook();

	return TRUE;
}


int CPcpThread::ExitInstance()
{
	if (m_pMainWnd != NULL)
	{
		m_lpMainWnd->DestroyWindow();
		delete m_lpMainWnd;
		m_lpMainWnd = NULL;
		m_pMainWnd = NULL;
	}

	m_lpThis = NULL;
	
	OleUninitialize();

	EmptyWorkingSet(GetCurrentProcess());

	CHooks::UnHook();

	CWinThread::ExitInstance();

	AfxEndThread(0);

	return 0;
}

CString g_strLastlyParam;
BOOL CPcpThread::CreatePcp(LPCTSTR lpszParams)
{
	g_strLastlyParam = lpszParams;

	GXX_LOG_HIT(_T("call fun: CreatePcp"));
	if (m_lpThis != NULL)
	{
		return TRUE;
	}
	else
	{
		m_lpThis = (CPcpThread*)AfxBeginThread(
			RUNTIME_CLASS(CPcpThread), 
			THREAD_PRIORITY_NORMAL, 
			0, 
			CREATE_SUSPENDED, 
			NULL);

		m_lpThis->m_strParams = lpszParams;

		m_lpThis->ResumeThread();
	}

	return m_lpThis != NULL;
}


BOOL CPcpThread::DestoryPcp()
{
	BOOL bRes = FALSE;
	if (m_lpThis != NULL)
	{
		bRes = m_lpThis->PostThreadMessage(WM_QUIT, NULL, NULL);
	}
	GXX_LOG_HIT(_T("Destory Pcp return %d"), bRes);

	//GXX_LOG_HIT(_T("the app thread handle=%d"), theApp.m_hThread);
	//bRes = 
	//GXX_LOG_HIT(_T("Destory Pcp quit app return %d"), bRes);

	if (!g_bHasExit)
	{
		theApp.ExitInstance();
	}
	return bRes;
}

BOOL CPcpThread::ShowPcp()
{
	if (m_lpThis != NULL)
	{

		if (m_lpThis->m_lpMainWnd)
		{
			m_lpThis->m_lpMainWnd->m_strParams = g_strLastlyParam;

			GXX_LOG_HIT(_T("���ݾ�����̨�����ò���"));
		}
		return PostMessage(m_hMainWnd, WM_HOSTMSG, 0, 0);
	}

	return FALSE;
}

BOOL CPcpThread::HidePcp()
{
	if (m_lpThis != NULL)
	{
		return PostMessage(m_hMainWnd, WM_HOSTMSG, 1, 0);
	}

	return FALSE;
}