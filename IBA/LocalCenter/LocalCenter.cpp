#include "stdafx.h"
#include ".\localcenter.h"
#include "..\IBA_DEBUG.h"
#include "..\IBALog.h"
#include "gxx_base.h"
#include "base64.h"
#include "G2XCommonTool.h"
#include "..\Encrypt.h"
#include "..\NetBarConfig.h"
#include "..\IBA.h"
#include "..\GlobalFunctions.h"
#include "..\Dal\IBADAL.h"
#include "GxxString.h"
#include "..\Socket\LocalServer.h"
#include <time.h>
#include "..\IBAHelpper.h"
#include "..\CashierHelper.h"
#include "..\ServletHellper.h"
#include "..\Servlet\CommonMethod.h"
#include "..\GlobalFunctions.h"
#include "..\CurrentNetBarUser.h"
#include "..\MainFrm.h"


using namespace G2XUtility;
using namespace IBAGlobal;
using namespace NS_DAL;

static const long DATA_CODE_NO_ERROR = 0;
static const long DATA_CODE_INVALID_USER = 1;  // ��Ч�˻�
static const long DATA_CODE_INVALID_PASSWORD = 2;  // �˺ź�������Ч
static const long DATA_CODE_LOCAL_CHECKOUT = 3; // �����ѽ��ˣ���δ�ϱ�����֮ǰ�����ϻ�
static const long DATA_CODE_LOCAL_NO_ACTIVE = 4; // ����δ����
static const long DATA_CODE_NO_USER_INFO = 5; // ����ȱ���û���Ϣ
static const long DATA_CODE_NO_USER_RATE = 6; // ��Ч�û���ȱ�ٿ�����Ϣ
static const long DATA_CODE_NO_BALANCE = 7; // ����
static const long DATA_CODE_CHANGE_IN_LOCALMODE = 8; //����ģʽ��������

static const long DATA_CODE_DATABASE_ERROR = 99; // ���ݿ��쳣

//����һ��ֻ��һ���߳̽����ϱ����ٽ���Դ
static CCriticalSection g_csReportLocalCenterUserLock;

SINGLETON_IMPLEMENT(CLocalCenter)
/*************************************************
Function:       //���캯��
Description:    //����˿�7801,7802,
				//Ϊÿ���˿�new SocketServer,
				//��m_mapHandles�����������������(queryLocalMode,LocalLogon,QueryBalance,ReportLogon,SqlQuery)
*************************************************/
CLocalCenter::CLocalCenter(void) 
{
	UINT nPortArray[SERVER_COUNT] = {7801,7802};

	for (UINT i = 0; i < SERVER_COUNT; i++) {
		m_nPortArray[i] = nPortArray[i];
		m_serverArray[i] = new CG2XSocketServer(this);
	}

	_addHandle("queryLocalMode", &CLocalCenter::HandleQueryLocalMode);
	_addHandle("localLogon", &CLocalCenter::HandleLocalLogon);
	_addHandle("queryBalance", &CLocalCenter::HandleQueryBalance);
	_addHandle("reportLogin", &CLocalCenter::HandleReportLogon);

	_addHandle("sqlQuery", &CLocalCenter::HandleSqlQuery);
}

CLocalCenter::~CLocalCenter(void)
{
	for (UINT i = 0; i < SERVER_COUNT; i++) {
		delete m_serverArray[i];
	}
}
/*************************************************
Function:       //InitLocalCenter
Description:    //Ϊÿ���˿ڳ�ʼ��������(����IOCP,��������worker_thread,׼����������)
				//�������ؼƷ��̺߳ͼ�������߳�. 
Return:         //�ɹ�����TRUE
*************************************************/
BOOL CLocalCenter::InitLocalCenter()
{
	BOOL bRes = FALSE;
	for (UINT i = 0; i < SERVER_COUNT; i++) {
		CG2XSocketServer* pServer = m_serverArray[i];
		if (pServer->Initialize()){
			if (pServer->Start(m_nPortArray[i])){
				bRes = TRUE;
			}
		}
	}

	m_thread.SetCallBack(this, &CLocalCenter::ThreadEntry);
	m_thread.Start();

	//m_threadTestNetwork.SetCallBack(this, &CLocalCenter::ThreadEntryTestNetworking);
	//m_threadTestNetwork.Start();
	return bRes;
}

BOOL CLocalCenter::RecordRegisteredUser(const USERINFO& userInfo)
{	
	if (userInfo._strAccount.IsEmpty() ||
		userInfo._strMemberID.IsEmpty() ||
		userInfo._strClassID.IsEmpty())
	{
		IBA_LOG(_T("RecordRegisteredUser Error: account:%s, memberid:%s, classid:%s"),
			userInfo._strAccount, userInfo._strMemberID, userInfo._strClassID);
		return FALSE;
	}

	// ȡ���˺�
	CGxxString<TCHAR> strAccount = userInfo._strAccount;
	strAccount.FillCharFromLeft(10, '0');

	// ȡ��memberId
	int nMemberId = _ttoi((LPCTSTR)userInfo._strMemberID);

	// ȡ������
	CString strPassword = userInfo._strPwd;
	strPassword.ReleaseBuffer();
	if (strPassword.GetLength() > 8)
		strPassword = strPassword.Left(8);
	if (!strPassword.IsEmpty())
	{
		int nLen = strPassword.GetLength();
		for (int i=nLen; i<8; ++i)
			strPassword.Append(_T("0"));
	}
	CString strPasswordMD5;
	CEncrypt::CalcMD5(strPassword, strPasswordMD5);

	// �����ϱ�ʱ��
	CString strReportTime = CIBAHelpper::GetCurrentTimeAsString(TRUE);

	// ��Ա����
	int nClassId = _ttoi((LPCTSTR)userInfo._strClassID);

	// �Ƿ񱾵�ע��
	//int nLocalRegister = static_cast<int>(userInfo._bLocalRegister);
	int nLocalRegister = 0;

	// ���ϱ����в�������
	CString sql;
	sql.Format(_T("select count(*) from localreport where memberId=%d"), nMemberId);
	if (CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) > 0) 
	{
		sql.Format(_T("update localreport set password='%s',reportTime='%s',localRegister=%d where memberId=%d"), 
			strPasswordMD5, _T(""), nMemberId);
	} 
	else 
	{
		sql.Format(_T("insert into localreport (memberId,netId,password,reportTime,localRegister) values(%d,'%s','%s','%s',%d)"), 
			nMemberId, strAccount.GetData(), strPasswordMD5, _T(""), nLocalRegister);
	}
	if (CIBADAL::GetInstance()->ExecuteSQL(sql) != 1) 
	{
		IBA_LOG(_T("RecordRegisteredUser������LocalReport��ʧ�ܣ�"));
		return FALSE;
	}
	return TRUE;
}

/**************************************************************************
*
* CG2XServerSocketDelegate
*
***************************************************************************/

