#include "stdafx.h"
#include ".\ibadal.h"
#include "..\IBA.h"
#include "..\IBAHelpper.h"
#include "..\Cashier.h"
#include "..\..\L2Markup\Markup.h"
#include "..\LocalMember.h"
#include <time.h>

BOOL CIBADAL::UpdateMemberInfo(CBalanceMethod & Balance)
{
	BOOL bRet = FALSE;

	CString strSQL;
	strSQL.Format(_T("select * from member where memberId=%d"), Balance.GetMemberId());

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		if (Rs.GetRecordCount() == 0)
		{
			bRet = Rs.AddNew();
			Rs.SetFieldValue(_T("memberId"), Balance.GetMemberId());
		}
		else
		{
			Rs.Edit();
		}

		Rs.SetFieldValue(_T("netBarId"), Balance.GetOpenNetbarId());
		Rs.SetFieldValue(_T("idType"), Balance.GetIdType());
		Rs.SetFieldValue(_T("idNumber"), Balance.GetPersonalId());
		Rs.SetFieldValue(_T("accountNetBarId"), Balance.GetAccountNetbarId());
		Rs.SetFieldValue(_T("serialNum"), Balance.GetSerailNum());
		Rs.SetFieldValue(_T("operator"), theApp.GetCurCashier()->GetName());
		Rs.SetFieldValue(_T("CashRegisterID"), CIBAConfig::GetInstance()->GetCashRegisterID());
		Rs.SetFieldValue(_T("classId"), Balance.GetUserClass());
		Rs.SetFieldValue(_T("sex"), Balance.GetSex());
		Rs.SetFieldValue(_T("name"), Balance.GetUserName());
		Rs.SetFieldValue(_T("membername"), Balance.GetMemberName());
		Rs.SetFieldValue(_T("available"), 0); //����
		Rs.SetFieldValue(_T("isMember"), Balance.GetIsMember());
		Rs.SetFieldValue(_T("allowReturn"), Balance.GetAllowReturn());
		Rs.SetFieldValue(_T("disallowLogout"), Balance.GetDisallowLogout());
		Rs.SetFieldValue(_T("operationDateTime"), CIBAHelpper::GetCurrentTimeAsString(TRUE));
		
		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::UpdateMemberInfo(int MemberID, LPCTSTR szNation, LPCTSTR szIDDepart, LPCTSTR szIDAddr, LPCTSTR szTel)
{
	BOOL bRet = FALSE;

	CString strSQL;
	strSQL.Format(_T("select * from member where memberId=%d"), MemberID);

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		int n = Rs.GetRecordCount();
		if (Rs.GetRecordCount() == 0)
		{
			bRet = Rs.AddNew();
			Rs.SetFieldValue(_T("memberId"), MemberID);
		}
		else
		{
			Rs.Edit();
		}
		
		//2015-0828 liyajun ����ֵΪNULL���ֶθ�ֵ�մ�ʱ����֪Ϊ�λᱨ������Ƚ�����Ϊ�մ���
		if(Rs.IsFieldNull(_T("tel")))
		{
			Rs.SetFieldEmpty(_T("tel"));
		}
		if(Rs.IsFieldNull(_T("job")))
		{
			Rs.SetFieldEmpty(_T("job"));
		}
		if(Rs.IsFieldNull(_T("address")))
		{
			Rs.SetFieldEmpty(_T("address"));
		}
		if(Rs.IsFieldNull(_T("department")))
		{
			Rs.SetFieldEmpty(_T("department"));
		}
		Rs.Update();

		Rs.Edit();
		Rs.SetFieldValue(_T("tel"), szTel);
		Rs.SetFieldValue(_T("job"), szNation);		// �����ǹ����ֶΣ���֪��Ϊʲô��������
		Rs.SetFieldValue(_T("address"), szIDAddr); 
		Rs.SetFieldValue(_T("department"), szIDDepart);
		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL NS_DAL::CIBADAL::UpdateMemberInfo(CLocalConsumeInfo & localInfo, UINT nNewMemberID)
{
	if (nNewMemberID == 0)
	{
		return FALSE;
	}
	BOOL bRet = FALSE;

	CString strSQL;
	strSQL.Format(_T("update member set memberId=%d where memberId=%d"), nNewMemberID,(INT)localInfo.GetMemberId());

	return this->ExecuteSQL(strSQL);

}
BOOL NS_DAL::CIBADAL::AddMemberInfo(CLocalRegisterInfo & registerInfo)
{
	BOOL bRet = FALSE;

	CString strSQL = _T("select * from member where memberId=0 and memberId=1");

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		bRet = Rs.AddNew();
		Rs.SetFieldValue(_T("memberId"), registerInfo.GetMemberId());
		
		Rs.SetFieldValue(_T("netBarId"), registerInfo.GetNetbarId());
		Rs.SetFieldValue(_T("idType"), registerInfo.GetIdType());
		Rs.SetFieldValue(_T("idNumber"), registerInfo.GetPersonalId());
		Rs.SetFieldValue(_T("accountNetBarId"), registerInfo.GetNetbarId());
		Rs.SetFieldValue(_T("serialNum"), registerInfo.GetNetId());
		Rs.SetFieldValue(_T("operator"), theApp.GetCurCashier()->GetName());
		Rs.SetFieldValue(_T("CashRegisterID"), CIBAConfig::GetInstance()->GetCashRegisterID());
		Rs.SetFieldValue(_T("classId"), registerInfo.GetUserClass());
		Rs.SetFieldValue(_T("sex"), registerInfo.GetSex());
		Rs.SetFieldValue(_T("name"), registerInfo.GetUserName());
		Rs.SetFieldValue(_T("membername"), registerInfo.GetUserName());
		Rs.SetFieldValue(_T("available"), 1); //����
		Rs.SetFieldValue(_T("isMember"), 0);
		Rs.SetFieldValue(_T("allowReturn"), 1);
		Rs.SetFieldValue(_T("disallowLogout"), 0);
		Rs.SetFieldValue(_T("operationDateTime"), CIBAHelpper::GetCurrentTimeAsString(TRUE));

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}


BOOL CIBADAL::GetMemberUpgradeRecord(UINT nMemberId, CMemberUpgradeDB& MemberUpgradeInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Format(_T("SELECT * FROM member WHERE memberId = %d"), nMemberId);

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0)
	{

		CString strTmp;
		UINT nTmp = 0;

		Rs.SetFieldValue(_T("operationTime"), strTmp);
		MemberUpgradeInfo.SetUpgradeTime(strTmp);

		Rs.SetFieldValue(_T("netBarId"), nTmp);
		MemberUpgradeInfo.SetNetBarId(nTmp);

		Rs.SetFieldValue(_T("refNo"), nTmp);
		MemberUpgradeInfo.SetRefNo(nTmp);

		Rs.SetFieldValue(_T("serialNo"), nTmp);
		MemberUpgradeInfo.SetSerialNo(nTmp);

		Rs.SetFieldValue(_T("memberId"), nTmp);
		MemberUpgradeInfo.SetMemberId(nTmp);

		Rs.SetFieldValue(_T("oldClassId"), nTmp);
		MemberUpgradeInfo.SetOldClassId(nTmp);

		Rs.SetFieldValue(_T("classId"), nTmp);
		MemberUpgradeInfo.SetClassId(nTmp);

		Rs.SetFieldValue(_T("reason"), strTmp);
		MemberUpgradeInfo.SetReason(strTmp);

		Rs.SetFieldValue(_T("operator"), strTmp);
		MemberUpgradeInfo.SetOperator(strTmp);

		Rs.Close();

		bRet = TRUE;
	}

	pDb->Release();

	return bRet;

}

BOOL CIBADAL::AddMemberUpgradeRecord(CMemberUpgradeDB & UpgradeInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Append(_T("select * from memberupgrade where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("operationTime"), UpgradeInfo.GetUpgradeTime());
		Rs.SetFieldValue(_T("netBarId"), UpgradeInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), UpgradeInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), UpgradeInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), UpgradeInfo.GetMemberId());
		Rs.SetFieldValue(_T("oldClassId"), UpgradeInfo.GetOldClassId());
		Rs.SetFieldValue(_T("classId"), UpgradeInfo.GetClassId());
		Rs.SetFieldValue(_T("reason"), UpgradeInfo.GetReason());
		Rs.SetFieldValue(_T("operator"), UpgradeInfo.GetOperator());

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddCreditRecord(CCreditInfoDB & CreditInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Append(_T("select * from credited where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("creditDate"), CreditInfo.GetCreditDate());
		Rs.SetFieldValue(_T("netBarId"), CreditInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), CreditInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), CreditInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), CreditInfo.GetMemberId());
		Rs.SetFieldValue(_T("creditAmount"), CreditInfo.GetCreditAmount());
		Rs.SetFieldValue(_T("point"), CreditInfo.GetPoint());
		Rs.SetFieldValue(_T("creditPresent"), CreditInfo.GetCreditPresent());
		Rs.SetFieldValue(_T("operator"), CreditInfo.GetOperator());
		Rs.SetFieldValue(_T("CashRegisterID"), CreditInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("week"), CreditInfo.GetWeek());
		Rs.SetFieldValue(_T("classId"), CreditInfo.GetCassId());
		Rs.SetFieldValue(_T("status"), CreditInfo.GetStatus());
		

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddCardCreditRecord(CCreditInfoDB & CreditInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Append(_T("select * from cardcredited where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("creditDate"), CreditInfo.GetCreditDate());
		Rs.SetFieldValue(_T("netBarId"), CreditInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), CreditInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), CreditInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), CreditInfo.GetMemberId());
		Rs.SetFieldValue(_T("creditAmount"), CreditInfo.GetCreditAmount());
		Rs.SetFieldValue(_T("category"), CreditInfo.GetCategory());
		Rs.SetFieldValue(_T("cardSerial"), CreditInfo.GetCardSerial());
		Rs.SetFieldValue(_T("point"), CreditInfo.GetPoint());
		Rs.SetFieldValue(_T("creditPresent"), CreditInfo.GetCreditPresent());
		Rs.SetFieldValue(_T("operator"), CreditInfo.GetOperator());
		Rs.SetFieldValue(_T("CashRegisterID"), CreditInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("week"), CreditInfo.GetWeek());
		Rs.SetFieldValue(_T("classId"), CreditInfo.GetCassId());
		Rs.SetFieldValue(_T("status"), CreditInfo.GetStatus());

		bRet = Rs.Update();

		Rs.Close();
	}
	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddCardOpenRecord(CCardOpenInfo & CardOpenInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	strSQL = (_T("select * from cardopen where 1=0")); 

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("operationDateTime"), CardOpenInfo.GetOperationDateTime());
		Rs.SetFieldValue(_T("netBarId"), CardOpenInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), CardOpenInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), CardOpenInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), CardOpenInfo.GetMemberId());
		Rs.SetFieldValue(_T("cardId"), CardOpenInfo.GetCardId());
		Rs.SetFieldValue(_T("deposit"), CardOpenInfo.GetDeposit());
		Rs.SetFieldValue(_T("costExpense"), CardOpenInfo.GetCostExpense());
		Rs.SetFieldValue(_T("operator"), CardOpenInfo.GetOperator());
		Rs.SetFieldValue(_T("CashRegisterID"), CardOpenInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("week"), CardOpenInfo.GetWeek());
		Rs.SetFieldValue(_T("classId"), CardOpenInfo.GetClassId());
		Rs.SetFieldValue(_T("status"), CardOpenInfo.GetStatus());

		bRet = Rs.Update();

		Rs.Close();
	}

	strSQL.Format(_T("select * from member where memberId=%d"), CardOpenInfo.GetMemberId());

	if (Rs.Open(strSQL))
	{
		if (Rs.GetRecordCount() == 0) 
		{
			 Rs.AddNew();
			 Rs.SetFieldValue(_T("memberId"), CardOpenInfo.GetMemberId());
		}
		else
		{
			Rs.Edit();
		}

		Rs.SetFieldValue(_T("operationDateTime"), CardOpenInfo.GetOperationDateTime());
		Rs.SetFieldValue(_T("startTime"), CardOpenInfo.GetOperationDateTime());
		Rs.SetFieldValue(_T("netBarId"), CardOpenInfo.GetNetBarId());
		Rs.SetFieldValue(_T("expireTime"), CString(_T("3000-01-01 00:00:00")));
		Rs.SetFieldValue(_T("idType"), CardOpenInfo.GetIdType());
		Rs.SetFieldValue(_T("idNumber"), CardOpenInfo.GetIdNumber());
		Rs.SetFieldValue(_T("serialNum"), CardOpenInfo.GetSerialNum());
		Rs.SetFieldValue(_T("accountNetBarId"),CardOpenInfo.GetAccountNetBarId());
		Rs.SetFieldValue(_T("cardId"), CardOpenInfo.GetCardId());
		Rs.SetFieldValue(_T("deposit"), CardOpenInfo.GetDeposit());
		Rs.SetFieldValue(_T("costExpense"), CardOpenInfo.GetCostExpense());
		Rs.SetFieldValue(_T("operator"), CardOpenInfo.GetOperator());
		Rs.SetFieldValue(_T("CashRegisterID"), CardOpenInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("classId"), CardOpenInfo.GetClassId());
		Rs.SetFieldValue(_T("sex"), CardOpenInfo.GetUserSex());
		Rs.SetFieldValue(_T("name"), CardOpenInfo.GetUserName());
		Rs.SetFieldValue(_T("available"), 0); //����

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddCardCloseRecord(CCardCloseInfo & CardCloseInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Append(_T("select * from cardclose where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("lossDate"), CardCloseInfo.GetLossDate());
		Rs.SetFieldValue(_T("netBarId"), CardCloseInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), CardCloseInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), CardCloseInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), CardCloseInfo.GetMemberId());
		Rs.SetFieldValue(_T("CardId"), CardCloseInfo.GetCardId());
		Rs.SetFieldValue(_T("deposit"), CardCloseInfo.GetDeposit());
		Rs.SetFieldValue(_T("operator"), CardCloseInfo.GetOperator());
		Rs.SetFieldValue(_T("appendDate"), CardCloseInfo.GetAppendDate());
		Rs.SetFieldValue(_T("CashRegisterID"), CardCloseInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("costExpense"), CardCloseInfo.GetCostExpense());
		Rs.SetFieldValue(_T("classId"), CardCloseInfo.GetClassId());
		
		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

/*************************************************
Function:       // AddReturnedRecord
Description:    // �����˿���˻��������ݱ�ĸ���
Calls:          // None
Table Accessed: // LastUser, Returned, Member
Table Updated: //  LastUser - ɾ����¼, Returned - ������¼, Member - �޸ļ�¼��AvailableΪ2(���˿�)
Input:          // ReturnedInfo��������CBrokenNetworkResume::DeregisterUser�еõ���һЩ���ڴ��˿��û�����Ϣ
Output:         // None
Return:         // �������ݿ�������ɹ�������TRUE
Others:         // None
*************************************************/
BOOL CIBADAL::AddReturnedRecord(CReturnedInfo & ReturnedInfo)
{
	BOOL bRet = FALSE;

	DeleteLastUser(ReturnedInfo.GetMemberId());           //��LastUser��ɾ����¼

	CString strSQL;

	strSQL = (_T("select * from returned where 1=0"));   //��returned��

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.AddNew())                  //����¼�¼
	{
		Rs.SetFieldValue(_T("returnDate"), ReturnedInfo.GetReturnDate());
		Rs.SetFieldValue(_T("netBarId"), ReturnedInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), ReturnedInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), ReturnedInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), ReturnedInfo.GetMemberId());
		Rs.SetFieldValue(_T("CardId"), ReturnedInfo.GetCardId());
		Rs.SetFieldValue(_T("deposit"), ReturnedInfo.GetDeposit());
		Rs.SetFieldValue(_T("operator"), ReturnedInfo.GetOperator());
		Rs.SetFieldValue(_T("CashRegisterID"), ReturnedInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("costExpense"), ReturnedInfo.GetCostExpense());
		Rs.SetFieldValue(_T("classId"), ReturnedInfo.GetClassId());
		Rs.SetFieldValue(_T("returnAmount"), ReturnedInfo.GetReturnAmount());
		Rs.SetFieldValue(_T("roundAmount"), ReturnedInfo.GetRoundAmount());

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	strSQL.Format(_T("update member set available = 2 where memberid=%d"), ReturnedInfo.GetMemberId());//����Member��

	ExecuteSQL(strSQL);

	return bRet;
}

