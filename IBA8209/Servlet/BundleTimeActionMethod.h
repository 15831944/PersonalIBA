#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CBundleTimeActionMethod : public CServletMethod
{
public :
	CBundleTimeActionMethod(void);
	~CBundleTimeActionMethod(void);

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


public :

	/**
	*��ʱ����ID��0���ް�ʱ
	*/
	UINT GetNetBarId();

	/**
	*��ʱ�ն���ˮ��
	*/
	UINT GetBundleTimeSerialNo();

	/**
	*��ʱ����
	*/
	UINT GetPayType();

	/**
	*��ʱ��������
	*/
	UINT GetPcClass();

	/**
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime();

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime();

	/**
	*��ʱ�ܽ��
	*/
	UINT GetAmount1();

	/**
	*�����Ѱ�ʱ���
	*/
	UINT GetAmount2();

	/**
	*ϵͳ�ο���,0: ������
	*/
	UINT GetRefNo();

	/**
	*�����Ѷ�����
	*/
	UINT GetFreezeMoney();

	/**
	*��ʱID
	*/
	UINT GetTimeId();


};

}