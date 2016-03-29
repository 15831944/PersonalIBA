#include "stdafx.h"
#include "netbarconfig.h"
#include "IBAConfig.h"
#include "IBA.h"
#include "Encrypt.h"
#include "IBAHelpper.h"
#include ".\dal\IBADAL.h"
#include "Cashier.h"

using namespace NS_DAL;

#define WRITE_CONFIG(refe,newValue,key,valueAt) \
	refe = newValue;\
	CConfigInfo configInfo;\
	configInfo.SetKeyString(key);\
	configInfo.SetKeyValue##valueAt(newValue);\
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, valueAt);

SINGLETON_IMPLEMENT(CNetBarConfig)

CNetBarConfig::CNetBarConfig(void)
: m_NetBarId(0), m_MainCenterPort(8080), m_BackupCenterPort(8080)
, m_strBossPwd(_T("6CC20790A53320DC85723029796DB837"))
, m_bLoadFromDB(FALSE), m_nUploadImage(0), m_nNoModifyMember(0)
, m_nShenzhen(0), m_nMustScan(0), m_nTempMember(0), m_nNeedActivation(0)
, m_nRealNamePassword(0), m_nAutoMatchIDCard(0), m_nRoundType(0)
, m_nSoftwareType(0), m_bBundleTimeWithCash(FALSE)
, m_nDisallowUnlock(0), m_nStoreInAmount(0), m_nInputActivation(0)
, m_bMustSynchronize(FALSE), m_bCreditDirect(FALSE)
{

//���Բ�����ʼ����������Ҫ����̨����
#ifdef _DEBUG
	m_nInputActivation = 1;
#endif // _DEBUG

	m_strIniFile = theApp.GetWorkPath();
	m_strIniFile.Append(_T("\\IBAConfig\\IBA.ini"));

	GPropertyInit(BTLimitTime, 50);// ��ʱ����ʱ��
	GPropertyInit(MustAutoReturn, 0);
	GPropertyInit(ScanEnabled, 0);
	GPropertyInit(KillCaptions, _T("")); // ȱʡ��ǿ�ƹر��κθ�������
	GPropertyInit(ForbidModifyPwd, 0 );
	GPropertyInit(LocalModeStartKind, CIBAGlobal::emLocalModeForbid);
	GPropertyInit(ReminderBalance, TRUE);
	GPropertyInit(ReminderAutoReturn, FALSE);
	GPropertyInit(UserRCheckIn, FALSE);
	GPropertyInit(ForbidCaptureScreen, FALSE);

	if (!LoadFromDB()) //����Ļ���ʹ�������ļ�
	{
		AfxMessageBox(IDS_CONNETCTDBFIALED);

		LoadFromIni();//�ӳ�ʼ���ļ�����
	}

	

	
}

CNetBarConfig::~CNetBarConfig(void)
{
	SaveToIni();
}

