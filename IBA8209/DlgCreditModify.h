#pragma once
#include "IBADialog.h"

//
//class: CDlgCreditModify ������ֵ���Ի���
//
class CDlgCreditModify : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgCreditModify)

public:
	CDlgCreditModify(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCreditModify();

// �Ի�������
	enum { IDD = IDD_DLG_CREDIT_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strNewMoney;
	CString m_strCreditMoney;
	CBCGPMaskEdit m_editNewMoney;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
