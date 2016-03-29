// WxMiddleware.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "WxMiddleware.h"

#include "ConnNotice.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CWxMiddlewareApp

BEGIN_MESSAGE_MAP(CWxMiddlewareApp, CWinApp)
END_MESSAGE_MAP()


// CWxMiddlewareApp ����

CWxMiddlewareApp::CWxMiddlewareApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_nTimerTemp = 0;
}


// Ψһ��һ�� CWxMiddlewareApp ����

CWxMiddlewareApp theApp;
CMainWnd theMainWnd;


// CWxMiddlewareApp ��ʼ��

BOOL CWxMiddlewareApp::InitInstance()
{
	CWinApp::InitInstance();

	//::MessageBox(NULL,_T("load dll!"), NULL, MB_OK);

	LPCTSTR lpszClassName=_T("IBATempTempWindow");
	WNDCLASS wcs = {0};
	wcs.lpszClassName = lpszClassName;
	wcs.cbClsExtra = 0;
	wcs.cbWndExtra = 0;
	wcs.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcs.hCursor = ::LoadCursor(NULL,IDC_ARROW);
	wcs.hIcon = NULL;
	wcs.hInstance = AfxGetInstanceHandle();
	wcs.lpfnWndProc = AfxWndProc;
	wcs.lpszMenuName = NULL;
	wcs.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	if (!AfxRegisterClass(&wcs))
		AfxThrowResourceException();

	theMainWnd.CreateEx(WS_EX_TOOLWINDOW,lpszClassName,NULL,0,CRect(0,0,0,0), NULL, 0 ,NULL);
	this->m_pMainWnd = &theMainWnd;

	m_nTimerTemp = SetTimer(NULL, 0, 10, TimerProc);

	return TRUE;
}

VOID CALLBACK CWxMiddlewareApp::TimerProc(
							   HWND hwnd,         // handle to window
							   UINT uMsg,         // WM_TIMER message
							   UINT_PTR idEvent,  // timer identifier
							   DWORD dwTime       // current system time
							   )
{
	if (idEvent == theApp.m_nTimerTemp)
	{
		if (CConnNotice::GetInstance()->GetWxMiddlewareDelegate() != NULL) 
		{
			KillTimer(NULL, idEvent);

			UINT d,n;
			CConnNotice::GetInstance()->GetWxMiddlewareDelegate()->getNetbarIdAndDomainId(&n, &d);

			CConnNotice::GetInstance()->ConnectNotice();
		}
	}
}

void WINAPI SetDelegate(CWxMiddlewareDelete* pDelegate)
{
	CConnNotice::GetInstance()->SetWxMiddlewareDelegate(pDelegate);
}

void WINAPI LoginStatus(LPCTSTR lpszTermId, int nStatus)
{
	CConnNotice::GetInstance()->SendClientLoginRet(lpszTermId, nStatus);
}

void WINAPI LogoutStatus(LPCTSTR lpszTermId, int nStatus)
{
	CConnNotice::GetInstance()->SendClientLogoutRet(lpszTermId, nStatus);
}

void WINAPI MessageStatus(int nStatus)
{
	CConnNotice::GetInstance()->SendClientMessageRet(nStatus);
}