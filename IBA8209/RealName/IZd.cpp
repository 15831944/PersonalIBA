
#include "stdafx.h"
#include "IZd.h"
#include "..\IBA.h"
#include "..\Encrypt.h"
#include "..\IBAHelpper.h"
#include <atlenc.h>

std::map<int, std::string> CIZd::m_mapI2S;
SINGLETON_IMPLEMENT(CIZd)

CIZd::CIZd()
: m_pfn_V2_OpenUserEx(NULL),
m_hIKCardID(NULL), 
m_pfn_V2_Login(NULL),
m_pfn_V2_LoginEx(NULL),
m_pfn_V2_Logout(NULL),
m_pfn_V2_AddInsider(NULL),
m_pfn_V2_LockMac(NULL),
m_pfn_V2_SendICCardData(NULL),
m_pfn_V2_SendDeductData(NULL),
m_pfn_V2_VerifyCard(NULL),
m_pfn_V2_GetCardPWD(NULL),
m_pfn_V2_GetCardPWDEx(NULL),
m_pfn_V2_GetCustomer(NULL),
m_pfn_V2_GetMobileVerificationCode(NULL),
m_pfn_V2_GetMobileVerificationCodeReturn(NULL),
m_pfn_V2_WXAccounts(NULL),
m_pfn_V2_KEYBalance(NULL),
m_pfn_V2_UseMacVerfy(NULL),
m_pfn_V2_SetCRC(NULL),
m_bLoadSuccess(FALSE),
m_bInitSuccess(FALSE)
{
	IBA_LOG0(_T("CIZd ���� 2"));
	// 2014-10-27 - qsc
	FmtErrorString();
	Load();
}

CIZd::~CIZd()
{
	IBA_LOG0(_T("CIZd ����"));
	if(m_hIKCardID != NULL)
	{
		::FreeLibrary(m_hIKCardID);
		m_hIKCardID = NULL;
	}
}

