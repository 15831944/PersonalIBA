// IDRSUCore.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "IDRSUCore.h"

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

// CIDRSUCoreApp

BEGIN_MESSAGE_MAP(CIDRSUCoreApp, CWinApp)
END_MESSAGE_MAP()


// CIDRSUCoreApp ����

CIDRSUCoreApp::CIDRSUCoreApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CIDRSUCoreApp ����

CIDRSUCoreApp theApp;


// CIDRSUCoreApp ��ʼ��

BOOL CIDRSUCoreApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

struct WDUserInfo
{
	char szName[40];
	char szSex[10];
	char szNation[20];
	char szBirthDate[50];
	char szAddress[200];
	char szID[40];
	char szFromDepart[200];
	char szStartDate[50];
	char szEndDate[50];
	char szNewAddress[200];
};

INT init()
{
	return 1;
}

INT WINAPI GetInfoFirstType_1(WDUserInfo* lpUserInfo)
{
	lstrcpy(lpUserInfo->szName, "����");
	lstrcpy(lpUserInfo->szSex, "��");
	lstrcpy(lpUserInfo->szID, "140401198001010039");
	lstrcpy(lpUserInfo->szNation, "����");

	return 1;
}

INT WINAPI GetInfoSecondType_1(WDUserInfo* lpUserInfo)
{
	lstrcpy(lpUserInfo->szName, "����");
	lstrcpy(lpUserInfo->szSex, "Ů");
	lstrcpy(lpUserInfo->szID, "350101198001010023");
	lstrcpy(lpUserInfo->szNation, "����");

	return 1;
}

INT WINAPI GetInfoSecondTypeMachine_1(WDUserInfo* lpUserInfo)
{
	lstrcpy(lpUserInfo->szName, "����");
	lstrcpy(lpUserInfo->szSex, "��");
	lstrcpy(lpUserInfo->szID, "371081198408010018");
	lstrcpy(lpUserInfo->szNation, "����");

	return 1;
}

INT WINAPI GetOtherType_1()
{
	return 1;
}
