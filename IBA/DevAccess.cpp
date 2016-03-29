#include "stdafx.h"
#include "DevAccess.h"
#include <tchar.h>
#include <winbase.h>
#include <iostream>
#include <winioctl.h>
#include "IBA_DEBUG.h"
#include "ibalog.h"
using namespace std;

//���캯�� - 1.��ȡ�豸 2.�ж��Ƿ�����Ҫ�Ĵ����豸 3.��ʼ�������豸
CDevAccess::CDevAccess()
{
	vector<tstring> DevPaths;
	//vector<tstring> HardwareIDs;
	m_hDev = NULL;
	m_bDevReady = FALSE;
	UINT nDevIdx = 0;

	//��ȡ����COMPORT�ӿ����豸	
	if(GetClassDevs(&GUID_DEVINTERFACE_COMPORT,&DevPaths/*,&HardwareIDs*/))
	{
		for(int i = 0;i < DevPaths.size(); ++i)
		{
			//�ж�pid��vid�Ƿ���ͬ
			TCHAR lpDevPath[256];
			_tcscpy(lpDevPath,DevPaths[i].c_str());
			_tcsupr(lpDevPath);
			IBA_LOG(_T("Find:%d:DevPath:%s"),i,DevPaths[i].c_str());
			TCHAR *p = _tcsstr(lpDevPath,_T("VID_067B&PID_2303"));			
			if(NULL != p)
			//if(0 == _tcscmp(lpHardwareID,_T("USB\\Vid_067b&Pid_2303&Rev_0400")))//��USB-Serial Controller D�Ƚ�
			{
				IBA_LOG(_T("Matched"));
				//�жϷ����ض��ַ���Ľ����ַ��Ƿ�һ��
				//�ȳ�ʼ��
				if(InitPort(DevPaths[i].c_str()))
				{
					IBA_LOG(_T("InitCommOk"));
					//��ʱ������һ��.��ΪӲ����ʱ�򲻷�������
					m_bDevReady = TRUE;
					nDevIdx = i;
					break;

					/*OVERLAPPED ov;
					RtlZeroMemory(&ov,sizeof(OVERLAPPED));
					ov.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

					EnterCriticalSection(&m_csSynCommDevice);

					//д���ض��ַ�
					//char lpWriteBuffer[] = "Mutil:\tA1\t�ܼ�100\thttp://www.dodonew.com";
					char lpWriteBuffer[] = "Logo";
					DWORD dwSize = 0;
					BOOL bSuccess = WriteFile(m_hDev,lpWriteBuffer,(DWORD)(strlen(lpWriteBuffer)),&dwSize,&ov);
					if(FALSE == bSuccess)
					{
						if(ERROR_IO_PENDING == GetLastError())
						{
							WaitForSingleObject(ov.hEvent,1000);
							IBA_LOG(_T("WriteWaitedObject"));
						}
					}
					//�����ַ�
					OVERLAPPED ov_read;
					RtlZeroMemory(&ov_read,sizeof(OVERLAPPED));
					ov_read.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
					char lpRecvBuffer[100];
					RtlZeroMemory(lpRecvBuffer,sizeof(lpRecvBuffer));
					dwSize = 0;
					if(FALSE == ReadFile(m_hDev,lpRecvBuffer,100,&dwSize,&ov_read))
					{
						if(ERROR_IO_PENDING == GetLastError())
						{
							WaitForSingleObject(ov_read.hEvent,1000);
							IBA_LOG(_T("ReadwaitedObject"));
						}
					}
					IBA_LOG(_T("Recved:%s"),lpRecvBuffer);
					if(0 == strcmp(lpRecvBuffer,lpWriteBuffer)) //�ַ���ͬ���ж�Ϊ�����豸
					{
						IBA_LOG(_T("Recved reponse.Ready!"));
						m_bDevReady = TRUE;
					}*/

				/*	OVERLAPPED ov_write;
					RtlZeroMemory(&ov_write,sizeof(OVERLAPPED));
					ov_write.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
					char lpTmp[] = "Logo";
					dwSize = 0;
					bSuccess = WriteFile(m_hDev,lpTmp,(DWORD)(strlen(lpTmp)),&dwSize,&ov_write);
					if(FALSE == bSuccess)
					{
						if(ERROR_IO_PENDING == GetLastError())
						{
							WaitForSingleObject(ov_write.hEvent,1000);
						}
					}
					LeaveCriticalSection(&m_csSynCommDevice);*/
				}
			}
		}

	}

	if(FALSE == m_bDevReady && NULL != m_hDev)
	{
		CloseHandle(m_hDev);
	}
	if(GetReadyState())
	{
		m_sDevPath = DevPaths[nDevIdx];
		//m_sDevLocation = HardwareIDs[0];
	}
}

