// RNS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
#include "commandlineinfo.h"

// CRNSApp:
// �йش����ʵ�֣������ RNS.cpp
//

class CRNSApp : public CWinApp
{
public:
	CRNSApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	CCommandLine m_CommandLine;
};

extern CRNSApp theApp;
