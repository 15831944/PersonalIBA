#pragma once

// CSetPageOfMonitor2 �Ի���

class CSetPageOfMonitor2 : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfMonitor2)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfMonitor2();   // ��׼���캯��
	virtual ~CSetPageOfMonitor2();

// �Ի�������
	enum { IDD = IDD_SET_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:

	BOOL m_bOpenAfterEnter;

	//����
	CString m_strLittleMoney;

	// �û�������Ͽ�
	CBCGPComboBox m_cboUserType;
	// �û�������б�ʽ��ɫ��ť
	CBCGPColorButton m_CPUserType;
	// ������ʾ/�����б��
	CCheckListBox m_ShowZoneList;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboUsertype();
	afx_msg void OnBnClickedButtonClusertype();

};
