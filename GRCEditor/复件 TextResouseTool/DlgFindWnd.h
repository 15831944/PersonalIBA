#pragma once


// CDlgFindWnd �Ի���

class CDlgFindWnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgFindWnd)

public:
	CDlgFindWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFindWnd();

// �Ի�������
	enum { IDD = IDD_DLG_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SearchNext();

	BOOL m_bCase;   // ƥ���Сд
	BOOL m_bAll;    // ��ȫƥ��
	CString m_strKeyword;
	CString m_strLastKeyword;


	int m_nLastResult;
};
