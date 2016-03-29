#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include "dal\IBADAL.h"
#include "Socket\LocalServer.h"
#include "resource.h"
#include "ServletHellper.h"
#include ".\activememberlist.h"
#include "ComputerListView.h"
#include "Encrypt.h"
#include "CashierHelper.h"
#include "MainFrm.h"
#include "RealName\IBARealName.h"

CActiveMemberList::CActiveMemberList(void)
{
	m_hWndGrid = NULL;
	m_CurPos = NULL;

	m_ActiveMemberMap.InitHashTable(257);
}

CActiveMemberList::~CActiveMemberList(void)
{
}

void CActiveMemberList::RefreshView()
{
	PostMessage(m_hWndGrid, WM_TIMER, 1001, NULL); //ˢ���û���ͼ
}

BOOL CActiveMemberList::LoadFromDB()
{
	Clear();

	BOOL bRet = FALSE;

	CString strSQL;

	strSQL = _T("select activemember.*, member.classId as userClassId from activemember");

	strSQL.Append(_T(" left outer join member on activemember.memberId = member.memberId where activemember.memberId <> 0"));

	IBA_TRACE(strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL)) 
	{
		bRet = TRUE;
	}
	else
	{
		pDb->Release();
		return FALSE;
	}

	int nCount = Rs.GetRecordCount();

	for (UINT i = 0; i < Rs.GetRecordCount(); i++)
	{
		CActiveMember ActiveMember;

		CString strTmp; INT nTmp = 0;

		if (Rs.GetFieldValue(_T("memberId"), nTmp))
		{
			ActiveMember.SetMemberID(nTmp);
		}

		if (Rs.GetFieldValue(_T("netId"), strTmp))
		{
			ActiveMember.SetNetId(strTmp);
		}

		if (Rs.GetFieldValue(_T("userClassId"), nTmp))
		{
			ActiveMember.SetClassId(nTmp);
		}

		if (Rs.GetFieldValue(_T("name"), strTmp))
		{
			ActiveMember.SetUserName(strTmp);
		}

		if (Rs.GetFieldValue(_T("sex"), nTmp))
		{
			ActiveMember.SetSex(nTmp);
		}

		if (Rs.GetFieldValue(_T("idNumber"), strTmp))
		{
			ActiveMember.SetPersonalID(strTmp);
		}

		if (Rs.GetFieldValue(_T("idType"), nTmp))
		{
			ActiveMember.SetIdType(nTmp);
		}

		if (Rs.GetFieldValue(_T("lastRemainAmount"), nTmp))
		{
			ActiveMember.SetAvailavleBalance(nTmp);
		}

		if (Rs.GetFieldValue(_T("amount"), nTmp))
		{
			ActiveMember.SetAmount(nTmp);
		}

		if (Rs.GetFieldValue(_T("refNo"), nTmp))
		{
			//ActiveMember.SetIsOnLine( nTmp > 0 );

			ActiveMember.SetRefNo(nTmp);
		}

		if (Rs.GetFieldValue(_T("status"), nTmp))
		{
			ActiveMember.SetStatus(nTmp);
		}

		Rs.GetFieldValue(_T("activationTime"), ActiveMember.ActivationTime);

		Rs.GetFieldValue(_T("updateTime"), ActiveMember.UpdateDataTime);

		Rs.GetFieldValue(_T("checkinTime"), ActiveMember.CheckInTime);

		// 2011/08/10-8201-gxx: 
		Rs.GetFieldValue(_T("nextCheckinTime"), ActiveMember.NextCheckinTime);

		if (CIBAHelpper::IsValidTime(ActiveMember.CheckInTime))
		{
			ActiveMember.SetIsOnLine(TRUE);
		}

		

		if (Rs.GetFieldValue(_T("termId"), strTmp))
		{
			ActiveMember.SetTerminalID(strTmp);
		}

		if (Rs.GetFieldValue(_T("pcClass"), nTmp))
		{
			ActiveMember.SetPCClassID(nTmp);
		}

		if (Rs.GetFieldValue(_T("isActive"), nTmp))
		{
			ActiveMember.SetActive(nTmp);
		}

		if (Rs.GetFieldValue(_T("payType"), nTmp))
		{
			ActiveMember.SetPayType(nTmp);
		}

		if (Rs.GetFieldValue(_T("advancePayment"), nTmp))
		{
			ActiveMember.SetAdvancePayment(nTmp);
		}

		if (Rs.GetFieldValue(_T("creditProcess"), strTmp))
		{
			ActiveMember.SetCreditProcess(strTmp);
		}

		// 2011-3-29-gxx-�������̨ID�Ķ�ȡ
		if (Rs.GetFieldValue(_T("lastNetBarId"), nTmp))
		{
			ActiveMember.SetCasherID(nTmp);
		}

		// 2011/05/13-gxx: ��ӱ�ע
		if (Rs.GetFieldValue(_T("Remark"), strTmp))
		{
			ActiveMember.SetRemark(strTmp);
		}

		// 2011/05/19-gxx: �����Ƿ�Ϊ��ʱ
		if (Rs.GetFieldValue(_T("registerBT"), nTmp))
		{
			ActiveMember.SetIsRegisterBT(nTmp);
		}

		// 2011/07/08-8201-gxx: �ۼƷⶥ��
		if (Rs.GetFieldValue(_T("dynamicMaxAmount"), nTmp))
		{
			ActiveMember.SetDynamicMaxAmount(nTmp);
		}

		// 2011/07/08-8201-gxx: �Ƿ񱾵��û�
		if (Rs.GetFieldValue(_T("isLocal"), nTmp))
		{
			ActiveMember.SetIsLocalUser(nTmp);
		}

		// 2011/07/08-8201-gxx: �ϻ�����, MD5ֵ
		if (Rs.GetFieldValue(_T("password"), strTmp))
		{
			ActiveMember.SetPassword(strTmp);
		}

		// 2011/07/18-8201-gxx: ����
		if (Rs.GetFieldValue(_T("PolicyInfo"), strTmp))
		{
			ActiveMember.SetPolicyInfo(strTmp);
		}

		// 2011/07/18-8201-gxx: ���ñ����û��ļ���ID�� ȷ���Ƿ�Ϊ�ָ�״̬
		if (ActiveMember.GetIsLocalUser())
		{
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
			{
				// ת����Ϊ�ָ�״̬
				CBrokenNetwork::SettingNetworkStatus(CIBAGlobal::emNetworkRestore);		
			}

			if (Rs.GetFieldValue(_T("classId"), nTmp))
			{
				ActiveMember.SetClassId(nTmp);
			}
		}

		// 2011/10/25-8230-gxx: 
		if (Rs.GetFieldValue(_T("roomMemberId"), nTmp))
		{
			ActiveMember.SetRoomMemberId(nTmp);
		}

		UpdateActiveMember(ActiveMember);

		Rs.MoveNext();
	}

	IBA_TRACE("�û�����Ϊ:%d",Rs.GetRecordCount());

	Rs.Close();
	
	pDb->Release();

	return bRet;
}

