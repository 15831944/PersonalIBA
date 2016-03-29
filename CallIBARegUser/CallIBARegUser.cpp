// CallIBARegUser.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CallIBARegUser.h"
#include "MD5.H"
#include <atlbase.h>
#include "HelpFuc.h"
#include "RealnameDllDef.h"

#define CALLIBAREGUSER_CFN_API extern "C" __declspec(dllexport)
// The following symbol used to force inclusion of this module for _USRDLL
#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// ���ǵ���������һ��ʾ����
extern "C" __declspec(dllexport) int __cdecl fnCallIBARegUser(PRegUserData pRegUserData)
{
	const UINT WM_REALNAME  = WM_USER + 100;
	
	// ����0�����ɹ�
	// ����1����̨δ����
	// ����2ע����������
	// ����3���ݵ����ݽṹ��С����
	// ����4����ָ��Ϊ��

	if(!pRegUserData)
		return 4;
	if(pRegUserData->cbCount != sizeof(RegUserData))
		return 3;

	HWND hIBA = ::FindWindow(_T("IBA_MAINWINDOW_CLASS"), NULL);
	if(hIBA != NULL)
	{

		CString strID = CDllHelper::AC2TC(pRegUserData->szCertificateID);
		// 2014-8-14 - qsc ��0
		{
			if (strID.GetLength() < 10)
			{
				strID.Insert(0, CString(_T("0000000000")).Left(10 - strID.GetLength()));
			}
		}
		CString strCID = strID;
		int nCIDTpe = atoi(pRegUserData->szCertificateType);
		CString strName = CDllHelper::AC2TC(pRegUserData->szName);
		CString strPWD = CDllHelper::AC2TC(pRegUserData->szPassword);
		CString strCountry = CDllHelper::AC2TC(pRegUserData->szCountry);
		CString strAddress = CDllHelper::AC2TC(pRegUserData->szAddr);

		CString strSign, strTime, strKey("jqdodonew1035");
		strKey.Format(_T("%s"), strKey);
		strTime.Format(_T("%ld"), time(NULL));

		// ��Ϊ�����˺������ֺ�Ӣ�ģ����Ա�����Բ��ÿ���
		strSign.Format(_T("%s%s%s"), strID, strTime, strKey);
		// �˺������ֺ���ĸ��ϣ������Ǳ�������
		CString strSignMd5 = CMD5::MD5String(strSign);

		CRegKey RegKey;
		if(ERROR_SUCCESS == RegKey.Create(HKEY_CURRENT_USER, _T("Software\\sunlike\\IBA\\RealName\\JQ\\OpenUser")))
		{
			RegKey.SetStringValue(_T("IDNo"), strID);
			RegKey.SetStringValue( _T("CertificateID"), strCID);
			RegKey.SetStringValue( _T("Name"), strName);
			RegKey.SetStringValue(_T("Country"), strCountry);
			RegKey.SetStringValue( _T("Address"), strAddress);	
			RegKey.SetStringValue( _T("Password"), strPWD);
			RegKey.SetStringValue( _T("Other"), _T(""));
			RegKey.SetStringValue( _T("vValue1"), strTime);
			RegKey.SetStringValue( _T("vValue2"), strSignMd5);
			RegKey.SetDWORDValue( _T("CertificateType"), nCIDTpe);
			RegKey.SetDWORDValue( _T("Sex"), pRegUserData->Sex);
		}
		else
		{
			return 2;
		}
		RegKey.Close();

		int nRet = (int)::SendMessage(hIBA, WM_REALNAME, 0, 0);
		return (nRet == 1);
	}
	return 1;
}