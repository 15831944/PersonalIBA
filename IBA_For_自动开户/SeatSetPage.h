#pragma once

//#include 
// CSetPageOfSeat �Ի���

class CSetPageOfSeat : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfSeat)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfSeat();
	virtual ~CSetPageOfSeat();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEATSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedButtonBrowser();
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	CString m_strBitmapPath;

};
