#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CLocalModeMethod : public CServletMethod
{
public :
	CLocalModeMethod(void);
	~CLocalModeMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*1:���ö��� 2:ͣ�ö���
	*/
	void SetOperationType(UINT newVal);

	/**
	*�ϴβ���ʱ��
	*/
	void SetLastOperationTime(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :


};

}