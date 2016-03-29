#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCheckout2Method : public CServletMethod
{
public :
	CCheckout2Method(void);
	~CCheckout2Method(void);

protected :

	void AddParams();
	void AddRespFields();
	virtual UINT	MakeLocalSerialNo();				//!< ��ȡ������ˮ��

	UINT m_nMemberId;
	
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
	*���Ѷ�֣�
	*/
	void SetCost(UINT newVal);

	/**
	*20040408200012
	*/
	void SetEndUsingTime(CString newVal);

	/**
	*����ID����ţ�ܡ�IC���޿��ϻ�������0�۷ѵ���ǰʱ�䣻��������ԭ��������۷ѡ�
	*/
	void SetSafeLogout(UINT newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);


public :


	UINT GetMemberId() const { return m_nMemberId; }
};

}