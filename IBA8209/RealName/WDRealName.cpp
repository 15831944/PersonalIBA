#include "stdafx.h"
#include "Proxy.h"
#include "..\IBAHelpper.h"
#include "..\IBA.h"
#include "..\RegImageDlg.h"
#include "..\ServletHellper.h"
#include "..\dal\IBADAL.h"
#include "..\Socket\WinSocket.h"
#include "..\CurrentNetBarUser.h"
#include ".\wdrealname.h"
#include "..\..\GCardID_RZX\RealName.h"
#include "..\Socket\LocalServer.h"

using namespace NS_REALNAME;

CNetBarUser NS_REALNAME::CWDRealName::s_NetBarUser;
CWDRealName::CWDRealName(void)
/*: m_hIDReader(NULL), m_hIDRSUCore(NULL)
, m_hWdCheck(NULL),*/ 
: m_nPort(0), m_hRealname(NULL)
, m_bInitSuccessed(FALSE)
{
	CIBALog::GetInstance()->Write(_T("CWDRealName"));

//	m_nWDState = 4; //Ĭ��Ϊ4����������

	/*m_pfnInitCheck = NULL;
	m_pfnUnInitCheck = NULL;

	m_pfnIDReaderInit = NULL;
	m_pfnIDReaderUninit = NULL;
	m_pfnIDReaderSetEventWnd = NULL;

	m_pfnIDRSUCoreInit = NULL;
	m_pfnGetInfoFirstType = NULL;
	m_pfnGetInfoSecondType = NULL;
	m_pfnGetInfoSecondTypeMachine = NULL;
	m_pfnGetOtherType = NULL;*/
	
	m_pfunInit	 = NULL;
	m_pfunStop	 = NULL;

	/*CStringArray strCameras;
	CL2TKDirectShowCamera::EnumDevices(strCameras);

	for (INT i = 0; i < strCameras.GetCount(); i++)
	{
		if (0 == strCameras.GetAt(i).CompareNoCase(_T("USB ��Ƶ�豸")))
		{
			CIBAConfig::GetInstance()->SetPersonImageDrvId(i);
			break;
		}
	}*/

	Load();

	m_bNeedUpload = FALSE;

}

CWDRealName::~CWDRealName(void)
{
	UnLoad();
}

