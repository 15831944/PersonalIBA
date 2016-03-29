#pragma once
#include "Servlet\ShiftTotalMethod.h"
#include "IBADialog.h"


// CDlgDutyLocal �Ի���

class CDlgDutyLocal : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgDutyLocal)

public:
	CDlgDutyLocal(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDutyLocal();

// �Ի�������
	enum { IDD = IDD_DUTY_LOCAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strCardMoney;       // ��ֵ�����
	CString m_strAccountShopping; // �˻�����
	CString m_strZSBConsume;      // ���ձ�����
	CString m_strZSBBalance;      // ���ձ����
	CString m_strCreditMoney;     // ��ֵ���
	CString m_strReturnMoney;     // �˿���
	CString m_strCashShopping;    // �ֽ���
	CString m_strGoodsStock;      // ��Ʒ�ɹ�֧��
	CString m_strOther;           // ����֧��
	CString m_strDeposit;         // Ѻ��
	CString m_strDutyIncome;      // �������
	CString m_strForwardReserve;  // ��һ��Ԥ��
	CString m_strCashTotal;       // �ֽ��ܶ�

	CString m_strOnDutyTime;
	NS_SERVLET::CShiftTotalMethod m_ShiftTotalMethod;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