/**
�ͻ������ӣ����ҽ��յ�һ������
*/
bool CLocalCenter::didClientConnected(CG2XSocketServer* pServer,  PER_SOCKET_CONTEXT* pClient, const char* pData, const int nBufLen )
{
	IBA_LOG(_T("Start didClientConnected"));
	GxxDictionaryPtr reqData;
	int nError = this->_Decode(pData, nBufLen, reqData); //�����ݽ������ֵ���
	if (nError != ERROR_CODE_NONE) {
		// ����ʧ�ܣ����ش�����Ϣ���ͻ���
		this->OutputErrorInfo(pServer, pClient, nError);
		return false;
	}
	
	this->_handleCommand(reqData->valueForKey("cmd")->stringValue(), reqData, pServer, pClient);

	// �����ӷ������轨���ͻ��˳����ӣ���Զ����false
	return false;
}
/*************************************************
Function:       //didDumpText
Description:    //�����־����CLocalCenter::dump��ͷ
Input:          //const char* pText - �����������
*************************************************/
void CLocalCenter::didDumpText( const char* pText, bool isErrorDump/* =false */ )
{
	GXX_TRACE("CLocalCenter::dump: %s", pText);
	CA2T ca(pText);
	IBA_LOG(_T("CLocalCenter::dump: %s"), (const TCHAR*)ca);
}

bool CLocalCenter::didHandleError(CG2XSocketServer* pServer,  PER_SOCKET_CONTEXT* pClient, ERROR_CODE code, DWORD dwLastErrorCode )
{
	return false;
}

/**************************************************************************
*
* 
*
***************************************************************************/

int CLocalCenter::_Decode( const char* data, int nLen, GxxDictionaryPtr& reqData)
{
	// ǰ���ֽڱ�ʾ�������ݵĳ���
	if (nLen < 2) {
		return ERROR_CODE_ILLEGAL;
	}
	BYTE bt1 = data[0];
	BYTE bt2 = data[1];
	int nReqLen = (int)bt1;
	nReqLen <<= 8;
	nReqLen += (int)bt2;
	if (nReqLen + 2 != nLen) {
		return ERROR_CODE_ILLEGAL;
	}
	
	std::string strReq;
	strReq.append(data+2, nReqLen);

	// base64����
	strReq = base64_decode(strReq);

	// ת����json�ֵ�
	reqData = GxxDictionary::create();
	if (!reqData->initWithJsonText(strReq.c_str())) {
		// ��Ч��JSON����
		return ERROR_CODE_ILLEGAL;
	}
	
	// ��֤������ȷ��
	if (!reqData->isKeyExist("cmd") || !reqData->isKeyExist("t") || !reqData->isKeyExist("checkcode")) {
		return ERROR_CODE_PARAM;
	}
	
	const char* key = "F3EA0F8D";

	time_t tm = time(NULL);
	long tmReq = reqData->valueForKey("t")->intValue();
	if (abs(tm - tmReq) > 5*60) {
		// ����5���ӣ�����֤��ʱ
		return ERROR_CODE_TIMEOUT; 
	}

	std::string codeString;
	codeString.append(reqData->valueForKey("cmd")->stringValue());
	codeString.append(reqData->valueForKey("t")->stringValue());
	codeString.append(key);
	CStringA strNetBarId; strNetBarId.Format("%d", CNetBarConfig::GetInstance()->GetNetBarId());
	codeString.append((const char*)strNetBarId);

	CStringA strMd5;

	CString strCodeString(codeString.c_str());
	//IBA_LOG(_T("CLocalCenter::_Decode  Checkcode before calc: %s"),strCodeString);

	CEncrypt::CalcMD5Bytes((void *)codeString.c_str(), codeString.length(), strMd5.GetBuffer(33));
	strMd5.ReleaseBuffer();

	char* lpsrc = strMd5.GetBuffer();
	int i = MultiByteToWideChar(CP_ACP, 0, lpsrc, -1, NULL, 0);
	wchar_t* strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, lpsrc, -1, strSrc, i);
	CString strbefMd5(strSrc);
	delete []strSrc;

	//IBA_LOG(_T("CLocalCenter::_Decode  Checkcode after calc: %s"),strbefMd5);

	if (strMd5.CompareNoCase(reqData->valueForKey("checkcode")->stringValue()) != 0){
		// У��ʧ��
		return ERROR_CODE_CHECK;
	}
	return ERROR_CODE_NONE;
}

void CLocalCenter::OutputErrorInfo( CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, int nErrorCode )
{
	CString str;
	switch (nErrorCode) {
		case ERROR_CODE_ILLEGAL:
			str = _T("�Ƿ�����");
			break;
		case ERROR_CODE_CHECK:
			str = _T("У��ʧ��");
			break;
		case ERROR_CODE_PARAM:
			str = _T("ȱ�ٲ���");
			break;
		case ERROR_CODE_TIMEOUT:
			str = _T("�������");
			break;
		case ERROR_CODE_DATA_TOO_LARGE:
			str = _T("���ݳ���̫��");
			break;
		default:
			str = _T("δ֪����");
			break;
	}

	GxxDictionaryPtr responseData = GxxDictionary::create();

	CStringA tmp;
	tmp.Format("%d", nErrorCode);	
	responseData->setValueForKey(GxxValue::create(tmp.GetBuffer()), "errorcode");
	tmp.ReleaseBuffer();

	responseData->setValueForKey(ToUtf8Value(str.GetBuffer()), "message");
	str.ReleaseBuffer();

	this->OutputInfo(pServer, pClient, responseData);
}

void CLocalCenter::OutputInfo( CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient, GxxDictionaryPtr outputInfo )
{
	const char* key = "F3EA0F8D";

	if (!outputInfo->isKeyExist("errorcode")) {
		// û�д�����������Ϊ0
		outputInfo->setValueForKey(GxxValue::create("0"), "errorcode");
	}

	// ���ʱ���
	time_t tm = time(NULL);
	CStringA strTmp;
	strTmp.Format("%lld", tm);
	outputInfo->setValueForKey(GxxValue::create((const char*)strTmp), "t");

	// ���У����
	strTmp.Append(key);
	strTmp.AppendFormat("%d", CNetBarConfig::GetInstance()->GetNetBarId());
	CStringA checkCode;
	CEncrypt::CalcMD5Bytes((void*)strTmp.GetString(), strTmp.GetLength(), checkCode.GetBuffer(33));
	checkCode.ReleaseBuffer(33);
	outputInfo->setValueForKey(GxxValue::create((const char*)checkCode), "checkcode");

	// base64����
	std::string strInfo = outputInfo->describe_d(0, false);
	strInfo = base64_encode((unsigned char*)strInfo.c_str(), strInfo.length());
	int nLen = strInfo.length();

	// ��д����
	CByteArray byteArray;
	byteArray.SetSize(nLen + 2);
	BYTE bt1,bt2;
	
	bt1 = (nLen & 0x0000FF00)>>8;
	bt2 = nLen & 0x000000FF;
	BYTE* pData = byteArray.GetData();
	pData[0] = bt1; pData[1] = bt2;
	memcpy(pData+2, strInfo.c_str(), nLen);

	if (nLen + 2 > MAX_BUFFER_LEN)
	{
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_DATA_TOO_LARGE);
		return;
	}

	// ���ͻ��˷�������
	pServer->SendData(pClient, pData, nLen + 2);

	
}

