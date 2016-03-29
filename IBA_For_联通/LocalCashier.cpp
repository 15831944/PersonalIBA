// LocalCashier.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include ".\localcashier.h"

// CLocalCashier

CLocalCashier::CLocalCashier()
{
	
}

CLocalCashier::~CLocalCashier()
{
}

BOOL CLocalCashier::CheckIn(CCashRegisterLoginMethod& CashRegisterLogin, CString strOperator)
{
	SetLocaNetbarlInfo();

	BOOL bRet = FALSE;
	
	CashRegisterLogin.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
	CashRegisterLogin.SetCashRegisterVersion(CIBALog::GetInstance()->GetAppVer());
	///////////////////////����ģʽ1ʱ������������0������֤��ʽ//////////////////////
	CashRegisterLogin.SetCashRegisterType(0);
	CashRegisterLogin.SetUserId(m_strName);
	CashRegisterLogin.SetPassword(m_strPassword);

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
		//
		if (_ttoi(m_strName) == CNetBarConfig::GetInstance()->GetNetBarId())
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
		m_nShiftType = CashRegisterLogin.GetShiftType();

		CashRegisterLogin.GetUserClassInfoArray(CNetBarConfig::GetInstance()->m_UserClassArray);
		//����ʶ��
		CNetBarConfig::GetInstance()->SetIssueNetBarId(CashRegisterLogin.GetIssueNetBarId());
		
		//Ȩ���б�

		m_strEnableModuleList = L"IBAP_TERMCONSUME,IBAP_CONSUMELIST,IBAP_OPERATIONQUERY,IBAP_REGISTER,IBAP_CASHSHIFTQUERY,IBAP_MODIFYPWD,IBAP_NETBARSET,IBAP_BUNDLETIME,IBAP_CONFIG,IBAP_CREDIT";

		m_strEnableModuleList.Append(_T(","));

		//�����������ʱ����
		CNetBarConfig::GetInstance()->m_NetbarBundleTime.Start();

		//�������ע���LastTran��Է���ɵĸ���
		theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\NetBarInfo\\LastTran"));
	}

	return bRet;
}

// ִ�������ķ��Ͷ�����֪ͨ
BOOL CLocalCashier::DoLocalMode()
{
	CLocalModeMethod LocalMode;

	LocalMode.SetOperationType(1);//

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

// ��ʼ��ʱ���ñ������ݿ�
BOOL CLocalCashier::SetLocaNetbarlInfo(void)
{
	CADODatabase LocalNetbar;

	if (!LocalNetbar.Open(_T("DSN=LOCALNETBAR;"))) 
	{
		CIBALog::GetInstance()->Write(_T("LocalNetbar Connected faile!"));
		return FALSE;
	}

	BOOL bRet = FALSE;

	CString strSQL;
	strSQL.Format(_T("select * from netbarcashregister where CashRegisterID = %d"), CIBAConfig::GetInstance()->GetCashRegisterID());//����


	CADORecordset Rs(&LocalNetbar);

	if (Rs.Open(strSQL))//��
	{
		if (Rs.GetRecordCount() == 0)
		{
			Rs.AddNew();
		}
		else
		{
			Rs.Edit();
			Rs.MoveFirst();
		}

		Rs.SetFieldValue(_T("CashRegisterID"), CIBAConfig::GetInstance()->GetCashRegisterID());
		Rs.SetFieldValue(_T("NetBarID"), CNetBarConfig::GetInstance()->GetNetBarId());
		Rs.SetFieldValue(_T("CashRegisterName"), _T("IBA"));

		CString strTmp, strIP;
		CIBAHelpper::GetIPAndMAC(strIP, strTmp);
		strTmp.Replace(_T("-"), _T(""));
		Rs.SetFieldValue(_T("pcMAC"), strTmp);
		
		Rs.SetFieldValue(_T("RemoteLogin"), 1);

		bRet = Rs.Update();
		Rs.Close();
	}

	LocalNetbar.Close();

	return bRet;
}


BOOL CLocalCashier::CheckLastDuty(CString& strLastOperator)
{
	return TRUE;
}

void CLocalCashier::WriteDutyInfo()
{
	//����ʲô��������
}