void CWDRealName::Load()
{
	//CRegKey RegKey;

	//if (ERROR_SUCCESS == RegKey.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Netshield5\\IDReader")))
	//{
	//	DWORD dwPort = 0;

	//	RegKey.QueryDWORDValue(_T("OpenPort"), dwPort);
	//	RegKey.Close();

	//	m_nPort = dwPort;
	//}

	//CString strWDPath;

	//if (ERROR_SUCCESS == RegKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\BARNAME")))
	//{
	//	TCHAR szPath[MAX_PATH] = {0};
	//	ULONG nLen = MAX_PATH;

	//	RegKey.QueryStringValue(_T("MyName"), szPath, &nLen);
	//	RegKey.Close();

	//	strWDPath = szPath;
	//}

	//strWDPath.MakeLower();
	//strWDPath.Replace(_T("upgrade.exe"), _T("Drv\\IDReader"));

	//CIBALog::GetInstance()->WriteFormat(_T("IDReader Path:%s"), strWDPath);

	////CIBAHelpper::AddEnvironmentVariable(strWDPath);

	//CIBAHelpper::SetDllDirectory(strWDPath);

	////******************************************************************************

	//m_hWdCheck = LoadLibrary(_T("WdCheck.dll")); 

	//if (m_hWdCheck != NULL)
	//{
	//	m_pfnInitCheck = (pfnInitCheck)GetProcAddress(m_hWdCheck, "InitCheck");
	//	m_pfnUnInitCheck = (pfnUnInitCheck)GetProcAddress(m_hWdCheck, "UninitCheck");
	//}
	//else
	//{
	//	CIBALog::GetInstance()->WriteFormat(_T("LoadLibrary WdCheck.dll Error = %d"), GetLastError());
	//}

	//m_hIDReader = LoadLibrary(_T("IDReader.dll"));

	//if (m_hIDReader != NULL)
	//{	
	//	m_pfnIDReaderInit = (pfnIDReaderInit)GetProcAddress(m_hIDReader, "Init");
	//	m_pfnIDReaderUninit = (pfnIDReaderUninit)GetProcAddress(m_hIDReader, "Uninit");
	//	m_pfnIDReaderSetEventWnd = (pfnIDReaderSetEventWnd)GetProcAddress(m_hIDReader, "SetEventWnd");
	//}
	//else
	//{
	//	AfxMessageBox(_T("��������ˢ������̬��(IDReader.dll)ʧ�ܣ��޷�ˢ����"));

	//	CIBALog::GetInstance()->WriteFormat(_T("LoadLibrary IDReader.dll Error = %d"), GetLastError());
	//}
	//
	//m_hIDRSUCore = LoadLibrary(_T("IDRSUCore.dll"));

	//if (m_hIDRSUCore != NULL)
	//{	
	//	m_pfnIDRSUCoreInit = (pfnIDRSUCoreInit)GetProcAddress(m_hIDRSUCore, "init");
	//	m_pfnGetInfoFirstType = (pfnGetInfoFirstType)GetProcAddress(m_hIDRSUCore, "GetInfoFirstType_1");
	//	m_pfnGetInfoSecondType = (pfnGetInfoSecondType)GetProcAddress(m_hIDRSUCore, "GetInfoSecondType_1");
	//	m_pfnGetInfoSecondTypeMachine = (pfnGetInfoSecondTypeMachine)GetProcAddress(m_hIDRSUCore, "GetInfoSecondTypeMachine_1");
	//	m_pfnGetOtherType = (pfnGetOtherType)GetProcAddress(m_hIDRSUCore, "GetOtherType_1");

	//	if (m_pfnIDRSUCoreInit != NULL)
	//	{
	//		INT nTmp = m_pfnIDRSUCoreInit();
	//		CIBALog::GetInstance()->WriteFormat(_T("m_pfnIDRSUCoreInit returm %d"), nTmp);
	//	}
	//}
	//else
	//{
	//	AfxMessageBox(_T("��������ɨ���Ƕ�̬��(IDRSUCore.dll)ʧ�ܣ��޷�ɨ�裡"));

	//	CIBALog::GetInstance()->WriteFormat(_T("LoadLibrary IDRSUCore.dll Error = %d"), GetLastError());
	//}
	
	m_hRealname = LoadLibrary(_T("realname.dll"));
	if(NULL != m_hRealname)
	{
		m_pfunInit= (FUN_Init)GetProcAddress(m_hRealname,"init");
		m_pfunStop= (FUN_Stop)GetProcAddress(m_hRealname,"stop");
	}
	else
	{
		AfxMessageBox(_T("�������ܶ�̬��(realname.dll)ʧ�ܣ�"));

		CIBALog::GetInstance()->WriteFormat(_T("LoadLibrary realname.dll Error = %d"), GetLastError());
	}
	
}

void CWDRealName::UnLoad()
{
	/*if (m_hWdCheck != NULL)
	{
		FreeLibrary(m_hWdCheck);
		m_hWdCheck = NULL;
	}

	if (m_hIDReader != NULL)
	{	
		FreeLibrary(m_hIDReader);
		m_hIDReader = NULL;
	}

	if (m_hIDRSUCore != NULL)
	{	
		FreeLibrary(m_hIDRSUCore);
		m_hIDRSUCore = NULL;
	}*/
	if(m_hRealname != NULL)
	{
		FreeLibrary(m_hRealname);
		m_hRealname = NULL;
	}
}

void  OnUserScan(CWDRealName::WDUserInfo *pUserInfo)
{
	IBA_LOG0(_T("����ˢ����Ϣ"));
	if(pUserInfo)
	{	
		CNetBarUser NetBarUser;
		// pUserInfo->EndDateû���õ�
		// pUserInfo->BirthDateû���õ�
		NetBarUser.SetUserName(CString(CA2T(pUserInfo->Name)));
		if (CString(CA2T(pUserInfo->Sex)).CompareNoCase(_T("��")) != 0)
		{
			NetBarUser.SetSex(1); //Ů��
		}
		else
		{
			NetBarUser.SetSex(0);	// �е�
		}
		NetBarUser.SetNation(CString(CA2T(pUserInfo->Nation)));
		NetBarUser.SetAddr(CString(CA2T(pUserInfo->Address)));
		if(0 == strcmp(pUserInfo->Type, "011"))
		{
			NetBarUser.SetCertificateType(12);	// Ŀǰ����ʹ�õ�ֻ�ж���֤
		}
		else
		{
			NetBarUser.SetCertificateType(99);
		}
		NetBarUser.SetDepartment(pUserInfo->FromDepart);
		NetBarUser.SetCertificateID(CString(CA2T(pUserInfo->Id)));

		NetBarUser.SetNetId(NetBarUser.GetCertificateID());
		NetBarUser.SetDataFrom(2);
		CWDRealName::s_NetBarUser = NetBarUser;
	}
	else
	{
		IBA_LOG0(_T("�û�ָ��Ϊ��"));
	}
	if(theApp.m_pMainWnd)
	{
		theApp.m_pMainWnd->SendMessage(WM_REALNAME, 0, 0);
	}
	else
	{
		IBA_LOG0(_T("������ָ��Ϊ��"));
	}
	
	//return FALSE;
	//SendMessage(g_hMainWnd,WU_USER_SCAN,0,(LPARAM)pUserInfo);
}

