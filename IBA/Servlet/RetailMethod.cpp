#include "stdafx.h"
#include "RetailMethod.h"
#include "..\IBAHelpper.h"
#include "..\Encrypt.h"

using namespace NS_SERVLET;

CRetailMethod::CRetailMethod(void)
: CServletMethod(_T("Retail")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CRetailMethod::~CRetailMethod(void)
{
}

void CRetailMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("cardId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("Password"), EPT_STRING, 16);
	m_Params.AddParam(_T("storeId"), EPT_STRING, 4);
	m_Params.AddParam(_T("payType"), EPT_NUMBER, 1);
	m_Params.AddParam(_T("retailType"), EPT_NUMBER, 1);
	m_Params.AddParam(_T("voucherNo"), EPT_STRING, 12);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);

	m_Params.AddParam(_T("Count"), EPT_NUMBER, 4);
	m_Params.AddParam(_T("SellRecords"), EPT_STRING, 0, EPLT_OWN);

	m_Params.AddParam(_T("termTime"), EPT_NUMBER, 14);
	m_Params.AddParam(_T("cashRegisterId"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("dutyId"), EPT_NUMBER, 12);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);
	
}

CString CRetailMethod::GetMethodName()
{
	CString strTmp;

	strTmp = m_strMethodName;

	if (GetCardType() == ECT_ID)
	{
		strTmp.Insert(0, _T("ID"));
	}

	return strTmp;
}

void CRetailMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CRetailMethod::SetCardId(UINT newVal)
{
	m_Params.SetValue(_T("cardId"), newVal);
}

void CRetailMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CRetailMethod::SetPassword(CString newVal)
{
	CString strTmp;

	if (newVal.IsEmpty())
	{
		strTmp = _T("PasswordPassword");
	}
	else
	{
		CIBAHelpper::FillString(newVal, 8, '0');

		CEncrypt::CalcMac(newVal, strTmp, m_strPSK, FALSE);
	}
	
	m_Params.SetValue(_T("Password"), strTmp);
}

void CRetailMethod::SetStoreId(UINT newVal)
{
	m_Params.SetValue(_T("storeId"), newVal);
}

void CRetailMethod::SetPayType(UINT newVal)
{
	m_Params.SetValue(_T("payType"), newVal);
}

void CRetailMethod::SetRetailType(UINT newVal)
{
	m_Params.SetValue(_T("retailType"), newVal);
}

void CRetailMethod::SetVoucherNo(CString newVal)
{
	m_Params.SetValue(_T("voucherNo"), newVal);
}

void CRetailMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CRetailMethod::SetCount(UINT newVal)
{
	m_Params.SetValue(_T("Count"), newVal);
}

void CRetailMethod::SetSellRecords(CSellRecordArray& SellRecords)
{
	SetCount(SellRecords.GetCount());

	CInputParamList aParams;
	aParams.AddParam(_T("commodityCode"), EPT_STRING, 16);
	aParams.AddParam(_T("quantity"), EPT_NUMBER, 6);
	aParams.AddParam(_T("price"), EPT_NUMBER, 6);
	aParams.AddParam(_T("amount"), EPT_NUMBER, 6);
	aParams.AddParam(_T("point"), EPT_NUMBER, 6);

	CString strTmp;

	for (INT i = 0; i < SellRecords.GetCount(); i++)
	{
		aParams.SetValue(_T("commodityCode"), SellRecords.GetAt(i).GetCommodityCode());
		aParams.SetValue(_T("quantity"), SellRecords.GetAt(i).GetQuantity());
		aParams.SetValue(_T("price"), SellRecords.GetAt(i).GetPrice());
		aParams.SetValue(_T("amount"), SellRecords.GetAt(i).GetAmount());
		aParams.SetValue(_T("point"), SellRecords.GetAt(i).GetPoint());

		strTmp.Append(aParams.GetAllValue());
	}

	m_Params.SetValue(_T("SellRecords"), strTmp);
}

void CRetailMethod::SetTermTime(CString newVal)
{
	m_Params.SetValue(_T("termTime"), newVal);
}

void CRetailMethod::SetCashRegisterId(UINT newVal)
{
	m_Params.SetValue(_T("cashRegisterId"), newVal);
}

void CRetailMethod::SetDutyId(UINT newVal)
{
	m_Params.SetValue(_T("dutyId"), newVal);
}

void CRetailMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("refNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("voucherNo"), EPT_STRING, 12);
	m_RespFields.AddRespField(_T("extInfo"), EPT_STRING, 3, FALSE);
	m_RespFields.AddRespField(_T("tranTime"), EPT_STRING, 14);
}

UINT CRetailMethod::GetRefNo()
{
	return m_RespFields.GetValueAsUINT(_T("refNo"));
}

CString CRetailMethod::GetVoucherNo()
{
	return m_RespFields.GetValueAsString(_T("voucherNo"));
}

CString CRetailMethod::GetExtInfo()
{
	return m_RespFields.GetValueAsString(_T("extInfo"));
}

CString CRetailMethod::GetTranTime()
{
	return m_RespFields.GetValueAsString(_T("tranTime"));
}

//******************************************************************************

CSellRecord::CSellRecord()
{
	m_nAmount = 0;
	m_nPoint = 0;
	m_nPrice = 0;
	m_nQuantity = 0;
}

void CSellRecord::SetCommodityCode(CString newVal)
{
	m_strCommodityCode = newVal;
}

void CSellRecord::SetQuantity(UINT newVal)
{
	m_nQuantity = newVal;
}

