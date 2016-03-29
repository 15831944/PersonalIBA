// ID2Reader.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ID2Reader.h"
#include "ID2ReaderDlg.h"
#include ".\id2reader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CID2ReaderApp

BEGIN_MESSAGE_MAP(CID2ReaderApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CID2ReaderApp ����

CID2ReaderApp::CID2ReaderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CID2ReaderApp ����

CID2ReaderApp theApp;


// CID2ReaderApp ��ʼ��

BOOL CID2ReaderApp::InitInstance()
{
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("_ID2READER_MUTEX_"));
	if(hMutex == NULL)
		return FALSE;
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return FALSE;
	}

	HANDLE hMutex2 = ::CreateMutex(NULL, FALSE, _T("_ID2READER_ALLOW_MUTEX_"));
	if(hMutex2 == NULL)
		return FALSE;

	if(GetLastError() != ERROR_ALREADY_EXISTS)
	{
		// ����������
		return FALSE;
	}
	else
	{
		CloseHandle(hMutex2);
	}


	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	char chPath[512];
	::GetModuleFileName(NULL, chPath, 512);
	GXX_LOG_CREATE(CA2W(chPath), 3); // ������־�����������������־

	GXX_LOG_HIT("-------------------------------------");
	GXX_LOG_HIT("- InitInstance");
	GXX_LOG_HIT("-------------------------------------");
	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	

	CID2ReaderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CID2ReaderApp::ExitInstance()
{
	GXX_LOG_HIT("DodoReader Exit!");

	return CWinApp::ExitInstance();
}


