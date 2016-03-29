#pragma once

namespace NS_DAL
{
// 	class CMemberInfoDB
// 	{
// 	private:
// 		CString m_CreditDate;
// 		UINT m_nClassID;
// 		UINT m_nNetBarId;
// 		UINT m_nRefNo;
// 		UINT m_nSerialNo;
// 		UINT m_nMemberId;
// 		UINT m_nCreditAmount;
// 		CString m_strOperator;
// 	public:
// 		CMemberInfoDB()
// 		{
// 			
// 		}
// 
// 	protected:
// 
// 	
// 	};


	class CMemberUpgradeDB
	{
	public:

		CMemberUpgradeDB()
		{
			m_nNetBarId = 0;//���ɺ�
			m_nRefNo = 0;//����������
			m_nSerialNo = 0;//���ݴ�����ˮ��
			m_nMemberId = 0;//�û���
			m_nClassId = 0;//�û�����ǰ��
			m_nOldClassId = 0;//�û������ȥ��
		}

	private :

		CString m_strUpgradeTime;//�û�����ʱ��
		UINT m_nNetBarId;//���ɺ�
		UINT m_nRefNo;//����������
		UINT m_nSerialNo;//���ݴ�����ˮ��
		UINT m_nMemberId;//�û���
		UINT m_nClassId;//�û�����ǰ��
		UINT m_nOldClassId;//�û������ȥ��
		CString m_strReason;//�û���������ԭ��
		CString m_strOperator;//������

	public :
		//�û�����ʱ��
		CString GetUpgradeTime() const { return m_strUpgradeTime; }
		void SetUpgradeTime(CString newVal) { m_strUpgradeTime = newVal; }
		//�û������ȥID
		UINT GetOldClassId() const { return m_nOldClassId; }
		void SetOldClassId(UINT newVal) { m_nOldClassId = newVal; }
		//�û�����ǰID
		UINT GetClassId() const { return m_nClassId; }
		void SetClassId(UINT newVal) { m_nClassId = newVal; }
		//���ɺ�
		UINT GetNetBarId() const { return m_nNetBarId; }
		void SetNetBarId(UINT newVal) { m_nNetBarId = newVal; }
		//���ݴ�����ˮ��
		UINT GetSerialNo() const { return m_nSerialNo; }
		void SetSerialNo(UINT newVal) { m_nSerialNo = newVal; }
		//����������
		UINT GetRefNo() const { return m_nRefNo; }
		void SetRefNo(UINT newVal) { m_nRefNo = newVal; }
		//�û���
		UINT GetMemberId() const { return m_nMemberId; }
		void SetMemberId(UINT newVal) { m_nMemberId = newVal; }
		//������
		CString GetOperator() const { return m_strOperator; }
		void SetOperator(CString newVal) { m_strOperator = newVal; }
		//�û���������ԭ��
		CString GetReason() const { return m_strReason; }
		void SetReason(CString newVal) { m_strReason = newVal; }

	};


	class CCreditInfoDB
	{
	public:

		CCreditInfoDB();

	private :

		CString m_CreditDate;
		UINT m_nNetBarId;
		UINT m_nRefNo;
		UINT m_nSerialNo;
		UINT m_nMemberId;
		UINT m_nCreditAmount;
		CString m_strOperator;
		CString m_strCategory;
		CString m_strCardSerial;
		UINT m_nCreditPresent;
		UINT m_nPoint;
		UINT m_nCashRegisterID;
		UINT m_nWeek;
		UINT m_nCassId;
		UINT m_nStatus;
		CString m_strDescription;

	public :	
		UINT GetCassId() const { return m_nCassId; }
		void SetCassId(UINT newVal) { m_nCassId = newVal; }
		UINT GetNetBarId() const { return m_nNetBarId; }
		void SetNetBarId(UINT newVal) { m_nNetBarId = newVal; }
		UINT GetSerialNo() const { return m_nSerialNo; }
		void SetSerialNo(UINT newVal) { m_nSerialNo = newVal; }
		UINT GetRefNo() const { return m_nRefNo; }
		void SetRefNo(UINT newVal) { m_nRefNo = newVal; }
		UINT GetMemberId() const { return m_nMemberId; }
		void SetMemberId(UINT newVal) { m_nMemberId = newVal; }
		UINT GetCreditAmount() const { return m_nCreditAmount; }
		void SetCreditAmount(UINT newVal) { m_nCreditAmount = newVal; }
		CString GetOperator() const { return m_strOperator; }
		void SetOperator(CString newVal) { m_strOperator = newVal; }
		CString GetCategory() const { return m_strCategory; }
		void SetCategory(CString newVal) { m_strCategory = newVal; }
		CString GetCardSerial() const { return m_strCardSerial; }
		void SetCardSerial(CString newVal) { m_strCardSerial = newVal; }
		UINT GetCreditPresent() const { return m_nCreditPresent; }
		void SetCreditPresent(UINT newVal) { m_nCreditPresent = newVal; }
		UINT GetPoint() const { return m_nPoint; }
		void SetPoint(UINT newVal) { m_nPoint = newVal; }
		UINT GetCashRegisterID() const { return m_nCashRegisterID; }
		void SetCashRegisterID(UINT newVal) { m_nCashRegisterID = newVal; }
		UINT GetWeek() const { return m_nWeek; }
		void SetWeek(UINT newVal) { m_nWeek = newVal; }
		CString GetDescription() const { return m_strDescription; }
		void SetDescription(CString newVal) { m_strDescription = newVal; }
		UINT GetStatus() const { return m_nStatus; }
		void SetStatus(UINT newVal) { m_nStatus = newVal; }
		CString GetCreditDate() const { return m_CreditDate; }
		void SetCreditDate(CString newVal) { m_CreditDate = newVal; }
	};