BOOL CNetBarConfig::LoadFromDB()
{
	CString strTmp;
	CConfigInfo configInfo;
	BOOL bReadSuccess = TRUE;

#define READ_CONFIG(refe,key,valueAt,defaultValue) \
	configInfo.Clear();\
	configInfo.SetKeyString(key);\
	if (CIBADAL::GetInstance()->GetSystemConfig(configInfo))\
	{\
	refe = configInfo.GetKeyValue##valueAt();\
	bReadSuccess = TRUE;\
	}\
	else\
	{\
	bReadSuccess = FALSE;\
	refe = defaultValue;\
	}
#define READ_CONFIG2(refe,key,valueAt,logOut) \
	configInfo.Clear();\
	configInfo.SetKeyString(key);\
	if (CIBADAL::GetInstance()->GetSystemConfig(configInfo))\
	{\
	bReadSuccess = TRUE;\
	refe = configInfo.GetKeyValue##valueAt();\
	}\
	else\
	{\
	bReadSuccess = FALSE;\
	CIBALog::GetInstance()->Write(logOut);\
	}

	//����̨�汾ID
	strTmp = _T("0");
	READ_CONFIG2(strTmp,_T("ICControlVerID"),3,_T("Read ICControlVerID Error!"));
	m_nICControlVerID = _ttoi(strTmp);

	
	//����ID,��ID,������Կ{
	configInfo.SetKeyString(_T("currNetBar"));

	if (CIBADAL::GetInstance()->GetSystemConfig(configInfo))
	{
		m_NetBarId = configInfo.GetKeyValue1();
		m_DomainId = configInfo.GetKeyValue2();
		m_NetBarKey = configInfo.GetKeyValue3();
	}
	else
	{
		CIBALog::GetInstance()->Write(_T("Read NetBarId Error!"));
		return FALSE;
	}
	//--------------------------------------------------------------------}

	//������Ϣ
	if (!CIBADAL::GetInstance()->GetAllNetBarInfo(m_NetBarInfoArray))
	{
		CIBALog::GetInstance()->Write(_T("Read NetBarInfo Error!"));
	}
	m_strNetBarName = GetNetBarName(GetNetBarId());

	//��������
	if (!CIBADAL::GetInstance()->GetAllPCClassInfo(m_PCClassInfoArray))
	{
		CIBALog::GetInstance()->Write(_T("Read PCClass Error!"));
	}

	//֤������
	if (!CIBADAL::GetInstance()->GetAllIDTypeInfo(m_IDTypeInfoArray))
	{
		CIBALog::GetInstance()->Write(_T("Read IDType Error!"));
	}

	//��Ա����

	if (!CIBADAL::GetInstance()->GetAllUserClassInfos(m_UserClassArray))
	{
		CIBALog::GetInstance()->Write(_T("Read UserClass Error!"));
	}

	//�����������Ϣ{
	//IP
	READ_CONFIG2(m_MainCenterIP,_T("ICServerIPAddress"),3,
		_T("Read MainCenterIP Error!"));

	READ_CONFIG2(strTmp,_T("ICServerPort"),3,
		_T("Read ICServerPort Error!"));
	if (bReadSuccess)
	{
		INT nIndex = strTmp.Find('/', 0);

		if (nIndex > 0)
		{
			m_MainCenterPath = strTmp.Mid(nIndex + 1);
			strTmp = strTmp.Left(nIndex);
			m_MainCenterPort = _ttoi(strTmp);
		}
	}
	//--------------------------------------------------------------------}

	//������IP
	READ_CONFIG2(m_BackupCenterIP,_T("ICGetTimeIPAddress"),3,
		_T("Read BackupCenterIP Error!"));

	//�������IP
	READ_CONFIG2(m_strStandbyHost,_T("standbyHost"),3,
		_T("Read standbyHost Error!"));

	//******************************************************************************
	READ_CONFIG2(strTmp,_T("ICGetTimePort"),3,
		_T("Read BackupCenterPort Error!"));
	if (bReadSuccess)
	{
		INT nIndex = strTmp.Find('/', 0);
		if (nIndex > 0)
		{
			m_BackupCenterPath = strTmp.Mid(nIndex + 1);

			strTmp = strTmp.Left(nIndex);
			m_BackupCenterPort = _ttoi(strTmp);
		}
	}

	//���ط���IP{
	READ_CONFIG(m_LocalServerIP,_T("serverIP"),3,_T(""));
	if (bReadSuccess)
	{
		m_LocalServerIP.Trim();

		//���±������ĵ�ַ
		configInfo.Clear();
		configInfo.SetKeyString(_T("localCenterAddress"));

		CString strTmp(m_LocalServerIP);
		strTmp.Append(_T(":8080"));
		configInfo.SetKeyValue3(strTmp);
		CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
	}
	else
	{
		CIBALog::GetInstance()->Write(_T("Read LocalServerIP Error!"));
	}

	if (m_LocalServerIP.IsEmpty())
	{
		CString strMAC;
		CIBAHelpper::GetIPAndMAC(m_LocalServerIP, strMAC);
	}
	//--------------------------------------------------------------------}

	READ_CONFIG(m_strProxyIP,_T("AuditProxyAddr"),3,_T("")); //PROXY
	if (bReadSuccess)
	{
		m_strProxyIP.Trim();
		if (m_strProxyIP.IsEmpty())
		{
			m_strProxyIP = m_LocalServerIP;
		}
	}
	else
	{
		CIBALog::GetInstance()->Write(_T("Read AuditProxyAddr Error!"));
		return FALSE;
	}

	READ_CONFIG2(m_strUnLockPassword,_T("clientPassword"),3,
		_T("Read UnLockPassword Error!")); // �ͻ��˽�������

	READ_CONFIG2(m_strBossPwd,_T("BossPwd"),3,
		_T("Read BossPwd Error!")); // �ϰ�����

	m_nRoundType = CIBADAL::GetInstance()->GetRoundType(m_NetBarId);

	m_bLoadFromDB = TRUE;

	// 2011/06/20-gxx: ��ʱʱ������, Ĭ��50����
	READ_CONFIG(BTLimitTime,_T("BTLimitTime"),1,50); 

	// 2011/07/19-8201-gxx: ��ȡʵ������
	READ_CONFIG(m_nAuditType,_T("AuditProxyAddr"),1,0);   

	// 2011/07/20-8201-gxx: ��ȡ�������
	READ_CONFIG(m_nSoftwareType, _T("SoftwareType"),1,2); 

	// 2011/07/20-8201-gxx: 
	READ_CONFIG(m_ForeRealName,_T("ForeRealName"),1,0);

	// 2011/08/10-8201-gxx: 
	READ_CONFIG(LocalModeStartKind, _T("LocalModeStartKind"),1,CIBAGlobal::emLocalModeManual);

	// 2011/09/07-8201-gxx: Ĭ��������������
	READ_CONFIG(ReminderBalance, _T("ReminderBalance"),1,TRUE); 

	// 2011/10/18-8206-gxx: 
	READ_CONFIG(ReminderAutoReturn, _T("ReminderAutoReturn"),1,FALSE);

	// 2011/12/12-8235-gxx: �Ƿ�����Ҽ��ϻ�����
	READ_CONFIG(UserRCheckIn, _T("UserRCheckIn"),1,FALSE);

	// 2012/06/28-8245-gxx: �Ƿ��ֹ����ԱԶ�̽���
	READ_CONFIG(ForbidCaptureScreen, _T("ForbidCaptureScreen"),1,FALSE);


	return TRUE;
}

