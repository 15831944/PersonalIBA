#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CAuditMethod : public CServletMethod
{
public :
	CAuditMethod(void);
	~CAuditMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	CString GetMethodName();

public :
	
	/**
	*�û���
	*/
	void SetMemberId(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*1:������ 2:������ 3:Ħ�� 4:����
	*/
	void SetAuditType(UINT newVal);

	/**
	*�����ӿ����ID
	*/
	void SetAuditId(CString newVal);


public :


};

}