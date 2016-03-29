#include "stdafx.h"
#include "CashierTotalMethod.h"

using namespace NS_SERVLET;

CCashierTotalMethod::CCashierTotalMethod(void)
: CServletMethod(_T("CashierTotal")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CCashierTotalMethod::~CCashierTotalMethod(void)
{
}

void CCashierTotalMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	CServletMethod::AddParams();
	m_Params.AddParam(_T("beginTime"), EPT_STRING, 14);
	m_Params.AddParam(_T("endTime"), EPT_STRING, 14);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);
}

void CCashierTotalMethod::SetBeginTime(CString newVal)
{
	m_Params.SetValue(_T("beginTime"), newVal);
}

void CCashierTotalMethod::SetEndTime(CString newVal)
{
	m_Params.SetValue(_T("endTime"), newVal);
}

void CCashierTotalMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CCashierTotalMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("CardCount"), EPT_NUMBER, 12);
	m_RespFields.AddRespField(_T("Amount"), EPT_NUMBER, 12);
}

UINT CCashierTotalMethod::GetCardCount()
{
	return m_RespFields.GetValueAsUINT(_T("CardCount"));
}

UINT CCashierTotalMethod::GetAmount()
{
	return m_RespFields.GetValueAsUINT(_T("Amount"));
}

