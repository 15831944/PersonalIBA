#pragma once

#include "servletmethod.h"

namespace NS_SERVLET
{

class CShiftStoreInMethod;

class CShiftStoreInRes
{
public :
	
	CShiftStoreInRes()
	{
		m_RespFields.AddRespField(_T("voucherNo"), EPT_NUMBER, 16);
		m_RespFields.AddRespField(_T("commodityCode"), EPT_STRING, 16);
		m_RespFields.AddRespField(_T("payType"), EPT_NUMBER, 1);
		m_RespFields.AddRespField(_T("quantity"), EPT_NUMBER, 6);
		m_RespFields.AddRespField(_T("amount"), EPT_NUMBER, 6);
	}

	CString GetVoucherNo()// ���ݱ��
	{
		return m_RespFields.GetValueAsString(_T("voucherNo"));
	}
	CString GetCommodityCode()// ��Ʒ����
	{
		return m_RespFields.GetValueAsString(_T("commodityCode"));
	}
	UINT GetPayType()// ֧�����ͣ�1�ֽ� 0���ֽ�
	{
		UINT nVal = m_RespFields.GetValueAsUINT(_T("payType"));
		IBA_CHECK_UINT(nVal);
		return nVal;
	}
	UINT GetQuantity()// ����
	{
		UINT nVal = m_RespFields.GetValueAsUINT(_T("quantity"));
		IBA_CHECK_UINT(nVal);
		return nVal;
	}
	UINT GetAmount()// ���
	{
		UINT nVal = m_RespFields.GetValueAsUINT(_T("amount"));
		IBA_CHECK_UINT(nVal);
		return nVal;
	}

public :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CShiftStoreInMethod;

};

typedef CArray<CShiftStoreInRes, CShiftStoreInRes&> CShiftStoreInResArray;


//
//class:Method: CShiftStoreInMethod ����¼��ѯ�ӿ�
//
class CShiftStoreInMethod :public CServletMethod
{
public:
	CShiftStoreInMethod(void);
	~CShiftStoreInMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	
	virtual BOOL DecodeResp(CString strResp);

public:
	//���ID
	void SetDutyId(UINT newVal);

	//����̨ID
	void SetCashRegisterId(UINT newVal);

	//����Ա����
	void SetOperator(CString newVal);
	

	

	/////////////////////������///////////////////////
	CString GetOndutyTime(); //YYYYMMDDhhmmss
	UINT GetRecordCount();

	/////////////////////ѭ���ķ���ֵ//////////////////
	
	CShiftStoreInRes& GetStoreRecord(INT nIndex);

	
private:
	CShiftStoreInResArray m_arrayRes;
};

}
