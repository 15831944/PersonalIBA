#pragma once
#include <afxtempl.h>
#include <map>
#include <vector>

//-------------------------------------------------------------------------
// class G2X_CLASSES : CG2XWnd ����Gdi++��ͼ��ģ�ⴰ����Ļ���
//
// CG2XWnd ���⻯����, ���������Ĵ��ڣ�û�д��ھ����û����Դ
//   ʵ���������޿ؼ�������QQ���������һ����ֻ��������һ�����ڡ�
//
// ע: 
//   1) G2XUI������У����е�CG2XWnd�ؼ�������Ϊ���ⴰ��, CWnd���ڳ���Ϊ��ʵ����
//   2) ���д�CG2XWnd��̳е�����������Ӷ��з��䣬��ʵ����ֻ��Ҫ���ٶ����CG2XWnd
//      ��DestroyWnd,�Ϳ����Զ��������е������ⴰ�ڡ�
//   3) ��Ҫ��CG2XWnd����ʹ��delete �ؼ���, CG2XWnd����ռ�õ��ڴ��Զ����ա�
//-------------------------------------------------------------------------
#define g_msg 
#pragma warning(disable: 4244)

G2XUI_BEGIN

class G2X_CLASSES CG2XToolTipCtrl;

class G2X_CLASSES CG2XWnd : public CG2XObject
{
	DECLARE_DYNCREATE(CG2XWnd)

	DECLARE_GDI_MESSAGE_MAP()

public:
	CG2XWnd(void);
	virtual ~CG2XWnd(void);

	void DestroyWnd();

	//---------------------------------------------------------------------
	// ��������Ľ���CG2XWnd����
	//		CWnd��ĵ�һ��CG2XWnd�������
	// pRealWnd: ��ʵ���ڣ�������һ����Ч�Ĵ���
	// bHighspeedRendering: �Ƿ����ø��ٻ�ͼ
	//---------------------------------------------------------------------
	BOOL CreateSuperWnd(
		CWnd* pRealWnd, 
		LPCRECT lpRect, 
		UINT nID=0, 
		BOOL bHighspeedRendering=TRUE);

	//---------------------------------------------------------------------
	// �������ⴰ��
	//---------------------------------------------------------------------
	BOOL Create(
		CG2XWnd* pParent, 
		LPCTSTR lpWndText, 
		LPCRECT lpRect, 
		UINT nID,
		G2XBorderStyle borderStyle=G2XBorderNone);

	static CG2XWnd* FromHandle(HG2XWND hG2XWnd);

	HG2XWND GetHandle() { return m_hG2XWnd; }
	HG2XWND GetSafeHandle();

	CG2XToolTipCtrl* GetToolTipCtrl();
	BOOL ReplaceToolTipCtrl(CG2XToolTipCtrl* pNewToolTip);

	BOOL RemoveChild(HG2XWND hG2XWnd);
	BOOL RemoveChild(CG2XWnd*& pWnd);

	BOOL Replace(UINT nID, CG2XWnd* pWndNew, BOOL bCopyOldInfo=FALSE);
	BOOL Replace(CG2XWnd* pWnd, CG2XWnd* pWndNew, BOOL bCopyOldInfo=FALSE);
public:
	// ������Ӧ(�����ڵ��ô˷���)
	//- pFrontBmp: ��pGraphics����һ����������
	g_msg virtual void OnDraw(Graphics *pGraphics, Bitmap *pFrontBmp);  

	// �����ͼ�����������������������Ӵ���
	virtual void DoDraw(Graphics *pGraphics, Bitmap *pFrontBmp);

	// ����ģʽָ��Ϊ��͸��ʱ�Զ�����,���Ʊ���,Ĭ����ָ���ı�����ɫ���
	virtual void DrawBackground(Graphics *pGraphics);
	
	
	// ��������
	virtual void DrawSelf(Graphics *pGraphics) { }       
	virtual void DrawBorder(Graphics *pGraphics);

public:
	void Redraw(BOOL bDrawAll = FALSE);    

	BOOL PointIdWnd(CPoint point); // ʵ�ʴ��ڵĵ��Ƿ��ڱ����ⴰ��
	void EnableWnd(BOOL bEnable, BOOL bRepaint=TRUE);
	void ShowWnd(BOOL bShow);
	void MoveWnd(CRect rcNew, BOOL bRepaint=TRUE);
	BOOL GetWndShown() { return m_bShown; }

