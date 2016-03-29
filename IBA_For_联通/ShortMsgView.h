#pragma once

#include "IBAFormView.h"

// CShortMsgView ������ͼ

class CShortMsgView : public CIBAFormView
{
	DECLARE_DYNCREATE(CShortMsgView)
	DECLARE_MESSAGE_MAP()

protected:
	CShortMsgView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CShortMsgView();

public:
	enum { IDD = IDV_SHORTMSG };
	virtual void OnInitialUpdate();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedButtonQuery();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnNMDblclkListMsg(NMHDR *pNMHDR, LRESULT *pResult);

private:
	// ��Ϣ�б�
	CBCGPListCtrl m_MsgList;
	// ��ʼʱ��
	CBCGPDateTimeCtrl m_btnStartTime;
	// ����ʱ�䰴ť�ؼ�
	CBCGPDateTimeCtrl m_btnEndTime;
	
	CString m_strResult;

	void InitListView();
	// ��ʼ��ʱ��ؼ�
	BOOL InitTimeCtrl(void);
	// ��ʼ��ʱ��ؼ�������
	BOOL InitTimeDate(void);
	// �����Ϣ�б�
	INT FillMsgList(void);

public:

	// ��ѯ��Ϣ
	void QueryShortMsgList(void);

};


