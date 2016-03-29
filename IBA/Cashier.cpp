
#include "stdafx.h"
#include ".\cashier.h"
#include "IBA.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include "dal\IBADAL.h"
#include "ActiveMemberView.h"
#include "RealName\IBARealName.h"
#include ".\Socket\LocalServer.h"
#include "Encrypt.h"
#include "Dal\RightsManagement.h"
#include "CashierHelper.h"
#include "LocalMember.h"


CCashier::CCashier(void)
{
	m_bIsBoss = FALSE;
	m_bLogin = FALSE; 
	m_bOnDuty = FALSE;
	m_nDutyID = 0;
	m_nShiftType = 0;
	m_bIsLocalModeLogin = FALSE;

}

CCashier::~CCashier(void)
{
}

//���������߲������Ƿ�����
BOOL CCashier::CheckLastDuty(CString& strLastOperator)
{
	CString strTmp;
	strTmp.Format(_T("LastDutyInfo-%d"), CIBAConfig::GetInstance()->GetCashRegisterID());

	CConfigInfo ConfigInfo;
	ConfigInfo.SetKeyString(strTmp);
	
	if (CIBADAL::GetInstance()->GetSystemConfig(ConfigInfo))
	{
		strLastOperator = ConfigInfo.GetKeyValue3();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CCashier::WriteDutyInfo()
{
	CString strTmp;
	strTmp.Format(_T("LastDutyInfo-%d"), CIBAConfig::GetInstance()->GetCashRegisterID());

	CConfigInfo ConfigInfo;
	
	ConfigInfo.SetKeyString(strTmp);
	ConfigInfo.SetKeyName(strTmp);
	ConfigInfo.SetKeyValue1(GetDutyID());
	ConfigInfo.SetKeyValue2(m_nShiftType);
	ConfigInfo.SetDescription(GetOnDutyTime());
	ConfigInfo.SetKeyType(_T("xxxx"));
	if (m_bOnDuty)
	{
		ConfigInfo.SetKeyValue3(GetName());
	}
	else
	{
		ConfigInfo.SetKeyValue3(_T(""));
	}

	CIBADAL::GetInstance()->UpdateSystemConfig(ConfigInfo, 9);

}

BOOL CCashier::HasPopedomOf(LPCTSTR lpszModuleName)
{
	//if (IsBoss()) //�ϰ�ӵ�����е�Ȩ��
	//{
	//	return TRUE;
	//}

	//CString strTmp(lpszModuleName);
	//strTmp.Append(_T(","));

	//return (m_strEnableModuleList.Find(strTmp) >= 0);

	return CRightsManagement::GetInstance()->HasRight(lpszModuleName);
}

BOOL CCashier::IsOnDuty()
{
	BOOL bRet = FALSE;

	bRet = m_bLogin && m_bOnDuty;

	return bRet;
}

CString CCashier::GetOnDutyTime() const
{
	if (!m_OnDutyTime.IsEmpty())
	{
		return CIBAHelpper::FormatCenterTime(m_OnDutyTime);
	}

	return m_OnDutyTime;
}

BOOL CCashier::CheckIn(CCashRegisterLoginMethod& CashRegisterLogin, CString strOperator)
{
	BOOL bRet = FALSE;

	CashRegisterLogin.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CashRegisterLogin.SetCashRegisterVersion(CIBALog::GetInstance()->GetAppVer());
	CashRegisterLogin.SetCashRegisterType(1);//��ͨģʽ����Ҫ��֤���ޱ������ݿ�
	CashRegisterLogin.SetUserId(m_strName);
	CashRegisterLogin.SetPassword(m_strPassword);

	time_t tm = time(NULL);
	CString strOrg;
	strOrg.Format(_T("%ld"), tm);

	CIBALog::GetInstance()->WriteFormat(_T("ʱ���:%d"), tm);
	CIBALog::GetInstance()->WriteFormat(_T("%s"), m_strPassword);
	
	CIBALog::GetInstance()->WriteFormat(_T("fileHash:%s"), CIBALog::GetInstance()->GetFileHash());

	CString strTmp;
	CEncrypt::CalcMD5(m_strPassword, strTmp);

	CIBALog::GetInstance()->WriteFormat(_T("%s"), strTmp);

	strOrg += CIBALog::GetInstance()->GetFileHash();
	strOrg += strTmp.Left(16);
	strOrg += strTmp.Left(16);
	CString strRes;
	CEncrypt::CalcMD5(strOrg, strRes);
	
	CashRegisterLogin.SetTimeChuo(tm);
	CashRegisterLogin.SetCheckCode(strRes);

	DWORD length = MAX_PATH;
	TCHAR szComputerName[MAX_PATH] = {0};
	::GetComputerName(szComputerName, &length);
	CString strComputerName(szComputerName);
	if(strComputerName.GetLength() > 15)
	{
		strComputerName.Left(15);
	}

	CashRegisterLogin.SetPCName(strComputerName);


	bRet = CashRegisterLogin.Execute();

	if (!bRet) //��������ʧ��
	{
		return bRet;
	}

	if (CashRegisterLogin.GetStatusCode() == 0) //ִ�гɹ�
	{
		bRet = TRUE;

		m_bLogin = TRUE;

		m_bIsBoss = FALSE;
		
		
		// 2013-12-3 15:26:04 qsc ���ַ�ʽ���У���Ϊ2591tset,2591a��ת�������ֺ�2591���
		// if (_ttoi(m_strName) == CNetBarConfig::GetInstance()->GetNetBarId())
		CString strNetbarId;
		strNetbarId.Format(_T("%d"), CNetBarConfig::GetInstance()->GetNetBarId());
		
		if(strNetbarId.GetLength() == m_strName.GetLength() &&
			strNetbarId == m_strName)
		{	
			//����ID�˻���¼
			m_bIsBoss = TRUE;
		}

		m_nDutyID = CashRegisterLogin.GetDutyId();

		m_bOnDuty = (m_nDutyID > 0);

		if (!m_bOnDuty)
		{
			m_nDutyID = 1; //���û���ϰ࣬��ǿ��Ϊ���1,�Ӷ�ǿ�����Ľ���ʱ������
		}
		
		m_OnDutyTime = CashRegisterLogin.GetOndutyTime();
		m_nShiftType = CashRegisterLogin.GetShiftType();     // ����������

		CashRegisterLogin.GetUserClassInfoArray(CNetBarConfig::GetInstance()->m_UserClassArray);
		
		CString strSQL;

		for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
		{
			strSQL.Format(_T("update memberclass set className='%s', isMember=%d where classId=%d"),
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName(),
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetIsMember(),
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID());

			CIBADAL::GetInstance()->ExecuteSQL(strSQL);
		}
		// 2014-8-11 - qsc
		SetCenterAddr(CashRegisterLogin);

		CNetBarConfig::GetInstance()->SetSoftwareType(CashRegisterLogin.GetSoftwareType());
		CNetBarConfig::GetInstance()->SetDomainId(CashRegisterLogin.GetDomainId());
		CNetBarConfig::GetInstance()->SetAuditType(CashRegisterLogin.GetAuditType());
		CNetBarConfig::GetInstance()->SetAllowNoCardLogin(CashRegisterLogin.GetAllowNoCardLogin());
		CNetBarConfig::GetInstance()->SetDodoNewHost(CashRegisterLogin.GetDodoNewHost());
		CNetBarConfig::GetInstance()->SetDodonewResource(CashRegisterLogin.GetDodonewResource());
		CNetBarConfig::GetInstance()->SetForeRealName(CashRegisterLogin.GetForeRealName());
		CNetBarConfig::GetInstance()->SetIssueNetBarId(CashRegisterLogin.GetIssueNetBarId());
		CNetBarConfig::GetInstance()->SetNoIDCardIn(CashRegisterLogin.GetNoIDCardIn());
		CNetBarConfig::GetInstance()->SetQueryHost(CashRegisterLogin.GetQueryHost());
		CNetBarConfig::GetInstance()->SetQueryHost2(CashRegisterLogin.GetQueryHost2());
		CNetBarConfig::GetInstance()->SetRealNameFrom(CashRegisterLogin.GetRealNameFrom());
		CNetBarConfig::GetInstance()->SetReturnOnlyClassId(CashRegisterLogin.GetReturnOnlyClassId());
		CNetBarConfig::GetInstance()->SetUpdateInterval(CashRegisterLogin.GetUpdateInterval());
		CNetBarConfig::GetInstance()->SetBundleTimeWithCash(CashRegisterLogin.GetBundleTimeUseBalance());
		CNetBarConfig::GetInstance()->SetMustSynchronize(CashRegisterLogin.GetMustSynchronize());
		CNetBarConfig::GetInstance()->SetCreditDirect(CashRegisterLogin.GetAllowCreditNegative() == 1);
		CNetBarConfig::GetInstance()->SetIsInternetCafe(CashRegisterLogin.GetIsInternetCafe());
		if(CashRegisterLogin.GetIsInternetCafe() > 0)
		{
			CNetBarConfig::GetInstance()->SetAddressInternetCafe(CashRegisterLogin.GetCafeAddress());
		}
		//�����������õĿ��Ʋ����ִ�
		CNetBarConfig::GetInstance()->PraseControlOption(CashRegisterLogin.GetControlOption());

		if (CNetBarConfig::GetInstance()->GetMonitorAuditProxy() == 1 && CIBAConfig::GetInstance()->GetCashRegisterID() == 1)
		{
			theApp.m_pMainWnd->SetTimer(1005, 5000, NULL);
		}

		// QSCADD-2013-6-17 14:34:45 �Ƚϳ���ǰ�汾�����Ĵ������İ汾
		if (CNetBarConfig::GetInstance()->GetForceQuitParam() == 1)
		{
			CString thisVer = CIBALog::GetInstance()->GetAppVer();
			thisVer.Remove(_T('.'));
			CString strVer = CNetBarConfig::GetInstance()->GetForceQuitVersion();
			if (thisVer.Compare(strVer) < 0)
			{
				CString strTip;
				strTip.Format(IDS_IBA_VERBELOW_TIP, strVer);
				// �������ĵİ汾
				HWND hWndTop;
				HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);
				theApp.IBAMsgBox2(strTip, MB_OK, TRUE);
				theApp.AutoRestart();
			}
		}

		//
		MonitorAuditProxy();

		//��������Ϣ
		CNetBarConfig::GetInstance()->SetAgentName(CashRegisterLogin.GetAgentName());
		CNetBarConfig::GetInstance()->SetAgentMobilePhone(CashRegisterLogin.GetAgentMobilePhone());
		CNetBarConfig::GetInstance()->SetAgentregularTel(CashRegisterLogin.GetAgentRegularTel());


		//Ȩ���б�
		m_strEnableModuleList = CashRegisterLogin.GetUserPrivileges().MakeUpper();
		m_strEnableModuleList.Append(_T(","));

		NS_DAL::CRightsManagement::GetInstance()->CreateManagement(m_strEnableModuleList, m_bIsBoss);
		
		//�����������ʱ����
		CNetBarConfig::GetInstance()->m_NetbarBundleTime.Start();

		//�������ע���LastTran��Է���ɵĸ���
		theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\NetBarInfo\\LastTran"));
	
		WriteLoginInfo(strOperator);//����û�

		//�����Ƿ���Ҫ�ύ
		if (IsOnDuty())
		{
			if (CBrokenNetwork::GetInstance()->m_centerUserUpLoad.CheckShouldResume())
			{
				CBrokenNetwork::GetInstance()->m_centerUserUpLoad.SetShouldResume(TRUE);
				CBrokenNetwork::GetInstance()->m_centerUserUpLoad.StartResume(0);
			}
			CBrokenNetwork::GetInstance()->m_threadRestore->Start();
		}

		// 2011/06/14-gxx: 
		((CMainFrame*)theApp.GetMainWnd())->m_wndOutput.RefreshAllInfoList();

		//{ 2011/07/13-8201-gxx: �������ݵ��������ݿ�
		LocalInfo.SetCasherAccess(m_strEnableModuleList);
		LocalInfo.SetControlParam(CashRegisterLogin.GetControlOption());
		CBrokenNetwork::GetInstance()->SaveLocalStatusInfo(LocalInfo);

		m_bIsLocalModeLogin = FALSE;
		//}

		// 2011/11/18-8210-gxx: 
		if (IsOnDuty())
		{
			CCashierHelper::InitDiankaPlatform();
		}

		CIBADAL::GetInstance()->AddCashierToDB();

		//����������ַ������
		CIBADAL::GetInstance()->UpdateInternetCafeInfo();
	}

	return bRet;
}

BOOL CCashier::CheckIn_atLogin(CCashRegisterLoginMethod_atLogin& CashRegisterLogin, CString strOperator)
{
	//���Buildʱ��
	CString strTime = CIBALog::GetInstance()->GetAppBuildTime();
	CIBALog::GetInstance()->WriteFormat(_T("Build Time: %s"), strTime);

	BOOL bRet = FALSE;

	CashRegisterLogin.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CashRegisterLogin.SetCashRegisterVersion(CIBALog::GetInstance()->GetAppVer());
	CashRegisterLogin.SetCashRegisterType(1);//��ͨģʽ����Ҫ��֤���ޱ������ݿ�
	CashRegisterLogin.SetUserId(m_strName);
	CashRegisterLogin.SetPassword(m_strPassword);

	time_t tm = time(NULL);
	CString strOrg;
	strOrg.Format(_T("%ld"), tm);

	CIBALog::GetInstance()->WriteFormat(_T("ʱ���:%d"), tm);
	CIBALog::GetInstance()->WriteFormat(_T("%s"), m_strPassword);

	CIBALog::GetInstance()->WriteFormat(_T("fileHash:%s"), CIBALog::GetInstance()->GetFileHash());

	CString strTmp;
	CEncrypt::CalcMD5(m_strPassword, strTmp);

	CIBALog::GetInstance()->WriteFormat(_T("%s"), strTmp);

	strOrg += CIBALog::GetInstance()->GetFileHash();
	strOrg += strTmp.Left(16);
	strOrg += strTmp.Left(16);
	CString strRes;
	CEncrypt::CalcMD5(strOrg, strRes);

	CashRegisterLogin.SetTimeChuo(tm);
	CashRegisterLogin.SetCheckCode(strRes);

	DWORD length = MAX_PATH;
	TCHAR szComputerName[MAX_PATH] = {0};
	::GetComputerName(szComputerName, &length);
	CString strComputerName(szComputerName);
	if(strComputerName.GetLength() > 15)
	{
		strComputerName.Left(15);
	}

	CashRegisterLogin.SetPCName(strComputerName);


	//�����½
	bRet = CashRegisterLogin.ExecuteCheckin();

	if (!bRet) //��������ʧ��
	{
		return bRet;
	}

	if (CashRegisterLogin.GetStatusCode() == 0) //ִ�гɹ�
	{
		bRet = TRUE;

		m_bLogin = TRUE;

		m_bIsBoss = FALSE;


		// 2013-12-3 15:26:04 qsc ���ַ�ʽ���У���Ϊ2591tset,2591a��ת�������ֺ�2591���
		// if (_ttoi(m_strName) == CNetBarConfig::GetInstance()->GetNetBarId())
		CString strNetbarId;
		strNetbarId.Format(_T("%d"), CNetBarConfig::GetInstance()->GetNetBarId());

		if(strNetbarId.GetLength() == m_strName.GetLength() &&
			strNetbarId == m_strName)
		{	
			//����ID�˻���¼
			m_bIsBoss = TRUE;
		}

		m_nDutyID = CashRegisterLogin.GetDutyId();

		m_bOnDuty = (m_nDutyID > 0);

		if (!m_bOnDuty)
		{
			m_nDutyID = 1; //���û���ϰ࣬��ǿ��Ϊ���1,�Ӷ�ǿ�����Ľ���ʱ������
		}

		m_OnDutyTime = CashRegisterLogin.GetOndutyTime();
		m_nShiftType = CashRegisterLogin.GetShiftType();     // ����������

		CashRegisterLogin.GetUserClassInfoArray(CNetBarConfig::GetInstance()->m_UserClassArray);

		CString strSQL;

		for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
		{
			strSQL.Format(_T("update memberclass set className='%s', isMember=%d where classId=%d"),
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName(),
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetIsMember(),
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID());

			CIBADAL::GetInstance()->ExecuteSQL(strSQL);
		}
		// 2014-8-11 - qsc
		SetCenterAddr(CashRegisterLogin);

		CNetBarConfig::GetInstance()->SetSoftwareType(CashRegisterLogin.GetSoftwareType());
		CNetBarConfig::GetInstance()->SetDomainId(CashRegisterLogin.GetDomainId());
		CNetBarConfig::GetInstance()->SetAuditType(CashRegisterLogin.GetAuditType());
		CNetBarConfig::GetInstance()->SetAllowNoCardLogin(CashRegisterLogin.GetAllowNoCardLogin());
		CNetBarConfig::GetInstance()->SetDodoNewHost(CashRegisterLogin.GetDodoNewHost());
		CNetBarConfig::GetInstance()->SetDodonewResource(CashRegisterLogin.GetDodonewResource());
		CNetBarConfig::GetInstance()->SetForeRealName(CashRegisterLogin.GetForeRealName());
		CNetBarConfig::GetInstance()->SetIssueNetBarId(CashRegisterLogin.GetIssueNetBarId());
		CNetBarConfig::GetInstance()->SetNoIDCardIn(CashRegisterLogin.GetNoIDCardIn());
		CNetBarConfig::GetInstance()->SetQueryHost(CashRegisterLogin.GetQueryHost());
		CNetBarConfig::GetInstance()->SetQueryHost2(CashRegisterLogin.GetQueryHost2());
		CNetBarConfig::GetInstance()->SetRealNameFrom(CashRegisterLogin.GetRealNameFrom());
		CNetBarConfig::GetInstance()->SetReturnOnlyClassId(CashRegisterLogin.GetReturnOnlyClassId());
		CNetBarConfig::GetInstance()->SetUpdateInterval(CashRegisterLogin.GetUpdateInterval());
		CNetBarConfig::GetInstance()->SetBundleTimeWithCash(CashRegisterLogin.GetBundleTimeUseBalance());
		CNetBarConfig::GetInstance()->SetMustSynchronize(CashRegisterLogin.GetMustSynchronize());
		CNetBarConfig::GetInstance()->SetCreditDirect(CashRegisterLogin.GetAllowCreditNegative() == 1);
		CNetBarConfig::GetInstance()->SetIsInternetCafe(CashRegisterLogin.GetIsInternetCafe());
		if(CashRegisterLogin.GetIsInternetCafe() > 0)
		{
			CNetBarConfig::GetInstance()->SetAddressInternetCafe(CashRegisterLogin.GetCafeAddress());
		}
		//�����������õĿ��Ʋ����ִ�
		CNetBarConfig::GetInstance()->PraseControlOption(CashRegisterLogin.GetControlOption());

		if (CNetBarConfig::GetInstance()->GetMonitorAuditProxy() == 1 && CIBAConfig::GetInstance()->GetCashRegisterID() == 1)
		{
			theApp.m_pMainWnd->SetTimer(1005, 5000, NULL);
		}

		// QSCADD-2013-6-17 14:34:45 �Ƚϳ���ǰ�汾�����Ĵ������İ汾
		if (CNetBarConfig::GetInstance()->GetForceQuitParam() == 1)
		{
			CString thisVer = CIBALog::GetInstance()->GetAppVer();
			thisVer.Remove(_T('.'));
			CString strVer = CNetBarConfig::GetInstance()->GetForceQuitVersion();
			if (thisVer.Compare(strVer) < 0)
			{
				CString strTip;
				strTip.Format(IDS_IBA_VERBELOW_TIP, strVer);
				// �������ĵİ汾
				HWND hWndTop;
				HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);
				theApp.IBAMsgBox2(strTip, MB_OK, TRUE);
				theApp.AutoRestart();
			}
		}

		//
		MonitorAuditProxy();

		//��������Ϣ
		CNetBarConfig::GetInstance()->SetAgentName(CashRegisterLogin.GetAgentName());
		CNetBarConfig::GetInstance()->SetAgentMobilePhone(CashRegisterLogin.GetAgentMobilePhone());
		CNetBarConfig::GetInstance()->SetAgentregularTel(CashRegisterLogin.GetAgentRegularTel());


		//Ȩ���б�
		m_strEnableModuleList = CashRegisterLogin.GetUserPrivileges().MakeUpper();
		m_strEnableModuleList.Append(_T(","));

		NS_DAL::CRightsManagement::GetInstance()->CreateManagement(m_strEnableModuleList, m_bIsBoss);

		//�����������ʱ����
		CNetBarConfig::GetInstance()->m_NetbarBundleTime.Start();

		//�������ע���LastTran��Է���ɵĸ���
		theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\NetBarInfo\\LastTran"));

		WriteLoginInfo(strOperator);//����û�

		//�����Ƿ���Ҫ�ύ
		if (IsOnDuty())
		{
			if (CBrokenNetwork::GetInstance()->m_centerUserUpLoad.CheckShouldResume())
			{
				CBrokenNetwork::GetInstance()->m_centerUserUpLoad.SetShouldResume(TRUE);
				CBrokenNetwork::GetInstance()->m_centerUserUpLoad.StartResume(0);
			}
			CBrokenNetwork::GetInstance()->m_threadRestore->Start();
		}

		// 2011/06/14-gxx: 
		((CMainFrame*)theApp.GetMainWnd())->m_wndOutput.RefreshAllInfoList();

		//{ 2011/07/13-8201-gxx: �������ݵ��������ݿ�
		LocalInfo.SetCasherAccess(m_strEnableModuleList);
		LocalInfo.SetControlParam(CashRegisterLogin.GetControlOption());
		CBrokenNetwork::GetInstance()->SaveLocalStatusInfo(LocalInfo);

		m_bIsLocalModeLogin = FALSE;
		//}

		// 2011/11/18-8210-gxx: 
		if (IsOnDuty())
		{
			CCashierHelper::InitDiankaPlatform();
		}

		CIBADAL::GetInstance()->AddCashierToDB();

		//����������ַ������
		CIBADAL::GetInstance()->UpdateInternetCafeInfo();
	}

	return bRet;
}

