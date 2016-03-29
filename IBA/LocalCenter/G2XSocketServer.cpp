#include "StdAfx.h"
#include "G2XSocketServer.h"

// ÿһ���������ϲ������ٸ��߳�(Ϊ������޶ȵ��������������ܣ���������ĵ�)
#define WORKER_THREADS_PER_PROCESSOR 2
// ͬʱͶ�ݵ�Accept���������(���Ҫ����ʵ�ʵ�����������)
#define MAX_POST_ACCEPT              10
// ���ݸ�Worker�̵߳��˳��ź�
#define EXIT_CODE                    NULL


// �ͷ�ָ��;����Դ�ĺ�

// �ͷ�ָ���
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// �ͷž����
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// �ͷ�Socket��
#define RELEASE_SOCKET(x)               {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}

#define IS_INVALID_HANDEL(x) (x == NULL || x == INVALID_HANDLE_VALUE)


//-----------------------------------------------------------------------//
// CG2XSocketContext
//-----------------------------------------------------------------------//

CG2XSocketContext::CG2XSocketContext()
{
	m_Socket = INVALID_SOCKET;
	memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
}

CG2XSocketContext::~CG2XSocketContext()
{
	if( m_Socket!=INVALID_SOCKET )
	{
		closesocket( m_Socket );
		m_Socket = INVALID_SOCKET;
	}
	// �ͷŵ����е�IO����������
	for( int i=0;i<m_arrayIoContext.GetCount();i++ )
	{
		delete m_arrayIoContext.GetAt(i);
	}
	m_arrayIoContext.RemoveAll();
}
//��������ʼ������ĳ��IO�����ӵ���Ϣ(socket,overlapped...)������ӵ�m_arrayIoContext
CG2XIoContext* CG2XSocketContext::GetNewIoContext()
{
	CG2XIoContext* p = new CG2XIoContext;

	m_arrayIoContext.Add( p );

	return p;
}

void CG2XSocketContext::RemoveContext(CG2XIoContext* pContext)
{
	ASSERT( pContext!=NULL );

	for( int i=0;i<m_arrayIoContext.GetCount();i++ )
	{
		if( pContext==m_arrayIoContext.GetAt(i) )
		{
			delete pContext;
			pContext = NULL;
			m_arrayIoContext.RemoveAt(i);				
			break;
		}
	}
}

//-----------------------------------------------------------------------//
// CG2XSocketServer
//-----------------------------------------------------------------------//

CG2XSocketServer::CG2XSocketServer(CG2XServerSocketDelegate* pDelegate):
							m_nThreads(0),
							m_nAcceptExCount(MAX_POST_ACCEPT),
							m_hShutdownEvent(NULL),
							m_hIOCompletionPort(NULL),
							m_phWorkerThreads(NULL),
							m_nPort(0),
							m_pMain(NULL),
							m_lpfnAcceptEx( NULL ),
							m_pListenContext( NULL )
{
	if (!pDelegate)
	{
		::MessageBoxA(NULL, "CG2XSocketServer ��ί�ж����ָ�벻��Ϊ��", "ERROR", MB_OK);
		ASSERT(0);
	}
	
	m_pEventDelegate = pDelegate;
}


CG2XSocketServer::~CG2XSocketServer()
{
	// ȷ����Դ�����ͷ�
	this->Stop();
}

