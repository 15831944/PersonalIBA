// ID2MonitorDlg.h : ͷ�ļ�
//

#pragma once

#include "..\..\Lib\ScanRecoInc.h"
#include "afxwin.h"
#include "capturemanager.h"

// CID2MonitorDlg �Ի���
class CID2MonitorDlg : public CDialog
{
// ����
public:
	CID2MonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ID2MONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnScanWork(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	void ScanInfo();

	afx_msg BOOL CheckIBARunning();

	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

public:
	BOOL m_bDeviceOpen;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedBtnScan();

	HWND m_hIBAWnd;
	CComboBox m_combTypes;

	CString m_strIniFile;

	CFont m_boldFont;

	INT m_nScanType;

	BOOL m_bScanFinish;

	UINT m_nTimerGap;
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnReadOther();
	afx_msg void OnClose();

	CCaptureManager m_captureManager;

public:
	void SetScanFinish();
	CComboBox m_combSystem;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedBtnAdvance();
};
