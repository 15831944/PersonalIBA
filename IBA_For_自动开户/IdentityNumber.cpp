#include "stdafx.h"
#include ".\identitynumber.h"

CIdentityNumber::CIdentityNumber(LPCTSTR lpszIdentityNumber)
{
	m_nProvince = 0;
	m_nCity = 0;
	m_nCounty = 0;
	m_nSex = 0;
	m_nAge = 0;
	m_nRealAge = 0;

	Attach(lpszIdentityNumber);//ȡ�����֤��ȡ�����֤��Ϣ
}

CIdentityNumber::~CIdentityNumber(void)
{
}
//ȡ�����֤��ȡ�����֤��Ϣ
void CIdentityNumber::Attach(LPCTSTR lpszIdentityNumber)
{
	m_strIdentityNumber = lpszIdentityNumber;
	m_strIdentityNumber.Trim();

	if (m_strIdentityNumber.GetLength() == 15)
	{
		m_strIdentityNumber.Insert(6, _T("19"));

		m_nProvince = _ttoi(m_strIdentityNumber.Mid(0, 2));
		m_nCity = _ttoi(m_strIdentityNumber.Mid(2, 2));
		m_nCounty  = _ttoi(m_strIdentityNumber.Mid(4, 2));

		m_BirthDay.SetDate(_ttoi(m_strIdentityNumber.Mid(6, 4)),
			_ttoi(m_strIdentityNumber.Mid(10, 2)), 
			_ttoi(m_strIdentityNumber.Mid(12, 2)));

		UINT nCurYear = COleDateTime::GetCurrentTime().GetYear();
		UINT nCurMonth = COleDateTime::GetCurrentTime().GetMonth();
		UINT nCurDay = COleDateTime::GetCurrentTime().GetDay();

		//������
		m_nAge = nCurYear - m_BirthDay.GetYear();

		// ����������
		if((nCurMonth > m_BirthDay.GetMonth()) ||
			((nCurMonth == m_BirthDay.GetMonth() && nCurDay >= m_BirthDay.GetDay())))
		{
			m_nRealAge = m_nAge;
		}
		else
		{
			m_nRealAge = m_nAge - 1;
		}

		int nTmp = _ttoi(m_strIdentityNumber.Mid(16, 1));

		m_nSex = 0;

		if (nTmp % 2 == 0) 
		{
			m_nSex = 1;
		}

	}
	else if (m_strIdentityNumber.GetLength() == 18)
	{
		m_nProvince = _ttoi(m_strIdentityNumber.Mid(0, 2));
		m_nCity = _ttoi(m_strIdentityNumber.Mid(2, 2));
		m_nCounty  = _ttoi(m_strIdentityNumber.Mid(4, 2));

		m_BirthDay.SetDate(_ttoi(m_strIdentityNumber.Mid(6, 4)),
			_ttoi(m_strIdentityNumber.Mid(10, 2)), 
			_ttoi(m_strIdentityNumber.Mid(12, 2)));

		UINT nCurYear = COleDateTime::GetCurrentTime().GetYear();
		UINT nCurMonth = COleDateTime::GetCurrentTime().GetMonth();
		UINT nCurDay = COleDateTime::GetCurrentTime().GetDay();

		//������
		m_nAge = nCurYear - m_BirthDay.GetYear();

		// ����������
		if((nCurMonth > m_BirthDay.GetMonth()) ||
			((nCurMonth == m_BirthDay.GetMonth() && nCurDay >= m_BirthDay.GetDay())))
		{
			m_nRealAge = m_nAge;
		}
		else
		{
			m_nRealAge = m_nAge - 1;
		}

		int nTmp = _ttoi(m_strIdentityNumber.Mid(16, 1));
		
		m_nSex = 0;

		if (nTmp % 2 == 0) 
		{
			m_nSex = 1;
		}
	}
	else
	{
		m_strIdentityNumber.Empty();
	}
}
//���ɼ����루ֻ��15λ��Ч��
TCHAR CIdentityNumber::MakeVerifyCode()
{
	UINT iS = 0;
	
	UINT iW[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
	
	static TCHAR szVerCode[] = _T("10X98765432");
	
	for (INT i = 0; i < 17; i++)
	{
		iS += (UINT)(m_strIdentityNumber.GetAt(i) - '0') * iW[i];
	}

	UINT iY = iS % 11;

	return szVerCode[iY];
}
//�������֤��Ч��
BOOL CIdentityNumber::IsValid()
{
	BOOL bRet = FALSE;

	if (m_strIdentityNumber.GetLength() == 17) //15����������
	{
		for (INT i = 0; i < m_strIdentityNumber.GetLength(); i++)
		{
			TCHAR c = m_strIdentityNumber.GetAt(i);

			if (c > 57 || c < 48) //����ĸ
			{
				return FALSE;
			}
		}

		if (m_BirthDay.GetStatus() == 0)
		{
			bRet = TRUE;
		}
	}
	else if (m_strIdentityNumber.GetLength() == 18)
	{
		if (m_strIdentityNumber.GetAt(17) == MakeVerifyCode())
		{
			bRet = TRUE;
		}
	}

	return bRet;
}

BOOL CIdentityNumber::IsBirthDay()
{
	COleDateTime dtCurTime = COleDateTime::GetCurrentTime();
	return  dtCurTime.GetMonth() == m_BirthDay.GetMonth() &&
		dtCurTime.GetDay() == m_BirthDay.GetDay();
}