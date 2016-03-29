
#pragma once

#include "IBADialog.h"

// ��ʼʱʹ�õ��Զ����ʱ

class CDefineBundleTimeDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CDefineBundleTimeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDefineBundleTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDefineBundleTimeDlg();

// �Ի�������
	enum { IDD  = IDD_ND_DEFINEBUNDTIME };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnEndTimeChanged();
	afx_msg void OnEnChangeEditHour();
	afx_msg void OnEnChangeEditMin();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	struct SelfDefBundleTime
	{
		UINT TimeId;       // ��ʱID
		CString BeginTime; // ��ʼʱ��
		UINT TimePass;     // ��ʱʱ��
		UINT Amount;       // ��ʱ���
		UINT PcClass;      
		UINT AccountType;  

		BOOL bIsSelected;

	}m_BundleTimeInfo;

private:
	// ��ʼʱ��ؼ�
	CBCGPDateTimeCtrl m_btnStartTime;
	CBCGPDateTimeCtrl m_btnEndTime;

	// ��ʼ��ʱ��ؼ�
	BOOL InitTimeCtrl(void);
	// ��ʼ��ʱ��ؼ�������
	BOOL InitTimeDate(void);
	// ��ʼ���ؼ�
	void InitOtherCtrl(void);
	
	CBCGPComboBox m_cboAccountType;
	
	CBCGPMaskEdit m_edtBundTimeMoney;
	// ����
	CBCGPComboBox m_cboArea;
	
	CEdit m_edtHours;
	CEdit m_edtMinutes;
};
