#include "stdafx.h"
#include "StoreQuantityMethod.h"

using namespace NS_SERVLET;

CStoreQuantityMethod::CStoreQuantityMethod(void)
: CServletMethod(_T("StoreQuantity")) //���ķ�������
{
	m_bUseQueryHost = TRUE;
	m_nRevTimeout = 120;	//��ʱ2����

	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CStoreQuantityMethod::~CStoreQuantityMethod(void)
{
}

void CStoreQuantityMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	CServletMethod::AddParams();
	m_Params.AddParam(_T("storeId"), EPT_NUMBER, 4);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);
}

void CStoreQuantityMethod::SetStoreId(UINT newVal)
{
	m_Params.SetValue(_T("storeId"), newVal);
}

void CStoreQuantityMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CStoreQuantityMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("recordCount"), EPT_NUMBER, 4, TRUE, TRUE);
	m_RespFields.AddRespField(_T("commodityInfo"), EPT_STRING, 22);

}

UINT CStoreQuantityMethod::GetRecordCount()
{
	return m_RespFields.GetValueAsUINT(_T("recordCount"));
}

CString CStoreQuantityMethod::GetCommodityInfo()
{
	return m_RespFields.GetValueAsString(_T("commodityInfo"));
}

CString CStoreQuantityMethod::GetCommodityCode(UINT nIndex)
{
	nIndex = 22 * nIndex;

	CString strTmp = GetCommodityInfo().Mid(nIndex, 22);

	return strTmp.Left(16).Trim();
}

UINT CStoreQuantityMethod::GetRemainQuantity(UINT nIndex)
{
	nIndex = 22 * nIndex;

	CString strTmp = GetCommodityInfo().Mid(nIndex, 22);

	return _ttoi(strTmp.Mid(16).Trim());
}