//��Ԥ�ȶ���������map��,���Ҷ�Ӧ�Ĵ�������������
void CLocalCenter::_handleCommand( const char* command, GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient )
{
	std::map<std::string, _HandleInfo>::iterator itFound = m_mapHandles.find(std::string(command));
	if (itFound != m_mapHandles.end()) {
		_HandleInfo& info = (*itFound).second;
		_HandleFunc func = info.handle;
		(this->*func)(reqData, pServer, pClient);
		return;
	}

	// ��������Ƿ�����Ϣ���ͻ���
	OutputErrorInfo(pServer, pClient, ERROR_CODE_ILLEGAL);
}
/*************************************************
Function:       //_addHandle
Description:    //������m_mapHandles�����pair. ����1��ʾ�����������2��ʾ���崦����
Input:          //const char* command                  -- �����ַ���
                //CLocalCenter::_HandleFunc memberFunc -- �������
*************************************************/
void CLocalCenter::_addHandle( const char* command, CLocalCenter::_HandleFunc memberFunc )
{
	_HandleInfo info;
	info.handle = memberFunc;
	this->m_mapHandles[std::string(command)] = info;
}

/*************************************************
Function:       // ThreadEntryTestNetworking
Description:    // ̽������Ͽ��̺߳�������������ʱ������ÿ��һ��ʱ��ͨ������������ʱ������������״̬��������粻ͨ����������״̬Ϊ�Ͽ���
				// �������״̬�ѱ�����Ͽ����򲻲��ԡ�
Calls:          // CGetTimeMethod::Execute
*************************************************/
void CLocalCenter::ThreadEntryTestNetworking()
{
	IBA_LOG(_T("Start thread:Test network status"));
	//liyajun Temp
#ifdef _DEBUG
	const DWORD _timerTime = 1000*10;
#else
	const DWORD _timerTime = 1000*90;
#endif
	
	do {
	//try{

		while(TRUE)
		{
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
			{
				CGetTimeMethod time;
				time.Execute();

				if (time.GetStatusCode() < 0)
				{
					IBA_TRACE(_T("�������������ѶϿ�"));
				}
			}		
			Sleep(_timerTime);
		}
		
	//}
	//catch(...)
	//{}
	} while(TRUE);
	
}

/*************************************************
Function:       // ThreadEntry
Description:    // ���ؼƷ��߳�-��������ʱ����,�������ʱ���˳�.���м��30s
Table Accessed: // SystemConfig/LocalMember/ActiveMember
Table Updated:  // SystemConfig:localCounting,LocalMember
*************************************************/
void CLocalCenter::ThreadEntry()
{
	IBA_LOG(_T("Start thread:Local charging"));
	const DWORD _oneMin = 1000*60;
	const DWORD _timerTime = _oneMin / 2; // �����. Note: ����������̨�ڼƷ�ʱ��������̨�߳�ÿ30sѭ��һ��
	do 
	{
	//try
	//{
		while (TRUE) 
		{
			//IBA_LOG(_T("���ؼƷ�:New loop"));
			/********У�鱾�ؼƷ�����̨*************/
			int nCashierID;
			int nLastUpdateTime;
			//�Ƿ����localCounting��¼
			int nRecordCount = CIBADAL::GetInstance()->GetLocalCounting(nCashierID,nLastUpdateTime);
			if(nRecordCount < 0) 
			{
				IBA_LOG(_T("Error.���ݿ���ұ��ؼƷѼ�¼ʧ��."));
				Sleep(_timerTime);
				continue;
			}
			if(nRecordCount > 0)//���������
			{
				//��ǰ���ؼƷ�����̨�Ƿ���ǵ�ǰ����̨
				if(nCashierID == CIBAConfig::GetInstance()->GetCashRegisterID())//�������ʱ��
				{
					if(FALSE == CIBADAL::GetInstance()->UpdateLastUpdateTime_LocalCounting()){Sleep(_timerTime);continue;}
				}
				else//���������ϴθ���ʱ��
				{
					//���ϴθ����Ƿ��ѳ���2����
					time_t now = time(NULL);
					if (now - nLastUpdateTime > 2*_oneMin)//���������Ϊ��ǰ����̨�Ʒ�
					{
						if(FALSE == CIBADAL::GetInstance()->UpdateCashierandTime_LocalCounting()){Sleep(_timerTime);continue;}
					}
					else//δ����.��ʾ��������̨���ڼƷ�,��ǰ����̨���Ʒ�,�˳�ѭ��.
					{
						IBA_LOG(_T("��������̨�ڼƷ�"));
						Sleep(_timerTime);
						continue;
					}
				}
			}
			else//�����������
			{
				if(FALSE == CIBADAL::GetInstance()->AddLocalCounting()){Sleep(_timerTime);continue;}
			}		
			/********��ʼ�Ʒ�*************/
			//ȡ��LocalMember�����м�¼
			LocalMembers lMembers;
			if(FALSE == CIBADAL::GetInstance()->GetAllLocalMember(lMembers)){Sleep(_timerTime);continue;}//û�м�¼,����

			//�������м�¼
			for(std::vector<CLocalMember>::iterator iter = lMembers.begin();iter != lMembers.end();++iter)
			{
				CLocalMember lmember = *iter;
				/**************************************************************************************
				note:�Ʒ�������localmember�ȼƷ�,����UDO,UDO��updatedata����IBA��ͬʱ����ActiveMember.
				���,��ʱ��ActiveMember��Ϣ���ͺ���localmember��,��������,����Ҫʹ��localmember.
				**************************************************************************************/

				//δ����Ĳ��Ʒ�
				CActiveMember ActiveMember;
				if(FALSE == CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(lmember.nMemberId,ActiveMember)){continue;}
				//δ���ߵ�?��ʱ��δ�ϱ�,���ֱ��߳�ȥ�˵�.
				if(FALSE == ActiveMember.GetIsOnLine()){continue;}
				//�ѽ��˵Ĳ��Ʒ�
				if(lmember.nIsReturned){continue;}
				//δ��½�Ĳ��Ʒ�
				if(FALSE == CIBAHelpper::IsValidTime(lmember.checkinTime) || lmember.checkinTime < COleDateTime(2015,1,1,0,0,0)){continue;}
				//�ۼƷⶥ��,������Ϊ0,���Ʒ�
				if(1 == lmember.nPayType && 0 == lmember.nBalance){continue;}
				//���Ԥ��.��ʱδ�����Ʒ�,��ʱ�ѵ�ת�����Ʒ�
				if(2 == lmember.nPayType)
				{
					//У��nextcheckinTime
					if(lmember.nNextCheckinTime < 0)//ֵ�Ƿ�,ת�����Ʒ�,����
					{	
						CIBADAL::GetInstance()->UpdateLocalMember_PayTypeInfo(lmember.nMemberId,0,0);
						continue;
					}
					else
					{
						time_t now = time(NULL);
						if(now < lmember.nNextCheckinTime)//��ʱδ�����Ʒ�
						{
							continue;;
						}
						else//��ʱ����,ת�����Ʒ�,����
						{
							CIBADAL::GetInstance()->UpdateLocalMember_PayTypeInfo(lmember.nMemberId,0,0);
							continue;;
						}
					}
				}

				//�����ʱ��,<30s����Ʒ�
				time_t lastT = lmember.nLastUpdateTime;
				time_t nowT = time(NULL);
				if (nowT - lastT < 30){continue;}// С��30����������    

				//��������Ϣ�����������Ч�����Ʒ�
				if(lmember.strRate.GetLength() != 96)
				{
					IBA_LOG(_T("���ؼƷ�:��Ч������Ϣ(%s),�޼Ʒ�"),lmember.strRate);
					continue;
				}

				COleDateTime lt(lastT);
				COleDateTime nt(nowT);
				CString strlt = lt.Format(_T("%Y-%m-%d %H:%M:%S"));
				CString strnt = nt.Format(_T("%Y-%m-%d %H:%M:%S"));
				IBA_LOG(_T("���ؼƷ� lastT=%s,nowT=%s"),strlt,strnt);
				IBA_LOG(_T("���ؼƷ� lmember.nBalance=%d,lmember.strRate=%s"),lmember.nBalance,lmember.strRate);
				//�������,����balance
				int nNewBalance = CCashierHelper::CalcExpense(lmember.strRate,lmember.nLastUpdateTime,lmember.nBalance);
				if(nNewBalance <= 0)
				{
					nNewBalance = 0;
				}
				if(0 > CIBADAL::GetInstance()->UpdateLocalMember_BalanceInfo(lmember.nMemberId,nNewBalance))//���һֱΪ0ʱ���᲻������
				{
					IBA_LOG(_T("Error.���ؼƷѸ������ݿ�Balanceʧ��."));
					continue;;
				}
				IBA_LOG(_T("���ؼƷ� localMember���:%d ActiveMember���:%d"),nNewBalance,ActiveMember.GetAvailavleBalance());

			}		
			//�ƷѼ��30s
			Sleep(_timerTime);
		}
		//}
		//catch(...) 
		//{
		//}
	}while (TRUE);
}

