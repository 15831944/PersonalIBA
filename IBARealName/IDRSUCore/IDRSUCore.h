// IDRSUCore.h : IDRSUCore DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CIDRSUCoreApp
// �йش���ʵ�ֵ���Ϣ������� IDRSUCore.cpp
//

class CIDRSUCoreApp : public CWinApp
{
public:
	CIDRSUCoreApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