//��������
CDevAccess::~CDevAccess()
{
	if(NULL != m_hDev)
	{
		CloseHandle(m_hDev);
	}
}

//��ȡĳ���豸�ӿ��������п����豸��·������·��������CreateFile
BOOL CDevAccess::GetClassDevs(const GUID *InterfaceClassGUID, std::vector<std::tstring>* pvDevPaths/*,std::vector<std::tstring>* pvHardwareIDs*/)
{
	if(NULL == InterfaceClassGUID || NULL == pvDevPaths /*|| NULL == pvHardwareIDs*/){return FALSE;}
	pvDevPaths->clear();
	//pvHardwareIDs->clear();

	//��ȡdevice information set
	HDEVINFO hDevInfo =  SetupDiGetClassDevs(InterfaceClassGUID,NULL,NULL,DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if(INVALID_HANDLE_VALUE == hDevInfo)
	{
		return FALSE;
	}

	//ö��device interfaces
	DWORD nMemberIndex = 0;
	SP_DEVICE_INTERFACE_DATA device_interface;
	RtlZeroMemory(&device_interface,sizeof(SP_DEVICE_INTERFACE_DATA));
	device_interface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	while(SetupDiEnumDeviceInterfaces(hDevInfo,NULL,InterfaceClassGUID,nMemberIndex,&device_interface))
	{
		nMemberIndex++;

		TCHAR DevPath[256];
		RtlZeroMemory(DevPath,sizeof(DevPath));

		//��ȡdevice_interface_detail����ռ�
		SP_DEVINFO_DATA device_information_element;
		RtlZeroMemory(&device_information_element,sizeof(SP_DEVINFO_DATA));
		device_information_element.cbSize = sizeof(SP_DEVINFO_DATA);

		DWORD nSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo,&device_interface,NULL,NULL,&nSize,NULL);

		//�ڶ��λ�ȡdetail
		PSP_DEVICE_INTERFACE_DETAIL_DATA pdev_interface_detail;
		pdev_interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)calloc(1,nSize);
		pdev_interface_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if(FALSE == SetupDiGetDeviceInterfaceDetail(hDevInfo,&device_interface,pdev_interface_detail,nSize,NULL,&device_information_element)){break;} 

		_tcscpy(DevPath,pdev_interface_detail->DevicePath);
		free(pdev_interface_detail); //ǧ��ǵ��ͷſռ�
		std::tstring sDevPath = DevPath;

		IBA_LOG(_T("Get:%d:%s"),(nMemberIndex-1),sDevPath.c_str());
		//��ע����ȡ��ַ��Ϣ
		/*TCHAR lpszTmp[512];
		RtlZeroMemory(lpszTmp,sizeof(lpszTmp));
		DWORD dataType;
		DWORD dwBuffersize = sizeof(lpszTmp);
		if (FALSE == SetupDiGetDeviceRegistryProperty(hDevInfo, &device_information_element, SPDRP_HARDWAREID, &dataType, (LPBYTE)lpszTmp, dwBuffersize, 0))
		{
			break;
		}
		std::tstring sHardwareID = lpszTmp;
		SetupDiDestroyDeviceInfoList(hDevInfo);*/

		//���浽vector��
		pvDevPaths->push_back(sDevPath);
		//pvHardwareIDs->push_back(sHardwareID);
	}

	//��ȡ���豸,����true.���򷵻�false
	if(pvDevPaths->size()){return TRUE;}

	return FALSE;
}

