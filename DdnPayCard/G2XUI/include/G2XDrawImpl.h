#pragma once
#include "G2XUser.h"
#include "GraphicsEx.h"

G2XUI_BEGIN

namespace G2XUIFrame{
class G2X_CLASSES CG2XCaptionBar;
class G2X_CLASSES CG2XDialog;

}

class G2X_CLASSES CG2XCommandBarButton;
class G2X_CLASSES CG2XWnd;

class G2X_CLASSES CG2XDrawImpl
{
public:
	static const UINT COLOR_DISABLE_BORDER		= RGB(150,150,150);
	static const UINT COLOR_DISABLE_BK			= RGB(200,200,200);
	static const UINT COLOR_DISABLE_TEXT		= RGB(126,140,150);
	static const UINT COLOR_TEXT				= RGB(0  ,0  ,0  );
	static const UINT COLOR_BORDER				= RGB(185,205,226);
	static const UINT COLOR_HOVER_BORDER		= RGB(62, 156,212);

public:
	CG2XDrawImpl();
	virtual ~CG2XDrawImpl(void);

	static Font* CreateFont(
		WCHAR* lpzName, 
		INT nSize, 
		INT nStyle=FontStyleRegular);

	static Font* CreateFont(CFont* pFont);

	static CSize MeasureString(
		Graphics* pGraphics, 
		LPCTSTR lpcText, 
		Font* lpFont);

	static void MeasureString(
		Graphics* pGraphics,
		LPCTSTR lpcText,
		const INT nTextLength,
		IN Font* lpFont,
		OUT LPRECT lpMaxRect,
		OUT LPRECT lpMinRect);

	static void MeasureString(
		CG2XWnd* pG2XWnd,
		LPCTSTR lpcText,
		const INT nTextLength,
		OUT LPRECT lpMaxRect,
		OUT LPRECT lpMinRect);

	static void MeasureStringEx(
		Graphics* pGraphics,
		LPCTSTR lpcText,
		IN INT * nRanges,
		IN INT nRangeCount,
		IN Font* lpFont,
		OUT SIZE * szRanges);

	//---------------------------------------------------------------------
	//- ����һ��ͼ���Ӧ�ĵ�Ӱͼ��
	//---------------------------------------------------------------------
	static Bitmap* CreateInvertedImage(
		Bitmap* pBmp);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//- �������Ʒ���
//- ��������
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
public:

	//---------------------------------------------------------------------
	//- ���ƾ���
	//---------------------------------------------------------------------
	void DrawRectangle(
		Graphics* pGraphics,
		LPCRECT lpRect,
		const Color& clr,
		INT nPenWidth=1);

	//---------------------------------------------------------------------
	//- ���ƾ���
	//---------------------------------------------------------------------
	void DrawRectangle(
		Graphics* pGraphics,
		INT x,
		INT y,
		INT cx,
		INT cy,
		const Color& clr,
		INT nPenWidth=1);
	
	//---------------------------------------------------------------------
	//- ���ƾ��α߿�
	//- clr1��clr2�ֱ�ָ���߿���������ɫ����������ɫ, �м�����ɫ=clr1��clr2���м�ֵ
	//- bOuterBorderָ���߿��Ƿ�������չ,����������չ
	//---------------------------------------------------------------------
	void DrawRectangleBorder(
		Graphics* pGraphics,
		LPCRECT lpRect,
		const Color& clr1,
		const Color& clr2,
		INT nBorderWidth,
		BOOL bOuterBorder=TRUE
		); 

	//---------------------------------------------------------------------
	//- ���ƾ��α߿�
	//- nColorCount��nBorderWidth���һ��
	//- ���nColorCount = 1,�߿����ɫֻ��һ����ɫ
	//- ���pColors=NULL,��ô�߿����ɫΪ��ɫ
	//- bOuterBorderָ���߿��Ƿ�������չ,����������չ
	//---------------------------------------------------------------------
	void DrawRectangleBorder(
		Graphics* pGraphics,
		LPCRECT lpRect,
		INT nBorderWidth,
		const Color* pColors,
		INT nColorCount,
		BOOL bOuterBorder=FALSE);

