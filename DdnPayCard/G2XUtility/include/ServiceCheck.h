#pragma once

//--------------------------------------
//	��������(�Ƿ�װ��������)
//--------------------------------------
class G2X_CLASSES CServiceCheck  
{
public:
	CServiceCheck(const char* lpszServiceName);
	~CServiceCheck();
public:
	bool HasInstalled();
	bool Start();
	bool Stop();
	bool Restart();
	bool WaitForRunning(unsigned long dwMilliseconds = 10000);
	bool WaitForStopped(unsigned long dwMilliseconds = 10000);
	bool IsRunning();
	bool IsStopped();
protected:
	void* m_hSCManager;
	void* m_hService;
	char m_szServiceName[200];
};

