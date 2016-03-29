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
	afx_msg void OnBnClickedCheckDynamicreverse();
private:
	// �Ƿ��Զ���ӡ�����
	BOOL m_bPrintShiftReport;
	// �Ƿ�ʹ�ܶ�̬���ý�
	BOOL m_bDynamicReverseMoney;
	// ����ʾ���ఴť
	BOOL m_bNotShowChgDutyBtn;
	// ���౨������
	CBCGPComboBox m_cboShiftReportType;

	// ���������
	int m_nShiftReportType;

	// �Ƿ������Ͻ����
	BOOL m_bIsFetchAmount;
	
	// ����༭����֧����
	BOOL m_bEditPayout;

	BOOL m_bAllowNextDutyMoney;
	
	CBCGPMaskEdit m_editFixReserve;	// �̶����ý��

	INT m_nShiftCount;
};
