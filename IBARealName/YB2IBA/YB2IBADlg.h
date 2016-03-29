// YB2IBADlg.h : ͷ�ļ�
//

#pragma once

#include "ListenSocket.h"
#include "ClientSocket.h"

// CYB2IBADlg �Ի���
class CYB2IBADlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
// ����
public:
	CYB2IBADlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YB2IBA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	virtual BOOL OnInitDialog();
	
	void WriteReg();

	BOOL SendMessageToIBA(UINT nType);

// ���ɵ���Ϣӳ�亯��
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

// ʵ��
protected:

	HICON m_hIcon;

	CListenSocket m_ListenSocket;

	CClientSocket m_ClientSocket;

	CString m_strIdNumber;

	CString m_strPath;

public:
	afx_msg void OnNcDestroy();
};
