#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CResetPwdMethod : public CServletMethod
{
public :
	CResetPwdMethod(void);
	~CResetPwdMethod(void);

protected :

	void AddParams();
	void AddRespFields();
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
	*��PSK���ܵ������루UNDES��
	*/
	void SetPassword(CString newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);


public :


};

}