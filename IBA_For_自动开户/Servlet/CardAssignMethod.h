#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCardAssignMethod : public CServletMethod
{
public :
	CCardAssignMethod(void);
	~CCardAssignMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*
	*/
	void SetCardId(UINT newVal);

	/**
	*������(��)
	*/
	void SetCostExpense(UINT newVal);

	/**
	*Ѻ�𣨷֣�
	*/
	void SetGuarantyMoney(UINT newVal);

	/**
	*֤������
	*/
	void SetPersonalId(UINT newVal);

	/**
	*��������
	*/
	void SetOpenNetbarId(UINT newVal);

	/**
	*
	*/
	void SetMemberName(CString newVal);

	/**
	*�����ɵ�PSK���ܵ����ģ�UNDES��
	*/
	void SetPassword(CString newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);

	/**
	*1: �Դ��� 0:��
	*/
	void SetHasCard(UINT newVal);


public :

	/**
	*
	*/
	UINT GetMemberId();

	/**
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*�����к�
	*/
	CString GetSerialNum();

	/**
	*���Ľ���ʱ��
	*/
	UINT GetTranTime();


};

}