BOOL CWDRealName::InitRealName()
{
	CString strProxyIP = CNetBarConfig::GetInstance()->GetProxyIP();
	m_param.ConsumeType = 1;
	//strncpy(m_param.hostip, CT2A(CNetBarConfig::GetInstance()->GetProxyIP()), sizeof(m_param.hostip));	
	strncpy(m_param.ConsumeVer, CT2A(CIBALog::GetInstance()->GetAppVer()), sizeof(m_param.ConsumeVer));
	
	// 2014-6-10 - qsc ���������ӵ��ն����п��ܺ�ע���ն�����һ������������û�нӿڷ���ע���ն�����Ҳֻ�������������
	m_param.ConsumeCount = CIBADAL::GetInstance()->GetTableRecordCount(_T("sm_terminfo"));

	CString strIP, strMac;
	CIBAHelpper::GetIPAndMAC(strIP, strMac);
	strncpy(m_param.HostIp, CT2A(strIP), sizeof(m_param.HostIp));
	strncpy(m_param.MacAddr, CT2A(strMac), sizeof(m_param.MacAddr));
	strcpy(m_param.Expiration, "20501001");
	sprintf(m_param.ConsumeNo, "%d", CNetBarConfig::GetInstance()->GetNetBarId());

	BOOL bRet = TRUE;

	INT nRet = 0;

	//����ֵ�� -1 ����ʧ��   0 ���óɹ�
	//����1 ��ģ������ó���һ������������ó�����ͬһ��Ŀ¼
	//	    2 ����ʧ�ܣ��򵯳���ʾ��Ϣ�����ó����Զ��˳���

	if(m_pfunInit)
	{
		nRet = m_pfunInit(&m_param, OnUserScan);
		if(nRet == 0)
		{
			m_bInitSuccessed = TRUE;
			IBA_LOG0(_T("���ܳ�ʼ���ɹ���"));
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
			AfxMessageBox(_T("���ܳ�ʼ��ʧ�ܣ�"));
			IBA_LOG0(_T("���ܳ�ʼ��ʧ�ܣ�"));
			return bRet;
		}
	}
	else
	{
		IBA_LOG0(_T("���ܳ�ʼ������ָ��Ϊ��"));
	}
	return bRet;

	//if (m_pfnInitCheck != NULL)
	//{
	//	nRet = m_pfnInitCheck(CT2A(CNetBarConfig::GetInstance()->GetProxyIP()), 2);
	//	
	//	if (-1 == nRet)
	//	{
	//		bRet = FALSE;
	//	}

	//	CIBALog::GetInstance()->WriteFormat(_T("InitCheck Return %d"), nRet);
	//}
	//else
	//{
	//	CIBALog::GetInstance()->Write(_T("The m_pfnInitCheck is Null!"));

	//	bRet = FALSE;
	//}

	//if (!bRet)//�����ļ�У��ʧ��
	//{
	//	AfxMessageBox(_T("������Ϣ���ļ�У��ʧ�ܣ������˳���"));
	//	return bRet;
	//}

	//if (m_pfnIDReaderInit != NULL)
	//{
	//	nRet = m_pfnIDReaderInit(m_nPort, AfxGetMainWnd()->GetSafeHwnd());

	//	if (0 == nRet || 101 == nRet) //�ɹ�
	//	{
	//		if (m_pfnIDReaderSetEventWnd != NULL)
	//		{
	//			m_pfnIDReaderSetEventWnd(AfxGetMainWnd()->GetSafeHwnd(), NULL);
	//		}
	//		else
	//		{
	//			CIBALog::GetInstance()->Write(_T("The m_pfnIDReaderSetEventWnd is Null!"));
	//		}			
	//	}
	//	else
	//	{
	//		CIBALog::GetInstance()->WriteFormat(_T("Init IDReader Failed!(Return %d)"), nRet);
	//	}
	//}
	//else
	//{
	//	CIBALog::GetInstance()->Write(_T("The m_pfnIDReaderInit is Null!"));
	//}

	//if (m_pfnIDRSUCoreInit != NULL)
	//{
	//	m_pfnIDRSUCoreInit();
	//}
	//else
	//{
	//	CIBALog::GetInstance()->Write(_T("The m_pfnIDRSUCoreInit is Null!"));
	//}

	CRealNameBase::InitRealName();

	ExecuteProxy();
	
	return bRet;
}

