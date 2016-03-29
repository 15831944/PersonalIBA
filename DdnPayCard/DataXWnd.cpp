// DataXWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include ".\DataXWnd.h"

// CDataXWnd
using namespace NS_DataX;

IMPLEMENT_DYNAMIC(CDataXWnd, CWnd)
CDataXWnd::CDataXWnd()
{
	m_lpReceiver = NULL;
}

CDataXWnd::~CDataXWnd()
{
}


BEGIN_MESSAGE_MAP(CDataXWnd, CWnd)
	ON_MESSAGE(CDataXWnd::WM_DATAX, OnDataX)
END_MESSAGE_MAP()

LRESULT CDataXWnd::OnDataX(WPARAM wParam, LPARAM lParam)
{
	INT nRet = -1 ;

	if (m_lpReceiver != NULL)
	{
		nRet = m_lpReceiver->OnDataArrival();
	}

	return nRet;
}

// CDataXWnd ��Ϣ�������


BOOL CDataXWnd::CreateRevWnd(IDataXReceive* lpReceiver)
{
	LPCTSTR szClassName = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));

	if (!Create(szClassName, _T("_DataX_Wnd_"), WS_CHILD, 
		CRect(0, 0, 0, 0), GetDesktopWindow(), NULL))
	{
		return FALSE;
	}

	m_lpReceiver = lpReceiver;

	return TRUE;
}

BOOL CDataXWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bRet = CWnd::PreCreateWindow(cs) ;

	WNDCLASS wc = {0};   

	::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wc);   

	//һ���������������Ҫ�޸ģ������������������������ұ����ڽ���ͨѶ
	wc.lpszClassName = _T("_DataX_Wnd_");  

	AfxRegisterClass(&wc);   

	cs.lpszClass = wc.lpszClassName;  

	return bRet;
}
