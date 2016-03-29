#pragma once

/**************************************************************************

CNDCreditDlg����ֵ

**************************************************************************/

#include "NDDialog.h"
#include "BCGPLabalButton.h"
#include "BCGPLinkStatic.h"
#include "afxwin.h"
#include "SShowPic.h"
#include "SShowPic2.h"

//
//clsss:CNDCreditDlg ��ֵ�Ի���
//
class CNDCreditDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDCreditDlg)
	DECLARE_MESSAGE_MAP()

public:
	CNDCreditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDCreditDlg();

// �Ի�������
	enum { IDD = IDD_ND_CREDIT };

protected:
	static const UINT TimeIdOfKeepFocus = 100;
	static const UINT TimerIdOfDrawUserInfo = 2000;

	// 2011/07/11-8201-gxx: 
	virtual BOOL DoLocalModeQuery();

	// 2011/07/11-8201-gxx: 
	virtual BOOL DoLocalModeOperate();

	void OnGetBalanceOK(CBalanceMethod & Balance);

	void DrawUserInfo();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonCancelactive();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonReturn();
	afx_msg void OnBnClickedButtonBundletime();
	afx_msg void OnBnClickedButtonConsumelist();
	afx_msg void OnStnClickedStaticAll();
	afx_msg void OnBnClickedButtonConsumelist2();
	afx_msg void OnBnClickedButtonDetail();

	BOOL m_bAutoQuery;

	BOOL m_bNoPresent;   //!<�Ƿ�����

	INT m_nAllMoney; //!<�ϼ����
	
	INT m_nCreditMoney; 
	CString m_strCreditMoney;
	CBCGPMaskEdit m_edtCreditMoney;
	
	CString m_strStartTime;

	CString m_strAllBalance;
	CString m_strDeposit;
	CString m_strTmpUserPwd;//�������ʱ�û�����
	CString m_strUserInfo;

	CBCGPLabelButton m_btnCancelActivation;
	CBCGPLabelButton m_btnConsumeList;
	CBCGPLabelButton m_btnReturn;
	CBCGPLabelButton m_btnBundleTime;
	CBCGPLabelButton m_btnDetail;
	CBCGPLabelButton m_btnZengshoubao;
	CBCGPLinkStatic m_btnBalanceAll;

	BOOL m_bLinkEnabled;

public:

	void SetNetId(CString newVal);
	
	void SetLinkEnabled(BOOL newVal = FALSE) { m_bLinkEnabled = newVal; }
	

	CString m_strBirthday;
	BOOL m_bPrintTicket;

	CSShowPic2 m_staticPhoto_zj;
	CSShowPic m_staticPhoto_zc;

	CString m_strPerID;
	UINT m_nIDType;
	afx_msg void OnBnClickedBtnRnReg();
	void ExeSfreg();

	LRESULT OnCanClose(WPARAM wParam, LPARAM lParam);
};
