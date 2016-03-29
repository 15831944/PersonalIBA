#include "stdafx.h"
#include "ActivationMethod.h"


using namespace NS_SERVLET;

CActivationMethod::CActivationMethod(void)
: CServletMethod(_T("Activation")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CActivationMethod::~CActivationMethod(void)
{
}

void CActivationMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("Mode"), EPT_NUMBER, 1);

	// 2014-11-12 ֤������
	m_Params.AddParam(_T("dataType"), EPT_NUMBER, 1);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
	m_Params.SetValue(_T("dataType"), 0);	// �̶������֤  0 ˢ���֤¼�롢1 �ֶ�¼�롢2ͨ��ɨ��֤��¼�롢4 �ֻ�����¼��
}

void CActivationMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CActivationMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CActivationMethod::SetMode(UINT newVal)
{
	m_Params.SetValue(_T("Mode"), newVal);
}

void CActivationMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();
}

void CActivationMethod::SetDotType( UINT newVal )
{
	m_Params.SetValue(_T("dataType"), newVal);	// �̶������֤  0 ˢ���֤¼�롢1 �ֶ�¼�롢2ͨ��ɨ��֤��¼�롢4 �ֻ�����¼��
}
