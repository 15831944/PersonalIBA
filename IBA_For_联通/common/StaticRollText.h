#pragma once
/**************************************************************************
class: CStaticRollText : public CStatic
for: �������ֵ��ı���ʾ�ؼ�, ��Ϊ�ؼ��趨Ҫ�������ַ�������
using:
    ���û������Ҫ����������;(����SetText ����ѭ������AddString)
    ����CStaticRollText::StartRolling��ʼ��������;

writer: guoxuxing
date: 2011-5-4
***************************************************************************/
// CStaticRollText
#include <queue>
#include <vector>
using namespace std;

namespace NS_COMMON{

class CStaticRollText : public CStatic
{
	DECLARE_DYNAMIC(CStaticRollText)

public:
	enum eRollStyle
	{
		eRoll_TopToBottom,  // ���ִ������·���
		eRoll_BottomToTop,  // ���ִ������Ϸ���
		eRoll_LeftToRight,  // ���ִ������ҷ���
		eRoll_RightToLeft   // ���ִ������󷭹�
	};

	static const UINT EVENT_ROLL = 1; 
	static const UINT EVENT_TRANSITION = 2;
	static const UINT COLOR_MASK = RGB(255,255,255);
	static const int MARGIN_LEFTRIGHT = 4;
	static const int MARGIN_TOPBOTTOM = 0;

public:
	CStaticRollText();
	virtual ~CStaticRollText();

public:
	void StartRolling(DWORD dwStayTime,
		int nDefaultGroup=0,
		eRollStyle RollStyle=eRoll_BottomToTop);

	void StopRolling();

	void EnableMouseStop(BOOL bEnable);					// ������ƶ����ؼ���ʱ,�Ƿ�������ֹͣ����

	void SetText(CString str,const TCHAR ch=_T('\n')); // �������֣����ַ���Ĭ���á�\n���ָ�

	void AddString(CString str,BOOL bFront=FALSE);		// �����ֶ������������,bFront��������ǰ�壬��������

	void ClearAll();									// �����������

	int GetStringCount();
	
	void SetGroupSize(int nGroupSize) // ����ÿһ��������ʾ���ٸ��ַ���
	{ 
		m_nGroupSize = nGroupSize; 
	}

	void SetBorderColor(COLORREF clr) { m_clrBorder = clr; }

	void SetBorderWidth(const int nWeight);

	void SetFont(CFont* pFont) { m_pFont = pFont; }

	void SetTxtColor(COLORREF clr) { m_clrTxt = clr; }

	
	void SetStayTime(DWORD dwTime) // ����ÿһ�����ֵı���ʱ�� (��Ҫ��StartRolling����֮ǰ����)
	{ 
		m_dwStayTime = dwTime; 
	} 

	void SetEffectTime(DWORD dwTime) // ���ֹ���Ч����ÿ֡���ʱ��
	{ 
		m_dwEffectTime = dwTime; 
	}

protected:
	virtual int GetMarginLeftRight() { return MARGIN_LEFTRIGHT; }
	virtual int GetMarginTopBottom() { return MARGIN_TOPBOTTOM; }

protected:
	virtual void OnDraw(CDC*pDC);
	virtual void DrawBorder(CDC* pDC,LPRECT rclpRect);
	virtual void DrawBackgroud(CDC* pDC,LPRECT lpRect);
	virtual void DrawText(CDC* pDC,LPRECT lpRect,CString strTxt);
	virtual void DrawTransitionEffects(CDC* pDC,LPRECT lpRect,CString strPre,CString strNext,int& nPercentage);

	void UpdateUI();

	void MoveNextShow(); // �ƶ�����һ�����ֵ���ʾ

	CString GetGroupText(vector<CString>& vecStrings);

protected:
	eRollStyle m_eStyle;			// ���ֹ������
	BOOL m_bMouseStop;				// ������ƶ����ؼ���ʱ,�Ƿ�������ֹͣ����

	deque<CString> m_queueTexts;	// ���ֶ���

	int m_nGroupSize;				// ÿһ�����֣��ַ����ĸ���,Ĭ��Ϊ1
	vector<CString> m_vecCurrent;	// ��ǰ�����ֵ��ַ�������

	DWORD m_dwStayTime;				// ÿ�����ֵĶ���ʱ��,ms
	DWORD m_dwEffectTime;           // ���ֹ���ʱ��ÿ֡���ʱ��,ms

	int m_nBorderWidth;				// �߿���
	COLORREF m_clrBorder;			// �߿���ɫ
	COLORREF m_clrTxt;              // ������ɫ

	int m_nTransitionPercentage;	// ���ֹ��Ȱٷֱ�
	BOOL m_bTransitionEffect;		// �Ƿ��ڹ���״̬
	CString m_strPre;				// ����ǰ���ı�

	CFont* m_pFont;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


}// end namespace