void CNetBarConfig::LoadFromIni()
{
	ReadValue(_T("NetBar"), _T("DomainId"), m_DomainId);
	ReadValue(_T("NetBar"), _T("NetBarId"), m_NetBarId);
	ReadValue(_T("NetBar"), _T("NetBarPSK"), m_NetBarKey);
	ReadValue(_T("NetBar"), _T("NetBarName"), m_strNetBarName);
	ReadValue(_T("NetBar"), _T("BossPwd"), m_strBossPwd);
	ReadValue(_T("NetBar"), _T("RoundType"), m_nRoundType);

	ReadValue(_T("Center"), _T("MainIP"), m_MainCenterIP);
	ReadValue(_T("Center"), _T("MainPort"), m_MainCenterPort);
	ReadValue(_T("Center"), _T("MainPath"), m_MainCenterPath);

	ReadValue(_T("Center"), _T("BackUpIP"), m_BackupCenterIP);
	ReadValue(_T("Center"), _T("BackUpPort"), m_BackupCenterPort);
	ReadValue(_T("Center"), _T("BackUpPath"), m_BackupCenterPath);

	ReadValue(_T("IServer"), _T("IServerIP"), m_LocalServerIP);
	ReadValue(_T("IBA"), _T("SoftwareType"), m_nSoftwareType, 2);

	CString strTmp;

	ReadValue(_T("Register"), _T("IDTypeInfos"), strTmp);

	CStringArray strArray;
	CIBAHelpper::SplitLine(strTmp, strArray);

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		CIDTypeInfo IDTypeInfo;

		CString strIDTypeInfo = strArray.GetAt(i);
		
		INT nIndex = strIDTypeInfo.Find(':');
		
		if (nIndex > 0)
		{
			INT nTmp = _ttoi(strIDTypeInfo.Left(nIndex));

			IDTypeInfo.SetIDTypeID(nTmp);
			IDTypeInfo.SetIDTypeName(strIDTypeInfo.Mid(nIndex + 1));

			m_IDTypeInfoArray.Add(IDTypeInfo);
		}
	}

	ReadValue(_T("NetBar"), _T("PCClassInfos"), strTmp);

	CIBAHelpper::SplitLine(strTmp, strArray);

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		CIDNameRecord IDNameRecord;

		CString strPCClassInfo = strArray.GetAt(i);

		INT nIndex = strPCClassInfo.Find(':');

		if (nIndex > 0)
		{
			INT nTmp = _ttoi(strPCClassInfo.Left(nIndex));

			IDNameRecord.SetID(nTmp);
			IDNameRecord.SetName(strPCClassInfo.Mid(nIndex + 1));

			m_PCClassInfoArray.Add(IDNameRecord);
		}
	}
}