	CRect VirtualToReal(CRect rect); // ���������ĳ������ת��Ϊʵ�ʴ����е�λ��
	void VirtualToReal(CPoint& point);
	void RealToVirtual(CPoint& point);
	CRect GetWndRect();  // ��ȡ�������ڸ������е�λ��
	void SetToolTip(BOOL bHasTip, LPCTSTR lpTip=NULL);
	
	BOOL IsEnabled() { return m_bEnable; }
	BOOL IsShown() { return m_bShown; }
	CWnd* GetSuperWnd();
	CG2XWnd* GetParentWnd();
	CG2XWnd* GetOwner();
	void SetOwner(CG2XWnd* pWnd);
	CG2XWnd* GetTopmostParentWnd();
	UINT GetControlID();

	CRect GetClientRect();
	void SetTransparent(bool bTransparent);
	BOOL GetTransparent() { return m_bTransparent; }
	void SetBKColor(const CColor& clr) { m_clrBk = clr; }
	void SetBkImage(const Image& img);

	CG2XWnd* GetWndByID(UINT nID);  // ��ȡID��Ӧ�����ⴰ��,�����������������������ⴰ��
	void SetWindowText(LPCTSTR strTxt, BOOL bRepaint=TRUE);
	CString GetWindowText();
	void SetFocus(BOOL bFocus);
	CG2XWnd* GetFocus();
	void EnableHoverOrLeave(BOOL bMouseHover, BOOL bMouseLeave, UINT nTimeElapse = 20);
	void SetTabStop();

	CFont* GetFont();
	void SetFont(CFont* pFont, BOOL bRepaint=TRUE);

	void SetCapture();     // ���ò��������קʱ���ã�����Ҫʱ����ReleaseCapture
	void ReleaseCapture(); // �ͷŲ���

	void EnableDragging(BOOL bEnable); // �Ƿ�������ק������Ļ��ͻ����OnDragging, ��OnDragged;

	INT GetChildWndCount();

	void SetG2XCursor(HCURSOR hCursor);
	void SetToolTipDelayTime(DWORD dwMs);

	INT GetBorderBreadth() { return m_nBorderBreadth; }
	G2XBorderStyle GetBorderStyle() { return m_styleBorder; }

public:

	//---------------------------------------------------------------------
	//- ����SuperWnd���͵�������Ϣ
	//---------------------------------------------------------------------
	virtual BOOL PreTranslateMessage(MSG* pMsg); 

	//---------------------------------------------------------------------
	//- ��G2XWnd��������Ϣ
	//---------------------------------------------------------------------
	BOOL SendMessage(UINT nMsg, UINT nGMsgType=GDIM_MESSAGE, WPARAM wParam=0, LPARAM lParam=0);

protected:

	//---------------------------------------------------------------------
	//- ����Ϣת�Ƹ����е������ⴰ�ڴ���
	//---------------------------------------------------------------------
	BOOL TransferMessage(MSG* pMsg);


	//---------------------------------------------------------------------
	//- �������е�G2XWnd��Ϣ
	//- nGMsgType:  GDIM_MESSAGE , GDIM_MOUSE , GDIM_COMMAND , GDIM_NOTIFY
	//---------------------------------------------------------------------
	virtual BOOL OnGdiWndMsg(UINT nGMsgType, UINT nID, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);

