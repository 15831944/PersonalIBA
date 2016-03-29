#pragma once

#include "Socket\BufferProcess.h"

class CComputerList;
class CActiveMember;

class CComputerInfo
{
public:

	friend class CComputerList;

	CComputerInfo(void);
	CComputerInfo(MANAGER_COMPUT_INFO& computerInfo);
	~CComputerInfo(void);

	enum EUPDATESTATE
	{
		EUS_NORMAL, EUS_ADD, EUS_CHANGE
	};

	enum ECOMPUTERSTATUS
	{
		ECS_OFFLINE,      //����
		ECS_ONLINE,       //����
		ECS_SUSPEND,      //ֹͣ 
		ECS_LOCKED,       //����
		ECS_UNLOCK,       //δ����
		ECS_BIRTHDAY,     //����
		ECS_VIP,		  //VIP
		ECS_BLACK,        //�������û�
		ECS_LITTERMONEY,  //����
		ECS_THIEF,        //�������(û�пͻ������ӣ���������Pingͨ)
		ECS_CONFLICT,     //��ͻ(TermId��ͻ������̨�ն�ʹ����ͬһ��TermId)
		ECS_NORECORD,	  //�޼�¼
	};

private :

	/**************���ʱ��,�ն���Ϣ�ϱ�����ʱ��********************/
	time_t m_lCheckTime; //�ϱ�ʱ�� - Attachʱ��Ϊ0���Ժ����ϱ��߳�������Ϊ�ϱ�ʱ��ʱ��

	/**************�ն���Ϣ********************/
	CString m_strComputerIP;
	CString m_strMac;				// Mac��ַ - Attachʱ��Ϊ���ַ������Ժ����ϱ��߳�������Ϊ���ϱ����ն�IP
	CString m_strTerminalID;
	CString m_strComputerName;
	UINT m_nPCClassID;
	CString m_strClientVersion;
	COleDateTime m_UpdateTime;		//����ʱ��
	EUPDATESTATE m_UpdateState;     //�ն�״̬�Ƿ��иı�  EUS_NORMAL ����״̬, EUS_ADD ����״̬,EUS_CHANGE ����״̬
	/*�ն�����/�ı�/����״̬ - Attachʱ��ΪEUS_ADD���ӣ��Ժ���ÿ��updateComputer����ΪEUS_CHANGE��ʾ�е���ǳ��ȱ仯��
	��UpdateComputerInfoToRow����ΪEUS_NORMAL����ʾ����״̬û�б仯*/
	UINT m_Status;					//Iserver���������ն˻���״̬
	//#define TERM_OFFLINE			0x0001		// ����  
	//#define TERM_ONLINE			0x0002		// ����
	//#define TERM_SUSPEND			0x0003		// �һ�
	//#define TERM_LOCKED			0x0004		// ����
	//#define TEMP_USER				0x0008		// ��ͨ�û�
	//#define MEMBER_USER			0x0010		// ��Ա
	//#define TERM_WARNINGBAL		0x0020		// ����
	//#define TERM_IDCONFLICT		0x0040		/// TermId��ͻ, ����̨�ն�ʹ����ͬһ��TermId
	//#define TERM_THIEF			0x0080		/// �������(û�пͻ�������,��������pingͨ)
	//#define BLACK_MAN				0x0100		// �������û�
	//#define TERM_UNAUTHORIZED		0x0200		/// �޼�¼�ն�(��ʱ�ᱻ����"�Ƿ��ն�"��ʹ�û���ǿ����)
	//#define BIRTHDAY				0x0400		// �˿�����
	//#define VIP					0x0800		// �ص��û�
	//#define TERM_MANUALUNLOCAL    0x0030      // ����
	//#define UNKNOWN_STATUS		0x8000		/// δ֪״̬
	UINT m_IDOStatus;				//��¼IDO��״̬�� 1 �Ʒ� 3 �һ� 5 ���� 7 ά��
	ECOMPUTERSTATUS m_ComputerStatus;//����m_Status��������Ϣ��ϸ�ֳ�Ҫ�ڻ�����ͼ��ʾ�����ն�״̬(��Ӧ��Դ�е��ַ���)
/*		* ECS_OFFLINE              ����     ------------   TERM_ONLINE == m_Status && m_IDOStatus !=3(��Ϊ�һ�) && û��UserInfo && m_strClientVersionΪ�� 
		* ECS_ONLINE               ����
		* ECS_SUSPEND              �һ�
		* ECS_LOCKED               ����      -----------    TERM_ONLINE == m_Status && m_IDOStatus !=3(��Ϊ�һ�) && û��UserInfo && m_strClientVersion��Ϊ�� 
		* ECS_UNLOCK               δ����
		* ECS_BIRTHDAY             �˿�����
		* ECS_VIP                  ��Ա�û�
		* ECS_BLACK                �������û�
		* ECS_LITTERMONTY          ����
		* ECS_THIEF                �������(û�пͻ������ӣ���������Pingͨ)
		* ECS_ CONFLICT            ��ͻ(TermId��ͻ������̨�ն�ʹ����ͬһ��TermId)
		* ECS_NORECORD             �޼�¼*/
	BOOL m_bShutdown;  // �Ƿ�ػ� - Attachʱ��ΪFALSE���Ժ����ϱ��߳�������Ϊ���ϱ����ն˵Ĺػ�״̬
	UINT m_LastNetBarID;
	UINT m_LastAccountType;