/*************************************************
Function:       // DeleteLastUser
Description:    // ��lastUser����ɾ����Ӧ��¼
Calls:          // ExcuteSQL
Table Accessed: // lastUser
Table Updated: // lastUser, ɾ����ӦmemberId�ļ�¼
Input:          // memberId
Output:         // None
Return:         // ִ�гɹ�����TRUE�����򷵻�FALSE
Others:         // None
*************************************************/
BOOL CIBADAL::DeleteLastUser(UINT nMemberId)
{
	CString strSQL;

	strSQL.Format(_T("delete FROM lastUser where memberid = %d"), nMemberId);

	//д��־
	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("DeleteLastUserInfo:%s\r\n"), strSQL);

	return ExecuteSQL(strSQL);
}

BOOL CIBADAL::GetShiftCashRecord(INT &nRemainBalance)
{
	BOOL bRet = FALSE;
	nRemainBalance = 0;

	CString strSQL;

	strSQL.Format(_T("select remainAmount , EndDate, CashRegisterId  from shiftcash "));

	strSQL.AppendFormat(_T("where CashRegisterId = '%d' "), CIBAConfig::GetInstance()->GetCashRegisterID());

	strSQL.Append(_T(" order by  EndDate desc limit 1 "));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0)
	{
		bRet = TRUE;

		Rs.GetFieldValue(_T("remainAmount"), nRemainBalance);

		Rs.Close();

		bRet = TRUE;
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddShiftCashRecord(CShiftCash & ShiftCash)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Append(_T("select * from shiftcash where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb, TRUE)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("StartDate"), ShiftCash.GetStartDate());
		Rs.SetFieldValue(_T("StandbyAmount"), ShiftCash.GetStandbyAmount());
		Rs.SetFieldValue(_T("Deposit"), ShiftCash.GetDeposit());
		Rs.SetFieldValue(_T("CreditAmount"), ShiftCash.GetCreditAmount());
		Rs.SetFieldValue(_T("PresentAmount"), ShiftCash.GetPresentAmount());
		Rs.SetFieldValue(_T("ReturnAmount"), 0 - ShiftCash.GetReturnAmount());
		Rs.SetFieldValue(_T("CardSoldAmount"), ShiftCash.GetCardSoldAmount());
		Rs.SetFieldValue(_T("CashSoldAmount"), ShiftCash.GetCashSoldAmount());
		Rs.SetFieldValue(_T("CardCreditedAmount"), ShiftCash.GetCardCreditedAmount());
		Rs.SetFieldValue(_T("TotalAmount"), ShiftCash.GetTotalAmount());
		Rs.SetFieldValue(_T("CostExpense"), ShiftCash.GetCostExpense());
		Rs.SetFieldValue(_T("Operator"), ShiftCash.GetOperator());
		//Rs.SetFieldValue(_T("NextOperator"), ShiftCash.GetNextOperator());
		CString strTemp(ShiftCash.GetNextOperator().GetString());
		Rs.SetFieldValue(_T("NextOperator"), strTemp);
		Rs.SetFieldValue(_T("Description"), ShiftCash.GetDescription());
		Rs.SetFieldValue(_T("EndDate"), ShiftCash.GetEndDate());
		Rs.SetFieldValue(_T("CashRegisterID"), ShiftCash.GetCashRegisterID());
		Rs.SetFieldValue(_T("remainAmount"), ShiftCash.GetRemainAmount());
		Rs.SetFieldValue(_T("Payout"), ShiftCash.GetPayout());
		Rs.SetFieldValue(_T("CountAmount"), ShiftCash.GetCountAmount());
		//Rs.SetFieldValue(_T("NextOperator"), ShiftCash.GetNextOperator());

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddCreditReverseInfo(CCreditInfoDB& CreditInfo)
{
	CString strSQL(_T("select * from credited "));

	strSQL.AppendFormat(_T("where memberid = %d and operator = '%s' and refno = %d"),
		CreditInfo.GetMemberId(), CreditInfo.GetOperator(), CreditInfo.GetRefNo());

	BOOL bRet = FALSE;

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb, TRUE)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0)
	{
		UINT nTmp = 0; 

		if (Rs.GetFieldValue(_T("creditAmount"), nTmp))
		{
			CreditInfo.SetCreditAmount(nTmp);
		}

		if (Rs.GetFieldValue(_T("creditPresent"), nTmp))
		{
			CreditInfo.SetCreditPresent(nTmp);
		}

		if (Rs.GetFieldValue(_T("serialNo"), nTmp))
		{
			CreditInfo.SetSerialNo(nTmp);
		}
		
		if (Rs.GetFieldValue(_T("point"), nTmp))
		{
			CreditInfo.SetPoint(nTmp);
		}

		Rs.AddNew();

		Rs.SetFieldValue(_T("creditDate"), CreditInfo.GetCreditDate());
		Rs.SetFieldValue(_T("netBarId"), CreditInfo.GetNetBarId());
		Rs.SetFieldValue(_T("refNo"), CreditInfo.GetRefNo());
		Rs.SetFieldValue(_T("serialNo"), CreditInfo.GetSerialNo());
		Rs.SetFieldValue(_T("memberId"), CreditInfo.GetMemberId());
		Rs.SetFieldValue(_T("creditAmount"), 0 - CreditInfo.GetCreditAmount());
		Rs.SetFieldValue(_T("point"), CreditInfo.GetPoint());
		Rs.SetFieldValue(_T("creditPresent"), 0 - CreditInfo.GetCreditPresent());
		Rs.SetFieldValue(_T("operator"), CreditInfo.GetOperator());
		Rs.SetFieldValue(_T("CashRegisterID"), CreditInfo.GetCashRegisterID());
		Rs.SetFieldValue(_T("week"), CreditInfo.GetWeek());
		Rs.SetFieldValue(_T("classId"), CreditInfo.GetCassId());
		Rs.SetFieldValue(_T("status"), CreditInfo.GetStatus());

		bRet = Rs.Update();
		
		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::GetTermLastUser(CString strTermId, UINT& nMemberId, CString& strNetId)
{
	BOOL bRet = FALSE;

	nMemberId = 0;

	CString strSQL;

	strSQL.Format(_T("SELECT activemember.memberId,member.serialNum FROM activemember, member WHERE activemember.memberId = member.memberId and termId = '%s' order by checkinTime desc"), strTermId);

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0)
	{
		bRet = TRUE;

		Rs.GetFieldValue(_T("memberId"), nMemberId);
		Rs.GetFieldValue(_T("serialNum"), strNetId);

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::GetNetBarUserInfo(UINT nMemberId, CNetBarUser & NetBarUser)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL.Format(_T("SELECT * FROM member WHERE memberId = %d"), nMemberId);

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0)
	{
		bRet = TRUE;

		CString strTmp;

		Rs.GetFieldValue(_T("name"), strTmp);
		NetBarUser.SetUserName(strTmp);

		Rs.GetFieldValue(_T("serialNum"), strTmp);
		NetBarUser.SetNetId(strTmp);

		Rs.GetFieldValue(_T("idNumber"), strTmp);
		NetBarUser.SetCertificateID(strTmp);

		Rs.Close();

		bRet = TRUE;
	}

	pDb->Release();

	return bRet;
}


BOOL CIBADAL::GetMemberId(CString strNetId, UINT &nMemberId)
{
	nMemberId = 0;

	BOOL bRet = FALSE;

	if (strNetId.GetLength() < 10)
	{
		strNetId.Insert(0, CString(_T("0000000000")).Left(10 - strNetId.GetLength()));
	}

	CString strSQL;

	strSQL.Format(_T("SELECT * FROM member WHERE serialNum = '%s'"), strNetId);

	//IBA_LOG(_T("SQL"),strSQL);
	IBA_LOG(_T("CIBADAL::GetMemberId sql=%s"),strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) && Rs.GetRecordCount() > 0)
	{
		bRet = TRUE;

		Rs.GetFieldValue(_T("memberId"), nMemberId);

		Rs.Close();

		bRet = TRUE;
	}

	pDb->Release();

	return bRet;
}
//����˺ż�¼
BOOL CIBADAL::SetAutoNetIdFromRecord(CString newValue)
{
	//�������ݿ�
	CConfigInfo configInfo;
	configInfo.Clear();
	configInfo.SetKeyString(_T("AutoNetId"));
	configInfo.SetKeyValue1(_tstoi(newValue));
	configInfo.SetKeyName(_T("AutoNetId"));
	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);

	return TRUE;

}

