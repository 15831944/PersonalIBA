#pragma once


//
//class:CSetPageOfRegister ����̨����->��������
//
class CSetPageOfRegister : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfRegister)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfRegister();
	virtual ~CSetPageOfRegister();

// �Ի�������
	enum { IDD = IDD_SET_REGISTER };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboPasswordtype();
	afx_msg void OnCbnSelchangeComboPasswordtype2();
	
	CBCGPComboBox m_cboUserClass;
	CBCGPComboBox m_cboIDType;

	CBCGPComboBox m_cboPwdType;
	CString m_strDefPwd;

	CBCGPComboBox m_cboTempPwdType;
	CString m_strTempDefPwd;
	
	// ������������
	CBCGPComboBox m_cboRegisterAge;
	// �������
	CString	  m_strRegisterMoney;
	// �û��Դ���
	BOOL m_bHasCard;

	BOOL m_bCheckIDCardNumber;

};
