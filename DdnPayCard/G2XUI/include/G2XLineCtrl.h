#pragma once

namespace G2XUI{
	

class G2X_CLASSES CG2XLineCtrl : public CG2XControl
{
	DECLARE_DYNCREATE(CG2XLineCtrl)
public:
	CG2XLineCtrl();
	virtual ~CG2XLineCtrl();

	DECLARE_GDI_MESSAGE_MAP()

public:
	//---------------------------------------------------------------------
	//- (x,y): ֱ�ߵ���ʼ����,����Ϊ��㣬����ˮƽ���һ��ߴ�ֱ���µ�ֱ��
	//- nBreadth: ֱ�ߵĴֶ�
	//- nLong: ֱ�ߵĳ���
	//- bHorizontal: TRUE:ˮƽ����, FALSE:��ֱ����
	//---------------------------------------------------------------------
	BOOL Create(CG2XWnd* pParent, INT x, INT y, INT nBreadth, INT nLong, bool bHorizontal, UINT nID);

	void SetGradientStyle(G2XGradientStyle style, const Color& clr1, const Color& clr2, 
		Color* pClr3=NULL, Color* pClr4=NULL, BOOL bReDraw=TRUE);

public:
	virtual void DrawControl(Graphics* pGraphics, G2XControlState ctrlState);

	G2XGradientStyle m_styleGradient;

	CColor m_colors[4];
};




}