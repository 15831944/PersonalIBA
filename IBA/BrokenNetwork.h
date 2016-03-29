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

	static BOOL RemoveLocalMember(UINT MemberId);

	friend class CCashRegisterLoginMethod_atLogin;

public:
	// �������һ��������½ʱ����Ϣ��ʹ�ý��б��ص�¼ʱ������ʹ��
	BOOL SaveLocalStatusInfo(CLocalStatusInfo& Info);

	BOOL LoadLocalStatusInfo();

	// �����ݿ��ȡ����ģʽ״̬
	static CIBAGlobal::emNetworkStatus LoadLocalModeStatus();

	// ע�Ტ����һ��MemberIDֵ
	static UINT RegisterMemberID();

	static void DisableNetwork(); //���뱾��ģʽ
	static void EnableNetwork();  //��������ģʽ
	static void RestoreNetwork(); //����ָ�ģʽ

	BOOL TestingNetwork();

private:
	void TestingResume(); // ���������Ƿ�Ӷ����еõ��ָ�
	void TestingUploadLocalConsume(); // ��ʱ�ϴ������û�������Ϣ
	void TestingTestBroken(); //һ����������ʧ�ܽ���Restoreģʽ��,�������в��������Ƿ���ĶϿ�Ҫ���뱾��ģʽ
	
	static void SettingNetworkStatus(CIBAGlobal::emNetworkStatus status);// ����״̬�������ݿ�(Systemconfig,������ǰ״̬���Ƿ񱾵�ģʽ),����״̬����ʾ

private:
	CCriticalSection m_TestUploadLock;

public:
	NS_COMMON::CThreadTemplate<CBrokenNetwork> *m_threadTestResume; // ���������Ƿ�ָ����߳�
	NS_COMMON::CThreadTemplate<CBrokenNetwork> *m_threadRestore; //�ϱ�����ģʽ�������û�����
	NS_COMMON::CThreadTemplate<CBrokenNetwork> *m_threadTestBroken; //���������Ƿ���ĶϿ�Ҫ���뱾��ģʽ
	BOOL bTestBrokenStarted;
	//BOOL m_bHasFirstTrying;
	CBrokenNetworkResume m_centerUserUpLoad;
};