void CActiveMemberList::AddActiveMember(NS_DAL::CLocalRegisterInfo& RegisterInfo)
{
	CActiveMember ActiveMember;

	ActiveMember.SetNetbarID(RegisterInfo.GetNetbarId());
	ActiveMember.SetMemberID(RegisterInfo.GetMemberId());
	ActiveMember.SetClassId(RegisterInfo.GetUserClass());
	ActiveMember.SetUserName(RegisterInfo.GetUserName());
	ActiveMember.SetNetId(RegisterInfo.GetNetId());
	ActiveMember.SetIdType(RegisterInfo.GetIdType());
	ActiveMember.SetPersonalID(RegisterInfo.GetPersonalId());
	ActiveMember.SetSex(RegisterInfo.GetSex());
	ActiveMember.SetAvailavleBalance(RegisterInfo.GetMoney());

	ActiveMember.SetAmount(0);
	ActiveMember.ActivationTime = COleDateTime::GetCurrentTime();
	ActiveMember.SetActive(TRUE);
	ActiveMember.SetStatus(CActiveMember::EStatus_Waiting);

	ActiveMember.SetAdvancePayment(ActiveMember.GetAvailavleBalance());

	ActiveMember.SetCreditProcess(ActiveMember.GetAdvancePaymentAsString());
	ActiveMember.SetCasherID(CIBAConfig::GetInstance()->GetCashRegisterID());

	ActiveMember.SetIsLocalUser(TRUE);
	
	CString strPwd = CIBAHelpper::EncodePassword(RegisterInfo.GetPassword());
	ActiveMember.SetPassword(strPwd);

	CString strNetId = ActiveMember.GetNetId();
	strNetId = strNetId.Right(8);
	strNetId.TrimLeft(_T('0')); // ���˵���� ��0
	ActiveMember.SetNetIdAlias(strNetId);

	RemoveActiveMember(ActiveMember.GetMemberID());

	UpdateActiveMember(ActiveMember);

	AddActiveMemberDB(ActiveMember);

	CIBADAL::GetInstance()->AddMemberInfo(RegisterInfo);

	//
	CLocalServer::GetInstance()->GetActiveMemberView()->GetActiveMemberGrid()->AddNewRow(ActiveMember, TRUE);

	int nAt = CLocalServer::GetInstance()->GetActiveMemberView()->FindUserAt(1, ActiveMember.GetNetId());
	//
	CLocalServer::GetInstance()->GetActiveMemberView()->SetSelUser(nAt, TRUE);
}

void CActiveMemberList::AddActiveMember(CBalanceMethod & Balance, BOOL bRegisterBT, // �Ƿ񿪻���ʱ
										UINT nPayout, int nBundleTimeType, int nScanType)
{
	CActiveMember ActiveMember;

	ActiveMember.SetNetbarID(Balance.GetOpenNetbarId());
	ActiveMember.SetMemberID(Balance.GetMemberId());
	ActiveMember.SetClassId(Balance.GetUserClass());
	ActiveMember.SetUserName(Balance.GetUserName());
	ActiveMember.SetNetId(Balance.GetSerailNum());
	ActiveMember.SetIdType(Balance.GetIdType());
	ActiveMember.SetPersonalID(Balance.GetPersonalId());
	ActiveMember.SetSex(Balance.GetSex());
	ActiveMember.SetAvailavleBalance(Balance.GetTotalBalacne());
	ActiveMember.SetScanType(nScanType);

	//{ 2011/05/19-gxx: �ж��Ƿ񿪻���ʱ
	if (bRegisterBT )
	{
		if (nBundleTimeType == CActiveMember::EPayType_Withholding)
		{
			IBA_CHECK_UINT(nPayout);
			ActiveMember.SetAmount(nPayout);
		}
		
		ActiveMember.SetIsRegisterBT(TRUE);
	}
	else
	{
		ActiveMember.SetAmount(0);
	}
	//}
	
	ActiveMember.ActivationTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetServerTime());
	ActiveMember.SetActive(TRUE);
	ActiveMember.SetStatus(CActiveMember::EStatus_Waiting);

	// 2011/04/25-gxx: =����� (����Ԥ�����ԱΪ����ʱ�û�Ϊ���˽��) + ����
	ActiveMember.SetAdvancePayment(Balance.GetTotalBalacne()+ActiveMember.GetAmount());
	
	ActiveMember.SetCreditProcess(ActiveMember.GetAdvancePaymentAsString());
	ActiveMember.SetCasherID(CIBAConfig::GetInstance()->GetCashRegisterID());

	CString strNetId = ActiveMember.GetNetId();
	strNetId = strNetId.Right(8);
	strNetId.TrimLeft(_T('0')); // ���˵���� ��0
	ActiveMember.SetNetIdAlias(strNetId);

	CActiveMember tmAct;
	if (GetActiveMemberNetID(ActiveMember.GetNetId(),tmAct, TRUE))
	{
		RemoveActiveMember(tmAct.GetMemberID());
	}
	else
	{
		RemoveActiveMember(ActiveMember.GetMemberID());
	}
	

	UpdateActiveMember(ActiveMember);

	AddActiveMemberDB(ActiveMember);

	//
	CLocalServer::GetInstance()->GetActiveMemberView()->GetActiveMemberGrid()->AddNewRow(ActiveMember, TRUE);

}



void CActiveMemberList::UpdateActiveMember(CActiveMember& ActiveMember, BOOL bUpdateDb /*= FALSE*/)
{
	//�ϻ�ʱ���������ʾ�û�����
	if (CIBAHelpper::IsValidTime(ActiveMember.GetCheckInTime()) &&
		ActiveMember.GetCheckInTime() > COleDateTime(2011,1,1,0,0,0))
	{
		ActiveMember.SetIsOnLine( TRUE );
	}

	CSingleLock lock(&ActiveMemberListLock, TRUE);

	m_UpdateMemberIdArray.Add(ActiveMember.GetMemberID());
	m_ActiveMemberMap.SetAt(ActiveMember.GetMemberID(), ActiveMember);

	lock.Unlock();

	if (bUpdateDb)
	{
		UpdateActiveMemberDB(ActiveMember);
	}
}

void CActiveMemberList::UpdateActiveMember(UINT nMemberId, UINT nCreditMoney, UINT nCreditPresent, UINT nPayout /*= 0*/)
{
	CActiveMember ActiveMember;

	if (GetActiveMember(nMemberId, ActiveMember))
	{
		// 2011/05/17-gxx: 
		//ActiveMember.SetAdvancePayment(ActiveMember.GetAdvancePayment() + nCreditMoney + nCreditPresent);
        ActiveMember.SetAdvancePayment(ActiveMember.GetAdvancePayment() + nCreditMoney + nCreditPresent-nPayout);

		CString strCreditProcess(ActiveMember.GetCreditProcess());

		if (nCreditMoney > 0)
		{
			if (nCreditMoney % 100 == 0)
			{
				strCreditProcess.AppendFormat(_T("+%d"), nCreditMoney / 100);
			}
			else
			{
				strCreditProcess.AppendFormat(_T("+%.1f"), nCreditMoney / 100.0);
			}
		}

		if (nCreditPresent > 0)
		{
			if (nCreditPresent % 100 == 0)
			{
				strCreditProcess.AppendFormat(_T("��%d"), nCreditPresent / 100);
			}
			else
			{
				strCreditProcess.AppendFormat(_T("��%.1f"), nCreditPresent / 100.0);
			}
		}

		if (nPayout > 0)
		{
			ActiveMember.SetAvailavleBalance(ActiveMember.GetAvailavleBalance() - nPayout);

			if (nPayout % 100 == 0)
			{
				strCreditProcess.AppendFormat(_T("-%d"), nPayout / 100);
			}
			else
			{
				strCreditProcess.AppendFormat(_T("-%.1f"), nPayout / 100.0);
			}
		}
		
		ActiveMember.SetCreditProcess(strCreditProcess);

		UpdateActiveMember(ActiveMember, TRUE);

		RefreshView();
	}
}


