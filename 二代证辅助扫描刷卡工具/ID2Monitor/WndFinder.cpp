#include "StdAfx.h"
#include ".\wndfinder.h"

CWndFinder::CWndFinder(void)
{
	m_hBaseWnd = NULL;
}

CWndFinder::~CWndFinder(void)
{
}

BOOL CWndFinder::Init(LPCTSTR lpszClassName, LPCTSTR lpszWindowName)
{
	m_hBaseWnd = ::FindWindow(lpszClassName, lpszWindowName);

	if (m_hBaseWnd == NULL)
	{
		GXX_LOG_ERROR("Can't find window: ClassName=\"%s\" WindowName=\"%s\"",
			lpszClassName, lpszWindowName);
		return FALSE;
	}
	else
	{
		GXX_LOG_HIT("Found window: ClassName=\"%s\" WindowName=\"%s\"",
			lpszClassName, lpszWindowName);
	}
	
	return TRUE;
}

BOOL CWndFinder::Init(LPCTSTR lpDialogCaption)
{
	m_hBaseWnd = FindDialog(lpDialogCaption);
	return m_hBaseWnd != NULL;
}
HWND CWndFinder::FindDialog(LPCTSTR lpszCaption)
{
	HWND hDlg = ::FindWindow(_T("#32770"),lpszCaption);
	if (!hDlg)
	{
		GXX_LOG_WARNING("δ�ҵ��Ի���\"%s\"", lpszCaption);
	}
	return hDlg;
}
BOOL CWndFinder::GetChildCtrlText(UINT nID, CString& strOut)
{
	if (!m_hBaseWnd)
	{
		GXX_LOG_WARNING("CWndFinder δ��ʼ��");
		return FALSE;
	}
	HWND hWnd = ::GetDlgItem(m_hBaseWnd, (int)nID);

	if (!hWnd)
	{
		GXX_LOG_WARNING("CWndFinder û���ҵ�ID:%d ��Ӧ�Ŀؼ�", nID);
		return FALSE;
	}

	::SendMessage(hWnd,   WM_GETTEXT,   WPARAM(100),   LPARAM(strOut.GetBuffer(100)));
	strOut.ReleaseBuffer();

	return TRUE;
}

static int g_nIndex = 0;
static HWND g_hWnd2 = NULL;
static int g_nDes = 0;
BOOL   CALLBACK   EnumChildProc2(HWND hwnd,LPARAM lParam)       //�ص�����
{
	if(hwnd) //����Ӵ��ڴ���
	{
		if (g_nDes == g_nIndex)
		{
			g_hWnd2 = hwnd;
			return FALSE;
		}
		g_nIndex++     ;//��¼�Ӵ��ڵ���Ŀ 
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CWndFinder::GetChildCtrlTextEx(UINT nCtrlOrder, CString& strOut)
{
	if (!m_hBaseWnd)
	{
		GXX_LOG_WARNING("CWndFinder δ��ʼ��");
		return FALSE;
	}

	g_hWnd2 = NULL;
	g_nIndex = 0;
	g_nDes = nCtrlOrder;
	::EnumChildWindows(m_hBaseWnd, (WNDENUMPROC)EnumChildProc2, nCtrlOrder);

	HWND hWnd = g_hWnd2;
	if (hWnd)
	{
		::SendMessage(hWnd,   WM_GETTEXT,   WPARAM(100),   LPARAM(strOut.GetBuffer(100)));
		strOut.ReleaseBuffer();
		return TRUE;
	}
	else
	{
		GXX_LOG_WARNING("û���ҵ�ID=%d�� ��Ӧ�Ŀؼ�", nCtrlOrder);
	}
	return FALSE;
}
