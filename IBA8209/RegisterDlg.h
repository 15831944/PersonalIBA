#pragma once

#include "NetBarUser.h"
#include "Cashier.h"
#include "NDDialog.h"
#include "BCGPLabalButton.h"
#include "DefineBundTimeDlg.h"

class CRegisterDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CRegisterDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterDlg();

// �Ի�������
	enum { IDD = IDD_ND_REGISTER };
	static HWND m_StaticPrehWnd;
	static BOOL m_StaticTmpMember;//�����ϴβ������Ƿ���ʱ�û�

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnSetfocusEditIdcardsno();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonBundletime();
	afx_msg void OnBnClickedButtonReadid2();
	afx_msg void OnScan1();
	afx_msg void OnRead2();
	afx_msg void OnScan2();
	afx_msg void OnScanPassport();
	afx_msg void OnScan2Back();
	afx_msg void OnScanOther();
	afx_msg LRESULT OnRealName(WPARAM wp, LPARAM lp);
	afx_msg void OnBnClickedCheckTempmember();
	afx_msg void OnBnClickedButtonReprint();
	afx_msg void OnBnClickedCheckPrint();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnCbnSelchangeComboBtsel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeEditPersonalid();
	afx_msg void OnCbnSelchangeComboUserclass();
	afx_msg void OnBnClickedRadioTempuser();
	afx_msg void OnBnClickedRadioMember();
	afx_msg void OnCbnSelchangeComboTempuserclass();

	void RegisterLocalUser();  // ע�᱾���û�
	void RegisterCenterUser(); // ע�������û�

public:
	void SetPhoneNumberOpen();


private:

	static const UINT TimerIdOfDisableInput = 1002;

	CFont m_MoneyFont;
	
	CBCGPComboBox m_cboSex;
	CBCGPComboBox m_cboUserClass;
	CBCGPComboBox m_cboTempUserClass;
	CBCGPComboBox m_cboIDType;
	CBCGPComboBox m_cboBundTimeSel;	// ��ʱѡ�������ؼ�
	CBCGPComboBox m_cboComputer;

	CEdit m_edtPersonalID; //��Ҫ��һ���ؼ�����������Ӧchange�¼�
	CBCGPMaskEdit m_edtMoney;
	CBCGPButton m_chkTempMember;
	
	CBCGPButton m_btnTempUserClass;
	CBCGPButton m_btnMemberUserClass;
	CBCGPLabelButton m_btnSefDefine;

	BOOL m_bPrintTicket;
	BOOL m_bUserHasCard;
	BOOL m_bNoPresent;

	//�û�����
	INT m_nUserSex;
	CString m_strUserName;
	CString m_strPersonalID;
	CString m_strCardId;
	CString m_strPassword;
	CString m_strNetId;
	CString m_strNetId2; //���������˺�
	CString m_strNation;
	CString m_strTel;
	CString m_strTelNum;
	CString m_strOfficer;
	BOOL	m_bIsTempUser; //�ǲ�����ʱ�û�

	//
	CDefineBundleTimeDlg m_DefineBundleTimeDlg;

	// 2011/09/07-8201-gxx: 

private:

	CRegisterMethod UserRegister;

	INT	m_nDefClassIndex; //!<Ĭ���û�����

	//������������������ʱ����
	
	BOOL m_bIsMember;		//!<�Ƿ�Ϊ��ʱ�û�
	BOOL m_bFromScan;       //!<��Ϣ�Ƿ�����ɨ��
	BOOL m_bFromCard;		//!<��Ϣ�Ƿ�����ˢ��
	BOOL m_bCanEditUserInfo;//!<�Ƿ���Ա༭�û���Ϣ
	BOOL m_bNetIdFromPersonalId; //!<�����˺�ǿ������֤������

	//���������м����

	UINT m_nCreditMoney;	//!<ʵ�ճ�ֵ���
	UINT m_nCreditPresent;  //!<��ֵ���ͽ��
	UINT m_nUserClassID;	//!<�����û�����
	CString m_strUserClassName; //!<�����û���������
	UINT m_nIdType;			//!<֤������
	UINT m_nCostExpense;	//!<������
	UINT m_nGuarantyMoney;  //!<Ѻ��

	//ע��ɹ������

	BOOL m_bAfterRegister;  //�Ƿ��Ѿ�ע��ɹ���
	UINT m_nMemberId;		//!<ע��ɹ�����û���

	//��ʱ��ر���
	UINT	m_nBundleTimeId;
	UINT	m_nBundleTimeType;
	UINT    m_nBundTimeClass;//��ʱ������ʽ
	UINT	m_nBundTimeMoney;
	CString m_strBundleTimeStartTime;
	CString m_strBundleTimeEndTime;
	CString m_strBundleTimeArea;
	CString m_strBalanceAfterBundleTime;

	CNetBarUser m_userInfo;

	BOOL m_bPhoneNumberOpen;
	BOOL m_bWaitReg;		// �ȴ�ע�����
	BOOL m_bClickRnReg;		// �����ʵ��ע�ᰴť
private:

	//������ʱ�û�����
	void MakeTmpMemberInfo();
	void ScanReco(UINT nType);
	void ClearData();
	void InitData();
	void InitContrls();

	void ResetNetworkBrokenControl();
	void InitIdTypeComboBox();
	void InitUserClassComboBox();
	void InitBundleTimeComboBox();
	void InitComputerComboBox();
	void EnableUserInfoControls(BOOL bEnable);
	void SelectIDType();
	BOOL GetCardIdFromCenter();
	BOOL CheckRealNameData();
	BOOL CheckInput();
	BOOL CheckMoney();
	void MakeUserPwd();
	BOOL IsBundleTime();
	void AutoUploadImage();
	void AutoCheckIn();

	void PrintTicket();
	void PrintTicket2();

	void PrintTicketPos();		// �ϵ�pos����ӡСƱ�ķ�ʽ
	
	void PrintTicketA4();		// ��ӡA4ֽ��ʽ
	
	void GetRegisterInfo();
	void SetRegisterInfo();
	void SaveRegisterInfo();
	void ShowRegisterResult(BOOL bLocalUser=FALSE);
	void ShowRegisterBundleTimeResult();

	// ����Ҫ��
	void OpenRServer();

	void ExeSfreg();

	// 2014-4-25 - qsc
	void SendRegDataToProxy();
public:
	
	void SetNetId(CString newVal) { m_strNetId = newVal; }
	void SetCardId(CString newVal) { m_strCardId = newVal; }
	void SetUserInfo(CNetBarUser& NetBarUser);

	virtual INT_PTR DoModal();

	afx_msg void OnBnClickedBtnCustomBoundletime();
	afx_msg void OnBnClickedBtnInputNumber();
	afx_msg void OnBnClickedBtnCapphtoto();
	afx_msg void OnBnClickedBtnRnReg();
	afx_msg LRESULT OnCanClose(WPARAM wP,LPARAM lP);
	afx_msg void OnBnClickedButtonQueryTel();
	afx_msg void OnBnClickedBtnPhoto();
protected:
	virtual void OnCancel();
};
