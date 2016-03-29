#pragma once

#include ".\IBADialog.h"
#include "CurrentNetBarUser.h"
#include "ActiveMember.h"

class CNDDialog : public CIBADialog
{
	DECLARE_MESSAGE_MAP()

protected:
	
	static const UINT TimeIdOfAutoQuery = 1000;
	static const UINT TimerIdOfRePosDialogs = 2;
	void RePosDialogs();

	static HWND hWndPre;
	void InitWindowPos();

protected:

	CNDDialog(UINT nIDTemplate, CWnd *pParent= NULL);
	~CNDDialog();

	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnScan1();
	afx_msg void OnScan2();
	afx_msg void OnRead2();

	void OnBnClickedButtonQuery();
	void OnBnClickedButtonScanreco();

	// 2011/07/08-8201-gxx: ��ȡ�����û�״̬, emUserStateNULL, emUserStateCenter,emUserStateLocal
	int GetUserState();

	// 2011/07/08-8201-gxx: ������ģʽ
	virtual BOOL DoLocalModeQuery();
	virtual BOOL DoLocalModeOperate();


protected:

	struct TFocusEnter
	{
		UINT nFocusID; //�������ڿؼ�ID
		UINT nEnterID; //Ҫģ�ⵥ���¼��Ŀؼ�ID
	};

	CArray<TFocusEnter, TFocusEnter> m_FocusEnterMap;
	
	void AddFocusEnter(UINT nFocusID, UINT nEnterID);

	BOOL IsMeGotFocus();

	void SimulateClickMe();

protected:

	CImageList m_Images;             //netbaruser.bmp�е�ͼ��
	BOOL m_bFromScan;				 //��Ϣ�Ƿ�����ɨ��
	UINT m_nMemberId;
	UINT m_nCardId;
	//BOOL m_bNDHasLocalQuery;	     //�Ƿ���й����ز�ѯ
	BOOL m_bAutoQuery;               //�Ƿ������ɴ���ǰҪ�����Զ���ѯ
	UINT m_nDlgTemplateID;			 //�Ի�����ԴID
	/*BOOL m_bCenterDisconnect;   */     //�Ƿ��������ĶϿ�.CheckCard��������-1ʱ,��ֱ�ӵ���DoLocalModeQuery��OnCenterDisconnectʱ����ΪTRUE

	CActiveMember m_ActiveMember;      //�û� - ����ģʽ��ʹ��
	CActiveMember m_LocalActiveMember; //�û� - ����ģʽ��ʹ��

	BOOL m_bIsMember;                  //��Ա
	BOOL m_bOnLine;                    //�Ƿ�����.=����BalanceMethod��OnlineInFact?��Ҫ�ϱ�ʱ����10������,��������
	CString m_strNetId;                //NetId
	CString m_strMemberName;           //�û���
	CString m_strName;                 //�û���(�Ա�)
	CString m_strPersonalID;           //�û�֤��ID
	UINT    m_nUserClassID;            //�û�����ID
	CString m_strUserClass;            //�û�����
	CString m_strTermId;               //�ն˺�
	CString m_strOpenNetBar;           //��������	
	
	int m_nMoneyAllInAccount;          //�˻����ܽ��(�������,Ѻ��,Ԥ��)
	CString m_strAllBalance;           //�˻����ܽ����ַ�����ʽ
	int m_nMoneyBalanceW;              //���ͳ�ֵ���
	int m_nMoneyBalanceY;              //���ͳ�ֵ���
	int m_nMoneyBalanceZ;              //�����˻����
	UINT m_nCurrentPoint;              //��ǰ����
	int m_nMoneyOnlineAmount;          //Ԥ�۽��
	int m_nMoneyGuaranty;              //Ѻ�� - ֻ����ʱ�û���

protected:

	void CheckCard(LPCTSTR strNetId);
	virtual void OnOldCard(UINT nCardId, UINT nMemberId);
	virtual void OnNewCard(UINT nCardId);

	BOOL QueryBalance(UINT nMemberId, UINT nCardId);
	BOOL ReQuery();

	virtual void OnGetBalanceOK(CBalanceMethod & Balance);
	virtual void OnGetBalanceFailed(CBalanceMethod & Balance);

	virtual void ClearData();

	//2011-4-14-gxx-������ĶϿ��Ĵ���
	virtual void OnCenterDisconnect();
	
	void ScanReco(UINT nType);

public:
	void SetNetId(CString newVal=_T(""), BOOL bAutoQuery = FALSE);  //��������ID�����Զ���ѯ��Ϊ�ձ�ʾ��ǰ�������û���ID
	void SetLocalActiveMember(CActiveMember& activeMember);
};