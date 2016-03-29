#pragma once
#include "G2XWnd.h"

G2XUI_BEGIN

class G2X_CLASSES CG2XControlColorData
{
public:
	CColor colorNormalBk;		// �ؼ�����״̬ʱ�ı�����ɫ
	CColor colorNormalText;		// �ؼ�����״̬ʱ��������ɫ
	CColor colorNormalBorder;	// �ؼ�����״̬ʱ�ı߿���ɫ
	
	CColor colorHoverBk;		// ��껬���ؼ�ʱ�ı�����ɫ
	CColor colorHoverText;		// ��껬���ؼ�ʱ��������ɫ
	CColor colorHoverBorder;	// ��껬���ؼ�ʱ�ı߿���ɫ

	CColor colorSinkageBk;		// ��갴��ʱ�ı�����ɫ
	CColor colorSinkageText;	// ��갴��ʱ��������ɫ
	CColor colorSinkageBorder;	// ��갴��ʱ�ı߿���ɫ

	CColor colorDisableBk;		// �ؼ�������ʱ�ı�����ɫ
	CColor colorDisableText;	// �ؼ�������ʱ��������ɫ
	CColor colorDisableBorder;	// �ؼ�������ʱ�ı߿���ɫ

	const CColor& GetBkColor(G2XControlState ctrlState) const;
	const CColor& GetTextColor(G2XControlState ctrlState) const;
	const CColor& GetBorderColor(G2XControlState ctrlState) const;
public:
	CG2XControlColorData::CG2XControlColorData()
	{
		colorNormalBk		= CColor(RGB(255,255,255));
		colorNormalText		= CColor(RGB(0,0,0));
		colorNormalBorder	= CColor(RGB(185,205,226));

		colorHoverBk		= CColor(RGB(255,255,255));
		colorHoverText		= CColor(RGB(0,0,0));
		colorHoverBorder	= CColor(RGB(58,146,199));

		colorSinkageBk		= CColor(RGB(255,255,255));
		colorSinkageText	= CColor(RGB(0,0,0));
		colorSinkageBorder	= CColor(RGB(58,146,199));

		colorDisableBk		= CColor(RGB(237,237,237));
		colorDisableText	= CColor(RGB(177,177,177));
		colorDisableBorder	= CColor(RGB(177,177,177));
	}
	CG2XControlColorData::CG2XControlColorData(const CG2XControlColorData& colorData)
	{
		CopyFrom(colorData);
	}
	void CopyFrom(const CG2XControlColorData& colorData);
};

class G2X_CLASSES CG2XControl : public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XControl)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XControl(void);
	virtual ~CG2XControl(void);

	static const VSCROLLBAR_ID = 101;
	static const HSCROLLBAR_ID = 102;
