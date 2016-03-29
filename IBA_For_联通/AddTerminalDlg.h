#pragma once


#include "IBADialog.h"
#include "SeatList.h"
// CDlgAddTerm �Ի���

class CDlgAddTerm : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgAddTerm)
	DECLARE_MESSAGE_MAP()

public:
	CDlgAddTerm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddTerm();


// �Ի�������
	enum { IDD = IDD_SEAT_ADDTERMINAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();

	afx_msg void OnCbnSelchangeComboTerminalid();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();

public:
	virtual BOOL OnInitDialog();

	void SetExistSeatList(CSeatList * newValue) { m_SeatList = newValue; }

	CString GetTerminalID(void) const { return  m_strTerminalID; }
	void SetTerminalID(CString newValue) { m_strTerminalID = newValue; }

	CString GetTerminalIP(void) const { return  m_strTerminalIP; }
	void SetTerminalIP(CString newValue) { m_strTerminalIP = newValue; }

private:
	// �ն�IP
	CBCGPComboBox m_cboTerminalIP;
	// �ն�ID
	CBCGPComboBox m_cboTerminalID;
	// ������ն�ID
	CString m_strTerminalID;
	// ������ն�IP
	CString m_strTerminalIP;
	//����������ָ��
	CSeatList * m_SeatList;

private:
	// ͨ�������������������е��ն���Ϣ
	void LoadComputeInfo(void);


};