BOOL CCashier::DoOnDuty(COnDutyMethod &OnDuty)
{
	BOOL bRet = FALSE;

	OnDuty.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	OnDuty.SetOperator(m_strName);

	bRet = OnDuty.Execute();

	if (OnDuty.GetStatusCode() == 0)
	{
		m_bOnDuty = TRUE;
		m_nDutyID = OnDuty.GetDutyId();
		m_OnDutyTime = OnDuty.GetOnDutyTime();
		m_nShiftType = OnDuty.GetShiftType();
		
		//дע���������ߵ�����Ա�������������ԱID�������������Ա��ţ���operators����˳�򣩣��������ʱ��
		WriteDutyInfo();

		// 2011/06/03-gxx: 
		if (CBrokenNetwork::GetInstance()->m_centerUserUpLoad.CheckShouldResume())
		{
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.SetShouldResume(TRUE);
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.StartResume(0);
		}

	/*	CBrokenNetwork::GetInstance()->m_threadRestore->Start();*/

		// 2011/06/14-gxx: ˢ�²�����־
		((CMainFrame*)theApp.GetMainWnd())->m_wndOutput.RefreshAllInfoList();

		CIBADAL::GetInstance()->AddCashierToDB();

		// 2011/11/18-8210-gxx: 
		CCashierHelper::InitDiankaPlatform();
	}

	return bRet;
}

