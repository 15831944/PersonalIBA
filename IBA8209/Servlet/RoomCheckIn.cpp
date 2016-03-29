#include "stdafx.h"
#include "RoomCheckIn.h"


using namespace NS_SERVLET;

CRoomCheckIn::CRoomCheckIn(void)
: CServletMethod(_T("RoomCheckin")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CRoomCheckIn::~CRoomCheckIn(void)
{
}

void CRoomCheckIn::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���

	CServletMethod::AddParams();
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("pcClass"), EPT_STRING, 2);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);
	m_Params.AddParam(_T("cashRegisterId"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("dutyId"), EPT_NUMBER, 12);
}


void CRoomCheckIn::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CRoomCheckIn::SetDutyId(UINT newVal)
{
	m_Params.SetValue(_T("dutyId"), newVal);
}

void CRoomCheckIn::SetCashRegisterId(UINT newVal)
{
	m_Params.SetValue(_T("cashRegisterId"), newVal);
}

void CRoomCheckIn::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CRoomCheckIn::SetPCClassID(UINT newVal)
{
	m_Params.SetValue(_T("pcClass"), newVal);
}

void CRoomCheckIn::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("tranTime"), EPT_STRING, 14);
}


CString CRoomCheckIn::GetTranTime()
{
	return m_RespFields.GetValueAsString(_T("tranTime"));
}
