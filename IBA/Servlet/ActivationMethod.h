#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{
//
// ��Ӽ���ķ���
//
class CActivationMethod : public CServletMethod
{
public :
	CActivationMethod(void);
	~CActivationMethod(void);

	enum ACTIVATION_MODE{

		ACTIVATION_YES = 1,  // ����
		ACTIVATION_NO = 2    // ������

	};

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
	* ���ü���ģʽ ACTIVATION_YES/NO
	*/
	void SetMode(UINT newVal);

	/**
	* ���ÿ۵�����
	*/
	void SetDotType(UINT newVal);

public :


};

}