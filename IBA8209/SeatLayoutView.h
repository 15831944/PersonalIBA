#pragma once


#include "SeatList.h"
#include "Seat.h"
#include "Seat2.h"

class CGdiPlusBitmap;
class CSeatList2;
// CSeatLayoutView ��ͼ
const size_t DefaultBitmapSize = 2000;
//const INT_PTR DefaultRadius = 12;
const INT_PTR DefaultRadius = 24;
const INT_PTR DefaultRadius2 = 50;

enum SEAT_COMMAND
{
	M_SEAT_LOCK,			// ������λ�˵��Ѿ��ͻ����˵��ϲ�
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
	
	static inline void SetMaxRight(int nVal)		{ m_nMaxRight = nVal; }
	static inline void SetMaxBottom(int nVal)		{ m_nMaxBottom = nVal; }
	static void SetMaxRightCmp(int nVal);
	static void SetMaxBottomCmp(int nVal);
	static inline int GetMaxRight()				{ return m_nMaxRight; }
	static inline int GetMaxBottom()				{ return m_nMaxBottom; }
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
	afx_msg void OnUpdateSeatAddAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSeatDelAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSeatComputerMenu(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	afx_msg void OnSeatComputerMenu();
	afx_msg void OnSeatAddAll();
	afx_msg void OnSeatDelAll();
private:
	//��ʾ��ģ̬����
	CPPToolTip m_ToolTip;
	//�����ƶ�Ԫ�صı��
	INT m_nIndex;
	//�Ƿ����ƶ���λ
	BOOL m_bMoveRect;
	INT m_nIndex2;
	BOOL m_bMoveRect2;
	//����ģʽ
	SEAT_COMMAND m_CommandMode;
	//��λͼ
	//CBitmap * m_pSeatBitmap;
	CGdiPlusBitmap *m_pSeatBitmap;
	//��λ״̬��ʾͼ
	CBitmap * m_pSeatTipsBitmap;
	//��λ��Ϣ�б�
	CSeatList * m_pSeatList;

	// ��������̨��WC
	CSeatList2 * m_pSeatList2;

	// �Ƿ�������ʾ
	BOOL m_bTotalView;
	//������Χ
	CSize m_sizeTotal;
	//��갴�µ�����
	CPoint m_ptPrePoint;
	CPoint m_ptPrePoint2;

	static int m_nMaxRight;				// �ն˵����ұߵ�����
	static int m_nMaxBottom;			// �ն˵���ͱߵ�����
private:
	// ������λͼ���ó�Ա�������ն���
	void LoadSeatBitmap(CGdiPlusBitmap * pSeatBitmap);
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
	void AddSeat(CPoint ptCurpoint, CString strTerminalID, CString strTerminalIP, INT_PTR nDirection = 0, BOOL bRedraw = TRUE, bool bAddScrollPos = TRUE);
	
	struct TermInfo{
		CString strTermID;
		CString strIP;
		bool operator<(const TermInfo& rhs) const  
		{   
			CStringA strIPA(strIP), strRhsA(rhs.strIP);
			ULONG ulIPL = ntohl(inet_addr((PCSTR)strIPA.GetString()));
			ULONG ulIPR = ntohl(inet_addr((PCSTR)strRhsA));
			if (ulIPL != ulIPR)   
				return (ulIPL < ulIPR);   
			return ulIPL < ulIPR;   
		}   
	};
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnUpdateSeat(WPARAM wParam, LPARAM lParam);
	void Loadseat();
	void SaveSeat();
	afx_msg void OnAddCashier();
	afx_msg void OnAddToilet();
	afx_msg void OnAddRefrigraetor();
	afx_msg void OnAddEntrance();
	void AddSeat2(Seat2Type type);
	afx_msg void OnMyrefresh();
	afx_msg void OnUpdateMyRefresh(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNewuser(CCmdUI *pCmdUI);
	afx_msg void OnPaint();

	void AutoAddAllSeatAfterSync();
	void UpdateSeatAfterSync();
};


