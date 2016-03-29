#pragma once

#include "IBADialog.h"
#include "dal\IDTypeInfo.h"
// CReturnDlg �Ի���

class CReturnDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CReturnDlg)
	DECLARE_MESSAGE_MAP()

public:
	CReturnDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReturnDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RETURN };

	void SetCheckOutInfo(CCheckOutinfo newVal) { m_CheckOutInfo = newVal; }

protected:

	void SetStaticFont();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CCheckOutinfo m_CheckOutInfo;

	CString m_strUserName;

	CString m_strNetId;

	CString m_strMoney;

	CString m_strPersonalId;
};