///////////////////////////////////////////////////////////////////
// �������̣߳�  ΪIOCP�������Ĺ������߳�
//         Ҳ����ÿ����ɶ˿��ϳ�����������ݰ����ͽ�֮ȡ�������д�����߳�
///////////////////////////////////////////////////////////////////
/*************************************************
Function:       //_WorkerThread
Description:    //�����̵߳��̺߳���
				//Note:����ʱ��_InitializeListenSocket���Ѿ�Ͷ����20��AcceptEx����,��������̺߳���һ��ʼ��û�пͻ�����������ǰ,�ͻ����GetQueuedCompletionStatus�ȴ�
				//�пͻ�������ʱ��
				//1.DoAccept���ô������һ�����ݵĴ���,������ʱ������false�ر���������Ͷ��AcceptEx,������ʱ������socket��IOCP����ʼͶ��Recv
				//2.�ڴ�����ʱ,��ѡ��Ͷ��Recv��Send
Calls:          //...
Input:          //lpParam - �̺߳�������(THREADPARAMS_WORKER*)����:1.SocketServer����ָ�� 2.�̱߳��
Return:         //�߳̽�������0
*************************************************/
DWORD WINAPI CG2XSocketServer::_WorkerThread(LPVOID lpParam)
{   
	//ȡ��SocketServerָ����̱߳��
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lpParam;
	CG2XSocketServer* pIOCPModel = (CG2XSocketServer*)pParam->pIOCPModel;
	int nThreadNo = (int)pParam->nThreadNo;

	char strText[256] = {0};
	sprintf(strText, "CG2XSocketServer::_InitializeIOCP -> _WorkerThread - Start worker thread %d.\n", nThreadNo);
	pIOCPModel->m_pEventDelegate->didDumpText(strText);

	OVERLAPPED           *pOverlapped = NULL;
	PER_SOCKET_CONTEXT   *pSocketContext = NULL;
	DWORD                dwBytesTransfered = 0;

	// ѭ����������ֱ�����յ�Shutdown��ϢΪֹ
	// m_hShutdownEvent��ʼΪ���ź�״̬,����ѭ��.CG2XSocketServer::Stop()��SetEvent,����ѭ��,�����߳�,���ͷ��߳��̲߳���
	while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0))
	{
		//��IOCP���л�ȡCP,ûȡ��ǰ�߳�������ᱻ����ס,��������������ʱ
		BOOL bReturn = GetQueuedCompletionStatus(
			pIOCPModel->m_hIOCompletionPort,
			&dwBytesTransfered,            //�������ݳ���
			(PULONG_PTR)&pSocketContext,   //Completion Key,�˿ڴ������������Socket�йص���Ϣ
			&pOverlapped,                  //����Overlapped�ṹ,�ýṹ����AcceptEx��Ͷ�ݹ�����
			INFINITE);                     //������ʱ

		// ����յ������˳���־����ֱ���˳�
		if ( EXIT_CODE==(DWORD)pSocketContext )
		{
			break;
		}

		// �ж��Ƿ�����˴���
		if( !bReturn )  
		{  
			DWORD dwErr = GetLastError();

			// ��ʾһ����ʾ��Ϣ
			if( !pIOCPModel->HandleError( pSocketContext,dwErr ) )
			{
				break;
			}

			continue;  
		}  
		else  
		{  	
			// ��ȡ����Ĳ��� - PER_IO_CONTEXT,��ΪͶ��ʱ��overlapped����PER_IO_CONTEXT��,���Է���ʱ��Ҳ��PER_IO_CONTEXT��,ͨ�����ַ�ʽ���������ṹ
			PER_IO_CONTEXT* pIoContext = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);  //MS�ĺ�,���ݳ�ԱpOverlapped�ĵ�ַ��ȡPER_IO_CONTEXT�ṹ�ĵ�ַ

			// �ж��Ƿ��пͻ��˶Ͽ���
			//Ͷ�ݵ��ǽ�����Ϣ������Ϣ,���ô����Disconnected����,��SocketServer���ͷ�IO��Ϣ
			if((0 == dwBytesTransfered) && ( RECV_POSTED==pIoContext->m_OpType || SEND_POSTED==pIoContext->m_OpType))  
			{  
				pIOCPModel->m_pEventDelegate->didClientDisconnected(pIOCPModel, pSocketContext);

				// �ͷŵ���Ӧ����Դ
				pIOCPModel->_RemoveContext( pSocketContext );

 				continue;  
			}  
			else
			{
				switch( pIoContext->m_OpType )  
				{  
					 // Accept  
				case ACCEPT_POSTED:   //��_InitializeListenSocket��Ͷ�ݵ�20��������ACCEPT����
					{ 
						// Ϊ�����Ӵ���ɶ��ԣ�������ר�ŵ�_DoAccept�������д�����������
						//ȡ��Accept����ĵ�һ�����ݰ���ʹ�ô��������,���������ݷ���ֵ�����Ƿ���Ͷ��Recv����.��ɺ�����Ͷ��Accept����
						pIOCPModel->_DoAccpet( pSocketContext, pIoContext );//����������Socket Context						
					}
					break;

					// RECV
				case RECV_POSTED:
					{
						// Ϊ�����Ӵ���ɶ��ԣ�������ר�ŵ�_DoRecv�������д����������
						pIOCPModel->_DoRecv( pSocketContext,pIoContext ); 
					}
					break;

					// SEND
					// �����Թ���д�ˣ�Ҫ������̫���ˣ���������⣬Send�������������һЩ
				case SEND_POSTED:
					{
						pIOCPModel->_DoSend( pSocketContext,pIoContext );
					}
					break;
				default:
					// ��Ӧ��ִ�е�����
					TRACE(_T("_WorkThread�е� pIoContext->m_OpType �����쳣.\n"));
					break;
				} //switch
			}//if
		}//if

	}//while

	TRACE(_T("�������߳� %d ���˳�.\n"),nThreadNo);

	// �ͷ��̲߳���
	RELEASE(lpParam);	

	return 0;
}



//====================================================================================
//
//				    ϵͳ��ʼ������ֹ
//
//====================================================================================