void CNetBarConfig::SaveToIni()
{
	SaveValue(_T("NetBar"), _T("DomainId"), m_DomainId);
	SaveValue(_T("NetBar"), _T("NetBarId"), m_NetBarId);
	SaveValue(_T("NetBar"), _T("NetBarPSK"), m_NetBarKey);
	SaveValue(_T("NetBar"), _T("NetBarName"), m_strNetBarName);
	SaveValue(_T("NetBar"), _T("BossPwd"), m_strBossPwd);

	SaveValue(_T("Center"), _T("MainIP"), m_MainCenterIP);
	SaveValue(_T("Center"), _T("MainPort"), m_MainCenterPort);
	SaveValue(_T("Center"), _T("MainPath"), m_MainCenterPath);

	SaveValue(_T("Center"), _T("BackUpIP"), m_BackupCenterIP);
	SaveValue(_T("Center"), _T("BackUpPort"), m_BackupCenterPort);
	SaveValue(_T("Center"), _T("BackUpPath"), m_BackupCenterPath);

	SaveValue(_T("IServer"), _T("IServerIP"), m_LocalServerIP);
	SaveValue(_T("IBA"), _T("SoftwareType"), m_nSoftwareType);

	CString strTmp;

	for (INT i = 0; i < m_IDTypeInfoArray.GetCount(); i++)
	{
		strTmp.AppendFormat(_T("%d:%s|"),
			m_IDTypeInfoArray.GetAt(i).GetIDTypeID(),
			m_IDTypeInfoArray.GetAt(i).GetIDTypeName());
	}

	SaveValue(_T("Register"), _T("IDTypeInfos"), strTmp);

	strTmp.Empty();

	for (INT i = 0; i < m_PCClassInfoArray.GetCount(); i++)
	{
		strTmp.AppendFormat(_T("%d:%s|"),
			m_PCClassInfoArray.GetAt(i).GetID(),
			m_PCClassInfoArray.GetAt(i).GetName());
	}

	SaveValue(_T("NetBar"), _T("PCClassInfos"), strTmp);

	SaveValue(_T("NetBar"), _T("RoundType"), m_nRoundType);
}


UINT CNetBarConfig::GetNetBarId() const
{
	return m_NetBarId;
}

CString CNetBarConfig::GetMainCenterIP() const
{
	return m_MainCenterIP;
}

CString CNetBarConfig::GetNetBarName() const
{
	return m_strNetBarName;
}

CString CNetBarConfig::GetNetBarName(UINT nNetbarId) const
{
	for (INT i = 0; i < m_NetBarInfoArray.GetCount(); i++)
	{
		if (m_NetBarInfoArray.GetAt(i).GetNetBarID() == nNetbarId)
		{
			return m_NetBarInfoArray.GetAt(i).GetName();
		}
	}

	CString strTmp;
	strTmp.Format(_T("%d"), nNetbarId);

	return strTmp;
}

CString CNetBarConfig::GetLocalServerIP() const
{
	return m_LocalServerIP;
}

void CNetBarConfig::SetLocalServerIP(CString newVal)
{
	m_LocalServerIP = newVal;
	
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("serverIP"));
	configInfo.SetKeyValue3(newVal);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

}

