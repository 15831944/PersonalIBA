#pragma once
#include "IBADialog.h"


// class:CDlgInputMobile �����ֻ�����ĶԻ���

class CDlgInputMobile : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgInputMobile)

public:
	CDlgInputMobile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInputMobile();

// �Ի�������
	enum { IDD = IDD_DLG_INPUT_MOBILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	CBCGPEdit m_edit;
	CString m_strPhoneNumber;
	virtual BOOL OnInitDialog();
};