BOOL CDevAccess::InitPort(const TCHAR* lpszDevPath,UINT baud, char parity,UINT databits,UINT stopbits)
{
	//�ȴ��豸
	HANDLE hDevice = CreateFile(lpszDevPath,           // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE, // ��д
		0,                     // ��ռģʽcomm devices must be opened w/exclusive-access 
		NULL,                  // no security attributs 
		OPEN_EXISTING,         // ֻ�����д��ڵ�
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,  // �첽��ʽ��(����ͬ���Ͳ���Ҫoverlapped�ṹ)
		NULL);                 // open_existingʱ��ΪNULL
	if(INVALID_HANDLE_VALUE == hDevice){return FALSE;}

	InitializeCriticalSection(&m_csSynCommDevice);
	EnterCriticalSection(&m_csSynCommDevice);

	//���ó�ʱ����
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 1000;        
	CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	if (FALSE == SetCommTimeouts(hDevice, &CommTimeouts)){return FALSE;}

	//���ü���¼�
	DWORD dwCommEvents = EV_RXCHAR;//�豸��ȡ���ַ����¼�
	if(FALSE == SetCommMask(hDevice,dwCommEvents)){return FALSE;}

	//��������
	DCB dcb;
	RtlZeroMemory(&dcb,sizeof(DCB));
	if(FALSE == GetCommState(hDevice,&dcb)){return FALSE;}
	dcb.EvtChar = 'q';
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	char szBaud[128];
	RtlZeroMemory(szBaud,sizeof(szBaud));
	sprintf(szBaud,"baud=%d parity=%c data=%d stop=%d",baud, parity, databits, stopbits);
	if(FALSE == BuildCommDCBA(szBaud,&dcb)){return FALSE;}

	if(FALSE == SetCommState(hDevice,&dcb)){return FALSE;}

	//flush�豸����͵ȴ���д����
	if(FALSE == PurgeComm(hDevice,PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR)){return FALSE;}

	//��ɳ�ʼ��,�ͷ�CS
	LeaveCriticalSection(&m_csSynCommDevice);
	
	m_hDev = hDevice;

	return TRUE;
}

BOOL CDevAccess::GetReadyState()
{
	return m_bDevReady;
}

