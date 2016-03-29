#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CIDCheckCardMethod : public CServletMethod
{
public :
	CIDCheckCardMethod(void);
	~CIDCheckCardMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	virtual BOOL	VerifyResp(CString strResp);		//!< У�鷵�ؼ������ݣ�����д

public :
	
	/**
	*��ӡˢ��ˮ��
	*/
	void SetSerialNum(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


	/**
	*0:�ֶ�����ӡˢ�� 1:ʵ������
	*/
	void SetSerialNumType(UINT newVal);

public :

	/**
	*�����룬��״̬��������ͬ
	*/
	UINT GetErrCode();

	/**
	*���ڿ��޸�
	*/
	UINT GetMemberId();

	CString GetMemberIdAsString();

	/**
	*���ڿ��޸�
	*/
	UINT GetCardId();
	
	CString GetCardIdAsString();

};

}