#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CModifyMemberMethod : public CServletMethod
{
public :
	CModifyMemberMethod(void);
	~CModifyMemberMethod(void);

protected :

	virtual CString GetMethodName();

	void AddParams();
	void AddRespFields();
public :
	
	/**
	*�û���¼��
	*/
	void SetMemberName(CString newVal);

	/**
	*��UNDES��
	*/
	void SetPassword(CString newVal);

	/**
	*ԭ֤������
	*/
	void SetOldPersonalId(CString newVal);

	/**
	*�û���ʵ����
	*/
	void SetUsername(CString newVal);

	/**
	*0:�� 1:Ů
	*/
	void SetSex(UINT newVal);

	/**
	*֤������
	*/
	void SetIdType(UINT newVal);

	/**
	*֤������
	*/
	void SetPersonalId(CString newVal);

	/**
	*��֤����
	*/
	void SetIdDepart(CString newVal);

	/**
	*���Ҵ���
	*/
	void SetCountryId(CString newVal);

	/**
	*סַ
	*/
	void SetAddress(CString newVal);

	/**
	*Email��ַ
	*/
	void SetEmail(CString newVal);

	/**
	*�绰
	*/
	void SetTel(CString newVal);

	/**
	*����
	*/
	void SetJob(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);

	/**
	*
	*/
	void SetOpenNetBarId(UINT newVal);


public :


};

}