//�Զ����ɵ��˺�
CString CIBADAL::GetAutoNetIdFromRecord()
{//��ȡ���ݿ�
	
	CConfigInfo configInfo;
	configInfo.SetKeyString(_T("NetIdIncrement"));
	
	CIBADAL::GetInstance()->GetSystemConfig(configInfo);

	INT nNetId = 0;//����λ��5λ
	
	if (configInfo.GetKeyValue1() >= 99999)
	{
		nNetId = 0;
	} 
	else
	{
		nNetId = configInfo.GetKeyValue1();
	}

	//�������ݿ�
	configInfo.Clear();
	configInfo.SetKeyString(_T("NetIdIncrement"));
	configInfo.SetKeyValue1(nNetId + 1);
	configInfo.SetKeyName(_T("NetIdIncrement"));
	configInfo.SetKeyType(_T("xxxx"));

	CIBADAL::GetInstance()->UpdateSystemConfig(configInfo, 1);

	//�����˺�
	CString strNetId;

	strNetId.Format(_T("%d%05d"), 
		CNetBarConfig::GetInstance()->GetNetBarId(),
		nNetId + 1);

	return strNetId;

}

//��LastUser����ɾ���ض��ն���
BOOL CIBADAL::DeleteLastUserInfo(CString strTermId)
{
	BOOL bRet = FALSE;

	CString strSQL;
	//��ʾȡһ����¼���ṹ��ԭ����ͬ����ȫ��Ϊ�գ�һ������д��¼��1=1ָȡ���м�¼
	strSQL.Append(_T("delete from lastuser "));

	if (!strTermId.IsEmpty())
	{
		strSQL.AppendFormat(_T("where lastuser.termId = '%s' "), strTermId);
	}

	CADODBConnInfo* pDb = NULL;

	//д��־
	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("DeleteLastUserInfo:%s\r\n"), strSQL);

	bRet = ExecuteSQL(strSQL);

	return bRet;
}