	//---------------------------------------------------------------------
	//- ����G2XWnd��������Ϣ
	//---------------------------------------------------------------------
	virtual BOOL OnGdiCommand(UINT nID, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------------------------------
	//- ����G2XWnd���Զ�����Ϣ
	//---------------------------------------------------------------------
	virtual BOOL OnGdiMessage(UINT nID, WPARAM wParam, LPARAM lParam, LONG* lpResult);

	//---------------------------------------------------------------------
	//- ����G2XWnd��֪ͨ��Ϣ
	//- wParam ָ��NMHDR�ṹ
	//---------------------------------------------------------------------
	virtual BOOL OnGdiNotify(UINT nID, WPARAM wParam, LPARAM lParam, LONG* lpResult);

	virtual BOOL OnGdiKeyboard(UINT nID, WPARAM wParam, LPARAM lParam, LONG* lpResult);

	g_msg void OnCreate();
	g_msg void OnMouseMove(UINT nFlags, CPoint point);
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);
	g_msg void OnLButtonUp(UINT nFlags, CPoint point);
	g_msg void OnRButtonDown(UINT nFlags, CPoint point);
	g_msg void OnRButtonUp(UINT nFlags, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	g_msg void OnLButtonDoubleClick(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave(); // ����뿪������
	g_msg void OnTimer(UINT nEvent);
	g_msg void OnDragging(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnDragged(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnSize();
	g_msg void OnShowWnd();
	g_msg void OnDestroy();
	g_msg BOOL OnKeyDown(UINT,UINT,UINT);
protected:
	void SetTimer(UINT nEvent, UINT nTimeElapse=10);
	void KillTimer(UINT nEvent);
	
	// ��SuperWnd��������֪ͨ
	LRESULT SendNotify(UINT nCode);
	
	BOOL NextTabStop();              // ��Tab��ʱ���Ƿ��ܴ����ƶ�����һ������

	UINT GetHoverTimeID();

	BOOL GetParentEnable();

	CG2XDrawImpl* GetDrawImpl();
	void CG2XWnd::CalcTextRect( LPCTSTR lpText,LPRECT lpRect,Gdiplus::Graphics *pGraphics );
protected:
	BOOL m_bEnable;         // �Ƿ����ñ��ؼ�
	BOOL m_bShown;          // �Ƿ���ʾ���ؼ�
	BOOL m_bHasToolTip;     // �Ƿ����ı���ʾ
	BYTE  m_nBorderBreadth;  // ���ڱ߿�Ŀ��,���Ϊ0��ʾû�б߿�
	G2XBorderStyle m_styleBorder; // �߿��ķ��

	bool m_bTipShown;       
	CString m_strTip;       // �ı���ʾ������
	UINT m_nTipDelayTime;   // �ı���ʾǰ���ӳ�ʱ��   

	bool m_bTransparent;            // �����ⴰ�ڵı����Ƿ�Ϊ͸��, TRUE:����ɫ���߱���ͼ��ȫ��������
	CColor m_clrBk;					// �����ⴰ�ڵı���ɫ,�������ͼ��Ϊ�ǿգ�����ɫ������
	Image* m_pBkImg;                // ����ͼ��
	BOOL m_bEnableHover;            // �Ƿ�������껮���¼�
	BOOL m_bEnableLeave;            // �Ƿ���������뿪�¼�
	UINT m_nHoverTime;              // ��껮��������
	CPoint m_mousePoint;            // ����ƶ������ⴰ���Ϸ������λ�ã������ʵ�ʴ��ڣ�
	UINT m_nMouseFlags;             // ctrl,shift,alt�ȼ����µı�־,�ο�WM_MOUSEMOVE
	BYTE m_nMouseInAndOut;
	
	BOOL m_bFocus;                  // �����ⴰ���Ƿ�Ϊ����״̬
	BOOL m_bTabStop;                // �Ƿ�����TabStop
	CFont* m_pFont;
	BOOL m_bEnableDragging;  
	CPoint m_ptDraggingFrom; 
	
	BOOL m_bHighspeedDraw;			// �Ƿ����ø߶Ȼ�ͼ

public:
	Bitmap* m_pFrontBk;				// ���ڵ�ǰ��λͼ,�����ػ�ʱ���Ƶ����λͼ��,���㴰�ڰ������Ƶ��豸����
	

	// �������ⴰ��ר��
private:
	typedef std::map<UINT, std::vector<HG2XWND> > MapTimerWnd;
	typedef std::vector<UINT> IDArray;

	MapTimerWnd m_mapTimerWnd;
	CG2XWnd* m_pFocus;
	CG2XWnd* m_pWndMouseIn;
	IDArray m_tabStops;
	CG2XWnd* m_pWndCaptured; // ��ǰ��������겶��Ĵ���

	CG2XToolTipCtrl* m_pToolTipCtrl;
	
	UINT m_nTabLastID;
	HCURSOR m_hCursor;	

public:
	static const UINT TIMERREDRAW = 0xFFFD;
	static const UINT TIMERHOVER = 0xFFFE;
	INT m_nRedrawRequestCount;
	typedef std::vector<HG2XWND> G2XWndArray;
	G2XWndArray m_arrayOnlyRedraw;
};

G2X_END