void CActiveMemberList::Clear()
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	m_ActiveMemberMap.RemoveAll();
}

INT_PTR CActiveMemberList::GetCount()
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	return m_ActiveMemberMap.GetCount();
}

void CActiveMemberList::StartEnumActiveMember()
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	m_CurPos = m_ActiveMemberMap.GetStartPosition();
}

BOOL CActiveMemberList::GetEachActiveMember(CActiveMember& ActiveMember)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	if (NULL == m_CurPos)
	{
		return FALSE;
	}

	UINT nMemberId = 0;

	m_ActiveMemberMap.GetNextAssoc(m_CurPos, nMemberId, ActiveMember);

	return TRUE;
}

BOOL CActiveMemberList::GetActiveMember(UINT nMemberId, CActiveMember& ActiveMember)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	return m_ActiveMemberMap.Lookup(nMemberId, ActiveMember);
}

BOOL CActiveMemberList::GetActiveMember(CString strTermId, CActiveMember& ActiveMember, BOOL bFullMatch)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	strTermId.MakeUpper();

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		// 2011/06/07-gxx: �޸�Ϊģ������Ŷ
		CString strTerm = ActiveMember.GetTerminalID();

		strTerm.MakeUpper();

		if (bFullMatch)
		{
			if ( strTerm == strTermId )
			{
				return TRUE;
			}
		}
		else
		{
			if (strTerm.Find(strTermId, 0) >= 0)
			{
				return TRUE;
			}
		}
		
	}

	return FALSE;
}
BOOL CActiveMemberList::GetActiveMemberBest(CString strTermId, CActiveMember& ActiveMember, BOOL bFullMatch/*=TRUE*/)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	strTermId.MakeUpper();

	vector<CActiveMember> vecRes;
	vector<CActiveMember> vecEqual;
	vector<CActiveMember> vecSame;

	COleDateTime dt;
	CActiveMember AMRes;

	int nFound = 0;

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember AM;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, AM);

		// 2011/06/07-gxx: �޸�Ϊģ������Ŷ
		CString strTerm = AM.GetTerminalID();

		strTerm.MakeUpper();

		BOOL bFound = FALSE;
		if (bFullMatch)
		{
			if ( strTerm == strTermId )
			{
				bFound = TRUE;		
			}
		}
		else
		{
			if (strTerm.Find(strTermId, 0) >= 0)
			{
				bFound = TRUE;
			}
		}
		if (bFound)
		{
			vecRes.push_back(AM);
			++nFound;
		}
	}

	if (nFound == 0)
	{
		return FALSE;
	}

	if (nFound == 1)
	{
		ActiveMember = vecRes[0];
		return TRUE;
	}
	
	// nFound > 1
	CString strTerm = vecRes[0].GetTerminalID();
	strTerm.MakeUpper();
	int nMin = 0;
	for(int i = 0; i < nFound; i++)
	{
		CString strTmp = vecRes[i].GetTerminalID();
		strTmp.MakeUpper();
		if (  strTmp == strTermId ) // ��������ն���ȫ��ͬ
		{
			vecEqual.push_back(vecRes[i]);
		}
		if (vecEqual.size() > 0)
		{
			continue;
		}
		if ( strTmp == strTerm ) // �ͽ����ĵ�һ���ն���ͬ
		{
			vecSame.push_back(vecRes[i]);
		}
		if (strTmp < strTerm)
		{
			nMin = i;
			strTerm = strTmp;
		}
	}

	if (vecEqual.size() == 1)
	{
		ActiveMember = vecEqual[0];
		return TRUE;
	}
	else if(vecEqual.size() > 1)
	{
		for(size_t i = 0; i < vecEqual.size(); i++)
			if (vecEqual[i].GetUpdateDataTime() > AMRes.GetUpdateDataTime())
			{
				AMRes = vecEqual[i];
			}

		ActiveMember = AMRes;
		return TRUE;
	}
	else if(vecSame.size() > 1)
	{
		// ���Ȳ������ߵ��û�
		vector<CActiveMember> vecOnline;
		for(size_t i = 0; i < vecSame.size(); i++)
		{
			if (vecSame[i].GetIsOnLine())
			{
				vecOnline.push_back(vecSame[i]);
			}
		}
		if (vecOnline.size() == 0)
		{
			for(size_t i = 0; i < vecSame.size(); i++)
				if (vecSame[i].GetUpdateDataTime() > AMRes.GetUpdateDataTime())
				{
					AMRes = vecSame[i];
				}

			ActiveMember = AMRes;
			return TRUE;
		}
		else if(vecOnline.size() == 1)
		{
			ActiveMember = vecOnline[0];
			return TRUE;
		}
		else
		{
			for(size_t i = 0; i < vecOnline.size(); i++)
				if (vecOnline[i].GetUpdateDataTime() > AMRes.GetUpdateDataTime())
				{
					AMRes = vecOnline[i];
				}

			ActiveMember = AMRes;
			return TRUE;
		}
	}
	else
	{
		ActiveMember = vecRes[nMin];
		return TRUE;
	}

	
}
BOOL CActiveMemberList::GetActiveMemberNetID(CString strNetId,CActiveMember& ActiveMember, BOOL bFullMatch)
{
	IBA_ASSERT(!strNetId.IsEmpty());

	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (bFullMatch)
		{
			if (0 == ActiveMember.GetNetId().CompareNoCase(strNetId))
			{
				return TRUE;
			}
		}
		else
		{
			CString strOld = ActiveMember.GetNetId();
			strOld.MakeLower();
			strNetId.MakeLower();
			if (strOld.Find(strNetId) >= 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CActiveMemberList::GetUpdateActiveMemberId()
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	UINT nTmp = 0;

	if (!m_UpdateMemberIdArray.IsEmpty())
	{
		nTmp = m_UpdateMemberIdArray.GetAt(0);
		m_UpdateMemberIdArray.RemoveAt(0);
	}

	return nTmp;
}

BOOL CActiveMemberList::RemoveActiveMember(UINT nMemberId)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);



	CActiveMember ActiveMember;
	GetActiveMember(nMemberId, ActiveMember);

	BOOL bRet = m_ActiveMemberMap.RemoveKey(nMemberId);

	if (bRet)
	{
		m_UpdateMemberIdArray.Add(nMemberId);
	}

	RemoveActiveMemberDB(nMemberId);


	// ��������Ҫ��----------------------------------------------------------
	CString strIniFile = theApp.GetWorkPath();
	strIniFile.Append(_T("\\IBAConfig\\nde_server.ini")); // base, Filename
	if (::PathFileExists(strIniFile))
	{
		TCHAR tRes[512] = {0};
		::GetPrivateProfileString(_T("base"), _T("Filename"), NULL, tRes, 512, strIniFile);
		if (::PathFileExists(tRes))
		{
			/*
			����û�:add|�û���|����

			ɾ���û�:del|�û���

			*/
			CString strParam;
			strParam.Format(_T("del|%s"), ActiveMember.GetNetId());

			IBA_LOG(_T("ִ��: %s %s"), tRes, strParam);

			ShellExecute(NULL, _T("open"), tRes, strParam, NULL, SW_SHOWNORMAL);
		}
		else
		{
			IBA_LOG(_T("ִ���ļ�:%s ������"), tRes);;
		}
	}
	else
	{
		IBA_LOG(_T("�����ļ�:%s ������"), strIniFile);
	}	

	//---------------------------------------------------------------------

	// ��ѯʵ�����߱�

	CString strSQL;

	strSQL.Format(_T("select * from auditinfo where MEMBERID=%d order by LogonTime desc"), nMemberId);

	IBA_TRACE(strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL)) 
	{
		bRet = TRUE;
	}
	else
	{
		IBA_LOG0(_T("SQL���ִ������, RemoveActiveMember"));
		pDb->Release();
		lock.Unlock();
		return bRet;
	}

	int nCount = Rs.GetRecordCount();
	if (nCount == 0)
	{
		Rs.Close();
		pDb->Release();
		lock.Unlock();
		return bRet;
	}
	CString strTermID, strIP;
	Rs.GetFieldValue(_T("MACHNAME"), strTermID);
	Rs.GetFieldValue(_T("IP"), strIP);
	Rs.Close();

	pDb->Release();

	if (strTermID.IsEmpty() || strIP.IsEmpty())
	{
		IBA_LOG0(_T("ʵ���ϻ���Ϣ���ն˺ź�IPΪ��"));
	}
	else
	{
		theApp.GetRealName()->AuditUserCheckOut(strTermID, strIP, nMemberId);
	}
	//CComputerInfo computer;
	//if (CLocalServer::GetInstance()->ComputerList.GetComputerInfo(ActiveMember.GetTerminalID(), computer))
	//{
	//	//CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
	//	theApp.GetRealName()->AuditUserCheckOut(
	//		computer.GetTerminalID(), computer.GetComputerIP(),nMemberId);
	//}
	//}

	// 2014-1-13-qsc �����ʱ����
	INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);

	if (nIndex >= 0) //Զ���»�
	{
		CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
		computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
		computer.ClearUserInfo();
		CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);

		//CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
	}


	lock.Unlock();



	return bRet;
}