BOOL CIZd::Load()
{
	IBA_LOG0(_T("Enter CIZd Loads"));

	CString tmpAppPath = theApp.GetWorkPath() + _T("\\IBABin\\");
	CString tmpLoadDllPath = tmpAppPath + _T("IKCardID.dll");
	m_hIKCardID = ::LoadLibrary(tmpLoadDllPath);

	BOOL bSuccess = FALSE;
	if (m_hIKCardID == NULL)
	{
		IBA_LOG0(_T("CIZd Loads Load IKCardID.dll ʧ��"));
		bSuccess = FALSE;
	}
	else
	{
		m_pfn_V2_OpenUserEx			= (FUN_V2_OpenUserEx)::GetProcAddress(m_hIKCardID, "V2_OpenUserEx");
		m_pfn_V2_Login				= (FUN_V2_Login)::GetProcAddress(m_hIKCardID, "V2_Login");
		m_pfn_V2_LoginEx			= (FUN_V2_LoginEx)::GetProcAddress(m_hIKCardID, "V2_LoginEx");
		m_pfn_V2_Logout				= (FUN_V2_Logout)::GetProcAddress(m_hIKCardID, "V2_Logout");
		m_pfn_V2_AddInsider			= (FUN_V2_AddInsider)::GetProcAddress(m_hIKCardID, "V2_AddInsider");
		m_pfn_V2_LockMac			= (FUN_V2_LockMac)::GetProcAddress(m_hIKCardID, "V2_LockMac");
		m_pfn_V2_SendICCardData		= (FUN_V2_SendICCardData)::GetProcAddress(m_hIKCardID, "V2_SendICCardData");
		m_pfn_V2_SendDeductData		= (FUN_V2_SendDeductData)::GetProcAddress(m_hIKCardID, "V2_SendDeductData");
		m_pfn_V2_VerifyCard			= (FUN_V2_VerifyCard)::GetProcAddress(m_hIKCardID, "V2_VerifyCard");
		m_pfn_V2_GetCardPWD			= (FUN_V2_GetCardPWD)::GetProcAddress(m_hIKCardID, "V2_GetCardPWD");
		m_pfn_V2_GetCardPWDEx		= (FUN_V2_GetCardPWDEx)::GetProcAddress(m_hIKCardID, "V2_GetCardPWDEx");
		m_pfn_V2_GetCustomer		= (FUN_V2_GetCustomer)::GetProcAddress(m_hIKCardID, "V2_GetCustomer");

		m_pfn_V2_GetMobileVerificationCode			= (FUN_V2_GetMobileVerificationCode)::GetProcAddress(m_hIKCardID, "V2_GetMobileVerificationCode");
		m_pfn_V2_GetMobileVerificationCodeReturn	= (FUN_V2_GetMobileVerificationCodeReturn)::GetProcAddress(m_hIKCardID, "V2_GetMobileVerificationCodeReturn");
		m_pfn_V2_WXAccounts			= (FUN_V2_WXAccounts)::GetProcAddress(m_hIKCardID, "V2_WXAccounts");
		m_pfn_V2_KEYBalance			= (FUN_V2_KEYBalance)::GetProcAddress(m_hIKCardID, "V2_KEYBalance");
		m_pfn_V2_UseMacVerfy		= (FUN_V2_UseMacVerfy)::GetProcAddress(m_hIKCardID, "V2_UseMacVerfy");
		m_pfn_V2_CloseUser			= (FUN_V2_CloseUser)::GetProcAddress(m_hIKCardID, "V2_CloseUser");
		m_pfn_V2_SetCRC				= (FUN_V2_SetCRC)::GetProcAddress(m_hIKCardID, "V2_SetCRC");

		// QSCTAG
		/*	if(!m_pfn_V2_SendICCardData)
		{
			IBA_LOG0(_T("m_pfn_V2_SendICCardDataΪ��"));
		}
		if(!m_pfn_V2_UseMacVerfy)
		{
			IBA_LOG0(_T("m_pfn_V2_UseMacVerfyΪ��"));
		}*/
		
		if(!m_pfn_V2_OpenUserEx)
		{
			IBA_LOG0(_T("m_pfn_V2_OpenUserEx��ȡʧ��"));
		}
		if(!m_pfn_V2_Login)
		{
			IBA_LOG0(_T("m_pfn_V2_Login��ȡʧ��"));
		}
		if(!m_pfn_V2_LoginEx)
		{
			IBA_LOG0(_T("m_pfn_V2_LoginEx��ȡʧ��"));
		}
		if(!m_pfn_V2_Logout)
		{
			IBA_LOG0(_T("m_pfn_V2_Logout��ȡʧ��"));
		}
		if(!m_pfn_V2_AddInsider)
		{
			IBA_LOG0(_T("m_pfn_V2_AddInsider��ȡʧ��"));
		}
		if(!m_pfn_V2_LockMac)
		{
			IBA_LOG0(_T("m_pfn_V2_LockMac��ȡʧ��"));
		}
		if(!m_pfn_V2_SendICCardData)
		{
			IBA_LOG0(_T("m_pfn_V2_SendICCardData��ȡʧ��"));
		}
		if(!m_pfn_V2_UseMacVerfy)
		{
			IBA_LOG0(_T("m_pfn_V2_UseMacVerfy��ȡʧ��"));
		}
		if(!m_pfn_V2_SendDeductData)
		{
			IBA_LOG0(_T("m_pfn_V2_SendDeductData��ȡʧ��"));
		}
		if(!m_pfn_V2_VerifyCard)
		{
			IBA_LOG0(_T("m_pfn_V2_VerifyCard��ȡʧ��"));
		}
		if(!m_pfn_V2_GetCardPWD)
		{
			IBA_LOG0(_T("m_pfn_V2_GetCardPWD��ȡʧ��"));
		}
		if(!m_pfn_V2_GetCardPWDEx)
		{
			IBA_LOG0(_T("m_pfn_V2_GetCardPWDEx��ȡʧ��"));
		}
		if(!m_pfn_V2_GetCustomer)
		{
			IBA_LOG0(_T("m_pfn_V2_GetCustomer��ȡʧ��"));
		}
		if(!m_pfn_V2_GetMobileVerificationCode)
		{
			IBA_LOG0(_T("m_pfn_V2_GetMobileVerificationCode��ȡʧ��"));
		}
		if(!m_pfn_V2_GetMobileVerificationCodeReturn)
		{
			IBA_LOG0(_T("m_pfn_V2_GetMobileVerificationCodeReturn��ȡʧ��"));
		}
		if(!m_pfn_V2_WXAccounts)
		{
			IBA_LOG0(_T("m_pfn_V2_WXAccounts��ȡʧ��"));
		}
		if(!m_pfn_V2_KEYBalance)
		{
			IBA_LOG0(_T("m_pfn_V2_KEYBalance��ȡʧ��"));
		}
		if(!m_pfn_V2_CloseUser)
		{
			IBA_LOG0(_T("m_pfn_V2_CloseUser��ȡʧ��"));
		}
		if(!m_pfn_V2_SetCRC)
		{
			IBA_LOG0(_T("m_pfn_V2_SetCRC��ȡʧ��"));
		}

		if(m_pfn_V2_OpenUserEx			!= NULL &&
			m_pfn_V2_Login				!= NULL &&
			m_pfn_V2_LoginEx			!= NULL &&
			m_pfn_V2_Logout				!= NULL &&
			m_pfn_V2_AddInsider			!= NULL &&
			m_pfn_V2_LockMac			!= NULL &&
			//m_pfn_V2_SendICCardData		!= NULL &&
			//m_pfn_V2_UseMacVerfy		!= NULL &&
			m_pfn_V2_SendDeductData		!= NULL &&
			m_pfn_V2_VerifyCard			!= NULL &&
			m_pfn_V2_GetCardPWD			!= NULL &&
			m_pfn_V2_GetCardPWDEx		!= NULL &&
			m_pfn_V2_GetCustomer		!= NULL &&
			m_pfn_V2_GetMobileVerificationCode			!= NULL &&
			m_pfn_V2_GetMobileVerificationCodeReturn	!= NULL &&
			m_pfn_V2_WXAccounts			!= NULL &&
			m_pfn_V2_KEYBalance			!= NULL && 
			m_pfn_V2_CloseUser			!= NULL &&
			m_pfn_V2_SetCRC				!= NULL)
		{
			IBA_LOG0(_T("���سɹ�"));
			m_bLoadSuccess = TRUE;
		}
		else
		{
			IBA_LOG0(_T("�к�����ȡʧ��"));
			m_bLoadSuccess = FALSE;
		}
	}
	IBA_LOG0(_T("Leave CIZd Loads"));
	return m_bLoadSuccess;
}

