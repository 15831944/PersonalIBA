#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CAssetStatusMethod : public CServletMethod
{
public :
	CAssetStatusMethod(void);
	~CAssetStatusMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�ն˱��
	*/
	void SetTermId(CString newVal);

	/**
	*�ʲ�ID����������������İ汾���ж������ϵͳID
	*/
	void SetAssetId(UINT newVal);

	/**
	*�ʲ�״��, ����������ǵ�ǰ�汾��
	*/
	void SetAssetStatus(CString newVal);


public :


};

}