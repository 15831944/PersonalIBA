#pragma once
#include "IBACaptureImage.h"


// CDlgCapture �Ի���

class CDlgCapture : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCapture)

public:
	CDlgCapture(LPCTSTR lpszImageName, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCapture();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CIBACaptureImage m_capImage;
	CString m_strImageName;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