BOOL CDevAccess::Reset()
{
	if(GetReadyState())
	{
		CloseHandle(m_hDev);
		TCHAR lpBuffer[256];
		_tcscpy(lpBuffer,m_sDevPath.c_str());
		if(InitPort(lpBuffer))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void CDevAccess::ShowLogo()
{
	if(GetReadyState())
	{
		WriteToDev(_T("Logo"),m_sDevPath.c_str());
	}	
}

void CDevAccess::ShowSuccess()
{
	if(GetReadyState())
	{
		WriteToDev(_T("Success"),m_sDevPath.c_str());
	}
}

void CDevAccess::CleanDisplay()
{
	if(GetReadyState())
	{
		WriteToDev(_T("Clean"),m_sDevPath.c_str());
	}
}

void CDevAccess::ShowQrInfo(const TCHAR *lpNumber,const TCHAR *lpPrice,const TCHAR *lpLink)
{
	if(GetReadyState())
	{
		TCHAR lpBuff[256];
		_stprintf(lpBuff,_T("Mutil:\tNO.:%s\t��ֵ:%s\t%s"),lpNumber,lpPrice,lpLink);
		WriteToDev(lpBuff,m_sDevPath.c_str());
	}
}

void CDevAccess::WriteToDev(const TCHAR* lpszCmd,const TCHAR* lpszDevPath)
{
	if(FALSE == GetReadyState())
	{
		return;
	}

	OVERLAPPED ov;
	RtlZeroMemory(&ov,sizeof(OVERLAPPED));
	ov.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	EnterCriticalSection(&m_csSynCommDevice);
	//PurgeComm(m_hDev,PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);

	char lpBuffer[120];
	RtlZeroMemory(lpBuffer,sizeof(lpBuffer));
	//��ת���ַ���
#ifdef _UNICODE
	string sTmp = unicode_to_multibyte(lpszCmd);
	strcpy(lpBuffer,sTmp.c_str());
	lpBuffer[119] = '\0'; //��120λ�ô��ضϣ���ֹд������̫��
#else
	strcpy(lpBuffer,lpszCmd);
#endif
	DWORD dwSize = 0;
	BOOL bSuccess = WriteFile(m_hDev,lpBuffer,(DWORD)(strlen(lpBuffer)),&dwSize,&ov);
	if(FALSE == bSuccess)
	{
		if(ERROR_IO_PENDING == GetLastError())
		{
			WaitForSingleObject(ov.hEvent,1000);
		}
	}	

	LeaveCriticalSection(&m_csSynCommDevice);

	return;
}

//BOOL CDevAccess::ReadDev(TCHAR* lpszOutbuffer,const TCHAR* lpszDevPath)
//{
//	//�����������120������жϣ�
//
//	//�ȴ��豸
//	HANDLE hDevice = CreateFile(lpszDevPath,           // pointer to name of the file
//		GENERIC_READ|GENERIC_WRITE, // ��д
//		0,                     // ��ռģʽcomm devices must be opened w/exclusive-access 
//		NULL,                  // no security attributs 
//		OPEN_EXISTING,         // ֻ�����д��ڵ�
//		FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_OVERLAPPED*/,  // �첽��ʽ��(����ͬ���Ͳ���Ҫoverlapped�ṹ)
//		NULL);                 // open_existingʱ��ΪNULL
//	if(INVALID_HANDLE_VALUE == hDevice){return FALSE;}
//	
//	EnterCriticalSection(&m_csSynCommDevice);
//
//	char lpBuffer[256];
//	RtlZeroMemory(lpBuffer,sizeof(lpBuffer));
//	DWORD dwSize = 0;
//	BOOL bSuccess = ReadFile(hDevice,lpBuffer,100,&dwSize,NULL);
//
//	LeaveCriticalSection(&m_csSynCommDevice);
//	CloseHandle(hDevice);
//
//	if(bSuccess)
//	{
//#ifdef _UNICODE
//		wstring sTmp = multibyte_to_unicode(lpBuffer);
//		wcscpy(lpszOutbuffer,sTmp.c_str());
//#else
//		strcpy(lpszOutbuffer,sTmp.c_str());
//#endif
//		return TRUE;
//	}
//
//	return FALSE;
//}

//ת��unicode��multibyte
std::string CDevAccess::unicode_to_multibyte(const wchar_t* lpUnicode)
{
	wstring str(lpUnicode);

	int  MultiLen = ::WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);  

	char * lpMulti = new  char[MultiLen+1];  

	memset(lpMulti,0,(MultiLen+1)*sizeof(char));  

	::WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPSTR)lpMulti,
		MultiLen ,
		NULL,
		NULL); 

	string strRet(lpMulti);

	delete[] lpMulti;

	return strRet;
}

//ת��unicode��multibyte
wstring CDevAccess::multibyte_to_unicode(const char* lpMultiByte)
{
	string str(lpMultiByte);

	int  UnicodeLen = ::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		//0,
		//NULL,
		NULL);  

	wchar_t * lpUnicode = new  wchar_t[UnicodeLen+1];  

	memset(lpUnicode,0,(UnicodeLen+1)*sizeof(wchar_t));  

	::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)lpUnicode,
		UnicodeLen /*,
		NULL,
		NULL*/); 

	wstring strRet(lpUnicode);

	delete[] lpUnicode;

	return strRet;
}