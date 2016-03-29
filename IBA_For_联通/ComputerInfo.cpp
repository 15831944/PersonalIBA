#include "stdafx.h"
#include ".\computerinfo.h"
#include "IBA.h"
#include "NetBarConfig.h"
#include "ActiveMember.h"

CComputerInfo::CComputerInfo(void)
{
	MANAGER_COMPUT_INFO computerInfo = {0};
	Attach(computerInfo);
}

CComputerInfo::CComputerInfo(MANAGER_COMPUT_INFO& computerInfo)
{
	Attach(computerInfo);
}
CComputerInfo::~CComputerInfo(void)
{
}

void CComputerInfo::Attach(MANAGER_COMPUT_INFO & computerInfo)
{
	static const size_t TempLen = 128;
	char szTmp[TempLen] = {0};

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.termID, LEN_TERMID);
	m_strTerminalID = CA2W(szTmp);
	m_strTerminalID.Trim();

#ifdef _DEBUG
	if (0 == m_strTerminalID.CollateNoCase(_T("T037")))
	{
		INT N = 0;
	}
#endif // _DEBUG

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.auditID, LEN_AUDITID);
	m_strAuditID = CA2W(szTmp);
	m_strAuditID.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.clientVersion, LEN_VERSION);
	m_strClientVersion = CA2W(szTmp);
	m_strClientVersion.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.IPAddr, LEN_IPADDR);
	m_strComputerIP = CA2W(szTmp);
	m_strComputerIP.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.computerName, MAX_COMPUTERNAME_LENGTH);
	m_strComputerName = CA2W(szTmp);
	m_strComputerName.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.location, LEN_LOCATION);
	m_strLcation = CA2W(szTmp);
	m_strLcation.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.memberType, LEN_MEMBERTYPE);
	m_strMemberType = CA2W(szTmp);
	m_strMemberType.Trim();

	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.pdk, LEN_PDK);
	m_strPDK = CA2W(szTmp);
	m_strPDK.Trim();

	//֤������
	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.personalID, LEN_PERSONID);
	m_strPersonalID = CA2W(szTmp);
	m_strPersonalID.Trim();

	//�����˺�
	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.szCardSerialNo, LEN_NETID);
	m_strSerialNo = CA2W(szTmp);
	m_strSerialNo.Trim();
	//m_strSerialNo.TrimLeft('0');

	//�û�����
	ZeroMemory(szTmp, TempLen);
	CopyMemory(szTmp, computerInfo.userName, LEN_USERNAME);
	m_strUserName = CA2W(szTmp);
	m_strUserName.Trim();

	m_Amount = computerInfo.amount;	
	m_AvailavleBalance = computerInfo.availavleBalance;
	
	long nTmp = (long)m_AvailavleBalance;
	if (nTmp < 0)
	{
		m_AvailavleBalance = 0;
	}
	
	m_CheckInTime = computerInfo.checkinTime;
	m_UpdateTime = computerInfo.updateTime;	
	m_CreditAmount = computerInfo.creditAmount;
	m_IDOStatus = computerInfo.dwIDOStatus;
	m_Status = computerInfo.dwStatus; 
	m_FreezeBalance = computerInfo.freezeBalance;
	m_IdType = computerInfo.idType;
	m_LastAccountType = computerInfo.lastAccountType;
	m_LastNetBarID = computerInfo.lastNetBarID;
	m_nNetbarID = computerInfo.netbarID;
	m_PayType = computerInfo.payType;
	m_nRefNo = computerInfo.refNo;
	m_UseAmount = computerInfo.useAmount;
	m_Sex = computerInfo.sex;
	m_nMemberID = computerInfo.memberID;

	//***********************************************************************

	InitState();

}

void CComputerInfo::InitState()
{
	//�ж��Ƿ����û�����

	if (m_nMemberID != 0 && m_strUserName.GetLength() > 0)
	{
		m_bHasUserInfo = TRUE;

		if (99 == m_nMemberID) //ά��ģʽ
		{
			m_AvailavleBalance = 0;
		}
	}
	else
	{
		m_nMemberID = 0;
		m_AvailavleBalance = 0;
		m_bHasUserInfo = FALSE;
	}

	//��ʼΪ����״̬
	m_UpdateState = EUS_ADD;

	CheckStatus();
}

