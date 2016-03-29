#include "stdafx.h"
#include "NetBarRoomMethod.h"

using namespace NS_SERVLET;

CNetbarRoomInfo::CNetbarRoomInfo()
{
	m_RespFields.AddRespField(_T("pcClass"), EPT_NUMBER, 2);
	m_RespFields.AddRespField(_T("areaName"), EPT_STRING, 30);
	m_RespFields.AddRespField(_T("memberId"), EPT_NUMBER, 12);
	m_RespFields.AddRespField(_T("roomBeginTime"), EPT_STRING, 14);
}

UINT NS_SERVLET::CNetbarRoomInfo::GetPcClass()
{
	return m_RespFields.GetValueAsUINT(_T("pcClass"));
}

CString NS_SERVLET::CNetbarRoomInfo::GetAreaName()
{
	return m_RespFields.GetValueAsString(_T("areaName"));
}

UINT NS_SERVLET::CNetbarRoomInfo::GetMemberId()
{
	return m_RespFields.GetValueAsUINT(_T("memberId"));
}

CString NS_SERVLET::CNetbarRoomInfo::GetBeginTime()
{
	return m_RespFields.GetValueAsString(_T("roomBeginTime"));
}

//******************************************************************************

CNetbarRoomMethod::CNetbarRoomMethod(void)
: CServletMethod(_T("NetBarRoom")) //���ķ�������
{
#ifndef _DEBUG
	m_bUseQueryHost = TRUE; //ʹ�ò�ѯ��
	m_nRevTimeout = 120;	//��ʱ2����
#endif // _DEBUG


	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CNetbarRoomMethod::~CNetbarRoomMethod(void)
{
}

void CNetbarRoomMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	CServletMethod::AddParams();
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);
}

BOOL CNetbarRoomMethod::DecodeResp(CString strResp)
{
	BOOL bRet = CServletMethod::DecodeResp(strResp);

	UINT nCount = GetRecordCount();

	m_RoomInfoList.RemoveAll();

	if (bRet && nCount > 0)
	{
		m_RoomInfoList.SetSize(nCount);

		INT nIndex = m_RespFields.GetAllLength();

		for (INT i = 0; i < m_RoomInfoList.GetCount(); i++)
		{
			CString strTmp = strResp.Mid(nIndex);
			m_RoomInfoList.GetAt(i).m_RespFields.DecodeResp(strTmp);
			nIndex += m_RoomInfoList.GetAt(i).m_RespFields.GetAllLength();
		}
	}

	return bRet;
}

void CNetbarRoomMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CNetbarRoomMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("recordCount"), EPT_NUMBER, 2);
}

UINT CNetbarRoomMethod::GetRecordCount()
{
	return m_RespFields.GetValueAsUINT(_T("recordCount"));
}

UINT NS_SERVLET::CNetbarRoomMethod::GetRoomID(INT nIndex)
{
	return m_RoomInfoList.GetAt(nIndex).GetPcClass();
}

CString NS_SERVLET::CNetbarRoomMethod::GetRoomName(INT nIndex)
{
	return m_RoomInfoList.GetAt(nIndex).GetAreaName();
}

UINT NS_SERVLET::CNetbarRoomMethod::GetMemberID(INT nIndex)
{
	return m_RoomInfoList.GetAt(nIndex).GetMemberId();
}

CString NS_SERVLET::CNetbarRoomMethod::GetOpenTime(INT nIndex)
{
	return m_RoomInfoList.GetAt(nIndex).GetBeginTime();
}