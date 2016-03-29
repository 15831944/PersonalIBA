#include "stdafx.h"
#include "HelpFunc.h"
#include "..\IBA\DAL\IBADAL.h"
#include "IBA.h"

// �ҳ���ʼСʱ������Сʱ֮�����Ŀ���
int HELPFUNC::GetMaxCostRateImp(int nBeginHour, int nEndHour, int nClassId, int nWeek, int nPCClass )
{
	INT iCostRate = -1;
	INT iCostRateTmp = -1;
	for(; nBeginHour <= nEndHour; nBeginHour++)
	{
		CString strHour;
		strHour.Format(_T("t%d"), nBeginHour);

		CString strSQL;
		if(-1 == nPCClass)
		{
			strSQL.Format(_T("SELECT %s FROM netbarbasepolicy WHERE classId = %d AND  week = %d ORDER BY %s DESC"), 
				strHour, nClassId, nWeek, strHour );
		}
		else
		{
			strSQL.Format(_T("SELECT %s FROM netbarbasepolicy WHERE classId = %d AND  week = %d AND pcClass = %d ORDER BY %s DESC"), 
				strHour, nClassId, nWeek, nPCClass, strHour );
		}

		//try
		//{
		CADODBConnInfo* pDb = NULL;

		if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) 
			return -1;

		CADORecordset Rs(pDb->GetConn());

		if (Rs.Open(strSQL) )
		{
			if (0 != Rs.GetRecordCount())
			{				
				Rs.GetFieldValue(strHour, iCostRateTmp);
			}
		}		
		Rs.Close();
		pDb->Release();

		if(iCostRate < iCostRateTmp)
		{
			iCostRate = iCostRateTmp;
		}
		//}
		//catch(...)
		//{
		//	IBA_LOG0(_T("��ѯ�����쳣"));
		//	return iCostRate;
		//}
	}
	return iCostRate;
}


int HELPFUNC::GetMaxCostRate(int nBeginHour, int nEndHour, int nClassId, int nPCClass)
{
	COleDateTime dtCurrent = COleDateTime::GetCurrentTime();
	// ��ʱ��ʼʱ��С�ڽ���ʱ�䣬Ҳ���ǿ����ʱ
	if(nBeginHour > nEndHour)
	{
		// ���������ʼ���
		int nCostRateTmp1 = HELPFUNC::GetMaxCostRateImp(nEndHour, 23, nClassId, dtCurrent.GetDayOfWeek(), nPCClass);
		int nCostRateTmp2 = HELPFUNC::GetMaxCostRateImp(0, nBeginHour, nClassId,
			(dtCurrent.GetDayOfWeek() + 1 > 7 ) ? 1 : dtCurrent.GetDayOfWeek() + 1, nPCClass);
		int nCostRate = (nCostRateTmp1 > nCostRateTmp2) ? nCostRateTmp1 : nCostRateTmp2;
		return nCostRate;
	}
	else
	{
		return HELPFUNC::GetMaxCostRateImp(nBeginHour, nEndHour, nClassId, dtCurrent.GetDayOfWeek(), nPCClass);
	}
}

