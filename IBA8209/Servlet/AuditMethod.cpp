#include "stdafx.h"
#include "AuditMethod.h"

using namespace NS_SERVLET;

CAuditMethod::CAuditMethod(void)
: CServletMethod(_T("Audit")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CAuditMethod::~CAuditMethod(void)
{
}

CString CAuditMethod::GetMethodName()
{
	CString strTmp;

	strTmp = m_strMethodName;

	if (GetCardType() == ECT_ID)
	{
		strTmp.Insert(0, _T("ID"));
	}

	return strTmp;
}

void CAuditMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("MemberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("NetBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("AuditType"), EPT_NUMBER, 1);
	m_Params.AddParam(_T("AuditId"), EPT_STRING, 12);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
}

void CAuditMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("MemberId"), newVal);
}

void CAuditMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("NetBarId"), newVal);
}

void CAuditMethod::SetAuditType(UINT newVal)
{
	m_Params.SetValue(_T("AuditType"), newVal);
}

void CAuditMethod::SetAuditId(CString newVal)
{
	m_Params.SetValue(_T("AuditId"), newVal);
}

void CAuditMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}