//ȡ�õ�ǰ���ߵ��û���Ϣ
BOOL CIBADAL::UpdateLastUserInfo(CString strTermId, CString strUpdateTime, UINT nMemberId)
{
	BOOL bRet = FALSE;

	CString strSQL;
	//��ʾȡһ����¼���ṹ��ԭ����ͬ����ȫ��Ϊ�գ�һ������д��¼��1=1ָȡ���м�¼
	strSQL.Append(_T("update lastuser "));

	if (!strTermId.IsEmpty())
	{
		strSQL.AppendFormat(_T("set lastuser.termId = '%s' "), strTermId);
	}

	if (!strUpdateTime.IsEmpty())
	{
		strSQL.AppendFormat(_T(" ,lastuser.UpdateTime = '%s' "), strUpdateTime);
	}

	strSQL.AppendFormat(_T("where lastuser.memberId = '%ud' "), nMemberId);

	CADODBConnInfo* pDb = NULL;

	//д��־
	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("UpdateLastUserInfo:%s\r\n"), strSQL);

	bRet = ExecuteSQL(strSQL);

	return bRet;
}

BOOL CIBADAL::AddLastUserInfo(CLastUserInfo& LastUserInfo)
{
	BOOL bRet = FALSE;

	CString strSQL;
	//��ʾȡһ����¼���ṹ��ԭ����ͬ����ȫ��Ϊ�գ�һ������д��¼��1=1ָȡ���м�¼
	strSQL.Append(_T("select * from lastuser where 1=0 "));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	//д��־
	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("AddLastUserInfo:%s"), strSQL);

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("termId"), LastUserInfo.GetTermId());
		Rs.SetFieldValue(_T("netId"), LastUserInfo.GetNetId());
		Rs.SetFieldValue(_T("password"), LastUserInfo.GetPassWord());
		Rs.SetFieldValue(_T("UpdateTime"), LastUserInfo.GetUpdateTime());
		Rs.SetFieldValue(_T("memberId"), LastUserInfo.GetMemberId());
		Rs.SetFieldValue(_T("suspend"), LastUserInfo.GetSuspend());
		Rs.SetFieldValue(_T("loginStyle"), LastUserInfo.GetLoginStyle());

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