/************************************************************************/
/* ��ѯ����״̬                                                                     */
/************************************************************************/
void CLocalCenter::HandleQueryLocalMode( GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient )
{
	IBA_LOG(_T("Start HandleQueryLocalMode"));
	// �жϲ����Ƿ����
	if (!reqData->isKeyExist("termID"))
	{
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_PARAM);
		return;
	}
	CString strTermID = CA2T(reqData->valueForKey("termID")->stringValue());// �ն˺�

	GxxDictionaryPtr outputInfo = GxxDictionary::create();
	if (!outputInfo->isKeyExist("errorcode")) 
	{
		outputInfo->setValueForKey(GxxValue::create("0"), "errorcode");
		outputInfo->setValueForKey(GxxValue::create("request successfully!"), "message");
	}	
	outputInfo->setValueForKey(GxxValue::create(GxxDictionary::create()), "data");

	//������ΪNormalʱ,localMember�����и��ն�����Ϊ����Broken,�������Ϊ�����Normal��
	long nMode(0);
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		IBA_LOG(_T("HandleQueryLocalMode-����Ͽ�.LocalMode=TRUE."));
		nMode = TRUE;
	}
	else
	{
		CLocalMember lMember;
		if(TRUE == CIBADAL::GetInstance()->GetLocalMemberByTermID(strTermID,lMember))
		{
			IBA_LOG(_T("HandleQueryLocalMode-��������.����������δ�ָ�.LocalMode=TRUE."));
			nMode = TRUE;
		}
	}

	outputInfo->setValueForKeyPath(GxxValue::create(nMode), "data.mode");	
	this->OutputInfo(pServer, pClient, outputInfo);
}

/***************************************************************************************************
Function:       //HandleLocalLogon
Description:    //�ն˱���ģʽ��½(����ģʽ��,�û��״ε�½ / �û�һֱ���ߵ����뱾��ģʽ,UDO�Զ����б��ص�¼)
Table Accessed: //LocalMember,LocalReport,CostRate
Table Updated:  //LocalMember
********************************************************************************************************/
void CLocalCenter::HandleLocalLogon( GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient )
{
	IBA_LOG(_T("Start HandleLocalLogon"));
	// �жϲ����Ƿ����
	if (
		!reqData->isKeyExist("termID") ||
		!reqData->isKeyExist("account") ||
		!reqData->isKeyExist("pwd")) 
	{
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_PARAM);
		return;
	}

	//��������
	CGxxString<TCHAR> strAccount = CA2T(reqData->valueForKey("account")->stringValue());// �˺�	
	strAccount.FillCharFromLeft(10, _T('0'));// �˺Ų���10λ����߲���	
	CString strTermID = CA2T(reqData->valueForKey("termID")->stringValue());// �ն˺�	
	CString strPwd = CA2T(reqData->valueForKey("pwd")->stringValue());// ����
	
	IBA_LOG(_T("�ն˱��ص�¼.TermID:%s ACcount:%s"),strTermID,strAccount.GetData());

	//׼�����ص�����
	GxxDictionaryPtr outputInfo = GxxDictionary::create();
	outputInfo->setValueForKey(GxxValue::create(GxxDictionary::create()), "data");// �����ĵ�������data�ڵ�	

