#pragma once

#include "IBADialog.h"
// CNetbarConfigDlg �Ի���

class CNetbarConfigDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CNetbarConfigDlg)

public:
	CNetbarConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetbarConfigDlg();

// �Ի�������
	enum { IDD = IDD_NETBARCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
