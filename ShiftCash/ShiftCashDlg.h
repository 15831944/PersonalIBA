// ShiftCashDlg.h : ͷ�ļ�
//

#pragma once


// CShiftCashDlg �Ի���
class CShiftCashDlg : public CDialog
{
// ����
public:
	CShiftCashDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SHIFTCASH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnExportExcel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_strFilePath;
	afx_msg void OnBnClickedBtnExportOnkeyCost();
	afx_msg void OnBnClickedBtnExportCardopen();
	afx_msg void OnBnClickedBtnExportCashPay();
};
