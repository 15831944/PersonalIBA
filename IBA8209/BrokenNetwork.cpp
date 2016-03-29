#include "stdafx.h"
#include ".\brokennetwork.h"
#include "Dal\ConfigInfo.h"
#include "Dal\IBADAL.h"
#include "IBA.h"
#include "Socket\LocalServer.h"
#include "Servlet\CommonMethod.h"
#include "BrokenNetworkResume.h"
#include "MainFrm.h"
#include "Cashier.h"
#include "IBAHelpper.h"
#include "Servlet\LocalConsumeMethod.h"
#include "MsgBoxDlg.h"
#include "BossPwdDlg.h"
#include "Dal\RightsManagement.h"

using namespace NS_SERVLET;

static const UINT nTestingTime = (60*1000)*1;  //1���Ӳ���һ��
static const UINT nUploadTime = (60*1000)*2;   //2�����ϴ�һ��
static BOOL bHasResumedCenterUser = FALSE;      //�Ƿ��Ѿ������˶�����������û��Ľ�����Ϣ��

static const TCHAR* lpKeyMemberID = _T("LocalModeConfig_1");  // ����MemberID���ۼ�
static const TCHAR* lpKeyCasher = _T("LocalModeConfig_2");
static const TCHAR* lpKeyControlOption = _T("LocalMode_ControlOption");
static const TCHAR* lpKeyCasherAccess = _T("LocalMode_Access"); // ����Ա����Ȩ��
static const TCHAR* lpKeyLocalModeStatus = _T("LocalModeStatus"); // ����ģʽ��״̬


CBrokenNetwork::CBrokenNetwork(void)
{
	m_threadTestResume = new NS_COMMON::CThreadTemplate<CBrokenNetwork>;
	m_threadRestore = new NS_COMMON::CThreadTemplate<CBrokenNetwork>;

	m_threadTestResume->SetCallBack(this, TestingResume);
	m_threadRestore->SetCallBack(this, TestingUploadLocalConsume);

	m_bHasFirstTrying = FALSE;
}

CBrokenNetwork::~CBrokenNetwork(void)
{
	UNIT_AUTO_TRACE("----------------����ά������CBrokenNetwork����----------------");

	try
	{
		UNIT_AUTO_TRACE("�ر��߳�");
		m_threadTestResume->Terminate();
		m_threadRestore->Terminate();

		Sleep(1000);
		delete m_threadTestResume;
		delete m_threadRestore;
	}
	catch (...)
	{
		
	}
	
}

CBrokenNetwork* CBrokenNetwork::GetInstance()
{
	static CBrokenNetwork bn;
	return &bn;
}

BOOL CBrokenNetwork::SaveLocalStatusInfo(CLocalStatusInfo& Info)
{
	CConfigInfo ConfigInfo;
	CString strTmp;

	// ����ԱȨ���ַ���
	strTmp = Info.GetCasherAccess();
	ConfigInfo.Clear();
	ConfigInfo.SetKeyString(lpKeyCasherAccess);
	ConfigInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(ConfigInfo,3);

	// ���ɿ��Ʋ���
	ConfigInfo.Clear();
	ConfigInfo.SetKeyString(lpKeyControlOption);
	strTmp = Info.GetControlParam();
	ConfigInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(ConfigInfo,3);

	return TRUE;
}

BOOL CBrokenNetwork::LoadLocalStatusInfo()
{
	CConfigInfo info;

	// ����ԱȨ���ַ���
	info.Clear();
	info.SetKeyString(lpKeyCasherAccess);
	if (CIBADAL::GetInstance()->GetSystemConfig(info))
	{
		CString strPri = info.GetKeyValue3();
		theApp.GetCurCashier()->m_strEnableModuleList = strPri;
	}

	// ���ɿ��Ʋ���
	info.Clear();
	info.SetKeyString(lpKeyControlOption);

	if (CIBADAL::GetInstance()->GetSystemConfig(info))
	{
		CString strPri = info.GetKeyValue3();
		CNetBarConfig::GetInstance()->PraseControlOption(strPri);	
	}

	return TRUE;
}