BOOL CWDRealName::UnInitRealName()
{
	CRealNameBase::UnInitRealName();

	//if (m_pfnIDReaderUninit != NULL)
	//{
	//	//m_pfnIDReaderUninit(); //���ÿ��ܻ�������ܵ�BUG
	//}

	//if (m_pfnUnInitCheck != NULL)
	//{
	//	m_pfnUnInitCheck();
	//}

	//�ȱ��������ļ�
	CIBAConfig::GetInstance()->SaveConfig();

	if(m_pfunStop != NULL)
	{
		m_pfunStop();
	}
	else
	{
		IBA_LOG0(_T("����ֹͣ����ָ��Ϊ��"));
	}
	
	//��������ʵ�����ܻῨסIBA�������˳�����������������ǿ�ƽ�������
	TerminateProcess(GetCurrentProcess(), 0);

	return TRUE;
}

INT CWDRealName::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	IBA_LOG0(_T("WD:OnRealNameMsg"));
	m_NetBarUser = s_NetBarUser;

	CNetBarUser NetBarUser;
	s_NetBarUser = NetBarUser;	// �������
	
	if (!CheckCard(m_NetBarUser.GetNetId())) return TRUE;

	if (m_nMemberId > 0)
	{	
		m_NetBarUser.SetMemberId(m_nMemberId);

		if (QueryBalance())
		{	
			if (CDeregisterHelpper::AllowAutoRefund(CCurrentNetBarUser::GetInstance()->Balance)) //�����Զ��˿�����
			{
				if (CDeregisterHelpper::DeregisterUser(CCurrentNetBarUser::GetInstance()->Balance))
				{
					OnNewUser(m_NetBarUser);

					return TRUE; //�Զ��˿���
				}
			}
		}

		if (AutoModifyMemberInfo())
		{
			QueryBalance();
		}

		//ɨ����ϲ�������Ƭ������
		/*if (NetBarUser.GetCertificateType() != 99)
		{
			pRealName->UploadImage(1, NetBarUser, TRUE);
		}*/

		OnOldUser(m_NetBarUser);
	}
	else
	{
		// 2013-12-6 15:31:50 qsc
		if (AutoMatchIDCard(m_NetBarUser))
		{
			AfxGetMainWnd()->BringWindowToTop();
			AfxGetMainWnd()->SetForegroundWindow();

			OnOldUser(m_NetBarUser);
		}
		else
		{
			OnNewUser(m_NetBarUser);
		}
	}
	return TRUE;

	//UNIT_AUTO_TRACE("CWDRealName::OnRealNameMsg(UINT nMsgType=%d, LPVOID lpData=%x, DWORD dwLen=%d)"
	//	,nMsgType,lpData,dwLen);

	//CRealNameBase::OnRealNameMsg(nMsgType, lpData, dwLen);

	//if (0 == nMsgType)
	//{
	//	ID_CARD_INFO id_card_info = {0};
	//	
	//	CopyMemory(&id_card_info, lpData, dwLen);

	//	CString strIdCardNumber(CA2T(id_card_info.szISN));

	//	strIdCardNumber.Trim();

	//	if (strIdCardNumber.IsEmpty())
	//	{
	//		IBA_LOG0(_T("error: ����Ϊ��!"));
	//		return 0;
	//	}

	//	CNetBarUser NetBarUser;
	//	//NetBarUser.SetIDNo(strIdCardNumber);
	//	NetBarUser.SetNetId(strIdCardNumber);

	//	if (!CheckCard(strIdCardNumber))
	//	{
	//		return 0;
	//	}

	//	ReplyMessage(0);

	//	if (m_nMemberId > 0) //�Ͽ����ȼ���
	//	{
	//		NetBarUser.SetMemberId(m_nMemberId);

	//		if (QueryBalance())
	//		{	
	//			if (CDeregisterHelpper::AllowAutoRefund(CCurrentNetBarUser::GetInstance()->Balance)) //�����Զ��˿�����
	//			{
	//				if (CDeregisterHelpper::DeregisterUser(CCurrentNetBarUser::GetInstance()->Balance))//�Զ��˿���
	//				{
	//					//��������û�����ݴ������ǣ����������û�����

	//					NetBarUser.SetCertificateID(strIdCardNumber);
	//					NetBarUser.SetCertificateType(99);
	//					NetBarUser.SetUserName(_T("�����û�"));
	//					NetBarUser.SetSex(0);
	//					NetBarUser.SetDataFrom(1); //ˢ��

	//					OnNewUser(NetBarUser);

	//					return 0; 
	//				}
	//			}
	//		}

	//		OnOldUser(NetBarUser);
	//	}
	//	else //�¿����Ʒ�δע��
	//	{
	//		//��������û�����ݴ������ǣ����������û�����

	//		NetBarUser.SetCertificateID(strIdCardNumber);
	//		NetBarUser.SetCertificateType(99);
	//		NetBarUser.SetUserName(_T("�����û�"));
	//		NetBarUser.SetSex(0);
	//		NetBarUser.SetDataFrom(1); //ˢ��

	//		OnNewUser(NetBarUser);
	//	}
	//}

	return 0;
}

