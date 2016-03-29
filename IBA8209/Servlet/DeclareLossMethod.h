#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CDeclareLossMethod : public CServletMethod
{
public :
	CDeclareLossMethod(void);
	~CDeclareLossMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	virtual CString GetMethodName();					//!<���ص�ǰ��������;

public :
	
	/**
	*֤������
	*/
	void SetPersonalId(CString newVal);

	/**
	*�������ɣ�һ�ڸ�ֵ��netBarId��ͬ
	*/
	void SetOpenNetbarId(UINT newVal);

	/**
	*
	*/
	void SetMemberName(CString newVal);

	/**
	*�����ɵ�PSK���ܣ�UNDES��
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


public :

	/**
	*
	*/
	UINT GetMemberId();


};

}