/*************************************************
Function:       // GetLastUserInfo
Description:    // ��lastuser�в�ѯ��¼
Calls:          // 
Table Accessed: // lastuser
Table Updated:  // lastuser
Input:          // nMemberId - ��������룬������MemberId��ѯ
                // strTermId - ���û��MemberId����strTermId��ѯ
Output:         // LastUserInfo - ȡ��ѯ���ĵ�һ����¼����Ϣ���浽����
Return:         // ��ѯ�ɹ�����TRUE�����򷵻�FALSE
Others:         // None
*************************************************/
BOOL CIBADAL::GetLastUserInfo(CLastUserInfo& LastUserInfo, UINT nMemberId, CString strTermId)
{
	BOOL bRet = FALSE;

	CString strSQL(_T("SELECT lastuser.* from lastuser "));
	//������ѯ��������
	if (nMemberId > 0)
	{
		strSQL.AppendFormat(_T("where lastuser.memberId = '%d' "), nMemberId);
	}

	if (!strTermId.IsEmpty())
	{
		strSQL.AppendFormat(_T("where lastuser.termId = '%s' "), strTermId);
	}

	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("GetLastUserInfo:%s"), strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{//ȡ��һ���鵽�ļ�¼
		if (Rs.GetRecordCount() > 0)
		{
			CString strTmp;
			UINT nTmp = 0;

			Rs.GetFieldValue(_T("termId"), strTmp);
			LastUserInfo.SetTermId(strTmp);

			Rs.GetFieldValue(_T("netId"), strTmp);
			LastUserInfo.SetNetId(strTmp);

			Rs.GetFieldValue(_T("password"), strTmp);
			LastUserInfo.SetPassWord(strTmp);

			Rs.GetFieldValue(_T("UpdateTime"), strTmp);
			LastUserInfo.SetUpdateTime(strTmp);

			Rs.GetFieldValue(_T("memberId"), nTmp);
			LastUserInfo.SetMemberId(nTmp);

			if (Rs.GetFieldCount() > 5)
			{
				Rs.GetFieldValue(_T("suspend"), nTmp);
				LastUserInfo.SetSuspend(nTmp); //�Ƿ�һ���
			}

			bRet = TRUE;
		}
		Rs.Close();
	}

	pDb->Release();

	return bRet;
}


//ȫ���˿����ݿ����

//���µ�ǰ���ߵ��û���Ϣ
BOOL CIBADAL::UpdateCheckOutInfo(CString strNetId, UINT nReturnState)
{
	BOOL bRet = FALSE;

	CString strSQL;
	//��ʾȡһ����¼���ṹ��ԭ����ͬ����ȫ��Ϊ�գ�һ������д��¼��1=1ָȡ���м�¼
	strSQL.Append(_T("update CheckOutInfo "));
	strSQL.AppendFormat(_T(" set CheckOutInfo.returnState = '%d' "), nReturnState);
	strSQL.AppendFormat(_T(" where CheckOutInfo.SerialNum = '%s' "), strNetId);

	//д��־
	IBA_LOG(CIBALog::ELT_SQL, _T("UpdateCheckOutInfo:%s\r\n"), strSQL);

	bRet = ExecuteSQL(strSQL);

	return bRet;
}

/*************************************************
Function:       // DeleteAllCheckOutInfo
Description:    // ɾ��uploadInfo�����ļ���ɾ��CheckOutInfo��
Table Updated: // ɾ��CheckOutInfo
Return:         // ɾ����ɹ��򷵻�TRUE
*************************************************/
BOOL CIBADAL::DeleteAllCheckOutInfo()
{
	CString strFileName = theApp.GetWorkPath() +  _T("\\IBAConfig\\UploadInfo.dat");

	DeleteFile(strFileName);

	CString strSQL(_T("TRUNCATE CheckOutInfo"));

	return ExecuteSQL(strSQL);
}

