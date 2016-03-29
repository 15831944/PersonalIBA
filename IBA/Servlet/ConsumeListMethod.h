#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CConsumeInfo
{
public :
	
	CConsumeInfo();

	/**
	*��������
	*/
	UINT GetNetBarId();

	/**
	*��������
	*/
	UINT GetConsumeType();

	/**
	*��ʼʱ��,ѭ����
	*/
	CString GetBeginTime();

	/**
	*����ʱ��,ѭ����
	*/
	CString GetEndTime();

	/**
	*��ʵ�������ѽ��,ѭ����
	*/
	INT GetAmount();

	/**
	*��ϸҵ�����ƣ�����Ϊ��
	*/
	CString GetBusinessName();

	/**
	*��ע������Ϊ��
	*/
	CString GetDescription();

private :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CConsumeListMethod;

};

typedef CArray<CConsumeInfo, CConsumeInfo&> CConsumeInfoArray;

class CConsumeListMethod : public CServletMethod
{
public :
	CConsumeListMethod(void);
	~CConsumeListMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	BOOL DecodeResp(CString strResp);

	CConsumeInfoArray m_ConsumeInfoList;

public :
	
	/**
	*�û���
	*/
	void SetMemberId(UINT newVal);

	/**
	*�������ͣ�0: ȫ�� 1:���� 2:��ֵ 3:������ 4����ţ���� 5:������(��) 6:�˿� 7����ֵ����8����ţ��ֵ
	*/
	void SetConsumeType(UINT newVal);

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

	/**
	*1: ��Ҫ������ϸҵ������ 0:����Ҫ 2:ͬʱ��Ҫ��ע
	*/
	void SetNeedBusinessName(UINT newVal);


public :

	/**
	*��¼����, Ŀǰ���100��
	*/
	UINT GetRecordCount();

	/**
	*��������
	*/
	UINT GetNetBarId(INT nIndex);

	/**
	*��������
	*/
	UINT GetConsumeType(INT nIndex);

	/**
	*��ʼʱ��,ѭ����
	*/
	CString GetBeginTime(INT nIndex);

	/**
	*����ʱ��,ѭ����
	*/
	CString GetEndTime(INT nIndex);

	/**
	*��ʵ�������ѽ��,ѭ����
	*/
	INT GetAmount(INT nIndex);

	/**
	*��ϸҵ�����ƣ�����Ϊ��
	*/
	CString GetBusinessName(INT nIndex);

	/**
	*��ע������Ϊ��
	*/
	CString GetDescription(INT nIndex);

};

}