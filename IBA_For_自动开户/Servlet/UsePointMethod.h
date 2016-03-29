#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CUsePointMethod : public CServletMethod
{
public :
	CUsePointMethod(void);
	~CUsePointMethod(void);

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
	*ʹ�û�������
	*/
	void SetPoint(UINT newVal);

	/**
	*��Ʒ����
	*/
	void SetQuantity(UINT newVal);

	/**
	*��Ʒ����
	*/
	void SetArticle(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);

	/**
	*ҵ�����ͣ�����DODONEW�ۻ���
	*/
	void SetBusinessType(UINT newVal);


public :

	/**
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*�û���ǰ����
	*/
	UINT GetCurrentPort();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();


};

}