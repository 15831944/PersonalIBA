#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CBundleTime2Method : public CServletMethod
{
public :
	CBundleTime2Method(void);
	~CBundleTime2Method(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û�ID������32λ������ʾ
	*/
	void SetMemberId(UINT newVal);

	/**
	*����ID������16λ������ʾ
	*/
	void SetNetbarId(UINT newVal);

	/**
	*���Ԥ�۰�ʱID
	*/
	void SetTimeId(UINT newVal);

	/**
	*����̨ID�������Ϊ8���ֽ�
	*/
	void SetTermId(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*��ʱ���
	*/
	UINT GetAmount();


};

}