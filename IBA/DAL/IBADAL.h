#pragma once

#include "..\Singleton.h"

#include "ConfigInfo.h"
#include "NetBarInfo.h"
#include "IDTypeInfo.h"
#include "DALJXC.h"
#include "DALUSER.h"
#include "..\Servlet\UserClassInfo.h"
#include "..\Servlet\BalanceMethod.h"
#include "..\NetBarUser.h"
#include "..\Servlet\MemberInfoMethod.h"
#include "..\LocalCenter\gxx_base.h"
#include "..\activemember.h"
#include "..\LocalCenter\LocalCenter.h"
#include "..\LocalMember.h"

using namespace NS_SERVLET;

namespace NS_DAL
{

class CIBADAL
{
	SINGLETON_DECLARE(CIBADAL)
	
public :
	static CIBADAL* GetInstance();

	BOOL GetDBConnInfo(CADODBConnInfo* & pDb, BOOL bForceConnect = TRUE);

	BOOL ExecuteSQL(CString strSQL);

	// @return -1: ִ��ʧ��
	// @return >=0: ������Ӱ�������
	int ExecuteSQLEx(CString strSQL);

	int CountOfRecordsWithSQL(CString strSQL);

	BOOL AddCashierToDB();
	BOOL UpdateInternetCafeInfo();
	//ϵͳ�������
	BOOL GetAllSystemConfig(CSystemConfigArray & ConfigArray);
	BOOL GetMaxKeyId(INT & nMaxKeyId);
	BOOL AddSystemConfig(CConfigInfo & configInfo);
	BOOL GetSystemConfig(CConfigInfo & configInfo);
	BOOL UpdateSystemConfig(CConfigInfo & configInfo, UINT whichKey = 1);

	BOOL GetAllIDTypeInfo(CIDTypeInfoArray& IdTypeInfos);
	BOOL GetAllNetBarInfo(CNetBarInfoArray& NetBarInfoArray);
	BOOL GetAllPCClassInfo(CIDNameRecordArray& IDNameRecords);
	BOOL GetAllUserClassInfos(CUserClassInfoArray & UserClassInfoRecords);

	BOOL GetShiftCash(CShiftCashArray& ShiftCashArray, CString strStartTime, CString strEndTime, CString strCashier);
	int GetLocalCounting(int &Keyvalue1, int &Keyvalue2);
	BOOL UpdateLastUpdateTime_LocalCounting();
	BOOL UpdateCashierandTime_LocalCounting();
	BOOL AddLocalCounting();
	
	//��ֵ�˿��

	BOOL AddCardCreditRecord(CCreditInfoDB & CreditInfo);
	BOOL AddCreditRecord(CCreditInfoDB & CreditInfo);
	BOOL AddCardOpenRecord(CCardOpenInfo & CardOpenInfo);
	BOOL AddCardCloseRecord(CCardCloseInfo & CardCloseInfo);
	BOOL AddReturnedRecord(CReturnedInfo & ReturnedInfo);
	BOOL AddShiftCashRecord(CShiftCash & ShiftCash);
	BOOL GetShiftCashRecord(INT &nRemainBalance);
	BOOL AddCreditReverseInfo(CCreditInfoDB& CreditInfo);

	

	// 2011/07/19-8201-gxx:
	BOOL AddLocalConsume(CLocalConsumeInfo& ConsumeInfo); 
	BOOL DeleteLocalConsume(UINT nMemberID);
	void GetLocalConsumes(CArray<CLocalConsumeInfo,CLocalConsumeInfo&>& ConsumeInfoArray);

	/**
	* ��ȡ����ϻ����û�
	*/
	BOOL GetTermLastUser(CString strTermId, UINT& nMemberId, CString& strNetId);
	BOOL GetNetBarUserInfo(UINT nMemberId, CNetBarUser & NetBarUser);
	BOOL GetMemberId(CString strNetId, UINT &nMemberId);
	BOOL DeleteLastUser(UINT nMemberId);

	//���������
	//ȡ����Ʒ��λ
	BOOL GetCommodityUint(CString strUintCode, CString & strCNUintName);

	BOOL GetCommodity(CCommodityArray& CommodityArray, CCommodityQueryCondition & CommodityQueryCondition);

