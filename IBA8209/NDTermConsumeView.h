#pragma once

#include "IBAFormView.h"

// CNDTermConsumeView ������ͼ

class CNDTermConsumeView : public CIBAFormView
{
	DECLARE_DYNCREATE(CNDTermConsumeView)
	DECLARE_MESSAGE_MAP()

protected:

	CNDTermConsumeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CNDTermConsumeView();

public:
	enum { IDD = IDV_TERMCONSUME };

private:

	CBCGPListCtrl m_ConsumeList;
	CBCGPComboBox m_cboTermID;
	CBCGPDateTimeCtrl m_StartTime;
	CBCGPDateTimeCtrl m_EndTime;

	CString m_strResult;

private:

	void InitListView();
	void InitData();
	void InitTimeControl();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);

public:

	virtual void OnInitialUpdate();
	void QueryTermId(LPCTSTR lpszTermId);
	
};


