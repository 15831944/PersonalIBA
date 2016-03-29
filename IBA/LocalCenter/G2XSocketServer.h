#pragma once

// winsock 2 ��ͷ�ļ��Ϳ�
#include <winsock2.h>
#include <MSWSock.h>
#pragma comment(lib,"ws2_32.lib")
#include <afxtempl.h>
#include <string>

// ���������� (1024*8)
// ֮����Ϊʲô����8K��Ҳ��һ�������ϵľ���ֵ
// ���ȷʵ�ͻ��˷�����ÿ�����ݶ��Ƚ��٣���ô�����õ�СһЩ��ʡ�ڴ�
#define MAX_BUFFER_LEN        8192  
// Ĭ�϶˿�
#define DEFAULT_PORT          12345    
// Ĭ��IP��ַ
#define DEFAULT_IP            "127.0.0.1"


//////////////////////////////////////////////////////////////////
// ����ɶ˿���Ͷ�ݵ�I/O����������
typedef enum _OPERATION_TYPE  
{  
	ACCEPT_POSTED,                     // ��־Ͷ�ݵ�Accept����
	SEND_POSTED,                       // ��־Ͷ�ݵ��Ƿ��Ͳ���
	RECV_POSTED,                       // ��־Ͷ�ݵ��ǽ��ղ���
	NULL_POSTED                        // ���ڳ�ʼ����������

}OPERATION_TYPE;

//====================================================================================
//
//				��IO���ݽṹ�嶨��(����ÿһ���ص������Ĳ���)
//
//====================================================================================

typedef struct CG2XIoContext
{
	OVERLAPPED     m_Overlapped;                               // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)              
	SOCKET         m_sockAccept;                               // ������������ʹ�õ�Socket
	WSABUF         m_wsaBuf;                                   // WSA���͵Ļ����������ڸ��ص�������������
	char           m_szBuffer[MAX_BUFFER_LEN];                 // �����WSABUF�������ַ��Ļ�����,����Ϊ8KB
	OPERATION_TYPE m_OpType;                                   // ��ʶ�������������(��Ӧ�����ö��)

	// ��ʼ��
	CG2XIoContext()
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));  
		ZeroMemory( m_szBuffer,MAX_BUFFER_LEN );
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = MAX_BUFFER_LEN;   //WSA��������С��ʼ��Ϊ8KB
		m_OpType     = NULL_POSTED;
	}
	// �ͷŵ�Socket
	~CG2XIoContext()
	{
		if( m_sockAccept!=INVALID_SOCKET )
		{
			closesocket(m_sockAccept);
			m_sockAccept = INVALID_SOCKET;
		}
	}
	// ���û���������
	void ResetBuffer()
	{
		ZeroMemory( m_szBuffer,MAX_BUFFER_LEN );
	}

} PER_IO_CONTEXT;


//====================================================================================
//
//	ÿһ���¿ͻ������ӣ���Ӧһ��CG2XSocketContext����
//
//====================================================================================
class CG2XSocketServer;
typedef class CG2XSocketContext
{  
public:
	CG2XSocketContext();
	virtual ~CG2XSocketContext();

private:
	friend class CG2XSocketServer;
	SOCKET      m_Socket;									// Client Socket
	SOCKADDR_IN m_ClientAddr;								// Client Address
	CArray<PER_IO_CONTEXT*> m_arrayIoContext;				// �ͻ���������������������ݣ�Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����
	DWORD m_dataOrDataPtr;									// �Զ���Ŀͻ�������(һ�����������ʶ�ͻ���)

protected:
	/* 
		Create one new CG2XIoContext, pushing it back to m_arrayIoContext, 
		return object's pointer.
	*/
	CG2XIoContext* GetNewIoContext();

	/*
		Remove CG2XIoContext object from m_arrayIoContext.
		Can't input NULL!
	*/
	void RemoveContext( CG2XIoContext* pContext );

public:

	/* Get IP Address */
	std::string GetAddress() { 
		return std::string(inet_ntoa(m_ClientAddr.sin_addr)); 
	}
	
	unsigned short GetPort() {
		return ntohs(m_ClientAddr.sin_port);
	}

	void SetData(DWORD data){ m_dataOrDataPtr = data; }

	DWORD GetData() { return m_dataOrDataPtr; }

}PER_SOCKET_CONTEXT;


// �������̵߳��̲߳���;
class CG2XServerSocketDelegate
{
	friend class CG2XSocketServer;

public:
	enum ERROR_CODE
	{
		ERROR_NO = 0,
		ERROR_INIT_WINSOCK,			 // ��ʼ��WinSock2.2ʧ��
		ERROR_INIT_STOPEVENT,		 // ������ֹͣ�¼�ʧ��
		ERROR_INIT_IOCP,			 // ��ʼ����ɶ˿�ʧ��
		ERROR_INIT_LISTEN_SOCKET,	 // ��ʼ������Socketʧ��
		ERROR_ATTACH_IOCP,			 // ����Ŀͻ���socket����ɶ˿ڰ�ʧ��
		//ERROR_CLIENT_EXCEPT_EXIT	 // �ͻ����쳣�˳�
		ERROR_SEND_ERROR,			 // �������ݳ���
	};

protected:
	// ����ͻ��˵������¼�,�����տͻ��˷��͵ĵ�һ������ �������false,��ô����ֹ�ͻ��˵Ľ���
	virtual bool didClientConnected(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, const char* pData, const int nBufLen) { return true; }

