#include "stdafx.h"
#include "ShiftCreditMethod.h"

using namespace NS_SERVLET;

CShiftCreditMethod::CShiftCreditMethod(void)
: CServletMethod(_T("ShiftCredit")) //���ķ�������
{
	m_bUseQueryHost = TRUE; //ʹ�ò�ѯ��
	m_nRevTimeout = 120;	//��ʱ2����

	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CShiftCreditMethod::~CShiftCreditMethod(void)
{
}

void CShiftCreditMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("dutyId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
}

void CShiftCreditMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CShiftCreditMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CShiftCreditMethod::SetDutyId(UINT newVal)
{
	m_Params.SetValue(_T("dutyId"), newVal);
}

void CShiftCreditMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CShiftCreditMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("recordCount"), EPT_NUMBER, 4);

}

BOOL CShiftCreditMethod::OnBegin()
{
	m_CreditInfos.RemoveAll();

	return TRUE;
}

BOOL CShiftCreditMethod::DecodeResp(CString strResp)
{
	BOOL bRet = CServletMethod::DecodeResp(strResp);

	UINT nCount = GetRecordCount();

	if (nCount > 0)
	{
		m_CreditInfos.SetSize(nCount);

		INT nIndex = m_RespFields.GetAllLength();

		for (INT i = 0; i < m_CreditInfos.GetCount(); i++)
		{
			CString strTmp = strResp.Mid(nIndex);
			m_CreditInfos.GetAt(i).m_RespFields.DecodeResp(strTmp);
			nIndex += m_CreditInfos.GetAt(i).m_RespFields.GetAllLength();
		}
	}

	return bRet;
}

UINT CShiftCreditMethod::GetRecordCount()
{
	return m_RespFields.GetValueAsUINT(_T("recordCount"));
}

CString CShiftCreditMethod::GetCreditTime(INT nIndex)
{
	return m_CreditInfos.GetAt(nIndex).GetCreditTime();
}

UINT CShiftCreditMethod::GetMemberId(INT nIndex)
{
	return m_CreditInfos.GetAt(nIndex).GetMemberId();
}

UINT CShiftCreditMethod::GetRefNo(INT nIndex)
{
	return m_CreditInfos.GetAt(nIndex).GetRefNo();
}

INT CShiftCreditMethod::GetCreditAmount(INT nIndex)
{
	return m_CreditInfos.GetAt(nIndex).GetCreditAmount();
}

INT CShiftCreditMethod::GetCreditPresent(INT nIndex)
{
	return m_CreditInfos.GetAt(nIndex).GetCreditPresent();
}

UINT CShiftCreditMethod::GetPoint(INT nIndex)
{
	return m_CreditInfos.GetAt(nIndex).GetPoint();
}

//******************************************************************************


CCreditInfo::CCreditInfo()
{
	m_RespFields.AddRespField(_T("creditTime"), EPT_NUMBER, 14);
	m_RespFields.AddRespField(_T("memberId"), EPT_NUMBER, 12);
	m_RespFields.AddRespField(_T("refNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("creditAmount"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("creditPresent"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("point"), EPT_NUMBER, 6);
}

CString CCreditInfo::GetCreditTime()
{
	return m_RespFields.GetValueAsString(_T("creditTime"));
}

UINT CCreditInfo::GetMemberId()
{
	return m_RespFields.GetValueAsUINT(_T("memberId"));
}

UINT CCreditInfo::GetRefNo()
{
	return m_RespFields.GetValueAsUINT(_T("refNo"));
}

INT CCreditInfo::GetCreditAmount()
{
	return m_RespFields.GetValueAsINT(_T("creditAmount"));
}

INT CCreditInfo::GetCreditPresent()
{
	return m_RespFields.GetValueAsINT(_T("creditPresent"));
}

UINT CCreditInfo::GetPoint()
{
	return m_RespFields.GetValueAsUINT(_T("point"));
}
