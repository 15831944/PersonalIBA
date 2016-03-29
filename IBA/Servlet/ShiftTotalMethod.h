#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCardInfoRecord
{
public :

	CCardInfoRecord();
	
	UINT GetClassId();
	UINT GetOpenQuantity();
	UINT GetReturnedQuantity();

protected :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CShiftTotalMethod;
};

class CShiftTotalMethod : public CServletMethod
{
public :
	CShiftTotalMethod(void);
	~CShiftTotalMethod(void);

public :

	void SetDutyId(UINT newVal);
	void SetOperator(CString newVal);
	void SetNeedBalance(UINT newVal);
	void SetLastTranTime(CString newVal);
	void SetLastTranType(UINT newVal);
	void SetLastRefNo(UINT newVal);

public :

	CString GetOndutyTime();
	INT GetReserveAmount();
	INT GetDeposit();
	INT GetCostExpense();
	INT GetCreditAmount();
	INT GetReturnedAmount();
	INT GetCashPay();
	INT GetCreditCard();
	INT GetTempBalance();

	INT GetNetBarPay();//�ʻ���������

	INT GetStoreInAmount(); //��Ʒ����֧��

	UINT GetRecordCount();
	CString GetCardInfoRecords();

	UINT GetClassId(INT nIndex);
	UINT GetOpenQuantity(INT nIndex);
	UINT GetReturnedQuantity(INT nIndex);

	// 2011/05/10-gxx: ���ձ�( ���ѣ����)
	INT GetZSBConsume();
	INT GetZSBBalance();

	// 2011/07/26-8201-gxx: �㿨����
	INT GetDiankaAmount();

	// 2014-11-3 - qsc ΢�����
	INT GetWxPayAmount();
protected :

	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д

	void AddParams();
	void AddRespFields();
	CArray<CCardInfoRecord, CCardInfoRecord> m_CardInfoRecords;

};

}