	//---------------------------------------------------------------------
	//- ����Բ�Ǿ��α߿�
	//- nCornerX: Բ�ǵ�ˮƽ�뾶
	//- nCornerY: Բ�ǵĴ�ֱ�뾶,nCornerY��nCornerXһ������ȵ�
	//---------------------------------------------------------------------
	void DrawRoundBorder(
		Graphics* pGraphics,
		LPCRECT lpRect,
		INT nBorderWidth,
		INT nCornerX,
		INT nCornerY,
		Color* pColors,
		INT nColorCount,
		BOOL bOuterBorder=FALSE
		);


	void DrawRoundBorder(Graphics* pGraphics,LPCRECT lpRect,Color* pColor,INT nCornerX,INT nCornerY,INT nBorderWidth);
	void DrawRoundBorder(Graphics* pGraphics,INT x,INT y,INT cx,INT cy,Color* pColor,INT nCornerX,INT nCornerY,INT nBorderWidth);
	void FillRoundRect(Graphics* pGraphics,LPCRECT lpRect,Color* pColor,INT nCornerX,INT nCornerY);

	void DrawFocusRect(Graphics* pGraphics,LPRECT lpRect);

	

	//---------------------------------------------------------------------
	//- ��ָ�������л���ͼ��,ͼ��ƽ����������
	//---------------------------------------------------------------------
	void DrawImg(
		Graphics* pGraphics,
		LPRECT lpDes, 
		Image* pImg,
		LPRECT lpSrc,
		BYTE nAlpha=255,
		BOOL bToGray = FALSE           // �Ƿ����Ҷȱ任
		);

	//---------------------------------------------------------------------
	//- ��ͼ���е�ָ����ɫת��Ϊ��͸����Ϊ0��ͼ��
	//---------------------------------------------------------------------
	void TranformToTransparent(
		Bitmap* pBmp, 
		const Color& clrTransparent);

	//---------------------------------------------------------------------
	//- ����������
	//---------------------------------------------------------------------
	void FillRectangle(
		Graphics* pGraphics,
		INT x,
		INT y,
		INT cx,
		INT cy,
		const Color& clr);

	//---------------------------------------------------------------------
	//- ����������
	//---------------------------------------------------------------------
	void FillRectangle(
		Graphics* pGraphics,
		LPCRECT lpRect,
		const Color& clr);

	//---------------------------------------------------------------------
	//- ����������
	//- gradientStyle: ������, �ο�G2XGradientStyle�Ķ���
	//---------------------------------------------------------------------
	void FillRectangle(
		Graphics* pGraphics,
		LPCRECT lpRect,
		G2XGradientStyle gradientStyle,
		const Color& clr1,
		const Color& clr2,
		const Color* pClr3 = NULL,
		const Color* pClr4 = NULL);

	//---------------------------------------------------------------------
	//- ���Ƽ�ͷ
	//- ��תʱ����ͷ��lpCenterPoint����ת���˺���Ĭ�ϻ������ҵļ�ͷ
	//- ����: ��Ҫ�������µļ�ͷ,��Ҫָ����ת�Ƕ�Ϊ90,����:180��,����:270��
	void DrawArrow(
		Graphics* pGraphics,
		LPCRECT lpRect, 
		LPPOINT lpCenterPoint, 
		INT nRotateAngle=0,
		const Color& clr=Color(0,0,0,0), 
		BOOL bFill=TRUE);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//- �������
//- ��������
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
public:
	
	//- �������»���
	virtual void DrawMinimizeBox(
		Graphics* pGraphics, 
		LPCRECT lpRect,
		G2XControlState state);

	//---------------------------------------------------------------------
	//- ������󻯿���߻ָ���
	//- bMaximized=TRUE: ��󻯿�, =FALSE: �ָ���
	//---------------------------------------------------------------------
	virtual void DrawMaximizeBox(
		Graphics* pGraphics, 
		LPCRECT lpRect,
		G2XControlState state,
		BOOL bMaximized=TRUE);

	//- ���Ʊ������Ĺرհ�ť
	virtual void DrawCaptionBarClose(
		Graphics* pGraphics, 
		LPCRECT lpRect,
		G2XControlState state);

	//- ���Ʊ������ı���, ���ش˷�������Ϊ�ض��ı��������Ʊ���
	virtual void DrawCaptionBarBk(
		Graphics* pGraphics,
		G2XUIFrame::CG2XCaptionBar* pCaptionBar,
		LPRECT lpRect);

	//- ����CG2XDialog�ı߿�
	virtual void DrawG2XDialogBorder(
		Graphics* pGraphics,
		G2XUIFrame::CG2XDialog* pDlg);

