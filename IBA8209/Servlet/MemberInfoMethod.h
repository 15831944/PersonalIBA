#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CMemberInfo
{
public :

	CMemberInfo();

	/**
	*�û���������
	*/
	UINT GetNetBarId();

	/**
	*�û�ID
	*/
	UINT GetMemberId();

	/**
	*�û�״̬,0����1��ʧ 2ע��
	*/
	UINT GetAvailable();

	/**
	*
	*/
	UINT GetCardId();

	/**
	*�û�����0��ʾ��ʱ�û�
	*/
	UINT GetUserClass();

	/**
	*Ѻ��
	*/
	UINT GetGuarantyMoney();

	/**
	*�û���������
	*/
	UINT GetAccountNetbarId();

	/**
	*�û���¼��
	*/
	CString GetMemberName();

	/**
	*����
	*/
	CString GetUserName();

	/**
	*0��man 1��woman
	*/
	UINT GetSex();

	/**
	*֤������
	*/
	CString GetPersonalId();

	/**
	*֤������ID
	*/
	UINT GetIdType();

	/**
	*��֤��λ
	*/
	CString GetIdDepart();

	/**
	*ISO3166
	*/
	UINT GetCountry();

	/**
	*�����к�
	*/
	CString GetSerailNum();

	/**
	*�����нӿ����ID
	*/
	CString GetRzxId();

	/**
	*�������ӿ����ID
	*/
	CString GetGlwId();

	/**
	*Ħ���ӿ����ID
	*/
	CString GetMgId();

	/**
	*�����ӿ����ID
	*/
	CString GetOtherId();

	/**
	*��ֵ�˻��������
	*/
	UINT GetBalance();

	/**
	*�û���ַ
	*/
	CString GetAddress();

	/**
	*EMAIL��ַ
	*/
	CString GetEmail();

	/**
	*��ϵ�绰
	*/
	CString GetTel();

	/**
	*����
	*/
	CString GetJob();

	/**
	*�û���ǰ����
	*/
	UINT GetCurrentPort();

protected :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�
	
	friend class CMemberInfoMethod;
};

class CMemberInfoMethod : public CServletMethod
{
public :
	CMemberInfoMethod(void);
	virtual ~CMemberInfoMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	virtual CString GetMethodName();
	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д

	CArray<CMemberInfo, CMemberInfo> m_MemberInfos;

public :
	
	/**
	*��ԱID������ʱ��д
	*/
	void SetMemberId(UINT newVal);

	/**
	*ҵ��������
	*/
	void SetNetBarId(UINT newVal);

	/**
	*�û�������¼�������Բ���
	*/
	void SetMemberName(CString newVal);

	/**
	*�û��������������Բ���
	*/
	void SetUsername(CString newVal);

	/**
	*�û�������֤�����룬���Բ���
	*/
	void SetPersonalId(CString newVal);

	/**
	*�����к�
	*/
	void SetSerialNum(CString newVal);

	/**
	*����������ѯ������Ϣ�ж���ʱ��ֻ�����û��Ŵ��ڸ�ֵ���û�
	*/
	void SetMinMemberId(UINT newVal);

	/**
	*�ڲ�����
	*/
	void SetCardId(UINT newVal);

	/**
	*�Ƿ��������(0�������� 1:����)
	*/
	void SetRound(UINT newVal);

	/**
	*0:ȫ����1:���� 2:�ѹ�ʧ 3:���˿�
	*/
	void SetAvailable(UINT newVal);


public :

	/**
	*�û���Ϣ������
	*/
	UINT GetMemberCount();

	/**
	*�û���������
	*/
	UINT GetNetBarId(INT nIndex);

	/**
	*�û�ID
	*/
	UINT GetMemberId(INT nIndex);

	/**
	*�û�״̬,0����1��ʧ 2ע��
	*/
	UINT GetAvailable(INT nIndex);

	/**
	*
	*/
	UINT GetCardId(INT nIndex);

	/**
	*�û�����0��ʾ��ʱ�û�
	*/
	UINT GetUserClass(INT nIndex);

	/**
	*Ѻ��
	*/
	UINT GetGuarantyMoney(INT nIndex);

	/**
	*�û���������
	*/
	UINT GetAccountNetbarId(INT nIndex);

	/**
	*�û���¼��
	*/
	CString GetMemberName(INT nIndex);

	/**
	*����
	*/
	CString GetUserName(INT nIndex);

	/**
	*0��man 1��woman
	*/
	UINT GetSex(INT nIndex);

	/**
	*֤������
	*/
	CString GetPersonalId(INT nIndex);

	/**
	*֤������ID
	*/
	UINT GetIdType(INT nIndex);

	/**
	*��֤��λ
	*/
	CString GetIdDepart(INT nIndex);

	/**
	*ISO3166
	*/
	UINT GetCountry(INT nIndex);

	/**
	*�����к�
	*/
	CString GetSerailNum(INT nIndex);

	/**
	*�����нӿ����ID
	*/
	CString GetRzxId(INT nIndex);

	/**
	*�������ӿ����ID
	*/
	CString GetGlwId(INT nIndex);

	/**
	*Ħ���ӿ����ID
	*/
	CString GetMgId(INT nIndex);

	/**
	*�����ӿ����ID
	*/
	CString GetOtherId(INT nIndex);

	/**
	*��ֵ�˻��������
	*/
	UINT GetBalance(INT nIndex);

	/**
	*�û���ַ
	*/
	CString GetAddress(INT nIndex);

	/**
	*EMAIL��ַ
	*/
	CString GetEmail(INT nIndex);

	/**
	*��ϵ�绰
	*/
	CString GetTel(INT nIndex);

	/**
	*����
	*/
	CString GetJob(INT nIndex);

	/**
	*�û���ǰ����
	*/
	UINT GetCurrentPort(INT nIndex);

};


}