#pragma once
#include "PayVerifyDlg.h"


// CContainDlg �Ի���

class CContainDlg : public CG2XDialog
{
	DECLARE_DYNAMIC(CContainDlg)

public:
	CContainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CContainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONTAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CPayVerifyDlg m_dlg;
	virtual BOOL OnInitDialog();
};