BOOL CWDRealName::IsWdWorking()
{
	// 2012/03/21-8238-gxx: ���ܵĸ�����̨�������
	if (CIBAConfig::GetInstance()->GetCashRegisterID() != 1)
	{
		return TRUE;
	}

	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("T.C. for Server"));

	if (hMutex != NULL)
	{
		CloseHandle(hMutex);

		return TRUE;
	}

	return FALSE;
}

BOOL CWDRealName::CheckRealName()
{
	CRealNameBase::CheckRealName();

	//GetWDState();

	//if (2 == m_nWDState) //����Ҫ���˳�
	//{	
	//	CIBALog::GetInstance()->Write(_T("����Ҫ���˳���"));

	//	TerminateProcess(GetCurrentProcess(), 0);

	//	return FALSE;
	//}

	return TRUE;
}

BOOL CWDRealName::IsScanEnabled()
{
	//if (theApp.GetCurCashier()->IsOnDuty())
	//{
	//	//return (m_hIDRSUCore != NULL);
	//	return m_bInitSuccessed;
	//}
	
	return FALSE;
}

BOOL CWDRealName::ScanUserInfo(UINT nType, CNetBarUser& NetBarUser, UINT nOption)
{
	/*if (!IsWdWorking())
	{
		AfxMessageBox(_T("������Ϣ����ѡ���С������ۺϹ���ϵͳ����"));

		return FALSE;
	}*/

	//NetBarUser.SetCanEditUserInfo(FALSE);

	//WDUserInfo wdUserInfo = {0};

	//INT nRet = 0;

	//switch (nType)
	//{
	//case 1:
	//	IBA_LOG0(_T("ɨ��һ��֤..."));
	//	
	//	NetBarUser.SetCertificateType(11);

	//	if (m_pfnScan_fst != NULL)
	//	{
	//		nRet = m_pfnScan_fst(&wdUserInfo, m_param.mykey);
	//	}
	//	else
	//	{
	//		CIBALog::GetInstance()->Write(_T("The m_pfnGetInfoFirstType is Null!"));
	//	}

	//	break;

	//case 2:
	//	IBA_LOG0(_T("ɨ�����֤..."));

	//	NetBarUser.SetCertificateType(12);

	//	if (m_pfnScan_sec != NULL)
	//	{
	//		nRet = m_pfnScan_sec(&wdUserInfo, m_param.mykey);
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("The m_pfnGetInfoSecondType is Null!"));
	//	}

	//	break;

	//case 3:
	//	IBA_LOG0(_T("��������֤..."));

	//	NetBarUser.SetCertificateType(12);

	//	if (m_pfnRead_sec != NULL)
	//	{
	//		nRet = m_pfnRead_sec(&wdUserInfo, m_param.mykey);
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("The m_pfnGetInfoSecondTypeMachine is Null!"));
	//	}

	//	break;

	//default:
	//	IBA_LOG0(_T("ɨ������֤��..."));

	//	NetBarUser.SetCanEditUserInfo(TRUE);
	//	NetBarUser.SetCertificateType(99);
	//	
	//	if (m_pfnFill_other != NULL)
	//	{
	//		nRet = m_pfnFill_other(&wdUserInfo, m_param.mykey);
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("The m_pfnGetOtherType is Null!"));
	//	}

	//	break;
	//}

	//IBA_LOG(_T("WD scan return: %d"), nRet);

	//nRet = ((0 == nRet) ? TRUE : FALSE);
	//if (nRet)
	//{
	//	try
	//	{
	//		NetBarUser.SetCertificateID(CString(CA2T(wdUserInfo.Id)));
	//		NetBarUser.SetUserName(CString(CA2T(wdUserInfo.Name)));
	//		NetBarUser.SetAddr(CString(CA2T(wdUserInfo.Address)));
	//		NetBarUser.SetNation(CString(CA2T(wdUserInfo.Nation)));
	//		NetBarUser.SetDepartment(CString(CA2T(wdUserInfo.FromDepart)));
	//		//NetBarUser.SetIDNo(NetBarUser.GetCertificateID());
	//		NetBarUser.SetNetId(NetBarUser.GetCertificateID());
	//		NetBarUser.SetDataFrom(2);

	//		NetBarUser.SetScanType(nType);

	//		if (CString(CA2T(wdUserInfo.Sex)).CompareNoCase(_T("��")) != 0)
	//		{
	//			NetBarUser.SetSex(1); //Ů��
	//		}
	//		m_NetBarUser = NetBarUser;

	//		////��ֵģ�飬ɨ����ϴ���Ƭ������
	//		//if (0 == nOption /*&& NetBarUser.GetCertificateType() != 99*/ )
	//		//{
	//		//	UploadImage(1, NetBarUser, TRUE);
	//		//}

	//		m_bNeedUpload = TRUE;

	//		IBA_LOG(_T("ScanInfo: ֤������=%s, ����=%s"), NetBarUser.GetCertificateID(),NetBarUser.GetUserName());
	//	}
	//	catch (...)
	//	{
	//		IBA_LOG0(_T("ɨ���������!"));
	//	}

	//}
	//else
	//{
	//	CIBALog::GetInstance()->Write(_T("WD scan failed!"));
	//}

	//return nRet;
	return TRUE;
}

