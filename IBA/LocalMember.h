#pragma once

//��Ӧ
class CLocalMember
{
public:
	CLocalMember::CLocalMember(void):nMemberId(0),nBalance(0),nBeforeUsed(0),
		nNextCheckinTime(0),nPcClass(0),nClassId(0),nPayType(0),nIsLocalCheckin(0),
		nLastUpdateTime(0),nIsReturned(0){};
	CLocalMember::~CLocalMember(void){};

public:
	//��ӦActiveMember��
	int nMemberId;                    //�û���
	int nBalance;		              //���
	int ntotalMoney;                  //�ܽ��(�ܽ���ڴ���ʱ=���)
	int nBeforeUsed;				  //�����ѽ��-��ӦActiveMember.Amount
	CString strMemberName;			  //�û���
	CString strNetId;				  //NetId
	CString strIdNumber;			  //Personal ID
	COleDateTime checkinTime;		  //�ն˵�¼ʱ��
	int nNextCheckinTime;             //��ʱ����ʱ��
	CString strTermId;				  //����LocalMember��¼ʱ,���Դ�������.����=ActiveMember.TermId
	int nPcClass;
	int nClassId;	
	int nPayType;	

	//�����ֶ�
	COleDateTime localCheckinTime;   //HandleLocalLogOnʱ����Ϊ��ǰʱ��
	int nIsLocalCheckin;             //HandleLocalLogOnʱ,��localMember��û��checkinʱ��,��ʾ֮ǰû��½,˵���Ǳ��ص�¼�ն�.��Ϊ1
	int nLastUpdateTime;			 //����ʱΪ�����ĵ�ǰʱ��,���ؼƷѸ���Ϊ�Ʒѵĵ�ǰʱ��
	COleDateTime checkoutTime;       //����ģʽ�½���ʱ�ĵ�ǰʱ��
	int nIsReturned;				 //�Ƿ��ѽ���,����ʱ����0
	CString strRate;				 //��pcclass��classId��CostRate���в��ҵĽ��
};

typedef std::vector<CLocalMember> LocalMembers;

class CLocalReport
{
public:
	CLocalReport::CLocalReport(void):nMemberId(0){};
	CLocalReport::~CLocalReport(void){};

public:
	int nMemberId;
	CString strNetId;
	CString strPassword;
	COleDateTime reportTime;
};