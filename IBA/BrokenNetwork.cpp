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
#include "Servlet\CreditMethod.h"
#include "LocalCenter\gxx_base.h"
#include "LocalCenter\LocalCenter.h"
#include "Servlet\BuckleAmountMethod.h"
#include "GlobalFunctions.h"
#include "CashierHelper.h"
#include "ServletHellper.h"
#include "DdnDataX.h"
#include "RealName\RealNameDataX.h"
#include "RealName\IZd.h"
#include "RealName\RealNameBase.h"
#include "RealName\IBARealName.h"
#include "MainFrm.h"

//���Ʒ���LocalMember�Լ�LocalCredit��
CCriticalSection g_csLocalCreditLock;
CCriticalSection g_csLocalMemberLock;
//CCriticalSection g_csDisplay
//���Ʒ�����������
CCriticalSection m_SettingNetworkStatusLock;

using namespace NS_SERVLET;
using namespace IBAGlobal;
using namespace NS_SERVLET;
//liyajun Temp
#ifdef _DEBUG
	static const UINT nTestingTime = 1000;  //1s�Ӳ���һ��
#else
	static const UINT nTestingTime = (60*1000)*1;  //1���Ӳ���һ��
#endif

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
	m_threadTestBroken = new NS_COMMON::CThreadTemplate<CBrokenNetwork>;

	m_threadTestResume->SetCallBack(this, TestingResume);
	m_threadRestore->SetCallBack(this, TestingUploadLocalConsume);	
	m_threadTestBroken->SetCallBack(this,TestingTestBroken);

	//m_bHasFirstTrying = FALSE;
	bTestBrokenStarted = FALSE;
}

CBrokenNetwork::~CBrokenNetwork(void)
{
	UNIT_AUTO_TRACE("----------------����ά������CBrokenNetwork����----------------");

	//UNIT_AUTO_TRACE("�ر��߳�");
	m_threadTestResume->Terminate();
	m_threadRestore->Terminate();
	m_threadTestBroken->Terminate();

	Sleep(1000);
	delete m_threadTestResume;
	delete m_threadRestore;
	delete m_threadTestBroken;
}

CBrokenNetwork* CBrokenNetwork::GetInstance()
{
	static CBrokenNetwork bn; //ֻ�г�������ʱ��Ψһ���䣬����ֻ��һ��
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
/*************************************************
Function:       //RestoreNetwork
Description:    //��������Ϊ�ָ�ģʽ
Calls:          //SettingNetworkStatus
Other:			//ֻ�ܴ�����ģʽ���ָ�ģʽ
*************************************************/
void CBrokenNetwork::RestoreNetwork()
{
	CSingleLock lock(&m_SettingNetworkStatusLock,TRUE);

	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)//ֻ�ܴ�Normal��Restore
	{
		IBA_LOG(_T("NetworkStatus => Restore"));
		SettingNetworkStatus(CIBAGlobal::emNetworkRestore);
	}	
}
/*************************************************
Function:       //DisableNetwork
Description:    //��������Ϊ����ģʽ,����bHasResumedCenterUserΪFALSE
Calls:          //SettingNetworkStatus
*************************************************/
void CBrokenNetwork::DisableNetwork()
{
	CSingleLock lock(&m_SettingNetworkStatusLock,TRUE);

	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		IBA_LOG(_T("NetworkStatus => Broken"));
		SettingNetworkStatus(CIBAGlobal::emNetworkBroken);
		bHasResumedCenterUser = FALSE;
	}	
}
/*************************************************
Function:       //EnableNetwork
Description:    //��������Ϊ����ģʽ�����IBA�Ƿ񱾵ص�¼�ģ�����������ִ�е�½
Calls:          //SettingNetworkStatus
*************************************************/
void CBrokenNetwork::EnableNetwork()
{
	CSingleLock lock(&m_SettingNetworkStatusLock,TRUE);

	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal)
	{
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)//ֻ�д�Broken�ָ�ʱ���Ŵ�ӡ��־���������־
		{
			IBA_LOG(_T("NetworkStatus => Normal"));
		}		
		SettingNetworkStatus(CIBAGlobal::emNetworkNormal);
	}

	if (theApp.GetCurCashier()->m_bIsLocalModeLogin)
	{
		IBA_TRACE("����ָ����Զ����µ�¼...");

		CCashRegisterLoginMethod crLogin;
		theApp.GetCurCashier()->CheckIn(crLogin, theApp.GetCurCashier()->GetName());

		if (crLogin.GetStatusCode() != 0)
		{
			IBA_LOG(L"����ָ������µ�¼ʧ��!");
		}
	}
}

