#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCancelBundleTimeMethod : public CServletMethod
{
public :
	CCancelBundleTimeMethod(void);
	~CCancelBundleTimeMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û�ID������32λ������ʾ
	*/
	void SetMemberId(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*����Ա��ϵͳ����
	*/
	void SetOperator(CString newVal);


public :


};

}