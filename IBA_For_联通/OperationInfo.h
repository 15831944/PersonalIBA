#pragma once


class COperationInfo 
{
public:
	COperationInfo();
	virtual ~COperationInfo();

private:
	//ʱ��
	COleDateTime m_OperationTime;
	//���룬��ʱ�û����У���ԱΪ��
	CString m_strPassWord;
	//����
	CString m_strUserName;
	//�����˺�
	CString m_strNetId;
	//������Ϣ(���ǳ�ȥԭ�ȵ���Ϣ�ֶ���ĸ���)
	CString m_strDescription;
	//��������,0���޴��������1Ϊ��ֵ��2Ϊ�˿3Ϊ������4Ϊ����
	UINT m_nOperationType;
	//�������˿����ֵ���, ������������Ѻ��
	UINT m_nOperationAmount;
	//�û���
	UINT m_nMemberId;
	//Ѻ��
	UINT m_nDeposit;
	//��ֵ���ͽ��
	UINT m_nPresentAmount;
	//�˿�ԭ�˺����
	UINT m_nAllAmount;
	//�Ƿ��а�ʱ����
	BOOL m_bIsBundTime;
	//��ʱ���
	UINT m_nBundTimeAmount;
	
	BOOL m_bIsMember;

	GProperty(IsResumeData, BOOL); // �Ƿ�Ϊ������������ʱ�Ĳ���
	GProperty(ClassId, UINT); // �û�����
	GProperty(TermId, CString); // �ն˺�
	GProperty(CheckInTime, COleDateTime); // �ϻ�ʱ��
	GProperty(IdNum, CString); // ֤������
	GProperty(ConsumeMoney, INT); // ���ѽ��

public:
	COleDateTime GetOperationTime() const { return m_OperationTime; }
	void SetOperationTime(COleDateTime NewValue) { m_OperationTime = NewValue; }

	CString GetPassWord() const { return m_strPassWord; }
	void SetPassWord(CString NewValue) { m_strPassWord = NewValue; }

	CString GetUserName() const { return m_strUserName; }
	void SetUserName(CString NewValue) { m_strUserName = NewValue; }

	CString GetNetId() const { return m_strNetId; }
	void SetNetId(CString NewValue) { m_strNetId = NewValue; }

	CString GetDescription() const { return m_strDescription; }
	void SetDescription(CString val) { m_strDescription = val; }

	UINT GetOperationType() const { return m_nOperationType; }
	void SetOperationType(UINT NewValue) { m_nOperationType = NewValue; } 

	UINT GetOperationAmount() const { return m_nOperationAmount; }
	void SetOperationAmount(UINT NewValue) { m_nOperationAmount = NewValue; } 

	UINT GetDeposit() const { return m_nDeposit; }
	void SetDeposit(UINT NewValue) { m_nDeposit = NewValue; } 

	UINT GetAllAmount() const { return m_nAllAmount; }
	void SetAllAmount(UINT NewValue) { m_nAllAmount = NewValue; } 

	UINT GetMemberId() const { return m_nMemberId; }
	void SetMemberId(UINT NewValue) { m_nMemberId = NewValue; } 

	BOOL GetIsBundTime() const { return m_bIsBundTime; }
	void SetIsBundTime(BOOL NewValue) { m_bIsBundTime = NewValue; } 

	UINT GetBundTimeAmount() const { return m_nBundTimeAmount; }
	void SetBundTimeAmount(UINT NewValue) { m_nBundTimeAmount = NewValue; } 

	UINT GetPresentAmount() const { return m_nPresentAmount; }
	void SetPresentAmount(UINT NewValue) { m_nPresentAmount = NewValue; } 

	BOOL GetIsMember() const { return m_bIsMember; }
	void SetIsMember(BOOL newVal) { m_bIsMember = newVal; }
};