BOOL CCashier::DoOffDuty(COffDutyMethod &OffDuty)
{
	OffDuty.SetDutyId(m_nDutyID);
	OffDuty.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	OffDuty.SetOperator(m_strName);

	BOOL bRet = OffDuty.Execute();

	if (OffDuty.GetStatusCode() == 0)
	{
		m_bLogin = FALSE;
		m_nDutyID = 1;
		m_bOnDuty = FALSE;
		m_nShiftType = 0;
		m_OnDutyTime.Empty();

		WriteDutyInfo();

		((CMainFrame*)theApp.GetMainWnd())->m_wndOutput.DeleteDutyFile();

		// 2011/11/18-8210-gxx: 
		CCashierHelper::UninitDiankaPlatform();
	}

	return bRet;
}

BOOL CCashier::DoRegister(CRegisterMethod &Register)
{
	Register.SetDutyId(m_nDutyID);
	Register.SetOperator(m_strName);
	Register.SetAccountNetbarId(CNetBarConfig::GetInstance()->GetIssueNetBarId());
	Register.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	Register.SetAuditType(CNetBarConfig::GetInstance()->GetAuditType());

	BOOL bRet = Register.Execute();

	Register.SaveTranInfo();

	if (Register.GetStatusCode() == 0)
	{
		// 2011/09/02-8201-gxx: 
		if (CBrokenNetwork::GetInstance()->m_centerUserUpLoad.CheckShouldResume())
		{
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.SetShouldResume(TRUE);
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.StartResume(0);
		}  // ִ�лָ�, ���������û��ڽ��˺����Ϣ
	}

	return bRet;
}