void CActiveMemberList::Copy(ActiveMemberMap& OtherActiveMemberMap)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;

		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		OtherActiveMemberMap.SetAt(ActiveMember.GetMemberID(), ActiveMember);
	}
}

void CActiveMemberList::Copy(CArray<CActiveMember>& arrMembers)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;

		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		arrMembers.Add(ActiveMember);
	}
}
//******************************************************************************

INT CActiveMemberList::Run()
{
	CoInitialize(NULL);

	while (TRUE)
	{

#ifdef _DEBUG
	Sleep(1000 * 5);
#else
	Sleep(1000 * 30);
#endif // _DEBUG

		try
		{
			if (theApp.GetCurCashier()->IsOnDuty())
			{
				CheckAllActiveMember();
			}
				
			RefreshView();
		}
		catch (...)
		{
			CIBALog::GetInstance()->Write(_T("CActiveMemberList::Run Error!"));
		}
	}

	CoUninitialize();

	CIBALog::GetInstance()->Write(_T("CActiveMemberList::Run Exit!"));

	return 0;
}

void CActiveMemberList::CheckAllActiveMember()
{
	ActiveMemberMap TempActiveMemberMap;

	Copy(TempActiveMemberMap);

	CActiveMember ActiveMember;

	POSITION pos = TempActiveMemberMap.GetStartPosition();

	while (pos != NULL)
	{
		UINT nMemberId = 0, nCardId = 0;

		CActiveMember ActiveMember;

		TempActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		// 2011/07/11-8201-gxx: 
		if (ActiveMember.GetIsLocalUser())
		{
			CheckLocalUser(ActiveMember);
		}
		else
		{
			// 2011/08/10-8201-gxx: ��Ӷ����ж�
			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
			{
				CheckBrokenCenterUser(ActiveMember);
			}
			else if (ActiveMember.GetMemberID() == ActiveMember.GetRoomMemberId())
			{
				CheckRoomUser(ActiveMember);
			}
			else
			{
				if (ActiveMember.IsNeedToQuery()) //��Ҫ�����Ĳ�ѯ
				{
					CIDCheckCardMethod checkCard;

					checkCard.SetSerialNum(ActiveMember.GetNetId());

					theApp.GetCurCashier()->DoCheckCard(checkCard);

					if (checkCard.GetStatusCode() == 0)
					{
						nCardId = checkCard.GetCardId();
						nMemberId = checkCard.GetMemberId();
					}

					if (nMemberId > 0)
					{
						CheckActiveMember(nCardId, nMemberId, ActiveMember);
					}
					else
					{
						if (checkCard.GetStatusCode() != - 1)
						{
							RemoveActiveMember(ActiveMember.GetMemberID());
							CIBALog::GetInstance()->Write(_T("-------------------����û�------------------"));
							// 2014-1-13-qsc 
							CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
						}
					}

					Sleep(50);
				}			
			}			
		}	
	}
}

