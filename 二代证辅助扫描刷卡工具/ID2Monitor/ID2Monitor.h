// ID2Monitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CID2MonitorApp:
// �йش����ʵ�֣������ ID2Monitor.cpp
//

class CID2MonitorApp : public CWinApp
{
public:
	CID2MonitorApp();

// ��д
	public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CID2MonitorApp theApp;
