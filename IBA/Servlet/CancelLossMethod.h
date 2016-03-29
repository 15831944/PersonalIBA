#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCancelLossMethod : public CServletMethod
{
public :
	CCancelLossMethod(void);
	~CCancelLossMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	virtual CString GetMethodName();					//!<���ص�ǰ��������;

public :
	
	/**
	*
	*/
	void SetMemberId(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*֤������
	*/
	void SetPersonalId(CString newVal);

	/**
	*���ģ�PSK����DES��
	*/
	void SetPassword(CString newVal);

	/**
	*����Ա����
	*/
	//void SetOperator(CString newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);


public :


};

}