// ��ȡ����ʱ��ʼʱ������Ҫ��Ǯ�����ݿ�ʼʱ��ͽ���ʱ�䣬���ؽ�Ǯ(��λ��)
// ��Ҫ��Ϊ�˼����ϴ���
BOOL HELPFUNC::GetMoneyToBundleTimeNotPCClass(CString &strStartTime, CString &strEndTime, int nUserClassID, int &nMoney)
{
	// ���ֵ������, Сʱ:���ӵĻ��Ͳ�������
	if(5 == strStartTime.GetLength())
	{
		strStartTime += _T(":00");
	}

	if(5 == strEndTime.GetLength())
	{
		strEndTime += _T(":00");
	}

	COleDateTime dtBegin, dtEnd, dtCurrent;
	CString strCurrent;
	dtCurrent = COleDateTime::GetCurrentTime();
	strCurrent.Format(_T("%d:%d:%d"), dtCurrent.GetHour(), dtCurrent.GetMinute(), dtCurrent.GetSecond()); 
	dtCurrent.ParseDateTime(strCurrent);	// ȥ������

	dtBegin.ParseDateTime(strStartTime);
	dtEnd.ParseDateTime(strEndTime);

	// 0:������ǰ��ʱ�� 1:������ǰ��ʱ
	int iToNextDay = -1;

	COleDateTimeSpan dtSpan;
	if(dtCurrent < dtBegin)
	{
		// ��ͨ��ǰ��ʱ
		iToNextDay = 0;

		dtSpan.SetDateTimeSpan(0, 
			dtBegin.GetHour() - dtCurrent.GetHour(),
			dtBegin.GetMinute() - dtCurrent.GetMinute(),
			dtBegin.GetSecond() - dtCurrent.GetSecond());
	}
	else if(dtCurrent > dtEnd)
	{
		// ��ǰʱ����ڰ�ʱ����ʱ�䣬 ���ǰ����ڶ���
		// ������ǰ��ʱ
		iToNextDay = 1;

		dtSpan.SetDateTimeSpan(0, 
			23 - dtCurrent.GetHour() + dtBegin.GetHour(),
			59 - dtCurrent.GetMinute(),
			59 - dtCurrent.GetSecond());
	}
	else
	{
		// ��ǰСʱ�ڵ�ǰ��ʱʱ���,���ý�������
		nMoney = 0;
		return TRUE;
	}

	int iCurrHour = dtCurrent.GetHour();
	int iBeginHour = dtBegin.GetHour();
	int iSpanHour = dtSpan.GetHours();

	if((iCurrHour >= 0 && iCurrHour <= 23) &&
		(iBeginHour >= 0 && iBeginHour  <= 23))
	{
		// ��ȡ��ʼ��ǰʱ�䵽��ʱʱ���֮���������
		int iCostRate = HELPFUNC::GetMaxCostRate(iCurrHour, iBeginHour, nUserClassID);

		if(0 <= iCostRate)
		{	
			// ������ = ���ʵ����ֵ *��ʱ��
			double dTime = dtSpan.GetHours() + (double)dtSpan.GetMinutes() / 60 + (double)dtSpan.GetSeconds() / 3600;
			double dCost = iCostRate * dTime / 100;

			int iCost = (int)dCost;
			if(dCost > iCost)
			{
				iCost++;		// ȡ��������Ϊ5.2Ԫʱ����ȡ��6Ԫ
			}

			nMoney = iCost * 100;

			return TRUE;
		}
		else
		{
			IBA_LOG0(_T("��ѯ���ʳ���"));
		}
	}

	nMoney = 0;
	return FALSE;
}