/*************************************************
Function:       // AddCheckOutInfo
Description:    // ���ݲ���CheckOutInfo��CheckOutInfo���в����¼�¼(������ж�Ӧ��¼����ɾ��)
Table Accessed: // CheckOutInfo
Table Updated: //  CheckOutInfo - ������¼�¼
Input:          // CheckOutInfo���ڴ����¼�¼
Return:         // ��ӳɹ�����TRUE
*************************************************/
BOOL CIBADAL::AddCheckOutInfo(CCheckOutinfo& CheckOutinfo)
{
	IBA_LOG(_T("CIBADAL::AddCheckOutInfo Start - For MemberId = %d"),(int)CheckOutinfo.GetMemberId());
	BOOL bRet = FALSE;

	CString strSQL;
	strSQL.AppendFormat(_T("select * from CheckOutInfo where memberId=%d"), (INT)CheckOutinfo.GetMemberId());

	CADODBConnInfo* pDb = NULL;
	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	//IBA_LOG(_T("TempUse.CIBADAL::AddCheckOutInfo: Ŀ  "))
	//CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("AddCheckOutInfo:%s\r\n"), strSQL);

	if (Rs.Open(strSQL) ) //������ҵ���¼��ɾ����¼
	{
		if (Rs.GetRecordCount() > 0)
		{
			if (!Rs.Delete())
			{
				IBA_LOG0(_T("CheckOutInfo�������ظ���¼��ɾ��ʧ��"));
				Rs.Close();
				pDb->Release();
				return FALSE;
			}
			if (!Rs.Update())
			{
				IBA_LOG0(_T("CheckOutInfo�������ظ���¼��ɾ������ʧ��"));
				Rs.Close();
				pDb->Release();
				return FALSE;
			}
		}	
	}
	else
	{
		pDb->Release();
		return FALSE;
	}

	if ( Rs.AddNew())   //����¼�¼
	{
		Rs.SetFieldValue(_T("checkOutTime"), CheckOutinfo.GetCheckOutTime());
		Rs.SetFieldValue(_T("SerialNum"), CheckOutinfo.GetSerialNum());
		Rs.SetFieldValue(_T("MemberId"), CheckOutinfo.GetMemberId());
		Rs.SetFieldValue(_T("ClassId"), CheckOutinfo.GetClassId());
		Rs.SetFieldValue(_T("BalanceMoney"), CheckOutinfo.GetBalanceMoney());
		Rs.SetFieldValue(_T("classState"), CheckOutinfo.GetClassState());
		Rs.SetFieldValue(_T("ClassName"), CheckOutinfo.GetClassName());
		Rs.SetFieldValue(_T("terminalId"), CheckOutinfo.GetTerminalId());
		Rs.SetFieldValue(_T("GuarantyMoney"), CheckOutinfo.GetGuarantyMoney());
		Rs.SetFieldValue(_T("UserName"), CheckOutinfo.GetUserName());
		Rs.SetFieldValue(_T("PersonalId"), CheckOutinfo.GetPersonalId());
		Rs.SetFieldValue(_T("returnState"), CheckOutinfo.GetReturnState());

		bRet = Rs.Update();		
		Rs.Close();

		IBA_LOG(_T("TempUse.CIBADAL::AddCheckOutInfo: ����¼�¼��CheckOutInfo�� - nMemberId = %d"),CheckOutinfo.GetMemberId());
	}

	pDb->Release();

	IBA_LOG(_T("CIBADAL::AddCheckOutInfo Result - %d"),(int)bRet);
	return bRet;
}

/*************************************************
Function:       // GetCheckOutInfo
Description:    // ��ȡ���˼�¼�����浽CheckOutArray��
Calls:          // None
Table Accessed: // CheckOutInfo
Table Updated: // None
Input:          // CheckOutArray�����ڱ����ȡ�ļ�¼
Output:         // None
Return:         // ���ݿ��ѯ�ɹ�������TRUE.���򷵻�FALSE
Others:         // None
*************************************************/
BOOL CIBADAL::GetCheckOutInfo(CCheckOutInfoArray& CheckOutArray)
{
	//IBA_LOG(_T("CIBADAL ::GetCheckOutInfo Start - select * from CheckOutInfo"));
	BOOL bRet = FALSE;

	CString strSQL(_T("SELECT * from CheckOutInfo "));              //��CheckOutInfo�л�ȡ���м�¼
	strSQL.Append(_T("order by CheckOutInfo.SerialNum desc "));

	//CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("GetCheckOutInfo:%s"), strSQL);

	CheckOutArray.RemoveAll();

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		for (UINT i = 0; i < Rs.GetRecordCount(); i++)
		{
			CCheckOutinfo CheckOutinfo;

			CString strTmp; 
			INT nTmp = 0;

			Rs.GetFieldValue(_T("checkOutTime"), strTmp);
			CheckOutinfo.SetCheckOutTime(strTmp);
			Rs.GetFieldValue(_T("SerialNum"), strTmp);
			CheckOutinfo.SetSerialNum(strTmp);
			Rs.GetFieldValue(_T("ClassName"), strTmp);
			CheckOutinfo.SetClassName(strTmp);
			Rs.GetFieldValue(_T("TerminalId"), strTmp);
			CheckOutinfo.SetTerminalId(strTmp);
			Rs.GetFieldValue(_T("UserName"), strTmp);
			CheckOutinfo.SetUserName(strTmp);
			Rs.GetFieldValue(_T("PersonalId"), strTmp);
			CheckOutinfo.SetPersonalId(strTmp);

			Rs.GetFieldValue(_T("MemberId"), nTmp);
			CheckOutinfo.SetMemberId(nTmp);
			Rs.GetFieldValue(_T("ClassId"), nTmp);
			CheckOutinfo.SetClassId(nTmp);
			Rs.GetFieldValue(_T("BalanceMoney"), nTmp);
			CheckOutinfo.SetBalanceMoney(nTmp);
			Rs.GetFieldValue(_T("classState"), nTmp);
			CheckOutinfo.SetClassState(nTmp);
			Rs.GetFieldValue(_T("GuarantyMoney"), nTmp);
			CheckOutinfo.SetGuarantyMoney(nTmp);
			Rs.GetFieldValue(_T("returnState"), nTmp);
			CheckOutinfo.SetReturnState(nTmp);

			CheckOutArray.Add(CheckOutinfo);

			bRet = TRUE;

			Rs.MoveNext();
		}

		Rs.Close();
	}

	pDb->Release();
	//CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("GetCheckOutInfoEnd"));
	IBA_LOG(_T("CIBADAL ::GetCheckOutInfo Result - %d"),(int)bRet);

	return bRet;
}

