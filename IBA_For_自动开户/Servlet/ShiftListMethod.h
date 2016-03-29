#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CShiftRecord
{
public:

	CShiftRecord();

	/**
	*�ϰ�ʱ��
	*/
	CString GetOnDutyTime();

	/**
	*�°�ʱ��
	*/
	CString GetOffDutyTime();

	/**
	*�������: �硢�С���
	*/
	INT GetShiftType();

	/**
	*����Ա
	*/
	CString GetOperator();

	/**
	*�ϰ౸�ý�
	*/
	INT GetReserveAmount();

	/**
	*Ѻ��
	*/
	INT GetDeposit();

	/**
	*������
	*/
	INT GetCostExpense();

	/**
	*��ֵ���
	*/
	INT GetCreditAmount();

	/**
	*�˿���
	*/
	INT GetReturnAmount();

	/**
	*�ֽ���
	*/
	INT GetCashPay();

	/**
	*��Ʒ�ɹ�֧��
	*/
	INT GetStoreInAmount();

	/**
	*����֧��
	*/
	INT GetPayout();

	/**
	*������һ��
	*/
	INT GetRemainAmount();

	/**
	*�Ͻ����
	*/
	INT GetFetchAmount();

	/**
	*�˻�����
	*/
	INT GetNetBarPay();

	/**
	*��ֵ����ֵ
	*/
	INT GetCardCredit();

	/**
	*��ע
	*/
	CString GetDescription();

	// 2011/08/03-8201-gxx: �㿨����
	INT GetDiankaSold();

protected :

	friend class CShiftListMethod;

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�
};

class CShiftListMethod : public CServletMethod
{
public :
	CShiftListMethod(void);
	~CShiftListMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д

	CArray<CShiftRecord, CShiftRecord> m_ShiftList;

public :
	
	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*����ʱ��
	*/
	CString GetServerTime();

	/**
	*�������Ľ����¼��
	*/
	INT GetRecordCount();

	/**
	*�ϰ�ʱ��
	*/
	CString GetOnDutyTime(INT nIndex);

	/**
	*�°�ʱ��
	*/
	CString GetOffDutyTime(INT nIndex);

	/**
	*�������: �硢�С���
	*/
	INT GetShiftType(INT nIndex);

	/**
	*����Ա
	*/
	CString GetOperator(INT nIndex);

	/**
	*�ϰ౸�ý�
	*/
	INT GetReserveAmount(INT nIndex);

	/**
	*Ѻ��
	*/
	INT GetDeposit(INT nIndex);

	/**
	*������
	*/
	INT GetCostExpense(INT nIndex);

	/**
	*��ֵ���
	*/
	INT GetCreditAmount(INT nIndex);

	/**
	*�˿���
	*/
	INT GetReturnAmount(INT nIndex);

	/**
	*�ֽ���
	*/
	INT GetCashPay(INT nIndex);

	/**
	*��Ʒ�ɹ�֧��
	*/
	INT GetStoreInAmount(INT nIndex);

	/**
	*����֧��
	*/
	INT GetPayout(INT nIndex);

	/**
	*������һ��
	*/
	INT GetRemainAmount(INT nIndex);

	/**
	*�Ͻ����
	*/
	INT GetFetchAmount(INT nIndex);

	/**
	*�˻�����
	*/
	INT GetNetBarPay(INT nIndex);

	/**
	*��ֵ����ֵ
	*/
	INT GetCardCredit(INT nIndex);

	/**
	*��ע
	*/
	CString GetDescription(INT nIndex);

	// 2011/08/03-8201-gxx: �㿨����
	INT GetDiankaSold(INT i);

};

}