// GCardID_RZX.h : GCardID_RZX DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CGCardID_RZXApp
// �йش���ʵ�ֵ���Ϣ������� GCardID_RZX.cpp
//

class CGCardID_RZXApp : public CWinApp
{
public:
	CGCardID_RZXApp();

// ��д
public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();

};


