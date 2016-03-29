#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CBundleTimeMethod : public CServletMethod
{
public :
	CBundleTimeMethod(void);
	~CBundleTimeMethod(void);

protected :

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û�ID������32λ������ʾ
	*/
	void SetMemberId(UINT newVal);

	/**
	*����ID������16λ������ʾ
	*/
	void SetNetbarId(UINT newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*���Ԥ�۰�ʱID
	*/
	void SetTimeId(UINT newVal);

	/**
	*��������
	*/
	void SetPcClass(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);

	/**
	*0:��ͨ��ʱ 1:Ԥ��ʱ 2:ת��ʱ 3�Զ����ʱ
	*/
	void SetReserve(UINT newVal);

	/**
	*��ʱ�˻�
	*/
	void SetAccountType(UINT newVal);

	//��ʼʱ��
	void SetBeginTime(CString newVal);

	//ʱ��
	void SetTimePass(UINT newVal);
	
	//��ʱ���
	void SetBundleAmount(UINT newVal);

	//��ֵ���
	void SetCreditAmount(UINT newVal);

public :

	/**
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime();

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime();

	/**
	*��ʱ���
	*/
	UINT GetAmount();

	/**
	*��ʱID
	*/
	UINT GetTimeId();

	/**
	*	�ͻ����Ƿ���Ҫ���µ���
		1����Ҫ 0:����Ҫ
		ע��״̬�벻Ϊ��Ҳ��Ҫ���ݴ�ֵ�ж��Ƿ���Ҫ���µ���
	*/
	BOOL GetNeedRelogin();
};

}