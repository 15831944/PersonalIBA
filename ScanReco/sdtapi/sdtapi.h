// sdtapi.h : sdtapi DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CsdtapiApp
// �йش���ʵ�ֵ���Ϣ������� sdtapi.cpp
//

class CsdtapiApp : public CWinApp
{
public:
	CsdtapiApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
