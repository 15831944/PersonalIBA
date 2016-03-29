#include "stdafx.h"
#include "IBAConfig.h"
#include "IBAHelpper.h"
#include "resource.h"
#include "NetBarConfig.h"
#include ".\activemember.h"
#include "IBALog.h"
#include "IBA.h"

CActiveMember::CActiveMember(void)
{
	m_nStatus = 0;
	m_bIsActive = FALSE;
	GPropertyInit( IsOnLine, FALSE );
	GPropertyInit( Sex, 0 );
	GPropertyInit( ClassId, 0);

	GPropertyInit( IdType, 0 );
	GPropertyInit( NetbarID, 0 );
	GPropertyInit( MemberID, 0 );
	GPropertyInit( PCClassID, 0 );
	GPropertyInit( PayType, 0 );
	GPropertyInit( Amount, 0 );
	GPropertyInit( AdvancePayment, 0 );
	GPropertyInit( CasherID, 0 );
	GPropertyInit( RefNo, 0 );
	GPropertyInit( IsRegisterBT, FALSE); 
	GPropertyInit( AvailavleBalance, 0 );

	// 2011/07/08-8201-gxx: 
	GPropertyInit3( DynamicMaxAmount, IsLocalUser, Password ,
		            0,                FALSE,       _T(""));

	// 2011/10/25-8230-gxx: 
	GPropertyInit( RoomMemberId, 0);

	GPropertyInit( ScanType, 0);
}

CActiveMember::~CActiveMember(void)
{
}

void CActiveMember::Attach(MANAGER_COMPUT_INFO & computerInfo)
{
	static const size_t TempLen = 128;
	char szTmp[TempLen] = {0};

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.termID, LEN_TERMID);
	CString strTermID = CA2W(szTmp);
	strTermID.Trim();
	GPropertyInit( TerminalID, strTermID );

	//ZeroMemory(szTmp, TempLen);
	//CopyMemory(szTmp, computerInfo.IPAddr, LEN_IPADDR);
	//m_strComputerIP = CA2W(szTmp);
	//m_strComputerIP.Trim();

	//ZeroMemory(szTmp, TempLen);
	//CopyMemory(szTmp, computerInfo.computerName, MAX_COMPUTERNAME_LENGTH);
	//m_strComputerName = CA2W(szTmp);
	//m_strComputerName.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.memberType, LEN_MEMBERTYPE);
	UserClassName = CA2W(szTmp);
	UserClassName.Trim();

	GPropertyInit( ClassId, CNetBarConfig::GetInstance()->GetUserClassId(UserClassName) );

	//{ 2011/05/16-gxx: ��Ӵ��жϵ���������⣺
	//    ���ݿ��б����˼���ID�ͼ������ƵĶ�Ӧ��ϵ.�����ݿ��е������ϵ���û�к�
	//    ����ͬ���Ļ������ܴ�����������: �ͻ����ϱ���IServer��classID��IServer
	//    ��������ݿ����Ҳ������classID�Ķ�Ӧ�ļ������ƣ���ôIServer�ͻ�����
	//    classID���� �������ƴ���IBA, ���Դ˴���ӳ���Ϊ1���жϣ��������������
	//    ����. (�������Ƶĳ��Ȳ�����Ϊ1����˿��������ж�)

	if (UserClassName.GetLength() == 1)
	{
		int iClass = _ttoi(UserClassName);
		if (iClass == 0)
		{
			iClass = 1;
		}
		SetClassId( iClass );
	}

	////}
	

	//֤������
	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.personalID, LEN_PERSONID);

	GPropertyInit( PersonalID, CA2W(szTmp));
	SET(PersonalID).Trim();

	//�����˺�
	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.szCardSerialNo, LEN_NETID);
	CString strNetID = CA2W(szTmp);
	strNetID.Trim();
	SetNetId( strNetID );

	//�û�����
	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.userName, LEN_USERNAME);
	CString strUserName = CA2W(szTmp);
	strUserName.Trim();
	GPropertyInit( UserName, strUserName);
	IBA_LOG(_T("ci.UserName=%s"),UserName);
	//char *realna = "��ͨ�û�";

	GPropertyInit( Amount, computerInfo.amount );	
	GPropertyInit( AvailavleBalance, computerInfo.availavleBalance);

	GPropertyInit( IdType, computerInfo.idType );
	GPropertyInit( NetbarID, computerInfo.netbarID );

	GPropertyInit( PayType, computerInfo.payType );
	GPropertyInit( Sex, computerInfo.sex );
	GPropertyInit( MemberID, computerInfo.memberID );
	GPropertyInit( RefNo, computerInfo.refNo ); 

	CheckInTime = computerInfo.checkinTime;		
	//CString strCheckinTime;
	//strCheckinTime = CheckInTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	//IBA_LOG(_T("ci.CheckinTime = %s"),strCheckinTime);

	UpdateDataTime = COleDateTime::GetCurrentTime();

	m_nStatus = EStatus_LocalOnline;
	SetIsOnLine( TRUE );
}

