#pragma once

#include "RealNameBase.h"

namespace NS_REALNAME
{

class CWDRealName : public CRealNameBase, public CThread
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

	HMODULE m_hIDReader;
	HMODULE m_hIDRSUCore;
	HMODULE m_hWdCheck;

	INT m_nWDState;

private:

	typedef INT (__cdecl *pfnIDReaderInit)(INT, HWND);
	typedef INT (__cdecl *pfnIDReaderUninit)();
	typedef INT (__cdecl *pfnIDReaderSetEventWnd)(HWND, HWND);

	pfnIDReaderInit m_pfnIDReaderInit;
	pfnIDReaderUninit m_pfnIDReaderUninit;
	pfnIDReaderSetEventWnd m_pfnIDReaderSetEventWnd;

	struct ID_CARD_INFO
	{
		char szISN[20]; //����  ��������Ϊ׼��
		char szPSN[20]; //ӡˢ�루��ʱ��ʹ�ã�
	};

	//******************************************************************************

	struct WDUserInfo
	{
		char szName[40];
		char szSex[10];
		char szNation[20];
		char szBirthDate[50];
		char szAddress[200];
		char szID[40];
		char szFromDepart[200];
		char szStartDate[50];
		char szEndDate[50];
		char szNewAddress[200];
	};

	typedef INT (__stdcall *pfnIDRSUCoreInit)();

	pfnIDRSUCoreInit m_pfnIDRSUCoreInit;


	typedef INT (__stdcall *pfnGetInfoFirstType)(WDUserInfo* lpUserInfo);

	pfnGetInfoFirstType m_pfnGetInfoFirstType;

	typedef INT (__stdcall *pfnGetInfoSecondType)(WDUserInfo* lpUserInfo);

	pfnGetInfoSecondType m_pfnGetInfoSecondType;

	typedef INT (__stdcall *pfnGetInfoSecondTypeMachine)(WDUserInfo* lpUserInfo);

	pfnGetInfoSecondTypeMachine m_pfnGetInfoSecondTypeMachine;

	typedef INT (__stdcall *pfnGetOtherType)();

	pfnGetOtherType m_pfnGetOtherType;

	//******************************************************************************

	typedef INT (__stdcall *pfnInitCheck)(char* lpWdHostIp, INT TypeId);
	
	pfnInitCheck m_pfnInitCheck;

	typedef void (__stdcall *pfnUnInitCheck)();

	pfnUnInitCheck m_pfnUnInitCheck;

};

}