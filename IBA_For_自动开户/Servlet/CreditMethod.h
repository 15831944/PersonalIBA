#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCreditMethod : public CServletMethod
{
public:
	CCreditMethod(void);
	~CCreditMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	virtual CString GetMethodName();

public:

	void SaveTranInfo();

	UINT GetMemberId();
	UINT GetAmount();

public:

	void SetMemberId(UINT newVal);
	void SetNetBarId(UINT newVal);
	void SetAmount(UINT newVal);
	void SetOperator(CString newVal);
	void SetNoPresent(UINT newVal);
	void SetTermTime(CString newVal);
	void SetCashRegisterId(UINT newVal);
	void SetDutyId(UINT newVal);
	void SetActivityId(UINT newVal);

public:

	//1	16	Mac	A	У���룬��У�������PSK����
	//	17	6	serialNo	N	���أ����޸ģ�
	//	23	2	Status	N	״̬�룬�μ�״̬���
	//	25	6	refNo	N	ϵͳ�ο���
	//	31	6	balance	N	��ֵ�˻����
	//	37	6	creditPresent	N	���γ�ֵ����
	//	43	6	availableBalance	N	��ǰ���ɿ������
	//	49	6	usePresent	N	���ͳ�ֵ
	//	55	14	Trantime	N	���Ľ���ʱ��


	UINT GetRefNo();
	UINT GetBalance();
	UINT GetCreditPresent();
	UINT GetAvailableBalance();
	UINT GetUsePresent();
	
	CString GetTrantime();

	UINT GetIncreasedPoint(); //�������ӵĻ���

};


}