//̽�������Ƿ�Ͽ����Ͽ�����FALSE,���򷵻�TRUE
BOOL CBrokenNetwork::TestingNetwork()
{
	CGetTimeMethod time;
	time.Execute();

	return time.GetStatusCode() >= 0 ;
}

/*************************************************
Function:       // TestingResume
Description:    // �̺߳�����̽�������Ƿ��ѻָ�һ��ʱ�䣬���������������Ϊ�ѻָ������������������̡߳�
				// ����Ͽ���DisableNetworkʱ���������ʱ�����ϴ�����״̬Ϊ����ʱ�������̡߳�����ָ��󣬽������̡߳�
Calls:          // TestingNetwork,EnableNetwork,TestingUploadLocalConsume
Return:         // ����ָ�����һ��ʱ��󣬷���
Others:         // �߳̽������´η�������ʱ�ֻ�������������ʹ�ò�ͬ���߳�ID
*************************************************/
void CBrokenNetwork::TestingResume()
{
	IBA_LOG(_T("Start thread:Test network resume"));
#ifdef _DEBUG
	const DWORD minRestoreTick = 500;
#else
	const DWORD minRestoreTick = 3 * 60 * 1000;// �������ٻָ�3�����ˣ���������Ϊ�����ѻָ�
	//const DWORD minRestoreTick = 500;
#endif

	CoInitialize(NULL);
	DWORD dwTickcout = 0;

	while(TRUE)
	{
		if (TestingNetwork())            //̽�������Ƿ�����
		{
			if (dwTickcout == 0)
			{
				dwTickcout = GetTickCount();
			}
			else
			{
				if (GetTickCount() - dwTickcout >= minRestoreTick)   //���һ��̽����������>=ĳ����ֵ
				{
					IBA_TRACE(_T("�����ѻָ�"));
					IBA_LOG(_T("�����ѻָ�"));
					//ֻ�е�localMember�м�¼ʱ������ʾ��ʼ�ϱ�����ģʽ
					CString sql;
					sql.Format(_T("select count(*) from localMember"));
					if(CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) > 0)
					{
						COperationInfo OperationInfo ;
						OperationInfo.SetDescription (_T("��ʼ�ϱ�����ģʽ��������..."));
						SendMessage(CMainFrame ::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM )(&OperationInfo));
					}
					EnableNetwork();                                //��������״̬����
					GetInstance()->m_threadRestore->Start();        //��ʼִ�ж����ָ��߳�
					return ;
				}
			}
		}
		else
		{
			dwTickcout = 0;
		}


#ifdef _DEBUG
		//Sleep(15000);
		Sleep(1000);
#else
		Sleep(nTestingTime);
		//Sleep(1000);
#endif 

	}
	CoUninitialize();
}

