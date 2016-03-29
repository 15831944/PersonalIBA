#include "stdafx.h"
#include "UsePointMethod.h"

using namespace NS_SERVLET;

CUsePointMethod::CUsePointMethod(void)
: CServletMethod(_T("UsePoint")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CUsePointMethod::~CUsePointMethod(void)
{
}

void CUsePointMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("point"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("Quantity"), EPT_NUMBER, 6);
	m_Params.AddParam(_T("Article"), EPT_STRING, 40);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);
	m_Params.AddParam(_T("businessType"), EPT_NUMBER, 2);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
}

void CUsePointMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CUsePointMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CUsePointMethod::SetPoint(UINT newVal)
{
	m_Params.SetValue(_T("point"), newVal);
}

void CUsePointMethod::SetQuantity(UINT newVal)
{
	m_Params.SetValue(_T("Quantity"), newVal);
}

void CUsePointMethod::SetArticle(CString newVal)
{
	m_Params.SetValue(_T("Article"), newVal);
}

void CUsePointMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CUsePointMethod::SetBusinessType(UINT newVal)
{
	m_Params.SetValue(_T("businessType"), newVal);
}

void CUsePointMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("refNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("currentPort"), EPT_NUMBER, 12);
	m_RespFields.AddRespField(_T("tranTime"), EPT_STRING, 14);
}

UINT CUsePointMethod::GetRefNo()
{
	return m_RespFields.GetValueAsUINT(_T("refNo"));
}

UINT CUsePointMethod::GetCurrentPort()
{
	return m_RespFields.GetValueAsUINT(_T("currentPort"));
}

CString CUsePointMethod::GetTranTime()
{
	return m_RespFields.GetValueAsString(_T("tranTime"));
}

