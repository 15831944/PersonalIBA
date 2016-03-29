#pragma once

// CRightsManagement 
//      Ȩ�޹���
// Remark:
//      ȫ��Ψһ����, ����̨��¼ʱ����Ȩ�޹���
//
// CRightsManagement::HasRight :�ж�����Ա�Ƿ����ĳȨ��
// CRightsManagement::TakeRight :ȡ��Ȩ��,�������Ա����ĳȨ�ޣ�TakeRight����TRUE
//      �������Աû��Ȩ�ޣ���������Ա�������������������ȷ��TakeRight����TRUE�������������FALSE

namespace NS_DAL{

#define TAKING_RIGHTS_DEFINE(rightName) \
	BOOL CRightsManagement::Has_##rightName()\
	{\
	CString strName = _T(#rightName);\
	return CRightsManagement::HasRight(strName);}\
	BOOL Take_##rightName(BOOL bShouldOnDuty=TRUE)\
	{\
	CString strName = _T(#rightName);\
	return CRightsManagement::TakeRight(strName,bShouldOnDuty);	}\
	void SetUI_##rightName(CCmdUI* pCmdUI, BOOL bShouldOnDuty=TRUE)\
	{\
	CString strName = _T(#rightName);\
	CRightsManagement::SetCmdUI(pCmdUI, strName, bShouldOnDuty);}

class CRightsManagement
{
protected:
	CRightsManagement()
	{
		m_bIsBoss = FALSE;
	}

public:
	static CRightsManagement* GetInstance()
	{
		static CRightsManagement rm;
		return &rm;
	}

	void CreateManagement(LPCTSTR lpRightsList, BOOL bBoss);

	// �Ƿ����ĳȨ��
	BOOL HasRight(LPCTSTR lpRightName);

	// ȡ��Ȩ�ޣ�û��Ȩ��ʱ��������Ա���������
	BOOL TakeRight(LPCTSTR lpRightName, BOOL bShouldOnDuty=TRUE);

	void SetCmdUI(CCmdUI *pCmdUI, LPCTSTR lpszName, BOOL bShouldOnDuty=TRUE);
	
public:
	//----------------------------------------//
	//                  Ȩ�޶���                 //
	//----------------------------------------//

	// ����̨����
	TAKING_RIGHTS_DEFINE(IBAP_CONFIG);

	// ��ʱ����
	TAKING_RIGHTS_DEFINE(IBAP_BUNDLETIME2);

	// �ϲ��˻�
	TAKING_RIGHTS_DEFINE(IBAP_MERGEMEMBER);

	// ���ֶһ���Ʒ
	TAKING_RIGHTS_DEFINE(IBAP_USEPOINT);

	// ��Ʒ����
	TAKING_RIGHTS_DEFINE(IBAP_RETAIL);

	// ����Ա������ѯ
	TAKING_RIGHTS_DEFINE(IBAP_OPERATIONQUERY);

	// ����Ա�����ѯ
	TAKING_RIGHTS_DEFINE(IBAP_CASHSHIFTQUERY);

	// �ն����Ѽ�¼
	TAKING_RIGHTS_DEFINE(IBAP_TERMCONSUME);

	// �鿴���Ѽ�¼
	TAKING_RIGHTS_DEFINE(IBAP_CONSUMELIST);

	// ��ֵ
	TAKING_RIGHTS_DEFINE(IBAP_CREDIT);

	// ��ֵ����ֵ
	TAKING_RIGHTS_DEFINE(IBAP_CARDCREDIT);

	// �û�����
	TAKING_RIGHTS_DEFINE(IBAP_UPGRADE);

	// �޸��˺�
	TAKING_RIGHTS_DEFINE(IBAP_CHANGESN);

	// ��ʱ
	TAKING_RIGHTS_DEFINE(IBAP_BUNDLETIME);

	// �޸��û�����
	TAKING_RIGHTS_DEFINE(IBAP_MODIFYUSER);

	// �˻�ֱ��
	TAKING_RIGHTS_DEFINE(IBAP_CREDITDIRECT);

	// �޸�����
	TAKING_RIGHTS_DEFINE(IBAP_MODIFYPWD);

	// ���ɲ�������
	TAKING_RIGHTS_DEFINE(IBAP_NETBARSET);

	// ���°�Ȩ��
	TAKING_RIGHTS_DEFINE(IBAP_ONOFFDUTY);

	// ����Ȩ��
	TAKING_RIGHTS_DEFINE(IBAP_REGISTER);

	// ��Ʒ�˻�Ȩ��
	TAKING_RIGHTS_DEFINE(IBAP_RETAILRETURN);

	// ������ƷȨ��
	TAKING_RIGHTS_DEFINE(IBAP_RETAILPRESENT);

	// ���ֶһ���Ʒ
	TAKING_RIGHTS_DEFINE(IBAP_RETAILPOINT);

	// ��ֵ����Ȩ��
	TAKING_RIGHTS_DEFINE(IBAP_CREDITREVERSE);

private:
	CString m_strRightsList;
	BOOL m_bIsBoss;
};

}