void CIZd::Conver_IC_USER( const IC_USER* pOrg, T_IC_USER *pDest )
{
	ZeroMemory(pDest, sizeof(IC_USER));
	_tcsncpy(pDest->szikIDNo, CA2T(pOrg->szikIDNo), 21);
	_tcsncpy(pDest->szikName, CA2T(pOrg->szikName), 31);
	_tcsncpy(pDest->szCertNumber, CA2T(pOrg->szCertNumber), 19);
	_tcsncpy(pDest->szMobile, CA2T(pOrg->szMobile), 12);

	pDest->iCertType = pOrg->iCertType;
	pDest->iCardType = pOrg->iCardType;
	pDest->iValidate = pOrg->iValidate;
}

void CIZd::Conver_IC_LOGIN( IC_LOGIN* pOrg, T_IC_LOGIN *pDest )
{
	ZeroMemory(pDest, sizeof(IC_USER));
	_tcsncpy(pDest->szikICNo, CA2T(pOrg->szikICNo), 21);
	_tcsncpy(pDest->szikIDNo, CA2T(pOrg->szikIDNo), 21);
	_tcsncpy(pDest->szikIP, CA2T(pOrg->szikIP), 16);
	_tcsncpy(pDest->szikHost, CA2T(pOrg->szikHost), 31);
	_tcsncpy(pDest->szicPassword, CA2T(pOrg->szicPassword), 16);

}

