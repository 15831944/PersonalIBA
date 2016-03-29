#include "stdafx.h"
#include ".\bcgpstatictips.h"

// CBCGPStaticTips

IMPLEMENT_DYNAMIC(CBCGPStaticTips, CBCGPStatic)
CBCGPStaticTips::CBCGPStaticTips()
:m_hwndTips(NULL)
{
}

CBCGPStaticTips::~CBCGPStaticTips()
{
}


BEGIN_MESSAGE_MAP(CBCGPStaticTips, CBCGPStatic)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CBCGPStaticTips::SetWindowText( LPCTSTR lpszString )
{
	CBCGPStatic::SetWindowText(lpszString);

	if(!m_hwndTips || !IsWindow(m_hwndTips))
	{
		CreateToolTip();
	}
	else
	{
		SetToolTip(lpszString);
	}
}



// CBCGPStaticTips ��Ϣ�������


int CBCGPStaticTips::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CreateToolTip();

	return 0;
}

HWND CBCGPStaticTips::CreateToolTip() 
{ 
	if(!m_hwndTips && !IsWindow(m_hwndTips))
	{
		m_hwndTips = CreateWindowEx(WS_EX_TOPMOST, 
			TOOLTIPS_CLASS, 
			NULL, 
			WS_POPUP | TTS_ALWAYSTIP, //| TTS_BALLOON, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			GetParent()->GetSafeHwnd(), 
			NULL, 
			AfxGetInstanceHandle(), 
			NULL); 
	}


	if (!m_hwndTips) 
	{ 
		return (HWND)NULL; 
	} 
	CString strWndText;
	GetWindowText(strWndText);

	HWND hWnd =GetSafeHwnd();
	TOOLINFO toolInfo = {0}; 
	toolInfo.cbSize = sizeof(TOOLINFO); 
	toolInfo.hwnd = GetParent()->GetSafeHwnd(); 
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	toolInfo.uId = (UINT_PTR)GetSafeHwnd(); 
	toolInfo.lpszText = (LPTSTR)strWndText.GetString(); 
	::SendMessage(m_hwndTips, TTM_ADDTOOL, 0, (LPARAM)&toolInfo); 
	return m_hwndTips; 
} 

LRESULT CBCGPStaticTips::SetToolTip(LPCTSTR pszText)  
{
	CString strText(pszText);
	TOOLINFO toolInfo = {0}; 
	toolInfo.cbSize = sizeof(TOOLINFO); 
	toolInfo.hwnd = GetParent()->GetSafeHwnd(); 
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	toolInfo.uId = (UINT_PTR)GetSafeHwnd(); 
	toolInfo.lpszText = (LPTSTR)strText.GetString(); 
	return ::SendMessage(m_hwndTips, TTM_UPDATETIPTEXT, 0, (LPARAM)&toolInfo); 
}

LRESULT CBCGPStaticTips::DeleteToolTip()  
{
	TOOLINFO toolInfo = {0}; 
	toolInfo.cbSize = sizeof(TOOLINFO); 
	toolInfo.hwnd = GetParent()->GetSafeHwnd(); 
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	toolInfo.uId = (UINT_PTR)GetSafeHwnd(); 
	return ::SendMessage(m_hwndTips, TTM_DELTOOL, 0, (LPARAM)&toolInfo); 
}
void CBCGPStaticTips::OnDestroy()
{
	CBCGPStatic::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	DeleteToolTip();
}

void CBCGPStaticTips::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	// ��MFC�ĶԻ�����д������໯��ʱ�򲢲�����ӦWM_CREATE��Ϣ
	// ��Ϊ���໰ʵ��DoDataExchange���е�,���������������OnInitDialog���
	// ���ʱ��ؼ��Ѿ��������	
	CBCGPStatic::PreSubclassWindow();
	DWORD dwSytle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	dwSytle |= SS_NOTIFY;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwSytle);

	CreateToolTip();
}


void CBCGPStaticTips::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strWndText;
	GetWindowText(strWndText);
	SetToolTip(strWndText);

	CBCGPStatic::OnMouseMove(nFlags, point);
}
