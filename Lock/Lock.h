// Lock.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
#define WM_CHECKPASSWORD		(WM_APP + 354)

// CLockApp:
// �йش����ʵ�֣������ Lock.cpp
//

class CLockApp : public CWinApp
{
public:
	CLockApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CLockApp theApp;