void CActiveMemberList::CheckRoomUser(CActiveMember& ActiveMember)
{
	if (ActiveMember.GetAvailavleBalance() > 0)
	{
		if (ActiveMember.IsNeedToQuery())
		{
			CIDCheckCardMethod checkCard;

			checkCard.SetSerialNum(ActiveMember.GetNetId());

			theApp.GetCurCashier()->DoCheckCard(checkCard);

			UINT nCardId = 0;
			UINT nMemberId = 0;
			if (checkCard.GetStatusCode() == 0)
			{
				nCardId = checkCard.GetCardId();
				nMemberId = checkCard.GetMemberId();
			}

			if (nMemberId > 0)
			{
				CheckActiveMember(nCardId, nMemberId, ActiveMember);
			}
			else
			{
				if (checkCard.GetStatusCode() != - 1)
				{
					RemoveActiveMember(ActiveMember.GetMemberID());
					CIBALog::GetInstance()->Write(_T("-------------------����û�------------------"));
				}
			}

			Sleep(50);
		}
		return;
	}

	BOOL bCanReturn = FALSE;

	// ���ز����ߣ�����������
	if (ActiveMember.GetStatus() == CActiveMember::EStatus_CenterOnLine || 
		(ActiveMember.GetStatus() == CActiveMember::EStatus_LocalOnline && ActiveMember.IsOffLinePossibility()))
	{
		BOOL bCanAuto = FALSE;
		if (ActiveMember.GetUpdateDataTime().GetStatus() != COleDateTime::valid)
		{
			bCanAuto = TRUE;
		}
		else
		{
			COleDateTimeSpan dtSpan = COleDateTime::GetCurrentTime() - ActiveMember.GetUpdateDataTime(); 

			if (dtSpan.GetTotalMinutes() >= 2.0) // 2����
			{
				bCanAuto = TRUE;
			}
		}

		if (bCanAuto)
		{
			ActiveMember.SetStatus(CActiveMember::EStatus_RoomAutoReturn); // ״̬��Ϊ�����Զ��˿�״̬
		}
		else
		{
			return;
		}
		

		CIDCheckCardMethod checkCard;
		checkCard.SetSerialNum(ActiveMember.GetNetId());
		theApp.GetCurCashier()->DoCheckCard(checkCard);
		UINT nCardId = 0;
		UINT nMemberId = 0;
		if (checkCard.GetStatusCode() == 0)
		{
			nCardId = checkCard.GetCardId();
			nMemberId = checkCard.GetMemberId();
		}
		if (nMemberId >0 )
		{
			CBalanceMethod Balance;
			Balance.SetCardId(nCardId);
			Balance.SetMemberId(nMemberId);

			theApp.GetCurCashier()->DoBalance(Balance);
			if (Balance.GetStatusCode()!=0)
			{
				return;
			}

			if (Balance.GetOnline() == 1)
			{
				// �Ѹ���ʱ���Ϊ�ɵǳ���ʱ�䣬��һ�ΰ������Զ��˿�ʱ�ж�
				ActiveMember.UpdateDataTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetLogoutTime());

				CString strTmp = CIBAHelpper::FormatTime(ActiveMember.UpdateDataTime);
				UpdateActiveMember(ActiveMember, TRUE);

				IBA_LOG(_T("���ķ��ذ����Զ��˿������ʱ��(MemberID=%d):%s"), ActiveMember.GetMemberID(), strTmp);
			}
			else
			{
				bCanReturn = TRUE;
			}
			
		}
		else
		{
			bCanReturn = FALSE;
		}
	}

	if (!bCanReturn && ActiveMember.GetStatus() == CActiveMember::EStatus_RoomAutoReturn)
	{
		CString strTmp1,strTmp2;
		strTmp1 = CIBAHelpper::FormatTime(COleDateTime::GetCurrentTime());
		strTmp2 = CIBAHelpper::FormatTime(ActiveMember.GetUpdateDataTime());
		if (COleDateTime::GetCurrentTime() >= ActiveMember.GetUpdateDataTime())
		{
			bCanReturn = TRUE;
		}
	}
		
	if (bCanReturn)
	{
		// ִ�а�������
		CRoomCheckOut roomCheckOut;
		roomCheckOut.SetMemberId(ActiveMember.GetMemberID());
		roomCheckOut.SetPCClass(ActiveMember.GetPCClassID());

		theApp.GetCurCashier()->DoRoomCheckOut(roomCheckOut);

		if (roomCheckOut.GetStatusCode() == 0)
		{
			BOOL bRead = GetActiveMember(ActiveMember.GetMemberID(),ActiveMember);
			CArray<UINT> arrMembers;
			GetRoomSMembers(ActiveMember.GetMemberID(), arrMembers);

			// ����������˵Ĳ�����־
			{
				COperationInfo OperationInfo;
				OperationInfo.SetOperationType(CIBAGlobal::emOpTypeRoomReturn);
				OperationInfo.SetIsBundTime(FALSE);

				if (bRead)
				{
					OperationInfo.SetUserName(ActiveMember.GetUserName());
					OperationInfo.SetNetId(ActiveMember.GetNetId());
					OperationInfo.SetMemberId(ActiveMember.GetMemberID());
					OperationInfo.SetTermId(ActiveMember.GetTerminalID());
					OperationInfo.SetIdNum(ActiveMember.GetNetId());
				}

				CString strTmp;
				strTmp += ActiveMember.GetPCClass();
				strTmp += _T(": ");
				strTmp.AppendFormat(LOAD_STRING(IDS_ROOM_BODY), arrMembers.GetCount());
				OperationInfo.SetDescription(strTmp);

				OperationInfo.SetCheckInTime(ActiveMember.GetActivationTime()); // �����Ŀ�ͨʱ��

				OperationInfo.SetClassId(ActiveMember.GetClassId());

				::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
				
				IBA_LOG(_T("�����Զ��˿�ɹ�!MemberID=%d"), ActiveMember.GetMemberID());
			}

			
		}
		
		ClearRoomMembers(ActiveMember.GetRoomMemberId());
	}
}

void CActiveMemberList::CheckActiveMember(UINT nCardId, UINT nMemberId, CActiveMember &ActiveMember)
{
	CBalanceMethod Balance;
	Balance.SetCardId(nCardId);
	Balance.SetMemberId(nMemberId);

	theApp.GetCurCashier()->DoBalance(Balance);

	if (Balance.GetStatusCode() == 1) //��Ч�û�
	{
		RemoveActiveMember(nMemberId);
		return;
	}	

	if (Balance.GetStatusCode() != 0) return; //ʧ�����˳�
	
	
	//�û����߸���UpdateDataTimeΪ����ʹ�õ���ʱ��
	//�û������ߣ�����UpdateDataTimeΪ�����»�ʱ�䣬�Զ��˿��ж�ʹ��
	ActiveMember.UpdateDataTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetLogoutTime());


	if (Balance.GetOnline() == 1) //����
	{
		ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine); //��Ϊ�������ߣ����ز�����״̬
	}
	else //������
	{
		if (Balance.IsNetbarMember()) // ��Ա
		{
			RemoveActiveMember(nMemberId);
			return;
		}
		else
		{
			if (Balance.GetPayType() > 0) //�ۼƷⶥ//���Ԥ��
			{
				//{ 2011/09/22-8201-gxx: �ۼƷⶥ������
				if (Balance.GetPayType() == CIBAGlobal::emPayTypeAccumulative 
					&& Balance.GetTotalBalacne() == 0)
				{
					CDeregisterHelpper::DeregisterUser(Balance);
					CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
					return;
				}
				//}
				
				
				ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);

				//�����´β�ѯʱ��Ϊ��ʱ����ʱ��
				ActiveMember.UpdateDataTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime());
			}
			else 
			{	
				//����Ǯ������Ѻ��
				if (Balance.GetTotalBalacne() > 0 || Balance.GetGuarantyMoney() > 0)
				{
					ActiveMember.SetStatus(CActiveMember::EStatus_Waiting); //��Ϊ���ϻ�״̬
					ActiveMember.SetActive(FALSE);
					ActiveMember.SetTerminalID(_T(""));
				}
				else
				{
					if (ActiveMember.GetStatus() == CActiveMember::EStatus_AutoReturn 
						|| CDeregisterHelpper::AllowAutoRefund(Balance)) //���˿�״̬���������Զ��˿�
					{
						CDeregisterHelpper::DeregisterUser(Balance);
						CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
						return;
					}

					ActiveMember.SetStatus(CActiveMember::EStatus_AutoReturn); //��Ϊ���Զ��˿�״̬
				}
			}
		}
	}

	//�����û���Ϣ

	INT nAllMoney = Balance.GetTotalBalacne();
	
	LONG nAvailavleBalance = ActiveMember.GetAvailavleBalance();

	INT nTmp = ActiveMember.GetAmount() + nAvailavleBalance - nAllMoney;

	ActiveMember.SetAmount(nTmp > 0 ? nTmp : 0);

	ActiveMember.SetAvailavleBalance(nAllMoney);

	UpdateActiveMember(ActiveMember, TRUE);
}