BOOL CNetBarConfig::GetValue(CString strSrc, LPCTSTR strKey, INT & nValue)
{
	strSrc.MakeLower();

	if (strSrc.Find(strKey) >= 0)
	{
		INT nIndex = strSrc.Find(_T("="));

		if (nIndex >= 0)
		{
			strSrc = strSrc.Mid(nIndex + 1);
			strSrc.Trim();
			nValue = _ttoi(strSrc);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CNetBarConfig::GetValue(CString strSrc, LPCTSTR strKey, CString & strValue)
{
	strSrc.MakeLower();

	if (strSrc.Find(strKey) >= 0)
	{
		INT nIndex = strSrc.Find(_T("="));

		if (nIndex >= 0)
		{
			strSrc = strSrc.Mid(nIndex + 1);
			strSrc.Trim();
			strValue = strSrc;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CNetBarConfig::PraseControlOption(CString strInfo)
{
	m_strControlOptionArray.RemoveAll();

	CIBAHelpper::SplitLine(strInfo, m_strControlOptionArray, ';');

	CString strTmp;

	for (INT i = 0; i < m_strControlOptionArray.GetCount(); i++)
	{
		strTmp = m_strControlOptionArray.GetAt(i);
	
		// ע: GetValue�ĵڶ���������Ҫȫ��Сд
		//

		if (GetValue(strTmp, _T("gintshenzhen"), m_nShenzhen))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintmustscan"), m_nMustScan))
		{
			continue;
		}

		if (GetValue(strTmp, _T("ginttempmember"), m_nTempMember))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintsfreg"), m_nUploadImage))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintneedactivation"), m_nNeedActivation))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintnomodifymember"), m_nNoModifyMember))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintglwpassword"), m_nRealNamePassword))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintautomatchidcard"), m_nAutoMatchIDCard))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintdisallowunlock"), m_nDisallowUnlock))
		{
			continue;
		}

		if (GetValue(strTmp, _T("gintinputactivation"), m_nInputActivation))
		{
			continue;
		}

		// 2011-4-15-gxx
		if (GetValue(strTmp, _T("gintscanenabled"), ScanEnabled))
		{
			continue;
		}

		// 2011/05/12-gxx: �Ƿ�ǿ���Զ��˿�ʱ��Ϊ0
		if (GetValue(strTmp, _T("gintmustautoreturn"), MustAutoReturn))
		{
			continue;
		}

		// 2011/06/10-gxx: 
		if (GetValue(strTmp, _T("gstrkillcaptions"), KillCaptions))
		{
			continue;
		}

		// 2011/06/30-gxx: 
		if (GetValue(strTmp, _T("gintforbidmodifypwd"), ForbidModifyPwd))
		{
			continue;
		}

	}

	//���¸������ݿ�
	SetRealNamePassword(m_nRealNamePassword);

	//���¸������ݿ�
	SetDisallowUnlock(m_nDisallowUnlock);

	return TRUE;
}

CString CNetBarConfig::GetUserClassName(UINT nClassID)
{
	CIBAString strTmp;

	// 2011/08/15-8201-gxx: �ж��Ƿ�Ϊ�����û�
	if (nClassID == CIBAGlobal::CLASSID_LOCALUSER)
	{
		strTmp.LoadString(IDS_LOCALUSER);
		return strTmp;
	}

	for (INT i = 0; i < m_UserClassArray.GetCount(); i++)
	{
		if (m_UserClassArray.GetAt(i).GetClassID() == nClassID)
		{
			strTmp = m_UserClassArray.GetAt(i).GetClassName();
			break;
		}
	}

	if (strTmp.IsEmpty())
	{
		strTmp.Format(_T("%d"), nClassID);
	}

	return strTmp;
}

UINT CNetBarConfig::GetUserClassId(CString strClassName)
{
	UINT nClassId = 0;

	for (INT i = 0; i < m_UserClassArray.GetCount(); i++)
	{
		if (m_UserClassArray.GetAt(i).GetClassName() == strClassName)
		{
			nClassId = m_UserClassArray.GetAt(i).GetClassID();
			break;
		}
	}

	return nClassId;
}

BOOL CNetBarConfig::GetIsMember(UINT nClassID)
{
	BOOL bRet = FALSE;

	for (INT i = 0; i < m_UserClassArray.GetCount(); i++)
	{
		if (m_UserClassArray.GetAt(i).GetClassID() == nClassID)
		{
			bRet = m_UserClassArray.GetAt(i).GetIsMember();
			break;
		}
	}

	return bRet;
}