CString CComputerInfo::GetPCClass()
{
	return CNetBarConfig::GetInstance()->GetPCClassName(m_nPCClassID);
}

CString CComputerInfo::GetMemberType() const
{
	CString strTmp;
	
	if (m_bHasUserInfo)
	{
		strTmp = m_strMemberType;
	}

	return strTmp;
}

UINT CComputerInfo::GetUserClassId() const
{
	return CNetBarConfig::GetInstance()->GetUserClassId(GetMemberType());
}

BOOL CComputerInfo::IsMember() const
{
	if (m_bHasUserInfo)
	{
		return CNetBarConfig::GetInstance()->GetIsMember(GetUserClassId());
	}

	return FALSE;
}

CString CComputerInfo::GetMemberIDAsString()
{
	CString strTmp;

	if (m_bHasUserInfo)
	{
		strTmp.Format(_T("%d"), m_nMemberID);
	}
	
	return strTmp;
}

CString CComputerInfo::GetNetId() const
{
	CString strTmp;

	if (m_bHasUserInfo)
	{
		strTmp = m_strSerialNo;
	}

	return strTmp;
}

CString CComputerInfo::GetAvailavleBalanceAsString()
{
	CString strTmp;

	if (m_bHasUserInfo)
	{
		double dblTmp = m_AvailavleBalance / 100.0;
#ifdef __USING_INTER_RESOURCE
		strTmp.Format(_T("%.2f"), dblTmp);
#else
		strTmp.Format(_T("%.2f"), dblTmp);
#endif
	}

	return strTmp;
}

CString CComputerInfo::GetSexAsString()
{
	CIBAString strTmp;

	if (m_bHasUserInfo)
	{
		if (m_Sex == 0)
		{
			strTmp.LoadString(IDS_MAN);
		}
		else
		{
			strTmp.LoadString(IDS_WOMAN);
		}
	}

	return strTmp;
}

CString CComputerInfo::GetCheckInTimeAsString()
{
	CString strTmp;

	// 2011/07/04-gxx: ����жϣ��ϻ�ʱ���Ƿ�Ϊ0��Ϊ0��ʾδ�ϻ�
	//if (m_CheckInTime == 0)
	if (m_CheckInTime.GetStatus() == COleDateTime::invalid)
	{
		return strTmp;
	}
	else if (m_bHasUserInfo || ECS_UNLOCK == m_ComputerStatus) //�����ϻ����߽���
	{
		//strTmp = m_CheckInTime.Format(_T("%m.%d %H:%M"));
		strTmp = m_CheckInTime.Format(_T("%H:%M"));
	}

	return strTmp;
}

CString CComputerInfo::GetAmountAsString()
{
	CString strTmp;

	if (m_bHasUserInfo)
	{
		double dblTmp = m_Amount / 100.0;
#ifdef __USING_INTER_RESOURCE
		strTmp.Format(_T("%.2f"), dblTmp);
#else 
		strTmp.Format(_T("%.2f"), dblTmp);
#endif
	}

	return strTmp;
}

