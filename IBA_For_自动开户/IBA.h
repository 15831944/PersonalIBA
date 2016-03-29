
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ibalog.h"
#include "netbarconfig.h"
#include "IBAConfig.h"
#include "IBAView.h"
#include "OperationInfo.h"
#include "ActiveMemberView.h"

class CCashier;
class CIBARealName;
class CDialogManager;

class CIBAApp : public CWinApp,
				public CBCGPWorkspace
{
public:
	CIBAApp();

	virtual void PreLoadState ();
	virtual BOOL LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd);
	
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CString GetWorkPath() const;

	void UpdateToolbarLastID(const CString&, BOOL bError=FALSE);
	void UpdateIBAStatus(BOOL bTryingConnection=FALSE);

	CString GetLastNetID()const;

public:

	INT IBAMsgBox(LPCTSTR lpszPrompt, UINT nType = MB_OK | MB_ICONINFORMATION, BOOL bFormat=TRUE);
	INT IBAMsgBox(UINT nResID, UINT nType = MB_OK | MB_ICONINFORMATION, BOOL bFormat=TRUE);
	INT IBAMsgBox2(LPCTSTR lpszPrompt, UINT nType = MB_OK | MB_ICONINFORMATION, BOOL bFormat=TRUE);

	CCashier* GetCurCashier();
	CIBARealName* GetRealName();
	CDialogManager* GetDialogManager();

	CIBAView* GetIBAView() const { return m_lpIBAView; }
	void SetIBAView(CIBAView* newVal) { m_lpIBAView = newVal; }
	
	//2011-03-21-gxx 
	CActiveMemberView* GetActiveMemberView();

	void UpdateIBA();
private:

	
	BOOL InitIBAEnv();
	void InitIBA();
	void OnLoginOK();

private :

	CCashier*		m_lpCurCashier;
	CIBAView*		m_lpIBAView;
	CIBARealName*	m_lpRealName;
	CDialogManager* m_lpDialogManager;

	CL2Instance m_IBAInstance;
	HMODULE m_hinstBCGCBRes;
	CString m_strWorkPath;

public:
	// 2011/06/24-gxx: �Զ���������IBA
	void AutoRestart();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


extern CIBAApp theApp;

class CIBAGlobal
{
public:
	static const UINT CLASSID_LOCALUSER = 9999;  // �����û��ļ���ID

	// ʵ�����Ͷ���
	enum emRealnameType
	{
		emRealnameNone = 0,
		emRealnameRZX = 1, // ������
		emRealnameGLW = 2, // ������
		emRealnameMoGen = 3, // Ħ��
		emRealnameSangDi = 4, // ɣ��
		emRealnameWD = 5, // ����
		emRealnameRZX2008 = 6, // ��������
		emRealnameHengXin = 9, // ����
	};

	// ��������
	enum emPasswordType
	{
		emPwdTypeRandom  = 0,   // �������
		emPwdTypeIDTail6 = 1,   // ֤����6λ��
		emPwdTypeFixed   = 2,   // �̶�����
		emPwdTypeInput   = 3,   // �û�����
		emPwdTypeEmpty   = 4    // ������
	};

	// ��������
	enum emOperationType
	{
		emOpTypeNULL = 0,
		emOpTypeCredit = 1,          // ��ֵ
		emOpTypeReturn = 2,          // �˿�
		emOpTypeOpen = 3,            // ����
		emOpTypeChangeMachine = 4,   // ����
		emOpTypeAutoReturn = 5,      // �Զ��˿�
		emOpTypeVipReturn = 6,       // ��Ա����
		emOpTypeBundleTime = 7,      // ��ʱ
		emOpTypeReturnBalance = 8,   // �����
		emOpTypeCreditDelete = 9,    // ��ֵ����
		emOpTypeCreditModify = 10,   // ��ֵ����	

		emOpTypeOpenLocal ,          // ���ؿ���
		emOpTypeLocalReturn,         // �����û��˿�
		emOpTypeLocalCredit,         // ���س�ֵ
		emOpTypeBrokenReturn,        // ������ʱ�û������˿�
		emOpTypeBrokenReturnVip,     // ���Ļ�Ա�û������˿�
		emOpTypeRoomReturn,          // ��������

		emOpTypeCount
	};

	static CString GetOperationName(int nOptType);

	// �����ĵ�����״̬
	enum emNetworkStatus
	{
		emNetworkNormal,             // ����������������, ��������״̬
		emNetworkBroken,             // �����жϣ�����Ӧ��ģʽ״̬
		emNetworkRestore             // �ָ�״̬�� �������жϵ�����������ȫ�����Ĺ��ɹ���
	};

	// �û�״̬����
	enum emUserState
	{   
		emUserStateNULL,             // �û�������
		emUserStateCenter,           // �����û�
		emUserStateLocal             // �����û�
	};

	// Ӧ��ģʽ���÷�ʽ
	enum emLocalModeStartUpKind
	{
		emLocalModeForbid,           // ����
		emLocalModeManual,           // �ֶ�
		emLocalModeAuto              // �Զ�
	};

	// ϵͳ����֮���û���ͼ��������ķ�Ӧ
	enum emOperationAfter
	{
		emOperationAfterNull,        // �س�/���˫���󲻵����κν���
		emOperationAfterCredit,      // ������ֵ����
		emOperationAfterRefund       // �������˽���
	};

	// �Ʒѷ�ʽ
	enum emPayType
	{
		emPayTypeNormal = 0,         // ��ͨ�Ʒ�
		emPayTypeAccumulative = 1,   // �ۼƷⶥ
		emPayTypeWithholding = 2     // ���Ԥ��
	};

	enum emAfterCheckOutFlag
	{
		emAfterCheckOutLock,     // ����
		emAfterCheckOutShutdown, // �ػ�
		emAfterCheckOutRestart   // ����
	};

	// 99
	static const UINT RZX_SPE_DOMAIN1 = 4; // �����У����ŵ���������ʵ����ID

public:
	static emNetworkStatus NetworkStatus;

	static DWORD s_dwStartUpTickcount;

	static HWND m_hCurRegisterDlg;		// ��ǰ�����Ի���ľ��
	static HWND m_hCurCreditDlg;			// ��ֵ�Ի�����
};