public:
	//---------------------------------------------------------------------
	//- �����ؼ�
	//- halign: �ؼ��ı�ˮƽ���뷽ʽ
	//- valign: �ؼ��ı���ֱ���뷽ʽ
	//- borderStyle: �ؼ��߿���
	//---------------------------------------------------------------------
	BOOL CreateControl(
		CG2XWnd*  pParent,
		LPCTSTR  lpText,
		LPCRECT  lpRect,
		UINT  nCtrlID,
		G2XTextHAlign  halign=G2XTextHAlignLeft,
		G2XTextVAlign  valign=G2XTextVAlignMiddle,
		G2XBorderStyle borderStyle = G2XBorderNone);

	//
	void EnableMultiStyleText(BOOL bEnable);

	//- �õ��ؼ���ǰ��״̬
	G2XControlState GetControlState();
	
	//- �ڿؼ��н��������£��Ƿ�������ƽ������
	void SetDrawFocus(BOOL bDrawFocus);
	
	//- �˿��Ƿ���Ҫ���ƽ������, �������ػ�ʱ,��Ҫ��Ҫ�˺����������Ƿ�Ҫ���������
	BOOL GetIsDrawFocus();

	//---------------------------------------------------------------------
	//- ����ͼ���б�
	//- imgList: �����ͼ���б�
	//- bWndFitImage: �ؼ��Ƿ��Զ���Ӧͼ��Ĵ�С
	//    [TRUE]: �����ؼ��Ĵ�С��ʹ������ͼ��Ĵ�Сһ��
	//   [FALSE]: �ؼ�����ԭ��
	//---------------------------------------------------------------------
	BOOL SetImageList(const CG2XImageList& imgList, BOOL bWndFitImage = FALSE);

	//---------------------------------------------------------------------
	//- ����ͼ���б�
	//- nImgWidth: ��ͼ��Ŀ��, ͼ���б����ͼ�����=ͼ��Ŀ��/nImgWidth, ����ȡ��
	//- nIDRes: ��ԴID
	//- lpszResType: λͼָ��ΪRT_BITMAP,�Զ������Ҫָ������,����:"PNG"
	//- bWndFitImage: �ؼ��Ƿ��Զ���Ӧͼ��Ĵ�С
	//    [TRUE]: �����ؼ��Ĵ�С��ʹ������ͼ��Ĵ�Сһ��
	//   [FALSE]: �ؼ�����ԭ��
	//- lpTransparentClr: ����ͼ���͸��ɫ, ʹ�û���ͼ��ʱ��������͸��ɫָ������ɫ��
	//---------------------------------------------------------------------
	BOOL SetImageList(INT nImageWidth, UINT nResID, LPCTSTR lpResType=RT_BITMAP, BOOL bWndFitImage=FALSE, Color* lpTransparentClr=NULL);

	//---------------------------------------------------------------------
	//- ����ͼ���б�
	//- nImgWidth: ��ͼ��Ŀ��, ͼ���б����ͼ�����=ͼ��Ŀ��/nImgWidth, ����ȡ��
	//- lpszImagePath: ͼ���·��
	//- bWndFitImage: �ؼ��Ƿ��Զ���Ӧͼ��Ĵ�С
	//    [TRUE]: �����ؼ��Ĵ�С��ʹ������ͼ��Ĵ�Сһ��
	//   [FALSE]: �ؼ�����ԭ��
	//- lpTransparentClr: ����ͼ���͸��ɫ, ʹ�û���ͼ��ʱ��������͸��ɫָ������ɫ��
	//---------------------------------------------------------------------
	BOOL SetImageList(INT nImageWidth, LPCTSTR lpszImagePath, BOOL bWndFitImage=FALSE, Color* lpTransparentClr=NULL);

	CG2XImageList* GetImageList() { return m_pImageList; }

public:

	// ����CG2XWnd::DrawSelf
	void DrawSelf(Gdiplus::Graphics *pGraphics);

	// ���ش˺����������ػ�ؼ��߿�
	virtual void DrawControlBorder(Graphics* pGraphics, G2XControlState ctrlState, G2XBorderStyle borderStyle);
	
	// �Ի�ؼ��������ش˺���,������ɻ�ͼ����
	virtual void DrawControl(Graphics* pGraphics, G2XControlState ctrlState);

//private:
	// ����CG2XWnd::DrawBorder
	void DrawBorder(Graphics *pGraphics);

protected:
	g_msg void OnCreate();
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);
	g_msg void OnLButtonUp(UINT nFlags, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();

protected:
	BOOL m_bMouseSinkage; // ��괦���³�״̬�������������²�û���ͷ�
	BOOL m_bMouseHover;
	G2XTextHAlign m_HTextAlign;
	G2XTextVAlign m_VTextAlign;
	BOOL m_bDrawFocus;
	BOOL m_bNotify;
	
	CSize m_szRoundCorner; // �߿�Բ�ǵĴ�С��Ĭ��SIZE(0,0)
public:
	CG2XImageList* m_pImageList; // ���û�е���SetImageList,m_pImageListָ�뽫Ϊ��
	CG2XControlColorData m_colorData;
};

G2X_END
