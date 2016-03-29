#pragma once

class CFullBundleTimeInfo
{
public:
	CFullBundleTimeInfo();

	BOOL IsNowEnabled(); //�����Ƿ����

	UINT GetClassId() const { return m_nClassId; }
	void SetClassId(UINT newVal) { m_nClassId = newVal; }

	CString GetBundleTimeArea() const { return m_strBundleTimeArea; }
	void SetBundleTimeArea(CString strValue) { m_strBundleTimeArea = strValue; }

	CString GetStartTime(BOOL bForce = FALSE);
	void SetStartTime(CString strValue) { m_strStartTime = strValue; }

	CString GetEndTime(BOOL bForce = FALSE);
	void SetEndTime(CString strValue) { m_strEndTime = strValue; }

	UINT GetBundleTimeId() const { return m_nBundleTimeId; }
	void SetBundleTimeId(UINT nValue) { m_nBundleTimeId = nValue; }

	UINT GetBundleTimeType() const { return m_nBundleTimeType; }
	void SetBundleTimeType(UINT nValue) { m_nBundleTimeType = nValue; }
	CString GetBundleTimeTypeString();

	UINT GetTimePass() const { return m_nTimePass; }
	void SetTimePass(UINT nValue) { m_nTimePass = nValue; }

	UINT GetBundleTimeMoney() const { return m_nBundleTimeMoney; }
	void SetBundleTimeMoney(UINT nValue) { m_nBundleTimeMoney = nValue; }

	CString GetDescription() const { return m_strDescription; }
	void SetDescription(CString strValue) { m_strDescription = strValue; }

private:

	CString m_strDescription;	//��ʱ��Ϣ����
	CString m_strBundleTimeArea;//��ʱ����
	CString m_strStartTime;		//��ʱ��ʼʱ��
	CString m_strEndTime;		//��ʱ��ֹʱ��

	UINT	m_nClassId;			//�û�����
	UINT	m_nBundleTimeMoney;	//��ʱ���
	UINT    m_nTimePass;		//��ʱʱ��
	UINT	m_nBundleTimeId;	//��ʱID�����ֲ�ͬʱ�ΰ�ʱ
	UINT	m_nBundleTimeType;	//��ʱ���ͣ����ֲ�ͬ�۷ѷ�ʽ
};

class CNetbarBundleTime : public CThread
{
public:
	CNetbarBundleTime(void);
	~CNetbarBundleTime(void);

	typedef CArray<CFullBundleTimeInfo, CFullBundleTimeInfo&> CBundleTimeInfoArray;

	CBundleTimeInfoArray m_BundleTimeInfoArray;//��ʱ����

	BOOL LoadBundTimeInfoFromServer();

	BOOL GetBundleTimeInfo(UINT nClassId, UINT nBundleTimeId, CFullBundleTimeInfo& BundleTimeInfo);

private:

	virtual int Run();

};