BOOL CWDRealName::UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption)
{
	//BOOL bScan = nOption;

	//if (!bScan) //�Ƿ�ɨ�裬ɨ����֤����Ҫ�ϴ���
	//{
	//	return FALSE;
	//}

	//UINT nUploadImageOption = 0;

	//if (CNetBarConfig::GetInstance()->GetUploadImage() == 3) //ǿ�Ƶ��������ɹر�
	//{
	//	nUploadImageOption |= CUploadImageDlg::UI_UPLOAD_NOTCLOSE;
	//}
	//else if (CNetBarConfig::GetInstance()->GetUploadImage() == 2) //ǿ�Ƶ���
	//{

	//}
	//else
	//{
	//	CIBALog::GetInstance()->Write(_T("�������ò����ϴ�ͼƬ��"));

	//	return FALSE;
	//}

	//if (nType == 0) //�����Զ�����
	//{

	//}
	//else if (nType == 1) //��ֵ
	//{
	//	//
	//}

	//m_NetBarUser = NetBarUser;

	//m_bNeedUpload = TRUE;

	//Start();

	return TRUE;

}

int CWDRealName::Run()
{
	//CoInitialize(NULL);

	//m_bNeedUpload = FALSE;

	//DeleteFile(theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg"));

	//if (!PathFileExists(_T("C:\\21.bmp")))
	//{
	//	IBA_LOG0(_T("C:\\21.bmp not found!"));
	//	return 0;
	//}

	//CImageEx TmpImage;
	//TmpImage.Load(_T("C:\\21.bmp"));
	//TmpImage.SaveAsJpeg(theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg"), 20);

	//CL2TKDirectShowCamera::ShootPicture(CIBAConfig::GetInstance()->GetPersonImageDrvId(), 
	//	theApp.GetWorkPath() + _T("\\IBATemp\\Person.jpg"));

	//if (SendImage(1))
	//{
	//	SendImage(2);
	//}

	////ɾ��ͼƬ�ļ�
	//DeleteFile(_T("C:\\21.bmp"));
	//DeleteFile(_T("C:\\22.bmp"));
	//DeleteFile(theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg"));
	//DeleteFile(theApp.GetWorkPath() + _T("\\IBATemp\\Person.jpg"));

	//CoUninitialize();

	return 0;
}

