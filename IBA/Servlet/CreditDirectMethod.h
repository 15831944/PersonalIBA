#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCreditDirectMethod : public CServletMethod
{
public :
	CCreditDirectMethod(void);
	~CCreditDirectMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û���
	*/
	void SetMemberId(UINT newVal);

	/**
	*���ɱ��
	*/
	void SetNetBarId(UINT newVal);

	/**
	*�˻����� 1:���� 2:���� 3:����
	*/
	void SetAccountType(UINT newVal);

	/**
	*0: ��Ǯ 1:��Ǯ
	*/
	void SetCreditMode(INT newVal);

	/**
	*��ֵ�������Ǹ���
	*/
	void SetAmount(INT newVal);

	/**
	*����̨��ǰʱ�䣬����ʱ��У��
	*/
	void SetTermTime(CString newVal);

	/**
	*����Ա
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


public :

	/**
	*�˻������
	*/
	INT GetAvailableBalance();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();


};

}