BOOL CIBADAL::AddOperatorRecord(CString& strOperator)
{
	BOOL bRet = FALSE;

	CString strSQL;
	//��ʾȡһ����¼���ṹ��ԭ����ͬ����ȫ��Ϊ�գ�һ������д��¼��1=1ָȡ���м�¼
	strSQL.Append(_T("select * from netbarcashier where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	//д��־
	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("AddOperatorRecord:%s"), strSQL);

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("userId"), strOperator);

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::GetOperatorRecord(COperatorArray& OperatorArray)
{
	BOOL bRet = FALSE;

	CString strSQL(_T("SELECT * from netbarcashier "));
	strSQL.Append(_T("order by netbarcashier.userId desc "));

	//CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("GetOperatorRecord:%s"), strSQL);

	OperatorArray.RemoveAll();

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		for (UINT i = 0; i < Rs.GetRecordCount(); i++)
		{
			CString strOperator;

			CString strTmp; INT nTmp = 0;

			Rs.GetFieldValue(_T("userId"), strOperator);

			OperatorArray.Add(strOperator);

			bRet = TRUE;

			Rs.MoveNext();
		}

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::AddClientShortMsg(CClientShortMessage& ClientShortMessage)
{
	BOOL bRet = FALSE;

	CString strSQL;
	//��ʾȡһ����¼���ṹ��ԭ����ͬ����ȫ��Ϊ�գ�һ������д��¼��1=1ָȡ���м�¼
	strSQL.Append(_T("select * from shortmessage where 1=0"));

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	//д��־
	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("AddClientShortMsg:%s"), strSQL);

	if (Rs.Open(strSQL) && Rs.AddNew())
	{
		Rs.SetFieldValue(_T("id"), ClientShortMessage.GetMsgID());
		Rs.SetFieldValue(_T("ip"), ClientShortMessage.GetTermIP());
		Rs.SetFieldValue(_T("TermID"), ClientShortMessage.GetTermID());
		Rs.SetFieldValue(_T("MsgBody"), ClientShortMessage.GetMsgBody());
		Rs.SetFieldValue(_T("SendTime"), ClientShortMessage.GetRevTime());

		bRet = Rs.Update();

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}

BOOL CIBADAL::GetClientShortMsg(CClientShortMsgArray& MsgArray, CString strStart, CString strEnd)
{
	BOOL bRet = FALSE;

	CString strSQL(_T("SELECT *  "));
	strSQL.Append(_T(" from shortmessage A "));
	strSQL.Append(_T(" where A.side = 0 "));
	strSQL.AppendFormat(_T("AND A.sendtime between '%s' and '%s' "), strStart, strEnd);
	strSQL.Append(_T("order by A.sendtime "));

	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("GetClientShortMsg:%s"), strSQL);

	MsgArray.RemoveAll();

	CADODBConnInfo* pDb = NULL;

	if (!GetDBConnInfo(pDb)) return bRet;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL))
	{
		for (UINT i = 0; i < Rs.GetRecordCount(); i++)
		{
			CClientShortMessage MsgInfo;

			CString strTmp; INT nTmp = 0;

			Rs.GetFieldValue(_T("MemberId"), nTmp);
			MsgInfo.SetMemberId(nTmp);

			Rs.GetFieldValue(_T("id"), nTmp);
			MsgInfo.SetMsgID(nTmp);

			Rs.GetFieldValue(_T("ip"), strTmp);
			MsgInfo.SetTermIP(strTmp);

			Rs.GetFieldValue(_T("TermID"), strTmp);
			MsgInfo.SetTermID(strTmp);

			Rs.GetFieldValue(_T("MsgType"), nTmp);
			MsgInfo.SetMsgType(nTmp);

			Rs.GetFieldValue(_T("MsgBody"), strTmp);
			MsgInfo.SetMsgBody(strTmp);

			Rs.GetFieldValue(_T("SendTime"), strTmp);
			MsgInfo.SetRevTime(strTmp);

			MsgArray.Add(MsgInfo);

			bRet = TRUE;

			Rs.MoveNext();
		}

		Rs.Close();
	}

	pDb->Release();

	return bRet;
}
/***************************************************************************************************************************************************
Function:       //UpdateAllowReturn
Description:    //���ӻ����AllowReturn���¼. ��DoBalance��ִ��.�����Ӧuserclass���ݴ��ڣ�����£��������һ���¼�¼
Table Updated:  //AllowReturn
Input:          //UserClass
                //AllowReturn
				//IsMember
Return:         //ʧ���򷵻�FALSE
***************************************************************************************************************************************************/
BOOL CIBADAL::UpdateAllowReturn(UINT UserClass,UINT AllowReturn,BOOL IsMember)
{
	BOOL bRet = false;

	CString strSQL;
	CADODBConnInfo* pDb = NULL;
	if (!GetDBConnInfo(pDb)) return bRet;
	CADORecordset Rs(pDb->GetConn());

	//�Ȳ鿴�����Ƿ��ж�Ӧ��¼
	int nCount = 0;
	strSQL.Format(_T("select count(*) from AllowReturn where userclass = %d"),UserClass);

	if (Rs.Open(strSQL))
	{
		Rs.GetFieldValue(_T("count(*)"),nCount);
	}
	Rs.Close();

	//��-���¼�¼
	if(nCount > 0)
	{
		strSQL.Format(_T("update allowreturn set allowreturn = %d,IsMember = %d where userclass = %d"),AllowReturn,IsMember,UserClass);
		if(Rs.Open(strSQL))
		{
			bRet = true;
		}
		Rs.Close();
	}
	//��-���Ӽ�¼
	else
	{
		strSQL.Format(_T("insert into allowreturn set userclass = %d,allowreturn = %d,IsMember = %d"),UserClass,AllowReturn,IsMember);
		if(Rs.Open(strSQL))
		{
			bRet = true;
		}
		Rs.Close();
	}
	pDb->Release();

	return bRet;
}
/***************************************************************************************************************************************************
Function:       //GetAllowReturn
Description:    //��AllowReturn���л�ȡֵ
Table Accessed: //AllowReturn
Input:          //UserClass
Output:			//pIsMember - �Ƿ�Ϊ��Ա,0-�ǻ�Ա,1-��Ա
Return:         //ʧ���򷵻�-1���ɹ�����AllowReturn����
***************************************************************************************************************************************************/
int CIBADAL::GetAllowReturn(UINT UserClass, UINT *pIsMember /*= NULL*/)
{
	int bRet = -1;

	CString strSQL;
	CADODBConnInfo *pDb = NULL;
	if(!GetDBConnInfo(pDb)) return bRet;
	CADORecordset Rs(pDb->GetConn());

	strSQL.Format(_T("select * from allowreturn where userclass = %d"),UserClass);

	if(Rs.Open(strSQL))
	{
		if(Rs.GetRecordCount() > 0)
		{
			Rs.GetFieldValue(_T("allowreturn"),bRet);
			if(pIsMember != NULL)
			{
				UINT IsMember;
				Rs.GetFieldValue(_T("IsMember"),IsMember);
				*pIsMember = IsMember;
			}
		}
	}
	Rs.Close();

	pDb->Release();

	return bRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_CreditInfo
Description:    //����localMember��balance,totalMoney��ֵ
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//nCreditAmount - ��ֵ���
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_CreditInfo(UINT nMemberId,UINT nCreditAmount)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0)
	{
		sql.Format(_T("update localMember set balance=balance+%d,totalMoney=totalMoney+%d where memberId=%d"),  nCreditAmount, nCreditAmount, nMemberId);
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_RateInfo
Description:    //����localMember��Rate
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//strRate - ����
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_RateInfo(UINT nMemberId,CString strRate)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0)
	{
		sql.Format(_T("update localMember set rate = '%s' where memberId=%d"), strRate, nMemberId);
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_PCClassId
Description:    //����localMember��PCClassId
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//PDClassId - ����ID
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_PCClassId(UINT nMemberId,UINT nPCClassId)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0 && nPCClassId > 0)
	{
		sql.Format(_T("update localMember set pcClass = %d where memberId=%d"), nPCClassId, nMemberId);
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_CheckInInfo
Description:    //����localMember��TermId,checkinTime,lastupdatetime
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//strTermId - �ն�ID
				//checkintime - ���ص�¼ʱ��
				//localCheckintime - ��ѡ.Ĭ����Чֵ
				//bIsLocalCheckin - �Ƿ񼤻���һ��½���ڱ��ص�¼
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_CheckInInfo(UINT nMemberId,CString strTermId,COleDateTime localCheckintime,COleDateTime Checkintime /*= COleDateTime(0,0,0,0,0,0)*/,BOOL bIsLocalCheckin/*=FALSE*/)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	CString strLocalCheckInTime,strCheckinTime;
	strLocalCheckInTime = localCheckintime.Format(_T("%Y-%m-%d %H:%M:%S"));
	strCheckinTime = Checkintime.Format(_T("%Y-%m-%d %H:%M:%S"));

	SYSTEMTIME systime;
	localCheckintime.GetAsSystemTime(systime);
	CTime ctime(systime);
	int nLocalcheckinTime = ctime.GetTime();
	
	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0)//Note:LastUpdateTime = localCheckinTime
	{
		if(CIBAHelpper::IsValidTime(Checkintime) && Checkintime > COleDateTime(2015,1,1,0,0,0))
		{
			sql.Format(_T("update localMember set localCheckinTime='%s',termId = '%s',checkinTime = '%s', lastUpdateTime = %d, isLocalCheckin = %d where memberId=%d"), strLocalCheckInTime,strTermId, strCheckinTime,nLocalcheckinTime,bIsLocalCheckin,nMemberId);
		}
		else
		{
			sql.Format(_T("update localMember set localCheckinTime='%s',termId = '%s',lastUpdateTime = %d,isLocalCheckin = %d where memberId=%d"), strLocalCheckInTime,strTermId, nLocalcheckinTime,bIsLocalCheckin,nMemberId);
		}		
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_CheckInInfo
Description:    //����localMember��TermId,checkinTime
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//strTermId - �ն�ID
				//checkintime - ���ص�¼ʱ��
				//localCheckintime - ��ѡ.Ĭ����Чֵ
				//bIsLocalCheckin - �Ƿ񼤻���һ��½���ڱ��ص�¼
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_PayTypeInfo(UINT nMemberId,UINT nPayType,int nextCheckinTime,BOOL bUpdateLastUpdateTime /*= TRUE*/)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	//CString strNextCheckinTime;
	//strNextCheckinTime = nextCheckinTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	//CString strLastUpdateTime;
	//COleDateTime lastUpdateDateTime = COleDateTime::GetCurrentTime();
	//strLastUpdateTime = lastUpdateDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	time_t lastUpdateDateTime = time(NULL);

	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0)
	{
		if(bUpdateLastUpdateTime)
		{
			sql.Format(_T("update localMember set payType=0,nextCheckinTime=%d,lastUpdateTime=%d where memberId=%d"),nPayType,(int)nextCheckinTime,(int)lastUpdateDateTime,nMemberId);
		}
		else
		{
			sql.Format(_T("update localMember set payType=0,nextCheckinTime=%d where memberId=%d"), nPayType,(int)nextCheckinTime,nMemberId);
		}		
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_BalanceInfo
Description:    //����localMember��Balance,LastUpdateTime
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//nNewBalance - �����
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_BalanceInfo(UINT nMemberId,UINT nNewBalance)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0)
	{
		time_t now = time(NULL);
		sql.Format(_T("update LocalMember set balance=%d,lastUpdateTime=%d where memberId=%d"), nNewBalance,now, nMemberId);
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}

