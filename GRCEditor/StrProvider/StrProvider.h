// StrProvider.h : StrProvider DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CStrProviderApp
// �йش���ʵ�ֵ���Ϣ������� StrProvider.cpp
//

class CStrProviderApp : public CWinApp
{
public:
	CStrProviderApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
BOOL __stdcall LoadStringContainer(const char* lpGrcFilePath);
void __stdcall GetStringA(UINT nID, char* pchOut, const unsigned int nSize);
void __stdcall GetStringW(UINT nID, wchar_t* pchOut, const unsigned int nSize);