	//- ���ƴ�CG2XFoundationWindow�̳еĴ���ı߿�
	virtual void DrawG2XFoundationWindowBorder(
		Graphics* pGraphics,
		LPRECT lpRect,
		INT nBorderBreadth,
		BOOL bRoundBorder,
		INT nCornerX=8,
		INT nCornerY=8
		);

	//---------------------------------------------------------------------
	//- ����push button
	//---------------------------------------------------------------------
	virtual void DrawPushButton(
		Graphics* pGraphics,
		LPCRECT lpRect,
		LPCTSTR lptString,
		const Color& clrText,
		CFont* pFont,
		G2XControlState ctrlState,
		G2XTextHAlign hAlign,
		G2XTextVAlign vAlign);

	//---------------------------------------------------------------------
	//- ����push button
	//---------------------------------------------------------------------
	virtual void DrawLinkButton(
		Graphics* pGraphics,
		LPCRECT lpRect,
		LPCTSTR lptString,
		const Color& clrText,
		CFont* pFont,
		G2XControlState ctrlState,
		G2XTextHAlign hAlign,
		G2XTextVAlign vAlign);

	//---------------------------------------------------------------------
	//- ����alpha button
	//---------------------------------------------------------------------
	virtual void DrawAlphaButton(
		Graphics* pGraphics,
		LPRECT lpRect,
		LPCTSTR lptString,
		const Color& clrText,
		CFont* pFont,
		G2XControlState ctrlState,
		G2XTextHAlign hAlign,
		G2XTextVAlign vAlign);

	//---------------------------------------------------------------------
	//- ���� ��ѡ��
	//---------------------------------------------------------------------

	virtual void DrawCheckBox(
		Graphics* pGraphics,
		LPCRECT lpRect,
		BOOL bCheck,
		LPCTSTR lptString,
		const Color& clrText,
		CFont* pFont,
		G2XControlState ctrlState,
		G2XTextHAlign hAlign,
		G2XTextVAlign vAlign);

	// ���� ��ѡ��ֻ���ƿ�򣬲������ı�
	virtual void DrawRadioBox(
		Graphics* pGraphics,
		LPRECT lpRect,
		BOOL bCheck,
		G2XControlState ctrlState=G2XControlStateNormal);

	// ���Ƹ�ѡ��
	virtual void DrawCheckBox(
		Graphics* pGraphics,
		LPRECT lpRect,
		BOOL bCheck,
		G2XControlState ctrlState=G2XControlStateNormal);

	//---------------------------------------------------------------------
	//- ���� ���οؼ����۵�ͼ��
	//---------------------------------------------------------------------

	virtual void DrawTreeIcon(
		HG2XWND, 
		Graphics*, 
		LPRECT, 
		G2XNodeState state, 
		G2XControlState ctrlState);

	virtual void DrawText(
		Graphics* pGraphics,
		LPCTSTR lpText,
		LPCRECT lpRect,
		CFont* pFont,
		const Color& clrText,
		G2XControlState ctrlState,
		G2XTextHAlign hAlign=G2XTextHAlignLeft,
		G2XTextVAlign vAlign=G2XTextVAlignMiddle,
		BOOL bTrimmingText=FALSE);

	virtual void DrawText(
		Graphics* pGraphics,
		LPCTSTR lpText,
		LPCRECT lpRect,
		Font* lpFont,
		const Color& clrText,
		G2XControlState ctrlState,
		G2XTextHAlign hAlign=G2XTextHAlignLeft,
		G2XTextVAlign vAlign=G2XTextVAlignMiddle,
		BOOL bTrimmingText=FALSE);


	virtual void DrawCommandBarButtonMask(
		Graphics* pGraphics,
		CG2XCommandBarButton* pBtn,
		LPCRECT lpRect,
		G2XControlState state
		);
public:
	CColor colorDisableBorder; // ���治����ʱ�ı߿���ɫ
	CColor colorDisableBk;     // ���治����ʱ�ı�����ɫ
	CColor colorDisableText;   // �ı�������ʱ����ɫ
	CColor colorText;          // ͨ���ı�����ɫ
	CColor colorBorder;        // ͨ�ñ߿����ɫ
	CColor colorHoverBorder;   // ��������Ͽ�ʱ�ı߿���ɫ
	Font* m_lpDefaultFont;     // Ĭ������
	Gdiplus::SmoothingMode m_graphicsSmoothingMode; // Ĭ��ģʽΪ�������
};


G2X_END