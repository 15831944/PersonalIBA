#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCardCreditMethod : public CServletMethod
{
public :
	CCardCreditMethod(void);
	~CCardCreditMethod(void);

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
	*�����,ZS:�ֽ𿨣�XN:���⿨
	*/
	void SetCategory(CString newVal);

	/**
	*�����
	*/
	void SetCardSerial(CString newVal);

	/**
	*�����룬������PSK�����μ���(UNDES)
	*/
	void SetCardPwd(CString newVal);

	/**
	*��ֵ���֣�//���ֶ�����
	*/
	void SetAmount(UINT newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);

	/**
	*ҵ������ID������̨��0
	*/
	void SetBusinessType(UINT newVal);

	/**
	*����������̨���
	*/
	void SetName(CString newVal);

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
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*��ֵ�˻����
	*/
	UINT GetBalance();

	/**
	*���γ�ֵ����
	*/
	UINT GetCreditPresent();

	/**
	*��ǰ���ɿ������
	*/
	UINT GetAvailableBalance();

	/**
	*���γ�ֵ���
	*/
	UINT GetAmount();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();


};

}