CString CActiveMember::GetPayTypeAsString()
{
	CIBAString strTmp;

	if ( GetIsOnLine() )
	{
		switch ( GetPayType() )
		{
		case EPayType_General:     strTmp.LoadString(IDS_PAYTYPE0); break;
		case EPayType_Capped:      strTmp.LoadString(IDS_PAYTYPE1); break;
		case EPayType_Withholding: strTmp.LoadString(IDS_PAYTYPE2); break;
		}
	}

	// 2011/10/26-8230-gxx: 
	if (GetRoomMemberId() > 0)
	{
		strTmp.LoadString(IDS_ROOM_WHOLE);
	}


	return strTmp;
}

CString CActiveMember::GetAvailavleBalanceAsString()
{
	CString strTmp;	

	//double dblTmp = GetAvailavleBalance() / 100.0;
	int nTmp = GetAvailavleBalance();

	//��������ģʽʱ��������������ȡ���������Ҫ�����������롣
	/*if(CIBAGlobal::emNetworkBroken == CIBAGlobal::NetworkStatus)
	{
		nTmp -= nTmp % 50;
	}
	if (nTmp < 0)
	{
		nTmp = 0;
	}*/

	double dblTmp = (double)nTmp / 100.0;

#ifdef __USING_INTER_RESOURCE
	strTmp.Format(_T("��%.2f"), dblTmp);
#else
	strTmp.Format(_T("%.2f"), dblTmp);
#endif

	return strTmp;
}

CString CActiveMember::GetAdvancePaymentAsString()
{
	CString strTmp;

	double dblTmp = GetAdvancePayment() / 100.0;

#ifdef __USING_INTER_RESOURCE
	strTmp.Format(_T("��%.2f"), dblTmp);
#else
	strTmp.Format(_T("%.2f"), dblTmp);
#endif

	return strTmp;
}

CString CActiveMember::GetSexAsString()
{
	return CIBAHelpper::GetSexAsString( GetSex() );
}

CString CActiveMember::GetCheckInTimeAsString()
{
	CString strTmp;

	if ( GetIsOnLine() )
	{
		strTmp = CheckInTime.Format(_T("%H:%M"));
	}

	return strTmp;
}

CString CActiveMember::GetActivationTimeAsString()
{
	CString strTmp;

	if (IsActive())
	{
		strTmp = ActivationTime.Format(_T("%H:%M"));
	}

	return strTmp;
}

BOOL CActiveMember::IsActive()
{
	return m_bIsActive;
}

CString CActiveMember::GetAmountAsString()
{
	CString strTmp;

	// 2011/05/19-gxx: ����ǿ�����ʱ�Ļ�Ҳ���Եõ����ѽ��
	// 2011/06/14-gxx: ���ѽ�Ϊ0������ʾ
	if (GetIsOnLine() || GetIsRegisterBT() || GetAmount()!=0)
	{
		int nTmp = GetAmount();

		//��������ģʽʱ��������������
		if(CIBAGlobal::emNetworkBroken == CIBAGlobal::NetworkStatus)
		{
			if(nTmp % 50 > 0)
			{
				//nTmp += 50;
				nTmp = 50*(nTmp/50 +1 ); 
			}
		}

		if (nTmp < 0)
		{
			nTmp = 0;
		}

		double dblTmp = (double)nTmp / 100.0;
		//double dblTmp = GetAmount() / 100.0;

#ifdef __USING_INTER_RESOURCE
		strTmp.Format(_T("��%.2f"), dblTmp);
#else
		strTmp.Format(_T("%.2f"), dblTmp);
#endif
	}

	return strTmp;
}

void CActiveMember::SetTerminalID(CString newVal)
{
	GPropertyInit( TerminalID, newVal );

	if (GetTerminalID().IsEmpty())
	{
		SetIsOnLine( FALSE );
	}
}

CString CActiveMember::GetUseTimeAsString()
{
	CString strTmp;

	if ( GetIsOnLine() )
	{
		COleDateTime dtNow = COleDateTime::GetCurrentTime();

		COleDateTimeSpan dts = dtNow - CheckInTime;

		//ϵͳʱ��������¼��в��ʱ�����ܴ���Ϊ�������
		if (dts < COleDateTimeSpan(0, 0, 0, 0))//ʱ��Ϊ��
		{
			strTmp.Format(_T("%.2d:%.2d"), 0, 0);
		}
		else//����
		{
			//{ 2011/04/22-gxx: �޸�ԭ��֧�����ʱ��24СʱΪ��ʱ��, GetHours()-->GetTotalHours()
			int nHours = (int)dts.GetTotalHours();
			strTmp.Format(_T("%.2d:%.2d"), nHours, dts.GetMinutes());
			//}		
		}
	}

	return strTmp;
}

