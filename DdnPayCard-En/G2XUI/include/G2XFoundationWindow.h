#pragma once

G2XUI_BEGIN
G2X_FRAME
// CG2XFoundationWindow

class G2X_CLASSES CG2XFoundationWindow : public CWnd
{
	DECLARE_DYNAMIC(CG2XFoundationWindow)

public:
	CG2XFoundationWindow();
	virtual ~CG2XFoundationWindow();

	BOOL Create(const RECT& rect,  LPCTSTR lpTitle, CWnd* pParentWnd);

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetG2XBaseWnd(CRuntimeClass* classG2XBase);
	void SetG2XCaptionbar(CRuntimeClass* classG2XCatipnbar);
	void SetG2XClientArea(CRuntimeClass* classG2XClientArea);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// ��ʾ or ���� ������
	void ShowCaptionBar(BOOL bShow=TRUE);
	void SetCaptionBarHeight(INT nHeight = 25);

	void DrawWindow(Graphics* pGraphics, Bitmap* pFrontBmp);

	// ���ô�������岻͸����, 255��ȫ��͸��,0 ��ʾ��ȫ͸��
	void SetAlpha(BYTE nAlpha=255, BOOL bRepaint=TRUE);

protected:
	// �����ں��������ػ汳��,���غ����ҪʹЧ���õ���ʾ����Ҫ��m_pG2XPanel������Ϊ͸��
	virtual void EraseBackground(Graphics* pGraphics, LPCRECT lpRect){}

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT OnIMEChar( WPARAM wParam, LPARAM lParam );
	afx_msg void OnChar(UINT nChar ,UINT nRepCnt, UINT nFlags);

	virtual void OnInitialUpdate();

	virtual BOOL DoIMEChar(LPCTSTR lpcStr);

public:
	void AddPlaceCtrl(CG2XWnd* pG2XWnd, G2XPanelPlace place);
	void AdjustG2XWnd();

	CG2XWnd* GetTopG2XWnd() const { return m_pG2XWnd; }
	CG2XPanel* GetPanel() const { return m_pG2XPanel; }
	CG2XCaptionBar* GetCaptionBar() const { return m_pG2XCaption; }

	INT GetBorderWidth() { return m_nBorderWidth; }

protected:
	bool DrawLayerWindow();
	virtual void CreateG2XUIObjects();
	void AdjustChildWindow();
	BOOL TranslateG2XAccelator(MSG* pMsg);
protected:
	CG2XWnd* m_pG2XWnd;
	CString m_strCaption;
	CG2XCaptionBar* m_pG2XCaption; // ������
	CG2XPanel* m_pG2XPanel; // �ͻ������
	BOOL m_bHasBorder;
	INT m_nBorderWidth;
	BOOL m_bRoundBorder;    // TRUE: Բ�Ǳ߿�, FALSE: ���α߿�
	SIZE m_szCorner;
	BYTE m_nAlpha;  // ���ڵ����ز�͸����
	CRuntimeClass* m_pClassBase;
	CRuntimeClass* m_pClassCaption;
	CRuntimeClass* m_pClassClientArea;
	Bitmap* m_pFrontBmp;
public:
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
};

G2X_END
G2X_END

