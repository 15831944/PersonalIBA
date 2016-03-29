#pragma once
#include "afxsock.h"

class CG2XSocketClient;

class CG2XClientSocketDelegate
{
	friend class CG2XSocketClient;
protected:
	// �Ѻͷ���˽�������
	virtual void didServerConnected(CG2XSocketClient* pSender) {}

	virtual void didSend(CG2XSocketClient* pSender) {}

	virtual void didReceiveData(CG2XSocketClient* pSender, void* buf, int bufLen) {}

	// �Ѻͷ����ʧȥ����, �����Ƿ��Զ�����
	virtual bool didServerDisconnected(CG2XSocketClient* pSender) { return true; } 
};

class CG2XSocketClient : public CAsyncSocket
{
public:
	CG2XSocketClient(CG2XClientSocketDelegate* pDelegate=NULL);
	virtual ~CG2XSocketClient(void);

	static const int MAX_BUF_LEN = 8192;

	// ���ӷ�����
	BOOL ConnectServer(LPCTSTR lpServerIp, unsigned int port);

	// �Ͽ��ͷ�����������
	BOOL DisconnectServer();

	BOOL IsConnected();

	BOOL SendData(const void* data, int len);

	// �Ƿ�����ͻ��˶Ͽ����Ӻ��Զ�����
	void EnableAutoConnect(BOOL bEnable);

protected:
	void OnConnect(int nErrorCode);
	void OnSend(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnClose(int nErrorCode);

private:
	// default: FALSE
	BOOL m_bEnableAutoConnect;
	unsigned int m_port;
	CString m_strServerIp;
	CG2XClientSocketDelegate* m_pDelegate;
};


//------------------------------------------------------------------------------------
//
// CG2XSocketClientSYNC: ������ͬ������
// ���������뷵�����ݣ�������Ϊ����Ч������
//
//------------------------------------------------------------------------------------
class CG2XSocketClientSYNC : public CDialog,public CG2XClientSocketDelegate
{
	DECLARE_MESSAGE_MAP()
protected:
	CG2XSocketClientSYNC();
	~CG2XSocketClientSYNC();

	enum
	{
		// Ӧ����һ����Ч�ĶԻ���ģ�壬�˴�������ǹ��ڶԻ����ģ��ID
		IDD = 100
	};
public:
	static CG2XSocketClientSYNC* SendData(LPCTSTR lpServerIp, unsigned int port ,const void* data, int len, unsigned int nTimeoutMs=20000);

	void Close();

protected:
	// �Ѻͷ���˽�������
	virtual void didServerConnected(CG2XSocketClient* pSender);

	virtual void didSend(CG2XSocketClient* pSender);

	virtual void didReceiveData(CG2XSocketClient* pSender, void* buf, int bufLen);

	// �Ѻͷ����ʧȥ����, �����Ƿ��Զ�����
	virtual bool didServerDisconnected(CG2XSocketClient* pSender); 


	BOOL OnInitDialog();
	void OnTimer(UINT_PTR uEvent);

	CG2XSocketClient client;

	void* sendedData;
	int sendLenght;
	CString m_strIp;
	unsigned int m_uPort;
	unsigned int m_nTimeoutMs;

public:
	/* errorCode 0: ���ͳɹ�
	   errorCode 1: �޷�����
	   errorCode 2: ���ӳ�ʱ
	*/
	int errorCode;

	// ��Ӧ���ص�����
	BYTE* responseData;

	// ���ص����ݵĳ���
	int dataLenght;
};
