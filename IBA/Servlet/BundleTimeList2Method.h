#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{
class CBundleTimeInfo2
{
public :

	CBundleTimeInfo2();

	/**
	*
	*/
	UINT GetBundleId();

	/**
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime();

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime();

	/**
	*1:���� 2:���� 3:�Զ�������
	*/
	UINT GetMode();

	/**
	*�Զ���������mode=1��2��Ч
	*/
	UINT GetDays();

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

	friend class CBundleTimeList2Method;

	CString FormatCenterTime(CString strServerTime);
};


class CBundleTimeList2Method : public CServletMethod
{
public :
	CBundleTimeList2Method(void);
	~CBundleTimeList2Method(void);

protected :

	void AddParams();
	void AddRespFields();

	CArray<CBundleTimeInfo2, CBundleTimeInfo2> m_BundleTimeList;

	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д

public :
	
	/**
	*
	*/
	void SetNetBarId(UINT newVal);

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
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime(INT nIndex);

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime(INT nIndex);

	/**
	*1:���� 2:���� 3:�Զ�������
	*/
	UINT GetMode(INT nIndex);

	/**
	*�Զ���������mode=1��2��Ч
	*/
	UINT GetDays(INT nIndex);

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