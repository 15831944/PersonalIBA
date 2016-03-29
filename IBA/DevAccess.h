////////////////////////////////////////////////////////////////////////////////////////
// 
//	Project:    Class DevAccess
//              Used for getting devices on the terminal and write&read specified device
//  System:     WinNT
//	File:       DevAccess.h
//	Start date:	2016.03.06
//	Version:    1.0
//	Author:     liyajun91@yahoo.com
//	Copyright 
//  Note:       Need import "setupapi.lib" , Need library "setupapi.lib"&"hid.lib", Headfiles "setupapi.h"&"hidsdi.h"&"hidusage.h"
//
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <iomanip>
#include <vector>

extern "C" 
{  
#include "setupapi.h" 
#include "hidsdi.h" 
//#include <iosfwd>
}
using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

class CDevAccess
{
public:
	CDevAccess::CDevAccess();
	CDevAccess::~CDevAccess();

public:
	//��ȡĳ���豸(�ӿ���)
	BOOL GetClassDevs(const GUID *InterfaceClassGUID, std::vector<std::tstring>* pvDevPaths/*,std::vector<std::tstring>* pvHardwareIDs*/);
	//�Դ����豸���г�ʼ��.��ʼ���ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL InitPort(const TCHAR* lpszDevPath,UINT baud = 115200, char parity = 'N',UINT databits = 8,UINT stopbits = 1);
	void WriteToDev(const TCHAR* lpszCmd,const TCHAR* lpszDevPath);
	//BOOL ReadDev(TCHAR* lpszOutbuffer,const TCHAR* lpszDevPath);

public:
	BOOL GetReadyState();
	BOOL Reset();
	void ShowLogo();
	void ShowSuccess();
	void CleanDisplay();
	void ShowQrInfo(const TCHAR *lpNumber,const TCHAR *lpPrice,const TCHAR *lpLink);

private:
	string unicode_to_multibyte(const wchar_t* lpUnicode);
	wstring multibyte_to_unicode(const char* lpMultiByte);

private:
	CRITICAL_SECTION m_csSynCommDevice; //���ƶ��豸�ķ���
	HANDLE m_hDev;//�豸���
	BOOL m_bDevReady;//�豸�Ƿ�׼����
	std::tstring m_sDevPath;//�豸·��
	std::tstring m_sDevLocation;//�豸��ַ(�����豸����)
};