BOOL CActiveMemberList::AddActiveMemberDB(CActiveMember &ActiveMember)
{
	CString strSQL;

	//��ɾ��ͬһ���˺ŵļ�¼����������ǰ�˿�û�гɹ�ɾ������
	strSQL.Format(_T("delete from activemember where netId = '%s'"), ActiveMember.GetNetId());
	CIBADAL::GetInstance()->ExecuteSQL(strSQL);

	// 2011/03/29-gxx: �������̨ID-ʹ��lastNetBarId�ֶ�
	// 2011/05/19-gxx: ����Ƿ񿪻���ʱ�ֶ�amount, registerBT
	// 2011/07/08-8201-gxx: ����dynamicMaxAmount, isLocal, password,(����ģʽ����)
	// 2011/07/12-8201-gxx: ����netIdAlias,�˺ű���
	// 2011/10/25-8230-gxx: ���Ӱ����������û���
	strSQL = _T("insert into activemember(memberId, netId, lastRemainAmount, advancePayment, ");
	strSQL.Append(_T("creditProcess, name, sex, idType, idNumber, classId, activationTime, status,"));
	strSQL.Append(_T("lastNetBarId, netBarId, amount, registerBT, "));
	strSQL.Append(_T("dynamicMaxAmount, isLocal, password, netIdAlias, "));
	strSQL.Append(_T("roomMemberId, "));
	strSQL.Append(_T("scanType "));
	strSQL.Append(_T(") values("));

	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetMemberID());
	strSQL.AppendFormat(_T("'%s',"), ActiveMember.GetNetId());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetAvailavleBalance());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetAdvancePayment());

	strSQL.AppendFormat(_T("'%s',"), ActiveMember.GetCreditProcess());
	strSQL.AppendFormat(_T("'%s',"), ActiveMember.GetUserName());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetSex());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetIdType());

	strSQL.AppendFormat(_T("'%s',"), ActiveMember.GetPersonalID());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetClassId());
	strSQL.AppendFormat(_T("'%s',"), ActiveMember.ActivationTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetStatus()); //��ʼΪ���ϻ�
	
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetCasherID());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetNetbarID());
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetAmount());
	strSQL.AppendFormat(_T("%d,"),  ActiveMember.GetIsRegisterBT());

	// 2011/07/08-8201-gxx: 
	strSQL.AppendFormat(_T("%d,"),  ActiveMember.GetDynamicMaxAmount());
	strSQL.AppendFormat(_T("%d,"),  ActiveMember.GetIsLocalUser());
	
	strSQL.AppendFormat(_T("'%s',"),  ActiveMember.GetPassword());

	// 2011/07/12-8201-gxx: 
	strSQL.AppendFormat(_T("'%s',"),  ActiveMember.GetNetIdAlias());

	// 2011/10/25-8230-gxx: 
	strSQL.AppendFormat(_T("'%s',"), ActiveMember.GetRoomMemberId());

	strSQL.AppendFormat(_T("%d"), ActiveMember.GetScanType());

	strSQL.Append(_T(")"));

	TRACE(strSQL);

	return CIBADAL::GetInstance()->ExecuteSQL(strSQL);
}

BOOL CActiveMemberList::UpdateActiveMemberDB(CActiveMember &ActiveMember)
{
	CString strSQL;

	strSQL.AppendFormat(_T("update activemember set lastRemainAmount=%d"), ActiveMember.GetAvailavleBalance());

	strSQL.AppendFormat(_T(",amount=%d"), ActiveMember.GetAmount());

	strSQL.AppendFormat(_T(",termId='%s'"), ActiveMember.GetTerminalID());

	strSQL.AppendFormat(_T(",pcClass=%d"), ActiveMember.GetPCClassID());

	strSQL.AppendFormat(_T(",updatetime='%s'"), CIBAHelpper::FormatTime(ActiveMember.UpdateDataTime));

	strSQL.AppendFormat(_T(",isActive=%d"), ActiveMember.IsActive());

	strSQL.AppendFormat(_T(",status=%d"), ActiveMember.GetStatus());

	strSQL.AppendFormat(_T(",activationTime='%s'"), CIBAHelpper::FormatTime(ActiveMember.ActivationTime));

	strSQL.AppendFormat(_T(",checkinTime='%s'"), CIBAHelpper::FormatTime(ActiveMember.CheckInTime));

	strSQL.AppendFormat(_T(",advancePayment=%d,refNo=%d"), ActiveMember.GetAdvancePayment(),2);

	CIBALog::GetInstance()->WriteFormat(_T(",advancePayment=%d,refNo=%d"), ActiveMember.GetAdvancePayment(),2);

	strSQL.AppendFormat(_T(",creditProcess='%s'"), ActiveMember.GetCreditProcess());

	// 2011/04/25-gxx: ���������˺�,�޸Ŀ��ź�Ҫˢ���˺�
	strSQL.AppendFormat(_T(",netId='%s'"), ActiveMember.GetNetId());

	// 2011/05/13-gxx: 
	strSQL.AppendFormat(_T(",remark='%s'"),ActiveMember.GetRemark());

	// 2011/07/11-8201-gxx: 
	strSQL.AppendFormat(_T(", dynamicMaxAmount=%d "),ActiveMember.GetDynamicMaxAmount());
	
	strSQL.AppendFormat(_T(", Password='%s'"),ActiveMember.GetPassword());

	// 2011/10/25-8230-gxx: 
	strSQL.AppendFormat(_T(", roomMemberId=%d"), ActiveMember.GetRoomMemberId());

	strSQL.AppendFormat(_T(" where memberid=%d"), ActiveMember.GetMemberID());

	return CIBADAL::GetInstance()->ExecuteSQL(strSQL);
}

BOOL CActiveMemberList::RemoveActiveMemberDB(UINT nMemberId)
{
	CString strSQL;

	strSQL.Format(_T("delete from activemember where memberId = %d or memberId = 0"), nMemberId);

	return CIBADAL::GetInstance()->ExecuteSQL(strSQL);
}


BOOL CActiveMemberList::ClearAllMembers()
{
	CString strSQL;

	strSQL = _T("delete from activemember");

	BOOL bRes = CIBADAL::GetInstance()->ExecuteSQL(strSQL);

	RefreshView();

	m_ActiveMemberMap.RemoveAll();

	CLocalServer::GetInstance()->GetActiveMemberView()->
		SendMessage(WM_COMMAND,IDM_MYREFRESH,0);

	return bRes;
}


INT CActiveMemberList::GetDynamicReserve(UINT nCasherID)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	int nSum = 0;

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (ActiveMember.IsMember() || ActiveMember.GetIsLocalUser()) continue;

		const UINT nID = ActiveMember.GetCasherID();

		if (nID == nCasherID || (nCasherID == 1 && nID == 0)) // 1��ʾ������̨��Ϊ0������Ϊ������̨��
		{
			nSum += ActiveMember.GetAdvancePayment();
		}
	}

	nSum = nSum / 100 * 100; //ȡ����Ԫ

	return nSum;
}

