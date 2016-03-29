// YB2IBA.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "YB2IBA.h"
#include "YB2IBADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYB2IBAApp ����

CYB2IBAApp::CYB2IBAApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CYB2IBAApp ����

CYB2IBAApp theApp;

BOOL CYB2IBAApp::CheckSingleInstance()
{
	const TCHAR singleMutex[] = _T("IBAREALNAME_YB2IBA");

	m_hMutex = ::CreateMutex(NULL, FALSE, singleMutex);
	
	if (m_hMutex != NULL)
	{
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

// CYB2IBAApp ��ʼ��

BOOL CYB2IBAApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!CheckSingleInstance())
	{
		return FALSE;
	}

	CString strCmdLine(m_lpCmdLine);
	
	if (0 != strCmdLine.CompareNoCase(_T("YBWW")))
	{
		return FALSE;
	}
	 
	AfxSocketInit();

	CYB2IBADlg* pDlg = new CYB2IBADlg;
	pDlg->Create(CYB2IBADlg::IDD);
	pDlg->ShowWindow(SW_HIDE);

	m_pMainWnd = pDlg;

	return TRUE;
}
