#pragma once
#include "WebBrowser.h"
#include "HoverButton.h"
#include "ImageEx.h"
#include "Common.h"

// CTipsWnd

class CTipsWnd : public CWnd, public CWebBrowserEventHandler
{
	DECLARE_DYNAMIC(CTipsWnd)
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()

public:
	CTipsWnd();
	virtual ~CTipsWnd();

protected:
	
	virtual void WebBrowser_TitleChange(CWebBrowser& Sender, LPCTSTR lpszText);
	virtual void WebBrowser_DocumentComplete(CWebBrowser& Sender, LPCTSTR lpszURL);
	virtual void WebBrowser_NewWindow2(CWebBrowser& Sender,LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual HRESULT WebBrowser_GetExternal(CWebBrowser& Sender, LPDISPATCH *lppDispatch);


public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedClosebtn();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
	virtual BOOL DestroyWindow();

private:
	Tips_ShowParam m_tips_showParam;

	BOOL m_blisComplete;
	BYTE m_Factor;

	static const UINT TimerIDOfStartDisplay = 1000; //��ʼ��ʾ
	static const UINT TimerIDOfDisplaying = 1001; //��ʾ����
	static const UINT TimerIDOfDisapper = 1002; //����

public:
	CImageEx m_backImg;
	CRect m_ClientRect;

	CWebBrowser *m_pWebBrowser;
	CHoverButton m_CloseBtn;
	CStatic      m_TitleStatic;
	
	CFont m_TextFont;

	int m_nTimeCount;

	LPDISPATCH m_lpExternalDisp;

private:
	CString CombinationWebPage(CString strTitle, CString strContent);

	int WirteWebPage(CString strTitle, CString strContent);
	void OpenUrl(CString strUrl, LPCTSTR postData, LPCTSTR headData);

	BOOL Aminhide();

	void StartDisplayWindow();
	void DisplayingWindow(UINT nTimerEvent);
	BOOL DisapperWindow(UINT nTimerEvent);
	BOOL IsMouseInWindowRect(); //����ڴ�����ΪTURE
	void HideBrowserBorder();

	BOOL CreateWebBrower();
	virtual void InitResource(); //��ʼ��Դ
	virtual void CalculateControlPostion(); //����ؼ�λ��
	virtual BOOL DrawWindow(CDC* pDC); //���ƴ���
	
	virtual HBRUSH SetStaticTitle(CDC* pDC); //���ƴ���

	LPDISPATCH GetHostApp();

	void HideTips();

	void SetAppearTime(INT nAppearTime);

	void OnLookDetail();

public:
	void SetTipsParam(Tips_ShowParam tips_showParam);

	void ShowTips(CString strUrl);

	void ShowTips(CString strTitle, CString strContents);
};


