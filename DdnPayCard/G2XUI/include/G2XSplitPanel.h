#pragma once
#include "g2xpanel.h"

//========================================
//= CG2XSplitPanel �ɷָ�����
//=		���ɷָ��:
//=			���������� or ����������
//=		�ָ�����п����ڴ����ָ����
//========================================
G2XUI_BEGIN

class G2X_CLASSES CG2XSplitPanel :
	public CG2XPanel
{
	DECLARE_DYNCREATE(CG2XSplitPanel)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XSplitPanel(void);
	virtual ~CG2XSplitPanel(void);

	//----------------------------------------------------
	// style:     ���ķָ����
	// sizeStyle: �����Ĵ�С���
	//----------------------------------------------------
	BOOL Create(
		CG2XWnd* pParent, 
		LPCRECT lpRect, 
		UINT nID, 
		G2XSplitPanelStyle style=G2XSplitPanelStyleHorizontal,
		G2XSplitPanelSizeStyle sizeStyle=G2XSplitPanelSizeStylePercentage);

	// Ϊ�ָ��Ŀ�����
	void AttachPanelLeft	(CG2XPanel* pPanel);
	void AttachPanelRight	(CG2XPanel* pPanel);
	void AttachPanelTop		(CG2XPanel* pPanel);
	void AttachPanelBottom	(CG2XPanel* pPanel);

	// �������, ���ش˷����������л����ڵ�������������
	//   ���磺 �ڷָ��ߵ��м���Ӱ�ť��������ťλ�á�
	virtual void AdjustPanel();	
	
	G2XSplitPanelStyle GetStyle();

	CG2XPanel* GetPanelLeft();
	CG2XPanel* GetPanelRight();
	CG2XPanel* GetPanelTop();
	CG2XPanel* GetPanelBottom();

	// ���÷ָ��ߵĿ�ȣ�������ָ��Ϊ0����ô�ָ��߲�����ʾҲ�����϶�
	void SetSplitLineBreadth(INT nBreadth);

	//----------------------------------------------------
	// ���÷ָ��ߵ���ʾλ�õı������ָ�����С�����ı�ʱ��
	// ��һ�������͵ڶ�������尴���������������С
	//----------------------------------------------------
	void SetPercentage(BYTE nPercentage);

	//----------------------------------------------------
	// ��������嵱ǰ���ֵĴ�С
	//   m_sizeStyle = G2XSplitPanelSizeStyleFirst/Secondʱ�����ò���Ч
	//----------------------------------------------------
	void SetKeepSize(UINT nSize);

	//----------------------------------------------------
	// �̶��ָ���
	//----------------------------------------------------
	void FixedSplitLine(BOOL bFixed);

	//----------------------------------------------------
	// nDis1: ���÷ָ������һ�����߽����С����
	// nDis2: ���÷ָ�����ڶ������߽����С����
	//----------------------------------------------------
	void SetMinimizeDistance(UINT nDis1, UINT nDis2);

protected:
	g_msg void OnCreate();
	g_msg void OnMouseMove(UINT nFlags, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();
	g_msg void OnDragging(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnDragged(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnSize();

	virtual void DrawSelf(Gdiplus::Graphics *pGraphics);

protected:
	RECT GetSplitLineRect(LPPOINT lpPt);

	void DrawTempSplitLine(CPoint& ptTmp);

protected:
	G2XSplitPanelStyle m_style;
	G2XSplitPanelSizeStyle m_sizeStyle;

	CG2XPanel* m_pPanelA;
	CG2XPanel* m_pPanelB;

	// �ָ��ߵĿ��
	INT m_nSplitLineBreadth; 

	// �ָ��ߵ�����λ��
	CPoint m_ptSplitLine;

	// �ָ�������λ�õİٷֱ�, 0~100
	BYTE m_nPercentage;

	// �Ƿ�̶��ָ���,�̶��ķָ��߲������϶�
	BOOL m_bFixedSplitLine;

	INT m_nKeepedSize;

	INT m_nMinimizeDistance1;

	INT m_nMinimizeDistance2;

	HCURSOR m_hSizeCursor;

	CDC* m_pTemDC;
};

G2X_END
