#include "stdafx.h"
#include "Checkout2Method.h"


using namespace NS_SERVLET;

CCheckout2Method::CCheckout2Method(void)
: CServletMethod(_T("Checkout2")) //���ķ�������
, m_nMemberId(0)
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CCheckout2Method::~CCheckout2Method(void)
{
}

void CCheckout2Method::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("cost"), EPT_NUMBER, 6);
	m_Params.AddParam(_T("endUsingTime"), EPT_NUMBER, 14);
	m_Params.AddParam(_T("safeLogout"), EPT_NUMBER, 4);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
}

UINT CCheckout2Method::MakeLocalSerialNo()
{
	return 0;
}

void CCheckout2Method::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);

	m_nMemberId = newVal;
}

void CCheckout2Method::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CCheckout2Method::SetCost(UINT newVal)
{
	m_Params.SetValue(_T("cost"), newVal);
}

void CCheckout2Method::SetEndUsingTime(CString newVal)
{
	m_Params.SetValue(_T("endUsingTime"), newVal);
}

void CCheckout2Method::SetSafeLogout(UINT newVal)
{
	m_Params.SetValue(_T("safeLogout"), newVal);
}

void CCheckout2Method::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CCheckout2Method::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}

