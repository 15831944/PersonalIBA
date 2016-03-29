#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CCheckInMethod : public CServletMethod
{
public :
	CCheckInMethod(void);
	~CCheckInMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	
	virtual BOOL	OnBegin();							//!< ִ������ǰ

	CString m_strPwd;

public :
	
	void SetUserPwd(CString newVal);

	/**
	*����ID������16λ������ʾ
	*/
	void SetNetbarId(UINT newVal);

	/**
	*��ʱID����һ������ʱ��д��������0
	*/
	void SetTimeId(UINT newVal);

	/**
	*������ʶ�������Ϊ8���ַ�
	*/
	void SetTermId(CString newVal);

	/**
	*���ܷ�ʽ 1: DES 2:TEA
	*/
	void SetEncryptMode(UINT newVal);

	/**
	*ID����
	*/
	void SetCardSerialNum(CString newVal);

	/**
	*��������ʶ
	*/
	void SetReaderSerialNum(CString newVal);

	/**
	*�ͻ��˻���MAC��ַ
	*/
	void SetMacAddress(CString newVal);

	/**
	*�ͻ��˻���IP��ַ
	*/
	void SetIpAddress(CString newVal);

	/**
	*�ͻ��˵�ǰ�汾
	*/
	void SetClientVersion(CString newVal);

	/**
	*1: �Զ����� 0:�û������˺ŵ���
	*/
	void SetAutoLogin(UINT newVal);


public :

	/**
	*ϵͳ�ο���, ������Ϊÿ������ά��refNo��refNo��0-999999ѭ����������ȷ������Ψһ��
	*/
	UINT GetRefNo();

	/**
	*������֣�
	*/
	UINT GetFreezeMoney();

	/**
	*��λ�۷�ʱ�䣨���ӣ�����ͨ�ʷѷ�ʽ��Ч����ο۷ѵİ�ʱ�η�ʽҲ�ɲ��ã���һ���Կ۷ѷ�ʽ�����ʱ������Ч
	*/
	UINT GetUnitTime();

	/**
	*�´ε���ʱ��
	*/
	UINT GetNextCheckinTime();

	/**
	*�ۼƷⶥ��̬�ⶥ��
	*/
	UINT GetDynamicMaxAmount();

	/**
	*��psk����pdk�������
	*/
	CString GetEpdk();

	/**
	*�������֣�
	*/
	UINT GetAvailableBalance();

	/**
	*0-23Сʱ�Ŀ��ʣ���/ÿСʱ����
	*/
	UINT GetPolicyInfo();

	/**
	*����
	*/
	CString GetUsername();

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
	*0:��ͨ 1:��ʱ�� 2:��ʱ��
	*/
	UINT GetPayType();

	/**
	*0:��ʵ�� 1:������ 2:������ 3:Ħ�� 4:ɣ�� 5:���� 6:������2008
	*/
	UINT GetAuditType();

	/**
	*��������ǰʱ�䣬����ʱ��ͬ��
	*/
	CString GetServerTime();

	/**
	*�û���ǰ����
	*/
	UINT GetCurrentPoint();

	/**
	*�û�����
	*/
	UINT GetClassId();

	/**
	*�Ƿ��Ա
	*/
	UINT GetIsMember();

	/**
	*��ID
	*/
	UINT GetDomainId();

	/**
	*֧����IP�˿�
	*/
	CString GetMainHost();

	/**
	*֧�ű�IP�˿�
	*/
	CString GetBackupHost();

	/**
	*��ֵIP�˿�
	*/
	CString GetDodonewHost();

	/**
	*ID������
	*/
	UINT GetCardId();

	/**
	*ID���û���Կ
	*/
	CString GetEpsk();

	/**
	*��ʱ����
	*/
	UINT GetBundleType();

	/**
	*��ʱ��ʼʱ��
	*/
	UINT GetBeginBundleTime();

	/**
	*��ʱ����ʱ��
	*/
	UINT GetEndBundleTime();

	/**
	*�û���
	*/
	UINT GetMemberId();

	/**
	*�������ɺ�
	*/
	UINT GetNetBarId();

	/**
	*�����˺�
	*/
	CString GetSerialNum();

	/**
	*���ͳ�ֵ���
	*/
	UINT GetBalance();

	/**
	*1:ǿ��ʹ��̽�� 0:��ǿ��
	*/
	UINT GetForceAudit();

	/**
	*����
	*/
	CString GetNation();

	/**
	*�绰����
	*/
	CString GetTel();

	/**
	*��macKey���ܵ�readerPSK, ������ţ����
	*/
	CString GetEReaderPsk();

	/**
	*��macKey���ܵ�dodonewKey�����ͻ��˵���dodonewҳ����
	*/
	CString GetEDodonewKey();

	/**
	*��ǰ�ն����ڻ�������
	*/
	UINT GetPcClass();

	/**
	*0:���������»� 1�������������»�
	*/
	UINT GetDisallowLogout();

	/**
	*�Ʒ�ģʽ:1,2,3,4,5
	*/
	UINT GetPayMode();

	/**
	*�ϴζ�۽��
	*/
	UINT GetLastRemainAmount();

	/**
	*��λ�۷ѽ��
	*/
	UINT GetUnitAmount();

	/**
	*�������
	*/
	UINT GetMinConsume();

	/**
	*��������ʹ��ʱ��
	*/
	UINT GetBeginFreeTime();

	/**
	*�������ʱ��
	*/
	UINT GetNextPayFreeTime();

	/**
	*�״ε���ʱ��
	*/
	CString GetFirstCheckinTime();

	/**
	*���ε���ǰ�ۼ����ѽ��
	*/
	UINT GetConsumeAmount();

	/**
	*���ε���ʱ��(���������Ʒ�ģʽ����ʱ���ֵһ�㲻��)
	*/
	CString GetCurrCheckinTime();


};

}