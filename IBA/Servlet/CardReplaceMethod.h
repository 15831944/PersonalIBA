#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCardReplaceMethod : public CServletMethod
{
public :
	CCardReplaceMethod(void);
	~CCardReplaceMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	virtual CString GetMethodName(); //!<���ص�ǰ��������

public :
	
	/**
	*
	*/
	void SetCardId(UINT newVal);

	/**
	*֤������
	*/
	void SetPersonalId(CString newVal);

	/**
	*��������
	*/
	void SetOpenNetbarId(UINT newVal);

	/**
	*
	*/
	void SetMemberName(CString newVal);

	/**
	*�¿�PSK���ܣ�UNDES��
	*/
	void SetPassword(CString newVal);

	/**
	*
	*/
	void SetLastpay(UINT newVal);

	/**
	*
	*/
	void SetLastRefNo(UINT newVal);

	/**
	*
	*/
	void SetEndUsingTime(CString newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);


public :

	/**
	*
	*/
	UINT GetMemberId();

	/**
	*ϵͳ�ο���,���ڿ�����
	*/
	UINT GetRefNo1();

	/**
	*ϵͳ�ο���,�����˿���
	*/
	UINT GetRefNo2();

	/**
	*�����к�
	*/
	CString GetSerialNum();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();


};

}