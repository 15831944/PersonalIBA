// DdnPayCard.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DdnPayCard.h"
#include "PcpThread.h"
#include "DdnPcpInc.h"
#include "MD5.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDdnPayCardApp

BEGIN_MESSAGE_MAP(CDdnPayCardApp, CWinApp)
END_MESSAGE_MAP()


// CDdnPayCardApp ����

CDdnPayCardApp::CDdnPayCardApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDdnPayCardApp ����

CDdnPayCardApp theApp;


// CDdnPayCardApp ��ʼ��

BOOL CDdnPayCardApp::InitInstance()
{
	CWinApp::InitInstance();

	
	WCHAR chPath[512] = {0};
	GetModuleFileNameW(this->m_hInstance,chPath,512);
	CStringW strFilew = chPath;
	strFilew = strFilew.Left(strFilew.ReverseFind(_T('\\')));
	strFilew += _T("\\CardLog\\");

	_wmkdir(strFilew);

	GXX_LOG_CREATE(strFilew);

	G2XStartup();

	return TRUE;
}

int CDdnPayCardApp::ExitInstance()
{
	G2XShutdown();
	return CWinApp::ExitInstance();
}
//******************************************************************************

//��������

LONG WINAPI DdnGetTime()
{
	time_t tNow = 0;

	time(&tNow);

	return (LONG)tNow;
}

void WINAPI DdnMD5String(LPCTSTR lpszString, LPTSTR lpszDest, DWORD dwLen)
{
	CString strMD5 = CMD5::MD5String(lpszString);

	lstrcpyn(lpszDest, strMD5.GetString(), min(dwLen, (DWORD)strMD5.GetLength()) + 1);
}

void WINAPI DdnMD5File(LPCTSTR lpszFileName, LPTSTR lpszDest, DWORD dwLen)
{
	CString strMD5 = CMD5::MD5File(lpszFileName);

	lstrcpyn(lpszDest, strMD5.GetString(), min(dwLen, (DWORD)strMD5.GetLength()) + 1);
}

//******************************************************************************

// 1.1�汾�Ľӿں���

BOOL WINAPI DdnInitializePcpA(LPCSTR lpszParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	GXX_LOG_HIT(_T("Call Fun: DdnInitializePcpA"));
	CPcpThread::CreatePcp(lpszParams);

	return TRUE;
}

BOOL WINAPI DdnInitializePcpW(LPCWSTR lpszParams)
{
	return DdnInitializePcpA(CW2A(lpszParams));
}

BOOL WINAPI DdnShowPcp()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	GXX_LOG_HIT(_T("Call Fun: DdnShowPcp"));
	return CPcpThread::ShowPcp();
}

BOOL WINAPI DdnHidePcp()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CPcpThread::HidePcp();
}

BOOL WINAPI DdnTerminatePcp()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CPcpThread::DestoryPcp();
}

//******************************************************************************

// 1.0�汾�Ľӿں���

void WINAPI CreatePayCardPlatformA(LPCSTR lpszParams)
{
	DdnInitializePcpA(lpszParams);

	Sleep(100);

	DdnShowPcp();
}

void WINAPI CreatePayCardPlatformW(LPCWSTR lpszParams)
{
	CreatePayCardPlatformA(CW2A(lpszParams));
}

void WINAPI DestroyPayCardPlatform()
{
	DdnTerminatePcp();
}

//******************************************************************************