BOOL CCashier::DoCredit(CCreditMethod & Credit)
{
	Credit.SetDutyId(m_nDutyID);
	Credit.SetOperator(m_strName);
	Credit.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());

	BOOL bRet = Credit.Execute();

	if (!bRet)
	{
		return FALSE;
	}

	if (Credit.GetStatusCode() == 0)
	{
		Credit.SaveTranInfo();

		//����CreditProcess
		CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(Credit.GetMemberId(), Credit.GetAmount(), Credit.GetCreditPresent());

		//д�����ݿ�credited
		CActiveMember Am;
		if(CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(Credit.GetMemberId(),Am))
		{
			CCreditInfoDB CreditInfo;
			CreditInfo.SetCardSerial(Am.GetNetId());
			CreditInfo.SetCreditDate(CIBAHelpper::FormatCenterTime(Credit.GetTrantime()));
			CreditInfo.SetRefNo(Credit.GetRefNo());
			CreditInfo.SetCreditPresent(Credit.GetCreditPresent());
			CreditInfo.SetCreditAmount(Credit.GetAmount());
			CreditInfo.SetOperator(theApp.GetCurCashier()->GetName());
			CreditInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			CreditInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
			CreditInfo.SetMemberId(Credit.GetMemberId());
			CreditInfo.SetSerialNo(Credit.GetLocalSerialNo());
			CreditInfo.SetCassId(Am.GetClassId());
			CIBADAL::GetInstance()->AddCreditRecord(CreditInfo);
		}

		if (CBrokenNetwork::GetInstance()->m_centerUserUpLoad.CheckShouldResume())
		{
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.SetShouldResume(TRUE);
			CBrokenNetwork::GetInstance()->m_centerUserUpLoad.StartResume(0);
		}  
	}

	return bRet;
}
/*************************************************
Function:       // MakeupCredit
Description:    // ���ϱ�����ģʽ�³�ֵ
Input:          // Credit - ����ִ�г�ֵ
Return:         // ��ֵ�ɹ��򷵻�TRUE
Others:         // ��DoCreditΨһ�����ǣ�������ActiveMember.CreditProcess.��Ϊ�Ѿ��ڱ��س�ֵʱ�����˸���.
*************************************************/
BOOL CCashier::MakeupCredit(CCreditMethod & Credit)
{

	Credit.SetDutyId(m_nDutyID);
	Credit.SetOperator(m_strName);
	Credit.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());

	BOOL bRet = Credit.Execute();

	if (!bRet)
	{
		return FALSE;
	}

	if (Credit.GetStatusCode() == 0)
	{
		Credit.SaveTranInfo();

		//CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(
		//	Credit.GetMemberId(), Credit.GetAmount(), Credit.GetCreditPresent());

		//д�����ݿ�credited
		CActiveMember Am;
		if(CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(Credit.GetMemberId(),Am))
		{
			CCreditInfoDB CreditInfo;
			CreditInfo.SetCardSerial(Am.GetNetId());
			CreditInfo.SetCreditDate(CIBAHelpper::FormatCenterTime(Credit.GetTrantime()));
			CreditInfo.SetRefNo(Credit.GetRefNo());
			CreditInfo.SetCreditPresent(Credit.GetCreditPresent());
			CreditInfo.SetCreditAmount(Credit.GetAmount());
			CreditInfo.SetOperator(theApp.GetCurCashier()->GetName());
			CreditInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			CreditInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
			CreditInfo.SetMemberId(Credit.GetMemberId());
			CreditInfo.SetSerialNo(Credit.GetLocalSerialNo());
			CreditInfo.SetCassId(Am.GetClassId());
			CIBADAL::GetInstance()->AddCreditRecord(CreditInfo);
		}
	}

	return bRet;
}

BOOL CCashier::DoMemberInfo(CMemberInfoMethod & MemberInfo)
{
	BOOL bRet = MemberInfo.Execute();

	return bRet;
}

