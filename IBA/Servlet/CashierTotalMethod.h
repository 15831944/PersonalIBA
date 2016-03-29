#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCashierTotalMethod : public CServletMethod
{
public :
	CCashierTotalMethod(void);
	~CCashierTotalMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*��ʼʱ�䣬�ձ�ʾ�����ע��ʱ�俪ʼ
	*/
	void SetBeginTime(CString newVal);

	/**
	*����ʱ�䣬�ձ�ʾ������
	*/
	void SetEndTime(CString newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);


public :

	/**
	*������
	*/
	UINT GetCardCount();

	/**
	*���ϼ�
	*/
	UINT GetAmount();


};

}