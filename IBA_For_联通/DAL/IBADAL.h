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

	BOOL AddCashierToDB();
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

	// 2011/08/16-8201-gxx: �ϴ������û���Ϣʱ����Ҫд���µ�memberID���û���Ϣ��member��
	BOOL AddMemberInfo(CLocalRegisterInfo & registerInfo);
	BOOL UpdateMemberInfo(CLocalConsumeInfo & localInfo, UINT nNewMemberID);

	// 2011/10/25-8230-gxx: ��ȡ�������ն�����
	BOOL GetRoomsComputerCount(CRoomInfo& roomInfo);

private :
	CADODBConnPool m_DBPool;//Զ������

	UINT m_nConnectState; //!< ��һ�����ӳɹ�����ʧ��
};

}

using namespace NS_DAL;