/*************************************************
Function:       // DoDeregister
Description:    // ������ִ��ע������������ReturnMode=2(���˰�ʱ������)�������ȡ�����ؼ������ʵ���»�
Input:          // Deregister 
				// ReturnMode - �˿�ģʽ.0:�����˿� 1:�˿���� 2:�˰�ʱ�����
Return:         // Deregister����0������TRUE
*************************************************/
BOOL CCashier::DoDeregister(CDeregisterMethod & Deregister,UINT nReturnMode /*=0*/)
{
	//����ģʽ,���»����LocalMember��¼
	BOOL bSuccessDBOperation(TRUE);
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		//��ѯLocalMember�Ƿ��м�¼
		CLocalMember lMember;
		BOOL bHasRecord = CIBADAL::GetInstance()->GetLocalMemberByMemberID(Deregister.GetMemberId(),lMember);
		if(bHasRecord)//�м�¼,�����˿���Ϣ
		{
			COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
			if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(Deregister.GetMemberId(),CheckoutTime,TRUE))
			{
				bSuccessDBOperation = FALSE;
				IBA_LOG(_T("Error.���±����˿��¼ʧ��."));
			}
		}
		else//�޼�¼,������¼,�����¼,���¼�¼
		{
			GxxDictionaryPtr lmPrt = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(Deregister.GetMemberId());  //������¼
			if(lmPrt)
			{
				if(CIBADAL::GetInstance()->InsertNewLocalMember(lmPrt))//�����¼
				{
					COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
					if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(Deregister.GetMemberId(),CheckoutTime,TRUE))//���¼�¼
					{
						bSuccessDBOperation = FALSE;
						IBA_LOG(_T("Error.���±����˿��¼ʧ��."));
					}	
				}
				else
				{
					bSuccessDBOperation = FALSE;
					IBA_LOG(_T("Error.Ϊ�˿���뱾�ؼ�¼ʧ��."));
				}
			}
			else
			{
				bSuccessDBOperation = FALSE;
				IBA_LOG(_T("Error.Ϊ�˿�����ؼ�¼ʧ��."));
			}
		}
	}
	else//����ģʽ,ִ��Deregister
	{
		//�����˿�
		Deregister.SetDutyId(m_nDutyID);
		Deregister.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
		Deregister.SetOperator(m_strName);

		Deregister.Execute();
		Deregister.SaveTranInfo();
	}	
	//����ʧ��,ֱ���˳�
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		if(FALSE == bSuccessDBOperation){return FALSE;}
	}
	else
	{
		if(0 != Deregister.GetStatusCode()){return FALSE;}
	}

	//�û�����.ʵ���»�
	CActiveMember ActiveMember;
	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(Deregister.GetMemberId(),ActiveMember);
	if(ActiveMember.GetIsOnLine())
	{
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(Deregister.GetMemberId());
		if (nIndex >= 0)
		{
			//ʵ���»�
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			theApp.GetRealName()->AuditUserCheckOut(computer.GetTerminalID(), computer.GetComputerIP(), Deregister.GetMemberId());
			//�Ƕ�ʵ���»�
			if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
			{
				NS_REALNAME::CheckoutInfo coi;
				ZeroMemory(&coi, sizeof(coi));
				CString strName,strSex;
				strSex = ActiveMember.GetSex()?_T("Ů"):_T("��");
				strName.Format(_T("%s(%s)"),ActiveMember.GetUserName(),strSex);
				_tcsncpy(coi.szName, strName, 100);
				_tcsncpy(coi.szNetId, ActiveMember.GetNetId(), 100);
				_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

				CString strIP;
				CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
				_tcsncpy(coi.szIP, strIP, 50);
				coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

				theApp.GetRealName()->CheckOut(&coi);
			}
			//ʵ������ʱ��.(�������δ���ý��˿�,��ִ��ʵ������ʱ��)
			if (CNetBarConfig::GetInstance()->GetReturnOnlyClassId() != ActiveMember.GetClassId())
			{
				theApp.GetRealName()->InvokeRealName(100, (LPVOID)ActiveMember.GetNetId().GetString());
			}
		}	
	}
	//else//�û�������,����ʵ��ҲҪ����ʵ���»� - ����Ҫ��
	//{
	//	if(CIBAGlobal::emRealnameJQ == CNetBarConfig::GetInstance()->GetAuditType())
	//	{
	//		IBA_LOG(_T("����ʵ���»� - �û�������,TermId��IPAddressΪ��"));
	//		CString strTerminalID(_T(""));
	//		CString strComputerIP(_T(""));
	//		theApp.GetRealName()->AuditUserCheckOut(strTerminalID,strComputerIP, Deregister.GetMemberId());
	//	}
	//}

	//�ж��˿�����.�˰�ʱ������,���ôӼ����б���ɾ���û�.
	if (nReturnMode != 2 /*&& !bIsOnLine*/)  //ReturnMode - 0:�����˿� 1:�˿���� 2:���˰�ʱ�����
	{
		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(Deregister.GetMemberId());
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);
	}

	//�Ǳ���ģʽ,Ҫ�������˿��¼�����ؿ�.����ģʽ,���ڲ��ϱ�ʱ����.
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		CReturnedInfo ReturnedInfo;
		ReturnedInfo.SetCardId(_ttoi(ActiveMember.GetNetId()));
		ReturnedInfo.SetReturnDate(CIBAHelpper::FormatCenterTime(Deregister.GetTranTime()));
		ReturnedInfo.SetRefNo(Deregister.GetRefNo());
		ReturnedInfo.SetDeposit(Deregister.GetGuarantyMoney());
		ReturnedInfo.SetOperator(theApp.GetCurCashier()->GetName());
		ReturnedInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
		ReturnedInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
		ReturnedInfo.SetMemberId(Deregister.GetMemberId());
		ReturnedInfo.SetSerialNo(Deregister.GetLocalSerialNo());
		ReturnedInfo.SetReturnAmount(Deregister.GetBalance() + Deregister.GetGuarantyMoney());
		ReturnedInfo.SetClassId(ActiveMember.GetClassId());

		CIBADAL::GetInstance()->AddReturnedRecord(ReturnedInfo);
	}

	return TRUE;
}

BOOL CCashier::DoBalance(CBalanceMethod & Balance)
{
	//2011-03-22-gxx-����Ƿ�ʵ�����ߵ�����
	CActiveMember ActiveMember;

	if( CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(
		Balance.GetMemberId(), ActiveMember) )
	{
		// 2011/06/14-gxx: �������ߣ����ϱ�ʱ����10����֮�ڣ�����Ϊ���û�ʵ������
		BOOL bOnLineInFact = FALSE;
		if (ActiveMember.GetStatus() == CActiveMember::EStatus_LocalOnline)		
		{
			COleDateTime dtTmp = ActiveMember.GetUpdateDataTime() + COleDateTimeSpan(0, 0, 10, 0);
			bOnLineInFact =( dtTmp >= COleDateTime::GetCurrentTime() );
		}

		// 2011/08/03-8201-gxx: ��ʱ�û����Ƿ�ʵ������һֱ��1
		if (ActiveMember.IsMember() == FALSE)
		{
			Balance.SetOnlineInFact(1);
		}
		Balance.SetOnlineInFact(bOnLineInFact);
	}
	else
	{
		IBA_TRACE("��Ա������");
		Balance.SetOnlineInFact(FALSE);
	}
	//


	BOOL bRet = Balance.Execute();


	if (Balance.GetStatusCode() == 0)
	{
		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(
			Balance.GetMemberId(), ActiveMember))
		{
			ActiveMember.SetClassId(Balance.GetUserClass());
			ActiveMember.SetAvailavleBalance(Balance.GetTotalBalacne());

			CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
		}
		CIBADAL::GetInstance()->UpdateMemberInfo(Balance);

		//����AllowReturn��Ϣ�����ر�
		CIBADAL::GetInstance()->UpdateAllowReturn(Balance.GetUserClass(),Balance.GetAllowReturn(),Balance.GetIsMember());
	}
	return bRet;
}

/*************************************************
Function:       // DoCheckCard
Description:    // У���Ա��
Calls:          // CServlet::Execute
Table Accessed: // None
Table Updated: //  None
Input:          // CheckCard�ṹ������SerialNum(��ӡˢ��ˮ��),SerialNumType(0:�ֶ�����ӡˢ�� 1:ʵ������),Operator(����Ա)
Output:         // CheckCard�ṹ������memberId(��Ա��)��cardId(�˺�)
Return:         // �ɹ�����TRUE��ʧ�ܷ���FALSe
Others:         // None
*************************************************/
BOOL CCashier::DoCheckCard(CIDCheckCardMethod& CheckCard)
{
	CheckCard.SetOperator(m_strName);

	return CheckCard.Execute();
}

BOOL CCashier::DoResetPwd(CResetPwdMethod & ResetPwd)
{
	ResetPwd.SetOperator(m_strName);

	BOOL bRet = ResetPwd.Execute();

	return bRet;
}

BOOL CCashier::DoActivation(CActivationMethod & Activation)
{
	BOOL bRet = FALSE;

	bRet = Activation.Execute();

	return bRet;
}