/*************************************************
Function:       //TestingUploadLocalConsume
Description:    //�ӱ���ģʽ�ָ�������ģʽ��,Ϊ����ģʽ�µĲ��������ϱ�����. - ֻ�ϱ������ߵ��û���
Table Accessed: //LocalMember, LocalCredit,CheckOutInfo
Table Updated:  //LocalMember, LocalCredit
Others:         //�߳�һֱ���в��˳�
*************************************************/
void CBrokenNetwork::TestingUploadLocalConsume()
{
	IBA_LOG(_T("Start thread:Upload local mode data for offline users"));
	CoInitialize(NULL);

	while(TRUE)
	{
#ifdef _DEBUG
		Sleep(30000);
#else
		Sleep(nUploadTime);  //2����
#endif	

		CSingleLock lock(&m_TestUploadLock, TRUE);

		//�Ƿ��ϱ����κμ�¼
		bool bHasResumedSth(FALSE);

		//����δ�ָ���������
		if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal)
		{
			continue;
		}

		//�������ؿ���(LocalMember)�ļ�¼
		GxxArrayPtr localNotReturnedArrayPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(_T("select * from localMember"));
		for (int i = 0; i < localNotReturnedArrayPtr->count(); i++)                        
		{
			GxxDictionaryPtr memberPtr = localNotReturnedArrayPtr->valueAtIndex(i)->getDictionary();
			UINT nMemberId = memberPtr->valueForKey("memberId")->intValue();
			CLocalMember lMember;
			CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberId,lMember);
			//���ߵ��û�,���ڴ˴������ϱ�
			CActiveMember ActiveMember;
			if(CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember))//
			{
				if(ActiveMember.GetIsOnLine())
				{
					continue;
				}
			}

			//�����ϴ�������
			bHasResumedSth = TRUE;

			//�ϱ����س�ֵ
			CCashierHelper::ReportLocalCredit(memberPtr);

			//�Ƿ����
			if(lMember.nIsReturned)//�ѽ���
			{
				//�Ƿ񱾵�ģʽ�µ�½
				if(lMember.nIsLocalCheckin)//����ģʽ�µ�½
				{
					//�ϱ�������Ϣ
					CCashierHelper::ReportLocalConsume(lMember);
					if(CNetBarConfig::GetInstance()->GetIsMember(lMember.nClassId))//��Ա-ȡ������
					{
						CActivationMethod Activation;
						Activation.SetMemberId(lMember.nMemberId);
						Activation.SetMode(CActivationMethod::ACTIVATION_NO);
						theApp.GetCurCashier()->DoActivation(Activation);
					}
					else//��ʱ�û�-�ϱ������˿�
					{
						CCashierHelper::ReportLocalReturn(lMember);
					}
				}
				else//����ģʽ�µ�½
				{
					if(CNetBarConfig::GetInstance()->GetIsMember(lMember.nClassId))//��Ա-�ϱ����ؽ���
					{
						CCashierHelper::ReportLocalReturnVip(lMember);
					}
					else//��ʱ�û�-�ϱ������˿�
					{
						CCashierHelper::ReportLocalReturn(lMember);
					}
				}
			}
			else//δ����
			{
				//��ѯ���
				CIDCheckCardMethod CheckCard;
				CString strTmp = lMember.strNetId;
				IBA_LOG(_T("strTmp = %s"),strTmp);
				if (strTmp.GetLength() < 10)
				{
					strTmp.Insert(0, CString(_T("0000000000")).Left(10 - strTmp.GetLength()));
				}
				CheckCard.SetSerialNum(strTmp);
				theApp.GetCurCashier()->DoCheckCard(CheckCard);

				if(CheckCard.GetStatusCode() == 0)
				{
					int nCardId = CheckCard.GetCardId();
					CBalanceMethod Balance;
					Balance.SetMemberId(nMemberId);
					Balance.SetCardId(nCardId);
					//Balance.SetRound(0);

					theApp.GetCurCashier()->DoBalance(Balance);
				}
				//����
				CActivationMethod Activation;

				Activation.SetMemberId(lMember.nMemberId);
				Activation.SetMode( CActivationMethod::ACTIVATION_YES);
				//Activation.SetDotType(nActivationType);
				theApp.GetCurCashier()->DoActivation(Activation);
			}

			//�����û���ͼ
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

			//ɾ���û���¼
			if(FALSE == RemoveLocalMember(nMemberId))
			{
				IBA_LOG(_T("Serious Error!ɾ�����ؿ��¼ʧ��!�����˳�."));
				TerminateProcess(GetCurrentProcess(),0);
			}
			Sleep(500);
		}

		//��鱾�ؿ�,��û�м�¼��,�͵����ϱ������־
		GxxArrayPtr localArrayPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(_T("select * from localMember"));
		if(0 == localArrayPtr->count() && bHasResumedSth)
		{
			COperationInfo OperationInfo ;
			OperationInfo.SetDescription (_T("�ϱ�����ģʽ�������!"));
			SendMessage(CMainFrame ::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM )(&OperationInfo));	
		}
	}
	CoUninitialize();
}