CIZd::ID_LOGINRETURN CIZd::V2_LoginEx( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szPwd, LPCSTR szIP)
{
	IBA_LOG0(_T("V2_LoginEx Enter"));
	ID_LOGINRETURN loginReturn;
	ZeroMemory(&loginReturn, sizeof(ID_LOGINRETURN));

	if(IsInit())
	{
		char szICNOTmp[21] = {0};
		char szIDNOTmp[21] = {0};
		char szPwdTmp[21] = {0};
		char szIPTmp[30] = {0};
		strncpy(szICNOTmp, szICNO, 21);
		strncpy(szIDNOTmp, szIDNO, 21);
		strncpy(szPwdTmp, szPwd, 21);
		strncpy(szIPTmp, szIP, 30);

		CStringA strLog;
		strLog.Format("���ݸ��Ƕ����Ϣ%s,%s,PWD:%s,%s", szICNOTmp, szIDNOTmp, szPwdTmp, szIPTmp);
		IBA_LOG0(CA2T(strLog));

		loginReturn =  m_pfn_V2_LoginEx(szICNOTmp, szIDNOTmp, szPwdTmp, szIPTmp);
		DealErrorMsg(loginReturn.iErrCode, loginReturn.szErrMsg);

		strLog.Format("��������:%s, ��������ȡ���ͣ�%d, ����%s, ֤�����ͣ�%d, ֤�����룺%s, ����ţ�%d, ��������:%s", 
			loginReturn.szCardId, loginReturn.iCardType, loginReturn.szName, loginReturn.iCertType, loginReturn.szCertNumber, 
			loginReturn.iErrCode, loginReturn.szErrMsg);
		IBA_LOG0(CA2T(strLog));
#ifdef _DEBUG
		loginReturn.iErrCode = 0;
#endif
	}
	else
	{
		IBA_LOG(_T("δ��ʼ��"));
		loginReturn.iErrCode = -1;
		strcpy(loginReturn.szErrMsg, "�Ƕ�ʵ��δ��ʼ��");
	}
	IBA_LOG0(_T("V2_LoginEx Leave"));
	return loginReturn;
}

CIZd::ID_RETURN CIZd::V2_Logout( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szIP)
{
	IBA_LOG0(_T("V2_Logout Enter"));
	ID_RETURN idReturn;
	ZeroMemory(&idReturn, sizeof(idReturn));

	if(IsInit())
	{
		char szICNOTmp[21] = {0};
		char szIDNOTmp[21] = {0};
		char szIPTmp[30] = {0};
		strncpy(szICNOTmp, szICNO, 21);
		strncpy(szIDNOTmp, szIDNO, 21);
		strncpy(szIPTmp, szIP, 30);

		CStringA strLog;
		strLog.Format("���ݸ��Ƕ����Ϣ%s,%s,%s", szICNO, szICNO, szIP);
		IBA_LOG0(CA2T(strLog));

		idReturn =  m_pfn_V2_Logout(szICNOTmp, szIDNOTmp, szIPTmp);
		DealErrorMsg(idReturn.iErrCode, idReturn.szErrMsg);

		strLog.Format("����:%d, ����������%s", 
			idReturn.iErrCode, idReturn.szErrMsg);
		IBA_LOG0(CA2T(strLog));
	}
	else
	{
		IBA_LOG(_T("δ��ʼ��"));
		idReturn.iErrCode = -1;
		strcpy(idReturn.szErrMsg, "�Ƕ�ʵ��δ��ʼ��");
	}
	IBA_LOG0(_T("V2_Logout Leave"));
	return idReturn;
}