/*******���������********/

	//����˺�����(LocalReport��)
	CLocalReport lr;
	if (FALSE == CIBADAL::GetInstance()->GetLocalReport(strAccount.GetData(),strPwd,lr))
	{
		IBA_LOG(_T("���ص�¼ʧ��.LocalReport�޶�Ӧ��¼.�˺Ż��������.(NetId:%s Password:%s)"),strAccount.GetData(),strPwd);
		this->PutDataCode(outputInfo, DATA_CODE_INVALID_PASSWORD);
		this->OutputInfo(pServer, pClient, outputInfo);
		return;
	}
	
	//����Ƿ񼤻��Լ��Ƿ����û���Ϣ
	UINT nMemberId = lr.nMemberId;
	CActiveMember ActiveMember;
	if(FALSE == CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember))
	{
		CString sql;
		sql.Format(_T("select count(*) from member where serialNum = '%s'"),strAccount.GetData());
		if(NS_DAL::CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) < 1)
		{
			IBA_LOG(_T("���ص�¼ʧ��.���û���Ϣ.(NetId:%s)"),strAccount.GetData());
			this->PutDataCode(outputInfo, DATA_CODE_NO_USER_INFO);
			this->OutputInfo(pServer, pClient, outputInfo);
			return;
		}
		IBA_LOG(_T("���ص�¼ʧ��.�û�δ����.(NetId:%s )"),strAccount.GetData());
		this->PutDataCode(outputInfo, DATA_CODE_LOCAL_NO_ACTIVE);
		this->OutputInfo(pServer, pClient, outputInfo);
		return;
	}

	//��������Ϣ����ʱ����
	if (ActiveMember.GetAvailavleBalance() == 0)
	{
		if(ActiveMember.GetPayType() == 0)//�����Ʒ����Ϊ0
		{
			IBA_LOG(_T("���ص�¼ʧ��.���Ϊ0.(NetId:%s)"),strAccount.GetData());
			this->PutDataCode(outputInfo, DATA_CODE_NO_BALANCE);
			this->OutputInfo(pServer, pClient, outputInfo);
			return ;
		}
		else 
		{
			if(ActiveMember.GetPayType() == 1 && ActiveMember.GetIsOnLine() == FALSE)//�ۼƷⶥ,���Ϊ0�Ҳ�����
			{
				IBA_LOG(_T("���ص�¼ʧ��.�ۼƷⶥ,���Ϊ0,���������µ�½.(NetId:%s)"),strAccount.GetData());
				this->PutDataCode(outputInfo, DATA_CODE_NO_BALANCE);
				this->OutputInfo(pServer, pClient, outputInfo);
				return ;
			}
			else
			{
				if(ActiveMember.GetPayType() == 2 && (ActiveMember.GetNextCheckinTime() < time(NULL)))//���Ԥ��,���Ѿ�����,���Ϊ0
				{
					IBA_LOG(_T("���ص�¼ʧ��.���Ԥ���ѵ�ʱ��,�����Ϊ0.(NetId:%s)"),strAccount.GetData());
					this->PutDataCode(outputInfo, DATA_CODE_NO_BALANCE);
					this->OutputInfo(pServer, pClient, outputInfo);
					return ;
				}
			}
		}
	}

	//����Ƿ��������ն��ϻ�(��checkinʱ���Լ�TermID�뵱ǰTermID��һ��)
	if(CIBAHelpper::IsValidTime(ActiveMember.GetCheckInTime()) && ActiveMember.GetTerminalID().CompareNoCase(strTermID) != 0)
	{
		IBA_LOG(_T("���ص�¼ʧ��.�û����������ն��ϻ�,�ұ����²�������(NetId:%s Password:%s)"),strAccount.GetData(),strPwd);
		this->PutDataCode(outputInfo, DATA_CODE_CHANGE_IN_LOCALMODE);
		this->OutputInfo(pServer, pClient, outputInfo);
		return ;
	}

	//��ȡ�ն�ID��Ӧ��PCClass
	strTermID.Trim();
	UINT CurPCClassId = CIBADAL::GetInstance()->GetPCClassIDWithTermID(strTermID);
	if(0 >= CurPCClassId)
	{
		IBA_LOG(_T("���ص�¼ʧ��,δ�ҵ��ն˺�(%s)��Ӧ������ID"),strTermID);
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_PARAM);
		return;
	}

	//��localMember�в��Ҽ�¼.û�����ȴ���������
	CLocalMember LocalMember;
	CString rate;
	if(FALSE == CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberId,LocalMember)) 
	{
		
		//����
		GxxDictionaryPtr memberPtr = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(nMemberId,strTermID);
		if (!memberPtr)
		{
			this->PutDataCode(outputInfo, DATA_CODE_DATABASE_ERROR);
			this->OutputInfo(pServer, pClient, outputInfo);
			return;
		}
		//����
		if (FALSE == CIBADAL::GetInstance()->InsertNewLocalMember(memberPtr))
		{
			this->PutDataCode(outputInfo, DATA_CODE_DATABASE_ERROR);
			this->OutputInfo(pServer, pClient, outputInfo);
			return;
		}
		//�����ɹ�,���¶�ȡһ��localMember
		CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberId,LocalMember);
	}
	else//localMember�б��ʹ��ڵļ�¼,���������ʧ��,�ɲ���ɾ��localMember��¼
	{
		if(LocalMember.nIsReturned > 0) //�Ѵ��ڵ�,���ڱ���ģʽ�½���,�������½.
		{
			IBA_LOG(_T("���ص�¼ʧ��.�û������ڱ���ģʽ�½���,���ϱ�����ǰ�����ٴε�½.(NetId:%s)"),strAccount.GetData());
			this->PutDataCode(outputInfo, DATA_CODE_LOCAL_CHECKOUT);
			this->OutputInfo(pServer, pClient, outputInfo);
			return ;
		}
	}
	
	//�����ն�����ID(��sm_terminfo����ȡ)�Ϳ���(��costrate����ȡ)��localMember��
	//���ҿ���
	rate = CIBADAL::GetInstance()->GetCostRateFromCostRate(LocalMember.nClassId,CurPCClassId);
	if(rate.GetLength() != 96)
	{
		IBA_LOG(_T("���ص�¼ʧ��,�޷���ȡ������Ϣ(NetId:%s)"),strAccount.GetData());
		this->PutDataCode(outputInfo, DATA_CODE_NO_USER_RATE);
		this->OutputInfo(pServer, pClient, outputInfo);
		return;
	}
	//���¿���
	if(0 > CIBADAL::GetInstance()->UpdateLocalMember_RateInfo(nMemberId,rate))
	{
		IBA_LOG(_T("���ص�¼ʧ��,�޷����¿�����Ϣ(NetId:%s)"),strAccount.GetData());
		this->PutDataCode(outputInfo, DATA_CODE_NO_USER_RATE);
		this->OutputInfo(pServer, pClient, outputInfo);
		return;
	}
	//��������ID
	if(0 > CIBADAL::GetInstance()->UpdateLocalMember_PCClassId(nMemberId,CurPCClassId))
	{
		IBA_LOG(_T("���ص�¼ʧ��,�޷������ն�������Ϣ(NetId:%s)"),strAccount.GetData());
		this->PutDataCode(outputInfo, DATA_CODE_DATABASE_ERROR);
		this->OutputInfo(pServer, pClient, outputInfo);
		return;
	}

	//�����ն�ID,checkintime,localcheckintime,islocalcheckin,LastUpdateTime	
	if (CIBAHelpper::IsValidTime(LocalMember.checkinTime) && LocalMember.checkinTime > COleDateTime(2015,1,1,0,0,0)) //�Ѿ���½,����localCheckinTimeΪ��ǰʱ��,�ն�IDΪ��ǰ�ն�
	{
		if (FALSE == CIBAHelpper::IsValidTime(LocalMember.localCheckinTime) || LocalMember.localCheckinTime < COleDateTime(2015,1,1,0,0,0))
		{
			COleDateTime localCheckinTime = COleDateTime::GetCurrentTime();
			CIBADAL::GetInstance()->UpdateLocalMember_CheckInInfo(nMemberId,strTermID,localCheckinTime);
		}
	}
	else //��δ��½.���α��ص�¼, ����checkintime, localCheckinTime,isLocalCheckIn.LastUpdateTime.ͬʱ�����ActiveMemer.checkinTime��TermId
	{
		COleDateTime checkintime = COleDateTime::GetCurrentTime();
		CIBADAL::GetInstance()->UpdateLocalMember_CheckInInfo(nMemberId,strTermID,checkintime,checkintime,TRUE);

		//Ϊ�˼�����ʾ,����ActiveMember.CheckinTime��TermId
		CString strtime;
		strtime = checkintime.Format(_T("%Y-%m-%d %H:%M:%S"));
		ActiveMember.SetCheckInTime(checkintime);		
		IBA_LOG(_T("�״α��ص�¼,checkinʱ��Ϊ:%s,memberIdΪ%d"),strtime,nMemberId);
		ActiveMember.SetTerminalID(strTermID);
		ActiveMember.SetPCClassID(CurPCClassId);
		CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember,TRUE);
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
	}

	//����ʱ��ɹ�,���¶�ȡһ��localMember
	CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberId,LocalMember);

