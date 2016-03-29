#include "stdafx.h"
#include "BuckleAmountMethod.h"


using namespace NS_SERVLET;

CBuckleAmountMethod::CBuckleAmountMethod(void)
: CServletMethod(_T("BuckleAmount")) //���ķ�������   ����̨���ؼƷѿ۷�
, m_nMemberId(0)
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CBuckleAmountMethod::~CBuckleAmountMethod(void)
{
}

void CBuckleAmountMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("checkinTime"), EPT_NUMBER, 14);
	m_Params.AddParam(_T("checkoutTime"), EPT_NUMBER, 14);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);
	m_Params.AddParam(_T("amount"), EPT_NUMBER, 6);
	m_Params.AddParam(_T("termId"), EPT_STRING, 8);

	m_Params.AddParam(_T("payType"), EPT_NUMBER, 2);

	m_Params.AddParam(_T("pcClass"), EPT_NUMBER, 2);


	SetNetBarId(m_nNetbarId);
}

UINT CBuckleAmountMethod::MakeLocalSerialNo()
{
	return 0;
}

void CBuckleAmountMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);

	m_nMemberId = newVal;
}

void CBuckleAmountMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CBuckleAmountMethod::SetCheckinTime( CString newVal )
{
	m_Params.SetValue(_T("checkinTime"), newVal);
}

void CBuckleAmountMethod::SetCheckoutTime( CString newVal )
{
	m_Params.SetValue(_T("checkoutTime"), newVal);
}

void CBuckleAmountMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CBuckleAmountMethod::SetBuckleAmount( UINT newVal )
{
	if (newVal % 50 != 0) {
		newVal = newVal - newVal % 50 + 50;
	}
	m_Params.SetValue(_T("amount"), newVal);
}


void CBuckleAmountMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}






void CBuckleAmountMethod::SetTermId( CString newVal )
{
	m_Params.SetValue(_T("termId"), newVal);
}

void CBuckleAmountMethod::SetPcClass( UINT newVal )
{
	m_Params.SetValue(_T("pcClass"), newVal);
}

void CBuckleAmountMethod::SetPayType( UINT newVal )
{
	m_Params.SetValue(_T("payType"), newVal);
}
