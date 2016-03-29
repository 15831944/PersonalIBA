
#pragma once

#pragma pack(1)
namespace NS_DataX
{


class IDataXSend
{
public:

	virtual INT SendData(INT nMsg, INT nParam1, INT nParam2, 
		LPVOID lpBuf, size_t cbLen)  = 0;

	virtual INT PostData(INT nMsg, INT nParam1, INT nParam2, 
		LPVOID lpBuf, size_t cbLen) = 0;
};

class IDataXReceive
{
public:

	virtual INT OnDataArrival() = 0;

protected:

	virtual INT OnDataArrival(INT nAppId, INT nMsg, INT nParam1, INT nParam2, 
		LPVOID lpBuf, size_t cbLen) = 0;
};

class  CDataXData
{
public:
	CDataXData(void);
	virtual ~CDataXData(void);

	struct DataX_Data_Header
	{
		INT  nAppId;		// �����ţ������ĸ������͵�����
		HWND hWnd;			// ���� WM_DDNDATAX ��Ϣ�Ĵ��ھ��

		INT  nMsg;			// ��Ϣֵ
		INT  nParam1;		// ����1
		INT  nParam2;		// ����2

		size_t cbDataLen;   // ���������ݳ���

		bool bSync;			// �Ƿ�Ϊͬ���ķ�ʽ
		INT  nResult;		// OnDataArrival�ķ���ֵ
	};

public:

	virtual BOOL Create(LPCTSTR lpszMemMapName, 
		size_t nMemMapLen = 1024 * 4, UINT nTimeout = 10 * 1000);

	BOOL WriteData(LPVOID lpBuf, size_t cbLen);

	BOOL ReadData(LPVOID lpBuf, size_t cbLen);

protected:

	BOOL WriteData(DataX_Data_Header& Data_Header, LPVOID lpBuf = NULL, size_t cbLen = 0);

	BOOL ReadData(DataX_Data_Header& Data_Header, LPVOID lpBuf = NULL, size_t cbLen = 0);

	CString GetMemMapName() const { return m_strMemMapName; }

	size_t GetMemMapLen() const { return m_nMemMapLen; }

	UINT GetTimeout() const { return m_nTimeout; }

	static const size_t Data_Header_Offset = 256;

private:

	CString m_strMemMapName;
	
	size_t m_nMemMapLen;

	UINT m_nTimeout;
};


#pragma pack()
}