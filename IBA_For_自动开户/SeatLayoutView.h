#pragma once


#include "SeatList.h"
#include "Seat.h"

// CSeatLayoutView ��ͼ
const size_t DefaultBitmapSize = 2000;
const INT_PTR DefaultRadius = 12;

enum SEAT_COMMAND
{
	M_SEAT_LOCK,
	M_SEAT_ADD,
	M_SEAT_SAVE,
	M_SEAT_LOAD,
	M_SEAT_COMPUTER_MENU    // 2011/10/21-8208-gxx: 
};

class CSeatLayoutView : public CScrollView
{
	DECLARE_DYNCREATE(CSeatLayoutView)
	DECLARE_MESSAGE_MAP()

protected:
	CSeatLayoutView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSeatLayoutView();

public:
	virtual void OnInitialUpdate();

protected:

	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuScrollzoom();
	afx_msg void OnUpdateMenuScrollzoom(CCmdUI *pCmdUI);
	afx_msg void OnMenuFullzoom();
	afx_msg void OnUpdateMenuFullzoom(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSeatLockseat();
	afx_msg void OnUpdateSeatLockseat(CCmdUI *pCmdUI);
	afx_msg void OnSeatAddseat();
	afx_msg void OnUpdateSeatAddseat(CCmdUI *pCmdUI);
	afx_msg void OnSeatLoadseat();
	afx_msg void OnUpdateSeatLoadseat(CCmdUI *pCmdUI);
	afx_msg void OnSeatDel();
	afx_msg void OnSeatRefresh();
	afx_msg void OnUpdateSeatDel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSeatRefresh(CCmdUI *pCmdUI);
	afx_msg void OnClockwise();
	afx_msg void OnUpdateClockwise(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	afx_msg void OnSeatComputerMenu();

private:
	//��ʾ��ģ̬����
	CPPToolTip m_ToolTip;
	//�����ƶ�Ԫ�صı��
	INT m_nIndex;
	//�Ƿ����ƶ���λ
	BOOL m_bMoveRect;
	//����ģʽ
	SEAT_COMMAND m_CommandMode;
	//��λͼ
	CBitmap * m_pSeatBitmap;
	//��λ��Ϣ�б�
	CSeatList * m_pSeatList;
	// �Ƿ�������ʾ
	BOOL m_bTotalView;
	//������Χ
	CSize m_sizeTotal;
	//��갴�µ�����
	CPoint m_ptPrePoint;

private:
	// ������λͼ���ó�Ա�������ն���
	void LoadSeatBitmap(CBitmap * pSeatBitmap);
	// ��ʾ��������
	void SetViewSize(BOOL bViewType, BOOL bDefault = FALSE);
	// �滭��λ������,���������X.Y����ķ���ϵ��
	void DrawSeat(CDC * pDC = NULL,  double XRatio = 1.0, double YRatio = 1.0);
	// ��������ʾģʽ�µ�����ת���ɹ���ģʽ�µ�����
	BOOL ToScrollRect(CRect& srcFullRect);
	// ת��������ģʽ�µĵ�����
	BOOL ToScrollPoint(CPoint& srcFullPoint);
	// ������������ʾģʽ�µ�����ת��������ģʽ�µ�����
	BOOL ToTotalRect(CRect& srcTotalRect);
	// ת��������ģʽ�µĵ�����
	BOOL ToTotalPoint(CPoint& srcTotalPoint);
	// �������������λ����
	void AddSeat(CPoint ptCurpoint, CString strTerminalID, CString strTerminalIP, INT_PTR nDirection = 0);

};


