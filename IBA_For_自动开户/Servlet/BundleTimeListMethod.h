#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CBundleTimeInfo
{
public :

	CBundleTimeInfo();

	/**
	*��ʱID
	*/
	UINT GetBundleId();

	/**
	*1: �ۼƷⶥ 2:���Ԥ��
	*/
	UINT GetBundleType();

	/**
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime();

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime();

	/**
	*ʱ������λ:Сʱ
	*/
	UINT GetTimePass();

	/**
	*��λ: ��
	*/
	UINT GetPrice();

	/**
	*��ע
	*/
	CString GetDescription();


protected :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CBundleTimeListMethod;

	CString FormatCenterTime(CString strServerTime);
};

class CBundleTimeListMethod : public CServletMethod
{
public :
	CBundleTimeListMethod(void);
	~CBundleTimeListMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	CArray<CBundleTimeInfo, CBundleTimeInfo> m_BundleTimeList;

	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д

public :
	
	void SetClassId(UINT newVal);

	/**
	*
	*/
	void SetMemberId(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*������Դ 0:����̨ 1:�ͻ���
	*/
	void SetPosition(UINT newVal);

	/**
	*����Ա(����̨��д���ͻ��˲���)
	*/
	void SetOperator(CString newVal);


public :

	/**
	*��¼��
	*/
	UINT GetRecordCount();

	/**
	*
	*/
	UINT GetBundleId(INT nIndex);

	/**
	*1: �ۼƷⶥ 2:���Ԥ��
	*/
	UINT GetBundleType(INT nIndex);

	/**
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime(INT nIndex);

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime(INT nIndex);

	/**
	*ʱ������λ:Сʱ
	*/
	UINT GetTimePass(INT nIndex);

	/**
	*��λ: ��
	*/
	UINT GetPrice(INT nIndex);

	/**
	*��ע
	*/
	CString GetDescription(INT nIndex);
};

}