CString CNetBarConfig::GetIDTypeName(UINT nIdType)
{
	CString strTmp;

	for (INT i = 0; i < m_IDTypeInfoArray.GetCount(); i++)
	{
		if (m_IDTypeInfoArray.GetAt(i).GetIDTypeID() == nIdType)
		{
			strTmp = m_IDTypeInfoArray.GetAt(i).GetIDTypeName();
			break;
		}
	}

	if (strTmp.IsEmpty())
	{
		strTmp.Format(_T("%d"), nIdType);
	}

	return strTmp;
}

UINT CNetBarConfig::GetDeposit(UINT nClassID)
{
	UINT nDeposit = 0;

	for (INT i = 0; i < m_UserClassArray.GetCount(); i++)
	{
		if (m_UserClassArray.GetAt(i).GetClassID() == nClassID)
		{
			nDeposit = m_UserClassArray.GetAt(i).GetDeposit();
			break;
		}
	}

	if (nDeposit <= 0)
	{
		nDeposit = 0;
	}

	return nDeposit;
}

BOOL CNetBarConfig::IsUseDeposit()
{
	for (INT i = 0; i < m_UserClassArray.GetCount(); i++)
	{
		if (m_UserClassArray.GetAt(i).GetDeposit() > 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CString CNetBarConfig::GetPCClassName(UINT nPCClass) 
{
	CString strTmp;

	for (INT i = 0; i < m_PCClassInfoArray.GetCount(); i++)
	{
		if (m_PCClassInfoArray.GetAt(i).GetID() == nPCClass)
		{
			strTmp = m_PCClassInfoArray.GetAt(i).GetName();

			break;
		}
	}

	return strTmp;
}


void CNetBarConfig::SetBossPwd(CString newVal)
{
	newVal.Insert(0, _T("IBA"));

	CString strPwdMD5;
	CEncrypt::CalcMD5(newVal, strPwdMD5);
	strPwdMD5.MakeUpper();
	CEncrypt::CalcMD5(strPwdMD5, strPwdMD5);
	strPwdMD5.MakeUpper();

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("BossPwd"));
	configInfo.SetKeyType(_T("xxxx"));
	configInfo.SetKeyValue3(strPwdMD5);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	m_strBossPwd = strPwdMD5;
}

void CNetBarConfig::SetRealNameFrom(UINT newVal)
{
	m_nRealNameFrom = newVal;
}

void CNetBarConfig::SetAllowNoCardLogin(BOOL newVal)
{
	m_bAllowNoCardLogin = newVal;
}

void CNetBarConfig::SetIssueNetBarId(UINT newVal)
{
	m_bIssueNetBarId = newVal;
}

void CNetBarConfig::SetNoIDCardIn(BOOL newVal)
{
	m_bNoIDCardIn = newVal;
}


void CNetBarConfig::SetAuditType(UINT newVal)
{
	m_nAuditType = newVal;

	if (m_nAuditType > 0) //�������������ʵ������Ĭ��ˢ����ʱ�Զ�������
	{
		m_nNeedActivation = 1; 
	}

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("AuditProxyAddr"));
	configInfo.SetKeyValue1(m_nAuditType);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
}

void CNetBarConfig::SetQueryHost2(CString newVal)
{
	m_strQueryHost2 = newVal;

	//newVal.Format(_T("http://%s/netbar/servlet/"), m_strQueryHost2);

	//CConfigInfo configInfo;
	//configInfo.SetKeyString(_T("queryHost2"));
	//configInfo.SetKeyValue3(newVal);
	//CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
}

void CNetBarConfig::SetQueryHost(CString newVal)
{
	m_strQueryHost = newVal;

	//newVal.Format(_T("http://%s/netbar/servlet"), m_strQueryHost);

	//CConfigInfo configInfo;
	//configInfo.SetKeyString(_T("queryHost"));
	//configInfo.SetKeyValue3(newVal);
	//CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
}

void CNetBarConfig::SetUpdateInterval(UINT newVal)
{
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("dodonewResource"));
	configInfo.SetKeyValue1(newVal);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);

	m_nUpdateInterval = newVal;
}

void CNetBarConfig::SetDodonewResource(CString newVal)
{
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("dodonewResource"));
	configInfo.SetKeyValue3(newVal);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	m_strDodonewResource = newVal;
}

