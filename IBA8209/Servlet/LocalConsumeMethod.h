#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CLocalConsumeMethod : public CServletMethod
{
public :
	CLocalConsumeMethod(void);
	~CLocalConsumeMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*��ǰ�û���
	*/
	void SetMemberId(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*�û�����
	*/
	void SetClassId(UINT newVal);

	/**
	*�˺�(һ����֤������ͬ)
	*/
	void SetSerialNum(CString newVal);

	/**
	*����
	*/
	void SetUserName(CString newVal);

	/**
	*֤������
	*/
	void SetIdNumber(CString newVal);

	/**
	*��ֵ���
	*/
	void SetCreditAmount(UINT newVal);

	/**
	*�������
	*/
	void SetConsumeAmount(UINT newVal);

	/**
	*�˿���
	*/
	void SetReturnAmount(UINT newVal);

	/**
	*����ʱ��yyyymmddhhmmss
	*/
	void SetCheckinTime(CString newVal);

	/**
	*�ǳ�ʱ��yyyymmddhhmmss
	*/
	void SetCheckoutTime(CString newVal);

	/**
	*�ն˺�
	*/
	void SetTermId(CString newVal);

	/**
	*��������
	*/
	void SetPcClass(UINT newVal);

	/**
	*0: �������� 2:���Ԥ�۰�ʱ
	*/
	void SetPayType(UINT newVal);

	/**
	*0���������� 1:�����д���
	*/
	void SetDataStatus(UINT newVal);

	/**
	*����Ա
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
	*ϵͳ�ο���
	*/
	UINT GetRefNO();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();
	
	/**
	*
	*/
	UINT GetMemberID();

};

}