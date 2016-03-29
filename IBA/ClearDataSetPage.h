#pragma once
#include "IBADialog.h"


// CClearDataSetPage �Ի���

class CClearDataSetPage : public CIBAPage 
{
	DECLARE_DYNAMIC(CClearDataSetPage)

public:
	CClearDataSetPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClearDataSetPage();

// �Ի�������
	enum { IDD = IDD_SET_CLEARDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	// ��ʼʱ��
	CBCGPDateTimeCtrl m_StartTime;
	// ����ʱ��
	CBCGPDateTimeCtrl m_EndTime;

	CFont m_Font;
	
	CPPToolTip m_ToolTip;
	
	CRect m_ToolTipRect;
	afx_msg void OnBnClickedClearShortMsg();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void InitTimeCtrl(void);
	void SetToolTipPos(UINT uID);
};
