#pragma once
#include "afxcmn.h"
#include "GxxListCtrl.h"


// CDlgAdvance �Ի���

class CDlgAdvance : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdvance)

public:
	CDlgAdvance(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAdvance();

// �Ի�������
	enum { IDD = IDD_DLG_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CGxxListCtrl m_listCtrl;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnAdd();
};
