#pragma once

G2XUI_BEGIN
G2X_FRAME

// ϵͳ��ť��Ϣ�ṹ
struct G2XSysBtnInfo
{
	INT nBtnWidth;							// ��ť���
	UINT nID;								// ��ťID
	BOOL bEnabled;							// ��ť������״̬
	CString strTip;							// ��ʾ�ı�
	INT iImageArray[G2XControlStateCount];	// �ؼ�����״̬�µ�ͼ������
	CColor clrArray1[G2XControlStateCount];	// �ؼ�����״̬�µİ�ť�ϱ�Ե����ɫ
	CColor clrArray2[G2XControlStateCount];	// �ؼ�����״̬�µİ�ť�±�Ե����ɫ
};

#define ZEOR_G2XSYSBTNINFO(info) \
	info.nBtnWidth=0;\
	info.nID = 0;\
	info.bEnabled = TRUE;\
	info.strTip = _T("");\
	for (int i=0; i < 4; i++) info.iImageArray[i] = -1;\
	info.clrArray1[G2XControlStateNormal]  = CColor(RGB(80,158,218));\
	info.clrArray1[G2XControlStateHover]   = CColor(RGB(158,202,234));\
	info.clrArray1[G2XControlStateSinkage] = CColor(RGB(158,202,234));\
	info.clrArray1[G2XControlStateDisable] = CColor(RGB(210,210,210));\
	info.clrArray2[G2XControlStateNormal]  = CColor(RGB(80,158,218),0);\
	info.clrArray2[G2XControlStateHover]   = CColor(RGB(158,202,234),0);\
	info.clrArray2[G2XControlStateSinkage] = CColor(RGB(158,202,234),0);\
	info.clrArray2[G2XControlStateDisable] = CColor(RGB(210,210,210),0);

class G2X_CLASSES CG2XSysBtnBar : public CG2XControl
{
	DECLARE_DYNCREATE(CG2XSysBtnBar)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XSysBtnBar(void);
	virtual ~CG2XSysBtnBar(void);

	BOOL Create(CG2XWnd* pParent, LPCRECT lpRect, G2XSysBtnInfo* pBtnInfos, INT nCount);

	void SetStyle(G2XSysBtnBarStyle style/*=G2XSysBtnBarStyleQQ2012*/, BOOL bRedraw=TRUE);

protected:
	virtual void DrawControl(Graphics *pGraphics,G2XControlState ctrlState);

	virtual void DrawSysBtn(Graphics *pGraphics, LPCRECT lpRect, G2XControlState btnState, UINT nID);

	virtual void DrawCustomBK(Graphics *pGraphics, LPCRECT lpRect, G2XControlState btnState);
	virtual void DrawCustomButton(Graphics *pGraphics, LPCRECT lpRect, const G2XSysBtnInfo& btnInfo, G2XControlState btnState);

	g_msg void OnCreate();
	g_msg void OnLButtonUp(UINT nFlags, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();

public:
	// ϵͳ��ť����ĳ��ID�Ƿ����
	BOOL ExistID(UINT nID);

	// ������ť
	BOOL EraseID(UINT nID);

	// ��ťʹ�ܻ���
	BOOL EnableID(UINT nID, BOOL bEnable);

	void InsertBefore(UINT nID, const G2XSysBtnInfo& btnInfo);
	void InsertAfter(UINT nNewID, const G2XSysBtnInfo& btnInfo);

	G2XSysBtnInfo* GetButtonInfo(UINT nID);

	BOOL SetImageList(const CG2XImageList& imgList);

public:
	std::vector<G2XSysBtnInfo> m_infoBtns;
	CG2XImageList m_imgList;

	BOOL m_bMaxiszed;
	UINT m_nHoverID;
	CColor m_clrSplitLine;  // �ָ��ߵ���ɫ

	G2XSysBtnBarStyle m_style; // ��ť�ķ��
};

G2X_END
G2X_END