////////////////////////////////////////////////////////////////////
// ��ʼ��WinSock 2.2
/*************************************************
Function:       //Initialize
Description:    //��ʼ��WinSock2.2��DLL�⣬��������ô���Ĵ�����
Calls:          //m_pEventDelegate->didHandleError
Return:         //���󷵻�false
*************************************************/
bool CG2XSocketServer::Initialize()
{    
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		m_pEventDelegate->didHandleError(this, NULL, CG2XServerSocketDelegate::ERROR_INIT_WINSOCK, GetLastError());
		return false; 
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//	����������
/*************************************************
Function:       //Start
Description:    //������������.��ʼ���ٽ���Դ(���ڹ������߳�ͬ�����¼�ί�ж����),�����¼�(ϵͳ�˳����¼�֪ͨ).
				//������ɶ˿�,�����������߳�(ProcessorNo. * 2)
				//����listen socket,�󶨵���ɶ˿�,�󶨷�������ַ�Ͷ˿�,��ʼ����
				//��ȡAcceptEx��AcceptExSockAddrs����ָ��
				//Ϊ����������׼��,����ȷ����10������,Ϊÿ������׼���ÿͻ���CONTEXT
Calls:          //...
Input:          //nPort        -- SocketServer�Ķ˿�
                //nThreadCount - �������߳�����
				//nAccecpExCount - ÿ���˿���ཨ������������
Return:         //�ɹ�����true,ʧ�ܷ���false
*************************************************/
bool CG2XSocketServer::Start(const short int nPort,int nThreadCount, int nAccecpExCount)
{
	m_pEventDelegate->didDumpText(GetLocalIP());

	if (!nPort)
	{
		m_pEventDelegate->didDumpText("port is invalid! Start SocketServer failed!");
		return false;
	}
	m_nPort = nPort;
	m_nThreads = nThreadCount;
	m_nAcceptExCount = nAccecpExCount;

	InitializeCriticalSection(&m_csContextList);
	InitializeCriticalSection(&m_csDelegate);

	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//�˹���λ,��ʼ���ź�

	if (IS_INVALID_HANDEL(m_hShutdownEvent))
	{
		m_pEventDelegate->didHandleError(this, NULL, CG2XServerSocketDelegate::ERROR_INIT_STOPEVENT, 0);
		return false;
	}
	//1.������ɶ˿� 2.�����������߳�(ProcessorNo. * 2)
	if (false == _InitializeIOCP())
	{
		m_pEventDelegate->didHandleError(this, NULL, CG2XServerSocketDelegate::ERROR_INIT_IOCP, WSAGetLastError());
		return false;
	}
	else
	{
		m_pEventDelegate->didDumpText("Initialize IOCP complete\n");
	}

	//1.����listen socket,�󶨵���ɶ˿�,�󶨷�������ַ�Ͷ˿�,��ʼ����
	//2.��ȡAcceptEx��AcceptExSockAddrs����ָ��
	//3.Ϊ����������׼��,����ȷ����10������,Ϊÿ������׼���ÿͻ���CONTEXT
	if( false==_InitializeListenSocket() )
	{
		m_pEventDelegate->didHandleError(this, NULL, CG2XServerSocketDelegate::ERROR_INIT_LISTEN_SOCKET, GetLastError());
		_UnInitialize();
		return false;
	}
	else
	{
		m_pEventDelegate->didDumpText("CG2XSocketServer::Start - Initialize listen socket complete\n");
	}

	m_pEventDelegate->didDumpText("CG2XSocketServer::Start - Ready! Waiting for a connection\n");

	return true;
}


////////////////////////////////////////////////////////////////////
//	��ʼ����ϵͳ�˳���Ϣ���˳���ɶ˿ں��߳���Դ
void CG2XSocketServer::Stop()
{
	if( m_pListenContext!=NULL && m_pListenContext->m_Socket!=INVALID_SOCKET )
	{
		// ����ر���Ϣ֪ͨ
		SetEvent(m_hShutdownEvent); //����Ϊ���ź�

		for (int i = 0; i < m_nThreads; i++)
		{
			// ֪ͨ���е���ɶ˿ڲ����˳�
			PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
		}

		// �ȴ����еĿͻ�����Դ�˳�
		WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

		// ����ͻ����б���Ϣ
		this->_ClearContextList();

		// �ͷ�������Դ
		this->_UnInitialize();

		m_pEventDelegate->didDumpText("stop listening!");
	}	
}


////////////////////////////////
// ��ʼ����ɶ˿�
/*************************************************
Function:       //_InitializeIOCP
Description:    //����һ����ɶ˿ڣ���������������*2���������߳�
Return:         //��ɶ˿ڴ���ʧ�ܷ���false,��������true
*************************************************/
bool CG2XSocketServer::_InitializeIOCP()
{
	// ������һ����ɶ˿�
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );

	if ( NULL == m_hIOCompletionPort)
	{
		return false;
	}

	// ���ݱ����еĴ�����������������Ӧ���߳���
	if (m_nThreads <= 0)
		m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();
	
	// Ϊ�������̳߳�ʼ�����
	m_phWorkerThreads = new HANDLE[m_nThreads];   
	
	// ���ݼ�����������������������߳�
	DWORD nThreadID;
	for (int i = 0; i < m_nThreads; i++)
	{
		THREADPARAMS_WORKER* pThreadParams = new THREADPARAMS_WORKER;  //���ݸ��߳���������-1.SocketServer����ָ�� 2.�̱߳��
		pThreadParams->pIOCPModel = this;    //��ǰsocketServer��ָ��
		pThreadParams->nThreadNo  = i+1;     //�̱߳��
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID); //�̺߳���Ϊ_WorkerThread,��������������
	}

	char strText[256] = {0};
	sprintf(strText, "Create %d worker threads\n", m_nThreads);
	m_pEventDelegate->didDumpText(strText); //���CLocalCenter::dump:������ַ���

	return true;
}


