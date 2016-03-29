// Lock.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Lock.h"
#include "LockDlg.h"
#include ".\lock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
char singleLockMutex[] = "2353FC39-9870-4cda-BB19-C7BBB7E60D3D";

// CLockApp{}


BEGIN_MESSAGE_MAP(CLockApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLockApp ����

CLockApp::CLockApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLockApp ����

CLockApp theApp;


// CLockApp ��ʼ��

BOOL CLockApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	HANDLE m_hMutex = CreateMutex(NULL, FALSE, singleLockMutex);
	if (m_hMutex)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			return FALSE;

		}
	}
	else
	{

		return FALSE;
	}

	m_pMainWnd = new CLockDlg();

	((CLockDlg*)m_pMainWnd)->Create(CLockDlg::IDD);

	return TRUE;
}

int CLockApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
