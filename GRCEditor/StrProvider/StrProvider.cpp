// StrProvider.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StrProvider.h"

#include "StringContainer.h"


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

// CStrProviderApp

BEGIN_MESSAGE_MAP(CStrProviderApp, CWinApp)
END_MESSAGE_MAP()


// CStrProviderApp ����

CStrProviderApp::CStrProviderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStrProviderApp ����

CStrProviderApp theApp;


// CStrProviderApp ��ʼ��

BOOL CStrProviderApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CStringContainer theStrContainer;

BOOL __stdcall LoadStringContainer(const char* lpGrcFilePath)
{
	return theStrContainer.LoadGrcFile(CString(CA2T(lpGrcFilePath)));
}
void __stdcall GetStringA(UINT nID, char* pchOut, const unsigned int nSize)
{
	theStrContainer.GetStringA_(nID, pchOut, nSize);
}
void __stdcall GetStringW(UINT nID, wchar_t* pchOut, const unsigned int nSize)
{
	theStrContainer.GetStringW_(nID, pchOut, nSize);	 ;
}