/////////////////////////////////////////////////////////////////
// ��ʼ��Socket
/*************************************************
Function:       //_InitializeListenSocket
Description:    //1.����listen socket,�󶨵���ɶ˿�,�󶨷�������ַ�Ͷ˿�,��ʼ����
				//2.��ȡAcceptEx��AcceptExSockAddrs����ָ��
				//3.Ϊ����������׼��,����ȷ����10������,Ϊÿ������׼���ÿͻ���CONTEXT
Calls:          //...
Return:         //�ɹ�����true
*************************************************/
bool CG2XSocketServer::_InitializeListenSocket()
{
	// AcceptEx �� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
	GUID GuidAcceptEx = WSAID_ACCEPTEX;  
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS; 

	// ��������ַ��Ϣ�����ڰ�Socket
	struct sockaddr_in ServerAddress;

	// �������ڼ�����Socket����Ϣ
	m_pListenContext = new PER_SOCKET_CONTEXT;

	// ��Ҫʹ���ص�IO�������ʹ��WSASocket������Socket���ſ���֧���ص�IO����
	m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_pListenContext->m_Socket) 
	{
		char strText[256] = {0};
		sprintf(strText, "error: Initialize socket failure, error code:%d\n", WSAGetLastError());
		m_pEventDelegate->didDumpText(strText, true);
		return false;
	}
	
	// ��Listen Socket������ɶ˿���
	if( NULL== CreateIoCompletionPort( (HANDLE)m_pListenContext->m_Socket, m_hIOCompletionPort,(DWORD)m_pListenContext, 0))  
	{  
		char strText[256] = {0};
		sprintf(strText, "error: Attach listen socket to IOCP failure, error code:%d\n", WSAGetLastError());
		m_pEventDelegate->didDumpText(strText, true);
 
		RELEASE_SOCKET( m_pListenContext->m_Socket );
		return false;
	}
	
	// ����ַ��Ϣ
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	// ������԰��κο��õ�IP��ַ�����߰�һ��ָ����IP��ַ 
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                      
	ServerAddress.sin_addr.s_addr = inet_addr(m_strIP);         
	ServerAddress.sin_port = htons(m_nPort);                          

	// �󶨵�ַ
	if (SOCKET_ERROR == bind(m_pListenContext->m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) 
	{
		char strText[256] = {0};
		sprintf(strText, "error: Binding listen socket failure, error code:%d\n", WSAGetLastError());
		m_pEventDelegate->didDumpText(strText, true);
		return false;
	}

	// ��ʼ���м���
	if (SOCKET_ERROR == listen(m_pListenContext->m_Socket,SOMAXCONN))
	{
		m_pEventDelegate->didDumpText("error: Listenning failure\n", true);
		return false;
	}

	//��ȡAcceptEx,AcceptExSockAddrs������ָ��

	// ʹ��AcceptEx��������Ϊ���������WinSock2�淶֮���΢�������ṩ����չ����
	// ������Ҫ�����ȡһ�º�����ָ�룬
	// ��ȡAcceptEx����ָ��
	DWORD dwBytes = 0;  
	if(SOCKET_ERROR == WSAIoctl(
		m_pListenContext->m_Socket, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidAcceptEx, 
		sizeof(GuidAcceptEx), 
		&m_lpfnAcceptEx, 
		sizeof(m_lpfnAcceptEx), 
		&dwBytes, 
		NULL, 
		NULL))  
	{  
		char strText[256] = {0};
		sprintf(strText, "error: SIO_GET_EXTENSION_FUNCTION_POINTER AcceptEx is NULL ,error code:%d\n", WSAGetLastError());
		m_pEventDelegate->didDumpText(strText, true);

		this->_UnInitialize();
		return false;  
	}  

	// ��ȡGetAcceptExSockAddrs����ָ�룬Ҳ��ͬ��
	if(SOCKET_ERROR == WSAIoctl(
		m_pListenContext->m_Socket, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs), 
		&m_lpfnGetAcceptExSockAddrs, 
		sizeof(m_lpfnGetAcceptExSockAddrs),   
		&dwBytes, 
		NULL, 
		NULL))  
	{  
		char strText[256] = {0};
		sprintf(strText, "error: SIO_GET_EXTENSION_FUNCTION_POINTER GetAcceptExSockAddrs is NULL ,error code:%d\n", WSAGetLastError());
		m_pEventDelegate->didDumpText(strText, true); 
		this->_UnInitialize();
		return false; 
	}  


	// ΪAcceptEx ׼��������Ȼ�󴴽�10��socket��connection���ڸ���ɶ˿�
	if (m_nAcceptExCount <= 0)
		m_nAcceptExCount = MAX_POST_ACCEPT;  //�˴�Ϊ10��,������Ҫ�������

	//Ϊÿ�����Ӵ�����Ӧ��PER_IO_CONTEXT��Ȼ�󴴽�Socket�Ͳ�����ɶ˿�Ͷ��һ��10��Accept����
	for( int i=0;i<m_nAcceptExCount;i++ )
	{
		// �½�һ��IO_CONTEXT������ʧ����ɾ����Ϣ
		PER_IO_CONTEXT* pAcceptIoContext = m_pListenContext->GetNewIoContext();//��������ʼ������ĳ��IO�����ӵ���Ϣ(socket,overlapped...)������ӵ�m_arrayIoContext

		if( false==this->_PostAccept( pAcceptIoContext ) )
		{
			m_pListenContext->RemoveContext(pAcceptIoContext);
			return false;
		}
	}

	char strText[256] = {0};
	sprintf(strText, "Post %d AcceptEx Requests\n", m_nAcceptExCount);
	m_pEventDelegate->didDumpText(strText);

	return true;
}

