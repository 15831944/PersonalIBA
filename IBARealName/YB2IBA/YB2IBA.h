// YB2IBA.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CYB2IBAApp:
// �йش����ʵ�֣������ YB2IBA.cpp
//

class CYB2IBAApp : public CWinApp
{
public:
	CYB2IBAApp();

// ��д
public:
	virtual BOOL InitInstance();

	HANDLE m_hMutex;

// ʵ��
	BOOL CheckSingleInstance();
};

extern CYB2IBAApp theApp;
