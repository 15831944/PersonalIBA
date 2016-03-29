#pragma once
/**************************************************************************

CIBARealName��

 1.ά��������ʵ������
 2.ʵ������Ĵ���������

**************************************************************************/


#include "RealNameBase.h"



class CIBARealName
{
public:
	CIBARealName(void);

	~CIBARealName(void);

	BOOL InitRealName();

	BOOL UnInitRealName();

	BOOL CheckRealName();

	INT  OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	INT  OnExecuteMsg(UINT nExecuteType, LPVOID lpData = NULL, DWORD dwLen = 0);

	void OnCardIn();

	INT  InvokeRealName(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	BOOL IsScanEnabled();

	BOOL ScanUserInfo(UINT nType, CNetBarUser& NetBarUser, UINT nOption = 0);

	BOOL DoScan(UINT nType);

	BOOL UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption = 0);

	BOOL ActivationMember(const CNetBarUser& netBarUser);

	BOOL IsRegisterEnabled();

	BOOL IsCheckInEnabled();

	BOOL IsMenuEnabled();

	BOOL NeedToCheckRegisterData();

	BOOL IsRealNameExtendMenuEnabled();

	//!<ˢ�����Ƿ�������
	BOOL RobFocusAfterSwipingCard();

	//!<����Ա��¼
	void OnCashierCheckIn();

	// 2011/10/27-8210-gxx: ֪ͨʵ���û��»�
	void AuditUserCheckOut(CString strTermID,CString strIP,UINT nMemberID);

	// 2014-10-10 - qsc �»� ����
	void CheckOut(const NS_REALNAME::CheckoutInfo* pCheckOutInfo);
public:

	BOOL CreateRealName();
	void DestoryRealName();

	NS_REALNAME::CRealNameBase* m_lpRealName;

};

