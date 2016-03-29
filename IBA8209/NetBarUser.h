#pragma once

class CNetBarUser
{
public:
	CNetBarUser(void);
	~CNetBarUser(void);

	BOOL GetCanEditUserInfo() const { return m_bCanEditUserInfo; }
	void SetCanEditUserInfo(BOOL newVal) { m_bCanEditUserInfo = newVal; }

	CString GetUserName() const { return m_strUserName; }
	void SetUserName(CString newVal) { m_strUserName = newVal; }

	CString GetPassword() const { return m_strPassword; }
	void SetPassword(CString newVal) { m_strPassword = newVal; }

	UINT GetSex() const { return m_Sex; }
	void SetSex(UINT newVal) { m_Sex = newVal; }

	CString GetAddr() const { return m_strAddr; }
	void SetAddr(CString newVal) { m_strAddr = newVal; }

	CString GetTel() const { return m_strTel; }
	void SetTel(CString newVal) { m_strTel = newVal; }

	CString GetIDNo() const { return m_strIDNo; }
	void SetIDNo(CString newVal) { m_strIDNo = newVal; }

	CString GetCertificateID() const { return m_strCertificateID; }
	void SetCertificateID(CString newVal) { m_strCertificateID = newVal; }

	UINT GetCertificateType() const { return m_nCertificateType; }
	void SetCertificateType(UINT newVal) { m_nCertificateType = newVal; }
	
	CString GetNetId() const { return m_strNetId; }
	void SetNetId(CString newVal) { m_strNetId = newVal; }

	UINT GetMemberId() const { return m_nMemberId; }
	void SetMemberId(UINT newVal) { m_nMemberId = newVal; }

	CString GetNation() const { return m_strNation; }
	void SetNation(CString newVal) { m_strNation = newVal; }

	CString GetDepartment() const { return m_strDepartment; }
	void SetDepartment(CString newVal) { m_strDepartment = newVal; }

	BOOL GetIsMember() const { return m_bIsMember; }
	void SetIsMember(BOOL newVal) { m_bIsMember = newVal; }

	UINT GetDataFrom() const { return m_nDataFrom; }
	void SetDataFrom(UINT newVal) { m_nDataFrom = newVal; }
	
	CString GetSignDate() const { return m_strSignDate; }
	void SetSignDate(CString newVal) { m_strSignDate = newVal; }

private :

	//������Դ��ɨ�軹��ˢ����
	UINT m_nDataFrom;
	
	//���ڿ���ʱ���Ƿ�����༭�û���Ϣ
	BOOL m_bCanEditUserInfo; 

	CString m_strNetId;

	CString m_strPassword;

	CString m_strUserName;

	CString m_strAddr;

	CString m_strTel;

	CString m_strIDNo;

	CString m_strCertificateID;

	CString m_strSignDate;

	UINT m_nCertificateType;

	CString m_strNation;

	CString m_strDepartment;

	UINT m_Sex;

	UINT m_nCardType;

	BOOL m_bIsMember;

	UINT m_nMemberId;

	GProperty(DeleteKey, BOOL);
	GProperty(ScanType, INT); // ɨ������:1ɨ��һ��֤,2ɨ�����֤,3��������֤

	GProperty(ActivationType, UINT);

};
