#pragma once
#include "servletmethod.h"

// 2011/05/09-gxx: ���
// class: CConsumeListMethodEx ���������Ѳ�ѯ�ӿ�,��ѯ��������Ѽ�¼
//
namespace NS_SERVLET
{

class CConsumeInfoEx
{
public :
	
	CConsumeInfoEx();

	UINT GetMemberID();
	
	CString GetCostTime();

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

public :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CConsumeListExMethod;

};

typedef CArray<CConsumeInfoEx, CConsumeInfoEx&> CConsumeInfoExArray;

class CConsumeListMethodEx : public CServletMethod
{
public :
	CConsumeListMethodEx(void);
	~CConsumeListMethodEx(void);

protected :

	void AddParams();
	void AddRespFields();

	BOOL DecodeResp(CString strResp);

	CConsumeInfoExArray m_ConsumeInfoList;

public :
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
	*�û�ID
	*/
	UINT GetMemberID(INT nIndex);

	/**
	*����ʱ��
	*/
	CString GetCostTime(INT nIndex);

	/**
	*���ѽ��
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