////////////////////////////////////////////////////////////
//	����ͷŵ�������Դ
void CG2XSocketServer::_UnInitialize()
{
	// ɾ��������
	DeleteCriticalSection(&m_csContextList);
	DeleteCriticalSection(&m_csDelegate);

	// �ر�ϵͳ�˳��¼����
	RELEASE_HANDLE(m_hShutdownEvent);

	// �ͷŹ������߳̾��ָ��
	for( int i=0;i<m_nThreads;i++ )
	{
		RELEASE_HANDLE(m_phWorkerThreads[i]);
	}
	
	RELEASE(m_phWorkerThreads);

	// �ر�IOCP���
	RELEASE_HANDLE(m_hIOCompletionPort);

	// �رռ���Socket
	RELEASE(m_pListenContext);

	m_pEventDelegate->didDumpText("Release resource completed!\n");

}

//////////////////////////////////////////////////////////////////
// Ͷ��Accept����
/*************************************************
Function:       //_PostAccept
Description:    //Ͷ��AcceptEx����
Calls:          //WSASocket,AcceptEx
Input:          //pAcceptIoContext - �����ӿͻ��˵�Socket��PER_IO_CONTEXT,���е�socket����������ֵ
Return:         //Ͷ��δ�ɹ���δȡ�÷���,����false
Others:         //AcceptEx = Ͷ�� + ���յ�һ������
*************************************************/
bool CG2XSocketServer::_PostAccept( PER_IO_CONTEXT* pAcceptIoContext )
{
	ASSERT( INVALID_SOCKET!=m_pListenContext->m_Socket );

	// ׼������
	DWORD dwBytes = 0;  
	pAcceptIoContext->m_OpType = ACCEPT_POSTED;            //��־Accept����
	WSABUF *p_wbuf   = &pAcceptIoContext->m_wsaBuf;        //WSABUFΪ��
	OVERLAPPED *p_ol = &pAcceptIoContext->m_Overlapped;    //OverlappedΪ��
	
	// Ϊ�Ժ�������Ŀͻ�����׼����Socket( ������봫ͳaccept�������� ) 
	pAcceptIoContext->m_sockAccept  = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  //������Ҫ��־WSA_FLAG_OVERLAPPED�ṹ
	if( INVALID_SOCKET==pAcceptIoContext->m_sockAccept )  
	{  
		m_pEventDelegate->didDumpText("post accept error! WSASocket return invalid socket", true); 
		return false;  
	} 

	// Ͷ��AcceptEx,����������,��һ���ֵ����ݳ���Ϊ���ܳ���- ������ַ�ĳ���(apiָ��),���ݳ���ָ��Ϊ0
	if(FALSE == m_lpfnAcceptEx( m_pListenContext->m_Socket, pAcceptIoContext->m_sockAccept, p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   
								sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes, p_ol))  
	{  
		if(WSA_IO_PENDING != WSAGetLastError())  
		{  
			m_pEventDelegate->didDumpText("post acceptEx error!", true); 
			return false;  
		}  
	} 

	return true;
}

