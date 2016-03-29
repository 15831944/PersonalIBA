#pragma once

/********************************************************************
CG2XThreadTemplate<TClass,TResult>
	ʹ���߳�ģ�壬����ָ�����Ա����Ϊ�̵߳�ִ�к���
wirter: guoxuxing
date: 2011-4-22
*********************************************************************/


namespace G2XUtility{

class CG2XThread;

class CG2XThreadDelegate
{
	friend class CG2XThread;
public:
	CG2XThreadDelegate(){}
	virtual~CG2XThreadDelegate(){}

protected:
	virtual void didInitInstance(CG2XThread* pThread); //{}
	virtual int didExitInstance(CG2XThread* pThread); //{return 0;}
	virtual void didRun(CG2XThread* pThread); //{}
	virtual void didErrorReport(CG2XThread* pThread, const char* lpszError); //{}

	// ����Ҫ�ж��Ƿ���԰�ȫ�˳��߳�ʱ����
	// ͬʱCG2XThreadDelegate����������Ҫ����didCanExitThread
	virtual bool didCanExitThread(CG2XThread* pThread); // { return pThread->m_bRequestSafeExit; }
};

class CG2XThread
{
	friend class CG2XThreadDelegate;
public:
	CG2XThread();
	virtual ~CG2XThread();

public:
	// �����߳�
	// pDelegate: �¼��������
	// bSuspend: true: ��������ͣ�̵߳�ִ��
	//           false: �������߳�����ִ��
	// bMustSafeExit: ָʾ����������ʱ,�Ƿ����ȴ��̰߳�ȫ�˳�
	// nTimeOut: ��ȫ�˳�ʱ�ĵȴ�ʱ��
	bool Create(CG2XThreadDelegate* pDelegate, bool bSuspend=true, bool bMustSafeExit=false, int nTimeOut=-1);

	// �ָ�����
	bool Start();

	// ��ͣ�̵߳�����
	bool Pause();

	// ǿ��ֹͣ�߳�
	bool Stop(int nExitCode=0);

	// ��ȫ�˳�, �ú�����һֱ�ȴ��̰߳�ȫ�����Ż᷵��
	// Run��������CG2XThreadDelegate::didRun�����б����е���CanSafeExit�Ĵ��룬
	// ������CanSafeExit����trueʱ���˳�run������
	// ����SafeStop��һֱ�ȴ�����ʱ��
	bool SafeStop();

	bool CanSafeExit();

	// ֪ͨ�߳���Ҫ��ȫ�˳���
	void SetRequestSafeExit();

protected:
	virtual void InitInstance();
	virtual unsigned ExitInstance();

protected:
	virtual void Run();
	virtual void ErrorReport(const char* lpszError);

private:
	static unsigned long ThreadEntry(void* pParam);

protected:
	void* m_hThread;
	void* m_hEventSafeStop;
	bool m_bMustSafeExit;
	int m_nSafeTimeOut;
	bool m_bActive;
	CG2XThreadDelegate* m_pDelegate;
	bool m_bRequestSafeExit; // �Ƿ�������ȫ�˳��ı�־
};


template<class TClass,typename TResult=void>
class CG2XThreadTemplate
{
public:
	friend class TClass;
	CG2XThreadTemplate()
	{
		m_pObj = NULL;
		m_callback = 0;
	}
	CG2XThreadTemplate(TClass *pObj,TResult (TClass::*callback_fun)())
	{
		SetCallBack(callback_fun);
	}

	// ���ûص�����������Ϊ���Ա����
	void SetCallBack(TClass *pObj,TResult (TClass::*callback_fun)())
	{
		m_pObj = pObj;
		m_callback = callback_fun;
	}

protected:
	virtual int Run()
	{
		ASSERT(m_pObj);
		ASSERT(m_callback!=NULL);

		(m_pObj->*m_callback)();
		return 0;
	}
	virtual void Run()
	{
		ASSERT(m_pObj);
		ASSERT(m_callback!=NULL);

		(m_pObj->*m_callback)();
	}

private:
	TClass* m_pObj;
	TResult (TClass::*m_callback)();
};

}