#pragma once

#include "IBADialog.h"

//
//class:CSetPageOfCredit ����̨����->��ֵ�˿����öԻ���
//
class CSetPageOfCredit : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfCredit)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfCredit();   // ��׼���캯��
	virtual ~CSetPageOfCredit();

// �Ի�������
	enum { IDD = IDD_SET_CREDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedCheckRefundment();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

private:
	// ʹ���Զ��˿�
	BOOL m_bAutoRefund;
	// �Զ��˿�ʱ��
	UINT m_nAutoRefundTime;
	// �س�����
	CBCGPComboBox m_cboReturnAction;
	// �Ƿ񵯳��Ի���
	BOOL m_bPopMsgBox;

	UINT m_nMaxCreditMoney;

};
