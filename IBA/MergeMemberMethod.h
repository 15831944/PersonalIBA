#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CMergeMemberMethod : public CServletMethod
{
public :
	CMergeMemberMethod(void);
	~CMergeMemberMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*��ǰ�û���
	*/
	void SetMemberId(UINT newVal);

	/**
	*��ǰ����ID
	*/
	void SetNetBarId(UINT newVal);

	/**
	*Ŀ���û���
	*/
	void SetDestMemberId(UINT newVal);

	/**
	*��ǰ�û�����,�����ӿ����봫"PASSWORDPASSWORD"ʱ�����Ĳ������롣
	*/
	void SetPassword(CString newVal);

	/**
	*����Ա������̨��ѯ��д
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


};

}