BOOL CWDRealName::SendToProxy(CByteArray &buffer)
{
	BOOL bRet = FALSE;

	CWinSocket ProxySocket;

	if (ProxySocket.Create())
	{
		if (ProxySocket.Connect(CNetBarConfig::GetInstance()->GetProxyIP(), 7834, 5000))
		{
			if (ProxySocket.Send(buffer.GetData(), buffer.GetSize()) > 0)
			{
				bRet = TRUE;
			}
		}
		else
		{
			IBA_LOG(_T("Can't connect to %s(7834)!"), CNetBarConfig::GetInstance()->GetProxyIP());
		}

		ProxySocket.Close();
	}

	return bRet;
}

BOOL CWDRealName::SendImage(UINT nType)
{
	//NET_HEAD head = {0};
	//head.Version = COM_VER_1;
	//head.Length = sizeof(ID_IMAGE);

	//CString strFile(theApp.GetWorkPath() + _T("\\IBATemp\\"));

	//if (nType == 1)
	//{
	//	head.Cmd = C_IDIMG;
	//	strFile.Append(_T("ID.jpg"));
	//}
	//else
	//{
	//	head.Cmd = C_CAPIMG;
	//	strFile.Append(_T("Person.jpg"));
	//}

	//IBA_LOG(_T("����ͼƬ:%s"), strFile);

	//ID_IMAGE IDImage = {0};

	//IDImage.idtype = m_NetBarUser.GetCertificateType();

	//if (m_NetBarUser.GetNetId().GetLength() == 11 && m_NetBarUser.GetNetId().GetAt(0) == _T('1'))
	//{
	//	CStringA strAID = CT2A(m_NetBarUser.GetCertificateID());
	//	CStringA strAPhone = CT2A(m_NetBarUser.GetNetId());
	//	_snprintf(IDImage.IdNumber, LEN_ID+1, "%s|%s", strAID.GetString(), strAPhone.GetString());
	//}
	//else
	//{
	//	lstrcpynA(IDImage.IdNumber, CT2A(m_NetBarUser.GetCertificateID()), LEN_ID);
	//}
	//
	//lstrcpynA(IDImage.name, CT2A(m_NetBarUser.GetUserName()), LEN_NAME);

	//CFile ImageFile;

	//if (ImageFile.Open(strFile, CFile::modeRead)) 
	//{
	//	if (ImageFile.GetLength() < LEN_IMAGE) 
	//	{
	//		ImageFile.Read(IDImage.image, (UINT)ImageFile.GetLength());
	//		IDImage.imageSize = (INT)ImageFile.GetLength();		
	//	}
	//	else
	//	{
	//		//��ʲô�أ��ļ�̫����
	//		IBA_LOG0(_T("Loading image error: the image file is too large!"));
	//		return FALSE;
	//	}

	//	ImageFile.Close();
	//	DeleteFile(strFile);
	//}
	//else
	//{
	//	CIBALog::GetInstance()->WriteFormat(_T("Failed to open %s!"), strFile);

	//	return FALSE;
	//}

	//CByteArray buffer;
	//buffer.SetSize(sizeof NET_HEAD + head.Length);

	//memcpy(buffer.GetData(), &head, sizeof NET_HEAD);
	//memcpy(buffer.GetData() + sizeof NET_HEAD, &IDImage, head.Length);

	//return SendToProxy(buffer);
	return TRUE;
}

BOOL CWDRealName::IsRegisterEnabled()
{
	/*if (!IsWdWorking())
	{
		AfxMessageBox(_T("������Ϣ����ѡ���С������ۺϹ���ϵͳ����"));

		return FALSE;
	}

	BOOL bRet = (m_nWDState != 1);
	
	if (!bRet)
	{
		AfxMessageBox(_T("������Ϣ����ǰ����������"));
	}
	
	return bRet;*/
	return TRUE;
}

BOOL CWDRealName::IsCheckInEnabled()
{
	/*if (!IsWdWorking())
	{
		AfxMessageBox(_T("������Ϣ����ѡ���С������ۺϹ���ϵͳ����"));

		return FALSE;
	}

	BOOL bRet = (m_nWDState != 1);

	if (!bRet)
	{
		AfxMessageBox(_T("������Ϣ����ǰ���������ϻ���"));
	}

	return bRet;*/
	return TRUE;
}

