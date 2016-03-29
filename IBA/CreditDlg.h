#pragma once

#include "NDDialog.h"
#include "BCGPLabalButton.h"

//
//class:CCreditDlg ��ֵ�˿�Ի���,����Ӧ����������ʱ����
//
class CCreditDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CCreditDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCreditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreditDlg();
	//��̬
	static HWND m_StaticPrehWnd;//�������ձ������ָ�룬��ʶ�����Ƿ��Ѿ���������Ϊ��ģ̬�ģ�

// �Ի�������
	enum { IDD = IDD_ND_CREDITANDDEREG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCancelactive();
	afx_msg void OnBnClickedButtonCreditprint();
	afx_msg void OnBnClickedButtonRefundment();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonCheckout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnRealName(WPARAM wp, LPARAM);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonConsumelist();
	afx_msg void OnBnClickedButtonReprint();
	afx_msg void OnBnClickedButtonBundtime();
	afx_msg void OnEnChangeEditIdcardsno();

	void OnGetBalanceOK(CBalanceMethod & Balance);
	void OnOldCard(UINT nCardId, UINT nMemberId);
	BOOL CheckReturnAction();//�жϻس�����������ִ��Ҫ�󷵻���

	//ִ�г�ֵ����
	BOOL DoCredit();
	BOOL CheckInput();
	void InitControls();
	void ClearData();
	BOOL MakeSureCredit(UINT nMoney);
	BOOL MakeSureDReg();

	void PrintTicket(CString strCreditMoney);
	void PrintCreditBTimeTicket();

	void DeregisterUser(BOOL bAutoRefund=FALSE);
	//ִ�����İ�ʱ����
	BOOL DoBundleTime();
	//ȡ����ʱ
	BOOL DoCancelBundleTime(CString strMemberID);
	//���߰�ʱ
	void DoBundTimeOnLine();

	// ѡ���ʱ
	void SelectBundTime();

	//��ʾ�û���Ϣ
	void ShowUserInfo(CBalanceMethod & Balance);
	void ShowBundleTimeInfo(CBalanceMethod & Balance);
	void ShowControls(CBalanceMethod & Balance);
	//!<�Զ��˿�
	void AutoDoRefundment(CBalanceMethod & Balance); 

public:
	
	void SetForReturn(BOOL newVal = TRUE){ m_bForReturn = newVal; }

	void SetFormCard(BOOL newVal = TRUE){ m_bFromCard = newVal; }

	virtual INT_PTR DoModal();

	// 2011/07/28-8201-gxx: 
	virtual BOOL DoLocalModeQuery();

	// 2011/07/28-8201-gxx: 
	virtual BOOL DoLocalModeCredit(UINT nCreditMoney);
	
private:

	BOOL m_bUserHasCard; //!<�û��Դ���
	BOOL m_bNoPresent;   //!<�Ƿ�����
	BOOL m_bPrint;		//�Ƿ��ӡ

	INT m_nAllMoney; //!<�ϼ����
	//��ǰ�ĳ�ֵ���
	UINT m_nCreditMoney; 
	UINT m_nPresentMoney;

	CString m_strCreditMoney;
	CString m_strPwd;//����Ŀؼ�����
	CString m_strTmpUserPwd;//�������ʱ�û�����
	CString m_strGiveBack;
	CString m_strMemberID;
	CString m_strNetBar;
	CString m_strDeposit;
	CString m_strBalanceW;
	CString m_strBalanceY;
	CString m_strBalanceZ;
	CString m_strResult;
	CString m_strPoint;
	CString m_strOnlineAmount;// ����Ԥ��

	//��ʱ��ر���
	BOOL    m_bAlreadyBundTime;//�Ƿ��Ѿ���ʱ
	UINT	m_nBundleTimeId;//��ʱID����ʼΪ0��Ϊ�ް�ʱ״̬
	UINT	m_nBundleTimeType;//��ʱ��������
	UINT    m_nBundTimeClass;//��ʱ��������

	CString m_strBundleTimeStartTime;
	CString m_strBundleTimeEndTime;
	CString m_strBundleTimeArea;//��ʱ�ص�����
	CString m_strBalanceAfterBundleTime;
	// ��ʱʱ��
	CString m_strBundTimeLimit;
	// ��ʱ���
	CString m_strBundTimeMoney;
	// ��ʱʣ��ʱ��
	CString m_strBTRemainTime;
	
	CBCGPMaskEdit m_edtCreditMoney;
	// �����˺�
	CEdit m_edtNetId;
	
	CBCGPLabelButton m_btnCancelActivation;
	CBCGPLabelButton m_btnConsumeList;

	BOOL m_bFromCard;  //!<��Ϣ�Ƿ�����ˢ��

	BOOL m_bForReturn; //!<Ϊ���˿�

	BOOL m_bHasLocalQuery; // 2011/07/28-8201-gxx: �Ƿ�Ϊ���ز�ѯ�Ľ��

	// 2011/10/21-8208-gxx: 
	CActiveMember m_ActiveMember;

	bool m_bMem;
public:
	afx_msg void OnBnClickedWxCredit();
};
