#pragma once
#include "resource.h"


// CDlgCheckPwd �Ի���

class CDlgCheckPwd : public CG2XDialog
{
	DECLARE_DYNAMIC(CDlgCheckPwd)

public:
	CDlgCheckPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCheckPwd();

// �Ի�������
	enum { IDD = IDD_DLG_CEHCKPWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
