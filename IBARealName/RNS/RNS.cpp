// RNS.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RNS.h"
#include "RNSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRNSApp


// CRNSApp ����

CRNSApp::CRNSApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRNSApp ����

CRNSApp theApp;


// CRNSApp ��ʼ��

BOOL CRNSApp::InitInstance()
{
	CWinApp::InitInstance();

	ParseCommandLine(m_CommandLine);

	//CRNSDlg dlg;
	//m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
	//	//�Ի���Ĵ���
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
	//	//�Ի���Ĵ���
	//}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
