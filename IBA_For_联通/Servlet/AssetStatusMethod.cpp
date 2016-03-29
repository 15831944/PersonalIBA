#include "stdafx.h"
#include "AssetStatusMethod.h"

using namespace NS_SERVLET;

CAssetStatusMethod::CAssetStatusMethod(void)
: CServletMethod(_T("AssetStatus")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CAssetStatusMethod::~CAssetStatusMethod(void)
{
}

void CAssetStatusMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	CServletMethod::AddParams();
	m_Params.AddParam(_T("termId"), EPT_STRING, 8);
	m_Params.AddParam(_T("assetId"), EPT_NUMBER, 8);
	m_Params.AddParam(_T("assetStatus"), EPT_STRING, 30);
}

void CAssetStatusMethod::SetTermId(CString newVal)
{
	if (newVal.GetLength() > 8)
	{
		newVal = newVal.Right(8);
	}

	m_Params.SetValue(_T("termId"), newVal);
}

void CAssetStatusMethod::SetAssetId(UINT newVal)
{
	m_Params.SetValue(_T("assetId"), newVal);
}

void CAssetStatusMethod::SetAssetStatus(CString newVal)
{
	m_Params.SetValue(_T("assetStatus"), newVal);
}

void CAssetStatusMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}

