#pragma once

#include "G2XSocketServer.h"
#include "..\Singleton.h"
#include "..\common\ThreadTemplate.h"
#include "gxx_base.h"
#include <map>
#include "LocalDefine.h"
#include "..\Excel\CriticalSection.h"

// �޴�
#define ERROR_CODE_NONE 0
// �Ƿ�����
#define ERROR_CODE_ILLEGAL 1 
// У��ʧ��
#define ERROR_CODE_CHECK 2 
// ȱ�ٲ���
#define ERROR_CODE_PARAM 3
// �������
#define ERROR_CODE_TIMEOUT 4
// ����̫��
#define ERROR_CODE_DATA_TOO_LARGE 5


//////////////////////////////////////////////////////////////////////////
// 2015/5/15 tmelody
// ע��ɹ���¼��������Ҫ����Ϣ�ṹ

typedef struct tagUserInfo
{
	CString _strAccount;		//�˺�
	CString _strMemberID;		//��Ա��
	CString _strPwd;			// 
	CString _strClassID;		//��Ա����
	BOOL	_bLocalRegister;	//�Ƿ񱾵�ע��

	tagUserInfo()
		: _strAccount(_T(""))
		, _strMemberID(_T(""))
		, _strPwd(_T(""))
		, _strClassID(_T(""))
		, _bLocalRegister(FALSE){
		}
}USERINFO, *PUSERINFO, *LPUSERINFO;


//////////////////////////////////////////////////////////////////////////
// 

class CLocalCenter : public CG2XServerSocketDelegate
{
	SINGLETON_DECLARE(CLocalCenter);
public:

	static CLocalCenter* GetInstance();

	BOOL InitLocalCenter();

	// 2015/5/15 tmelody
	// ��¼�����û�
	BOOL RecordRegisteredUser(const USERINFO& userInfo);

	/**************************************************************************
	*
	* CG2XServerSocketDelegate
	*
	***************************************************************************/
private:
	virtual bool didClientConnected(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, const char* pData, const int nBufLen);
	virtual void didDumpText(const char* pText, bool isErrorDump/* =false */);
	virtual bool didHandleError(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, ERROR_CODE code, DWORD dwLastErrorCode);

private:
	void ThreadEntry();
	void ThreadEntryTestNetworking();

	
	GxxValuePtr ToUtf8Value(LPCWSTR lpText);
	
	std::wstring GetDataCodeErrorMessage(long nDataCodeError);
	void PutDataCode(GxxDictionaryPtr& outputInfo, long nDataCodeError);
	BOOL IsBundleTimeEnd(GxxDictionaryPtr& memberPtr);

private:
	typedef void (CLocalCenter::*_HandleFunc)(GxxDictionaryPtr, CG2XSocketServer* , PER_SOCKET_CONTEXT*) ;


	struct _HandleInfo {
		_HandleFunc handle;
	};
	// ���������������� 
	int _Decode(const char* data, int nLen, GxxDictionaryPtr& reqData);

	void _handleCommand(const char* command, GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient);
	void _addHandle(const char* command, CLocalCenter::_HandleFunc memberFunc);

private:
	/** 
	 * ����ͻ��������ѯ����ģʽ
	*/
	void HandleQueryLocalMode(GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient);
	/**
	 * ����ͻ������󱾵ص�¼
	*/
	void HandleLocalLogon(GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient);
	/**
	 * ����ͻ��������ѯ���
	*/
	void HandleQueryBalance(GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient);
	/**
	 * �����ϱ���¼״̬
	*/
	void HandleReportLogon(GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient);

private:
	/**
	 * sql����ѯ����
	*/
	void HandleSqlQuery(GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient);

public:
	void OutputErrorInfo(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, int nErrorCode);
	void OutputInfo(CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, GxxDictionaryPtr outputInfo);

private:
	NS_COMMON::CThreadTemplate<CLocalCenter> m_thread;
	NS_COMMON::CThreadTemplate<CLocalCenter> m_threadTestNetwork;
	std::map<std::string, _HandleInfo> m_mapHandles;

	static const UINT SERVER_COUNT = 2;
	UINT m_nPortArray[SERVER_COUNT];
	CG2XSocketServer *m_serverArray[SERVER_COUNT];

	// ��¼��ʼ����ʱ��
	DWORD m_dwStartUpTick;

};
