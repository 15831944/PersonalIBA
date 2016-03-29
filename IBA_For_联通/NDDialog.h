#pragma once

#include ".\IBADialog.h"
#include "CurrentNetBarUser.h"

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

	CImageList m_Images;

	BOOL m_bFromScan;
	
	UINT m_nMemberId;
	UINT m_nCardId;

	BOOL m_bIsMember; //!<��Ա
	BOOL m_bOnLine;   //!<�Ƿ�����

	CString m_strNetId;
	CString m_strName;
	CString m_strPersonalID;

	UINT    m_nUserClassID;
	CString m_strUserClass;

	CString m_strAllBalance;
	CString m_strMemberName;

	CString m_strTermId;
	CString m_strOpenNetBar;

	int m_nMoneyOnlineAmount;  //����Ԥ��
	int m_nMoneyBalanceW; //���ͽ��
	int m_nMoneyBalanceY; //���ͽ��
	int m_nMoneyBalanceZ; //����

	UINT m_nCurrentPoint; //��ǰ����

	BOOL m_bAutoQuery;

	UINT m_nDlgTemplateID;

	//
	CActiveMember m_LocalActiveMember;

	// 2011/04/29-gxx: 
	BOOL m_bCenterDisconnect;

	CActiveMember m_ActiveMember;
	BOOL m_bNeedActivation;

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
};