UINT CBrokenNetwork::RegisterMemberID()
{
	CConfigInfo configInfo;
	configInfo.Clear();
	configInfo.SetKeyString(lpKeyMemberID);

	if (CIBADAL::GetInstance()->GetSystemConfig(configInfo))
	{
		UINT nID = configInfo.GetKeyValue1();
		ASSERT(nID >= 0x0FFFFFFF);
		nID -= 1;
		configInfo.SetKeyValue1(nID);
		CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
		return nID;
	}
	else
	{
		CConfigInfo configInfo;
		configInfo.SetKeyString(lpKeyMemberID);
		configInfo.SetKeyValue1((UINT)-1);
		CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
		return (UINT)-1;
	}
}

void CBrokenNetwork::DisableNetwork()
{
	static INT s_nCountOfNo = 0;
	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		switch(CNetBarConfig::GetInstance()->GetLocalModeStartKind())
		{
		case CIBAGlobal::emLocalModeForbid:
			return; // ���þ��޷�����Ӧ��ģʽ

		case CIBAGlobal::emLocalModeManual:
			{
				CDlgBossPwdEx dlg;
				if (dlg.DoModal() != IDOK)
				{
					return;
				}	
			}
			break;
		case CIBAGlobal::emLocalModeAuto:
			{
				if (s_nCountOfNo > 0)
				{
					return ;
				}

				CMsgBoxDlg dlg;
				CIBAString strTmp;
				strTmp.LoadString(IDS_LOCALMODE_INFO);
				dlg.SetText(strTmp,FALSE);//_T("����̨�����ĵ������������жϣ��Ƿ����Ӧ��ģʽ?")
				dlg.SetType(MB_YESNO|MB_ICONQUESTION);

				UINT nRet = dlg.DoModal();

				//UINT nRet = //theApp.IBAMsgBox(_T("����̨�����ĵ������������жϣ��Ƿ����Ӧ��ģʽ?"),MB_YESNO|MB_ICONQUESTION);
				if (nRet == IDNO)
				{
					s_nCountOfNo++;
					return;
				}
			}
			break;
		}

		GetInstance()->m_threadTestResume->Start();
	}
	SettingNetworkStatus(CIBAGlobal::emNetworkBroken);

	// 2011/08/24-8201-gxx: ���¿���ͨ�������ұߵ�״̬��ʾ.
	theApp.UpdateIBAStatus();

	bHasResumedCenterUser = FALSE;
}

void CBrokenNetwork::EnableNetwork()
{
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		if( CLocalServer::GetInstance()->ActiveMemberList.HasLocalUser() )
		{
			SettingNetworkStatus(CIBAGlobal::emNetworkRestore);
			
		}
		else
		{
			SettingNetworkStatus(CIBAGlobal::emNetworkNormal);
		}
		
		theApp.UpdateIBAStatus();
	}

	GetInstance()->m_bHasFirstTrying = FALSE; // ʹ����һ�ζ���ʱ��toolbar����ʾ"���ڳ�����������"����

	if ( !bHasResumedCenterUser )
	{
		if (CBrokenNetwork::GetInstance()->m_centerUserUpLoad.CheckShouldResume())
		{
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.SetShouldResume(TRUE);
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.StartResume(0);
		}
		bHasResumedCenterUser = TRUE;
	}
	
	if (theApp.GetCurCashier()->m_bIsLocalModeLogin)
	{
		UNIT_AUTO_TRACE("����ָ����Զ����µ�¼...");

		CCashRegisterLoginMethod crLogin;
		theApp.GetCurCashier()->CheckIn(crLogin, theApp.GetCurCashier()->GetName());

		if (crLogin.GetStatusCode() != 0)
		{
			CIBALog::GetInstance()->Write(_T("����ָ������µ�¼ʧ��!"));
		}
	}
}


BOOL CBrokenNetwork::TestingNetwork()
{
	CGetTimeMethod time;
	time.Execute();

	return time.GetStatusCode() >= 0 ;
}

