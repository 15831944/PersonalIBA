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
#include "LocalCenter\G2XCommonTool.h"
#include "CreditDlg.h"
#include "NDCreditDlg.h"
#include "..\Lib\ScanRecoInc.h"
#include "LocalMember.h"
using namespace NS_DAL;

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
		//if (ActiveMember.GetIsLocalUser())
		//{
		//	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
		//	{
		//		// ת����Ϊ�ָ�״̬
		//		CBrokenNetwork::SettingNetworkStatus(CIBAGlobal::emNetworkRestore);		
		//	}

		//	if (Rs.GetFieldValue(_T("classId"), nTmp))
		//	{
		//		ActiveMember.SetClassId(nTmp);
		//	}
		//}

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

	ActiveMember.SetIsLocalUser(TRUE);  //ֻ�б���ע��Ĳ���Ϊ�����û�
	
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
	
	//IBA_LOG(_T("Temp.CActiveMemberList:AddActiveMember: After RemoveActiveMember and LocalAuditUserQuit, Start to UpdateActiveMember(ActiveMember)"));
	UpdateActiveMember(ActiveMember);

	//IBA_LOG(_T("Temp.CActiveMemberList:AddActiveMember: After RemoveActiveMember and LocalAuditUserQuit, Start to AddActiveMemberDB(ActiveMember)"));
	AddActiveMemberDB(ActiveMember);

	//IBA_LOG(_T("Temp.CActiveMemberList:AddActiveMember: After RemoveActiveMember and LocalAuditUserQuit, Start to AddNewRow"));
	CLocalServer::GetInstance()->GetActiveMemberView()->GetActiveMemberGrid()->AddNewRow(ActiveMember, TRUE);

}

//����ʱ����RemoveActiveMemberPostActivation
void CActiveMemberList::AddActiveMemberPostActivation(CBalanceMethod & Balance, BOOL bRegisterBT, // �Ƿ񿪻���ʱ
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
		RemoveActiveMemberDBPostActivation(tmAct.GetMemberID());
	}
	else
	{
		RemoveActiveMemberDBPostActivation(ActiveMember.GetMemberID());
	}

	//IBA_LOG(_T("Temp.CActiveMemberList:AddActiveMember: After RemoveActiveMember and LocalAuditUserQuit, Start to UpdateActiveMember(ActiveMember)"));
	UpdateActiveMember(ActiveMember);

	//IBA_LOG(_T("Temp.CActiveMemberList:AddActiveMember: After RemoveActiveMember and LocalAuditUserQuit, Start to AddActiveMemberDB(ActiveMember)"));
	AddActiveMemberDB(ActiveMember);

	//IBA_LOG(_T("Temp.CActiveMemberList:AddActiveMember: After RemoveActiveMember and LocalAuditUserQuit, Start to AddNewRow"));
	CLocalServer::GetInstance()->GetActiveMemberView()->GetActiveMemberGrid()->AddNewRow(ActiveMember, TRUE);

}

void CActiveMemberList::AddActiveMember(CActiveMember& ActiveMember)
{
	UpdateActiveMember(ActiveMember);
	AddActiveMemberDB(ActiveMember);
	CLocalServer::GetInstance()->GetActiveMemberView()->GetActiveMemberGrid()->AddNewRow(ActiveMember, TRUE);
}



void CActiveMemberList::UpdateActiveMember(CActiveMember& ActiveMember, BOOL bUpdateDb /*= FALSE*/)
{
	//IBA_LOG(_T("Temp.CActiveMemberList::UpdateActiveMember - Start"));
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
	//IBA_LOG(_T("Temp.CActiveMemberList::UpdateActiveMember - End"));
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

		ActiveMember.SetAvailavleBalance(ActiveMember.GetAvailavleBalance() + nCreditMoney);
		
		ActiveMember.SetCreditProcess(strCreditProcess);

		UpdateActiveMember(ActiveMember, TRUE);

		RefreshView();
	}
}

