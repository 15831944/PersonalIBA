#pragma once
#include "LocalMember.h"

class CActiveMember;

namespace NS_DAL{
class CCheckOutinfo;
class CLocalConsumeInfo;
}

class GxxDictionary;


class CCashierHelper
{
public:
	CCashierHelper(void);
	~CCashierHelper(void);

public:
	static BOOL InitDiankaPlatform();
	static BOOL UninitDiankaPlatform();

	static BOOL SellDianka();// �㿨����

	static BOOL CashSellDianka(LPCTSTR lpURL);// ����ͻ��˷��͹������ֽ���㿨������

public://����ģʽ

	static void MakeCheckOutInfo(CActiveMember& , NS_DAL::CCheckOutinfo&);

	static void MakeLocalConsumeInfo(CActiveMember& , NS_DAL::CLocalConsumeInfo&);

	static BOOL RemoveLocalCredit(CString checkCode);//ɾ�����س�ֵ��¼

	static BOOL RemoveLocalCredit(UINT MemberId);//ɾ�����س�ֵ��¼

	static UINT CalcExpense(CString rate,time_t lastUpdateTime,UINT nLastBalance);//���ؼƷ�

	static UINT CalcRemainTime(UINT nBalance,time_t LastUpdateTime,CString strRate);//����ʣ��ʱ��

	static CString DoCredit_InLocal(CActiveMember&, UINT nCreditMoeny);//���س�ֵ
	
	static void ReportLocalCredit(GxxDictionary* pLocalMember);// �ϱ����س�ֵ��¼������
	
	static void ReportLocalReturn(CLocalMember& localMember);//�ϱ������˿��¼
	
	static void ReportLocalReturnVip(CLocalMember& localMember);//�ϱ����ؽ��˼�¼
	
	static void ReportLocalConsume(CLocalMember& localMember);//�ϱ��������Ѽ�¼
};