void CBrokenNetwork::TestingResume()
{
	CoInitialize(NULL);
	while(TRUE)
	{
#ifdef _DEBUG
		Sleep(15000);
#else
		Sleep(nTestingTime);
#endif // _DEBUG
		
		try
		{
			if (TestingNetwork())
			{
				EnableNetwork();
				GetInstance()->m_threadRestore->Start();
				return ;
			}
		}
		catch (...)
		{
		}
		
	}
	CoUninitialize();
}

void CBrokenNetwork::TestingUploadLocalConsume()
{
	CoInitialize(NULL);

	while(TRUE)
	{
#ifdef _DEBUG
		Sleep(30000);
#else
		Sleep(nUploadTime);
#endif	

		CSingleLock lock(&m_TestUploadLock, TRUE);

		try
		{
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
			{
				continue;
			}

			CArray<CLocalConsumeInfo,CLocalConsumeInfo&> ArrayConsumeInfo;
			CIBADAL::GetInstance()->GetLocalConsumes(ArrayConsumeInfo);

			if( !CLocalServer::GetInstance()->ActiveMemberList.HasLocalUser() && 
				CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore &&
				ArrayConsumeInfo.GetCount() == 0)
			{
				SettingNetworkStatus(CIBAGlobal::emNetworkNormal); // û�б����û��ˣ��ɽ�����״̬��Ϊ����״̬
				theApp.UpdateIBAStatus();
			}

			INT nUploadCount = 0;
		
			for (int i = 0; i < ArrayConsumeInfo.GetCount(); i++)
			{
				if (!theApp.GetCurCashier()->IsOnDuty())
				{
					continue;
				}

				CLocalConsumeInfo& Info = ArrayConsumeInfo.GetAt(i);

				CLocalConsumeMethod ConsumeMethod;
				ConsumeMethod.SetCashRegisterId( CIBAConfig::GetInstance()->GetCashRegisterID() );
				ConsumeMethod.SetCheckinTime( CIBAHelpper::TranformTime(Info.GetCheckinTime()) );
				ConsumeMethod.SetCheckoutTime( CIBAHelpper::TranformTime(Info.GetCheckoutTime()) );
				ConsumeMethod.SetClassId( Info.GetClassId() );
				ConsumeMethod.SetConsumeAmount( Info.GetConsumeAmount() );
				ConsumeMethod.SetCreditAmount( Info.GetCreditAmount() );
				ConsumeMethod.SetDataStatus( Info.GetDataStatus() );
				ConsumeMethod.SetDutyId( theApp.GetCurCashier()->GetDutyID() );
				ConsumeMethod.SetIdNumber( Info.GetIdNumber() );
				ConsumeMethod.SetMemberId( Info.GetMemberId() );
				ConsumeMethod.SetNetBarId( Info.GetNetbarId() );
				ConsumeMethod.SetOperator( theApp.GetCurCashier()->GetName() );
				ConsumeMethod.SetPayType( Info.GetPayType() );
				ConsumeMethod.SetPcClass( Info.GetPCClass() );
				ConsumeMethod.SetReturnAmount( Info.GetReturnAmount() );
				ConsumeMethod.SetSerialNum( Info.GetSerialNum() );
				ConsumeMethod.SetTermId( Info.GetTermId() );
				ConsumeMethod.SetUserName( Info.GetName() );

				if (!theApp.GetCurCashier()->DoLocalConsume(ConsumeMethod))
				{		
					CIBALog::GetInstance()->WriteFormat(_T("�����������Ѽ�¼ʧ��,memberID=%d, netID=%s"), (int)Info.GetMemberId(),
						Info.GetSerialNum());
				}
				else if(ConsumeMethod.GetStatusCode() < 0)
				{
					CIBALog::GetInstance()->WriteFormat(_T("��������������Ϣʱ���������ж���"));
				}
				else if(ConsumeMethod.GetStatusCode() > 0)
				{
					if (ConsumeMethod.GetStatusCode() == ESC_SWSB73)
					{
						CIBALog::GetInstance()->WriteFormat(_T("�����������Ѽ�¼ʧ�ܣ�ԭ��:����Ա��ǿ���°�"));
						continue;
					}
					CIBALog::GetInstance()->WriteFormat(_T("�����������Ѽ�¼ʧ��,memberID=%d, netID=%s,��¼��ɾ��"), (int)Info.GetMemberId(),
						Info.GetSerialNum());
					
					CIBADAL::GetInstance()->DeleteLocalConsume(Info.GetMemberId());
				}
				else
				{
					if (!CIBADAL::GetInstance()->DeleteLocalConsume(Info.GetMemberId()))
					{
						CIBALog::GetInstance()->WriteFormat(_T("�����ݿ�������������Ѽ�¼ʧ��,memberID=%d, netID=%s"), (int)Info.GetMemberId(),
							Info.GetSerialNum());
					}
					
					// д��Member��
					if (ConsumeMethod.GetMemberID() > 0)
					{
						CIBADAL::GetInstance()->UpdateMemberInfo(Info, ConsumeMethod.GetMemberID());
					}
					
					COperationInfo OpInfo;
					OpInfo.SetOperationType( CIBAGlobal::emOpTypeLocalReturn );
					OpInfo.SetAllAmount( Info.GetConsumeAmount() + Info.GetReturnAmount() );
					OpInfo.SetMemberId( Info.GetMemberId() );
					OpInfo.SetNetId( Info.GetSerialNum() );
					OpInfo.SetOperationAmount( Info.GetReturnAmount() );
					OpInfo.SetOperationTime( COleDateTime::GetCurrentTime() );
					OpInfo.SetDescription(_T("�����ָ��󲹴�������Ϣ������"));
					OpInfo.SetUserName( Info.GetName() );
					OpInfo.SetIsResumeData(TRUE);

					::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OpInfo);
				}

				nUploadCount++;

				Sleep(1000); // �ȴ�1�룬��ֹ���Ĵ����æ
			}

			if ( CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal )
			{
				CoUninitialize();
				return;
			}

			if (nUploadCount == ArrayConsumeInfo.GetCount() && 
				CLocalServer::GetInstance()->ActiveMemberList.HasLocalUser() == FALSE)
			{
				// ����������Ϣȫ���ϴ���ϲ����û���ͼû�б����û�
				SettingNetworkStatus(CIBAGlobal::emNetworkNormal);
				theApp.UpdateIBAStatus();
				CoUninitialize();
				return ;
			}

		}
		catch (...)
		{
			CoUninitialize();
			return;
		}
	}
	CoUninitialize();
}