void CActiveMemberList::UpdateActiveMember( UINT nMemberId, UINT nNewBlance )
{
	CActiveMember ActiveMember;

	if (GetActiveMember(nMemberId, ActiveMember)) 
	{
		ActiveMember.SetAvailavleBalance(nNewBlance);

		UpdateActiveMember(ActiveMember, TRUE);
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

//ʹ��MemberId�����û�
BOOL CActiveMemberList::GetActiveMember(UINT nMemberId, CActiveMember& ActiveMember)
{
	CSingleLock lock(&ActiveMemberListLock, TRUE);

	return m_ActiveMemberMap.Lookup(nMemberId, ActiveMember);
}

//ʹ��TermId�����û�,��ʹ�ò���ȫƥ��,i.e.Ŀ���û���TermId�а�������strTermId����.
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

/*************************************************
Function:       // GetActiveMemberBest
Description:    // ��ActiveMemberMap�и���strTermId�������ƥ���Ԫ�ء�
				// ƥ�����ȼ����ִ���ȫһ����UpdateDataTime��� > �ִ���ȫһ�� > �û�������UpdateDataTime��� > UpdateDataTime��� 
				// > �ִ����
				// �����ڽ��бȽϵ����ȼ�ֻ��1��ѡ��ʱ���ͱ�ʾ���ҳɹ�
Calls:          // None
Table Accessed: // None
Table Updated:  // None
Input:          // strTermId - Ҫ���ҵ��ն˺�
                // bFullMatch - �Ƿ�ֻ������ȫƥ�䣬FALSE��ʾֻҪĿ���а����������ն˺ż���ƥ��
Output:         // ActiveMember - ���ҵ�����ƥ��Ľ��
Return:         // TRUE
Others:         // None
*************************************************/
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

	//����Map��Ѱ����strTermId��ͬ��Ԫ�أ�����vector
	while (pos != NULL)
	{
		UINT nMemberId = 0;

		CActiveMember AM;
		m_ActiveMemberMap.GetNextAssoc(pos, nMemberId, AM); //posΪMap����һ��λ��,nMemberIdΪ��ȡԪ�ص�Keyֵ,AMΪ����ȡԪ��

		// 2011/06/07-gxx: �޸�Ϊģ������
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

	if (nFound == 1)        //ֻ��1����Ӧ�ģ���ֱ�ӷ���
	{
		ActiveMember = vecRes[0];
		return TRUE;
	}
	
	// nFound > 1
	CString strTerm = vecRes[0].GetTerminalID();
	strTerm.MakeUpper();
	int nMin = 0;
	//���������ҵ���Ԫ�أ��������������ȫ��ͬ��Ԫ�ط���vecEqual�������һ���ն˺���ȫ��ȵ�Ԫ�طŵ�vecSame
	for(int i = 0; i < nFound; i++)
	{
		CString strTmp = vecRes[i].GetTerminalID();
		strTmp.MakeUpper();
		if (  strTmp == strTermId ) // ��������ն���ȫ��ͬ������vecEqual
		{
			vecEqual.push_back(vecRes[i]);
		}
		if (vecEqual.size() > 0)    // vecEqual��size��Ϊ0���ͽ�����һ��ѭ���������������
		{
			continue;
		}
		if ( strTmp == strTerm ) // �ͽ����ĵ�һ���ն���ͬ������vecSame
		{
			vecSame.push_back(vecRes[i]);
		}
		if (strTmp < strTerm)   // С�ڽ����ĵ�һ���նˣ������丳ֵ����һ���ն�
		{
			nMin = i;
			strTerm = strTmp;
		}
	}

	//����������ն˺���ȫ��ȵ�ֻ��1����������Ϊ���������
	if (vecEqual.size() == 1)
	{
		ActiveMember = vecEqual[0];
		return TRUE;
	}
	//���򷵻����и���ʱ�������
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
	//û����ȫһ�µġ������������̵ġ�
	else if(vecSame.size() > 1)
	{
		// ���Ȳ������ߵ��û������û�����ߵ��û����򷵻ظ���ʱ������ġ�ֻ��һ�����ߵ��򷵻���һ�������򷵻ظ���ʱ������ġ����򷵻س�����̵ġ�
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

//ʹ��Net�����û�,��ʹ�ò���ȫƥ��,i.e.Ŀ���û���NetId�а�������strNetId����.
BOOL CActiveMemberList::GetActiveMemberNetID(CString strNetId,CActiveMember& ActiveMember, BOOL bFullMatch /*=TRUE*/)
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

//����Ҫ���µ������б���(MemberID)��ȡ��ǰ��һ������ֵ���أ����������б���ɾ��
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
	IBA_LOG(_T("RemoveActiveMember.MemberId:%d NetId:%s TermId:%s"),nMemberId,ActiveMember.GetNetId(),ActiveMember.GetTerminalID());

	BOOL bRet = m_ActiveMemberMap.RemoveKey(nMemberId);

	if (bRet)
	{
		m_UpdateMemberIdArray.Add(nMemberId);
	}
	RemoveActiveMemberDB(nMemberId);

	//IBA_LOG0(_T("in RemoveActiveMember"));	
	// 2014-3-11 - qsc
	// ��������Ҫ��----------------------------------------------------------
	//CString strIniFile = theApp.GetWorkPath();
	//strIniFile.Append(_T("\\IBAConfig\\nde_server.ini")); // base, Filename
	//if (::PathFileExists(strIniFile))
	//{
	//	TCHAR tRes[512] = {0};
	//	::GetPrivateProfileString(_T("base"), _T("Filename"), NULL, tRes, 512, strIniFile);
	//	if (::PathFileExists(tRes))
	//	{
	//		/*
	//		����û�:add|�û���|����

	//		ɾ���û�:del|�û���

	//		*/
	//		CString strParam;
	//		strParam.Format(_T("del|%s"), ActiveMember.GetNetId());

	//		IBA_LOG(_T("ִ��: %s %s"), tRes, strParam);

	//		ShellExecute(NULL, _T("open"), tRes, strParam, NULL, SW_SHOWNORMAL);
	//	}
	//	else
	//	{
	//		IBA_LOG(_T("ִ���ļ�:%s ������"), tRes);;
	//	}
	//}
	//else
	//{
	//	IBA_LOG(_T("�����ļ�:%s ������"), strIniFile);
	//}	

	//---------------------------------------------------------------------

	// ��ѯʵ�����߱�-20151230 liyajun�����ϻ�ǰ��ʵ���»�,��ʱȡ��
	//try
	//{
	//	CString strSQL;

	//	strSQL.Format(_T("select * from auditinfo where MEMBERID=%d order by LogonTime desc"), nMemberId);

	//	IBA_TRACE(strSQL);

	//	CADODBConnInfo* pDb = NULL;

	//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb))
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - Result :GetDBConnInfo for quering in auditinfo failed."));
	//		return FALSE;
	//	}

	//	CADORecordset Rs(pDb->GetConn());
	//	if (Rs.Open(strSQL)) 
	//	{
	//		bRet = TRUE;
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - Result :quering in auditinfo failed!"));
	//		pDb->Release();
	//		lock.Unlock();
	//		return bRet;
	//	}

	//	int nCount = Rs.GetRecordCount();
	//	if (nCount == 0)
	//	{
	//		Rs.Close();
	//		pDb->Release();
	//		lock.Unlock();
	//		return bRet;
	//	}

	//	CString strTermID, strIP;
	//	Rs.GetFieldValue(_T("MACHNAME"), strTermID);
	//	Rs.GetFieldValue(_T("IP"), strIP);
	//	Rs.Close();
	//	pDb->Release();

	//	if (strTermID.IsEmpty() || strIP.IsEmpty())
	//	{
	//		IBA_LOG0(_T("ʵ���ϻ���Ϣ���ն˺ź�IPΪ��"));
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - AuditUserCheckOut ..."));
	//		theApp.GetRealName()->AuditUserCheckOut(strTermID, strIP, nMemberId);
	//	}
	//}
	//catch (...)
	//{
	//	IBA_LOG0(_T("Check db, throw a exception..."));
	//	bRet = FALSE;
	//}

	lock.Unlock();

	return bRet;
}

//����ʱɾ���û����������»���Ϣ
BOOL CActiveMemberList::RemoveActiveMemberDBPostActivation(UINT nMemberId)
{
	//IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - Start for MemberId = %d"),nMemberId);	
	CSingleLock lock(&ActiveMemberListLock, TRUE);



	CActiveMember ActiveMember;
	GetActiveMember(nMemberId, ActiveMember);

	BOOL bRet = m_ActiveMemberMap.RemoveKey(nMemberId);

	if (bRet)
	{
		m_UpdateMemberIdArray.Add(nMemberId);
	}

	RemoveActiveMemberDB(nMemberId);


	//IBA_LOG0(_T("in RemoveActiveMember"));	
	// 2014-3-11 - qsc
	// ��������Ҫ��----------------------------------------------------------
	//CString strIniFile = theApp.GetWorkPath();
	//strIniFile.Append(_T("\\IBAConfig\\nde_server.ini")); // base, Filename
	//if (::PathFileExists(strIniFile))
	//{
	//	TCHAR tRes[512] = {0};
	//	::GetPrivateProfileString(_T("base"), _T("Filename"), NULL, tRes, 512, strIniFile);
	//	if (::PathFileExists(tRes))
	//	{
	//		/*
	//		����û�:add|�û���|����

	//		ɾ���û�:del|�û���

	//		*/
	//		CString strParam;
	//		strParam.Format(_T("del|%s"), ActiveMember.GetNetId());

	//		IBA_LOG(_T("ִ��: %s %s"), tRes, strParam);

	//		ShellExecute(NULL, _T("open"), tRes, strParam, NULL, SW_SHOWNORMAL);
	//	}
	//	else
	//	{
	//		IBA_LOG(_T("ִ���ļ�:%s ������"), tRes);;
	//	}
	//}
	//else
	//{
	//	IBA_LOG(_T("�����ļ�:%s ������"), strIniFile);
	//}	

	//---------------------------------------------------------------------

	// ��ѯʵ�����߱�
	//�����ɾ�������û�ʱ�������»���Ϣ
	//try
	//{
	//	CString strSQL;

	//	strSQL.Format(_T("select * from auditinfo where MEMBERID=%d order by LogonTime desc"), nMemberId);

	//	IBA_TRACE(strSQL);
	//	IBA_LOG0(strSQL);

	//	CADODBConnInfo* pDb = NULL;

	//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb))
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - Result :GetDBConnInfo for quering in auditinfo failed."));
	//		return FALSE;
	//	}

	//	CADORecordset Rs(pDb->GetConn());
	//	if (Rs.Open(strSQL)) 
	//	{
	//		bRet = TRUE;
	//		//IBA_LOG(_T("Execute %s success."), strSQL);
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - Result :quering in auditinfo failed!"));
	//		pDb->Release();
	//		lock.Unlock();
	//		return bRet;
	//	}

	//	//IBA_LOG0(_T("GetRecordCount ..."));
	//	int nCount = Rs.GetRecordCount();
	//	if (nCount == 0)
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - Result :quering auditinfo count = 0"));
	//		Rs.Close();
	//		pDb->Release();
	//		lock.Unlock();
	//		return bRet;
	//	}

	//	//IBA_LOG0(_T("Get record values ..."));
	//	CString strTermID, strIP;
	//	Rs.GetFieldValue(_T("MACHNAME"), strTermID);
	//	Rs.GetFieldValue(_T("IP"), strIP);
	//	Rs.Close();
	//	pDb->Release();

	//	if (strTermID.IsEmpty() || strIP.IsEmpty())
	//	{
	//		IBA_LOG0(_T("ʵ���ϻ���Ϣ���ն˺ź�IPΪ��"));
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - AuditUserCheckOut ..."));
	//		theApp.GetRealName()->AuditUserCheckOut(strTermID, strIP, nMemberId);
	//	}
	//}
	//catch (...)
	//{
	//	IBA_LOG0(_T("Check db, throw a exception..."));
	//	bRet = FALSE;
	//}
	
	//CComputerInfo computer;
	//if (CLocalServer::GetInstance()->ComputerList.GetComputerInfo(ActiveMember.GetTerminalID(), computer))
	//{
	//	//CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
	//	theApp.GetRealName()->AuditUserCheckOut(
	//		computer.GetTerminalID(), computer.GetComputerIP(),nMemberId);
	//}
	//}

	// 2014-2-25 - qsc
	// 2014-1-13-qsc �����ʱ����
	//INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);

	//if (nIndex >= 0) //Զ���»�
	//{
	//	CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
	//	computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
	//	computer.ClearUserInfo();
	//	CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);

	//	//CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
	//}


	lock.Unlock();


	//IBA_LOG0(_T("CActiveMemberList::RemoveActiveMember - End"));
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
/*************************************************
Function:       // Run
Description:    // �̺߳�����ÿ��30s�������м����û�������(��Ҫ��ActiveMember��)..
Calls:          // CheckAllActiveMember
Others:         // ���̴߳����������У��������ʱ�˳�
*************************************************/
INT CActiveMemberList::Run()
{
	IBA_LOG(_T("Start thread:Check offline ActiveMember"));
	CoInitialize(NULL);

	while (TRUE)
	{

#ifdef _DEBUG
		Sleep(1000 * 5);
#else
		Sleep(1000 * 30);
#endif // _DEBUG

		//try
		//{
		if (theApp.GetCurCashier()->IsOnDuty())
		{
			CheckAllActiveMember();
		}

		RefreshView();
		//}
		//catch (...)
		//{			
		//	DWORD dwErrorCode = GetLastError();
		//	char szErrorMsg[1024];
		//	GetErrorMessage((int)dwErrorCode,szErrorMsg);
		//	int i = MultiByteToWideChar(CP_UTF8, 0, szErrorMsg, -1, NULL, 0);
		//	WCHAR* strSrc;
		//	strSrc = new WCHAR[i+1];
		//	MultiByteToWideChar(CP_UTF8, 0, szErrorMsg, -1, strSrc, i);
		//	CIBALog::GetInstance()->WriteFormat(_T("CActiveMemberList::Run Error! ErrorCode:%d, ErrorMsg:%s"),(int)dwErrorCode,strSrc);
		//	delete strSrc;		
		//}
	}

	CoUninitialize();

	CIBALog::GetInstance()->Write(_T("CActiveMemberList::Run Exit!"));

	return 0;
}

/*************************************************
Function:       // CheckAllActiveMember
Description:    // ��������鼤���û�
Calls:          // CheckLocalUser,CheckRoomUser,CheckActiveMember,RemoveActiveMember
Table Updated: //  ActiveMember
*************************************************/
void CActiveMemberList::CheckAllActiveMember()
{
	ActiveMemberMap TempActiveMemberMap;

	Copy(TempActiveMemberMap);

	CActiveMember ActiveMember;

	POSITION pos = TempActiveMemberMap.GetStartPosition();

	while (pos != NULL)                                                  //����Map��ÿһ���û�
	{
		UINT nMemberId = 0, nCardId = 0;

		CActiveMember ActiveMember;

		TempActiveMemberMap.GetNextAssoc(pos, nMemberId, ActiveMember);


		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)         //����ģʽ
		{
			if(ActiveMember.IsNeedToQuery())
			{
				CheckBrokenCenterUser(ActiveMember);
			}				
		}

		else if (ActiveMember.GetMemberID() == ActiveMember.GetRoomMemberId())   //����ģʽ���ȼ������û�
		{
			CheckRoomUser(ActiveMember);
		}
		//������������Ĳ�ѯ��
		else
		{
			if (ActiveMember.IsNeedToQuery())    
			{
				CIDCheckCardMethod checkCard;

				checkCard.SetSerialNum(ActiveMember.GetNetId());

				theApp.GetCurCashier()->DoCheckCard(checkCard);  //CheckCard

				if (checkCard.GetStatusCode() == 0)
				{
					nCardId = checkCard.GetCardId();
					nMemberId = checkCard.GetMemberId();
				}

				if (nMemberId > 0)
				{
					CheckActiveMember(nCardId, nMemberId, ActiveMember);
				}
				//���δ�����û���ֱ��Remove
				else
				{
					if (checkCard.GetStatusCode() != - 1)
					{
						RemoveActiveMember(ActiveMember.GetMemberID());
						CIBALog::GetInstance()->Write(_T("-------------------����û�------------------"));
						// 2014-3-28 - qsc QSCTAG ��ʱע��
						// 2014-1-13-qsc 
						//CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
					}
				}

				Sleep(50);
			}				
		}	
	}
}

/*************************************************
Function:       //CheckRoomUser
Description:    //�������û������ص��߳���2���ӣ��Զ����ˡ�
Table Updated: //  ActiveMember
Input:          //ActiveMember - �����İ����û�
*************************************************/
void CActiveMemberList::CheckRoomUser(CActiveMember& ActiveMember)
{
	//����Ǯ��CheckCard��Ȼ���������û�һ��CheckActiveMember
	if (ActiveMember.GetAvailavleBalance() > 0)
	{
		if (ActiveMember.IsNeedToQuery())//�����Ҫ��ѯ
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

	if (ActiveMember.GetStatus() == CActiveMember::EStatus_CenterOnLine ||     //��������
		(ActiveMember.GetStatus() == CActiveMember::EStatus_LocalOnline && ActiveMember.IsOffLinePossibility())) //�������ߵ����ܵ�����
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

/***************************************************************************************************************************************************
Function:       //CheckActiveMember
Description:    //Ϊ����IsNeedQuery���û����͸����û���Ϣ(����ģʽ��)
				//DoBalance
				//�û����ߣ�����ΪEStatus_CenterOnLine(IsNeedQuery:curtime - updatetime>0����Ҫ��ѯ,��ʾ�´μ�����ѯ)
				//�û������ߣ��������ProcessActiveMemberInAbnormalCases
				//����Balance�����û�Amout��AvailableBalance��Ϣ..
Input:          // nCardId      -  ��������������Balance
                // nMemberId    -  ��������������Balance
Output:         //ActiveMember - ���º���û���Ϣ
Others:			//���IsNeedQuery,ֻ��EStatus_CenterOnLine - �����û�/��ʱδ�ϻ�����90s�ҵ�ʱ�û�,(��ʱ�û�������updatetimeΪ��ʱʱ��,��ʱ��δ��,��������IsNeedQuery
					����,��������ѯ)�Լ�EStatus_LocalOnLine - ���������û������߳���90s�Ż�ִ��.
***************************************************************************************************************************************************/
void CActiveMemberList::CheckActiveMember(UINT nCardId, UINT nMemberId, CActiveMember &ActiveMember)
{
	//�����
	CBalanceMethod Balance;
	Balance.SetCardId(nCardId);
	Balance.SetMemberId(nMemberId);

	theApp.GetCurCashier()->DoBalance(Balance);

	if (Balance.GetStatusCode() == 1) //��Ч�û�
	{
		IBA_LOG(_T("Temp.CheckActiveMember:RemoveActiveMember-MemberId = %d ��Ч�û�"),ActiveMember.GetMemberID());
		RemoveActiveMember(nMemberId);
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);
		return;
	}	
	if (Balance.GetStatusCode() != 0) return; //ʧ�����˳�
	
	ActiveMember.UpdateDataTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetLogoutTime());//����Ѿ��ǳ��ˣ���Ҫʹ�õǳ�ʱ��


	//�û����ߣ���ʾ�ǻ���������Ϊ��������
	if (Balance.GetOnline() == 1) 
	{
		ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);
	}

	else//�û�������,ȷ�Ϻʹ���
	{
		if(ProcessActiveMemberInAbnormalCases(Balance,ActiveMember)) { return;}
	}


	//�����Ļ�ȡ���µ����
	//�û�������ʱ,�����޷���Iserver��ȡ�˻���Ϣ.���Դ�����Balance�ӿڻ�ȡ,����(�����ѽ�� + ���) - �µ��˻���� = �µ������ѽ��
	INT nAllMoney = Balance.GetTotalBalacne();
	INT nTmp = ActiveMember.GetAmount() + ActiveMember.GetAvailavleBalance() - nAllMoney; 
	ActiveMember.SetAmount(nTmp > 0 ? nTmp : 0);   //�����û����ѽ��
	ActiveMember.SetAvailavleBalance(nAllMoney);   //�����û���ǰ���
	UpdateActiveMember(ActiveMember, TRUE);
	CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

	IBA_LOG(_T("Temp.CheckActiveMember.SetAvailableBalance = %d, for MemberId = %d"),ActiveMember.GetAvailavleBalance(),ActiveMember.GetMemberID());	
}

/***************************************************************************************************************************************************
Function:       //ProcessActiveMemberInAbnormalCases
Description:    //CheckActiveMember�У����û�����/�����߳���һ��ʱ��(ʱ����IsOffLinePossibility������),���ض���ȷ�Ϻʹ���.
					1.��Ա�û�/��ʱ�û�����̨���ò������˿�,ֱ�ӴӼ����б����Ƴ�.(i.e.˵��a.��Ա�û��ٴ��ϻ�ʱ�����Զ�����,b.��ʱ�û���������)
					2.����
						a.�ۼƷⶥ�������Ϊ0,ֱ���˿�ע��
						b.�ۼƷⶥ/���Ԥ��,����updatetimeΪ����ʱ��
						c.�����Ʒ����Ϊ0,�����˿��������Զ��˿�.���򲻽��д���,һֱ��ʾ���û���ͼ,������Ա����.
						d.�����Ʒѻ���Ǯ,��״̬ΪEStatus_Waiting,���ٽ��м��
Input:          //Balance - ���ķ��ص��˻���Ϣ
Return:         //�Ƴ��˼����û������Զ��˿��ˣ�����TRUE.���򷵻�FALSE.
***************************************************************************************************************************************************/
BOOL CActiveMemberList::ProcessActiveMemberInAbnormalCases(CBalanceMethod& Balance,CActiveMember &ActiveMember)
{
	if (Balance.IsNetbarMember()) // ��Ա���ǲ������˿ȡ������
	{
		IBA_LOG(_T("Temp.CheckActiveMember:RemoveActiveMember-MemberId = %d ��Ա/�������˿�&������"),ActiveMember.GetMemberID());
		RemoveActiveMember(Balance.GetMemberId());
		return TRUE;
	}
	else //�����˿����ʱ�û�
	{
		if (Balance.GetPayType() > 0) //�ۼƷⶥ/���Ԥ��
		{
			//�û������ߣ��ۼƷⶥ�����Ϊ0���Զ��˿�
			if (Balance.GetPayType() == CIBAGlobal::emPayTypeAccumulative && Balance.GetTotalBalacne() == 0)
			{
				IBA_LOG(_T("Temp.CheckActiveMember:DeregisterUser-MemberId = %d ��ʱ�û�&�ۼƷⶥ&������&�����&�����˿�"),ActiveMember.GetMemberID());
				CString strErrMsg;
				if(FALSE == CDeregisterHelpper::DeregisterUser(Balance.GetMemberId(),strErrMsg))
				{
					IBA_LOG(_T("�Զ��˿�ʧ��.MemberId=%d,����:%s"),Balance.GetMemberId(),strErrMsg);
				}
				CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
				return TRUE;
			}

			/*����������ΪEStatus_CenterOnLine.һ����ʱʱ�䵽,
				1.���Ļ������PayType=0,
				2.����CurrentTime > Balance.EndTime(IsNeedQuery���������Check����)
			���,������������ƷѵĴ����߼�,�������ֻ������updatetime = ��ʱ����ʱ��,����Ҫ���ദ��.*/
			ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);

			ActiveMember.UpdateDataTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetEndTime());
		}
		else //�����Ʒ�
		{	
			//����Ǯ������Ѻ��,��ΪEStatus_Waiting,���ٲ�ѯ
			if (Balance.GetTotalBalacne() > 0 || Balance.GetGuarantyMoney() > 0)
			{
				ActiveMember.SetStatus(CActiveMember::EStatus_Waiting); 
				ActiveMember.SetTerminalID(_T(""));
				//ActiveMember.SetActive(FALSE);				
			}
			else //û��Ǯ�ˣ������˿��������Զ��˿�.   ---ע�⣺��û��Ǯ�ˣ���û���Զ��˿�ʱ��Ҫ����̨���ÿ��Ƿ����������ѡ���AllowAutoRefund����false.
			{
				if(CDeregisterHelpper::AllowAutoRefund(Balance))//����һ��,һ��Ҫ�Ѿ��»��ģ����»�ʱ�䳬������̨���õ��Զ��˿�ʱ��.���⣬��̨�����˲������˿����Ҳ�����˿�.
				{
					ActiveMember.SetStatus(CActiveMember::EStatus_AutoReturn); 
				}
				if (ActiveMember.GetStatus() == CActiveMember::EStatus_AutoReturn)
				{
					IBA_LOG(_T("Temp.CheckActiveMember:DeregisterUser-MemberId = %d ��ʱ�û�&������&�����&�����˿�&���趨ʱ��"),ActiveMember.GetMemberID());
					CString strErrMsg;
					if(FALSE == CDeregisterHelpper::DeregisterUser(Balance.GetMemberId(),strErrMsg))
					{
						IBA_LOG(_T("�Զ��˿�ʧ��.MemberId=%d,����:%s"),Balance.GetMemberId(),strErrMsg);
					}
					CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
					return TRUE;
				}
				else
				{
					//ûǮ��,�������˲������Զ��˿�,�����ж���,������ѯ
				}
			}
		}
	}
	return FALSE;
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
	strSQL.AppendFormat(_T("%d,"), ActiveMember.GetRoomMemberId());

	strSQL.AppendFormat(_T("%d"), ActiveMember.GetScanType());

	strSQL.Append(_T(")"));

	TRACE(strSQL);

	return CIBADAL::GetInstance()->ExecuteSQL(strSQL);
}