BOOL HELPFUNC::GetMoneyToBundleTime( CString &strStartTime, CString &strEndTime, int nUserClassID, int nPCClass, int &nMoney )
{
	// ���ֵ������, Сʱ:���ӵĻ��Ͳ�������
	if(5 == strStartTime.GetLength())
	{
		strStartTime += _T(":00");
	}

	if(5 == strEndTime.GetLength())
	{
		strEndTime += _T(":00");
	}

	COleDateTime dtBegin, dtEnd, dtCurrent;
	CString strCurrent;
	dtCurrent = COleDateTime::GetCurrentTime();
	strCurrent.Format(_T("%d:%d:%d"), dtCurrent.GetHour(), dtCurrent.GetMinute(), dtCurrent.GetSecond()); 
	dtCurrent.ParseDateTime(strCurrent);	// ȥ������

	dtBegin.ParseDateTime(strStartTime);
	dtEnd.ParseDateTime(strEndTime);

	// 0:������ǰ��ʱ�� 1:������ǰ��ʱ
	int iToNextDay = -1;

	COleDateTimeSpan dtSpan;
	if(dtCurrent < dtBegin)
	{
		// ��ͨ��ǰ��ʱ
		iToNextDay = 0;

		dtSpan.SetDateTimeSpan(0, 
			dtBegin.GetHour() - dtCurrent.GetHour(),
			dtBegin.GetMinute() - dtCurrent.GetMinute(),
			dtBegin.GetSecond() - dtCurrent.GetSecond());
	}
	else if(dtCurrent > dtEnd)
	{
		// ��ǰʱ����ڰ�ʱ����ʱ�䣬 ���ǰ����ڶ���
		// ������ǰ��ʱ
		iToNextDay = 1;

		dtSpan.SetDateTimeSpan(0, 
			23 - dtCurrent.GetHour() + dtBegin.GetHour(),
			59 - dtCurrent.GetMinute(),
			59 - dtCurrent.GetSecond());
	}
	else
	{
		// ��ǰСʱ�ڵ�ǰ��ʱʱ���,���ý�������
		nMoney = 0;
		return TRUE;
	}

	int iCurrHour = dtCurrent.GetHour();
	int iBeginHour = dtBegin.GetHour();
	int iSpanHour = dtSpan.GetHours();

	if((iCurrHour >= 0 && iCurrHour <= 23) &&
		(iBeginHour >= 0 && iBeginHour  <= 23))
	{
		// ��ȡ��ʼ��ǰʱ�䵽��ʱʱ���֮���������
		int iCostRate = HELPFUNC::GetMaxCostRate(iCurrHour, iBeginHour, nUserClassID, nPCClass);

		if(0 <= iCostRate)
		{	
			// ������ = ���ʵ����ֵ *��ʱ��
			double dTime = dtSpan.GetHours() + (double)dtSpan.GetMinutes() / 60 + (double)dtSpan.GetSeconds() / 3600;
			double dCost = iCostRate * dTime / 100;

			int iCost = (int)dCost;
			if(dCost > iCost)
			{
				iCost++;		// ȡ��������Ϊ5.2Ԫʱ����ȡ��6Ԫ
			}

			nMoney = iCost * 100;

			return TRUE;
		}
		else
		{
			IBA_LOG0(_T("��ѯ���ʳ���"));
		}
	}

	nMoney = 0;
	return FALSE;
}

// ��ȡ����ʱ����������Ǯ
int HELPFUNC::GetMoneyToForwardBundle(double dAllBalance, int nUserClassID , int nSelBundleTimeId , int nPCClass, BOOL bIsForwardBundle)
{
	CFullBundleTimeInfo BundleTimeInfo;
	CString strBundleTimeStartTime(""), strBundleTimeEndTime("");

	int nMoneyToForwardBundle = 0;
	if(CNetBarConfig::GetInstance()->m_NetbarBundleTime.GetBundleTimeInfo(nUserClassID, nSelBundleTimeId, BundleTimeInfo))
	{
		// ��ʱ���
		UINT nBundleMoney = BundleTimeInfo.GetBundleTimeMoney();
		// ת���ɷ�
		dAllBalance *= 100;

		if(!bIsForwardBundle)		// ��ǰʱ���ڰ�ʱʱ�����
		{
			nMoneyToForwardBundle = dAllBalance - nBundleMoney;
			if(nMoneyToForwardBundle > 0)
			{
				nMoneyToForwardBundle = 0;	// Ǯ�㹻��ʱ
			}
			else
			{
				nMoneyToForwardBundle = nBundleMoney - dAllBalance;
			}


		}
		else
		{
			//��ʼʱ��
			strBundleTimeStartTime = BundleTimeInfo.GetStartTime(TRUE).Left(5);		
			//����ʱ��
			strBundleTimeEndTime = BundleTimeInfo.GetEndTime(TRUE).Left(5);

			int nMoney;

			// ��ȡ����ʱ��ʼʱ���������

			if(-1 == nPCClass)
			{
				if(!HELPFUNC::GetMoneyToBundleTimeNotPCClass(strBundleTimeStartTime, strBundleTimeEndTime, nUserClassID, nMoney))
				{
					IBA_LOG0(_T("��ǰ��ʱ����"));
				}
			}
			else
			{
				if(!HELPFUNC::GetMoneyToBundleTime(strBundleTimeStartTime, strBundleTimeEndTime, nUserClassID, nPCClass, nMoney))
				{
					IBA_LOG0(_T("��ǰ��ʱ����"));
				}
			}
			

			// ������ = �˻�ʣ���ܵĽ�� - ��ʱ����ǰʱ������Ҫ��Ǯ - ��ʱ���

			nMoneyToForwardBundle =  dAllBalance -  nMoney - nBundleMoney;

			// ����û�ʣ���ܽ��㹻��ʱ���ò���
			if(nMoneyToForwardBundle > 0)
			{
				nMoneyToForwardBundle = 0;
			}
			else
			{
				nMoneyToForwardBundle = nMoney + nBundleMoney - dAllBalance;
			}
		}

		// ���裬�����ֵ4.3Ԫ�ͳ�ֵ5Ԫ
		if(0 != nMoneyToForwardBundle % 100)
		{
			nMoneyToForwardBundle += 100;
		}
	}
	return nMoneyToForwardBundle;
}