	BOOL GetStoreCommodity(CCommodityArray& CommodityArray, CCommodityQueryCondition & CommodityQueryCondition);
	//���۸����
	BOOL GetPriceCommodity(CCommodityArray& CommodityArray, CCommodityQueryCondition & CommodityQueryCondition);
	//������ͼ۸����
	BOOL GetSortPriceCommodity(CCommodityArray& CommodityArray, CCommodityQueryCondition & CommodityQueryCondition);
	BOOL AddRetailVoucher(CRetailVoucher& RetailVoucher);
	BOOL AddRetailDetail(CRetailDetail& RetailDetail);
	BOOL GetRetailInfo(CRetailInfoArray & RetailInfoArray, CRetailQueryCondition & RetailQueryCondition);
	BOOL GetCommoditySoldInfo(CString strCommodityCode, CCommoditySoldInfo& CommoditySoldInfo);
	
	BOOL ZeroAllCommodityRemainQuantity();
	BOOL UpdateCommodityRemainQuantity(CString strCommodityCode, UINT nRemainQuantity);
	
	BOOL ZeroAllCommodityStoreInQuantity();
	BOOL UpdateCommodityStoreInQuantity(CString strCommodityCode, UINT nStoreInQuantity, UINT nStoreInAmount);

	//��Ʒͳ������
	BOOL GetCommoditySoldInfo(CCommoditySoldInfoArray& CommoditySoldInfoArray);
	
	//�û�����
	BOOL AddMemberUpgradeRecord(CMemberUpgradeDB& MemberUpgradeInfo);
	BOOL GetMemberUpgradeRecord(UINT nMemberId, CMemberUpgradeDB& MemberUpgradeInfo);

	//�ͻ�����Ϣ
	BOOL AddClientShortMsg(CClientShortMessage& MemberUpgradeInfo);
	BOOL GetClientShortMsg(CClientShortMsgArray& MsgArray, CString strStart, CString strEnd);

	//�Զ����ɵ��˺�
	BOOL SetAutoNetIdFromRecord(CString newValue);
	CString GetAutoNetIdFromRecord();

	//����Ա��Ϣ
	BOOL AddOperatorRecord(CString& strOperator);
	BOOL GetOperatorRecord(COperatorArray& OperatorArray);

	//ȫ���˿����ݿ����

	UINT GetRoundType(UINT nNetbarId);
	BOOL DeleteAllCheckOutInfo();

	//���µ�ǰ���ߵ��û���Ϣ
	BOOL UpdateCheckOutInfo(CString strNetId, UINT nReturnState);
	BOOL AddCheckOutInfo(CCheckOutinfo& CheckOutinfo);
	BOOL GetCheckOutInfo(CCheckOutInfoArray& CheckOutArray);

	//�ϻ��û�����
	BOOL UpdateLastUserInfo(CString strTermId,  CString strUpdateTime, UINT nMemberId);
	BOOL AddLastUserInfo(CLastUserInfo& LastUserInfo);
	
	//��ѯ���ɰ����ַ�ʽ��ѯ�������nMemberId��ѯ��strTermIdΪ�գ���strTermId��nMemberId = 0��
	BOOL GetLastUserInfo(CLastUserInfo& LastUserInfo, UINT nMemberId = 0, CString strTermId = _T(""));
	
	//ɾ���ض��ն���
	BOOL DeleteLastUserInfo(CString strTermId);
	
	BOOL UpdateMemberInfo(CBalanceMethod & Balance);

	// �������壬��֤��λ�����֤��ַ
	BOOL UpdateMemberInfo(int MemberID, LPCTSTR szNation, LPCTSTR szIDDepart, LPCTSTR szIDAddr, LPCTSTR szTel);

	// 2011/08/16-8201-gxx: �ϴ������û���Ϣʱ����Ҫд���µ�memberID���û���Ϣ��member��
	BOOL AddMemberInfo(CLocalRegisterInfo & registerInfo);
	BOOL UpdateMemberInfo(CLocalConsumeInfo & localInfo, UINT nNewMemberID);

	// 2011/10/25-8230-gxx: ��ȡ�������ն�����
	BOOL GetRoomsComputerCount(CRoomInfo& roomInfo);

	// 2014-6-10 - qsc ��ȡ���¼��
	ULONG GetTableRecordCount(LPCTSTR szTable, LPCTSTR szWhere = NULL);

	// 2014-10-10 - qsc ͨ���ն�ID��ȡIP
	BOOL GetIPFromTermId(LPCTSTR szTermId, CString &strIP);

