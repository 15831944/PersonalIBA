#include "stdafx.h"
#include "ResetPwdMethod.h"
#include "..\Encrypt.h"
#include "..\IBAHelpper.h"

using namespace NS_SERVLET;

CResetPwdMethod::CResetPwdMethod(void)
: CServletMethod(_T("ResetPwd")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CResetPwdMethod::~CResetPwdMethod(void)
{
}

void CResetPwdMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	CServletMethod::AddParams();

	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("personalId"), EPT_STRING, 18);
	m_Params.AddParam(_T("password"), EPT_STRING, 16);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);

}

void CResetPwdMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CResetPwdMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CResetPwdMethod::SetPersonalId(CString newVal)
{
	m_Params.SetValue(_T("personalId"), newVal);
}

void CResetPwdMethod::SetPassword(CString newVal)
{
	CString strTmp;

	CIBAHelpper::FillString(newVal, 8, '0');

	CEncrypt::CalcMac(newVal, strTmp, m_strPSK, FALSE);
	//CEncrypt::CalcMac(newVal, strTmp, _T("944FF0E934B33DCC"), FALSE);
	

	m_Params.SetValue(_T("password"), strTmp);
}

void CResetPwdMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CResetPwdMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}