BOOL CActiveMemberList::UpdateActiveMemberDB(CActiveMember &ActiveMember)
{
	//IBA_LOG(_T("Temp.CActiveMemberList::UpdateActiveMemberDB - Start"));

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

	//CIBALog::GetInstance()->WriteFormat(_T(",advancePayment=%d,refNo=%d"), ActiveMember.GetAdvancePayment(),2);

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

	IBA_LOG(_T("Temp.CActiveMemberList::UpdateActiveMemberDB - %s"),strSQL);

	return CIBADAL::GetInstance()->ExecuteSQL(strSQL);

	//IBA_LOG(_T("Temp.CActiveMemberList::UpdateActiveMemberDB - End"));

}

BOOL CActiveMemberList::RemoveActiveMemberDB(UINT nMemberId)
{
	//IBA_LOG0(_T("enter RemoveActiveMemberDB"));
	CString strSQL;

	strSQL.Format(_T("delete from activemember where memberId = %d or memberId = 0"), nMemberId);

	return CIBADAL::GetInstance()->ExecuteSQL(strSQL);
	//IBA_LOG0(_T("leave RemoveActiveMemberDB"));	
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

//void CActiveMemberList::CheckLocalUser(CActiveMember& ActiveMember)
//{
//	IBA_TRACE("��鱾���û�...");
//	
//	if (!ActiveMember.GetIsLocalUser())
//	{
//		IBA_ASSERT2(0,"����Ϊ�����û�");
//		return;
//	}
//
//	if (ActiveMember.GetAvailavleBalance() == 0)
//	{
//		if (ActiveMember.GetPolicyInfo().IsEmpty())   //����ȡ�����ʾ�ȡ��ȡ�����������ٴ���
//		{
//			CString strSQL;
//
//			strSQL.Format(_T("select PolicyInfo from ActiveMember where memberId=%d"),(INT)ActiveMember.GetMemberID());
//
//			IBA_TRACE(strSQL);
//
//			CADODBConnInfo* pDb = NULL;
//
//			if (CIBADAL::GetInstance()->GetDBConnInfo(pDb))
//			{
//
//				CADORecordset Rs(pDb->GetConn());
//
//				if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0) 
//				{
//					Rs.GetFieldValue(_T("PolicyInfo"), ActiveMember.SetPolicyInfo());
//					Rs.Close();
//					pDb->Release();
//				}
//				else
//				{
//					pDb->Release();
//				}
//			}
//		}
//
//		UINT nLocalExpense = ActiveMember.GetLocalExpense(FALSE);   //��ȡ�������ѵĽ�ΪAmount��ͨ�����ʼ��㣬������ǰСʱʣ��ʱ�������Ϊ��ǰʣ��ʱ�䡣���������ϲ��롣
//		nLocalExpense += 5;
//		if (nLocalExpense >= (UINT)ActiveMember.GetAdvancePayment())  //����������ѽ�� <= Ԥ������ + 5��
//		{
//			CCashierHelper::ReturnLocalUser(ActiveMember);  //����LocalConsume�����Ϳͻ����»���Ϣ����ActiveMemberMap��ɾ����Ӧ�������û���ͼ
//		}
//	}
//}

/*************************************************
Function:       // CheckBrokenCenterUser
Description:    // ����ģʽ�����IsNeedQuery�������������û�(����90Sδ�յ�Iserver����,����,��ʱ�û��ѵ��ڵ�δ�ϻ�)
Input:          // ActiveMember - ������û�
*************************************************/
void CActiveMemberList::CheckBrokenCenterUser(CActiveMember& ActiveMember)
{
	if (ActiveMember.GetIsLocalUser())
	{
		IBA_ASSERT2(0,"����Ϊ�����û�");
		return;
	}

	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		return;
	}

	int nMemberId = ActiveMember.GetMemberID();
	if(nMemberId == 0) return;

	//��localMember��ȡ�û��»�ʱ��
	COleDateTime checkoutTime;
	COleDateTime zerotime(1970,1,1,0,0,0);
	CLocalMember localMember;
	if(CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberId,localMember))  //�Ѿ��»�
	{
		if(CIBAHelpper::IsValidTime(localMember.checkoutTime) && localMember.checkoutTime > zerotime)
		{
			checkoutTime  = localMember.checkoutTime;
		}		
	}
	else if(CIBAHelpper::IsValidTime(ActiveMember.GetCheckInTime()) && ActiveMember.GetCheckInTime() > zerotime) //���ϻ���û�»�
	{
		checkoutTime = ActiveMember.GetCheckInTime();
	}	
	else  //�¿����û�.δ���ϻ����»�
	{
		checkoutTime = zerotime;
	}

	//��ͬCheckActiveMemberһ������
	if(CIBAHelpper::IsValidTime(checkoutTime) && checkoutTime > zerotime)
	{
		ActiveMember.SetUpdateDataTime(checkoutTime);
	}
	else
	{
		ActiveMember.SetUpdateDataTime(zerotime);
	}

	//�ٴ�ȷ���û��Ƿ�����
	if (CIBAHelpper::IsValidTime(ActiveMember.GetCheckInTime()) && ActiveMember.GetCheckInTime() > COleDateTime(2011,1,1,0,0,0))
	{
		ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);
	}
	else
	{

		//�Ȼ�ȡ��̨�����Ƿ�����˿�,��ȡ������ֱ�ӷ���
		int nParamAllowReturn;
		UINT IsMember = 0;
		int n = CIBADAL::GetInstance()->GetAllowReturn(ActiveMember.GetClassId(),&IsMember);
		if(n >= 0)
		{
			nParamAllowReturn = n;
		}
		else//��ѯ�����Ƿ�����˿���Ϣ,�ݲ�����
		{
			ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);
			return;
		}

		if (IsMember > 0 || !nParamAllowReturn == 0) // ��Ա���ǲ������˿ȡ������
		{
			IBA_LOG(_T("Temp.CheckBrokenCenterUser:RemoveActiveMember-MemberId = %d ��Ա/�������˿�&������"),ActiveMember.GetMemberID());
			RemoveActiveMember(nMemberId);
			return;
		}
		else //�����˿����ʱ�û�
		{
			if (ActiveMember.GetPayType() > 0) //�ۼƷⶥ/���Ԥ��
			{
				//�û������ߣ��ۼƷⶥ�����Ϊ0���Զ��˿�
				if (ActiveMember.GetPayType() == CIBAGlobal::emPayTypeAccumulative && ActiveMember.GetAvailavleBalance() == 0)
				{
					IBA_LOG(_T("Temp.CheckBrokenCenterUser:DeregisterUser-MemberId = %d ��ʱ�û�&�ۼƷⶥ&�����&������&�����˿�"),ActiveMember.GetMemberID());
					CString strErrMsg;
					if(FALSE == CDeregisterHelpper::DeregisterUser(nMemberId,strErrMsg))
					{
						IBA_LOG(_T("Temp.CheckBrokenCenterUser�˿�ʧ��-MemberId = %d,����:%s"),nMemberId,strErrMsg);
					}
					CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
					return;
				}

				/*����������ΪEStatus_CenterOnLine.һ����ʱʱ�䵽,
					1.���Ļ������PayType=0,
					2.����CurrentTime > Balance.EndTime(IsNeedQuery���������Check����)
				���,������������ƷѵĴ����߼�,�������ֻ������updatetime = ��ʱ����ʱ��,����Ҫ���ദ��.*/
				ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);
			
				time_t time = ActiveMember.GetNextCheckinTime();
				COleDateTime nextdatetime(time);
				if(CIBAHelpper::IsValidTime(nextdatetime) && nextdatetime > zerotime)
				{
					ActiveMember.UpdateDataTime = nextdatetime;
				}
				
			}
			else //�����Ʒ�
			{	
				//����Ǯ������Ѻ��,��ΪEStatus_Waiting,���ٲ�ѯ
				if (ActiveMember.GetAvailavleBalance() > 0)
				{
					ActiveMember.SetStatus(CActiveMember::EStatus_Waiting); 
					ActiveMember.SetTerminalID(_T(""));
					//ActiveMember.SetActive(FALSE);				
				}
				else //û��Ǯ�ˣ������˿��������Զ��˿�.   ---ע�⣺��û��Ǯ�ˣ���û���Զ��˿�ʱ��Ҫ����̨���ÿ��Ƿ����������ѡ���AllowAutoRefund����false.
				{
					if(CDeregisterHelpper::AllowAutoRefundInLocalMode(ActiveMember,nParamAllowReturn,localMember.checkoutTime))//����һ��,һ��Ҫ�Ѿ��»��ģ����»�ʱ�䳬������̨���õ��Զ��˿�ʱ��.���⣬��̨�����˲������˿����Ҳ�����˿�.
					{
						ActiveMember.SetStatus(CActiveMember::EStatus_AutoReturn); 
					}
					if (ActiveMember.GetStatus() == CActiveMember::EStatus_AutoReturn)
					{
						IBA_LOG(_T("Temp.CheckBrokenCenterUser:ReturnLocalCenterUser-MemberId = %d ��ʱ�û�&�����&������&�����˿�&�»����趨ʱ��"),ActiveMember.GetMemberID());
						CString strErrMsg;
						if(FALSE == CDeregisterHelpper::DeregisterUser(nMemberId,strErrMsg))
						{
							IBA_LOG(_T("CheckBrokenCenterUser.�Զ��˿�ʧ��.MemberId=%d,����:%s"),nMemberId,strErrMsg);
						}
						CLocalServer::GetInstance()->GetComputerListView()->SendMessage(WM_COMMAND,IDM_MYREFRESH,0);
						return ;
					}
					else
					{
						//ûǮ��,�������˲������Զ��˿�,�����ж���,������ѯ
					}
				}
			}
		}
	}
	return;
}


	///**************************************************************************************************************/
	//}
	////���Ϊ0 - ���ǰ�ʱ����û���ڽ��г�ֵ������
	////		  - ���ڰ�ʱ�������ʱʱ�䵽�ͽ���
	//if (ActiveMember.GetAvailavleBalance() == 0)      //������Ϊ0
	//{
	//	if (ActiveMember.GetNextCheckinTime() == 0)
	//	{
	//		//���жϳ�ֵ���ֵ�˿��δ�������������˵�����ڳ�ֵ��������������
	//		BOOL bIsNDCredit(FALSE);
	//		BOOL bIsCredit(FALSE);
	//		LPTSTR lpszWndCaption = new TCHAR[30];
	//		memset(lpszWndCaption,0,30);
	//		HWND hwnd = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();
	//		if(::GetWindowText(hwnd,lpszWndCaption,30) > 0)
	//		{
	//			CString strWndCation(lpszWndCaption);
	//			strWndCation.Trim();
	//			bIsCredit = !strWndCation.CompareNoCase(_T("��ֵ�˿�"));
	//			bIsNDCredit = !strWndCation.CompareNoCase(_T("��ֵ"));
	//		}
	//		if(!(bIsCredit || bIsNDCredit))  //��ֵ����δ����
	//		{
	//			//UINT nLocalExpense = ActiveMember.GetLocalExpense(FALSE);
	//			//nLocalExpense += 5;
	//			//if (nLocalExpense >= (UINT)ActiveMember.GetAdvancePayment())
	//			//{
	//			IBA_LOG(_T("Temp.CActiveMemberList::CheckBrokenCenterUser.�û����Ϊ0������!"));
	//			CCashierHelper::ReturnLocalCenterUser(ActiveMember);//ִ�б���ģʽ�Ľ��˻��»�				
	//		}
	//	}
	//	else //��ʱ
	//	{			
	//		COleDateTime odt((__int64)ActiveMember.GetNextCheckinTime());
	//		COleDateTime curTime = COleDateTime::GetCurrentTime();
	//		if (curTime >= odt)//��ʱ����
	//		{
	//			IBA_LOG(_T("Temp.CActiveMemberList::CheckBrokenCenterUser.�û���ʱ���������Ϊ0������!"));
	//			CCashierHelper::ReturnLocalCenterUser(ActiveMember);
	//		}

	//	}
	//}
	////��Ϊ0 -  ��Ա -        �ϻ��У��������߳���1Сʱ������
	//else     
	//{
	//	//��Ա
	//	if (ActiveMember.IsMember())   
	//	{
	//		if(!ActiveMember.GetTerminalID().IsEmpty())  //���ϻ�
	//		{
	//			CComputerInfo ComInfo;
	//			if (CLocalServer::GetInstance()->ComputerList.GetComputerInfo(ActiveMember.GetMemberID(),ComInfo)) //���ҵ��ն���Ϣ
	//			{
	//				if (ComInfo.GetComputerStatus() != CComputerInfo::ECS_ONLINE)  //��������
	//				{
	//					COleDateTime dtNow = COleDateTime::GetCurrentTime();
	//					COleDateTimeSpan span = dtNow - ActiveMember.GetUpdateDataTime();
	//					IBA_TRACE("��Ա���ߣ�ʱ�� ��=%.2f",span.GetTotalMinutes());
	//					
	//					double dMins = span.GetTotalMinutes();
	//					if (dMins >= 60.0)                                                         //��һ�θ���ʱ������ڴ���60���ӣ�����
	//					{
	//					/*	CCheckOutinfo chInfo;
	//						CCashierHelper::MakeCheckOutInfo(ActiveMember,chInfo);*/
	//						//chInfo.SetCheckOutTime() = CIBAHelpper::FormatTime(ActiveMember.GetUpdateDataTime());
	//						IBA_LOG(_T("Temp.CActiveMemberList::CheckBrokenCenterUser.��Ա�ϻ��У����������߳���1Сʱ������!"));
	//						CCashierHelper::ReturnLocalCenterUser(ActiveMember);
	//					}
	//				}
	//			}
	//		}
	//	}

	//	//��ʱ�û�
	//	else    
	//	{
	//		//if (!ActiveMember.GetTerminalID().IsEmpty())     //�ϻ���
	//		//{
	//		//	CComputerInfo ComInfo;
	//		//	if (CLocalServer::GetInstance()->ComputerList.GetComputerInfo(ActiveMember.GetMemberID(),ComInfo))  //���ҵ��ն���Ϣ
	//		//	{
	//		//		// ��ʱ�û��ϻ�������������
	//		//		if (ComInfo.GetComputerStatus() != CComputerInfo::ECS_ONLINE)    //����
	//		//		{
	//		//			INT nCost = ActiveMember.GetLocalExpense();
	//		//			if (nCost >= ActiveMember.GetAdvancePayment())               //nLocalExpense >= AdvancePayment���Զ�����
	//		//			{
	//		//				ActiveMember.SetAvailavleBalance(0);
	//		//				ActiveMember.SetAmount(ActiveMember.GetAdvancePayment());
	//		//				IBA_LOG(_T("Temp.CActiveMemberList::CheckBrokenCenterUser.��ʱ�û��ϻ��У����������ˣ�nLocalExpense >= AdvancePayment������!"));
	//		//				CCashierHelper::ReturnLocalCenterUser(ActiveMember);	
	//		//			}
	//		//		}
	//		//	}
	//		//}
	//	}
	//}



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

//���������б������ڸ��ն�����������û������ĳ���û���RoomMemberId>0������Ϊ������Ϊ�������򡣷���RoomMemberId.
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
			Sleep(500);

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
