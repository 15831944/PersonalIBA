#pragma once
#include "afxwin.h"
#include ".\common\StaticRollText.h"
#include "bcgptoolbarctrlcommon.h"
#include "..\L2ToolKit\GdipButton.h"

//
//class:CDlgIBAFastlane ����ͨ�����Ի���
//
#define WM_UPDATEROLLTEXT WM_USER+100

class CDlgIBAFastlane : public CStringLoaderDlg<CBCGPDialog>
{
	DECLARE_DYNAMIC(CDlgIBAFastlane)
	DECLARE_MESSAGE_MAP()

public:
	CDlgIBAFastlane(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgIBAFastlane();

// �Ի�������
	enum { IDD = IDD_DLG_IBA_FAST };

	static const UINT EVENT_TIME_BUGLE = 1;   // �������Ƚ���Ķ�ʱ���¼�
	static const UINT TIME_BUGLE = 1*1000;    // 1S�У�������һ��
	static const int COUNT_TURN = 1*60;      // �������(1����)
       

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK(){ return ; }
	virtual void OnCancel(){ return; }
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboFast();
	afx_msg void OnCbnSelendokComboFast();
	afx_msg void OnCbnSelendcancelComboFast();
	afx_msg LRESULT OnUpdateRollText(WPARAM wParam, LPARAM lParam);

	// 2011/08/24-8201-gxx: 0:����ģʽ,1:Ӧ��ģʽ,2:�ָ�ģʽ,3:���ڳ�����������
	afx_msg LRESULT OnUpdateIBAStatus(WPARAM wParam, LPARAM lParam);

public:
	void UpdateBugle(); // ˢ�����ȵ���ʾ
	void StartBugleTurn(BOOL bStartActive=TRUE); // ��ʼ���Ƚ���
	void StopBugleTurn(BOOL bLastActive=FALSE);  // ֹͣ���Ƚ���


private:
	CVCenterStatic m_static1;
	CVCenterStatic m_static2;

	BOOL m_bBugleActive; // �����Ƿ�Ϊ�״̬,(����Ĭ��Ϊ����״̬)
	int m_nTurnCount; 
public:

	CBCGPComboBox m_combFast;
	//CComboBoxEx m_combFast;
	NS_COMMON::CStaticRollText m_stcContent;
	CStatic m_stcPicStatus;
	CImageEx m_imgStatus;
	INT m_nIBAStatus;

	CPPToolTip m_ToolTip;


	
	afx_msg void OnTimer(UINT nIDEvent);
	
	afx_msg void OnStnClickedPicStatus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
