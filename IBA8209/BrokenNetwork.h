#pragma once
#include "common\ThreadTemplate.h"
#include "Servlet\CashRegisterLoginMethod.h"
#include "IBA.h"
#include "BrokenNetworkResume.h"

// 2011/07/08-8201-gxx: 
// �������ƣ���������������±������ݣ���������¼�������
//
class CBrokenNetwork
{
	CBrokenNetwork(void);

public:

	~CBrokenNetwork(void);

	static CBrokenNetwork* GetInstance();

public:
	// �������һ��������½ʱ����Ϣ��ʹ�ý��б��ص�¼ʱ������ʹ��
	BOOL SaveLocalStatusInfo(CLocalStatusInfo& Info);

	BOOL LoadLocalStatusInfo();

	// �����ݿ��ȡ����ģʽ״̬
	static CIBAGlobal::emNetworkStatus LoadLocalModeStatus();

	// ע�Ტ����һ��MemberIDֵ
	static UINT RegisterMemberID();

	static void DisableNetwork();
	static void EnableNetwork();

	static void SettingNetworkStatus(CIBAGlobal::emNetworkStatus status);

	BOOL TestingNetwork();

private:
	void TestingResume(); // ���������Ƿ�Ӷ����еõ��ָ�
	void TestingUploadLocalConsume(); // ��ʱ�ϴ������û���Ϣ

private:
	CCriticalSection m_TestUploadLock;

public:
	NS_COMMON::CThreadTemplate<CBrokenNetwork> *m_threadTestResume; // ���������Ƿ�ָ����߳�
	NS_COMMON::CThreadTemplate<CBrokenNetwork> *m_threadRestore; 
	BOOL m_bHasFirstTrying;
	CBrokenNetworkResume m_centerUserUpLoad;
};