CIZd::ID_GETCARDPWD CIZd::V2_GetCardPWDEx(LPCSTR szIDNO)
{
	IBA_LOG0(_T("V2_GetCardPWDEx Enter"));
	ID_GETCARDPWD cardPwd;
	ZeroMemory(&cardPwd, sizeof(cardPwd));

	if(IsInit())
	{
		char szIDNOTmp[21] = {0};
		strncpy(szIDNOTmp, szIDNO, 21);

		CStringA strLog;
		strLog.Format("���ݸ��Ƕ����Ϣ%s", szIDNOTmp);
		IBA_LOG0(CA2T(strLog));

		cardPwd =  m_pfn_V2_GetCardPWDEx(szIDNOTmp);
		DealErrorMsg(cardPwd.iErrCode, cardPwd.szErrMsg);

		strLog.Format("����:%d, ����������%s", 
			cardPwd.iErrCode, cardPwd.szErrMsg);
		IBA_LOG0(CA2T(strLog));
	}
	else
	{
		IBA_LOG(_T("δ��ʼ��"));
		cardPwd.iErrCode = -1;
		strcpy(cardPwd.szErrMsg, "�Ƕ�ʵ��δ��ʼ��");
	}
	IBA_LOG0(_T("V2_GetCardPWDEx Leave"));
	return cardPwd;
}

CIZd::ID_RETURN CIZd::V2_CloseUser( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szIP, LPCSTR szTermId)
{
	IBA_LOG0(_T("V2_CloseUser Enter"));
	ID_RETURN idReturn;
	ZeroMemory(&idReturn, sizeof(idReturn));

	if(IsInit())
	{
		char szICNOTmp[21] = {0};
		char szIDNOTmp[21] = {0};
		char szIPTmp[30] = {0};
		char szTermId[20] = {0};
		char szCurTime[15] = {0}; 

		CTime tm  = CTime::GetCurrentTime();
		CStringA strCurTime;
		strCurTime.Format("%04d%02d%02d%02d%02d%02d", 
			tm.GetYear(), tm.GetMonth(), tm.GetDay(),
			tm.GetHour(), tm.GetMinute(), tm.GetSecond());

		strncpy(szICNOTmp, szICNO, 21);
		strncpy(szIDNOTmp, szIDNO, 21);
		strncpy(szIPTmp, szIP, 30);
		strncpy(szTermId, szTermId, 20);
		strncpy(szCurTime, strCurTime, 15);
		
		CStringA strLog;
		strLog.Format("���ݸ��Ƕ����Ϣ%s,%s,%s,%s", szICNO, szICNO, szIP, szTermId);
		IBA_LOG0(CA2T(strLog));

		idReturn =  m_pfn_V2_CloseUser(szICNOTmp, szIDNOTmp, szIPTmp, szTermId, szCurTime);
		DealErrorMsg(idReturn.iErrCode, idReturn.szErrMsg);

		strLog.Format("����:%d, ����������%s",
			idReturn.iErrCode, idReturn.szErrMsg);
		IBA_LOG0(CA2T(strLog));
	}
	else
	{
		IBA_LOG(_T("δ��ʼ��"));
		idReturn.iErrCode = -1;
		strcpy(idReturn.szErrMsg, "�Ƕ�ʵ��δ��ʼ��");
	}
	IBA_LOG0(_T("V2_CloseUser Leave"));
	return idReturn;
}

