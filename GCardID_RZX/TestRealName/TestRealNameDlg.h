// TestRealNameDlg.h : ͷ�ļ�
//

#pragma once


// CTestRealNameDlg �Ի���
class CTestRealNameDlg : public CDialog
{
// ����

	DECLARE_MESSAGE_MAP()

public:
	CTestRealNameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTREALNAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();


	CString m_strIDNumber;
	
	CString m_strName;

	CString m_strPeronId;

	CString m_strPwd;

	CString m_strIDType;

};