BOOL CCashier::DoStoreIn(CStoreInMethod & StoreIn)
{
	StoreIn.SetDutyId(m_nDutyID);
	StoreIn.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	StoreIn.SetOperator(m_strName);

	BOOL bRet = StoreIn.Execute();

	return bRet;
}

BOOL CCashier::DoShiftTotal(CShiftTotalMethod & ShiftTotal)
{//���෽��
	ShiftTotal.SetDutyId(m_nDutyID);
	ShiftTotal.SetOperator(m_strName);
	ShiftTotal.SetNeedBalance(CIBAConfig::GetInstance()->GetDynamicReserve());

	BOOL bRet = ShiftTotal.Execute();

	return bRet;
}

BOOL CCashier::DoCreditReverse(CCreditReverseMethod & CreditReverse)
{
	CreditReverse.SetOperator(m_strName);
	CreditReverse.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CreditReverse.SetDutyId(m_nDutyID);

	BOOL bRet = CreditReverse.Execute();

	return bRet;

}

/*************************************************
Function:       //DoCheckout2
Description:    //�˹��ǳ���Ա.ʵ���»�.�����û�
Table Updated:  //LastUser
Input:          //Checkout2 -  
				//bClearData - �Ƿ������û��ͱ��ؿ��¼ 
Return:         // �ɹ��򷵻�TRUE�����򷵻�FALSE
*************************************************/
BOOL CCashier::DoCheckout2(CCheckout2Method & Checkout2, BOOL bClearData /*=TRUE*/)
{
	//�û�������,����ǳ�.����IBA�Զ��Ƴ�.(CheckActiveMember)
	CActiveMember ActiveMember;
	BOOL bActive = CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(Checkout2.GetMemberId(),ActiveMember);
	if(FALSE == bActive || FALSE == ActiveMember.GetIsOnLine())
	{
		//2016-0309 ֱ���Ƴ����������IBA�Զ��Ƴ�
		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(Checkout2.GetMemberId());  //ȡ�����ؼ���
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);//������ͼ
		return FALSE;
	}

	BOOL bSuccessDBOperation(TRUE);

	//����ģʽ
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		//��ѯLocalMember�Ƿ��м�¼
		CLocalMember lMember;
		BOOL bHasRecord = CIBADAL::GetInstance()->GetLocalMemberByMemberID(Checkout2.GetMemberId(),lMember);
		if(bHasRecord)//�м�¼,�����˿���Ϣ
		{
			COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
			if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(Checkout2.GetMemberId(),CheckoutTime,TRUE))
			{
				bSuccessDBOperation = FALSE;
				IBA_LOG(_T("Error.���±��ؽ��˼�¼ʧ��."));
			}
		}
		else//�޼�¼,������¼,�����¼,���¼�¼
		{
			GxxDictionaryPtr lmPrt = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(Checkout2.GetMemberId());  //������¼
			if(lmPrt)
			{
				if(CIBADAL::GetInstance()->InsertNewLocalMember(lmPrt))//�����¼
				{
					COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
					if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(Checkout2.GetMemberId(),CheckoutTime,TRUE))//���¼�¼
					{
						bSuccessDBOperation = FALSE;
						IBA_LOG(_T("Error.���±��ؽ��˼�¼ʧ��."));
					}	
				}
				else
				{
					bSuccessDBOperation = FALSE;
					IBA_LOG(_T("Error.Ϊ���˲��뱾�ؼ�¼ʧ��."));
				}
			}
			else
			{
				bSuccessDBOperation = FALSE;
				IBA_LOG(_T("Error.Ϊ���˴������ؼ�¼ʧ��."));
			}
		}
	}
	else//����ģʽ
	{
		//�˹��ǳ�
		Checkout2.SetOperator(m_strName);
		Checkout2.Execute();
	}

	//Fail.����FALSE
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		if(FALSE == bSuccessDBOperation){return FALSE;}
	}
	else
	{
		if(0 != Checkout2.GetStatusCode()){return FALSE;}
	}

	//Success
	//ʵ���»�
	INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(Checkout2.GetMemberId());
	if (nIndex >= 0)
	{
		CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
		theApp.GetRealName()->AuditUserCheckOut(computer.GetTerminalID(), computer.GetComputerIP(), Checkout2.GetMemberId());
		//�Ƕ�ʵ��
		if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
		{
			NS_REALNAME::CheckoutInfo coi;
			ZeroMemory(&coi, sizeof(coi));
			CString strName,strSex;
			strSex = ActiveMember.GetSex()?_T("Ů"):_T("��");
			strName.Format(_T("%s(%s)"),ActiveMember.GetUserName(),strSex);
			_tcsncpy(coi.szName, strName, 100);
			_tcsncpy(coi.szNetId, ActiveMember.GetNetId(), 100);
			_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

			CString strIP;
			CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
			_tcsncpy(coi.szIP, strIP, 50);
			coi.nCheckoutType = CIBAGlobal::cot_MemLogout;

			theApp.GetRealName()->CheckOut(&coi);
		}
	}
	//�����б���ɾ���û�,ɾ�����ؿ��¼(LastUser)
	if(bClearData)
	{
		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(Checkout2.GetMemberId());  //ȡ�����ؼ���
		CIBADAL::GetInstance()->DeleteLastUser(Checkout2.GetMemberId()); //��LastUser��ɾ����Ӧ��¼
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);//������ͼ
	}
	return TRUE;	
}

BOOL CCashier::DoModifyMember(CModifyMemberMethod & ModifyMember)
{
	ModifyMember.SetOperator(m_strName);

	BOOL bRet = ModifyMember.Execute();

	return bRet;
}

BOOL CCashier::DoBundleTimeList(CBundleTimeListMethod & BundleTimeList)
{
	BundleTimeList.SetOperator(m_strName);
	BundleTimeList.SetPosition(0);
	BOOL bRet = BundleTimeList.Execute();

	return bRet;
}

BOOL CCashier::DoBundleTimeAction(CBundleTimeActionMethod & BundleTimeAction)
{
	BOOL bRet = BundleTimeAction.Execute();

	return bRet;
}

BOOL CCashier::DoBundleTime(CBundleTimeMethod & BundleTime)
{
	BundleTime.SetOperator(m_strName);
	BundleTime.SetDutyId(m_nDutyID);
	BundleTime.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	BundleTime.SetNetbarId(CNetBarConfig::GetInstance()->GetNetBarId());

	BOOL bRet = BundleTime.Execute();

	return bRet;
}

BOOL CCashier::DoCancelBundleTime(CCancelBundleTimeMethod & CancelBundleTime)
{
	CancelBundleTime.SetOperator(m_strName);
	CancelBundleTime.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());

	BOOL bRet = CancelBundleTime.Execute();

	return bRet;
}

BOOL CCashier::DoCardReplace(CCardReplaceMethod & CardReplace)
{
	CardReplace.SetOperator(m_strName);
	CardReplace.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CardReplace.SetDutyId(m_nDutyID);

	BOOL bRet = CardReplace.Execute();

	return bRet;
}

BOOL CCashier::DoMergeMember(CMergeMemberMethod & MergeMember)
{
	MergeMember.SetOperator(m_strName);
	MergeMember.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	MergeMember.SetDutyId(m_nDutyID);
	MergeMember.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());

	BOOL bRet = MergeMember.Execute();

	return bRet;
}


BOOL CCashier::DoShiftCredit(CShiftCreditMethod & ShiftCredit)
{
	ShiftCredit.SetOperator(m_strName);
	ShiftCredit.SetDutyId(m_nDutyID);

	BOOL bRet = ShiftCredit.Execute();

	return bRet;
}

BOOL CCashier::DoConsumeList(CConsumeListMethod & ConsumeList)
{
	ConsumeList.SetOperator(m_strName);
	ConsumeList.SetNeedBusinessName(2);

	BOOL bRet = ConsumeList.Execute();

	return bRet;
}

