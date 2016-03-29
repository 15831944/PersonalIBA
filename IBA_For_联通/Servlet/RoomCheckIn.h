#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CRoomCheckIn : public CServletMethod
{
public :
	CRoomCheckIn(void);
	~CRoomCheckIn(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û�ID������32λ������ʾ
	*/
	void SetMemberId(UINT newVal);

	// ����ID������������ID
	void SetPCClassID(UINT newVal);

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


public :
	CString GetTranTime();
};

}