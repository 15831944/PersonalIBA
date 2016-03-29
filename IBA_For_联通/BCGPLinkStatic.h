/**************************************************************************
file:  BCGPLinkStatic.h
class: CBCGPLinkStatic :public CBCGPStatic
for:   ������ʾ��������ʽ��static,�����������ɫ,static�ı�����ɫ��������ɫ������
	BCGP���������ɫ.
date:  2011-03-17
author:guoxuxing
**************************************************************************/

#pragma once


// CBCGPLinkStatic

class CBCGPLinkStatic : public CBCGPStatic
{
	DECLARE_DYNAMIC(CBCGPLinkStatic)

public:
	CBCGPLinkStatic();
	virtual ~CBCGPLinkStatic();

	enum emMouseState
	{
		emLinkNormal = 0,     // mouse beyond the link
		emLinkHover = 1,   // mouse above the link
		emLinkVisited = 2       // mouse click the link
	};

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetLinkAddress(CString strLink,BOOL bAutoOpen=FALSE);

public:
	void SetTransparent(BOOL bTransparent);  // ���ñ����Ƿ�͸��
	void SetVerticalCenter(BOOL bVCenter);   // �����ı��Ƿ�ֱ����
	void SetBkColor(COLORREF clr);
	void SetLinkColor(COLORREF clr);         // �������ӵ���ɫ
	void SetHoverColor(COLORREF clr);        // �������Խ���ǵ���ɫ
	void SetVisitedColor(COLORREF clr);      // �������ӱ����ʺ����ɫ

	void  SetFont(CFont *pFont);
	CFont* GetFont();
	void SetHoverFont(CFont *pFont);
	CFont* GetHoverFont();

protected:

	virtual void DrawLinkName(CDC* pDC,RECT rect,CString strName,UINT nMouseState);
	virtual void DrawLinkBk(CDC* pDC,RECT rect,BOOL bTransparent,UINT nMouseState);

public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnBCGSetControlAero (WPARAM, LPARAM);

protected:
	CFont *m_pFont;
	CFont *m_pHoverFont;
	CString m_strLinkAddress;
	BOOL m_bAutoOpen;
	COLORREF m_clrBk;
	COLORREF m_clrLink;
	COLORREF m_clrHover;
	COLORREF m_clrVisited;
	BOOL m_bVCenter;
	BOOL m_bTransparent;
	BOOL m_bHaveVisited;

	UINT m_nMouseState;
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


