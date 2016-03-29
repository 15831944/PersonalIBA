#include "StdAfx.h"
#include "GetSystemConfigMethod.h"

using namespace NS_SERVLET;

CGetSystemConfigMethod::CGetSystemConfigMethod(void)
: CServletMethod(_T("GetSystemConfig")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CGetSystemConfigMethod::~CGetSystemConfigMethod(void)
{

}

void CGetSystemConfigMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("stringField"), EPT_STRING, 50);	// Ҫ��ȡ�Ĳ���/�ֶ�
}

void CGetSystemConfigMethod::SetNetbarId(int nNetbarId)
{
	m_Params.SetValue(_T("netBarId"), nNetbarId);
}

void CGetSystemConfigMethod::SetStringField(CString newVal)
{
	m_Params.SetValue(_T("stringField"), newVal);
}

void CGetSystemConfigMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("value1"), EPT_STRING, 11);
	m_RespFields.AddRespField(_T("value2"), EPT_NUMBER, 11);
	m_RespFields.AddRespField(_T("stringField"), EPT_NUMBER, 100);
}

CString CGetSystemConfigMethod::GetStringFieldValue()
{
	return m_RespFields.GetValueAsString(_T("stringField"));
}

UINT CGetSystemConfigMethod::GetValue1()
{
	return m_RespFields.GetValueAsUINT(_T("value1"));
}

UINT CGetSystemConfigMethod::GetValue2()
{
	return m_RespFields.GetValueAsUINT(_T("value2"));
}


