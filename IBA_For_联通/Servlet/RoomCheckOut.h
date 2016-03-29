#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CRoomCheckOut : public CServletMethod
{
public :
	CRoomCheckOut(void);
	~CRoomCheckOut(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û�ID������32λ������ʾ
	*/
	void SetMemberId(UINT newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);


	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);

	// ����ID,������������ID
	void SetPCClass(UINT newVal);

public :
	CString GetTranTime();
};

}