#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CBalanceMethod : public CServletMethod
{
public :
	CBalanceMethod(void);
	~CBalanceMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	virtual CString GetMethodName();

	UINT m_nMemberId;
	
public :
	
	UINT GetMemberId() const { return m_nMemberId; }

	/**
	*�û���
	*/
	void SetMemberId(UINT newVal);

	/**
	*���ɱ��
	*/
	void SetNetBarId(UINT newVal);

	/**
	*�ڲ�����
	*/
	void SetCardId(UINT newVal);

	/**
	*�Ƿ��������(0:������ 1:����)
	*/
	void SetRound(UINT newVal);

	//2011/3/11//gxx-�Ƿ�ʵ������
	void SetOnlineInFact(UINT newVal);


	UINT GetCardId();

public :

	/**
	*1������ 2��������
	*/
	UINT GetOnline();

	/**
	*��ֵ�˻��������
	*/
	INT GetBalance();

	/**
	*�û���������
	*/
	UINT GetOnlineNetBarId();

	/**
	*�û������ն���ˮ
	*/
	UINT GetOnlineSerialNo();

	/**
	*�û�Ԥ�۽��
	*/
	UINT GetOnlineAmount();

	/**
	*�������͵����������̶�Ϊ1
	*/
	UINT GetNetBarCount();

	/**
	*�������͵�����ID(��ǰ����ID)
	*/
	UINT GetNetBarId();

	/**
	*���á������˻������
	*/
	INT GetCreditPresent();

	/**
	*���á����ͳ�ֵ�����
	*/
	INT GetUsePresent();

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
	*�����к�
	*/
	CString GetSerailNum();

	/**
	*�û���ǰ����
	*/
	UINT GetCurrentPoint();

	/**
	*���ͳ�ֵ���
	*/
	INT GetCashBalance();

	/**
	*Ӧ�����
	*/
	INT GetReturnAmount();

	/**
	*=1 �ص��û�
	*/
	UINT GetImportantMember();

	/**
	*���ߵ��ն˺�
	*/
	CString GetTermId();

	/**
	*ʵ��ID
	*/
	CString GetAuditId();

	/**
	*�ܻ���
	*/
	UINT GetTotalPoint();

	/**
	*�Ƿ���ţ��Ա 1: �� 0: ��
	*/
	UINT GetIsMember();

	/**
	*�Ƿ��ֹ�ϻ� 1: �� 0:��
	*/
	UINT GetDisallowLogin();

	/**
	*yyyymmddhhmmss
	*/
	CString GetLogoutTime();

	/**
	*�û�PSK
	*/
	CString GetEpsk();

	/**
	*��������
	*/
	UINT GetPayType();

	/**
	*��ʱ��ʼʱ��
	*/
	CString GetStartTime();

	/**
	*��ʱ����ʱ��
	*/
	CString GetEndTime();

	/**
	*��ʱ���
	*/
	UINT GetPrice();

	/**
	*��������
	*/
	UINT GetOpenNetbarId();

	/**
	*0:������ 1:���� 2:ֻ��Ѻ��
	*/
	UINT GetAllowReturn();

	/**
	*0:���������»� 1:�����������»�
	*/
	UINT GetDisallowLogout();

	/**
	*�ϴ�����ʱ��(��)������ֵΪ0
	*/
	UINT GetLastConsumeTime();

	/**
	*�ϴ����ѽ��(��)������ֵΪ0
	*/
	INT GetLastConsumeAmount();

	/**
	*�ʻ�����
	*/
	CString GetPassword();
	
	/**
	*1:��Ҫ���� 0:���輤��
	*/
	UINT GetNeedActivation();

	/**
	*�ܽ��
	*/
	INT GetTotalBalacne();


	/**
	*������ʱ��
	*/
	CString GetServerTime();

	/**
	*�Ƿ�Ϊ���ɻ�Ա
	*/
	BOOL IsNetbarMember();
};

}