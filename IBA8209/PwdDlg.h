#pragma once

#include "IBADialog.h"

//
//class:CDlgPwd ����ʱ��������Ի���
//
class CDlgPwd : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgPwd)
	DECLARE_MESSAGE_MAP()

public:
	CDlgPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPwd();

// �Ի�������
	enum { IDD = IDD_ND_REGISTER_PASSWORD };

protected:

	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnEnChangeEditPwd();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void OnCancel();

private:
	CEdit m_edtPwd;
	CString m_strPwd;

public :

	CString GetPwd() const { return m_strPwd; }
};
