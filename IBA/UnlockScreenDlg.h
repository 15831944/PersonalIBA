#pragma once

#include "IBADialog.h"

// CUnlockScreenDlg �Ի���

class CUnlockScreenDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CUnlockScreenDlg)
	DECLARE_MESSAGE_MAP()

public:
	CUnlockScreenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUnlockScreenDlg();

// �Ի�������
	enum { IDD = IDD_UNLOCKSCREENDLG };

	CString GetPwd() const { return m_strPwd; }

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	CString m_strPwd;

};
