#include "StdAfx.h"
#include ".\capturemanager.h"

CCaptureManager::CCaptureManager(void)
{
}

CCaptureManager::~CCaptureManager(void)
{
}

CaptureItem& CCaptureManager::GetItem(int iIndex)
{
	return m_items[iIndex];
}	

UINT ToUINT(const char * lpStr)
{
	BYTE bt[100];
	CGxxStringA strA(lpStr);
	if (strA.GetLength() % 2 != 0)
	{
		strA.Insert(0,"0");
	}
	int nSize = strA.Parse16ToBuf(bt, 100);
	UINT nXX[4] = {1, 256, 256*256, 256*256*256};
	UINT nVal = 0;
	for (int i=0; i < nSize; i++)
	{
		BYTE by = bt[nSize-i-1];
		nVal += UINT(by)*nXX[i];
	}
	return nVal;
}

void CCaptureManager::ReadFromConfig()
{
	m_items.clear();

	CString strIni;
	GetModuleFileName(NULL,strIni.GetBuffer(256),256);
	strIni.ReleaseBuffer();

	strIni = strIni.Left(strIni.ReverseFind('\\'));
	strIni += "\\other.ini";

	int nCount = ::GetPrivateProfileInt("items_count", "count", 0, strIni);
	if (nCount == 0)
	{
		MessageBox(NULL,"��ȡ��������û������ϵͳ����", NULL, MB_OK);
		GXX_LOG_ERROR("��ȡ��������û������ϵͳ����, count=0");
		return;
	}

	for(int i=0; i < nCount; i++)
	{
		CaptureItem item;

		CString strAppName;
		strAppName.Format("item%d", i+1);

		CString strTmp;
		::GetPrivateProfileString(strAppName, "name", "��", strTmp.GetBuffer(200), 200, strIni);
		strTmp.ReleaseBuffer();
		item.strName = strTmp;

		INT iTmp = ::GetPrivateProfileInt(strAppName, "isDialog", 0, strIni);
		item.bDialog = (iTmp > 0 ? TRUE:FALSE);

		::GetPrivateProfileString(strAppName, "windowName", "��", strTmp.GetBuffer(200), 200, strIni);
		strTmp.ReleaseBuffer();
		item.strWindowName = strTmp;

		::GetPrivateProfileString(strAppName, "className", "��", strTmp.GetBuffer(200), 200, strIni);
		strTmp.ReleaseBuffer();
		item.strClassName = strTmp;

		::GetPrivateProfileString(strAppName, "cardId", "0", strTmp.GetBuffer(200), 200, strIni);
		strTmp.ReleaseBuffer();
		item.nCardId = ToUINT(strTmp.GetString());


		::GetPrivateProfileString(strAppName, "numberId", "0", strTmp.GetBuffer(200), 200, strIni);
		strTmp.ReleaseBuffer();
		item.nNumberId = ToUINT(strTmp.GetString());

		::GetPrivateProfileString(strAppName, "nameId", "0", strTmp.GetBuffer(200), 200, strIni);
		strTmp.ReleaseBuffer();
		item.nNameId = ToUINT(strTmp.GetString());

		m_items.push_back(item);

	}
	
}

int CCaptureManager::GetCount()
{
	return (int)m_items.size();
}

