#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CTermConsumeInfo
{
public :

	CTermConsumeInfo(void);
	
	/**
	*�û���
	*/
	UINT GetMemberId();

	/**
	*��ʼʱ��,ѭ����(�������)
	*/
	CString GetBeginTime();

	/**
	*����ʱ��,ѭ����(�������)
	*/
	CString GetEndTime();

	/**
	*����λ����
	*/
	UINT GetAmount();
	
private :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CTermConsumeMethod;

};

//******************************************************************************

class CTermConsumeMethod : public CServletMethod
{
public :
	CTermConsumeMethod(void);
	~CTermConsumeMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	BOOL DecodeResp(CString strResp);

	CArray<CTermConsumeInfo, CTermConsumeInfo&>  m_TermConsumeList;

public :
	
	/**
	*�ն˱��
	*/
	void SetTermed(CString newVal);

	/**
	*��ʼʱ�䣬�ձ�ʾ�����ע��ʱ�俪ʼ
	*/
	void SetBeginTime(CString newVal);

	/**
	*����ʱ�䣬�ձ�ʾ������
	*/
	void SetEndTime(CString newVal);

	/**
	*����Ա
	*/
	void SetOperator(CString newVal);


public :

	/**
	*��¼����, Ŀǰ���100��
	*/
	UINT GetRecordCount();

	/**
	*�û���
	*/
	UINT GetMemberId(INT nIndex);

	/**
	*��ʼʱ��,ѭ����(�������)
	*/
	CString GetBeginTime(INT nIndex);

	/**
	*����ʱ��,ѭ����(�������)
	*/
	CString GetEndTime(INT nIndex);

	/**
	*����λ����
	*/
	UINT GetAmount(INT nIndex);

};

}