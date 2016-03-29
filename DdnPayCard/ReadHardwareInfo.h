#pragma once

#include <string>

class CReadHardwareInfo
{
public:
	CReadHardwareInfo(void);
	~CReadHardwareInfo(void);

	// ��ȡ����������
	CString GetHostCode();

	// ��ȡ���ܺ�Ļ�����
	CString GetHostCodeEx(CString& strHost);

private:
	BOOL _getCode();
	BYTE _szCodeBuffer[4096];
	UINT _nBufferLen;
};	

BOOL WinNTHDSerialNumAsPhysicalRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen );
BOOL WinNTHDSerialNumAsScsiRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen );
UINT FindAwardBios( BYTE** ppBiosAddr );
UINT FindAmiBios( BYTE** ppBiosAddr );
UINT FindPhoenixBios( BYTE** ppBiosAddr );
BOOL Get(OUT BYTE* szSystemInfo, OUT UINT* pLen );