#pragma once

#include "RealNameBase.h"

namespace NS_REALNAME
{

//class CWDRealName : public CRealNameBase, public CThread
	class CWDRealName : public CRealNameBase
{
public:
	CWDRealName(void);
	virtual ~CWDRealName(void);

	virtual BOOL InitRealName();

	virtual BOOL UnInitRealName();
	
	virtual BOOL CheckRealName();

	virtual BOOL IsScanEnabled();

	virtual BOOL ScanUserInfo(UINT nType, CNetBarUser& NetBarUser, UINT nOption);

	//!<ɨ��֤�����������û��Ƿ�ע�����ʾע����߳�ֵ�˿�Ի���
	virtual BOOL DoScan(UINT nType);

	virtual INT  OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	virtual BOOL UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption = 0);

	//!<�Ƿ�������
	virtual BOOL IsRegisterEnabled();

	//!<�Ƿ�����ͻ��˵�¼
	virtual BOOL IsCheckInEnabled();

	//!<�Ƿ���ҪУ�鿪��ʵ������
	virtual BOOL NeedToCheckRegisterData();

public:
	BOOL m_bNeedUpload;

private:

	void Load();

	void UnLoad();

	void GetWDState();

	BOOL SendToProxy(CByteArray &buffer);

	BOOL SendImage(UINT nType);

	BOOL IsWdWorking();

	virtual int Run();

private:

	//!<�Զ�����ʵ������
	virtual BOOL AutoModifyMemberInfo();

	INT m_nPort;

	/*HMODULE m_hIDReader;
	HMODULE m_hIDRSUCore;
	HMODULE m_hWdCheck;*/
	HMODULE m_hRealname;

	//INT m_nWDState;
public:

	typedef struct WDUserInfo
	{
		char Name[40]; 			// ����
		char Sex[10];			// �Ա�
		char Nation[20];		// ����
		char BirthDate[50];		// ���������գ�1985-08-16��
		char Address[200];		// ��ַ
		char Id[40];			// ֤������
		char Type[4];			// ֤������(��011����ʾ���֤;��099����ʾ����֤��)
		char FromDepart[200];	// ��֤����
		char StartDate[50];		// ��Ч���ޣ���ʼ���ڣ�
		char EndDate[50];		// ��Ч���ޣ���ֹ���ڣ�
	}*pWDUserInfo;

	typedef struct WDParam
	{
		char ConsumeVer[24]; // �ƷѰ汾��							// ���봫
		char ConsumeNo[50];  // �Ʒ�ע�����к�		
		int	 ConsumeCount;    // ����ע���ն���
		int  ConsumeType;    // �Ʒ����� (1ϣ֮�� 2 pubwin  3����)		// ���봫
		char Expiration[12]; // �Ʒ����ʹ�ý�ֹ����  ��ʽ�������գ�20201022��
		char MacAddr[16];    // ��ע��Ű󶨵Ļ���MAC ��ʽ��00E04C47105A
		char HostIp[16];     // ��ע��Ű󶨵Ļ���IP ��ʽ: 202.119.23.54
	}*pWDParam;
	static CNetBarUser s_NetBarUser;
private:

	/*typedef INT (__cdecl *pfnIDReaderInit)(INT, HWND);
	typedef INT (__cdecl *pfnIDReaderUninit)();
	typedef INT (__cdecl *pfnIDReaderSetEventWnd)(HWND, HWND);

	pfnIDReaderInit m_pfnIDReaderInit;
	pfnIDReaderUninit m_pfnIDReaderUninit;
	pfnIDReaderSetEventWnd m_pfnIDReaderSetEventWnd;*/

	//struct ID_CARD_INFO
	//{
	//	char szISN[20]; //����  ��������Ϊ׼��
	//	char szPSN[20]; //ӡˢ�루��ʱ��ʹ�ã�
	//};

	WDParam m_param;
	BOOL m_bInitSuccessed;
	typedef void (__cdecl* PFunOnUserScan)(WDUserInfo *pUserInfo);
	typedef int (__cdecl* FUN_Init)(WDParam *pParam,PFunOnUserScan pfUserScan);
	typedef void (__cdecl* FUN_Stop)();

	FUN_Init m_pfunInit;
	FUN_Stop m_pfunStop;

	//******************************************************************************

	//struct WDUserInfo
	//{
	//	char szName[40];
	//	char szSex[10];
	//	char szNation[20];
	//	char szBirthDate[50];
	//	char szAddress[200];
	//	char szID[40];
	//	char szFromDepart[200];
	//	char szStartDate[50];
	//	char szEndDate[50];
	//	char szNewAddress[200];
	//};

	//typedef INT (__stdcall *pfnIDRSUCoreInit)();

	//pfnIDRSUCoreInit m_pfnIDRSUCoreInit;


	//typedef INT (__stdcall *pfnGetInfoFirstType)(WDUserInfo* lpUserInfo);

	//pfnGetInfoFirstType m_pfnGetInfoFirstType;

	//typedef INT (__stdcall *pfnGetInfoSecondType)(WDUserInfo* lpUserInfo);

	//pfnGetInfoSecondType m_pfnGetInfoSecondType;

	//typedef INT (__stdcall *pfnGetInfoSecondTypeMachine)(WDUserInfo* lpUserInfo);

	//pfnGetInfoSecondTypeMachine m_pfnGetInfoSecondTypeMachine;

	//typedef INT (__stdcall *pfnGetOtherType)();

	//pfnGetOtherType m_pfnGetOtherType;

	////******************************************************************************

	//typedef INT (__stdcall *pfnInitCheck)(char* lpWdHostIp, INT TypeId);
	//
	//pfnInitCheck m_pfnInitCheck;

	//typedef void (__stdcall *pfnUnInitCheck)();

	//pfnUnInitCheck m_pfnUnInitCheck;

};

}