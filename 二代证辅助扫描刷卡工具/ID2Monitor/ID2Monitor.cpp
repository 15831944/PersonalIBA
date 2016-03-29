// ID2Monitor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ID2Monitor.h"
#include "ID2MonitorDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CID2MonitorApp

BEGIN_MESSAGE_MAP(CID2MonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CID2MonitorApp ����

CID2MonitorApp::CID2MonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CID2MonitorApp ����

CID2MonitorApp theApp;


// CID2MonitorApp ��ʼ��

BOOL CID2MonitorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	CString strPath;
	GetModuleFileName(NULL,strPath.GetBuffer(255),255);
	strPath.ReleaseBuffer();

	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));

	CString strEvnPath = strPath;

	strPath += _T("\\ID2MonitorLog\\");

	_mkdir(strPath);
	GXX_LOG_CREATE(CT2W(strPath),7);

	GXX_LOG_HIT("InitInstance------------ID2MonitorV1.0.0.25");

	CString strTmp;

	strTmp.GetEnvironmentVariable(_T("Path"));

	if (strTmp.Right(1) != _T(";"))
	{
		strTmp.AppendChar(';');
	}

	strTmp.Append(strEvnPath+"\\Scan");

	if (SetEnvironmentVariable(_T("Path"), strTmp))
	{
		GXX_LOG_HIT("���û��������ɹ�");
	}
	else
	{
		GXX_LOG_HIT("���û�������ʧ��!");
	}

	

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CID2MonitorDlg dlg;
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

int CID2MonitorApp::ExitInstance()
{
	GXX_LOG_HIT("ExitInstance-------------");

	return CWinApp::ExitInstance();
}