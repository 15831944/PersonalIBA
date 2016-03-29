#pragma once

#include "ActiveMember.h"
#include "Servlet\BalanceMethod.h"
#include "Dal\IDTypeInfo.h"

using namespace NS_SERVLET;

class CActiveMemberList : public CThread
{
public:
	CActiveMemberList(void);
	~CActiveMemberList(void);

	void RefreshView();

	void SetWndGrid(HWND newVal) { m_hWndGrid = newVal; }

	typedef CMap<UINT, UINT, CActiveMember, CActiveMember&> ActiveMemberMap;

public:
	// ֪ͨ�ͻ��˳��԰�ʱ���µ�¼�Ľṹ����
	struct TReLoginRequest
	{
		static const UINT TRY_MAX = 6;   // ����Դ���
		UINT memberId;		// ��ԱID
		UINT lastRefNo;		// �����ж��Ƿ��Ե�¼�ɹ������ñ��
		UINT tryCount;		// ���ԵĴ��� 
		UINT firstTryTime;	// ��һ�γ���ʱ��
	};

	// 2011-03-30-gxx: ���һ���ͻ��˰�ʱ���µ�¼�ĳ���
	void AddReLoginRequest(TReLoginRequest& btt);
	
	void TryReLogin();

private:

	CArray<TReLoginRequest> m_arrReLoginRequest;

private:

	HWND m_hWndGrid;

	//!<�洢ȫ�������û�����
	ActiveMemberMap m_ActiveMemberMap;
	
	//!<�洢���µ��û����������ڿ���ˢ�½�����ʾ
	CArray<UINT, UINT> m_UpdateMemberIdArray;

	POSITION m_CurPos;

public :

	::CCriticalSection ActiveMemberListLock;

	BOOL LoadFromDB();

	void Copy(ActiveMemberMap& OtherActiveMemberMap);
	void Copy(CArray<CActiveMember>& arrMembers);

	void Clear();

	INT_PTR GetCount();

	// 2011/07/14-8201-gxx: 
	void AddActiveMember(NS_DAL::CLocalRegisterInfo& RegisterInfo);

	// 2011/05/19-gxx: �������û�	,�޸ĺ���������,���2����
	void AddActiveMember(CBalanceMethod& Balance, 
		BOOL bRegisterBT=FALSE, // �Ƿ񿪻���ʱ
		UINT nPayout=0,
		int nBundleTimeType = CActiveMember::EPayType_Capped,
		int nScanType=0); // ������ʱ����
	
	//�����û�������Ϣ
	void UpdateActiveMember(CActiveMember& ActiveMember, BOOL bUpdateDb = FALSE);
	
	//�û���ֵ������ȸ���
	void UpdateActiveMember(UINT nMemberId, UINT nCreditMoney, UINT nCreditPresent, UINT nPayout = 0);

	//ɾ���û�
	BOOL RemoveActiveMember(UINT nMemberId);
	
	//�йص�ǰ�Ѿ����µ��û�
	BOOL GetUpdateActiveMemberId();

	//
	BOOL GetActiveMemberNetID(CString strNetId,CActiveMember& ActiveMember, BOOL bFullMatch=TRUE);

	//ͨ����ԱID��ȡ��Ա��Ϣ
	BOOL GetActiveMember(UINT nMemberId, CActiveMember& ActiveMember);

	//ͨ���ն�ID��ȡ��Ա��Ϣ
	BOOL GetActiveMember(CString strTermId, CActiveMember& ActiveMember, BOOL bFullMatch=TRUE);

	// 2011/07/04-gxx: 	һ���ն�ID���ܶ�Ӧ����û��������ϱ�ʱ��������Ǹ�ActiveMember
	BOOL GetActiveMemberBest(CString strTermId, CActiveMember& ActiveMember, BOOL bFullMatch=TRUE);

	//ö�٣�����֮ǰ����������
	void StartEnumActiveMember();
	BOOL GetEachActiveMember(CActiveMember& ActiveMember);

	//2011-03-21-gxx-��ӣ���ȡ����ܺ�(ֻ������ʱ�û�)
	INT  GetDynamicReserve(UINT nCasherID);

	//������еĻ�Ա�����ݿ��еĻ�Ա
	BOOL ClearAllMembers();

	// 2011/06/13-gxx: (˽�б�ɹ���)
	void CheckActiveMember(UINT nCardId, UINT nMemberId, CActiveMember &ActiveMember);

	// 2011/06/29-gxx: �Ƿ������ֵ
	BOOL IsAllowCredit(UINT nMemberId);

	// 2011/07/08-8201-gxx: ĳ�˺Ŷ�Ӧ���û��Ƿ�Ϊ�����û�, emUserStateNULL,emUserStateCenter,           // �����û�
	//                      emUserStateLocal 
	int GetUserStateByNetId(CString strNetId);

	// 2011/07/12-8201-gxx: 
	BOOL HasLocalUser();

	// 2011/07/15-8201-gxx: ��鱾���û�
	void CheckLocalUser(CActiveMember& ActiveMember);

	// 2011/07/27-8201-gxx: ��ȡ�����û���ֵ�ܶ�, nCasherID����̨ID
	INT GetLocalTotalMoney(UINT nCasherID);

	// 2011/08/10-8201-gxx: 
	void CheckBrokenCenterUser(CActiveMember& ActiveMember);

	// 2011/10/26-8230-gxx: ��������Ϣ�����������������˺�
	BOOL FillRoomInfo(NS_DAL::CRoomInfo& info);

	// 2011/10/26-8230-gxx: �ж�ĳ�������Ƿ�Ϊ������������ǣ���ô����
	BOOL IsRoomArea(UINT pcClass, UINT& nRoomMemberID);

	// 2011/10/26-8230-gxx: 
	BOOL GetRoomSMembers(UINT nRoomMemberID,CArray<UINT>& arrMembers);

	// 2011/11/02-8210-gxx: 
	void CheckRoomUser(CActiveMember& ActiveMember);

	// 2011/11/02-8210-gxx: 
	void ClearRoomMembers(UINT nRoomMemberID);

public:
	BOOL UpdateActiveMemberDB(CActiveMember &ActiveMember);
	
	BOOL RemoveActiveMemberDB(UINT nMemberId);

private :

	virtual INT Run();

	void CheckAllActiveMember();
	
private:

	BOOL AddActiveMemberDB(CActiveMember &ActiveMember);
};


