#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CUpgradeMethod : public CServletMethod
{
public :
	CUpgradeMethod(void);
	~CUpgradeMethod(void);

protected :

	void AddParams();
	void AddRespFields();

public :
	
	/**
	*�û�ID
	*/
	void SetMemberId(UINT newVal);

	/**
	*����ID
	*/
	void SetNetBarId(UINT newVal);

	/**
	*ԭ����
	*/
	void SetOldClassId(UINT newVal);

	/**
	*�¼���
	*/
	void SetNewClassId(UINT newVal);

	/**
	*��������
	*/
	void SetReason(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();

	UINT GetRefNo();
};

}