/*************************************************
Function:       // SettingNetworkStatus
Description:    // ��������״̬.CIBAGlobal::NetworkStatus�Լ���SystemConfig.localMode.��������״̬����ʾ.
Calls:          // None
Table Accessed: // None
Table Updated: //  SystemConfig, ����������óɶϿ���localModeStatus�ֶ���Ϊ1��������Ϊ0
Input:          // Ҫ���óɵ�����״̬ 0-Normal, 1-Broken, 2-Restore
Output:         // None
Return:         // None
Others:         // None
*************************************************/
void CBrokenNetwork::SettingNetworkStatus(CIBAGlobal::emNetworkStatus status)
{
	/*IBA_LOG(_T("NetworkStatus => %s"),status?_T("Broken"):_T("Normal"));*/
	CIBAGlobal::NetworkStatus = status;
	//UINT nStatus(0);
	//if(CIBAGlobal::emNetworkBroken == status)
	//{
	//	nStatus = 1;
	//}
	//else
	//{
	//	nStatus = 0;
	//}
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("localModeStatus"));
	configInfo.SetKeyValue1(status);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo);

	//��Restoreģʽ��ʾ������������
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore)
	{
		theApp.UpdateIBAStatus(TRUE);
	}
	else
	{
		theApp.UpdateIBAStatus();
	}	
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

	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken )
	{
		GetInstance()->DisableNetwork();
		GetInstance()->m_threadTestResume->Start();
	}
	//����ʱ���ָ�ģʽ��Ϊ����ģʽ�������Զ��ٴν�������ģʽ
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore)
	{
		GetInstance()->EnableNetwork();
	}

	return CIBAGlobal::NetworkStatus;
}

/*************************************************
Function:       //RemoveLocalMember
Description:    //��LocalMember��ɾ����¼(�ȼ��������޼�¼����)
Calls:          //...
Table Accessed: //LocalMember
Table Updated:  //LocalMember
Input:          //MemberId - ��ɾ����¼��MemberId
Return:         //ɾ���ɹ�����TRUE
Others:         //
*************************************************/
BOOL CBrokenNetwork::RemoveLocalMember(UINT MemberId)
{
	IBA_LOG(_T("CBrokenNetwork::RemoveLocalMember - Start for MemberId = %d"),MemberId);
	CSingleLock lock(&g_csLocalMemberLock,TRUE);

	CString sql;

	sql.Format(_T("select * from localMember where memberId=%d"),MemberId);
	if(CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) < 1 )
	{
		lock.Unlock();
		IBA_LOG(_T("CBrokenNetwork::RemoveLocalMember - End. No record in table whose MemberId = %d"),MemberId);
		return TRUE;
	}

	sql.Format(_T("delete from localMember where memberId=%d"), MemberId);

	if(CIBADAL::GetInstance()->ExecuteSQLEx(sql) <1)
	{
		IBA_LOG(_T("CBrokenNetwork::RemoveLocalMember - Result is FALSE for MemberId = %d"),MemberId);
		lock.Unlock();
		GXX_ASSERT(0,"��localMemberɾ������ʧ�ܣ�");
		return FALSE;
	}

	IBA_LOG(_T("CBrokenNetwork::RemoveLocalMember - Result is TRUE for MemberId = %d"),MemberId);
	lock.Unlock();
	return TRUE;
}