// ��̨����"��ֹ�˻���ʱ"�� Ҳ���˻���Ǯ����������ʱ, ���ط�
// �˻���Ǯ����������ʱ�󣬵���ʱ����ʱ������Ҫ��Ǯ
int HELPFUNC::GetCashForwardBundleNoBalance(double dAllBalance, int nUserClassID,  int nSelBundleTimeId, int nPCClass, BOOL bIsForwardBundle)
{
	CFullBundleTimeInfo BundleTimeInfo;
	CString strBundleTimeStartTime(""), strBundleTimeEndTime("");
	int nMoneyToForwardBundle = 0;

	if(CNetBarConfig::GetInstance()->m_NetbarBundleTime.GetBundleTimeInfo(
		nUserClassID, nSelBundleTimeId, BundleTimeInfo))
	{
		UINT nBundleMoney = BundleTimeInfo.GetBundleTimeMoney();		// ��ʱ���

		if(!bIsForwardBundle)		// ��ǰʱ���ڰ�ʱʱ�����
		{
			// Ӧ���ֽ� = ��ʱ���
			return nBundleMoney;
		}
		else
		{
			//��ʼʱ��
			strBundleTimeStartTime = BundleTimeInfo.GetStartTime(TRUE).Left(5);
			//����ʱ��
			strBundleTimeEndTime = BundleTimeInfo.GetEndTime(TRUE).Left(5);

			CString strBundleTimeMoney;
			int nMoney;

			// ��ȡ����ʱ��ʼʱ���������
			if(-1 == nPCClass)
			{
				if(!HELPFUNC::GetMoneyToBundleTimeNotPCClass(strBundleTimeStartTime, strBundleTimeEndTime, nUserClassID, nMoney))
				{
					IBA_LOG0(_T("��ǰ��ʱ����--��ȡӦ���ֽ����"));
				}
			}
			else
			{
				if(!HELPFUNC::GetMoneyToBundleTime(strBundleTimeStartTime, strBundleTimeEndTime, nUserClassID, nPCClass, nMoney))
				{
					IBA_LOG0(_T("��ǰ��ʱ����--��ȡӦ���ֽ����"));
				}
			}

			// ת���ɷ�
			dAllBalance *= 100;
			// ����˻�ʣ���ܵĽ�� > ��ʱ����ǰʱ������Ҫ��Ǯ 
			// ��Ӧ���ֽ� = ��ʱ���

			// ����˻�ʣ���ܵĽ�� < ��ʱ����ǰʱ������Ҫ��Ǯ 
			// ��Ӧ���ֽ� = ��ʱ����ǰʱ������Ҫ��Ǯ - �˻�ʣ���ܵĽ�� + ��ʱ���
			int nDiff = dAllBalance  - nMoney;
			if(nDiff > 0)
			{
				return nBundleMoney;
			}
			else
			{
				return static_cast<int>(nBundleMoney + (-nDiff));
			}

		}

	}
	else
	{
		return 0;
	}
}

// ��ȡ��ʱ������
int HELPFUNC::GetBundleTimeMoney(int nUserClassID , int nSelBundleTimeId )
{
	CFullBundleTimeInfo BundleTimeInfo;
	CString strBundleTimeStartTime(""), strBundleTimeEndTime("");

	int nMoneyToForwardBundle = 0;
	if(CNetBarConfig::GetInstance()->m_NetbarBundleTime.GetBundleTimeInfo(
		nUserClassID, nSelBundleTimeId, BundleTimeInfo))
	{
		// ��ʱ���
		return BundleTimeInfo.GetBundleTimeMoney();

	}
	else
	{
		return 0;
	}
}