void CSellRecord::SetPrice(UINT newVal)
{
	m_nPrice = newVal;
}

void CSellRecord::SetAmount(UINT newVal)
{
	m_nAmount = newVal;
}

void CSellRecord::SetPoint(UINT newVal)
{
	m_nPoint = newVal;
}

void CSellRecord::SetCommodityBarCode(CString newVal)
{
	m_strCommodityBarCode = newVal;
}

void CSellRecord::SetCommodityName(CString newVal)
{
	m_strCommodityName = newVal;
}

/******************************���߹���****************************/

CRetailOnlineMethod::CRetailOnlineMethod(void)
: CServletMethod(_T("RetailOnline")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CRetailOnlineMethod::~CRetailOnlineMethod(void)
{
}

void CRetailOnlineMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	m_Params.AddParam(_T("memberId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("cardId"), EPT_NUMBER, 12);
	m_Params.AddParam(_T("netBarId"), EPT_NUMBER, 5);
	m_Params.AddParam(_T("Password"), EPT_STRING, 16);
	m_Params.AddParam(_T("storeId"), EPT_STRING, 4);
	m_Params.AddParam(_T("payType"), EPT_NUMBER, 1);
	m_Params.AddParam(_T("retailType"), EPT_NUMBER, 1);
	m_Params.AddParam(_T("voucherNo"), EPT_STRING, 12);
	m_Params.AddParam(_T("Operator"), EPT_STRING, 10);

	m_Params.AddParam(_T("Count"), EPT_NUMBER, 4);
	m_Params.AddParam(_T("SellRecords"), EPT_STRING, 0, EPLT_OWN);

	m_Params.AddParam(_T("termTime"), EPT_NUMBER, 14);
	m_Params.AddParam(_T("cashRegisterId"), EPT_NUMBER, 2);
	m_Params.AddParam(_T("dutyId"), EPT_NUMBER, 12);

	m_Params.SetValue(_T("netbarId"), m_nNetbarId);

}

void CRetailOnlineMethod::SetMemberId(UINT newVal)
{
	m_Params.SetValue(_T("memberId"), newVal);
}

void CRetailOnlineMethod::SetNetBarId(UINT newVal)
{
	m_Params.SetValue(_T("netBarId"), newVal);
}

void CRetailOnlineMethod::SetStoreId(UINT newVal)
{
	m_Params.SetValue(_T("storeId"), newVal);
}

void CRetailOnlineMethod::SetPayType(UINT newVal)
{
	m_Params.SetValue(_T("payType"), newVal);
}

void CRetailOnlineMethod::SetRetailType(UINT newVal)
{
	m_Params.SetValue(_T("retailType"), newVal);
}

void CRetailOnlineMethod::SetVoucherNo(CString newVal)
{
	m_Params.SetValue(_T("voucherNo"), newVal);
}

void CRetailOnlineMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("Operator"), newVal);
}

void CRetailOnlineMethod::SetCount(UINT newVal)
{
	m_Params.SetValue(_T("Count"), newVal);
}

void CRetailOnlineMethod::SetSellRecords(CSellRecordArray& SellRecords)
{
	SetCount(SellRecords.GetCount());

	CInputParamList aParams;
	aParams.AddParam(_T("commodityCode"), EPT_STRING, 16);
	aParams.AddParam(_T("quantity"), EPT_NUMBER, 6);
	aParams.AddParam(_T("price"), EPT_NUMBER, 6);
	aParams.AddParam(_T("amount"), EPT_NUMBER, 6);
	aParams.AddParam(_T("point"), EPT_NUMBER, 6);

	CString strTmp;

	for (INT i = 0; i < SellRecords.GetCount(); i++)
	{
		aParams.SetValue(_T("commodityCode"), SellRecords.GetAt(i).GetCommodityCode());
		aParams.SetValue(_T("quantity"), SellRecords.GetAt(i).GetQuantity());
		aParams.SetValue(_T("price"), SellRecords.GetAt(i).GetPrice());
		aParams.SetValue(_T("amount"), SellRecords.GetAt(i).GetAmount());
		aParams.SetValue(_T("point"), SellRecords.GetAt(i).GetPoint());

		strTmp.Append(aParams.GetAllValue());
	}

	m_Params.SetValue(_T("SellRecords"), strTmp);
}

void CRetailOnlineMethod::SetTermTime(CString newVal)
{
	m_Params.SetValue(_T("termTime"), newVal);
}

void CRetailOnlineMethod::SetCashRegisterId(UINT newVal)
{
	m_Params.SetValue(_T("cashRegisterId"), newVal);
}

void CRetailOnlineMethod::SetDutyId(UINT newVal)
{
	m_Params.SetValue(_T("dutyId"), newVal);
}

void CRetailOnlineMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

	m_RespFields.AddRespField(_T("refNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("voucherNo"), EPT_STRING, 12);
	m_RespFields.AddRespField(_T("extInfo"), EPT_STRING, 3, FALSE);
	m_RespFields.AddRespField(_T("tranTime"), EPT_STRING, 14);
}

UINT CRetailOnlineMethod::GetRefNo()
{
	return m_RespFields.GetValueAsUINT(_T("refNo"));
}

CString CRetailOnlineMethod::GetVoucherNo()
{
	return m_RespFields.GetValueAsString(_T("voucherNo"));
}

CString CRetailOnlineMethod::GetExtInfo()
{
	return m_RespFields.GetValueAsString(_T("extInfo"));
}

CString CRetailOnlineMethod::GetTranTime()
{
	return m_RespFields.GetValueAsString(_T("tranTime"));
}