void CBrokenNetwork::SettingNetworkStatus(CIBAGlobal::emNetworkStatus status)
{
	CIBAGlobal::NetworkStatus = status;
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("localModeStatus"));
	configInfo.SetKeyValue1(status);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo);
}

CIBAGlobal::emNetworkStatus CBrokenNetwork::LoadLocalModeStatus()
{
	CConfigInfo configInfo;
	configInfo.SetKeyString(lpKeyLocalModeStatus);
	
	if (CIBADAL::GetInstance()->GetSystemConfig(configInfo))
	{
		switch(configInfo.GetKeyValue1())
		{
		case 0:
			CIBAGlobal::NetworkStatus = CIBAGlobal::emNetworkNormal;
			break;
		case 1:
			CIBAGlobal::NetworkStatus = CIBAGlobal::emNetworkBroken;
			break;
		case 2:
			CIBAGlobal::NetworkStatus = CIBAGlobal::emNetworkRestore;
			break;
		default:
			CIBAGlobal::NetworkStatus = CIBAGlobal::emNetworkNormal;
			break;
		}
	}

	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		GetInstance()->DisableNetwork();
		GetInstance()->m_threadTestResume->Start();
	}
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore)
	{
		GetInstance()->m_threadTestResume->Start();
	}

	return CIBAGlobal::NetworkStatus;
}

