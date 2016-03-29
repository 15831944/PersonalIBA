#pragma once
#include "servletmethod.h"
#include "UserClassInfo.h"

namespace NS_SERVLET
{

class CCashRegisterLoginMethod : public CServletMethod
{
public :
	CCashRegisterLoginMethod(void);
	~CCashRegisterLoginMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	virtual BOOL DecodeResp(CString strResp);		//!< ��������ֵ������д

	CUserClassInfoArray m_UserClassInfoArray;

public :
	
	/**
	*��ǰ����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*����̨���� 0: �����汾 1:�ޱ������ݿ�汾
	*/
	void SetCashRegisterType(UINT newVal);

	/**
	*�û�����
	*/
	void SetUserId(CString newVal);

	/**
	*UNDES����
	*/
	void SetPassword(CString newVal);

	/**
	*����̨�汾��
	*/
	void SetCashRegisterVersion(CString newVal);

	void SetTimeChuo(UINT nTime);

	void SetCheckCode(CString strCheckCode);

	// ����������ȡǰ��15λ
	void SetPCName(CString strCheckCode);
public :

	/**
	*������ID
	*/
	UINT GetDomainId();

	/**
	*������IP�˿�
	*/
	CString GetMainHost();

	/**
	*���ı���IP�˿�
	*/
	CString GetBackupHost();

	/**
	*�û��������
	*/
	UINT GetClassCount();

	CString GetClassInfo();

	/**
	*���˿��û�ID
	*/
	UINT GetReturnOnlyClassId();

	/**
	*DDN���ĵ�ַ
	*/
	CString GetDodoNewHost();

	/**
	*DDN��Դ��������ַ
	*/
	CString GetDodonewResource();

	/**
	*��Դ���¼��
	*/
	UINT GetUpdateInterval();

	/**
	*����ʵ��Ҫ��,0:��Ҫ��ʵ��,1:������ſ�,2:�û�����Ψһ,3:֤������Ψһ,4:���֤��Ψһ,5:��Ա���֤��Ψһ
	*/
	UINT GetForeRealName();

	/**
	*��ǰ���ID��Ϊ0��ʾδ�ϰ࣬���ι���(����1����).
	*/
	UINT GetDutyId();

	/**
	*�ϰ�ʱ�䣬dutyId=0������
	*/
	CString GetOndutyTime();

	/**
	*ϵͳ��ѯ����ַ�˿�
	*/
	CString GetQueryHost();

	/**
	*����ϵͳ��ѯ����ַ�˿�
	*/
	CString GetQueryHost2();

	/**
	*0:��ʵ�� 1:������ 2:������ 3:Ħ�� 4:ɣ�� 5:���� 6.������2008
	*/
	UINT GetAuditType();

	/**
	*��������ַ
	*/
	CString GetPopupURL();

	/**
	*���ID��ϵͳ��Ч,1:ʹ��ˢ���ϻ�ģʽ,0:ʹ�ò忨�ϻ�ģʽ
	*/
	UINT GetNoIDCardIn();

	/**
	*��������ID
	*/
	UINT GetIssueNetBarId();

	/**
	*����ϵͳʱ��
	*/
	CString GetServerTime();

	/**
	*1: �����޿��ϻ� 0:������
	*/
	UINT GetAllowNoCardLogin();

	/**
	*0: �����ƣ�1:ǿ��ɨ�� 2:ʵ�����
	*/
	UINT GetRealNameFrom();

	/**
	*����̨ѡ�����,��������÷ֺŷָ�
	*/
	CString GetControlOption();

	/**
	*1:����IC�����ð�ʵ��
	*/
	UINT GetCardRealName();

	/**
	*����ԱȨ�ޣ���ʽ:[Ȩ������,Ȩ�����ơ�]
	*/
	CString GetUserPrivileges();

	/**
	*������� 1:��� 2:�а� 3:��࣬û���ϰ��򷵻�0
	*/
	UINT GetShiftType();

	/**
	*�����ֵ�ֻ�
	*/
	CString GetAgentMobilePhone();

	/**
	*����̶��绰
	*/
	CString GetAgentRegularTel();

	/**
	*�����������
	*/
	CString GetAgentName();

	/**
	*
	������� 
	0��IC��
	1: ID��
	2: ţ��V1
	3�����
	4: ţ��V2
	*/
	UINT GetSoftwareType();

	void GetUserClassInfoArray(CUserClassInfoArray & UserClassInfoArray);

	/**
	*ȡ��ͨ������
	*/
	CString GetNoticeInfo();


	/**
	*��ر��ݷ�������ַ
	*/
	CString GetStandbyHost();


	/**
	*�Ƿ������˻����������Ԥ�۰�ʱ, 1������ 0��������
	*/
	UINT GetBundleTimeUseBalance();


	/**
	*�Զ�ͬ��
	*/
	UINT GetMustSynchronize();


	/**
	*�����ֵΪ����
	*/
	UINT GetAllowCreditNegative();
};

}