	// ����ͻ��˵��쳣�˳��¼�
	virtual void didClientExceptExit(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient) {}

	// ���������Ϣ
	virtual bool didHandleError(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, ERROR_CODE code, DWORD dwLastErrorCode) { return false; }

	// �����������
	virtual void didRecvData(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, const char* pData, const int nBufLen) {};

	// �ͻ��˶Ͽ�����
	virtual void didClientDisconnected(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient) {}

	// ���ݷ������
	virtual void didSendCompleted(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, const char* pHasSendedData, const int nLen) {}

	virtual void didDumpText(const char* pText, bool isErrorDump=false) {}
};


//====================================================================================
//
//				CG2XSocketServer�ඨ��
//
//====================================================================================


typedef struct _tagThreadParams_WORKER
{
	CG2XSocketServer* pIOCPModel;                             // (SocketServer)��ָ�룬���ڵ������еĺ���
	int         nThreadNo;                                    // �̱߳��
} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 

// CG2XSocketServer��
class CG2XSocketServer
{
public:
	CG2XSocketServer(CG2XServerSocketDelegate* pDelegate);
	~CG2XSocketServer(void);

public:

	/**
	   ����������
      nPort: �����Ķ˿�
	  nThreadCount: �������̵߳ĸ���, if nThreadCount <= 0 then thread count = number of cpu cores * 2
	  nAccecpExCount: ͬʱͶ�ݵ�AccecptEx�������
	*/
	bool Start(
		const short int nPort, 
		int nThreadCount = 0, 
		int nAccecptExCount = 10);

	//	ֹͣ������
	void Stop();

	// ����Socket��
	bool Initialize();

	// ж��Socket�⣬��������
	void UnloadSocketLib() { WSACleanup(); }

	// ��ñ�����IP��ַ
	char* GetLocalIP();

	// ���ü����˿�
	void SetPort( const int& nPort ) { m_nPort=nPort; }

	int GetPort() { return m_nPort; }

	/* send data to client */
	int SendData(PER_SOCKET_CONTEXT* pClient, const void* pData, const int nDataLen);

protected:

	/* Initialize IOCP */
	bool _InitializeIOCP();

	/* Initialize Listenning Socket */
	bool _InitializeListenSocket();

	void _UnInitialize();

	// Ͷ��Accept����
	bool _PostAccept( PER_IO_CONTEXT* pAcceptIoContext ); 

	// Ͷ�ݽ�����������
	bool _PostRecv( PER_IO_CONTEXT* pIoContext );

	bool _PostSend( PER_IO_CONTEXT* pIoContext );

	// ����ͻ����������
	bool _DoAccpet( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext );

	// ���н��յ����ݵ����ʱ�򣬽��д���
	bool _DoRecv( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext );

	bool _DoSend( PER_SOCKET_CONTEXT* pSocketContect, PER_IO_CONTEXT* pIoContext );

	// ���ͻ��˵������Ϣ�洢��������
	void _AddToContextList( PER_SOCKET_CONTEXT *pSocketContext );

	// ���ͻ��˵���Ϣ���������Ƴ�
	void _RemoveContext( PER_SOCKET_CONTEXT *pSocketContext );

	// ��տͻ�����Ϣ
	void _ClearContextList();

	// ������󶨵���ɶ˿���
	bool _AssociateWithIOCP( PER_SOCKET_CONTEXT *pContext);

	// ������ɶ˿��ϵĴ���
	bool HandleError( PER_SOCKET_CONTEXT *pContext,const DWORD& dwErr );

	// �̺߳�����ΪIOCP�������Ĺ������߳�
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	// ��ñ����Ĵ���������
	int _GetNoOfProcessors();

	// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ�
	bool _IsSocketAlive(SOCKET s);


private:

	HANDLE                       m_hShutdownEvent;              // ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�

	HANDLE                       m_hIOCompletionPort;           // ��ɶ˿ڵľ��

	HANDLE*                      m_phWorkerThreads;             // �������̵߳ľ��ָ��

	int		                     m_nThreads;                    // ���ɵ��߳�����

	char						 m_strIP[24];                   // �������˵�IP��ַ
	int                          m_nPort;                       // �������˵ļ����˿�

	CDialog*                     m_pMain;                       // ������Ľ���ָ�룬����������������ʾ��Ϣ

	CRITICAL_SECTION             m_csContextList;               // ����Worker�߳�ͬ���Ļ�����

	CRITICAL_SECTION			 m_csDelegate;					// �¼�ί�ж�����õĻ�����

	CArray<PER_SOCKET_CONTEXT*>  m_arrayClientContext;          // �ͻ���Socket��Context��Ϣ        

	PER_SOCKET_CONTEXT*          m_pListenContext;              // ���ڼ�����Socket��Context��Ϣ

	LPFN_ACCEPTEX                m_lpfnAcceptEx;                // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����

	LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs; 

	CG2XServerSocketDelegate* m_pEventDelegate;
	
	int m_nAcceptExCount;
	

};