/*******���ص�¼�ɹ�,������Ϣ************/
	int nConsumeTime = 0;
	if(CIBAHelpper::IsValidTime(LocalMember.checkinTime))
	{
		time_t lastUpdateTime = time(NULL);
		COleDateTimeSpan span = COleDateTime(lastUpdateTime) - LocalMember.checkinTime;;
		int nSecs = (int)span.GetTotalSeconds();
		if (nSecs > 0) 
		{
			nConsumeTime = nSecs;
		}
	}	
	int nConsumeType = LocalMember.nPayType;
	int nClassId = LocalMember.nClassId;
	int nIsVIP = CNetBarConfig::GetInstance()->GetIsMember(nClassId) ? 1 : 0;
	int nPCClass = LocalMember.nPcClass;
	int nBalance = LocalMember.nBalance;
	nBalance -= nBalance % 50;
	if (nBalance < 0) 
	{
		nBalance = 0;
	}
	int nUsedMoney = LocalMember.ntotalMoney - nBalance; 

	outputInfo->setValueForKeyPath(GxxValue::create((long)nMemberId), "data.memberID");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nBalance), "data.balance");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nConsumeTime), "data.consumeTime");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nConsumeType), "data.consumeType");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nClassId), "data.classID");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nIsVIP), "data.isVIP");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nPCClass), "data.pcClass");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nUsedMoney), "data.usedMoney");
	char sCheckinTime[128];//��½ʱ��
	memcpy(sCheckinTime,CT2A(LocalMember.checkinTime.Format(_T("%Y-%m-%d %H:%M:%S")).GetBuffer()),sizeof(sCheckinTime));
	outputInfo->setValueForKeyPath(GxxValue::create(sCheckinTime), "data.checkinTime");
	//char sMemberName[128];//�û���
	//memcpy(sMemberName,CT2A(LocalMember.strMemberName.GetBuffer()),sizeof(sMemberName));
	//outputInfo->setValueForKeyPath(GxxValue::create(sMemberName), "data.memberName");
	//IBA_LOG(_T("OnLocalLogon SendToUDO:UserName:%s"),LocalMember.strMemberName);
	CString sql;//�û���
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxDictionaryPtr memberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);
	outputInfo->setValueForKeyPath(GxxValuePtr(memberPtr->valueForKey("memberName")), "data.memberName");
	char sIdNumber[128];//�û�֤��ID��
	memcpy(sIdNumber,CT2A(LocalMember.strIdNumber.GetBuffer()),sizeof(sIdNumber));
	outputInfo->setValueForKeyPath(GxxValue::create(sIdNumber), "data.idNumber");
	outputInfo->setValueForKeyPath(GxxValue::create((long)LocalMember.nNextCheckinTime), "data.nextCheckinTime");//��ʱ����ʱ��
	char sRate[128];//����
	memcpy(sRate,CT2A(LocalMember.strRate.GetBuffer()),sizeof(sRate));
	IBA_LOG(_T("HandleLocalLogon: SendTo UDO Rate = %s"),LocalMember.strRate);
	outputInfo->setValueForKeyPath(GxxValue::create(sRate), "data.costRate");
	
	this->PutDataCode(outputInfo, DATA_CODE_NO_ERROR);;
	this->OutputInfo(pServer, pClient, outputInfo);
}

void TestBalance(int nTag, CString strNetId, int nMemberID)
{
	CIDCheckCardMethod IDCheckCard;
	IDCheckCard.SetSerialNum(strNetId);
	IDCheckCard.SetSerialNumType(1);

	theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

	int nCardId = 0;
	if (IDCheckCard.GetStatusCode() == 0)
	{
		nCardId = IDCheckCard.GetCardId();

		CBalanceMethod& Balance = CCurrentNetBarUser::GetInstance()->Balance;
		Balance.SetMemberId(nMemberID);
		Balance.SetCardId(nCardId);
		Balance.SetRound(0);

		theApp.GetCurCashier()->DoBalance(Balance);

		int nStatus = Balance.GetStatusCode();
		if (nStatus == 0)
		{
			// �ܽ��
			int a = Balance.GetTotalBalacne();

			// ��ֵ�˻��������
			int b = Balance.GetBalance();

			// �ϴ����ѽ��(��)������ֵΪ0
			int c = Balance.GetLastConsumeAmount();

			// Ӧ�����
			int d = Balance.GetReturnAmount();

			// Ѻ��
			int e = Balance.GetGuarantyMoney();

			// �û�Ԥ�۽��
			int f = Balance.GetOnlineAmount();

			IBA_TRACE(_T("%d_____�ܽ��:%d, ��ֵ�˻��������:%d, �ϴ����ѽ��:%d, Ӧ�����:%d, Ѻ��:%d, �û�Ԥ�۽��:%d"),
				nTag, a, b, c, d, e, f);
		}
	}
}

