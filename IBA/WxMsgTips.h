/************************************************************************/
/* @ desc: ΢�ų�ֵ��ʾ                                                 */
/* @ date: 2015/5/5														*/
/* @ author: tmelody			                                        */
/* @ note:						                                        */
/*		��������ó���㴰�ڣ���ô����̨���ڽ����ڷǼ���״			*/
/*						                                                */
/************************************************************************/

#pragma once

#include "HoverButton.h"

//////////////////////////////////////////////////////////////////////////
// ����ʵ��һ�����ܣ������������

class CSpecStatic : public CStatic
{
	DECLARE_DYNAMIC(CSpecStatic)
	DECLARE_MESSAGE_MAP()

public:
	CSpecStatic();

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

protected:
	HCURSOR m_hCursor;
};


//////////////////////////////////////////////////////////////////////////
// 

class CWxMsgTips : public CWnd
{
	DECLARE_DYNAMIC(CWxMsgTips)
	DECLARE_MESSAGE_MAP()

public:
	CWxMsgTips();
	virtual ~CWxMsgTips();

public:
	BOOL Create();

	void SetUserID(LPCTSTR lpszID);				// �˺�
	void SetPayAmount(LPCTSTR lpszAmount);		// ֧�����
	void SetGiveAmount(LPCTSTR lpszAmount);		// ����
	void SetTermID(LPCTSTR lpszTermID);			// ������
	void SetTime(LPCTSTR lpszTime);				// ʱ��

	void ShowRBCorner();

protected:
	void InitResource();
	void AdjustPosition();
	BOOL IsMouseIn();
	void ConstructorInfoText();

protected:
	// 
	virtual BOOL Create(LPCTSTR lpszClassName, 
		LPCTSTR lpszWindowName, 
		DWORD dwStyle, 
		const RECT& rect, 
		CWnd* pParentWnd, 
		UINT nID, 
		CCreateContext* pContext /* = NULL */);
	virtual void PostNcDestroy();

protected:
	// message
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnCloseClicked();
	afx_msg void OnBtnDetailClicked();

protected:
	static const UINT TimerIDOfPerSec			= 3000;
	static const UINT TimerIDOfPerSecInterval	= 1000;

	static const UINT TextItemCount = 5;
	static const UINT NormalStaticCount = 4;
	static const UINT TextItemSpace = 3;

private:
	BOOL			m_bCreated;		// �Ƿ��Ѿ�����
	CHoverButton	m_BtnClose;		// �رհ�ť
	CStatic			m_StcTitle;		// ����
	CFont			m_FntText;		// ����
	CImageEx		m_ImgBack;		// ����ͼ

	RECT			m_ScreenRect;	// �����С
	CRect			m_WndRect;		// ���ڴ�С����ͼƬ��СΪ׼

	int				m_nShowTimes;	// ��ʾ��ʱ��
	int				m_nCurTimes;	// ��ǰʱ��

	BOOL			m_bMouseTrack;
	BOOL			m_bMouseIn;

	CStatic			m_aTexts[NormalStaticCount];	// ���ֿؼ�
	CFont			m_FntDetailText;				// ��ʾ���������
	CSpecStatic		m_StcDetail;					// �������

	CString			m_strUserID;			// �˺�
	CString			m_strPayAmount;			// ֧�����
	CString			m_strGiveAmount;		// ����
	CString			m_strTermID;			// ������
	CString			m_strTime;				// ʱ��
};