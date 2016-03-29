// WebBrowser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include ".\WebBrowser.h"


// CWebBrowser

IMPLEMENT_DYNCREATE(CWebBrowser, CHtmlView)

CWebBrowser::CWebBrowser()
: m_bContextMenuEnabled(TRUE)
{
}

CWebBrowser::~CWebBrowser()
{
}

BEGIN_MESSAGE_MAP(CWebBrowser, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CWebBrowser ��Ϣ�������

void CWebBrowser::OnTitleChange(LPCTSTR lpszText)
{
	// TODO: �ڴ����ר�ô����/����û���

	CHtmlView::OnTitleChange(lpszText);
}

void CWebBrowser::OnDocumentComplete(LPCTSTR lpszURL)
{
	// TODO: �ڴ����ר�ô����/����û���

	CHtmlView::OnDocumentComplete(lpszURL);
}


void CWebBrowser::OnDestroy()
{
	m_pBrowserApp.Release();

	m_wndBrowser.DestroyWindow();

	CWnd::OnDestroy();
}

int CWebBrowser::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CWebBrowser::PostNcDestroy()
{
	//��ֹ��ɾ��
	//CHtmlView::PostNcDestroy();
}

BOOL CWebBrowser::PreTranslateMessage(MSG* pMsg)
{
	if (WM_RBUTTONUP == pMsg->message || WM_CONTEXTMENU == pMsg->message)
	{
		if (!IsContextMenuEnabled()) //�����Ҽ��˵��ͼ��̲˵���
		{
			return TRUE;
		}
	}

	return CHtmlView::PreTranslateMessage(pMsg);
}
