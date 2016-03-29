#pragma once

#include "IBADialog.h"
#include "dal\IBADAL.h"
// CConfigOfOther �Ի���

class CConfigOfOther : public CIBADialog
{
	DECLARE_DYNAMIC(CConfigOfOther)
	DECLARE_MESSAGE_MAP()

public:
	CConfigOfOther();
	virtual ~CConfigOfOther();

// �Ի�������
	enum { IDD = IDD_NETBAROTHERINFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CString m_strConfig1;
	CString m_strConfig2;
	CString m_strConfig3;
	CString m_strDescription;

	CString m_strKeyString;

	CSystemConfigArray m_ConfigArray;
	CBCGPListCtrl m_ListConfig;

public:

	virtual BOOL OnInitDialog();

	void LoadConfig();
	virtual void OnOK();

	afx_msg void OnNMClickListConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
