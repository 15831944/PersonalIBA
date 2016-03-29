#include "stdafx.h"
#include "CancelBundleTimeMethod.h"

using namespace NS_SERVLET;

CCancelBundleTimeMethod::CCancelBundleTimeMethod(void)
: CServletMethod(_T("CancelBundleTime")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CCancelBundleTimeMethod::~CCancelBundleTimeMethod(void)
{
}

void CCancelBundleTimeMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);
}

void CCancelBundleTimeMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CCancelBundleTimeMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CCancelBundleTimeMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CCancelBundleTimeMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}