/*************************************************
Function:       // TestingTestBroken
Description:    // �̺߳���������������ʧ�ܽ���Restoreģʽ�󣬿���̽�������Ƿ���ĶϿ�(����1���Ӷ�����ʧ��).����Ǿͽ��뱾��ģʽ.���뱾��ģʽ������ģʽ���˳��߳�. 
*************************************************/
void CBrokenNetwork::TestingTestBroken()
{
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		return;
	}

	IBA_LOG(_T("Start thread:Test network Broken"));	

	const DWORD minTimeTick = 1 * 60 * 1000;//����1����,���������Ȼδ�ָ�,���ж��Ƿ���뱾��
	const DWORD nInterval = 1 * 1000; //���Լ��Ϊ1����

	CoInitialize(NULL);
	DWORD dwTickcout = GetTickCount();   //��ʼ��ʱʱ��
	DWORD dwTickcoutPast = 0;            //�Ѳ���ʱ��
	DWORD dwTickcoutCurr;                //��ǰʱ��

	while(TRUE)
	{

		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			return;
		}
		CGetTimeMethod time;
		time.Execute();

		if(time.GetStatusCode() >= 0)
		{
			//�ɹ�->��������ģʽ
			EnableNetwork();
			bTestBrokenStarted = FALSE;
			return;
		}
		else
		{
			//ʧ��->�ж��Ƿ��ѵ�1����
			dwTickcoutCurr = GetTickCount();
			dwTickcoutPast = dwTickcoutCurr - dwTickcout;
			if(dwTickcoutPast > minTimeTick)//����1����,�������ڽ���ѡ��
			{
				//ֻ���ж�MainFrame�Ƿ��Ѿ�����		
				if(theApp.m_pMainWnd != NULL)
				{
					CIBAString strTemp;
					strTemp.LoadString(IDS_ENTERLOCALMODE_QUERY);
					while(true)//����ѯ���Ƿ���뱾��ģʽѭ��
					{
						CMsgBoxDlg dlg(theApp.m_pMainWnd);
						dlg.SetText(strTemp,FALSE);
						dlg.SetType(MB_RETRYCANCEL|MB_ICONEXCLAMATION);

						if(IDENTERLOCAL == dlg.DoModal())//���뱾��ģʽ
						{
							if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
							{
								IBA_TRACE("�����ĵ����������ѶϿ�");

								CString strCap;
								CIBAString strCaption;
								strCaption.LoadString(IDS_LOCALMODE_CAPTION);
								CIBAString strTmp;
								strTmp.LoadString(IDS_LOCALMODE_INFO);

								CMsgBoxDlg dlg(theApp.m_pMainWnd);
								//����̨��Ʒ����ĵ��������жϣ������뱾��ģʽ����ע�����¼��㣺\n\n1.����ֻ̨��ʹ�ü����ֵ�˿���˹��ܣ��������¿��û�\n2.�¼����û�ԭ���˻��еĽ����ɼ���ֻ���ڱ���ģʽ�³�ֵʹ�ã��Ѿ�������û�����Ӱ��\n3.�ͻ���ֻ�ܲ鿴�����ʣ������������»����������������\n4.����ָ������Եȼ����ӣ�����ɲ�������ģʽ�����ݺ���ʹ��\n
								dlg.SetText(strTmp,FALSE);
								dlg.SetType(MB_OK|MB_ICONQUESTION);
								dlg.SetCaption(strCaption);

								dlg.DoModal();

								DisableNetwork(); 
								IBA_LOG(L"m_threadTestResume => Start");
								m_threadTestResume->Start();//�����̡߳�����ָ���Ϊ�������û��������ؼ�¼�̺߳���������ȷ���ָ��Ժ�������ֻ��������״̬Ϊ��������������ָ�����localMemberɾ����¼��

								bTestBrokenStarted = FALSE;
								return;
							}
							break;//����1
						}
						else //���³���3��,�ɹ����������ģʽ,ʧ�ܼ�������ѭ��
						{
							for(int i = 0; i<30 ; ++i)
							{					
								Sleep(1000);
								CGetTimeMethod time;
								time.Execute();

								if(time.GetStatusCode() >= 0)
								{
									EnableNetwork();
									bTestBrokenStarted = FALSE;
									return;
								}
							}
						}
					}
				}
			}
		}

		Sleep(nInterval);

	}
	CoUninitialize();
}