void CActiveMemberList::AddReLoginRequest(TReLoginRequest& btt)
{
	BOOL bFo = FALSE;

	// �ж��Ƿ����ظ���
	for (int i = 0; i < m_arrReLoginRequest.GetCount(); i++ )
	{
		if (m_arrReLoginRequest.GetAt(i).memberId == btt.memberId)
		{
			bFo = TRUE;
			break;
		}
	}

	if (!bFo)
	{
		m_arrReLoginRequest.Add(btt);
	}
}

void CActiveMemberList::TryReLogin()
{
	if (m_arrReLoginRequest.GetCount() == 0)
	{
		return;
	}

	for (INT i = 0; i < m_arrReLoginRequest.GetCount(); )
	{
		TReLoginRequest* pReLoginRequest = &(m_arrReLoginRequest.GetAt(i));

		CActiveMember ActiveMember;

		if (!GetActiveMember(pReLoginRequest->memberId, ActiveMember)) //�û�������
		{
			m_arrReLoginRequest.RemoveAt(i);
			continue;
		}

		if (TReLoginRequest::TRY_MAX <= pReLoginRequest->tryCount) //һ���Ӻ�ʧ��
		{
			m_arrReLoginRequest.RemoveAt(i);
			continue;
		}

		if (ActiveMember.GetRefNo() != pReLoginRequest->lastRefNo) //�ɹ���
		{
			IBA_TRACE("��ʱ�ɹ�!");
			m_arrReLoginRequest.RemoveAt(i);
			continue;
		}

		if (GetTickCount() - pReLoginRequest->firstTryTime > 8 * 1000) //����8S
		{
			CLocalServer::GetInstance()->LocalBundleTime(pReLoginRequest->memberId);

			pReLoginRequest->tryCount++;
		}

		i++; //��һ��
	}
}

BOOL CActiveMemberList::IsAllowCredit(UINT nMemberID)
{
	CComputerInfo info;
	if ( CLocalServer::GetInstance()->ComputerList.GetComputerInfo(nMemberID, info) )
	{
		if (info.GetComputerStatus() == CComputerInfo::ECS_SUSPEND)
		{
			theApp.IBAMsgBox(_T("���û����ڹһ��У��������ֵ"));
			return FALSE;
		}	
	}
	return TRUE;
}

int CActiveMemberList::GetUserStateByNetId(CString strNetId)
{
	CActiveMember ActiveMember;
	if (!GetActiveMemberNetID(strNetId, ActiveMember, FALSE))
	{
		return CIBAGlobal::emUserStateNULL;
	}
	if (ActiveMember.GetIsLocalUser())
	{
		return CIBAGlobal::emUserStateLocal;
	}
	return CIBAGlobal::emUserStateCenter;
}

BOOL CActiveMemberList::HasLocalUser()
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	while (pos != NULL)
	{
		UINT nMemberId = 0;
		CActiveMember ActiveMember;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (ActiveMember.GetIsLocalUser())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CActiveMemberList::CheckLocalUser(CActiveMember& ActiveMember)
{
	IBA_TRACE("��鱾���û�...");
	
	if (!ActiveMember.GetIsLocalUser())
	{
		IBA_ASSERT2(0,"����Ϊ�����û�");
		return;
	}

	if (ActiveMember.GetAvailavleBalance() == 0)
	{
		if (ActiveMember.GetPolicyInfo().IsEmpty())
		{
			CString strSQL;

			strSQL.Format(_T("select PolicyInfo from ActiveMember where memberId=%d"),
				(INT)ActiveMember.GetMemberID());

			IBA_TRACE(strSQL);

			CADODBConnInfo* pDb = NULL;

			if (CIBADAL::GetInstance()->GetDBConnInfo(pDb))
			{

				CADORecordset Rs(pDb->GetConn());

				if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0) 
				{
					Rs.GetFieldValue(_T("PolicyInfo"), ActiveMember.SetPolicyInfo());
					Rs.Close();
					pDb->Release();
				}
				else
				{
					pDb->Release();
				}
			}
		}

		UINT nLocalExpense = ActiveMember.GetLocalExpense(FALSE);
		nLocalExpense += 5;
		if (nLocalExpense >= (UINT)ActiveMember.GetAdvancePayment())
		{
			CCashierHelper::ReturnLocalUser(ActiveMember);
		}
	}
}

void CActiveMemberList::CheckBrokenCenterUser(CActiveMember& ActiveMember)
{
	if (ActiveMember.GetIsLocalUser())
	{
		IBA_ASSERT2(0,"����Ϊ�����û�");
		return;
	}

	if (ActiveMember.GetPolicyInfo().IsEmpty() || ActiveMember.GetNextCheckinTime()==0)
	{
		CString strSQL;

		strSQL.Format(_T("select PolicyInfo,NextCheckInTime from ActiveMember where memberId=%d"),
			(INT)ActiveMember.GetMemberID());

		IBA_TRACE("CActiveMemberList::CheckBrokenCenterUser");
		IBA_TRACE(strSQL);

		CADODBConnInfo* pDb = NULL;

		if (CIBADAL::GetInstance()->GetDBConnInfo(pDb))
		{

			CADORecordset Rs(pDb->GetConn());

			if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0) 
			{
				Rs.GetFieldValue(_T("PolicyInfo"), ActiveMember.SetPolicyInfo());
				Rs.GetFieldValue(_T("NextCheckInTime"), ActiveMember.SetNextCheckinTime());
				Rs.Close();
				pDb->Release();
			}
			else
			{
				IBA_LOG0(_T("CheckBrokenCenterUser�� �����ݿ��ȡPolicyInfo,NextCheckInTimeʧ��"));
				pDb->Release();
			}
		}
	}

	if (ActiveMember.GetAvailavleBalance() == 0)
	{
		if (ActiveMember.GetNextCheckinTime() == 0)
		{
			// �����ڰ�ʱ״̬, ���Ǯ���꣬��ô�ͽ���

			UINT nLocalExpense = ActiveMember.GetLocalExpense(FALSE);
			nLocalExpense += 5;
			if (nLocalExpense >= (UINT)ActiveMember.GetAdvancePayment())
			{
				CCashierHelper::ReturnLocalCenterUser(ActiveMember);
			}
		}
		else
		{
			// ���ڰ�ʱ״̬�������ʱ����ʱ���ѵ�����ô�ͽ���
			COleDateTime odt((__int64)ActiveMember.GetNextCheckinTime());

			COleDateTime curTime = COleDateTime::GetCurrentTime();

			// �����ǰʱ����ڰ�ʱ����ʱ�䣬��Ϊ���Խ�����.
			if (curTime >= odt)
			{
				CCashierHelper::ReturnLocalCenterUser(ActiveMember);
			}

		}
	}
	else
	{
		if (ActiveMember.IsMember())
		{
			// �����Ա����δ�ϻ�״̬����ô���Խ���
			if (ActiveMember.GetTerminalID().IsEmpty())
			{
				CCashierHelper::ReturnLocalCenterUser(ActiveMember);
			}
			else
			{
				// ��Ա�����ϻ�״̬������������
				CComputerInfo ComInfo;
				if (CLocalServer::GetInstance()->ComputerList.GetComputerInfo(ActiveMember.GetMemberID(),ComInfo))
				{
					if (ComInfo.GetComputerStatus() != CComputerInfo::ECS_ONLINE)
					{
						COleDateTime dtNow = COleDateTime::GetCurrentTime();
						COleDateTimeSpan span = dtNow - ActiveMember.GetUpdateDataTime();
						IBA_TRACE("��Ա���ߣ�ʱ�� ��=%.2f",span.GetTotalMinutes());
						if (span.GetTotalMinutes() >= 60.0)
						{
							CCheckOutinfo chInfo;
							CCashierHelper::MakeCheckOutInfo(ActiveMember,chInfo);

							chInfo.SetCheckOutTime() = CIBAHelpper::FormatTime(ActiveMember.GetUpdateDataTime());
							CCashierHelper::ReturnLocalCenterUser(ActiveMember, &chInfo);
						}
					}
				}
			}	
		}
		else
		{
			if (!ActiveMember.GetTerminalID().IsEmpty())
			{
				CComputerInfo ComInfo;
				if (CLocalServer::GetInstance()->ComputerList.GetComputerInfo(ActiveMember.GetMemberID(),ComInfo))
				{
					// ��ʱ�û��ϻ�������������
					if (ComInfo.GetComputerStatus() != CComputerInfo::ECS_ONLINE)
					{
						INT nCost = ActiveMember.GetLocalExpense();
						if (nCost >= ActiveMember.GetAdvancePayment())
						{
							ActiveMember.SetAvailavleBalance(0);
							ActiveMember.SetAmount(ActiveMember.GetAdvancePayment());
							CCashierHelper::ReturnLocalCenterUser(ActiveMember);	
						}
					}
				}
			}
		}
	}
}


