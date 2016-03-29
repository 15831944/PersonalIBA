#pragma once

G2XUI_BEGIN

class G2X_CLASSES CG2XPanel : public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XPanel)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XPanel(void);
	virtual ~CG2XPanel(void);

	struct PlaceInfo
	{
		G2XPanelPlace placeStyle;
		INT nRightDistance;
		INT nBottomDistance;
		INT nCX; // �ؼ�����ʼ���
		INT nCY; // �ؼ�����ʼ�߶�
	};

public:
	void AddPlaceCtrl(HG2XWND hG2XWnd, G2XPanelPlace placeStyle);
	void AddPlaceCtrl(CG2XWnd* pG2XWnd, G2XPanelPlace placeStyle);
	void AddPlaceCtrl(UINT nID, G2XPanelPlace placeStyle);
	void AddPlaceCtrlEx(CG2XWnd* pG2XWnd, G2XPanelPlace placeStyle, const RECT& defaultPosition);

	// if pWnd ��new������, bDeleteMem��������ָ��ΪTRUE, OnDestroyʱ��G2XPanel���ͷŸ��ӵĴ���
	void AttachWindow(CWnd* pWnd, BOOL bDeleteMem=TRUE);
	CWnd* GetAttachWindow();

protected:
	g_msg void OnSize();
	g_msg void OnShowWnd();
	g_msg void OnDestroy();
	virtual void DrawSelf(Gdiplus::Graphics *pGraphics);

	std::map<HGDIWND,PlaceInfo> m_mapPlaceInfo;

	CWnd* m_pAttachWnd;
	BOOL m_bDeleteAttachWndMem;
};

G2X_END