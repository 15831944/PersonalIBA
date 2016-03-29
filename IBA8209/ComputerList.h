#pragma once

#include "ComputerInfo.h"

class CComputerList
{
public:

	class CStatInfo
	{
	public:

		friend class CComputerList;

		CStatInfo()
		{
			m_nOnline = 0;
			m_nLock = 0;
			m_nUnLock = 0;
			m_ManCount = 0;
			m_WomanCount = 0;
			m_nAllCount = 0;
			m_ThiefCount = 0;
			//��Ա�û���Ŀ
			m_nMemberUserCount = 0;
			//��ͨ�û���Ŀ
			m_nCommonUserCount = 0;
			//��ʱ�û�
			m_nBundTimePayCount = 0;
			//��ͨ�Ʒ��û�
			m_nCommonPayCount = 0;
		}

		UINT GetOnline() const { return m_nOnline; }
		UINT GetLock() const { return m_nLock; }
		UINT GetUnLock() const { return m_nUnLock; }
		UINT GetManCount() const { return m_ManCount; }
		UINT GetWomanCount() const { return m_WomanCount; }
		UINT GetThiefCount() const { return m_ThiefCount; }
		UINT GetAllCount() const { return m_nAllCount; }
		//��ͨ�û���Ŀ
		UINT GetCommonUserCount() const { return m_nCommonUserCount; }
		//��Ա�û���Ŀ
		UINT GetMemberUserCount() const { return m_nMemberUserCount; }
		//��ʱ�û���Ŀ
		UINT GetBundTimePayCount() const { return m_nBundTimePayCount; }
		//��ͨ�Ʒ��û�
		UINT GetCommonPayCount() const { return m_nCommonPayCount; }

	private:

		UINT m_nAllCount;

		UINT m_nOnline;

		UINT m_nLock;

		UINT m_nUnLock;

		UINT m_ManCount;

		UINT m_WomanCount;

		UINT m_ThiefCount;
		//��Ա�û���Ŀ
		UINT m_nMemberUserCount;
		//��ͨ�û���Ŀ
		UINT m_nCommonUserCount;
		//��ʱ�û�
		UINT m_nBundTimePayCount;
		//��ͨ�Ʒ��û�
		UINT m_nCommonPayCount;
	};

public:
	CComputerList(void);
	~CComputerList(void);

private :

	//!<�洢ȫ���ն˵�������
	CArray<CComputerInfo, CComputerInfo&> m_ComputerArray;
	
	//!<�洢���µĵ������������ڿ���ˢ�½�����ʾ
	CArray<INT_PTR, INT_PTR&> m_UpdateIndexArray;
	
	//!<�洢�ؼ��֣�����׼ȷ�Ŀ��ٲ��ң������ڲ�ʹ��
	std::map<CString, INT_PTR> m_ComputerKeyMap;

	//!<�洢�����ն˺�����ĵ�������������ģ������
	CArray<INT_PTR, INT_PTR&> m_SortIndexArray;

public :

	BOOL AddComputer(CComputerInfo& computer);
	
	BOOL UpdateComputer(INT_PTR nIndex, CComputerInfo& computer);

	void Clear();

	INT_PTR GetCount();

	//!<��ȷ����
	INT_PTR Find(CString strTermID, CString strComputerIP);
	INT_PTR Find(UINT nMemberID);

	/**
	* ģ������
	* @param nKeyType �ؼ�������
	* @param lpszKeyValue �ؼ���
	*/
	INT_PTR LookFor(UINT nKeyType, LPCTSTR lpszKeyValue);

	void MakeComputerKeyMap();

	/**
	* ��ȡ������Ϣ��
	* @param ��������
	*/
	CComputerInfo& GetComputer(INT_PTR nIndex);

	// 2011/06/20-gxx: ��ȡ������Ϣ
	BOOL GetComputerInfo(UINT nMemberID, CComputerInfo&);

	BOOL GetComputerInfo(CString strTermID, CComputerInfo&);

	/**
	* ��ȡ������µĵ�����Ϣ��������ֻҪ��Ϊ�����Ч�ʡ�
	* @return С��0���ʾʧ��
	*/
	INT_PTR GetUpdateComputer();
	
	/**
	* �ж��ǲ�����Ч��������
	*/
	BOOL IsInvaildIndex(INT_PTR nIndex);

	/**
	* ��ȡͳ����Ϣ
	*/
	BOOL GetStatInfo(CStatInfo & StatInfo);
	
	/**
	* �������еĵ���״̬
	*/
	void UpdateAllComputerStatus();

	/**
	* ����û���Ϣ
	*/
	void ClearComputerUserInfo(UINT nMemberId);
};