	typedef CArray<CCreditInfoDB, CCreditInfoDB&> CCreditInfoDBArray;


	class CCardOpenInfo
	{
	public:

		CCardOpenInfo();

	private:

		CString m_strOperationDateTime;
		UINT m_nNetBarId;
		UINT m_nRefNo;
		UINT m_nSerialNo;
		UINT m_nMemberId;
		UINT m_nCardId;
		UINT m_nDeposit;
		UINT m_nCostExpense;
		CString m_strOperator;
		UINT m_nCashRegisterID;
		UINT m_nWeek;
		UINT m_nClassId;
		UINT m_nStatus;
		CString m_strDescription;
		CString m_strSerialNum;
		CString m_strIdNumber;
		UINT m_nIdType;
		UINT m_nAccountNetBarId;
		CString m_strUserName;
		UINT m_nUserSex;

	public:

		UINT GetUserSex() const { return m_nUserSex; }
		void SetUserSex(UINT newVal) { m_nUserSex = newVal; }
		UINT GetAccountNetBarId() const { return m_nAccountNetBarId; }
		void SetAccountNetBarId(UINT newVal) { m_nAccountNetBarId = newVal; }
		CString GetUserName() const { return m_strUserName; }
		void SetUserName(CString newVal) { m_strUserName = newVal; }
		UINT GetIdType() const { return m_nIdType; }
		void SetIdType(UINT newVal) { m_nIdType = newVal; }
		CString GetIdNumber() const { return m_strIdNumber; }
		void SetIdNumber(CString newVal) { m_strIdNumber = newVal; }
		CString GetSerialNum() const { return m_strSerialNum; }
		void SetSerialNum(CString newVal) { m_strSerialNum = newVal; }
		CString GetOperationDateTime() const { return m_strOperationDateTime;}
		UINT GetNetBarId() const { return m_nNetBarId;}
		UINT GetRefNo() const { return m_nRefNo;}
		UINT GetSerialNo() const { return m_nSerialNo;}
		UINT GetMemberId() const { return m_nMemberId;}
		UINT GetCardId() const { return m_nCardId;}
		UINT GetDeposit() const { return m_nDeposit;}
		UINT GetCostExpense() const { return m_nCostExpense;}
		CString GetOperator() const { return m_strOperator;}
		UINT GetCashRegisterID() const { return m_nCashRegisterID;}
		UINT GetWeek() const { return m_nWeek;}
		UINT GetClassId() const { return m_nClassId;}
		UINT GetStatus() const { return m_nStatus;}
		CString GetDescription() const { return m_strDescription;}

		void SetOperationDateTime(CString newVal) { m_strOperationDateTime = newVal; }
		void SetNetBarId(UINT newVal) { m_nNetBarId = newVal; }
		void SetRefNo(UINT newVal) { m_nRefNo = newVal; }
		void SetSerialNo(UINT newVal) { m_nSerialNo = newVal; }
		void SetMemberId(UINT newVal) { m_nMemberId = newVal; }
		void SetCardId(UINT newVal) { m_nCardId = newVal; }
		void SetDeposit(UINT newVal) { m_nDeposit = newVal; }
		void SetCostExpense(UINT newVal) { m_nCostExpense = newVal; }
		void SetOperator(CString newVal) { m_strOperator = newVal; }
		void SetCashRegisterID(UINT newVal) { m_nCashRegisterID = newVal; }
		void SetWeek(UINT newVal) { m_nWeek = newVal; }
		void SetClassId(UINT newVal) { m_nClassId = newVal; }
		void SetStatus(UINT newVal) { m_nStatus = newVal; }
		void SetDescription(CString newVal) { m_strDescription = newVal; }
	};


	class CCardCloseInfo
	{
	public:

		CCardCloseInfo();

	private:

