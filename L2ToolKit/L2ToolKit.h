// L2ToolKit.h : L2ToolKit DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CL2ToolKitApp
// �йش���ʵ�ֵ���Ϣ������� L2ToolKit.cpp
//

class CL2ToolKitApp : public CWinApp
{
public:
	CL2ToolKitApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