void CActiveMember::SetClassId(UINT newVal)
{
	GPropertyInit( ClassId, newVal );

	UserClassName = CNetBarConfig::GetInstance()->GetUserClassName(GetClassId());
}

/*************************************************
Function:       //IsOffLinePossibility
Description:    //�ն����ߵ��û����������ϴ�OnClientUpdateData��ʱ�䳬��90s������Ϊ����������
Return:         //�����ͷ���TRUE.
*************************************************/
BOOL CActiveMember::IsOffLinePossibility()
{
	if ( GetIsOnLine() ) //CheckInTime���ڣ����ն��е�½
	{
		COleDateTimeSpan dtSpan = COleDateTime::GetCurrentTime() - UpdateDataTime; //��ǰʱ�����һ�ε�OnClientUpdateDataʱ��

		// 2014-1-10-qsc ��6m30s�ĳ�2m30s 
		//return dtSpan.GetTotalSeconds() > (2 * 60 + 30); //��һ�εĸ���ʱ��͵�ǰʱ��ļ��>2m30s
		int nSeconds =(int)(dtSpan.GetTotalSeconds()); 
		if(nSeconds > (3 * 60 + 30))
		{
			//IBA_LOG(_T("status = EStatus_LocalOnline, NeedQuery! Time past from last ClientUpdateData is %dS"),nSeconds);
		}		
		return nSeconds > (3 * 60 + 30); //��һ�εĸ���ʱ��͵�ǰʱ��ļ��>1m30s
		// ������Ϊ���������ˡ�
	}
	
	return FALSE;
}

BOOL CActiveMember::IsMember()
{
	return CNetBarConfig::GetInstance()->GetIsMember( GetClassId() );
}

CString CActiveMember::GetPCClass()
{
	return CNetBarConfig::GetInstance()->GetPCClassName( GetPCClassID() );
}

CString CActiveMember::GetTermIdInfo()
{
	CString strTmp;

	if (GetTerminalID().IsEmpty())
	{

	}
	else
	{
		strTmp = GetTerminalID();

		if (!IsActive() && GetAvailavleBalance() > 0) //ʧȥ��������Ǯ
		{
			strTmp.Append(_T("(ʧȥ����)"));
		}
	}

	return strTmp;
}

CString CActiveMember::GetUserName(BOOL bWithSex /*= FALSE*/)
{
	CString strTmp = UserName;

	if (bWithSex)
	{
		strTmp.AppendFormat(_T("(%s)"), GetSexAsString());
	}

	return strTmp;
}

/*************************************************
Function:       //IsNeedToQuery
Description:    //�Ƿ���Ҫ��ѯ��
				//EStatus_LocalOnline - ����90S����Ϊ��Ҫ��ѯ
				//EStatus_CenterOnline - ÿ�ζ���Ҫ��ѯ
				//EStatus_AutoReturn - �ϴθ���ʱ����Ч�����ϴ�ʱ������������̨���ã�����Ϊ��Ҫ��ѯ
				//EStatus_RoomAutoReturn - �ϴθ���ʱ����Ч�����ϴ�ʱ��������2���ӣ�����Ϊ��Ҫ��ѯ
Return:			//��Ҫ��ѯ�ͷ���true.���򷵻�false.
*************************************************/
BOOL CActiveMember::IsNeedToQuery()
{
	if (GetStatus() == EStatus_LocalOnline) //��������
	{
		return IsOffLinePossibility(); 
	}
	else if (GetStatus() == EStatus_CenterOnLine) //��������
	{
		return COleDateTime::GetCurrentTime() >= UpdateDataTime;
	}
	else if (GetStatus() == EStatus_AutoReturn) //�Զ��˿�
	{
		//Ϊinvalid��null,��Ҫ��ѯ
		if (UpdateDataTime.GetStatus() != COleDateTime::valid) 
		{
			return TRUE;
		}

		COleDateTimeSpan dtSpan = COleDateTime::GetCurrentTime() - UpdateDataTime; 

		//���ϴ�ʱ������������̨���ã�����Ϊ��Ҫ��ѯ
		if (dtSpan.GetTotalMinutes() >= GetRefundmentTime())//��������̨���� - ǿ���Զ��˿� = 0�� ��ʱ�û�Ǯ����󼸷��Ӻ��˿� = ��������
		{
			return TRUE;
		}
	}
 
	else if (GetStatus() == EStatus_RoomAutoReturn)   //�����Զ��˿�
	{
		if (UpdateDataTime.GetStatus() != COleDateTime::valid)
		{
			return TRUE;
		}

		COleDateTimeSpan dtSpan = COleDateTime::GetCurrentTime() - UpdateDataTime; 

		int nSeconds = (int)(dtSpan.GetTotalSeconds());

		if (nSeconds >= 200.0) // ����2���ӣ����˺žͿ����Զ��˿���
		{
			return TRUE;
		}
	}

	return FALSE;
}
//����̨������ǿ���Զ��˿����0�����򷵻�"����̨����"-"��ֵ�˿�����"-��ʱ�û�Ǯ����󼸷��Ӻ��˿�
UINT CActiveMember::GetRefundmentTime()
{
	if (CNetBarConfig::GetInstance()->GetMustAutoReturn() == 1)
	{
		return 0;
	}

	return CIBAConfig::GetInstance()->GetRefundmentTime();
}

