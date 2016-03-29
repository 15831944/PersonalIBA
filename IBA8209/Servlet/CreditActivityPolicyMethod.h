#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCreditActivityPolicyMethod : public CServletMethod
{
public :
	CCreditActivityPolicyMethod(void);
	~CCreditActivityPolicyMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*��¼��
	*/
	UINT GetRecordCount();

	/**
	*�ID
	*/
	UINT GetActivityId();

	/**
	*�����
	*/
	CString GetActivityName();

	/**
	*�����
	*/
	UINT GetActivityType();

	/**
	*�û����� 0:���޼���
	*/
	UINT GetClassId();

	/**
	*��ͳ�ֵ���
	*/
	UINT GetLimitAmount();

	/**
	*��ʾ������ֵ����0ʱ����ֵ���������limitAmount
	*/
	UINT GetDisplayAmount();

	/**
	*��ʼʱ��yyyymmddhhmmss
	*/
	CString GetStartTime();

	/**
	*��ֹʱ��yyyymmddhhmmss
	*/
	CString GetStopTime();


};

}