BOOL CCashier::DoTermConsume(CTermConsumeMethod & TermConsume)
{
	TermConsume.SetOperator(m_strName);

	BOOL bRet = TermConsume.Execute();

	return bRet;
}

BOOL CCashier::DoUpgrade(CUpgradeMethod & Upgrade)
{
	Upgrade.SetOperator(m_strName);

	BOOL bRet = Upgrade.Execute();

	return bRet;
}

BOOL CCashier::DoCardCredit(CCardCreditMethod & CardCredit)
{
	CardCredit.SetOperator(m_strName);
	CardCredit.SetBusinessType(0);
	CardCredit.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CardCredit.SetDutyId(m_nDutyID);

	BOOL bRet = CardCredit.Execute();

	return bRet;
}

BOOL CCashier::DoRetail(CRetailMethod & RetailMethod)
{
	RetailMethod.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	RetailMethod.SetDutyId(m_nDutyID);
	RetailMethod.SetOperator(m_strName);
	RetailMethod.SetTermTime(CIBAHelpper::GetCurrentTimeAsString());
	RetailMethod.SetStoreId(CIBAConfig::GetInstance()->GetStoreID());

	BOOL bRet = RetailMethod.Execute();

	return bRet;
}

BOOL CCashier::DoRetailOnline(CRetailOnlineMethod & RetailOnlineMethod)
{
	RetailOnlineMethod.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	RetailOnlineMethod.SetDutyId(m_nDutyID);
	RetailOnlineMethod.SetOperator(m_strName);
	RetailOnlineMethod.SetTermTime(CIBAHelpper::GetCurrentTimeAsString());
	RetailOnlineMethod.SetStoreId(CIBAConfig::GetInstance()->GetStoreID());

	BOOL bRet = RetailOnlineMethod.Execute();

	return bRet;
}

BOOL CCashier::DoStoreQuantity(CStoreQuantityMethod & StoreQuantity)
{
	StoreQuantity.SetOperator(m_strName);
	StoreQuantity.SetStoreId(CIBAConfig::GetInstance()->GetStoreID());

	BOOL bRet = StoreQuantity.Execute();

	return bRet;
}

BOOL CCashier::DoUsePoint(CUsePointMethod & UsePoint)
{
	UsePoint.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
	UsePoint.SetOperator(m_strName);
	UsePoint.SetBusinessType(0);

	BOOL bRet = UsePoint.Execute();

	return bRet;
}

// ִ�������ķ��Ͷ�����֪ͨ
BOOL CCashier::DoLocalMode()
{
	CLocalModeMethod LocalMode;

	LocalMode.SetOperationType(2);//�رն���

	if (m_strName.IsEmpty())
	{
		LocalMode.SetOperator(_T("admin"));//Ĭ�ϵĶ����û�
	} 
	else
	{
		LocalMode.SetOperator(m_strName);//��ǰ����Ա
	}
	LocalMode.SetLastOperationTime(CIBAHelpper::GetCurrentTimeAsString());

	BOOL bRet = LocalMode.Execute();

	return bRet;
}

BOOL CCashier::DoAssetStatus()
{
	CAssetStatusMethod AssetStatus;

	DWORD length = MAX_PATH;
	TCHAR szComputerName[MAX_PATH] = {0};
	GetComputerName(szComputerName, &length);

	AssetStatus.SetTermId(szComputerName);//�ն˺�
	AssetStatus.SetAssetId(CNetBarConfig::GetInstance()->GetICControlVerID());//ICC�汾��
	// 2014-4-28 qsc
	//QSCT1 AssetStatus.SetAssetStatus(CIBALog::GetInstance()->GetAppVer());//����̨����汾��
	{
		// 2014-4-28 ����SFREG�汾�ϱ� QSCT1
		// memcpy(checkin.version, (LPCSTR)strVer, strVer.GetLength());

		CString strSFREGPath = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\sfreg.exe");
		CString strSfregVerion;
		strSfregVerion.Empty();
		if(PathFileExists(strSFREGPath))
		{
			CFileVersionInfo fvi;
			if (!fvi.Create(strSFREGPath))
			{
				IBA_LOG0(_T("error: ��ȡsfreg.exe�İ汾��ʧ��"));
			}
			else
			{
				IBA_LOG(_T("sfreg.exe version :%s"), fvi.GetFileVersion());
				strSfregVerion = fvi.GetFileVersion();
			}
		}
		CString strAllVer;
		if (strSfregVerion.IsEmpty())
		{
			strAllVer = CIBALog::GetInstance()->GetAppVer(); 
		}
		else
		{
			strAllVer.Format(_T("%s,%s"), CIBALog::GetInstance()->GetAppVer(), strSfregVerion);
		}
		AssetStatus.SetAssetStatus(strAllVer);
	}
	BOOL bRet = AssetStatus.Execute();

	return bRet;
}


void CCashier::WriteLoginInfo(CString strOperator)//��¼��дע���
{
	//дע���֮ǰ�������setProfilekey��дǰ׼��
	theApp.WriteProfileString(_T("NetBarInfo"), _T("LastOperator"), strOperator);

	//����ע���
	CString strTmp = theApp.GetProfileString(_T("NetBarInfo"), _T("Operators"));
	strTmp.Trim();
	if (strTmp.IsEmpty())
	{
		//��������û���ע���
		COperatorArray OperatorArray;
		CIBADAL::GetInstance()->GetInstance()->GetOperatorRecord(OperatorArray);

		for (INT_PTR i = 0; i < OperatorArray.GetCount(); i ++)
		{
			strTmp += OperatorArray.GetAt(i) + _T("|");
		}
		theApp.WriteProfileString(_T("NetBarInfo"), _T("Operators"), strTmp);
	}
}

BOOL CCashier::DoBundleTime2(CBundleTime2Method & BundleTime2)
{	
	BundleTime2.SetOperator(m_strName);
	
	return BundleTime2.Execute();
}

BOOL CCashier::DoBundleTimeList2(CBundleTimeList2Method & BundleTimeList2)
{
	BundleTimeList2.SetOperator(m_strName);
	
	return BundleTimeList2.Execute();
}

BOOL CCashier::DoShiftList(CShiftListMethod & ShiftList)
{
	ShiftList.SetOperator(m_strName);

	return ShiftList.Execute();
}

BOOL CCashier::DoDetailData(CDetailDataMethod & DetailData)
{
	return DetailData.Execute();
}

BOOL CCashier::DoCheckIn(CCheckInMethod & CheckInData)
{
	CheckInData.SetClientVersion(_T("9.9.9.9"));
	CheckInData.SetEncryptMode(1);

	return CheckInData.Execute();
}

BOOL CCashier::DoCreditDirect(CCreditDirectMethod& CreditDirect)
{
	CreditDirect.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CreditDirect.SetDutyId(m_nDutyID);
	CreditDirect.SetOperator(m_strName);
	CreditDirect.SetTermTime(CIBAHelpper::GetCurrentTimeAsString());

	return CreditDirect.Execute();
}

BOOL CCashier::DoConsumeListEx(CConsumeListMethodEx & ConsumeListEx)
{
	ConsumeListEx.SetOperator(m_strName);

	return ConsumeListEx.Execute();
}

