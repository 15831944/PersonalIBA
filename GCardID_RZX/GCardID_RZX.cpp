// GCardID_RZX.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "GCardID_RZX.h"

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


// CGCardID_RZXApp ����

CGCardID_RZXApp::CGCardID_RZXApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGCardID_RZXApp ����

CGCardID_RZXApp theApp;

// CGCardID_RZXApp ��ʼ��

BOOL CGCardID_RZXApp::InitInstance()
{
	CWinApp::InitInstance();

	WCHAR ch[512] = {0};
	GetModuleFileNameW(this->m_hInstance, ch, 512);
	GXX_LOG_CREATE(ch, 1);

	GXX_LOG_HIT("InitInstance()");

	return TRUE;
}

int CGCardID_RZXApp::ExitInstance()
{
	GXX_LOG_HIT("ExitInstance()");
	return CWinApp::ExitInstance();
}