void CWDRealName::GetWDState()
{
	return ;
	//CConfigInfo configInfo;
	//configInfo.SetKeyString(_T("WDSTATE"));

	//if (!CIBADAL::GetInstance()->GetSystemConfig(configInfo))
	//{
	//	CIBALog::GetInstance()->Write(_T("Read WDSTATE Error!"));

	//	return;
	//}

	////Cmd: �������ͣ� // 1 ��ֹ�ϻ� 2 �����˳����˳�  3 ������ʾ��Ϣ 4 ��������

	//if (configInfo.GetKeyValue1() == 0 || configInfo.GetKeyValue1() == m_nWDState)
	//{
	//	CIBALog::GetInstance()->Write(_T("WDState no change: m_nWDState-%d, KeyValue1-%d"), m_nWDState, configInfo.GetKeyValue1());
	//	
	//	return;
	//}

	//m_nWDState = configInfo.GetKeyValue1();

	//CString strTmp(_T("������Ϣ��"));
	//
	//strTmp.Append(configInfo.GetKeyValue3());

	//CIBALog::GetInstance()->Write(strTmp);

	//switch (configInfo.GetKeyValue1())
	//{
	//case 1:		
	//case 2:
	//case 3:
	//	AfxMessageBox(strTmp);
	//	break;

	//case 4:
	//	break;
	//}
}

BOOL CWDRealName::AutoModifyMemberInfo()
{
	if (CNetBarConfig::GetInstance()->GetNoModifyMember() == 0)
	{
		return FALSE;
	}

	//ֻ��֤���������ж�

	if (m_NetBarUser.GetCertificateType() == CCurrentNetBarUser::GetInstance()->Balance.GetIdType())
	{
		return FALSE;
	}

	CModifyMemberMethod ModifyMember;//ֻ����ʵ�����ڵ�����

	ModifyMember.SetUsername(CCurrentNetBarUser::GetInstance()->Balance.GetUserName());
	ModifyMember.SetOpenNetBarId(CCurrentNetBarUser::GetInstance()->Balance.GetOpenNetbarId());
	ModifyMember.SetPersonalId(CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId());//ʵ��֤����
	ModifyMember.SetOldPersonalId(CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId());//����ԭ֤����
	ModifyMember.SetMemberName(CCurrentNetBarUser::GetInstance()->Balance.GetMemberName());
	ModifyMember.SetSex(CCurrentNetBarUser::GetInstance()->Balance.GetSex());
	ModifyMember.SetIdType(m_NetBarUser.GetCertificateType());//ʵ������
	ModifyMember.SetPassword(_T("PASSWORDPASSWORD"));

	theApp.GetCurCashier()->DoModifyMember(ModifyMember);

	return TRUE;
}

BOOL CWDRealName::NeedToCheckRegisterData()
{
	return TRUE; //������ҪУ��
}

BOOL CWDRealName::DoScan(UINT nType)
{
	return TRUE;
	//CNetBarUser NetBarUser;

	//if (ScanUserInfo(nType, NetBarUser, TRUE))
	//{
	//	//if (!CheckCard(NetBarUser.GetIDNo())) return TRUE;
	//	if (!CheckCard(NetBarUser.GetNetId())) return TRUE;

	//	if (m_nMemberId > 0)
	//	{	
	//		NetBarUser.SetMemberId(m_nMemberId);

	//		if (QueryBalance())
	//		{	
	//			if (CDeregisterHelpper::AllowAutoRefund(CCurrentNetBarUser::GetInstance()->Balance)) //�����Զ��˿�����
	//			{
	//				if (CDeregisterHelpper::DeregisterUser(CCurrentNetBarUser::GetInstance()->Balance))
	//				{
	//					OnNewUser(NetBarUser);

	//					return TRUE; //�Զ��˿���
	//				}
	//			}
	//		}

	//		if (AutoModifyMemberInfo())
	//		{
	//			QueryBalance();
	//		}

	//		//ɨ����ϲ�������Ƭ������
	//		if (NetBarUser.GetCertificateType() != 99)
	//		{
	//			UploadImage(1, NetBarUser, TRUE);
	//		}

	//		OnOldUser(NetBarUser);
	//	}
	//	else
	//	{
	//		OnNewUser(NetBarUser);
	//	}

	//	return TRUE;
	//}

	//return FALSE;
}