INT CActiveMemberList::GetLocalTotalMoney(UINT nCasherID)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	POSITION pos = m_ActiveMemberMap.GetStartPosition();

	int nSum = 0;

	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (!ActiveMember.GetIsLocalUser()) 
			continue;

		const UINT nID = ActiveMember.GetCasherID();

		if (nID == nCasherID || (nCasherID == 1 && nID == 0)) // 1��ʾ������̨��Ϊ0������Ϊ������̨��
		{
			nSum += ActiveMember.GetAdvancePayment();
		}
	}

	nSum = nSum / 100 * 100; //ȡ����Ԫ

	return nSum;
}

BOOL CActiveMemberList::FillRoomInfo(CRoomInfo& info)
{
	if (info.GetRoomID() == 0)
	{
		ASSERT(info.GetRoomID() > 0);
		return FALSE;
	}

	UINT nPCClass = info.GetRoomID();
	UINT nRoomMemberID = info.GetMemberID();
	

	POSITION pos = m_ActiveMemberMap.GetStartPosition();
	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (ActiveMember.GetPCClassID() == nPCClass)
		{
			info.SetOnlineCount() += (ActiveMember.GetTerminalID().IsEmpty()?0:1);
		}
		
		if (nRoomMemberID == nMemberId)
		{
			info.SetNetId(ActiveMember.GetNetId());
		}
	}	

	return TRUE;
}

BOOL CActiveMemberList::IsRoomArea(UINT pcClass, UINT& nRoomMemberID)
{
	nRoomMemberID = 0;
	POSITION pos = m_ActiveMemberMap.GetStartPosition();
	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (ActiveMember.GetPCClassID() == pcClass && ActiveMember.GetRoomMemberId() > 0)
		{
			nRoomMemberID = ActiveMember.GetRoomMemberId();
			return TRUE;
		}
	}	
	return FALSE;
}

BOOL CActiveMemberList::GetRoomSMembers(UINT nRoomMemberID,CArray<UINT>& arrMembers)
{
	if (nRoomMemberID == 0)
	{
		ASSERT(nRoomMemberID>0);
		return FALSE;
	}
	

	POSITION pos = m_ActiveMemberMap.GetStartPosition();
	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember ActiveMember;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);

		if (ActiveMember.GetRoomMemberId() == nRoomMemberID)
		{
			arrMembers.Add(ActiveMember.GetMemberID());
		}
	}	
	return TRUE;
}

void CActiveMemberList::ClearRoomMembers(UINT nRoomMemberID)
{
	CArray<UINT> arrMembers;
	if (!GetRoomSMembers(nRoomMemberID, arrMembers))
	{
		IBA_LOG0(_T("�������û����ж�ȡ�����ڵ������û���Ϣʧ��"));
		//return FALSE;
	}

	IBA_LOG(_T("֪ͨ�����ڵ����е�UDO�»�,������MemberID=%d"), nRoomMemberID);

	for(int i=0; i < arrMembers.GetCount(); i++)
	{
		UINT nMemberId = arrMembers.GetAt(i);
		CActiveMember ActiveMember;
		GetActiveMember(nMemberId,ActiveMember);

		BOOL bClear = TRUE;
		if (ActiveMember.IsMember())
		{
			if (ActiveMember.GetPayType() > 0)
			{
				bClear = FALSE;
			}
		}
		else
		{
			// ���˺�����а�ʱ״̬������������ô���������
			if (ActiveMember.GetPayType() > 0 || ActiveMember.GetAvailavleBalance() > 0)
			{
				bClear = FALSE;
			}
		}
		INT_PTR nIn = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);

		IBA_LOG0(_T("�������˺Ž��ˣ����˺Ÿ��ű��ǳ������"));
		if ( nIn!=-1 )
		{
			// ���͸�ָ���ͻ���, �ǳ�����������
			CLocalServer::GetInstance()->LocalCheckOutUser(nIn, nMemberId, 2);

			IBA_LOG(_T("�����Զ����ˣ�֪ͨUDO�»��� TermID=%s MemberID=%d"), ActiveMember.GetTerminalID(), ActiveMember.GetMemberID());
		}

		if (bClear)
		{	
			RemoveActiveMember(nMemberId);

			if (!ActiveMember.IsMember())
			{
				CDeregisterMethod Deregister;

				Deregister.SetMemberId(nMemberId);
				Deregister.SetPersonalId(ActiveMember.GetPersonalID());
				Deregister.SetMemberName(ActiveMember.GetUserName());
				Deregister.SetCardId(0);
				Deregister.SetPassword(_T(""));
				Deregister.SetEndUsingTime(_T(""));

				theApp.GetCurCashier()->DoDeregister(Deregister);

				if (Deregister.GetStatusCode() == 0)
				{	

				}
				else
				{
					IBA_LOG(_T("��������ʱ���������nRoomMemberID=%d����û�MemberID=%dʧ��"),nRoomMemberID, nMemberId);
				}
			}
		}
		else
		{
			/*ActiveMember.SetRoomMemberId(0);
			ActiveMember.SetRemark(LOAD_STRING(IDS_ROOM_HAS_REFUND));
			UpdateActiveMember(ActiveMember,TRUE);*/
		}
	}

	CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);
}
