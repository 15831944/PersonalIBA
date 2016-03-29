#include "stdafx.h"
#include "UpgradeMethod.h"

using namespace NS_SERVLET;

CUpgradeMethod::CUpgradeMethod(void)
: CServletMethod(_T("Upgrade")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CUpgradeMethod::~CUpgradeMethod(void)
{
}

void CUpgradeMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("oldClassId"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("newClassId"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("reason"), EPT_STRING, 60);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
}

void CUpgradeMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CUpgradeMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CUpgradeMethod::SetOldClassId(UINT newVal)
{
	m_Params.SetValue(_T("oldClassId"), newVal);
}

void CUpgradeMethod::SetNewClassId(UINT newVal)
{
	m_Params.SetValue(_T("newClassId"), newVal);
}

void CUpgradeMethod::SetReason(CString newVal)
{
	m_Params.SetValue(_T("reason"), newVal);
}

void CUpgradeMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CUpgradeMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("refNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("tranTime"), EPT_STRING, 14);
}

CString CUpgradeMethod::GetTranTime()
{
	return m_RespFields.GetValueAsString(_T("tranTime"));
}


UINT CUpgradeMethod::GetRefNo()
{
	return m_RespFields.GetValueAsUINT(_T("refNo"));
}