/*************************************************
Function:       //_DoAccpet
Description:    //ȡ��Accept����ĵ�һ�����ݰ���ʹ�ô��������,���������ݷ���ֵ�����Ƿ���Ͷ��Recv����.(����false��ʾ������,ֱ�ӹر�����.�����ʾ������)
				//������socket����ɶ˿ڹ���.��ɺ�����Ͷ��Accept����
Calls:          //GetAcceptExSockAddrs,m_pEventDelegate->didClientConnected,m_pEventDelegate->didRecvData
Input:          //PER_SOCKET_CONTEXT* pSocketContext - ����Socket��������
                //PER_IO_CONTEXT* pListenContext     - ������Socket��������
Return:         //m_pEventDelegate->didClientConnected����false,������;����,����false.
Others:         //ע��ÿ����������Ҫ�Լ���overlap,���PER_IO_CONTEXT���ܹ���
				//��ɵ�socket�󶨺�ͳ��˳�����
*************************************************/
bool CG2XSocketServer::_DoAccpet( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pListenContext )
{
	SOCKADDR_IN* ClientAddr = NULL;
	SOCKADDR_IN* LocalAddr = NULL;  
	int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);  

	///////////////////////////////////////////////////////////////////////////
	// 1. ����ȡ������ͻ��˵ĵ�ַ��Ϣ
	//////////////////////////////////////////////////////////////////////////
	this->m_lpfnGetAcceptExSockAddrs(pListenContext->m_wsaBuf.buf, 
		pListenContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN)+16)*2),  //���ݲ��ֵĳ���
		sizeof(SOCKADDR_IN)+16, 
		sizeof(SOCKADDR_IN)+16, 
		(LPSOCKADDR*)&LocalAddr, 
		&localLen, 
		(LPSOCKADDR*)&ClientAddr,
		&remoteLen);  

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. ʹ�ô����ദ�����ӳɹ��ͽ��յ�һ������.�����������ӵ�socket��IOCP��.
	//		Note:����������Socket��Context��Ҫ������һ��AcceptExͶ��,��overlap�ṹֻ�ܶ�Ӧ���β���,�������
	//         ����һ���µ�Context���ڵ�������.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	PER_SOCKET_CONTEXT* pNewSocketContext = new PER_SOCKET_CONTEXT;
	pNewSocketContext->m_Socket           = pListenContext->m_sockAccept;  //�������ӵ�socket
	memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));//CONTEXT.m_ClientAddr = �ͻ��˵�ַ
	
	bool bRes = false;
	
	//Ϊ�˱�֤�����̴߳�������ʱ,������Ϊ������������IBA�еĶ��������һЩ����,���ﻹ��ʹ���ٽ���Դ�Ƚϱ���.
	EnterCriticalSection(&m_csDelegate);
		
	//Note:����Ҫ���յڶ������ݵ����  ---   �뷵��false,�ѷ���������� �ͷ�socket,������Ͷ����һ��AcceptEx.
	bRes = m_pEventDelegate->didClientConnected(this, pNewSocketContext, pListenContext->m_wsaBuf.buf, pListenContext->m_Overlapped.InternalHigh); 
	LeaveCriticalSection(&m_csDelegate);

	//��ҪͶ���µ�AcceptEx����ȥվ��
	if (!bRes)
	{
		RELEASE( pNewSocketContext );  //ɾ��socket���������Ϣ���ر�����

		// Ͷ����һ��AcceptEx����
		this->_PostAccept( pListenContext );
		return false;
	}


	// �����ӵ�sokcet����ɶ˿ڰ� -- �ߵ�����˵������Ҫ�ٽ��յڶ���������
	if( false==this->_AssociateWithIOCP( pNewSocketContext ) )
	{
		EnterCriticalSection(&m_csDelegate);
		m_pEventDelegate->didHandleError(this, pNewSocketContext, CG2XServerSocketDelegate::ERROR_ATTACH_IOCP, GetLastError());
		LeaveCriticalSection(&m_csDelegate);

		RELEASE( pNewSocketContext );
		return false;
	}  

	//���ݴ���,�������ݴ���.ò����didClientConnected��Щ��ͻ.�Ѿ���didClientConnected�н��д�����,�������ظ����д���           
	if (pListenContext->m_Overlapped.InternalHigh > 0)   //I/O�˿ڴ��͵����ݳ�����overlapped�ṹ��.
	{
		m_pEventDelegate->didRecvData(this, pNewSocketContext, pListenContext->m_wsaBuf.buf, pListenContext->m_Overlapped.InternalHigh);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. �������������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
	///////////////////////////////////////////////////////////////////////////////////////////////////
	PER_IO_CONTEXT* pNewIoContext = pNewSocketContext->GetNewIoContext();
	pNewIoContext->m_OpType       = RECV_POSTED;
	pNewIoContext->m_sockAccept   = pNewSocketContext->m_Socket;
	// ���Buffer��Ҫ���������Լ�����һ�ݳ���
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	// �����֮�󣬾Ϳ��Կ�ʼ�����Socket��Ͷ�����������
	if( false==this->_PostRecv( pNewIoContext) )
	{
		pNewSocketContext->RemoveContext( pNewIoContext );
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ���Ͷ�ݳɹ�����ô�Ͱ������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ(��Ҫͳһ���������ͷ���Դ)
	this->_AddToContextList( pNewSocketContext );

	////////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ʹ�����֮�󣬰�Listen Socket��IoContext�����ݻ��������ã�Ȼ��׼��Ͷ���µ�AcceptEx
	pListenContext->ResetBuffer();     //ֻ�����û��������ݣ���û�йر�����
	return this->_PostAccept( pListenContext ); 	
}

////////////////////////////////////////////////////////////////////
// Ͷ�ݽ�����������
/*************************************************
Function:       //_PostRecv
Description:    //Ͷ��Recv����
Calls:          //WSARecv
Input:          //PER_IO_CONTEXT* pIoContext - ����Ͷ�������Socket������
Return:         //�ɹ�����true
*************************************************/
bool CG2XSocketServer::_PostRecv( PER_IO_CONTEXT* pIoContext )
{
	// ��ʼ������
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	WSABUF *p_wbuf   = &pIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->m_OpType = RECV_POSTED;

	// ��ʼ����ɺ�Ͷ��WSARecv����
	int nBytesRecv = WSARecv( pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL );

	// �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
	if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
	{
		m_pEventDelegate->didDumpText("post the first WSARecv failure unsuccessfully!", true);
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////
// ���н��յ����ݵ����ʱ�򣬽��д���
/*************************************************
Function:       //_DoRecv
Description:    //����Recv��I/O����
Calls:          //m_pEventDelegate->didRecvData
Input:          //PER_SOCKET_CONTEXT* pSocketContext  -- ����socket������
                //PER_IO_CONTEXT* pIoContext          -- ����socket������
Return:         //�ɹ�����true
*************************************************/
bool CG2XSocketServer::_DoRecv( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext )
{
	// �Ȱ���һ�ε�������ʾ���֣�Ȼ�������״̬��������һ��Recv����

	m_pEventDelegate->didRecvData(this, pSocketContext, pIoContext->m_wsaBuf.buf, pIoContext->m_Overlapped.InternalHigh);

	// Ȼ��ʼͶ����һ��WSARecv����
	return _PostRecv( pIoContext );
}


int CG2XSocketServer::SendData(PER_SOCKET_CONTEXT* pClient, const void* pData, const int nDataLen)
{
	if (nDataLen > MAX_BUFFER_LEN){
		ASSERT(0);
		return 0;
	}

	PER_IO_CONTEXT* pNewIoContext = pClient->GetNewIoContext();
	pNewIoContext->m_OpType       = SEND_POSTED;
	pNewIoContext->m_sockAccept   = pClient->m_Socket;
	memcpy(pNewIoContext->m_szBuffer, pData, nDataLen);
	pNewIoContext->m_Overlapped.InternalHigh = nDataLen;

	//int nRes = send(pClient->m_Socket, pNewIoContext->m_wsaBuf.buf, pNewIoContext->m_Overlapped.InternalHigh, 0);

	// ���Buffer��Ҫ���������Լ�����һ�ݳ���
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	// ��ʼ������
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	WSABUF *p_wbuf   = &pNewIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pNewIoContext->m_Overlapped;
	p_wbuf->len = nDataLen;

	pNewIoContext->m_OpType = SEND_POSTED;

	// Ͷ�ݷ�������
	int nSend = WSASend( pNewIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, dwFlags, p_ol, NULL );

	// �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
	if ((SOCKET_ERROR == nSend) && (WSA_IO_PENDING != WSAGetLastError()))
	{
		m_pEventDelegate->didHandleError(this, pClient, CG2XServerSocketDelegate::ERROR_SEND_ERROR, WSAGetLastError());
		return -1;
	}
	return nSend;
}

/*************************************************
Function:       //_DoSend
Description:    //����Recv��I/O����
Calls:          //m_pEventDelegate->didSendCompleted
Input:          //PER_SOCKET_CONTEXT* pSocketContext  -- ����socket������
				//PER_IO_CONTEXT* pIoContext          -- ����socket������
Return:         //�ɹ�����true
*************************************************/
bool CG2XSocketServer::_DoSend(PER_SOCKET_CONTEXT* pSocketContect, PER_IO_CONTEXT* pIoContext)
{
	//int nRes = send(pSocketContect->m_Socket, pIoContext->m_wsaBuf.buf, pIoContext->m_Overlapped.InternalHigh, 0);
	//return nRes != SOCKET_ERROR;
	m_pEventDelegate->didSendCompleted(this, pSocketContect, pIoContext->m_szBuffer, pIoContext->m_Overlapped.InternalHigh);
	return true;
}


/////////////////////////////////////////////////////
// �����(Socket)�󶨵���ɶ˿���
/*************************************************
Function:       //_AssociateWithIOCP
Description:    //����ͻ���ͨ�ŵ�SOCKET�󶨵�IOCP
Calls:          //CreateIoCompletionPort
Input:          //PER_SOCKET_CONTEXT *pContext - ���ӵ�socket��������
Return:         //�ɹ�����true
*************************************************/
bool CG2XSocketServer::_AssociateWithIOCP( PER_SOCKET_CONTEXT *pContext )
{
	// �����ںͿͻ���ͨ�ŵ�SOCKET�󶨵���ɶ˿���
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPort, (DWORD)pContext, 0);

	if (NULL == hTemp)
	{
		m_pEventDelegate->didDumpText("CreateIoCompletionPort() error!", true);
		m_pEventDelegate->didHandleError(this, m_pListenContext, CG2XServerSocketDelegate::ERROR_INIT_IOCP, GetLastError());
		return false;
	}

	return true;
}


//====================================================================================
//
//				    ContextList ��ز���
//
//====================================================================================


//////////////////////////////////////////////////////////////
// ���ͻ��˵������Ϣ�洢��������
void CG2XSocketServer::_AddToContextList( PER_SOCKET_CONTEXT *pHandleData )
{
	EnterCriticalSection(&m_csContextList);

	m_arrayClientContext.Add(pHandleData);	
	
	LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
//	�Ƴ�ĳ���ض���Context
void CG2XSocketServer::_RemoveContext( PER_SOCKET_CONTEXT *pSocketContext )
{
	EnterCriticalSection(&m_csContextList);

	for( int i=0;i<m_arrayClientContext.GetCount();i++ )
	{
		if( pSocketContext==m_arrayClientContext.GetAt(i) )
		{
			RELEASE( pSocketContext );			
			m_arrayClientContext.RemoveAt(i);			
			break;
		}
	}

	LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
// ��տͻ�����Ϣ
void CG2XSocketServer::_ClearContextList()
{
	EnterCriticalSection(&m_csContextList);

	for( int i=0;i<m_arrayClientContext.GetCount();i++ )
	{
		delete m_arrayClientContext.GetAt(i);
	}

	m_arrayClientContext.RemoveAll();

	LeaveCriticalSection(&m_csContextList);
}



//====================================================================================
//
//				       ����������������
//
//====================================================================================



////////////////////////////////////////////////////////////////////
// ��ñ�����IP��ַ
/*************************************************
Function:       //GetLocalIP
Description:    //ȡ�ñ�����IP��ַ�б��еĵ�һ����ַ
Calls:          //gethostname��gethostbyname
Return:         //char* m_strIP -- IP��ַ�ַ���
*************************************************/
char* CG2XSocketServer::GetLocalIP()
{
	// ��ñ���������
	char hostname[MAX_PATH] = {0};
	gethostname(hostname,MAX_PATH);                
	struct hostent FAR* lpHostEnt = gethostbyname(hostname); //���ذ�����ַ���͵�ַ�б��hostent�ṹ��
	if(lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	// ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP(��Ϊһ̨�������ܻ�󶨶��IP)
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      

	// ��IP��ַת�����ַ�����ʽ
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	strcpy(m_strIP,inet_ntoa(inAddr));        

	return m_strIP;
}

///////////////////////////////////////////////////////////////////
// ��ñ����д�����������
int CG2XSocketServer::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}


/////////////////////////////////////////////////////////////////////
// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ���������һ����Ч��Socket��Ͷ��WSARecv����������쳣
// ʹ�õķ����ǳ��������socket�������ݣ��ж����socket���õķ���ֵ
// ��Ϊ����ͻ��������쳣�Ͽ�(����ͻ��˱������߰ε����ߵ�)��ʱ�򣬷����������޷��յ��ͻ��˶Ͽ���֪ͨ��

bool CG2XSocketServer::_IsSocketAlive(SOCKET s)
{
	int nByteSent=send(s,"",0,0);
	if (-1 == nByteSent) return false;
	return true;
}

///////////////////////////////////////////////////////////////////
// ��ʾ��������ɶ˿��ϵĴ���
bool CG2XSocketServer::HandleError( PER_SOCKET_CONTEXT *pContext,const DWORD& dwErr )
{
	return true;

	//// ����ǳ�ʱ�ˣ����ټ����Ȱ�  
	//if(WAIT_TIMEOUT == dwErr)  
	//{  	
	//	// ȷ�Ͽͻ����Ƿ񻹻���...
	//	if( !_IsSocketAlive( pContext->m_Socket) )
	//	{
	//		this->m_pEventDelegate->didClientExceptExit(pContext);
	//		this->_RemoveContext( pContext );
	//		return true;
	//	}
	//	else
	//	{
	//		this->_ShowMessage( _T("���������ʱ��������...") );
	//		return true;
	//	}
	//}  

	//// �����ǿͻ����쳣�˳���
	//else if( ERROR_NETNAME_DELETED==dwErr )
	//{
	//	this->_ShowMessage( _T("��⵽�ͻ����쳣�˳���") );
	//	this->_RemoveContext( pContext );
	//	return true;
	//}

	//else
	//{
	//	this->_ShowMessage( _T("��ɶ˿ڲ������ִ����߳��˳���������룺%d"),dwErr );
	//	return false;
	//}
}




