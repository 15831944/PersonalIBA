#pragma once
/**************************************************************************
class: CBrokenNetworkResume ������ָ�����
date: 2011-3-28-gxx-add

**************************************************************************/

#include "Dal\IDTypeInfo.h"

class CBrokenNetworkResume : protected CThread
{
public:
	CBrokenNetworkResume(void);
	~CBrokenNetworkResume(void);

	BOOL Init();

	// ��ȡ�Ƿ�Ӧ�ûָ�
	BOOL GetShouldResume();
	void SetShouldResume(BOOL bResume = TRUE);
	
	// ��ʼ�ָ�
	void StartResume(INT nConnectionState);

public:

	virtual INT Run();

	BOOL CheckShouldResume();

	//ִ�еǳ������������ĵģ�
	BOOL DeregisterUser(NS_DAL::CCheckOutinfo& CheckOutinfo);

protected:
	CCriticalSection m_TestUploadLock;
	BOOL m_bShouldResume;
};
