#pragma once
#include "IBACaptureImage.h"
#include "afxwin.h"
#include "SShowPic.h"
#include "IBADialog.h"


// CDlgCapture �Ի���

class CDlgCapture : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgCapture)

public:
	CDlgCapture(LPCTSTR szPath, LPCTSTR lpszImageName, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCapture();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CIBACaptureImage m_capImage;
	CString m_strImageName;
	BOOL m_bOK;
	CComboBox m_comboDevices;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//Ҫ��ʾ��Ƭ�ٻָ� CSShowPic m_picShow;
	afx_msg void OnBnClickedBtnChoosecamera();

	void EnableControls(BOOL bEnable = TRUE);
};
