#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CStoreQuantityMethod : public CServletMethod
{
public :
	CStoreQuantityMethod(void);
	~CStoreQuantityMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	/**
	*ѭ����
	*/
	CString GetCommodityInfo();

public :
	
	/**
	*�ֿ�ID
	*/
	void SetStoreId(UINT newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*��¼����
	*/
	UINT GetRecordCount();

	/**
	*��Ʒ���룬ѭ����
	*/
	CString GetCommodityCode(UINT nIndex);
	
	/**
	*���������ѭ����
	*/
	UINT GetRemainQuantity(UINT nIndex);


};

}