#include "stdafx.h"
#include "DetailDataMethod.h"

using namespace NS_SERVLET;

CDetailDataMethod::CDetailDataMethod(void)
: CServletMethod(_T("DetailData")) //���ķ�������
{
	m_bUseQueryHost = TRUE; //ʹ�ò�ѯ��
	m_nRevTimeout = 120;	//��ʱ2����

	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CDetailDataMethod::~CDetailDataMethod(void)
{
}

void CDetailDataMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("tranType"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("classId"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("beginTime"), EPT_STRING, 14);
	m_Params.AddParam(_T("endTime"), EPT_STRING, 14);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);
	m_Params.AddParam(_T("pageNum"), EPT_NUMBER, 3);

	m_Params.SetValue(_T("netBarId"), m_nNetbarId);
}

void CDetailDataMethod::SetPageNum(UINT newVal)
{
	m_Params.SetValue(_T("pageNum"), newVal);
}

void CDetailDataMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CDetailDataMethod::SetTranType(UINT newVal)
{
	m_Params.SetValue(_T("tranType"), newVal);
}

void CDetailDataMethod::SetClassId(UINT newVal)
{
	m_Params.SetValue(_T("classId"), newVal);
}

void CDetailDataMethod::SetBeginTime(CString newVal)
{
	m_Params.SetValue(_T("beginTime"), newVal);
}

void CDetailDataMethod::SetEndTime(CString newVal)
{
	m_Params.SetValue(_T("endTime"), newVal);
}

void CDetailDataMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CDetailDataMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("serverTime"), EPT_STRING, 14);
	m_RespFields.AddRespField(_T("pageSize"), EPT_NUMBER, 3);
	m_RespFields.AddRespField(_T("recordCount"), EPT_NUMBER, 3);
	//m_RespFields.AddRespField(_T("detailInfo"), EPT_STRING, xxx);
}

BOOL CDetailDataMethod::DecodeResp(CString strResp)
{
	BOOL bRet = CServletMethod::DecodeResp(strResp);

	INT nIndex = m_RespFields.GetAllLength();

	m_strDetailInfo = strResp.Mid(nIndex);

	return bRet;
}


CString CDetailDataMethod::GetServerTime()
{
	return m_RespFields.GetValueAsString(_T("serverTime"));
}

UINT CDetailDataMethod::GetRecordCount()
{
	return m_RespFields.GetValueAsUINT(_T("recordCount"));
}

CString CDetailDataMethod::GetDetailInfo()
{
	//return m_RespFields.GetValueAsString(_T("detailInfo"));

	return m_strDetailInfo;
}


UINT CDetailDataMethod::GetPageSize()
{
	return m_RespFields.GetValueAsUINT(_T("pageSize"));
}