BOOL CActiveMember::LoadFromDB(CString strNetId)
{
	return FALSE;
}

/*************************************************
Function:       //GetLocalExpense
Description:    //��ȡ�������ѽ�� - bFilter�����Ƿ����0.5Ԫ������ȡ�����ˣ�Ĭ��Ϊ�ǡ������������or��½ʱ������ֱ�ӷ���ActiveMember.GetAmout������ͨ�����ʽ��м��㡣
				//����ʱ��������ѽ�����ǰСʱ���ͼ���Ϊ��ǰСʱʣ������ʱ�䡣
Calls:          //..
Table Accessed: //None
Table Updated:  //None
Input:          //bFilter - �Ƿ����0.5Ԫ����ȡ����Ĭ��ΪTRUE.
Output:         //None
Return:         //������ı������ѽ��
Others:         //ֻ���ڱ���ע����û�ʹ�ã�����
*************************************************/
UINT CActiveMember::GetLocalExpense(BOOL bFilter) 
{
	if (PolicyInfo.IsEmpty())
	{
		return GetAmount();
	}
	if (PolicyInfo.GetLength() != 24*4)
	{
		IBA_ASSERT2(FALSE, "warning:������Ϣ����");
		CIBALog::GetInstance()->WriteFormat(_T("�����û�������Ϣ����,netId=%s, ����=%s"), NetId, PolicyInfo);
		return GetAmount();
	}
	if (CheckInTime.GetStatus() != COleDateTime::valid || CheckInTime == 0)
	{
		return GetAmount();
	}
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeSpan = timeNow - CheckInTime;

	UINT nPolicyHours[24] = {0};
	for (int i = 0; i < 96; i+=4)
	{
		nPolicyHours[i/4] = (UINT)_ttoi(PolicyInfo.Mid(i, 4));
	}

	double dExp = 0.0;
	int iStartHour = CheckInTime.GetHour();
	int iStartSecond = CheckInTime.GetSecond()+ CheckInTime.GetMinute()*60;
	int nTotalSecond = (int)timeSpan.GetTotalSeconds();
	for(int iSecond = nTotalSecond; iSecond > 0; ) // ����expenseѭ����expense = �ڵ�ǰСʱ������ * ��ǰСʱ�������
	{
		if (nPolicyHours[iStartHour] == 0)
		{
			IBA_ASSERT2(FALSE,"warning:������Ϣ����,����Ϊ0");
			CIBALog::GetInstance()->WriteFormat(_T("�����û�������Ϣ����,netId=%s, ����=%s"), NetId, PolicyInfo);
			return GetAmount();
		}
		double dSecondPolicy = nPolicyHours[iStartHour%24] / 3600.0;  //��ǰСʱÿ��ķ���

		int nSeconds = 3600 - iStartSecond; 
		if (nSeconds >= iSecond)  //�����ʼ����ʱ���Ǹ�Сʱ��ʣ������>�ܹ����ѵ�������˵�����л��Ѷ��ڸ�Сʱ��
		{
			nSeconds = iSecond;   //��������ʱ�� = �Ǹ�Сʱ��ʣ�������� liyajun note: �������뷽���Ƿ����ֻҪ����ʱ��δ������ǰСʱ����ô�����������������˵�ǰСʱ��ʣ��ʱ�䣿
		}
		
		dExp += dSecondPolicy * nSeconds; //��ǰСʱ���� = ��ǰСʱ����� * ��ʼʱ��Сʱ��ʣ��������note:�ӵ�һ��ѭ����ʣ����������Ϊ0.
		iSecond -= nSeconds; 
		iStartHour ++;
		//2015-0828 liyajun
		if (iStartHour == 24)
		{
			iStartHour = 0;
		}

		iStartSecond = 0; // ��һ��ѭ���Ժ���ʼ�붼Ϊ0
	}

	if (bFilter) //Filter��ʾ���ϲ���
	{
		UINT nExp = UINT(dExp+49)/50*50;  //���Ͻ��в��룬��͵�λΪ50 
		return nExp;
	}
	else
	{
		return UINT(dExp);
	}
	
}