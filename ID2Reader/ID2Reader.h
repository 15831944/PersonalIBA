// ID2Reader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

// CID2ReaderApp:
// �йش����ʵ�֣������ ID2Reader.cpp
//

class CID2ReaderApp : public CWinApp
{
public:
	CID2ReaderApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

protected:


};

extern CID2ReaderApp theApp;
