#pragma once

G2XUI_BEGIN

class G2X_CLASSES CG2XCalendar : public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XCalendar)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XCalendar(void);
	virtual ~CG2XCalendar(void);

	enum Week{
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday	
	};
protected:
	virtual void DrawSelf(Gdiplus::Graphics* pGraphics);
	g_msg void OnCreate();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseHover(UINT nFlags, CPoint point);
	void OnMouseLeave();
protected:
	virtual void DrawBorder(Gdiplus::Graphics* pGraphics);
	virtual void DrawBackground(Gdiplus::Graphics* pGraphics, LPRECT lpRect);
	virtual void DrawHead(Gdiplus::Graphics* pGraphics, LPRECT lpRect);
	virtual void DrawYear(Gdiplus::Graphics* pGraphics, LPRECT lpRect);
	virtual void DrawMonth(Gdiplus::Graphics* pGraphics, LPRECT lpRect);
	virtual void DrawYearLeftBtn(Gdiplus::Graphics* pGraphics, LPRECT lpRect, BOOL bHover);
	virtual void DrawYearRightBtn(Gdiplus::Graphics* pGraphics, LPRECT lpRect, BOOL bHover);
	virtual void DrawMonthLeftBtn(Gdiplus::Graphics* pGraphics, LPRECT lpRect, BOOL bHover);
	virtual void DrawMonthRightBtn(Gdiplus::Graphics* pGraphics, LPRECT lpRect, BOOL bHover);
	virtual void DrawToday(Gdiplus::Graphics* pGraphics, LPRECT lpRect);
	virtual void DrawWeekdayName(Gdiplus::Graphics* pGraphics, LPRECT lpRect ,LPCTSTR lpName);

	virtual void DrawDay(
		Gdiplus::Graphics* pGraphics, LPRECT lpRect, 
		BOOL bHover, 
		BOOL bSelected, 
		INT nDay,
		INT nFlag=0   // -1��ʾ��һ���µ�, 0��ʾ��ǰ��, 1��ʾ��һ��
		);

	void HoverRedrawHelper(void* pAddr, INT newVal);

	
private:
	INT WeekDayToIndex(INT nWeekDay);
	INT IndexToWeekDay(INT nIndex);

	Week GetWeekDay(INT year,INT month, INT day);

	INT GetPreMonthDays(INT* lpMonth=NULL);
	INT GetNextMonthDays(INT* lpMonth=NULL);
	INT GetMonthDays(INT month);

public:
	// ���õ�һ���Ƿ�Ϊ������
	void SetFirstSunday(BOOL bSunday);

	// ���ܵ�һ���Ƿ�涨Ϊ������,�˴�����Ϊ��һ��Ϊ������
	// ����һ�����ڵĵ�һ�쵽��7�������
	void SetWeekDayName(
		LPCTSTR lp1 = _T("��"),
		LPCTSTR lp2 = _T("һ"),
		LPCTSTR lp3 = _T("��"),
		LPCTSTR lp4 = _T("��"),
		LPCTSTR lp5 = _T("��"),
		LPCTSTR lp6 = _T("��"),
		LPCTSTR lp7 = _T("��"));

	void SetDate(INT year, INT month, INT day);
	void SetDate(const COleDateTime& dt);

	COleDateTime GetDate();

public:
	
	INT m_nYear;
	INT m_nMonth;
	INT m_nDay;
	
	CSize m_szYearBtn;
	CSize m_szMonthBtn;

	CRect m_rcHead;  // ���� �꣬��
	CRect m_rcYear;
	CRect m_rcMonth;
	CRect m_rcYearLeftBtn;
	CRect m_rcYearRightBtn;
	CRect m_rcMonthLeftBtn;
	CRect m_rcMonthRightBtn;

	CRect m_rcWeekNames[7];
	CRect m_rcWeekDays[6][7];

	INT m_nWeekDays[7];
	CString m_strWeekDays[7];
	
	CRect m_rcToday;

	INT m_nMarginLeft;
	INT m_nMarginRight;
	INT m_nMarginTop;
	INT m_nMarginBottom;

	INT m_nHeadHeight;
	INT m_nWeekNameHeight;

	INT m_nDayGap;
	INT m_nTodayHeight;

	BOOL m_bFirstSunday;

	Week m_weekCurrent;
	Week m_weekMonthFirstDay; // ��ǰ�µĵ�һ�����ڼ�

	INT m_nHoverRow,m_nHoverCol;
	INT m_nHoverDay;

	BOOL m_bYearLHover;
	BOOL m_bYearRHover;
	BOOL m_bMonthLHover;
	BOOL m_bMonthRHover;

	INT m_nMatrixDays[6][7];
};

G2X_END