	/**************�û���Ϣ********************/
	BOOL m_bHasUserInfo;		//�Ƿ����û���Ϣ - Attachʱ����MemberId��UserName�����Ƿ�Ϊ0�ж��Ƿ�������Ժ���AttachActiveMember�͸��Ļ�Ա�˺ŵ�ʱ����ΪTRUE
	CString m_strMemberType;	//��Ա����
	CString m_strUserName;
	UCHAR m_Sex;
	CString m_strPersonalID;	
	CString m_strAuditID;		//�����ӿ����ID�����ڼ����֤��
	UINT m_nMemberID;
	UINT m_IdType;				//ID������
	UINT m_PayType;
	CString m_strPDK;			//�û����˷����ܳ�
	CString m_strSerialNo;		//�����˺�
	UINT m_nSerialNo;			//�����˺�

	/**************������Ϣ********************/
	INT m_Amount;
	UINT m_CreditAmount;		//��ֵ���
	UINT m_UseAmount;			//ʹ�ý��
	ULONG m_FreezeBalance;		//�������
	LONG m_AvailavleBalance;	//�������
	COleDateTime m_CheckInTime;	//��½ʱ��
	/**************������Ϣ********************/	
	CString m_strLcation;
	UINT m_nNetbarID;		
	UINT m_nRefNo;				//���ķ��ص�ϵͳ�ο��ţ�������Ϊÿ������ά��������룬��0~999999ѭ����������ȷ������Ψһ

public:
	//����������
	void SetUserName(CString val) { m_strUserName = val; }
	void SetTerminalID(CString val) { m_strTerminalID = val; }
	void SetPCClassID(UINT newVal) { m_nPCClassID = newVal; }
	void SetAvailavleBalance(ULONG val) { m_AvailavleBalance = val; }
	void SetMemberID(UINT val) { m_nMemberID = val; }
	void SetNetId(CString val) { m_strSerialNo = val; m_bHasUserInfo = TRUE; }
	void SetMac(CString val) { m_strMac = val; }
	void SetIsShutdown(BOOL val) { m_bShutdown = val; }
	void SetCheckTime(time_t val) { m_lCheckTime = val; }
	////////////////////////////////////////

	//GPropertyReadOnly( ComputerIP, CString ); // ����IP
	//GPropertyReadOnly( TerminalID, CString ); // �ն˺�
	//GPropertyReadOnly( ComputerName, CString ); // ��������
	//GPropertyReadOnly( PCClassID, UINT ); // ��������ID
	//GPropertyReadOnly( MemberID, UINT ); // ��ԱID
	//GPropertyReadOnly( RefNo, UINT ); // ���ú�
	//GPropertyReadOnly( SerialNo, UINT ); //

	CString GetComputerIP() const { return m_strComputerIP; }

	CString GetTerminalID() const { return m_strTerminalID; }
	
	CString GetComputerName() const { return m_strComputerName; }

	UINT GetPCClassID() const { return m_nPCClassID; }
	CString GetPCClass(); // ��ȡ������������

	CString GetMemberType() const;
	UINT GetUserClassId() const;
	BOOL IsMember() const;

	UINT GetMemberID() const { return m_nMemberID; }
	CString GetMemberIDAsString();

	UINT GetRefNo() const { return m_nRefNo; }

	UINT GetSerialNo() const { return m_nSerialNo; }

	UINT GetNetbarID() const { return m_nNetbarID; }

	LONG GetAvailavleBalance() const { return m_AvailavleBalance; }
	CString GetAvailavleBalanceAsString();

	ULONG GetFreezeBalance() const { return m_FreezeBalance; }

	CString GetUserName(BOOL bWithSex = FALSE);

	CString GetSexAsString();
	UCHAR GetSex() const { return m_Sex; }

	CString GetPersonalID() const { return m_strPersonalID; }

	UINT GetIdType() const { return m_IdType; }

	CString GetPayTypeAsString();
	UINT GetPayType() const { return m_PayType; }

	UINT GetAmount() const { return m_Amount; }
	CString GetAmountAsString();

	UINT GetCreditAmount() const { return m_CreditAmount; }

	UINT GetUseAmount() const { return m_UseAmount; }

	CString GetCheckInTimeAsString();
	COleDateTime GetCheckInTime() const { return m_CheckInTime; }

	COleDateTime GetUpdateTime() const { return m_UpdateTime; }

	UINT GetLastAccountType() const { return m_LastAccountType; }

	UINT GetLastNetBarID() const { return m_LastNetBarID; }

	CString GetPDK() const { return m_strPDK; }

	CString GetAuditID() const { return m_strAuditID; }

	CString GetClientVersion() const { return m_strClientVersion; }

	CString GetLcation() const { return m_strLcation; }

	CString GetStatusAsString();

	UINT GetIDOStatus() const { return m_IDOStatus; }

	CString GetNetId() const;

	CString GetUseTimeAsString();

	CString GetMac() const { return m_strMac; }
	BOOL GetIsShutdown() const { return m_bShutdown; }
	time_t GetCheckTime() const { return m_lCheckTime; }

	UINT GetStatus() const {return m_Status;}

public :

	void Attach(MANAGER_COMPUT_INFO & computerInfo);

	// 2011/06/09-gxx: 
	void AttachActiveMember(CActiveMember& ActiveMember);

private :

	void InitState();

	void CheckStatus();
	
public :

	BOOL HasUser() const;
	BOOL HasUserInfo() const { return m_bHasUserInfo; }
	BOOL HasClient() const;

	ECOMPUTERSTATUS GetComputerStatus() const;
	void SetComputerStatus(ECOMPUTERSTATUS newVal);

	EUPDATESTATE GetUpdateState() const { return m_UpdateState; }
	void SetUpdateState(EUPDATESTATE newVal) { m_UpdateState = newVal; }

	void ClearUserInfo();
	//void Clear();
};
