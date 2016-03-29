#include "StdAfx.h"
#include "..\Servlet\CommonParam.h"

using namespace NS_SERVLET;

CCommonParam::CCommonParam(void)
: CServletMethod(_T("CommonParam")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CCommonParam::~CCommonParam(void)
{

}

void CCommonParam::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("param"), EPT_STRING, 20);	// Ҫ��ȡ�Ĳ���/�ֶ�
}

void CCommonParam::SetNetbarId(int nNetbarId)
{
	m_Params.SetValue(_T("netBarId"), nNetbarId);
}

void CCommonParam::SetParam(CString newVal)
{
	m_Params.SetValue(_T("param"), newVal);
}

void CCommonParam::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("commonRequest"), EPT_STRING, 20);
}

CString CCommonParam::GetCommonRequest()
{
	return m_RespFields.GetValueAsString(_T("commonRequest"));
}

BOOL NS_SERVLET::CCommonParam::VerifyResp( CString strResp )
{
	return TRUE;
}
