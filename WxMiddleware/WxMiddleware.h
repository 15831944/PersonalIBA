// WxMiddleware.h : WxMiddleware DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CWxMiddlewareApp
// �йش���ʵ�ֵ���Ϣ������� WxMiddleware.cpp
//

class CWxMiddlewareApp : public CWinApp
{
public:
	CWxMiddlewareApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	static VOID CALLBACK TimerProc( HWND hwnd, /* handle to window */ UINT uMsg, /* WM_TIMER message */ UINT_PTR idEvent, /* timer identifier */ DWORD dwTime /* current system time */ );
protected:
	UINT m_nTimerTemp;
};
