#pragma once

#include "IBADialog.h"

// CRealNameDeRegDlg �Ի���

class CRealNameDeRegDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CRealNameDeRegDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRealNameDeRegDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRealNameDeRegDlg();

// �Ի�������
	enum { IDD = IDD_RN_CARDDEREGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
public:
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
private:
	// ��ʱ������
	CString m_strNetId;
	CEdit m_edtNetId;
};