void CNetBarConfig::SetDodoNewHost(CString newVal)
{
	m_strDodoNewHost = newVal;

	CString strTmp;
	strTmp.Format(_T("http://%s/dodonew/front/index.jsp"), m_strDodoNewHost);

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("Increment_addr1"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	configInfo.Clear();
	configInfo.SetKeyString(_T("Increment_addr4"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	configInfo.Clear();
	configInfo.SetKeyString(_T("Increment_addr5"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/dodonew/interface/termadvertise.jsp"), m_strDodoNewHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("Increment_addr5"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

}

void CNetBarConfig::SetReturnOnlyClassId(BOOL newVal)
{
	m_bReturnOnlyClassId = newVal;
}

void CNetBarConfig::SetNetBarInfo(UINT nNetBarId, LPCTSTR lpNetBarKey)
{
	m_NetBarKey = lpNetBarKey;
	m_NetBarId = nNetBarId;

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("currNetBar"));
	configInfo.SetKeyValue3(m_NetBarKey);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	configInfo.Clear();
	configInfo.SetKeyString(_T("currNetBar"));
	configInfo.SetKeyValue1(m_NetBarId);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo);
}

void CNetBarConfig::SetDomainId(UINT nDomainId)
{
	if (nDomainId != m_DomainId)
	{
		m_DomainId = nDomainId;

		CConfigInfo configInfo;
		configInfo.SetKeyString(_T("currNetBar"));
		configInfo.SetKeyValue2(m_DomainId);
		CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 2);
	}
}

void CNetBarConfig::SetMainHost(CString strMainHost, CString strPath)
{
	if (!strPath.IsEmpty())
	{
		m_MainCenterPath = strPath;
	}

	INT nIndex = strMainHost.Find(':');

	if (nIndex > 0)
	{
		CString strTmp = strMainHost.Left(nIndex);

		if (strTmp != m_MainCenterIP)
		{
			m_MainCenterIP = strTmp;
			
			CConfigInfo configInfo;
			configInfo.SetKeyString(_T("ICServerIPAddress"));
			configInfo.SetKeyValue3(m_MainCenterIP);
			CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
		}

		const INT nTmp = _ttoi(strMainHost.Mid(nIndex + 1));

		if (nTmp != m_MainCenterPort)
		{
			m_MainCenterPort = nTmp;

			strTmp.Format(_T("%d/%s"), m_MainCenterPort, m_MainCenterPath);

			CConfigInfo configInfo;
			configInfo.SetKeyString(_T("ICServerPort"));
			configInfo.SetKeyValue3(strTmp);
			CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
		}
	}

	CString strTmp;
	strTmp.Format(_T("http://%s/netbar/services/Checkin"), strMainHost);

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("checkinURL"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/services/Checkout"), strMainHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("checkoutURL"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);


	strTmp.Format(_T("http://%s/netbar/services/Freeze"), strMainHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("freezeURL"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/services/GetTime"), strMainHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("SyncTimeURL"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/ad/index.jsp"), strMainHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("lockScreenURL"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/services/AssetStatus"), strMainHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("AssetStatusURL"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

}

void CNetBarConfig::SetBackUpHost(CString strBackUpHost, CString strPath)
{
	if (!strPath.IsEmpty())
	{
		m_BackupCenterPath = strPath;
	}

	INT nIndex = strBackUpHost.Find(':');

	if (nIndex > 0)
	{
		CString strTmp = strBackUpHost.Left(nIndex);

		if (strTmp != m_BackupCenterIP)
		{
			m_BackupCenterIP = strTmp;

			CConfigInfo configInfo;
			configInfo.SetKeyString(_T("ICGetTimeIPAddress"));
			configInfo.SetKeyValue3(m_BackupCenterIP);
			CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
		}

		const INT nTmp = _ttoi(strBackUpHost.Mid(nIndex + 1));

		if (nTmp != m_BackupCenterPort)
		{
			m_BackupCenterPort = nTmp;

			strTmp.Format(_T("%d/%s"), m_BackupCenterPort, m_BackupCenterPath);

			CConfigInfo configInfo;
			configInfo.SetKeyString(_T("ICGetTimePort"));
			configInfo.SetKeyValue3(strTmp);
			CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
		}
	}

	CString strTmp;
	strTmp.Format(_T("http://%s/netbar/services/Checkin"), strBackUpHost);

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("checkinURL_1"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/services/Checkout"), strBackUpHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("checkoutURL_1"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/services/Freeze"), strBackUpHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("freezeURL_1"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

	strTmp.Format(_T("http://%s/netbar/services/GetTime"), strBackUpHost);

	configInfo.Clear();
	configInfo.SetKeyString(_T("SyncTimeURL_1"));
	configInfo.SetKeyValue3(strTmp);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);

}

void CNetBarConfig::SetStandbyHost(CString newVal)
{
	m_strStandbyHost = newVal;

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("standbyHost"));
	configInfo.SetKeyName(_T("standbyHost"));
	configInfo.SetKeyValue3(m_strStandbyHost);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
}

void CNetBarConfig::SetProxyIP(CString newVal)
{
	m_strProxyIP = newVal;

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("AuditProxyAddr"));
	configInfo.SetKeyValue3(m_strProxyIP);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 3);
}

UINT CNetBarConfig::GetSoftwareType() const
{
#ifdef _DEBUG
	return 4;
#endif // _DEBUG
	
	return m_nSoftwareType;
}

void CNetBarConfig::SetSoftwareType(UINT newVal)
{
	m_nSoftwareType = newVal;

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("SoftwareType"));
	configInfo.SetKeyName(_T("SoftwareType"));
	configInfo.SetKeyValue1(m_nSoftwareType);
	configInfo.SetKeyType(_T("xxxx"));
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
}

void CNetBarConfig::SetDisallowUnlock(INT val)
{ 
	m_nDisallowUnlock = val; 

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("DisallowUnlock"));
	configInfo.SetKeyValue1(val);
	configInfo.SetKeyType(_T("xxxx"));

	if (val >= 1)
	{
		//������������ʱ��ά��ʱ��Ϊ3����
		CConfigInfo configInfo2;
		configInfo2.SetKeyString(_T("MaxKeepTime"));
		configInfo2.SetKeyValue1(3);
		CIBADAL::GetInstance()->UpdateSystemConfig(configInfo2, 1);
	} 
	
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
}

