#include "StdAfx.h"
#include "ThreadHelpper.h"
#include "NetBarConfig.h"
#include ".\Servlet\GetSystemConfigMethod.h"
#include "Servlet\CommonParam.h"
#include "IBALog.h"

using namespace NS_SERVLET;

void CThreadHelpper::StartThead(IN LPTHREAD_START_ROUTINE lpStartAddress)
{
	HANDLE hThread = CreateThread(NULL, 0, lpStartAddress, NULL, 0, 0);
	//SetThreadPriority(hThread,THREAD_PRIORITY_LOWEST);
	CloseHandle(hThread);
}

DWORD CThreadHelpper::GetSysConfig(LPVOID pParam)
{
	// ��ѯ�Ƿ�����΢��֧��
	CCommonParam commmonParam;	
	commmonParam.SetParam(_T("wxpay"));
	commmonParam.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());
	commmonParam.Execute();
	if(0 == commmonParam.GetStatusCode())
	{	
		CString strRequset = commmonParam.GetCommonRequest();
		UINT nRequest = _ttoi(strRequset);
		IBA_LOG(_T("�Ƿ�ͨ΢��֧��:%d"), nRequest);

		CNetBarConfig::GetInstance()->SetOpenWx(nRequest);
	}

	// ��ѯ�Ƿ����õ绰��֤
	commmonParam.SetParam(_T("telverify"));
	commmonParam.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());
	commmonParam.Execute();
	if(0 == commmonParam.GetStatusCode())
	{	
		CString strRequset = commmonParam.GetCommonRequest();
		UINT nRequest = _ttoi(strRequset);
		IBA_LOG(_T("���ĵ绰��֤����:%d"), nRequest);
		CNetBarConfig::GetInstance()->SetTelVerify(nRequest);
	}

	// ��ѯfileList��ַ���洢�����ݿ��
	CGetSystemConfigMethod getSystemConfigMethod;
	getSystemConfigMethod.SetStringField(_T("versionManageHost"));
	getSystemConfigMethod.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());
	getSystemConfigMethod.Execute();

	if(0 == getSystemConfigMethod.GetStatusCode())
	{
		CString strFieldValue = getSystemConfigMethod.GetStringFieldValue();
		UINT Value1 = getSystemConfigMethod.GetValue1();
		UINT Value2 = getSystemConfigMethod.GetValue2();
		CNetBarConfig::GetInstance()->SetSystemConfig(_T("versionManageHost"), 
			strFieldValue, Value1, Value2);
	}

	getSystemConfigMethod.SetStringField(_T("noticeServerAddress"));
	getSystemConfigMethod.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());
	getSystemConfigMethod.Execute();

	if(0 == getSystemConfigMethod.GetStatusCode())
	{
		CString strFieldValue = getSystemConfigMethod.GetStringFieldValue();
		UINT Value1 = getSystemConfigMethod.GetValue1();
		UINT Value2 = getSystemConfigMethod.GetValue2();
		CNetBarConfig::GetInstance()->SetSystemConfig(_T("noticeServerAddress"), 
			strFieldValue, Value1, Value2);
	}

	getSystemConfigMethod.SetStringField(_T("LoginQrCode"));
	getSystemConfigMethod.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());
	getSystemConfigMethod.Execute();

	if(0 == getSystemConfigMethod.GetStatusCode())
	{
		CString strFieldValue = getSystemConfigMethod.GetStringFieldValue();
		UINT Value1 = getSystemConfigMethod.GetValue1();
		UINT Value2 = getSystemConfigMethod.GetValue2();
		CNetBarConfig::GetInstance()->SetSystemConfig(_T("LoginQrCode"), 
			strFieldValue, Value1, Value2);
	}

	getSystemConfigMethod.SetStringField(_T("LogoutQrCode"));
	getSystemConfigMethod.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());
	getSystemConfigMethod.Execute();

	if(0 == getSystemConfigMethod.GetStatusCode())
	{
		CString strFieldValue = getSystemConfigMethod.GetStringFieldValue();
		UINT Value1 = getSystemConfigMethod.GetValue1();
		UINT Value2 = getSystemConfigMethod.GetValue2();
		CNetBarConfig::GetInstance()->SetSystemConfig(_T("LogoutQrCode"), 
			strFieldValue, Value1, Value2);
	}
	return 0;
}
