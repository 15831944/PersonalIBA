#pragma once


// CCrashShowDlg �Ի���

class CCrashShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CCrashShowDlg)

public:
	CCrashShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCrashShowDlg();

// �Ի�������
	enum { IDD = IDD_CRASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};
