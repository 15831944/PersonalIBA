#pragma once

#include "Singleton.h"
#include "IniConfig.h"
#include "NetbarBundleTime.h"
#include "dal\NetBarInfo.h"
#include "Servlet\UserClassInfo.h"
#include "dal\IDTypeInfo.h"

using namespace NS_DAL;

class CNetBarConfig : public CIniConfig
{
	SINGLETON_DECLARE(CNetBarConfig)

public :
	static CNetBarConfig* GetInstance();

	UINT GetNetBarId() const;
	CString GetNetBarName() const;
	CString GetNetBarKey() const { return m_NetBarKey; }

	CString GetLocalServerIP() const;
	void SetLocalServerIP(CString newVal);

	CString GetMainCenterIP() const;
	UINT GetMainCenterPort() const { return m_MainCenterPort; }
	CString GetMainCenterPath() const { return m_MainCenterPath; }

	CString GetBackupCenterIP() const { return m_BackupCenterIP; }
	UINT GetBackupCenterPort() const { return m_BackupCenterPort; }
	CString GetBackupCenterPath() const { return m_BackupCenterPath; }

	CString GetStandbyHost() const { return m_strStandbyHost; }
	void SetStandbyHost(CString newVal);

	CString GetBossPwd() const { return m_strBossPwd; }
	void SetBossPwd(CString newVal);

	CString GetUnLockPassword() const { return m_strUnLockPassword; }
	void SetUnLockPassword(CString newVal);

	CString GetProxyIP() const { return m_strProxyIP; }
	void SetProxyIP(CString newVal);



public:
	
	BOOL LoadFromDB();
	void LoadFromIni();
	void SaveToIni();
	BOOL IsLoadFromDB() const { return m_bLoadFromDB; }

public :

	CNetbarBundleTime		m_NetbarBundleTime;	//��ʱ�б���Ϣ
	CIDNameRecordArray		m_PCClassInfoArray; //��������
	CUserClassInfoArray		m_UserClassArray;	//�û������б�
	CIDTypeInfoArray		m_IDTypeInfoArray;	//֤�������б�
	CNetBarInfoArray		m_NetBarInfoArray;	//������Ϣ

	CString GetNetBarName(UINT nNetbarId) const;
	CString GetPCClassName(UINT nPCClass);
	CString GetUserClassName(UINT nClassID);
	CString GetIDTypeName(UINT nIdType);

	BOOL IsUseDeposit();
	UINT GetDeposit(UINT nClassID);
	BOOL GetIsMember(UINT nClassID);//�ж��Ƿ��ǻ�Ա
	UINT GetUserClassId(CString strClassName);//�����ֵ�ID

private :

	BOOL m_bLoadFromDB; //�Ƿ�ʹ�����ݿ�

	UINT m_NetBarId;
	UINT m_DomainId;
	CString m_NetBarKey;
	CString m_strNetBarName;

	CString m_LocalServerIP;
	CString m_strProxyIP;
	CString m_strBossPwd;

	CString m_MainCenterIP;
	CString m_MainCenterPath;
	UINT m_MainCenterPort;

	CString m_BackupCenterIP;
	CString m_BackupCenterPath;
	UINT m_BackupCenterPort;
	
	CString m_strStandbyHost;

private:

	BOOL m_bReturnOnlyClassId;

	CString m_strDodoNewHost;

	CString m_strDodonewResource;

	UINT m_nUpdateInterval;

	UINT m_ForeRealName;

	CString m_strQueryHost;
	CString m_strQueryHost2;

	UINT m_nAuditType;

	UINT m_nSoftwareType;//����Ʒ�����

	BOOL m_bNoIDCardIn;

	UINT m_bIssueNetBarId;
	
	BOOL m_bAllowNoCardLogin;

	UINT m_nRealNameFrom;

	CString m_strUnLockPassword;

	UINT m_nICControlVerID;
	
	UINT m_nRoundType;
	
	BOOL m_bBundleTimeWithCash; //�ֽ��ʱ

	BOOL m_bMustSynchronize;

	BOOL m_bCreditDirect; //!<�Ƿ�����ֱ��

public :

	// 2011-4-15-gxx: �Ƿ�����ɨ��
	GPropertyReadOnly(ScanEnabled, INT);

	// 2011/05/12-gxx: �Ƿ�ǿ���Զ��˿�ʱ��Ϊ0,  1:��ʾǿ��
	GPropertyReadOnly(MustAutoReturn, INT);

	// 2011/06/10-gxx: �Ƿ�ǿ�ƹر�"���ɿ�������"�ȳ���,
	//                 KillCaptions����Щ����ı������ϣ���"|"�ָ�
	GPropertyReadOnly(KillCaptions, CString );  
	
	// 2011/06/20-gxx: ��ʱ������ʱ��,��"���ʱ����XXX����֮�ڲ������ʱ"
	GPropertyNeedSet( BTLimitTime, UINT); 
	void SetBTLimitTime(const UINT nNewVal);

	// 2011/06/30-gxx: �����Ĳ������Ƿ������޸�����
	GPropertyReadOnly(ForbidModifyPwd, INT);

	// 2011/08/10-8201-gxx: Ӧ��ģʽ�����÷�ʽ
	GPropertyNeedSet( LocalModeStartKind, INT );
	void SetLocalModeStartKind(INT nKind);

	// 2011/09/07-8201-gxx: �յ��ͻ���������Ϣ���Ƿ���������ʾ
	GPropertyNeedSet( ReminderBalance, BOOL );
	void SetReminderBalance(BOOL newVal);

	GPropertyNeedSet( ReminderAutoReturn, BOOL);
	void SetReminderAutoReturn(BOOL newVal);

