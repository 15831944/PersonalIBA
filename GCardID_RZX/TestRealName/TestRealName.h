// TestRealName.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTestRealNameApp:
// �йش����ʵ�֣������ TestRealName.cpp
//

class CTestRealNameApp : public CWinApp
{
public:
	CTestRealNameApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestRealNameApp theApp;
