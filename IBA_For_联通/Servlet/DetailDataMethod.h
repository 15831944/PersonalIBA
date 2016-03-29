#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CDetailDataMethod : public CServletMethod
{
public :
	CDetailDataMethod(void);
	~CDetailDataMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д

	CString m_strDetailInfo;

public :
	
	/**
	*Ҫ��ѯ��ҳ��
	*/
	void SetPageNum(UINT newVal);

	/**
	*
	*/
	void SetNetBarId(UINT newVal);

	/**
	*��������: 1���� 2��ֵ 3�˿� 4���� 5��ֵ�� 6�û�����
	*/
	void SetTranType(UINT newVal);

	/**
	*�û�����0����ѯȫ������
	*/
	void SetClassId(UINT newVal);

	/**
	*��ѯ��ʼʱ��
	*/
	void SetBeginTime(CString newVal);

	/**
	*��ѯ��ֹʱ��
	*/
	void SetEndTime(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*����ʱ��
	*/
	CString GetServerTime();

	/**
	*ÿҳ��¼��
	*/
	UINT GetPageSize();

	/**
	*��¼��(���ڵ���100��ʾ��������)
	*/
	UINT GetRecordCount();

	/**
	*�����ע
	*/
	CString GetDetailInfo();


};

}