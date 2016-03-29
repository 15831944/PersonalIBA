#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "ServletHellper.h"
#include "NetBarConfig.h"
#include ".\netbarbundletime.h"

CFullBundleTimeInfo::CFullBundleTimeInfo()
: m_nBundleTimeId(0)
, m_nBundleTimeType(0), m_nTimePass(0)
, m_nBundleTimeMoney(0), m_nClassId(0)
{

}

CString CFullBundleTimeInfo::GetStartTime(BOOL bForce /*= FALSE*/)
{
	if (bForce && m_strStartTime.IsEmpty())
	{
		COleDateTime dtTmp = COleDateTime::GetCurrentTime();
		return dtTmp.Format(_T("%H:%M:%S"));
	}

	return m_strStartTime;
}

CString CFullBundleTimeInfo::GetEndTime(BOOL bForce /*= FALSE*/)
{
	if (bForce && m_strEndTime.IsEmpty())
	{
		COleDateTime dtTmp = COleDateTime::GetCurrentTime();

		dtTmp += COleDateTimeSpan(0, m_nTimePass, 0, 0);

		return dtTmp.Format(_T("%H:%M:%S"));
	}

	return m_strEndTime;
}

CString CFullBundleTimeInfo::GetBundleTimeTypeString()
{	
	CIBAString strTmp;

	strTmp.LoadString(IDS_BUNDLETIMETYPE);

	if (1 == m_nBundleTimeType)
	{
		strTmp = strTmp.Left(strTmp.Find('|'));
	}
	else
	{
		strTmp = strTmp.Mid(strTmp.Find('|') + 1);
	}

	return strTmp;
}

BOOL CFullBundleTimeInfo::IsNowEnabled()
{
	if (m_strEndTime.IsEmpty()) //û�й̶�ʱ��İ�ʱ
	{
		return TRUE;
	}

	BOOL bNowCanBT = FALSE;

	COleDateTime StartTime, EndTime, CurTime = COleDateTime::GetCurrentTime();

	StartTime.ParseDateTime(m_strStartTime);
	StartTime.SetDateTime(CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
		StartTime.GetHour(), StartTime.GetMinute(), StartTime.GetSecond());

	EndTime.ParseDateTime(m_strEndTime);
	EndTime.SetDateTime(CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
		EndTime.GetHour(), EndTime.GetMinute(), EndTime.GetSecond());

	if (StartTime > EndTime) //�����ʱ
	{
		if (CurTime < EndTime)
		{
			StartTime -= COleDateTimeSpan(1, 0, 0, 0); //��ȥһ��
		}
		else
		{
			EndTime += COleDateTimeSpan(1, 0, 0, 0); //����һ��
		}
	}

	if (CurTime < StartTime) //��ʱ��û��ʼ
	{
		
	}
	else if (CurTime > EndTime) //��ʱ�Ѿ����ڣ�ֻ�ܿ�ʼ������İ�ʱ
	{

	}
	else //��ʱ���ڽ���
	{
		bNowCanBT = TRUE;
	}

	return bNowCanBT;
}


//******************************************************************************

CNetbarBundleTime::CNetbarBundleTime(void)
{
}

CNetbarBundleTime::~CNetbarBundleTime(void)
{
	
}

/*************************************************
Function:       // LoadBundTimeInfoFromServer
Description:    // Ϊ�û������б��и�����������Ļ�ȡ��ʱ�����б���ŵ�m_BundleTimeInfoArray��
Calls:          // CCashier::DoBundleTimeList
Table Accessed: // None
Table Updated: // None
Input:          // None
Output:         // m_BundleTimeInfoArray
Return:         // DoBundleTimeList�ɹ���ʧ�ܶ�����TRUE??
Others:         // ��ȷ��
*************************************************/
BOOL CNetbarBundleTime::LoadBundTimeInfoFromServer()
{
	m_BundleTimeInfoArray.RemoveAll();

	NS_SERVLET::CBundleTimeListMethod BundleTimeList;

	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)  // ��ȡ�û������б�Ϊ������������Ļ�ȡ��ʱ�����б�
	{
		BundleTimeList.SetClassId(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID());//ΪBundleTimeList���û�Ա����ID

		theApp.GetCurCashier()->DoBundleTimeList(BundleTimeList); //�����Ļ�ȡ��ʱ�����б���������䵽BundleTimeList��

		if (BundleTimeList.GetStatusCode() == 0)
		{
			for (INT j = 0; j < (INT)BundleTimeList.GetRecordCount(); j++)
			{
				CFullBundleTimeInfo BundleTimeInfo;

				BundleTimeInfo.SetBundleTimeId(BundleTimeList.GetBundleId(j));
				BundleTimeInfo.SetStartTime(BundleTimeList.GetStartTime(j));
				BundleTimeInfo.SetEndTime(BundleTimeList.GetEndTime(j));
				BundleTimeInfo.SetTimePass(BundleTimeList.GetTimePass(j));
				BundleTimeInfo.SetBundleTimeMoney(BundleTimeList.GetPrice(j));
				BundleTimeInfo.SetBundleTimeType(BundleTimeList.GetBundleType(j));
				BundleTimeInfo.SetDescription(BundleTimeList.GetDescription(j));

				BundleTimeInfo.SetClassId(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID());

				m_BundleTimeInfoArray.Add(BundleTimeInfo);
			}
		}
	}

	return TRUE;
}

//��ȡ��ʱ����
BOOL CNetbarBundleTime::GetBundleTimeInfo(UINT nClassId, UINT nBundleTimeId, CFullBundleTimeInfo& BundleTimeInfo)
{
	for (INT i = 0; i < m_BundleTimeInfoArray.GetCount(); i++)
	{	
		if (m_BundleTimeInfoArray.GetAt(i).GetClassId() == nClassId
			&& m_BundleTimeInfoArray.GetAt(i).GetBundleTimeId() == nBundleTimeId)
		{
			BundleTimeInfo = m_BundleTimeInfoArray.GetAt(i);

			return TRUE;
		}
	}

	return FALSE;
}

/*************************************************
Function:       // CNetbarBundleTime::Run()
Description:    // �̺߳��������ڻ�ȡ���û�����İ�ʱ���ԣ�������������ͬ���������ͬ������
Calls:          // LoadBundTimeInfoFromServer,Synchronize
Table Accessed: // None
Table Updated: // None
Input:          // None
Output:         // ��ʱ���Դ�ŵ�m_BundleTimeInfoArray��
Return:         // 0
Others:         // ��ѭ��
*************************************************/
int CNetbarBundleTime::Run()
{
	IBA_LOG(_T("Start thread:Get bundletime list"));
	CoInitialize(NULL);

	//try
	//{
	LoadBundTimeInfoFromServer();    //Ϊ�û������б��и�����������Ļ�ȡ��ʱ�����б���ŵ�m_BundleTimeInfoArray��



	if (CNetBarConfig::GetInstance()->GetMustSynchronize()) //����ԱCheckinʱ��CashRegisterLogin.GetMustSynchronize()����ȡ������
	{
		CServletHellper::Synchronize(); //�뱾��serverͬ��
	}
	//}
	//catch (...)
	//{
	//	CIBALog::GetInstance()->Write(_T("CNetbarBundleTime::Run() Error!"));
	//}

	CoUninitialize();

	m_lpThread = NULL;

	return 0;
}