BOOL CCashier::LocalModeLogin()
{
	CString strPwdMD5;
	CEncrypt::CalcMD5(m_strPassword, strPwdMD5);

	CString strSQL;
	strSQL.Format(_T("select * from cashregister where ID = %d"), CIBAConfig::GetInstance()->GetCashRegisterID());

	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		if (Rs.GetRecordCount() == 0)
		{
			Rs.Close();
			pDb->Release();
			return FALSE;
		}

		CString strName;
		if (!Rs.GetFieldValue(_T("name"),strName))
		{
			Rs.Close();
			pDb->Release();
			return FALSE;
		}
		CString strPwd;
		if (!Rs.GetFieldValue(_T("password"),strPwd))
		{
			Rs.Close();
			pDb->Release();
			return FALSE;
		}

		if (strPwd != strPwdMD5 || strName != m_strName)
		{
			Rs.Close();
			pDb->Release();
			return FALSE;
		}

		m_bIsLocalModeLogin = TRUE;
	
		CString strID;
		Rs.GetFieldValue(_T("dutyID"),strID);
		m_nDutyID = (UINT)_ttoi(strID);

		m_bLogin = TRUE;
		m_bIsBoss = FALSE;
		// 2013-12-3 15:26:04 qsc ���ַ�ʽ���У���Ϊ2591tset,2591a��ת�������ֺ�2591���
		// if (_ttoi(m_strName) == CNetBarConfig::GetInstance()->GetNetBarId())
		CString strNetbarId;
		strNetbarId.Format(_T("%d"), CNetBarConfig::GetInstance()->GetNetBarId());

		if(strNetbarId.GetLength() == m_strName.GetLength() &&
			strNetbarId == m_strName)
		{	
			//����ID�˻���¼
			m_bIsBoss = TRUE;
		}
		m_bOnDuty = TRUE;
		if (m_nDutyID==0)
		{
			m_nDutyID = 1;
		}

		CBrokenNetwork::GetInstance()->LoadLocalStatusInfo();

		// 2011/11/09-8210-gxx: 
		NS_DAL::CRightsManagement::GetInstance()->CreateManagement(m_strEnableModuleList, m_bIsBoss);

		Rs.Close();
		pDb->Release();

		// 2011/07/25-8201-gxx: 
		((CMainFrame*)theApp.GetMainWnd())->m_wndOutput.RefreshAllInfoList();

		return TRUE;
	}

	pDb->Release();
	return FALSE;
}

BOOL CCashier::DoLocalConsume(CLocalConsumeMethod& LocalConsume)
{
	return LocalConsume.Execute();
}

BOOL CCashier::DoNetbarRoom(CNetbarRoomMethod& RoomMethod)
{
	RoomMethod.SetOperator(m_strName);

	return RoomMethod.Execute();
}

BOOL CCashier::DoRoomCheckIn(CRoomCheckIn& RoomCheckInMethod)
{
	RoomCheckInMethod.SetOperator(m_strName);
	RoomCheckInMethod.SetDutyId(m_nDutyID);
	RoomCheckInMethod.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());

	return RoomCheckInMethod.Execute();
}

BOOL CCashier::DoRoomCheckOut(CRoomCheckOut& RoomCheckOutMethod)
{
	RoomCheckOutMethod.SetOperator(m_strName);
	RoomCheckOutMethod.SetDutyId(m_nDutyID);
	RoomCheckOutMethod.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());

	return RoomCheckOutMethod.Execute();
}

BOOL CCashier::MonitorAuditProxy()
{
	if (CNetBarConfig::GetInstance()->GetMonitorAuditProxy() == 0)
	{
		return TRUE;
	}
	if (CIBAConfig::GetInstance()->GetCashRegisterID() != 1)
	{
		if (CNetBarConfig::GetInstance()->GetAllowOpenUser() == FALSE)
		{
			// ������̨��������
			AfxMessageBox(L"������̨δ��������������̨�������δ����������̨��ֹ��������������ȷ�ϴ��������ȷ������������������̨!", MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}
	m_bAuditProxyRunning = TRUE;


#ifdef DEBUG
	CString strProxy(theApp.GetWorkPath());
	strProxy.Append(_T("\\IBABin\\123.exe"));

	CL2Process SunlikeAuditProxy;
	SunlikeAuditProxy.Attach(strProxy);
	BOOL bRes = SunlikeAuditProxy.CheckRunning(NULL);
	if (SunlikeAuditProxy.IsRunning())
	{
		AfxMessageBox(L"Is Running");
	}
#else
	
	

	if (CIBAConfig::GetInstance()->GetCashRegisterID() == 1)
	{
		// ������̨�����������ж�
		HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("_SUNLIKEAUDITPROXY_MUTEX_"));
		if (hMutex != NULL)
		{
			CloseHandle(hMutex);
			CString strProxy(theApp.GetWorkPath());
			strProxy.Append(_T("\\IBABin\\SunlikeAuditProxy.exe"));

			CL2Process SunlikeAuditProxy;
			SunlikeAuditProxy.Attach(strProxy);
			if (!SunlikeAuditProxy.IsRunning())
			{
				m_bAuditProxyRunning = FALSE;
			}
		}
		else
		{
			m_bAuditProxyRunning = FALSE;
		}
		if (m_bAuditProxyRunning == FALSE)
		{
			AfxMessageBox(L"�������û��������·����������̨��ֹ��������������ȷ�ϴ��������ȷ���������µ�������̨!", MB_OK|MB_ICONWARNING);
			CNetBarConfig::GetInstance()->SetAllowOpenUser(FALSE);
		}
		else
		{
			CNetBarConfig::GetInstance()->SetAllowOpenUser(TRUE);
		}
	}
	
#endif

	return m_bAuditProxyRunning;
	
}

void CCashier::SetCenterAddr(CCashRegisterLoginMethod& CashRegisterLogin)
{
	// 2014-8-18 - qsc ��¼��ʱ�����ϵ��߼�
	CString strMain = CashRegisterLogin.GetMainHost();
	CString strBk = CashRegisterLogin.GetBackupHost();
	CString strStandby = CashRegisterLogin.GetStandbyHost();
	CNetBarConfig::GetInstance()->SetMainHost(strMain);
	CNetBarConfig::GetInstance()->SetBackUpHost(strBk);
	CNetBarConfig::GetInstance()->SetStandbyHost(strStandby);//��ط�����


	//// ����ǰ��ͨ�ĵ�ַ�ͷ��ص����ĵ�ַ��һ�£����滻�����ĵ�ַΪ��ǰ��ַ
	//// Ȼ��ƥ��͵�ǰ��ַ��ͬ�ı����ĵ�ַ��������ĵ�ַ���滻Ϊ���ص����ĵ�ַ
	//CString strRequesAddr = CashRegisterLogin.GetUrlFromIndex(CashRegisterLogin.m_nFirstCenterIndex);
	//CString strMain = CashRegisterLogin.GetMainHost();
	//CString strBk = CashRegisterLogin.GetBackupHost();
	//CString strStandby = CashRegisterLogin.GetStandbyHost();

	//IBA_LOG(_T("��¼�ɹ���ַ:%s,��%s,��:%s,��:%s"), strRequesAddr, strMain, strBk, strStandby);
	//if(0 != strRequesAddr.CompareNoCase(strMain))
	//{
	//	if(0 == strRequesAddr.CompareNoCase(strBk))
	//	{
	//		strBk = strMain;
	//	}
	//	else if(0 == strRequesAddr.CompareNoCase(strStandby))
	//	{
	//		strStandby = strMain;
	//	}	
	//	strMain = strRequesAddr;
	//}
	//
	//IBA_LOG(_T("��%s,��:%s,��:%s"), strMain, strBk, strStandby);
	//CNetBarConfig::GetInstance()->SetMainHost(strMain);
	//CNetBarConfig::GetInstance()->SetBackUpHost(strBk);
	//CNetBarConfig::GetInstance()->SetStandbyHost(strStandby);//��ط�����
}

BOOL CCashier::DoBuckleAmount( CBuckleAmountMethod& BuckleAmountMethod )
{
	BuckleAmountMethod.SetOperator(m_strName);
	//BuckleAmountMethod.SetDutyId(m_nDutyID);
	return BuckleAmountMethod.Execute();
}
