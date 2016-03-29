#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCreditInfo
{
public:
	
	CCreditInfo();

	/**
	*��ֵʱ�䣬yyyymmddhhmmss
	*/
	CString GetCreditTime();

	/**
	*�û���
	*/
	UINT GetMemberId();

	/**
	*�ο���
	*/
	UINT GetRefNo();

	/**
	*��ֵ���
	*/
	INT GetCreditAmount();

	/**
	*��ֵ����
	*/
	INT GetCreditPresent();

	/**
	*��ֵ����
	*/
	UINT GetPoint();

protected :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CShiftCreditMethod;

};

typedef CArray<CCreditInfo, CCreditInfo> CCreditInfoArray;

class CShiftCreditMethod : public CServletMethod
{
public :
	CShiftCreditMethod(void);
	~CShiftCreditMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д
	virtual BOOL	OnBegin();							//!< ִ������ǰ
	CCreditInfoArray m_CreditInfos;

public :
	
	/**
	*�û��Ŵ���0����ѯָ���û�
	*/
	void SetMemberId(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*��¼��
	*/
	UINT GetRecordCount();

	/**
	*��ֵʱ�䣬yyyymmddhhmmss
	*/
	CString GetCreditTime(INT nIndex);

	/**
	*�û���
	*/
	UINT GetMemberId(INT nIndex);

	/**
	*�ο���
	*/
	UINT GetRefNo(INT nIndex);

	/**
	*��ֵ���
	*/
	INT GetCreditAmount(INT nIndex);

	/**
	*��ֵ����
	*/
	INT GetCreditPresent(INT nIndex);

	/**
	*��ֵ����
	*/
	UINT GetPoint(INT nIndex);


};

}