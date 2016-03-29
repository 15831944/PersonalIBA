#pragma once

#include "IBADialog.h"
#include "dal\IBADAL.h"
#include "ColorGridCtrl.h"
#include "Servlet\ShiftTotalMethod.h"

//
//class:CDlgIBADuty ���°�Ի���
//
class CDlgIBADuty : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgIBADuty)
	DECLARE_MESSAGE_MAP()

public:
	CDlgIBADuty(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgIBADuty();

// �Ի�������
	enum { IDD = IDD_DUTY };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonStat();
	afx_msg void OnEnChangeEditPayout();
	afx_msg void OnEnChangeEditNextreserve();
	afx_msg void OnEnChangeEditAll();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonJxcprint();
	afx_msg void OnEnChangeEditNextreverse();
	afx_msg void OnBnClickedButtonStorecheck();

private:

	void SetStaticFont();
	void InitContrls();
	void HideShiftTotalInfo();
	void GetEachMoney(INT& nPayout, INT& nRemainAmount, INT& nFetchAmount);
	void PrintTicket(CShiftCash &ShiftCash);
	void ShowShiftTotalInfo();
	UINT GetShiftType();
	void DoOnDuty();
	void DoOffDuty();
	INT  Round(double dsrc);
	CString GetShiftName(UINT nShiftType);

	//2011-03-16-gxx-����°�����Ա�Ĳ�����־
	void DeleteOffDutyOperationFile();

private:

	INT m_nAllMoney;//�ֽ��ܶ�
	INT m_nNextReserve;//��һ�ౣ�����

	NS_SERVLET::CShiftTotalMethod m_ShiftTotalMethod;

	CString m_strPayout;//����֧�����
	CString m_strFetchAmount;//ȡ�õ��ֽ��ܶ�
	CString m_strDescription;//��ע����

	CEdit m_edtPayout; //����ΪCEdit������ӦCHANGE
	// ��һ��Ԥ����
	CEdit m_edtNextReverse;
	// ������һ��Ľ��
	CString m_strNextReverse;

	CString m_strOnDutyTime;

	INT m_nDutyIncome; // �������
	INT m_nLocalCreditMoney;

private:

	CBCGPMaskEdit m_edtReverveAmount;//����α��ý��
	CBCGPComboBox m_cboShift;//����Աת��

private:

	CIBAGrid m_wndGridTree;//����ؼ�
	void InitGridTree();
	void FillGird();
};