	// 2014-10-15 - qsc �Ƕ����ز���
	BOOL AddZdRnInfo(LPCTSTR szSN, LPCTSTR szTermId, LPCTSTR szUserName );
	BOOL DelZdRnInfo(LPCTSTR szSN);
	BOOL GetZdRnInfo(LPCTSTR szSN, CString &strTermId, COleDateTime& updateTime);
	void DelZdRnOutDateData();
	// 2014-10-29 - qsc
	BOOL GetZdRnInfoByTermId( LPCTSTR szTermId, CString &strSN, CString &strName );
	BOOL SetHaveChange( LPCTSTR szSN, int HaveChange );
	BOOL GetHaveChange( LPCTSTR szSN, int& HaveChange );
	// 2015-0820 liyajun ��ȡ���� ����ÿСʱ���ٷ�
	BOOL GetCostRate(int nWeek, int nHour, int nClassId, int nPCClass , int& nCostRate );
	CString GetCostRateFromCostRate(int nClassId, int nPCClass);
	//AllowReturn�������ֵ����Balance�ӿ�
	BOOL UpdateAllowReturn(UINT UserClass,UINT AllowReturn,BOOL IsMember);
	int GetAllowReturn(UINT UserClass, UINT* pIsMember = NULL);


	/**
	 * �õ��ն�ID���ڵĻ�������ID, @return 0:��ʾ��Ч�Ļ�������ID
	*/
	int GetPCClassIDWithTermID(LPCTSTR lpszTermID);
	GxxArrayPtr ReadRecordsFromDB( LPCTSTR lpszSql);
	GxxDictionaryPtr ReadRecordFromDB (LPCTSTR lpszSql);

	//LocalMember�����
	int UpdateLocalMember_CreditInfo(UINT nMemberId,UINT nCreditAmount);
	int UpdateLocalMember_RateInfo(UINT nMemberId,CString strRate);
	int UpdateLocalMember_PCClassId(UINT nMemberId,UINT nPCClassId);
	int UpdateLocalMember_CheckInInfo(UINT nMemberId,CString strTermId,COleDateTime localcheckintime,COleDateTime CheckinTime=COleDateTime(0,0,0,0,0,0), BOOL bIsLocalCheckin=FALSE);
	int UpdateLocalMember_PayTypeInfo(UINT nMemberId,UINT nPayType,int nextCheckinTime,BOOL bUpdateLastUpdateTime = TRUE);
	int UpdateLocalMember_BalanceInfo(UINT nMemberId,UINT nNewBalance);
	int UpdateLocalMember_CheckOutInfo(UINT nMemberId,COleDateTime CheckOutTime,BOOL bIsReturned);
	BOOL GetLocalMemberByMemberID(int nMemberId, CLocalMember& LocalMember);
	BOOL GetLocalMemberByTermID(CString strTermID,CLocalMember& LocalMember);
	BOOL GetAllLocalMember(LocalMembers& lMembers);
	BOOL RemoveLocalMember(UINT nMemberId);
	int GetCountOfOnlineUser();

	GxxDictionaryPtr CreateNewLocalMemberFromActiveMember(CActiveMember &ActiveMember, LPCTSTR lpszTermID= NULL);   //��ActiveMember���󴴽�localMember��¼
	GxxDictionaryPtr CreateNewLocalMemberWithMemberID(UINT nMemberID, LPCTSTR lpszTermID = NULL);   //��ActiveMember���CostRate����localMember��¼
	BOOL InsertNewLocalMember(GxxDictionaryPtr memberPtr);
	
	//LocalCredit����
	BOOL InsertNewLocalCredit(UINT nMemberId, UINT nCreditAmount, CString strCheckCode);
	BOOL DeleteLocalCredit(CString strCheckCode);

	//LocalReport����
	BOOL GetLocalReport(CString strNetId, CString strPassword, CLocalReport& lr);

	//Member����
	BOOL GetUserNameFromMember(CString strSerialNum, CString& strUserName);
	BOOL GetCardIdFromMember(CString strSerialNum,UINT& nCardId);
	BOOL GetDepositFromMember(CString strSerialNum,UINT& nDeposit);
	GxxDictionaryPtr GetMemberFromMember(CString strSerialNum, BOOL bUseNetbarId = FALSE);

private :
	CADODBConnPool m_DBPool;//Զ������

	UINT m_nConnectState; //!< ��һ�����ӳɹ�����ʧ��
};

}

using namespace NS_DAL;