void CNetBarConfig::SetRealNamePassword(INT val)
{
	m_nRealNamePassword = val; 

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("AllowChangePwd"));

	if (val == 2) 
	{
		configInfo.SetKeyValue1(0);
	} 
	else
	{
		configInfo.SetKeyValue1(1);
	}

	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
}

UINT CNetBarConfig::GetStoreInAmount()
{ 
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("StoreInAmount"));
	CIBADAL::GetInstance()->GetSystemConfig(configInfo);
	m_nStoreInAmount = configInfo.GetKeyValue1();
	return m_nStoreInAmount; 
}

void CNetBarConfig::SetStoreInAmount(UINT val) 
{ 
	m_nStoreInAmount = val; 

	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("StoreInAmount"));
	configInfo.SetKeyValue1(m_nStoreInAmount);
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);
}

void CNetBarConfig::SetBTLimitTime(const UINT nNewVal)
{
	WRITE_CONFIG(BTLimitTime,nNewVal,_T("BTLimitTime"),1);
}

void CNetBarConfig::SetForeRealName(UINT newVal)
{
	WRITE_CONFIG(m_ForeRealName,newVal,_T("ForeRealName"),1);
}

void CNetBarConfig::SetLocalModeStartKind(INT nKind)
{
	WRITE_CONFIG(LocalModeStartKind, nKind, _T("LocalModeStartKind"),1);
}

void CNetBarConfig::SetReminderBalance(BOOL newVal)
{
	WRITE_CONFIG(ReminderBalance, newVal, _T("ReminderBalance"),1);
}

void CNetBarConfig::SetReminderAutoReturn(BOOL newVal)
{
	WRITE_CONFIG(ReminderAutoReturn, newVal, _T("ReminderAutoReturn"),1);
}

void CNetBarConfig::SetUserRCheckIn(BOOL newVal)
{
	WRITE_CONFIG(UserRCheckIn, newVal, _T("UserRCheckIn"),1);
}

INT CNetBarConfig::GetMustScan() const
{
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		return 0;
	}
	return m_nMustScan;
}

void CNetBarConfig::SetForbidCaptureScreen(BOOL newVal)
{
	WRITE_CONFIG(ForbidCaptureScreen, newVal, _T("ForbidCaptureScreen"),1);
}