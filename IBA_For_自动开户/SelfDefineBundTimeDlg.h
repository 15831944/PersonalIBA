
#pragma once
#include "IBADialog.h"

//
//class:CDlgBundTimeSelfDef �Զ����ʱ�Ի���
//
class CDlgBundTimeSelfDef : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgBundTimeSelfDef)
	DECLARE_MESSAGE_MAP()

public:
	CDlgBundTimeSelfDef(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBundTimeSelfDef();

	void SetMemberId(UINT val) { m_nMemberId = val; }
	
	void SetIsMember(BOOL bIsMember) { m_bIsMember = bIsMember; }
// �Ի�������
	enum { IDD = IDD_ND_BUNDTIME_SELFDEFINE };

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

	UINT m_nMemberId;
	
	CBCGPComboBox m_cboAccountType;
	
	CBCGPMaskEdit m_edtBundTimeMoney;
	// ����
	CBCGPComboBox m_cboArea;
	
	CEdit m_edtHours;
	CEdit m_edtMinutes;

	BOOL m_bIsMember;
};