CIZd::ID_LOGINRETURN CIZd::V2_OpenUserEx( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szPwd, LPCSTR szIP, LPCSTR szDeductDotSign)
{
	IBA_LOG0(_T("V2_OpenUserEx Enter"));
	ID_LOGINRETURN loginReturn;
	ZeroMemory(&loginReturn, sizeof(ID_LOGINRETURN));

	if(IsInit())
	{
		char szICNOTmp[21] = {0};
		char szIDNOTmp[21] = {0};
		char szPwdTmp[21] = {0};
		char szIPTmp[30] = {0};
		char szCurTime[15] = {0};
		char szDeductDotSignTmp[20] = {0};
		char szCashNum[20] = {0};

		CTime tm  = CTime::GetCurrentTime();
		CStringA strCurTime;
		strCurTime.Format("%04d%02d%02d%02d%02d%02d", 
			tm.GetYear(), tm.GetMonth(), tm.GetDay(),
			tm.GetHour(), tm.GetMinute(), tm.GetSecond());

		CStringA strCashNum;
		strCashNum.Format("%d", CIBAConfig::GetInstance()->GetCashRegisterID());

		strncpy(szICNOTmp, szICNO, 21);
		strncpy(szIDNOTmp, szIDNO, 21);
		strncpy(szIPTmp, szIP, 30);
		strncpy(szPwdTmp, szPwd, 21);
		strncpy(szCurTime, strCurTime, 15);
		strncpy(szDeductDotSignTmp, szDeductDotSign, 20);
		strncpy(szCashNum, strCashNum, 20);

		CStringA strLog;
		strLog.Format("���ݸ��Ƕ����Ϣ%s,%s,PWD:%s,%s,%s,%s,%s", szICNOTmp, szIDNOTmp, szPwdTmp, szIPTmp, szDeductDotSignTmp, szCurTime, szCashNum);
		IBA_LOG0(CA2T(strLog));

		loginReturn =  m_pfn_V2_OpenUserEx(szICNOTmp, szIDNOTmp, szPwdTmp, szIPTmp, szDeductDotSignTmp, szCurTime, szCashNum);
		DealErrorMsg(loginReturn.iErrCode, loginReturn.szErrMsg);

		strLog.Format("��������:%s, ��������ȡ���ͣ�%d, ����%s, ֤�����ͣ�%d, ֤�����룺%s, ����ţ�%d, ��������:%s", 
			loginReturn.szCardId, loginReturn.iCardType, loginReturn.szName, loginReturn.iCertType, loginReturn.szCertNumber, 
			loginReturn.iErrCode, loginReturn.szErrMsg);
		IBA_LOG0(CA2T(strLog));

#ifdef _DEBUG
		loginReturn.iErrCode = 0;
#endif
	}
	else
	{
		IBA_LOG(_T("δ��ʼ��"));
		loginReturn.iErrCode = -1;
		strcpy(loginReturn.szErrMsg, "�Ƕ�ʵ��δ��ʼ��");
	}
	IBA_LOG0(_T("V2_OpenUserEx Leave"));
	return loginReturn;
}

CIZd::ID_ADDINSIDR CIZd::V2_AddInsider( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szPwd)
{
	IBA_LOG0(_T("V2_AddInsider Enter"));
	ID_ADDINSIDR addInsidr;
	ZeroMemory(&addInsidr, sizeof(addInsidr));

	TCHAR szikICNo[21];			//�󶨿����
	TCHAR szikIDNo[21];			//ʵ�������
	TCHAR szikName[31]; 		//����
	int   iIDCardType;			//֤������
	TCHAR szCertificateID[19];	//֤������
	int   iErrCode;				//����ţ�����������0��ʾ�ɹ�
	TCHAR szErrMsg[101];		//�������������Error��Ϊ0�Ļ�
	int   iCardGetType;			//ˢ֤����
	int   iIsCheck;				//�Ƿ���� 0��ʾû��1����ʾδ���2��ʾͨ��

	if(IsInit())
	{
		char szICNOTmp[21] = {0};
		char szIDNOTmp[21] = {0};
		char szPwdTmp[21] = {0};
		strncpy(szICNOTmp, szICNO, 21);
		strncpy(szIDNOTmp, szIDNO, 21);
		strncpy(szPwdTmp, szPwd, 21);

		CStringA strLog;
		strLog.Format("���ݸ��Ƕ����Ϣ%s,%s", szICNO, szICNO);
		IBA_LOG0(CA2T(strLog));

		addInsidr =  m_pfn_V2_AddInsider(szICNOTmp, szIDNOTmp, szPwdTmp);
		DealErrorMsg(addInsidr.iErrCode, addInsidr.szErrMsg);

		strLog.Format("�󶨿����:%s, ʵ������ţ�%s, ����%s, ֤�����ͣ�%d, ֤�����룺%s, ����ţ�%d, ��������:%s, ˢ֤����:%d, �Ƿ����:%d", 
			addInsidr.szikICNo, addInsidr.szikIDNo, addInsidr.szikName, addInsidr.iIDCardType, addInsidr.szCertificateID, 
			addInsidr.iErrCode, addInsidr.szErrMsg, addInsidr.iCardGetType, addInsidr.iIsCheck);
		IBA_LOG0(CA2T(strLog));
	}
	else
	{
		IBA_LOG(_T("δ��ʼ��"));
		addInsidr.iErrCode = -1;
		strcpy(addInsidr.szErrMsg, "�Ƕ�ʵ��δ��ʼ��");
	}
	IBA_LOG0(_T("V2_AddInsider Leave"));
	return addInsidr;
}

