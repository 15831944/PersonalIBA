#pragma once
#include "G2XWnd.h"

G2XUI_BEGIN

class G2X_CLASSES CG2XScrollBar :
	public CG2XControl
{
	DECLARE_DYNCREATE(CG2XScrollBar)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XScrollBar(void);
	virtual ~CG2XScrollBar(void);

	BOOL Create(CG2XWnd* pParent, BOOL bHorizontal, CRect rcInParent, UINT nID);
protected:
	//virtual void DrawSelf(Graphics* pGraphics);
	virtual void DrawControl(Graphics* pGraphics,G2XControlState ctrlState);

	// ���ƹ�������ߵİ�ť���߶����İ�ť
	virtual void DrawLeftButton(Graphics* pGraphics,LPRECT lpRect,G2XControlState ctrlState);

	// ���ƹ������ұߵİ�ť���ߵײ��İ�ť
	virtual void DrawRightButton(Graphics* pGraphics,LPRECT lpRect,G2XControlState ctrlState);

	// ���ƻ���
	virtual void DrawSlider(Graphics* pGraphics,LPRECT lpRect,G2XControlState ctrlState);

	// ���Ʊ����߿�
	virtual void DrawBlackground(Graphics* pGraphics,LPRECT lpRect,G2XControlState ctrlState);

protected:
	g_msg void OnCreate();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseHover(UINT nFlags, CPoint point);
	void OnMouseLeave();

	g_msg void OnDragging(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnDragged(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnSize();
	g_msg void OnTimer(UINT nEvent);
protected:
	RECT GetSliderRect();
	BOOL TestPointInButton(CPoint);
	virtual void OnLeftButtonClick();
	virtual void OnRightButtonClick();

	void RelayOut();

public:
	void SetRange(INT nFrom, INT nEnd, INT nPage);
	void SetPosition(INT nPos);
	INT GetPosition();

	void SetIncreasement(INT nInc);
	INT GetRangeDown();
	INT GetRangeUp();
	INT GetPageSize();

protected:
	INT GetPositonLong(INT nPos);
	INT GetLongPositionOff(INT nLong);
	void ChangePosAndNotify(INT nPos);

protected:
	INT m_nPosition;  // ����ĵ�ǰλ��
	INT m_nRangeDown; // �������ķ�Χ����
	INT m_nRangeUp;   // �������ķ�Χ����
	INT m_nPageSize;  // ҳ�Ĵ�С
	BOOL m_bHorizontal; // �Ƿ�Ϊˮƽ������
	BOOL m_bMouseDown;

	CPoint m_ptMouseDown;
	INT m_nMouseIn;

	BOOL m_bDraggingSlider;

	INT m_nPosDraggingFrom;

	CRect m_rcBtn1;
	CRect m_rcBtn2;
	CRect m_rcSliderArea; // ��������
	CSize m_szSlider;     // ����Ĵ�С

	INT m_nIncMent; // ÿ��һ�����ǰ�ť������һ�ƶ���λ�ô�С��
};

G2X_END