CString CComputerInfo::GetUseTimeAsString()
{
	CString strTmp;

	// 2011/07/11-8201-gxx: 
	if (m_CheckInTime.GetStatus() == COleDateTime::invalid ||
		m_CheckInTime.GetStatus() == COleDateTime::null)
	{
		return strTmp;
	}

	if (m_bHasUserInfo || ECS_UNLOCK == m_ComputerStatus)
	{
		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		
		COleDateTimeSpan dts = dtNow - m_CheckInTime;

		//ϵͳʱ��������¼��в��ʱ�����ܴ���Ϊ�������
		if (dts < COleDateTimeSpan(0, 0, 0, 0))//ʱ��Ϊ��
		{
			strTmp.Format(_T("%.2d:%.2d"), 0, 0);
		}
		else if( m_CheckInTime == 0 ) // �ϻ�ʱ��Ϊ0����ʾû���ϻ�
		{
			strTmp = _T("");
			return strTmp;
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

CString CComputerInfo::GetPayTypeAsString()
{
	CIBAString strTmp;

	if (m_bHasUserInfo)
	{
		switch (m_PayType)
		{
		case 0: strTmp.LoadString(IDS_PAYTYPE0); break;
		case 1: strTmp.LoadString(IDS_PAYTYPE1); break;
		case 2: strTmp.LoadString(IDS_PAYTYPE2); break;
		}
	}

	return strTmp;
}


CString CComputerInfo::GetStatusAsString()
{
	CIBAString strTmp;

	switch (m_ComputerStatus)
	{
	case ECS_ONLINE:
		strTmp.LoadString(IDS_TERMONLINE);
		break;

	case ECS_OFFLINE:
		strTmp.LoadString(IDS_TERMOFFLINE);
		break;

	case ECS_LOCKED:
		strTmp.LoadString(IDS_TERMLOCKED);
		break;

	case ECS_LITTERMONEY:
		strTmp.LoadString(IDS_TERMLITTERMONEY);
		break;

	case ECS_THIEF:
		strTmp.LoadString(IDS_TERMTHIEF);
		break;

	case ECS_SUSPEND:
		strTmp.LoadString(IDS_TERMSUSPEND);
		break;

	case ECS_BIRTHDAY:
		strTmp.LoadString(IDS_TERMBIRTHDAY);
		break;

	case ECS_VIP:
		strTmp.LoadString(IDS_TERMVIP);
		break;

	case ECS_BLACK:
		strTmp.LoadString(IDS_TERMBLACK);
		break;

	case ECS_NORECORD:
		strTmp.LoadString(IDS_TERMNORECORD);
		break;

	case ECS_CONFLICT:
		strTmp.LoadString(IDS_TERMCONFLICT);
		break;

	case ECS_UNLOCK:
		strTmp.LoadString(IDS_TERMUNLOCK);
		break;
	}

	return strTmp;
}

#define TERM_OFFLINE			0x0001		// ����  
#define TERM_ONLINE				0x0002		// ����
#define TERM_SUSPEND			0x0003		// �һ�
#define TERM_LOCKED				0x0004		// ����
#define TEMP_USER				0x0008		// ��ͨ�û�
#define MEMBER_USER				0x0010		// ��Ա
#define TERM_WARNINGBAL			0x0020		// ����
#define TERM_IDCONFLICT			0x0040		/// TermId��ͻ, ����̨�ն�ʹ����ͬһ��TermId
#define TERM_THIEF				0x0080		/// �������(û�пͻ�������,��������pingͨ)
#define BLACK_MAN				0x0100		// �������û�
#define TERM_UNAUTHORIZED		0x0200		/// �޼�¼�ն�(��ʱ�ᱻ����"�Ƿ��ն�"��ʹ�û���ǿ����)
#define BIRTHDAY				0x0400		// �˿�����
#define VIP						0x0800		// �ص��û�
#define TERM_MANUALUNLOCAL      0x0030      // ����
#define UNKNOWN_STATUS			0x8000		/// δ֪״̬

void CComputerInfo::CheckStatus()
{
	ECOMPUTERSTATUS nRet = ECS_OFFLINE;

	if (TERM_ONLINE == m_Status)
	{
		if (3 == m_IDOStatus) //�һ�
		{
			nRet = ECS_SUSPEND;
		}
		else
		{
			if (m_bHasUserInfo)
			{
				nRet = ECS_ONLINE; //����
			}
			else if (!m_strClientVersion.IsEmpty())
			{
				nRet = ECS_LOCKED; 
			}
			else
			{
				nRet = ECS_OFFLINE;
			}
		}
	}
	else if (TERM_MANUALUNLOCAL == m_Status)
	{
		m_bHasUserInfo = FALSE;
		nRet = ECS_UNLOCK;
	}
	else if (TERM_SUSPEND == m_Status)
	{
		nRet = ECS_SUSPEND;
	}
	else if (TERM_THIEF == m_Status)//���Ƶ���
	{
		nRet = ECS_THIEF;
	}
	else if (TERM_LOCKED == m_Status)
	{
		if (3 == m_IDOStatus) //�һ�
		{
			nRet = ECS_SUSPEND;
		}
		else
		{
			nRet = ECS_LOCKED;
		} 
	}
	
	if (ECS_ONLINE == nRet)
	{
		if (11 == m_IdType || 12 == m_IdType)  //�������֤�����ж�����
		{
			CString strBirthday, strToday;

			if (m_strPersonalID.GetLength() == 18)
			{
				strBirthday = m_strPersonalID.Mid(10, 4);
			}
			else if (m_strPersonalID.GetLength() == 15)
			{
				strBirthday = m_strPersonalID.Mid(8, 4);
			}

			SYSTEMTIME st = {0};
			GetLocalTime(&st);
			
			strToday.Format(_T("%02d%02d"), st.wMonth, st.wDay);
			
			if (strToday.Compare(strBirthday) == 0)
			{
				nRet = ECS_BIRTHDAY;
			}
		}
	
		//�ж����
		if (m_AvailavleBalance < (INT)CIBAConfig::GetInstance()->GetMinMoney()
			&& m_PayType == 0)
		{
			nRet = ECS_LITTERMONEY;
		}
	}

	m_ComputerStatus = nRet;
}

void CComputerInfo::SetComputerStatus(ECOMPUTERSTATUS newVal)
{
	if (ECS_OFFLINE == newVal)
	{
		m_strClientVersion.Empty();
		m_strComputerName.Empty();

		//�����ǰ�ǽ���״̬���򲻸���Ϊ����
		
		if (GetComputerStatus() == ECS_UNLOCK)
		{
			return;
		}
	}
	else if (ECS_UNLOCK == newVal)
	{
		m_nNetbarID = 0;
		m_bHasUserInfo = FALSE;
	}
	else if (ECS_THIEF == newVal)
	{
		m_Status = TERM_THIEF;

		if (HasClient())
		{
			return;
		}
	}
	
	m_ComputerStatus = newVal;
}

BOOL CComputerInfo::HasClient() const
{
	BOOL bRet = TRUE;

	switch (m_ComputerStatus)
	{
	case ECS_OFFLINE:
	case ECS_THIEF:

		bRet = FALSE;
		break;
	}

	return bRet;
}

BOOL CComputerInfo::HasUser() const
{
	BOOL bRet = FALSE;

	switch (m_ComputerStatus)
	{
	case ECS_ONLINE:
	
	case ECS_BIRTHDAY:
	case ECS_VIP:
	case ECS_BLACK:
	case ECS_LITTERMONEY:
		
		bRet = TRUE;
		break;

	case ECS_SUSPEND:
		bRet = m_bHasUserInfo;
		break;
	}

	return bRet;
}

void CComputerInfo::ClearUserInfo()
{
	m_strSerialNo.Empty();
	m_strMemberType.Empty();
	m_strUserName.Empty();
	m_strPersonalID.Empty();
	m_strLcation.Empty();
	m_strPDK.Empty();
	m_strSerialNo.Empty();	
	m_strAuditID.Empty();

	m_nMemberID = 0;
	m_nRefNo = 0;
	m_nSerialNo = 0;
	m_nNetbarID = 0;

	m_Sex = 0;
	m_IdType = 0;
	m_PayType = 0;
	m_Amount = 0;
	m_CreditAmount = 0;
	m_UseAmount = 0;
	m_FreezeBalance = 0;
	m_AvailavleBalance = 0;

	m_LastNetBarID = 0;
	m_LastAccountType = 0;

	m_bHasUserInfo = FALSE;
}

CString CComputerInfo::GetUserName(BOOL bWithSex)
{
	CString strTmp = m_strUserName;

	if (bWithSex && !strTmp.IsEmpty())
	{
		strTmp.AppendFormat(_T("(%s)"), GetSexAsString());
	}

	return strTmp;
}

void CComputerInfo::AttachActiveMember(CActiveMember& ActiveMember)
{
	SetMemberID( ActiveMember.GetMemberID() );
	SetUserName( ActiveMember.GetUserName() );
	SetNetId( ActiveMember.GetNetId() );
	SetAvailavleBalance( ActiveMember.GetAvailavleBalance() );
	m_Amount = ActiveMember.GetAmount();
	m_PayType = ActiveMember.GetPayType();
	m_IdType = ActiveMember.GetIdType();
	m_strPersonalID = ActiveMember.GetPersonalID();
	m_CheckInTime = ActiveMember.GetCheckInTime();
	m_strMemberType = ActiveMember.GetUserClassName();
	m_Sex = ActiveMember.GetSex();
}