/***************************************************************************************************
Function:       //HandleQueryBalance
Description:    //�ն˲�ѯ���ؼƷ����
Table Accessed: //LocalMember,LocalReport,CostRate
Table Updated:  //LocalMember
********************************************************************************************************/
void CLocalCenter::HandleQueryBalance( GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient )
{
	if (!reqData->isKeyExist("memberID")) 
	{
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_PARAM);
		return;
	}
	UINT nMemberID = reqData->valueForKey("memberID")->intValue();

	IBA_LOG(_T("Start HandleQueryBalance-MemberId:%d"),nMemberID);
	
	//��localMember���в���
	CLocalMember lMember;
	if(FALSE == CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberID,lMember))//�޶�Ӧ��¼,������Ч�û�
	{
		GxxDictionaryPtr outputInfo = GxxDictionary::create();
		this->PutDataCode(outputInfo, DATA_CODE_INVALID_USER);		// ��Ч�û�
		this->OutputInfo(pServer, pClient, outputInfo);
		return;
	}

	//����ConsumeTime
	UINT nConsumeTime = 0;
	if (CIBAHelpper::IsValidTime(lMember.checkinTime)) 
	{
		COleDateTimeSpan span = COleDateTime::GetCurrentTime() - lMember.checkinTime;
		int nSecs = (int)span.GetTotalSeconds();
		if (nSecs > 0) 
		{
			nConsumeTime = nSecs;
		}
	}
	
	//����ʣ��ʱ��
	UINT nRemainTime = CCashierHelper::CalcRemainTime(lMember.nBalance,lMember.nLastUpdateTime,lMember.strRate);
	IBA_LOG(_T("CLocalCenter::HandleQueryBalance RemainTime = %d"),nRemainTime);

	// ��ëȡ��
	UINT nBalance = lMember.nBalance - lMember.nBalance % 50;
	if (nBalance < 0)
	{
		nBalance = 0;
	}

	//��������
	GxxDictionaryPtr outputInfo = GxxDictionary::create();

	outputInfo->setValueForKey(GxxValue::create(GxxDictionary::create()), "data");
	outputInfo->setValueForKeyPath(GxxValue::create((long)0), "data.code");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nRemainTime), "data.remainTime");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nBalance), "data.balance");
	outputInfo->setValueForKeyPath(GxxValue::create((long)nConsumeTime), "data.consumeTime");
	outputInfo->setValueForKeyPath(GxxValue::create((long)lMember.nPayType), "data.consumeType");

	char sNetCheckinTime[128];
	COleDateTime NextCheckinTime((time_t)lMember.nNextCheckinTime);
	memcpy(sNetCheckinTime,CT2A(NextCheckinTime.Format(_T("%Y-%m-%d %H:%M:%S")).GetBuffer()),sizeof(sNetCheckinTime));
	outputInfo->setValueForKeyPath(GxxValue::create(sNetCheckinTime), "data.nextCheckinTime");

	int nUsedMoney = (lMember.ntotalMoney - nBalance);
	outputInfo->setValueForKeyPath(GxxValue::create((long)nUsedMoney), "data.usedMoney");

	IBA_LOG(_T("Temp. CLocalCenter::HandleQueryBalance Send to UDO :\r\n \
				balance = %d \r\n \
				ConsumeTime = %d \r\n \
				ConsumeType = %d \r\n \
				usedMoney = %d \r\n \
				for MemberId = %d"),nBalance,nConsumeTime,lMember.nPayType,nUsedMoney,lMember.nMemberId);

	//�������ָ���,��ʼ�ϱ�����ģʽ����
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal) 
	{
		//���ؿ�����û���¼.�޼�¼�Ͳ����ϱ�.
		CLocalMember lMember;
		if(FALSE == CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberID,lMember))
		{
			this->PutDataCode(outputInfo, DATA_CODE_NO_ERROR);
			this->OutputInfo(pServer, pClient, outputInfo);
			return;
		}

		//�ϱ����س�ֵ
		CString sql;
		sql.Format(_T("select * from localMember where memberId=%d and isReturned=0"), nMemberID);
		GxxDictionaryPtr memberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);
		CCashierHelper::ReportLocalCredit(memberPtr);

		//����ģʽ��½�����ĵ�½
		if(lMember.nIsLocalCheckin)//����ģʽ��¼
		{
			//�ϱ�������Ϣ
			CCashierHelper::ReportLocalConsume(lMember);
			//�����û�
			CActivationMethod Activation;

			Activation.SetMemberId(lMember.nMemberId);
			Activation.SetMode( CActivationMethod::ACTIVATION_YES);
			//Activation.SetDotType(nActivationType);
			if(FALSE == theApp.GetCurCashier()->DoActivation(Activation))
			{
				IBA_LOG(_T("Error.�ϱ������û�.����ʧ��(����ģʽ��½).MemberId:%d,ErrorCode:%d,ErrorMsg:%s"),lMember.nMemberId,Activation.GetStatusCode(),Activation.GetStatusMessage());
			}
		}
		else//����ģʽ��½
		{
			if(CNetBarConfig::GetInstance()->GetIsMember(lMember.nClassId))//��Ա-�˹��ǳ�,Ȼ�󼤻�
			{
				CCheckout2Method Checkout2;
				Checkout2.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
				Checkout2.SetSafeLogout(0);
				Checkout2.SetMemberId(lMember.nMemberId);	
				Checkout2.SetOperator(theApp.GetCurCashier()->GetName());

				if(FALSE == Checkout2.ExecuteCheckout2())
				{
					IBA_LOG(_T("Error.�ϱ������û�.Ϊ���߻�Ա�˹��ǳ�ʧ��.MemberId:%d,ErrorCode:%d,ErrorMsg:%s"),lMember.nMemberId,Checkout2.GetStatusCode(),Checkout2.GetStatusMessage());
				}				

				//�����û�
				CActivationMethod Activation;

				Activation.SetMemberId(lMember.nMemberId);
				Activation.SetMode( CActivationMethod::ACTIVATION_YES);
				//Activation.SetDotType(nActivationType);
				if(FALSE == theApp.GetCurCashier()->DoActivation(Activation))
				{
					IBA_LOG(_T("Error.�ϱ������û�.����ʧ��(����ģʽ��½).MemberId:%d,ErrorCode:%d,ErrorMsg:%s"),lMember.nMemberId,Checkout2.GetStatusCode(),Checkout2.GetStatusMessage());
				}
			}
		}

		//��ѯ���
		CIDCheckCardMethod CheckCard;
		CString strTmp = lMember.strNetId;
		IBA_LOG(_T("strTmp = %s"),strTmp);
		if (strTmp.GetLength() < 10)
		{
			strTmp.Insert(0, CString(_T("0000000000")).Left(10 - strTmp.GetLength()));
		}
		CheckCard.SetSerialNum(strTmp);
		theApp.GetCurCashier()->DoCheckCard(CheckCard);

		if(CheckCard.GetStatusCode() == 0)
		{
			int nCardId = CheckCard.GetCardId();
			CBalanceMethod Balance;
			Balance.SetMemberId(lMember.nMemberId);
			Balance.SetCardId(nCardId);
			//Balance.SetRound(0);

			theApp.GetCurCashier()->DoBalance(Balance);
		}	

		//ɾ���û���¼
		if(FALSE == CBrokenNetwork::RemoveLocalMember(lMember.nMemberId))
		{
			IBA_LOG(_T("Serious Error!ɾ�����ؿ��¼ʧ��!�����˳�."));
			TerminateProcess(GetCurrentProcess(),0);
		}

		//��֪�ͻ����Ѿ��ϴ�
		outputInfo->setValueForKeyPath(GxxValue::create((long)1), "data.centerResumed");	

		////��������û��Ƿ��ϱ����.���������bHasResumedForOnlineUser,�Ա���UDO��������ģʽ
		//int nCount = CIBADAL::GetInstance()->GetCountOfOnlineUser();
		//if(0 == nCount)
		//{
		//	bHasResumedForOnlineUser = TRUE;
		//}
		
		//��鱾�ؿ�,��û�м�¼��,�͵����ϱ������־
		GxxArrayPtr localArrayPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(_T("select * from localMember"));
		if(0 == localArrayPtr->count())
		{
			COperationInfo OperationInfo ;
			OperationInfo.SetDescription (_T("�ϱ�����ģʽ�������!"));
			SendMessage(CMainFrame ::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM )(&OperationInfo));	
		}
	}
	//������Σ����Ƿ����޴���
	this->PutDataCode(outputInfo, DATA_CODE_NO_ERROR);
	this->OutputInfo(pServer, pClient, outputInfo);
	OutputDebugString(_T("\n\n================end\n"));
}