bool CIZd::IsIDPwdCorrect(LPCSTR szID, LPCSTR szPwd)
{
	CIZd::ID_GETCARDPWD cardPwd = CIZd::GetInstance()->V2_GetCardPWDEx(szID);

	std::string strEncode = Encode(szPwd);
	CStringA strLog;
	strLog.Format("�����˺�:%s,���룺%s, �����%s, ZD��%s",
		szID, szPwd, strEncode.c_str(), cardPwd.szPWD);
	IBA_LOG0(CA2T(strLog));
	
	if(strEncode == cardPwd.szPWD)
	{
		IBA_LOG0(_T("��ȷ"));
		return true;
	}
	else
	{
		IBA_LOG0(_T("����"));
		return false;
	}
}

bool CIZd::IsErrorCodeExist(int nError)
{
	if(m_mapI2S.count(nError) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CIZd::DealErrorMsg(int nError, char *szError)
{
	if(IsErrorCodeExist(nError))
	{
		std::string strError;
		GetErrorString(nError, strError);
		strncpy(szError, strError.c_str(), 100);
	}
}

void CIZd::GetErrorString(int nError, std::string& strError)
{
	if(IsErrorCodeExist(nError))
	{
		strError = m_mapI2S[nError];
	}
	else
	{
		strError = ""; 
	}

}

void CIZd::FmtErrorString()
{
	m_mapI2S[-1]	 = "δ֪����";
	m_mapI2S[-3]	 = "�������ӿڲ����ڻ���������";
	m_mapI2S[-4]	 = "�������ӿ��Ѿ���ʼ��";
	m_mapI2S[-5]	 = "�������ӿ��Ѿ���ֹ";
	m_mapI2S[-9]	 = "��������кŲ���ȷ";
	m_mapI2S[-10]	 = "�����벻�淶";
	m_mapI2S[-11]	 = "��Ч������";
	m_mapI2S[-12]	 = "�汾����ȷ";
	m_mapI2S[-13]	 = "��Ч����";
	m_mapI2S[-14]	 = "�Ự����ʧЧ";
	m_mapI2S[-15]	 = "����������¼ʧ�ܣ���Ҫ����ע��";
	m_mapI2S[-17]	 = "�ÿ�����Ч����δע��";
	m_mapI2S[-18]	 = "�ÿ�����ע��";
	m_mapI2S[-19]	 = "�ÿ�����ͣ��";
	m_mapI2S[-20]	 = "�ÿ�����ע��";
	m_mapI2S[-21]	 = "�ÿ������ڸõ���ʹ��";
	m_mapI2S[-22]	 = "��������û��Ȩ��";
	m_mapI2S[-25]	 = "��Ч֤��";
	m_mapI2S[-26]	 = "��֤����ע��";
	m_mapI2S[-28]	 = "�ù˿Ͳ����������ʸ�";
	m_mapI2S[-29]	 = "����δע��";
	m_mapI2S[-30]	 = "�Ƿ��ĳ�������";
	m_mapI2S[-31]	 = "����δӪҵ";
	m_mapI2S[-32]	 = "MAC����";
	m_mapI2S[-33]	 = "���Ŵ���";
	m_mapI2S[-34]	 = "��������汾����ȷ";
	m_mapI2S[-35]	 = "������Ҫ���°�";
	m_mapI2S[-36]	 = "��������汾����";
	m_mapI2S[-1000]	 = "KEY�е�����";
	m_mapI2S[-1001]	 = "��Ų�ƥ��";
	m_mapI2S[-1002]	 = "�޷��������ܶ���";
	m_mapI2S[-1003]	 = "����ʧ��";
	m_mapI2S[-1004]	 = "��ֵ���к��ظ�";
	m_mapI2S[-1005]	 = "KEY״̬�쳣";
	m_mapI2S[-1006]	 = "�򿪼�������ʧ��";
	m_mapI2S[-1007]	 = "��������keyʧ��";
	m_mapI2S[-1008]	 = "����ʧ��";
	m_mapI2S[-1009]	 = "δ����KEY����KEYӲ������";
	m_mapI2S[-1010]	 = "�����쳣";
	m_mapI2S[-1011]	 = "�Ʒ�����汾�Ų�����Ҫ��";
	m_mapI2S[-1012]	 = "�������ڹ涨ʱ����δ��¼";
	m_mapI2S[-1013]	 = "�ն˻�δ����";
	m_mapI2S[-1014]	 = "�˿����ѳ����ն˻���Ȩ����";
	m_mapI2S[-1015]	 = "�ն˻���ȫ�汾�Ų�����Ҫ��";
	m_mapI2S[-1016]	 = "δ���ҵ�����������";
}


int CIZd::SwitchZd2Ddn(int nSwitch)
{
	int nRet = 99;
	switch(nSwitch)
	{
	case 92:
	case 111:
		nRet = 12;
		break;

	case 112:
	case 98:
	case 91:
		nRet = 11;
		break;
	
	case 113:
		nRet = 93;
		break;

	case 114:
		nRet = 90;
		break;

	case 123:
		nRet = 91;
		break;

	case 133:
		nRet = 15;
		break;
	
	case 96:
	case 414:
		nRet = 94;
		break;

	case 94:
		nRet = 20;
		break;

	default:
		nRet = 99;
	}
	return nRet;
	//DDN:11:���֤|12:����֤|15:ѧ��֤|20:��ʻ֤|21:�㶫��ס֤|
	//22:���ھ�ס֤|90:����֤|91:����֤|92:ʿ��֤|93:���ڱ�|94:����|95:̨��֤|96:����֤|99:����֤��|
	//ZD 111,'���֤'             
	//112,'��ʱ���֤'          
	//113,'���ڲ�'              
	//114,'����֤'              
	//123,'����֤'              
	//133,'ѧ��֤'              
	//414,'����'                
	//990,'����֤��'

	//�ֹ�¼�룺
	//91,"һ�����֤"
	//92,"�������֤"
	//93,"����֤"
	//94,"��ʻ֤"
	//95,"�л����񹲺͹���ᱣ�Ͽ�"
	//96,"�й�����"
	//97,"���⼰�۰�̨֤��"
	//98,"��ʱ���֤"
}


struct MD5_CTX
{
	ULONG num[2];			/* number of _bits_ handled mod 2^64 */
	ULONG buf[4];			/* scratch buffer */
	BYTE  input[64];		/* input buffer */
	BYTE  digest[16];		/* actual digest after MD5Final call */
};

void WINAPI MD5Init(MD5_CTX* lpCtx);
void WINAPI MD5Update(MD5_CTX* lpCtx, const void * lpBuf, unsigned int nLen);
void WINAPI MD5Final(MD5_CTX* lpCtx);
// ��MD5����BASE64����
std::string CIZd::Encode(LPCSTR szPWD)
{
	MD5_CTX ctx = {0};

	CStringA strTmp = szPWD;

	MD5Init(&ctx);

	MD5Update(&ctx, strTmp.GetBuffer(), strTmp.GetLength());

	MD5Final(&ctx);

	int iSize = 0;
	iSize = 16; //MD5�̶�����16λ
	int iLen = Base64EncodeGetRequiredLength(iSize, ATL_BASE64_FLAG_NOCRLF);
	LPSTR pData = new char[iLen+1];
	memset(pData, 0, iLen);
	Base64Encode((const BYTE *)ctx.digest, iSize, pData, &iLen, ATL_BASE64_FLAG_NOCRLF);
	
	char szTmp[64] = {0};
	strncpy(szTmp, pData, iLen);
	
	std::string stdRet = szTmp;
	return stdRet;
}
