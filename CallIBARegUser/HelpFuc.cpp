#include "stdafx.h"
#include "HelpFuc.h"

// Tת���ֽ�
CStringA CDllHelper::TC2AC(LPCTSTR szTrans)
{
	CStringA strTmp;
#ifdef _UNICODE
	DWORD dwLen = WideCharToMultiByte(CP_OEMCP,NULL,szTrans,-1,NULL,0,NULL,FALSE);		// �Ȼ�ȡ�ַ�����ĳ���
	char* pArr = new char[dwLen];
	WideCharToMultiByte (CP_ACP,NULL,szTrans,-1,pArr,dwLen,NULL,FALSE);
	strTmp = pArr;
	delete pArr;
#else
	strTmp = szTrans;
#endif
	return strTmp;
}

// ���ֽ�תT
CString CDllHelper::AC2TC(LPCTSTR szTrans)
{
	CString strTmp;
#ifdef _UNICODE
	DWORD dwLen = MultiByteToWideChar(CP_OEMCP,NULL,szTrans,-1,NULL,0,NULL,FALSE);		// �Ȼ�ȡ�ַ�����ĳ���
	wchar_t *pArr = new wchar_t[dwLen];
	MultiByteToWideChar (CP_ACP,NULL,szTrans,-1,pArr,dwLen,NULL,FALSE);
	strTmp = pArr;
	delete pArr;
#else
	strTmp = szTrans;
#endif
	return strTmp;
}