/************************************************************************/
/* �ϱ���¼״̬                                                                     */
/************************************************************************/
void CLocalCenter::HandleReportLogon( GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient )
{
	IBA_LOG(_T("Start HandleReportLogon"));
	// �жϲ����Ƿ����
	if (
		!reqData->isKeyExist("account") || 
		!reqData->isKeyExist("memberID") || 
		!reqData->isKeyExist("p") ||
		!reqData->isKeyExist("costRate") ||
		!reqData->isKeyExist("classID") ||
		!reqData->isKeyExist("termID") ||
		!reqData->isKeyExist("pcClass")) 
	{
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_PARAM);
		return;
	}

	//�˺�
	CGxxString<TCHAR> strAccount = CA2T(reqData->valueForKey("account")->stringValue());
	strAccount.FillCharFromLeft(10, '0');	
	//memberId
	int nMemberId = reqData->valueForKey("memberID")->intValue();
	IBA_LOG(_T("HandleReportLogon: Get memberId = %d"),nMemberId);
	//����
	CString strPassword = CA2T(reqData->valueForKey("p")->stringValue());
	//�ϱ�ʱ��
	CString strReportTime = CIBAHelpper::GetCurrentTimeAsString(TRUE);
	//����
	CString strCostRate = CA2T(reqData->valueForKey("costRate")->stringValue());
	IBA_LOG(_T("HandleReportLogon: Get Costrate = %s"),strCostRate);
	//��Ա����
	int nClassId = reqData->valueForKey("classID")->intValue();
	IBA_LOG(_T("HandleReportLogon: Get nClassId = %d"),nClassId);
	//��������
	int nPcClass = reqData->valueForKey("pcClass")->intValue();
	IBA_LOG(_T("HandleReportLogon: Get nPcClass = %d"),nPcClass);
	//�ն˺�
	CString strTermID = CA2T(reqData->valueForKey("termID")->stringValue());

	BOOL bResult = TRUE;
	
	// ���ϱ����и��»��������
	CString sql;
	sql.Format(_T("select count(*) from localreport where memberId=%d"), nMemberId);
	if (CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) > 0) 
	{
		sql.Format(_T("update localreport set password='%s',reportTime='%s' where memberId=%d"), 
			strPassword, strReportTime, nMemberId);
	} 
	else 
	{
		sql.Format(_T("insert into localreport (memberId,netId,password,reportTime) values(%d,'%s','%s','%s')"), 
			nMemberId, strAccount.GetData(), strPassword, strReportTime);
	}
	if (CIBADAL::GetInstance()->ExecuteSQL(sql) != 1) 
	{
		CIBALog::GetInstance()->WriteFormat(_T("Error.�ϱ���½��ϢHandleReportLogonʧ��.%s"),sql);
		bResult = FALSE;
	}

	// �����ʱ��и�������
	/*if (!strCostRate.IsEmpty())
	{
		sql.Empty();
		sql.Format(_T("select count(*) from CostRate where  pcClass=%d and classId=%d"), nPcClass, nClassId);
		if (CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) > 0)
		{
			sql.Empty();
			sql.Format(_T("update CostRate set rate='%s' where  pcClass=%d and classId=%d"), strCostRate, nPcClass, nClassId);
			IBA_LOG(_T("HandleReportLogon Update costRate:%s"),sql);
			if (CIBADAL::GetInstance()->ExecuteSQLEx(sql) < 0) 
			{
				IBA_LOG(_T("�ϱ���½��ϢHandleReportLogon����CostRateʧ��,%s"),sql);
				bResult = FALSE;
			}
		} 
		else 
		{
			sql.Empty();
			sql.Format(_T("insert into CostRate (pcClass, classId, rate) values(%d,%d,'%s')"), nPcClass, nClassId, strCostRate);
			IBA_LOG(_T("HandleReportLogon Insert costRate:%s"),sql);
			if (CIBADAL::GetInstance()->ExecuteSQLEx(sql) != 1) 
			{				
				IBA_LOG(_T("�ϱ���½��ϢHandleReportLogonʧ������CostRateʧ��.%s"),sql);
				bResult = FALSE;
			}
		}

		CActiveMember ActiveMember;
		if(!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember))
		{
			IBA_LOG(_T("Abnormal! Cant find ActiveMember in ActiveMemberList for MemberId = %d"),nMemberId);
		}
		else
		{
			ActiveMember.SetPolicyInfo(strCostRate); 
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(false);
		}
	}

	if (!bResult)
	{
		IBA_LOG(_T("Abnormal.HandleReportLogonʧ��!"));
	}*/

	GxxDictionaryPtr outputInfo = GxxDictionary::create();

	// ��Զ����UDO�ϱ��ɹ�, UDO�����ϱ��Ľ��������
	this->PutDataCode(outputInfo, DATA_CODE_NO_ERROR);

	// �������ݸ��ͻ���
	this->OutputInfo(pServer, pClient, outputInfo);
}

/************************************************************************/
/* sql����ѯ����                                                                    */
/************************************************************************/
void CLocalCenter::HandleSqlQuery( GxxDictionaryPtr reqData, CG2XSocketServer* pServer, PER_SOCKET_CONTEXT* pClient )
{
	IBA_LOG(_T("Start HandleSqlQuery"));
	// �жϲ����Ƿ����
	if (!reqData->isKeyExist("sql")) 
	{
		this->OutputErrorInfo(pServer, pClient, ERROR_CODE_PARAM);
		return;
	}

	GxxDictionaryPtr dataPtr = GxxDictionary::create();

	GxxArrayPtr recordsPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(CA2T(reqData->valueForKey("sql")->stringValue()));

	dataPtr->setValueForKey(GxxValue::create((long)recordsPtr->count()), "count");
	dataPtr->setValueForKey(GxxValue::create(recordsPtr), "records");

	GxxDictionaryPtr outputInfo = GxxDictionary::create();
	outputInfo->setValueForKey(ToUtf8Value(GetDataCodeErrorMessage(DATA_CODE_NO_ERROR).c_str()), "message");
	outputInfo->setValueForKey(GxxValue::create(dataPtr), "data");

	this->OutputInfo(pServer, pClient, outputInfo);

}


GxxValuePtr CLocalCenter::ToUtf8Value( LPCWSTR lpText )
{
	return GxxValue::create(G2XUtility::CG2XCommonTool::WideChar_to_Utf8(std::wstring(lpText)).c_str());
}

std::wstring CLocalCenter::GetDataCodeErrorMessage( long nDataCodeError )
{
	std::wstring msg;
	switch(nDataCodeError){

		case DATA_CODE_NO_ERROR:
			msg = L"����ɹ�";
			break;

		case DATA_CODE_INVALID_USER:
			msg = L"��Ч�û�";
			break;

		case DATA_CODE_INVALID_PASSWORD:
			msg = L"�˺ź�������Ч";
			break;

		case DATA_CODE_LOCAL_CHECKOUT:
			msg = L"�����ѽ��ˣ���δ�ϱ�����֮ǰ�����ϻ�";
			break;

		case DATA_CODE_LOCAL_NO_ACTIVE:
			msg = L"����δ����";
			break;

		case DATA_CODE_NO_USER_INFO:
			msg = L"����ȱ���û���Ϣ";
			break;

		case DATA_CODE_NO_USER_RATE:
			msg = L"��Ч�û���ȱ�ٿ�����Ϣ";
			break;

		case DATA_CODE_DATABASE_ERROR:
			msg = L"���ݿ��쳣";
			break;

		case DATA_CODE_NO_BALANCE:
			msg = L"����";
			break;

		case DATA_CODE_CHANGE_IN_LOCALMODE:
			msg = L"����ģʽ�²�������";
			break;

		default:
			msg = L"δ֪����";
			break;
	}

	return msg;
}

void CLocalCenter::PutDataCode( GxxDictionaryPtr& outputInfo, long nDataCodeError )
{
	outputInfo->setValueForKey(ToUtf8Value(GetDataCodeErrorMessage(nDataCodeError).c_str() ), "message");

	if (!outputInfo->isKeyExist("data")) 
	{
		outputInfo->setValueForKey(GxxValue::create(GxxDictionary::create()), "data");
	}
	outputInfo->setValueForKeyPath(GxxValue::create(nDataCodeError), "data.code");
}

BOOL CLocalCenter::IsBundleTimeEnd( GxxDictionaryPtr& memberPtr )
{
	if (memberPtr->intValueForKey("payType") == 0) 
	{
		// �ǰ�ʱ���϶��ǰ�ʱ�ѽ���
		return TRUE;
	}

	time_t nextCheckinTime = memberPtr->intValueForKey("nextCheckinTime");
	time_t now = time(NULL);

	return now > nextCheckinTime;
}


