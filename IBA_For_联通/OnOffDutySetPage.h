#pragma once

// CSetPageOfDuty �Ի���

class CSetPageOfDuty : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfDuty)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfDuty();   // ��׼���캯��
	virtual ~CSetPageOfDuty();

// �Ի�������
	enum { IDD = IDD_SET_ONOFFDUTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboShiftcount();

private:
	// �Ƿ��Զ���ӡ�����
	BOOL m_bPrintShiftReport;
	// �Ƿ�ʹ�ܶ�̬���ý�
	BOOL m_bDynamicReverseMoney;
	// ���౨������
	CBCGPComboBox m_cboShiftReportType;

	// ���������
	int m_nShiftReportType;

	// �Ƿ������Ͻ����
	BOOL m_bIsFetchAmount;
	
	// ����༭����֧����
	BOOL m_bEditPayout;

	BOOL m_bAllowNextDutyMoney;
	
	INT m_nShiftCount;
};
