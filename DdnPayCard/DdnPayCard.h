// DdnPayCard.h : DdnPayCard DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������

#define WM_HOSTMSG	(WM_USER + 1)

// CDdnPayCardApp
// �йش���ʵ�ֵ���Ϣ������� DdnPayCard.cpp
//

class CDdnPayCardApp : public CWinApp
{
public:
	CDdnPayCardApp();

// ��д
public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CDdnPayCardApp theApp;
extern bool g_bHasExit;