		CString m_strLossDate;
		UINT m_nNetBarId;
		UINT m_nRefNo;
		UINT m_nSerialNo;
		UINT m_nMemberId;
		UINT m_nCardId;
		UINT m_nDeposit;
		CString m_strOperator;
		CString m_strAppendDate;
		CString m_strDescription;
		UINT m_nCashRegisterID;
		UINT m_nCostExpense;
		UINT m_nClassId;

	public :

		CString GetLossDate() const { return m_strLossDate;}
		void SetLossDate(CString newVal) { m_strLossDate = newVal; }
		UINT GetNetBarId() const { return m_nNetBarId;}
		void SetNetBarId(UINT newVal) { m_nNetBarId = newVal; }
		UINT GetRefNo() const { return m_nRefNo;}
		void SetRefNo(UINT newVal) { m_nRefNo = newVal; }
		UINT GetSerialNo() const { return m_nSerialNo;}
		void SetSerialNo(UINT newVal) { m_nSerialNo = newVal; }
		UINT GetMemberId() const { return m_nMemberId;}
		void SetMemberId(UINT newVal) { m_nMemberId = newVal; }
		UINT GetCardId() const { return m_nCardId;}
		void SetCardId(UINT newVal) { m_nCardId = newVal; }
		UINT GetDeposit() const { return m_nDeposit;}
		void SetDeposit(UINT newVal) { m_nDeposit = newVal; }
		CString GetOperator() const { return m_strOperator;}
		void SetOperator(CString newVal) { m_strOperator = newVal; }
		CString GetAppendDate() const { return m_strAppendDate;}
		void SetAppendDate(CString newVal) { m_strAppendDate = newVal; }
		CString GetDescription() const { return m_strDescription;}
		void SetDescription(CString newVal) { m_strDescription = newVal; }
		UINT GetCashRegisterID() const { return m_nCashRegisterID;}
		void SetCashRegisterID(UINT newVal) { m_nCashRegisterID = newVal; }
		UINT GetCostExpense() const { return m_nCostExpense;}
		void SetCostExpense(UINT newVal) { m_nCostExpense = newVal; }
		UINT GetClassId() const { return m_nClassId;}
		void SetClassId(UINT newVal) { m_nClassId = newVal; }

	};



	class CReturnedInfo
	{
	public:

		CReturnedInfo();

	private:

		CString m_strReturnDate;
		UINT m_nNetBarId;
		UINT m_nRefNo;
		UINT m_nSerialNo;
		UINT m_nMemberId;
		UINT m_nCardId;
		UINT m_nReturnAmount;

		UINT m_nDeposit;
		CString m_strOperator;
		CString m_strAppendDate;
		CString m_strDescription;
		UINT m_nCashRegisterID;
		UINT m_nCostExpense;
		UINT m_nClassId;
		UINT m_nRoundAmount;

	public :

		CString GetReturnDate() const { return m_strReturnDate;}
		void SetReturnDate(CString newVal) { m_strReturnDate = newVal; }
		UINT GetNetBarId() const { return m_nNetBarId;}
		void SetNetBarId(UINT newVal) { m_nNetBarId = newVal; }
		UINT GetRefNo() const { return m_nRefNo;}
		void SetRefNo(UINT newVal) { m_nRefNo = newVal; }
		UINT GetSerialNo() const { return m_nSerialNo;}
		void SetSerialNo(UINT newVal) { m_nSerialNo = newVal; }
		UINT GetMemberId() const { return m_nMemberId;}
		void SetMemberId(UINT newVal) { m_nMemberId = newVal; }
		UINT GetCardId() const { return m_nCardId;}
		void SetCardId(UINT newVal) { m_nCardId = newVal; }
		UINT GetReturnAmount() const { return m_nReturnAmount; }
		void SetReturnAmount(UINT newVal) { m_nReturnAmount = newVal; }
		UINT GetDeposit() const { return m_nDeposit;}
		void SetDeposit(UINT newVal) { m_nDeposit = newVal; }
		CString GetOperator() const { return m_strOperator;}
		void SetOperator(CString newVal) { m_strOperator = newVal; }
		UINT GetRoundAmount() const { return m_nRoundAmount; }
		void SetRoundAmount(UINT newVal) { m_nRoundAmount = newVal; }
		CString GetDescription() const { return m_strDescription;}
		void SetDescription(CString newVal) { m_strDescription = newVal; }
		UINT GetCashRegisterID() const { return m_nCashRegisterID;}
		void SetCashRegisterID(UINT newVal) { m_nCashRegisterID = newVal; }
		UINT GetCostExpense() const { return m_nCostExpense;}
		void SetCostExpense(UINT newVal) { m_nCostExpense = newVal; }
		UINT GetClassId() const { return m_nClassId;}
		void SetClassId(UINT newVal) { m_nClassId = newVal; }

	};


}