#include "stdafx.h"
#include "BundleTime2Method.h"

using namespace NS_SERVLET;

CBundleTime2Method::CBundleTime2Method(void)
: CServletMethod(_T("BundleTime2")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CBundleTime2Method::~CBundleTime2Method(void)
{
}

void CBundleTime2Method::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netbarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("timeId"), EPT_NUMBER, 4);
	m_Params.AddParam(_T("termId"), EPT_STRING, 8);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
}

void CBundleTime2Method::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CBundleTime2Method::SetNetbarId(UINT newVal)
{
	m_Params.SetValue(_T("netbarId"), newVal);
}

void CBundleTime2Method::SetTimeId(UINT newVal)
{
	m_Params.SetValue(_T("timeId"), newVal);
}

void CBundleTime2Method::SetTermId(CString newVal)
{
	m_Params.SetValue(_T("termId"), newVal);
}

void CBundleTime2Method::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CBundleTime2Method::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("refNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("amount"), EPT_NUMBER, 6);
}

UINT CBundleTime2Method::GetRefNo()
{
	return m_RespFields.GetValueAsUINT(_T("refNo"));
}

UINT CBundleTime2Method::GetAmount()
{
	return m_RespFields.GetValueAsUINT(_T("amount"));
}