/***************************************************************************************************************************************************
Function:       //UpdateLocalMember_CheckOutInfo
Description:    //����localMember��isReturned,CheckoutTime
Table Accessed: //LocalMember
Input:          //nMemberId - �û���
				//CheckOutTime - �ǳ�ʱ��
				//bIsReturned - �Ƿ�ǳ�
Return:         //ʧ���򷵻�-1���ɹ�����Ӱ�������.Note:������ǰ��ֵ����,������0.
***************************************************************************************************************************************************/
int CIBADAL::UpdateLocalMember_CheckOutInfo(UINT nMemberId,COleDateTime CheckOutTime,BOOL bIsReturned)
{
	int nRet = -1;
	//��ѯlocalMember���Ƿ������û���¼
	CString sql;
	sql.Format(_T("select * from LocalMember where  memberId=%d"), nMemberId);
	GxxArrayPtr membersPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	CString strCheckoutTime;
	strCheckoutTime = CheckOutTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	GxxDictionaryPtr memberPtr;
	if(membersPtr->count() > 0)
	{
		time_t now = time(NULL);
		sql.Format(_T("update localMember set checkoutTime='%s',isReturned=%d where memberId=%d"), strCheckoutTime,(int)bIsReturned, nMemberId);
		nRet = CIBADAL::GetInstance()->ExecuteSQLEx(sql);
	}
	return nRet;
}