	// 2011/12/12-8235-gxx: �Ƿ�����û���ͼ�Ҽ��ϻ�����
	GPropertyNeedSet( UserRCheckIn, BOOL);
	void SetUserRCheckIn(BOOL newVal);

	GPropertyNeedSet( ForbidCaptureScreen, BOOL);
	void SetForbidCaptureScreen(BOOL newVal);


	//����Ʒ�����
	UINT GetSoftwareType() const;
	void SetSoftwareType(UINT newVal);

	BOOL GetMustSynchronize() const { return m_bMustSynchronize; }
	void SetMustSynchronize(BOOL newVal) { m_bMustSynchronize = newVal; }

	UINT GetRoundType() const { return m_nRoundType; }

	UINT GetICControlVerID() const { return m_nICControlVerID; }

	UINT GetRealNameFrom() const { return m_nRealNameFrom; }
	void SetRealNameFrom(UINT newVal);

	BOOL GetAllowNoCardLogin() const { return m_bAllowNoCardLogin; }
	void SetAllowNoCardLogin(BOOL newVal);

	UINT GetIssueNetBarId() const { return m_bIssueNetBarId; }
	void SetIssueNetBarId(UINT newVal);

	BOOL GetNoIDCardIn() const { return m_bNoIDCardIn; }
	void SetNoIDCardIn(BOOL newVal);

	UINT GetAuditType() const { return m_nAuditType; }
	void SetAuditType(UINT newVal);

	CString GetQueryHost2() const { return m_strQueryHost2; }
	void SetQueryHost2(CString newVal);

	CString GetQueryHost() const { return m_strQueryHost; }
	void SetQueryHost(CString newVal);

	UINT GetForeRealName() const { return m_ForeRealName; }
	void SetForeRealName(UINT newVal);

	UINT GetUpdateInterval() const { return m_nUpdateInterval; }
	void SetUpdateInterval(UINT newVal);

	CString GetDodonewResource() const { return m_strDodonewResource; }
	void SetDodonewResource(CString newVal);

	CString GetDodoNewHost() const { return m_strDodoNewHost; }
	void SetDodoNewHost(CString newVal);

	BOOL GetReturnOnlyClassId() const { return m_bReturnOnlyClassId; }
	void SetReturnOnlyClassId(BOOL newVal);

	BOOL GetBundleTimeWithCash() const { return m_bBundleTimeWithCash; }
	void SetBundleTimeWithCash(BOOL newVal) { m_bBundleTimeWithCash = newVal; }

	BOOL GetCreditDirect() const { return m_bCreditDirect; }
	void SetCreditDirect(BOOL newVal) { m_bCreditDirect = newVal; }



public:

	void SetDomainId(UINT nDomainId);
	UINT GetDomainId(){ return m_DomainId; }
	void SetNetBarInfo(UINT nNetBarId, LPCTSTR lpNetBarKey);
	void SetMainHost(CString strMainHost, CString strPath = _T(""));
	void SetBackUpHost(CString strBackUpHost, CString strPath = _T(""));

private :
	//���Ŀ��Ʋ�����ȡ
	CStringArray m_strControlOptionArray;//���Ʋ�������

	INT m_nShenzhen;

	INT m_nMustScan;//ǿ��ɨ��
	
	INT m_nTempMember;//��ʱ�û�
	
	INT m_nNeedActivation;//���뼤��
	
	INT m_nUploadImage;//�ϴ���Ƭ

	INT m_nNoModifyMember;//��֤�û���Ϣ

	INT m_nRealNamePassword;//�Ƿ���ù�����ǿ������

	INT m_nAutoMatchIDCard; //�Զ�ƥ�����֤����

	// 1Ϊ��ֹ������0Ϊ��������
	INT m_nDisallowUnlock;

	INT m_nInputActivation; //���뼤��

public :

	BOOL GetValue(CString strSrc, LPCTSTR strKey, INT & nValue);
	BOOL GetValue(CString strSrc, LPCTSTR strKey, CString & strValue);

	BOOL PraseControlOption(CString strInfo);

	INT GetShenzhen() const {return m_nShenzhen;}
	INT GetMustScan() const;
	INT GetTempMember() const { return m_nTempMember; }
	INT GetNeedActivation() const { return m_nNeedActivation; }
	INT GetUploadImage() const { return m_nUploadImage; }
	INT GetNoModifyMember() const { return m_nNoModifyMember; }
	INT GetAutoMatchIDCard() const { return m_nAutoMatchIDCard; }
	INT GetInputActivation() const { return m_nInputActivation; }

	INT GetRealNamePassword() const  { return m_nRealNamePassword; }
	void SetRealNamePassword(INT val);

	INT GetDisallowUnlock() const { return m_nDisallowUnlock; }
	void SetDisallowUnlock(INT val);

private:

	CString m_strAgentMobilePhone;//�������ƶ��绰

	CString m_strAgentName;//����������

	CString m_strAgentregularTel;//�����̶̹��绰

public:
	//��������Ϣ
	CString GetAgentName() const { return m_strAgentName; } //����������
	void SetAgentName(CString newVal) { m_strAgentName = newVal; }

	CString GetAgentMobilePhone() const { return m_strAgentMobilePhone; } //�������ƶ��绰
	void SetAgentMobilePhone(CString newVal) { m_strAgentMobilePhone = newVal; }

	CString GetAgentregularTel() const { return m_strAgentregularTel; } //�����̶̹��绰
	void SetAgentregularTel(CString newVal) { m_strAgentregularTel = newVal; }

private:

	UINT m_nStoreInAmount; //�����

public:

	UINT GetStoreInAmount();
	void SetStoreInAmount(UINT val); 
	
};
