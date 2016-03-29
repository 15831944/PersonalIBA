
#include "stdafx.h"
#include "IBA.h"
#include ".\computerlist.h"
//#include ".\json\gxx_base.h"
#include ".\Servlet\WinHttpRequest.h"
#include "NetBarConfig.h"
#include "IBAHelpper.h"
#include ".\Servlet\MachReportMethod.h"
#include ".\Socket\LocalServer.h"
#include ".\dal\IDTypeInfo.h"

using namespace NS_SERVLET;

#include <iphlpapi.h> 

#pragma comment(lib, "IPHLPAPI.lib") 

CComputerList::CComputerList(void)
{
	m_pThread = NULL;
	m_Devices.RemoveAll();
	m_nSerialNo = 0;
}

CComputerList::~CComputerList(void)
{
	Clear();
}
//�����ն˺ź�IP��ɵ�����
/*
* ����m_ComputerArray���m_ComputerKeyMap��m_SortIndexArray 
* ���m_ComputerKeyMap��m_SortIndexArray
* m_ComputerKeyMap��Ӽ�ֵ��(��Ӧ��m_ComputerArray�������)����ֵΪstrTermID+strComputerIP��ֵΪ����m_ComputerArray�е�����
* m_SortIndexArray�������ֵ(��Ӧ��m_ComputerArray������� ) -- ��ʵ���Ǵ�1��ʼ������*/
void CComputerList::MakeComputerKeyMap()
{
	m_ComputerKeyMap.clear();
	m_SortIndexArray.RemoveAll();

	for (INT_PTR i = 0; i < m_ComputerArray.GetCount(); i++)
	{
		CComputerInfo& ComputerInfo = m_ComputerArray.GetAt(i);

		//���¹ؼ�������
		CString strKey;
		strKey.Format(_T("%s+%s"), ComputerInfo.GetTerminalID(), ComputerInfo.GetComputerIP());
		m_ComputerKeyMap.insert(std::make_pair(strKey, i));

		//����������������

		BOOL bInsert = FALSE;

		for (INT_PTR j = 0; j < m_SortIndexArray.GetCount(); j++)
		{
			CComputerInfo& ComputerInfo2 = m_ComputerArray.GetAt(m_SortIndexArray.GetAt(j));

			if (ComputerInfo2.GetTerminalID().CompareNoCase(ComputerInfo.GetTerminalID()) > 0)
			{
				m_SortIndexArray.InsertAt(j, i);
				bInsert = TRUE;
				break;
			}
		}

		if (!bInsert)
		{
			m_SortIndexArray.Add(i);
		}
	}
}
//��Ӽ���������Ϣ
/*************************************************
Function:       //AddComputer
Description:    //���computerInfo.���Ѵ�����ͬTermId+IP����,�������,�������.����m_UpdateIndexArray��m_ComputerKeyMap
Input:          //computer - �����computerInfo
Return:         //�����˸���,����false������ӣ�����true
*************************************************/
BOOL CComputerList::AddComputer(CComputerInfo& computer)
{
	INT_PTR nIndex = Find(computer.GetTerminalID(), computer.GetComputerIP());

	//���ն˺ź�IP�Ѿ�����,����computerInfo
	if (nIndex >= 0)
	{
		UpdateComputer(nIndex, computer);
		return FALSE;
	}

	nIndex = m_ComputerArray.Add(computer);
	
	m_UpdateIndexArray.Add(nIndex);

	//Ϊ�����Ч�ʣ�����ÿ�ζ������£��ȵ�һ�ֶ������Ժ�����Զ�����???
	if (m_ComputerKeyMap.size() > 0)
	{
		MakeComputerKeyMap();
	}

	return TRUE;
}
//�����ض�����������Ϣ
/*************************************************
Function:       //UpdateComputer
Description:    //����nIndexλ�õ�ComputerInfo,��m_UpdateStateΪEUS_CHANGE,PCClassID���ֲ���.���nIndex��m_UpdateIndexArray.
Input:          //nIndex - �����õĳ�Ա����
                //computer - ������Դ
Return:         //������Ч����false.
*************************************************/
BOOL CComputerList::UpdateComputer(INT_PTR nIndex, CComputerInfo& computer)
{
	if (IsInvaildIndex(nIndex))
	{
		return FALSE;
	}
	
	computer.SetUpdateState(CComputerInfo::EUS_CHANGE);
	computer.SetPCClassID(m_ComputerArray.GetAt(nIndex).GetPCClassID()); //PCClassID���ֲ���
	m_ComputerArray.SetAt(nIndex, computer); //����nIndexλ��ComputerInfo
		
	m_UpdateIndexArray.Add(nIndex);

	return TRUE;
}
/*************************************************
Function:       //Find
Description:    //��TermId+IP�����ն�,��������
Input:          //CString strTermID - �ն˺�
				//CString strComputerIP - �ն�IP
Return:         //��Ӧ�ն���m_ComputerKeyMap������
other:			//��m_ComputerKeyMap�в���
*************************************************/
INT_PTR CComputerList::Find(CString strTermID, CString strComputerIP)
{
	INT_PTR nRet = -1;

	CString strKey;

	strKey.Format(_T("%s+%s"), strTermID, strComputerIP);

	std::map<CString, INT_PTR>::iterator iter = m_ComputerKeyMap.find(strKey); 
	
	if (iter != m_ComputerKeyMap.end()) 
	{
		nRet = iter->second;
	} 

	return nRet;
}
/*************************************************
Function:       //Find
Description:    //��MemberID�����ն�,��������
Input:          //UINT nMemberID - ���������ݵ��û���
Return:         //��Ӧ�ն���m_ComputerArray������
*************************************************/
INT_PTR CComputerList::Find(UINT nMemberID)
{
	for (INT_PTR i = 0; i < m_ComputerArray.GetCount(); i++)
	{
		if (m_ComputerArray.GetAt(i).GetMemberID() == nMemberID)
		{
			return i;
		}
	}

	return -1;
}
//������Ϣ��0//�ն˺Ų��ң�1//�����˺Ų��ң�2//IP����
INT_PTR CComputerList::LookFor(UINT nKeyType, LPCTSTR lpszKeyValue)
{
	for (INT_PTR i = 0; i < m_SortIndexArray.GetCount(); i++)
	{
		CComputerInfo& ComputerInfo = m_ComputerArray.GetAt(m_SortIndexArray.GetAt(i));
		
		switch (nKeyType)
		{
		case 0: //�ն˺Ų���
			//ʵ������һ��ģ��ƥ�䣬ֻҪ�ִ����д��ֶε��׸��Ӵ�������Ϊ�ǡ�
			if (ComputerInfo.GetTerminalID().MakeUpper().Find(CString(lpszKeyValue).MakeUpper()) >= 0)
			{
				return m_SortIndexArray.GetAt(i);
			}

			break;

		case 1: //�����˺Ų���

			if (ComputerInfo.GetNetId().MakeUpper().Find(CString(lpszKeyValue).MakeUpper()) >= 0)
			{
				return m_SortIndexArray.GetAt(i);
			}

			break;

		case 2:  //IP����

			CString strTmp(ComputerInfo.GetComputerIP());
			
			if (strTmp.GetLength() > lstrlen(lpszKeyValue))
			{
				strTmp = ComputerInfo.GetComputerIP().Right(lstrlen(lpszKeyValue));
			}

			if (strTmp.CompareNoCase(lpszKeyValue) == 0)
			{
				return m_SortIndexArray.GetAt(i);
			}

			break;
		
		}
	}

	return -1;
}
//���
void CComputerList::Clear()
{
	m_ComputerArray.RemoveAll();
	m_UpdateIndexArray.RemoveAll();
	m_SortIndexArray.RemoveAll();
	m_ComputerKeyMap.clear();
}
//�������Ŀ
INT_PTR CComputerList::GetCount()
{
	return m_ComputerArray.GetCount();
}
//�õ�������ŵ��ն���Ϣ
CComputerInfo& CComputerList::GetComputer(INT_PTR nIndex)
{
	IsInvaildIndex(nIndex);

	return m_ComputerArray.GetAt(nIndex);
}

BOOL CComputerList::GetComputerInfo(UINT nMemberID, CComputerInfo& cInfo)
{
	INT_PTR iP = Find(nMemberID);
	if ( iP < 0 )
		return FALSE;

	cInfo = GetComputer(iP);
	return TRUE;
}

BOOL CComputerList::GetComputerInfo(CString strTermID, CComputerInfo& outInfo)
{
	for (int i=0; i < m_ComputerArray.GetCount(); i++)
	{
		CComputerInfo & info = m_ComputerArray.GetAt(i);
		if (info.GetTerminalID() == strTermID)
		{
			outInfo = info;
			return TRUE;
		}
	}
	return FALSE;
}
//��ǰ���µĵ�һ����¼
INT_PTR CComputerList::GetUpdateComputer()
{
	INT_PTR nIndex = -1;

	if (!m_UpdateIndexArray.IsEmpty())
	{
		nIndex = m_UpdateIndexArray.GetAt(0);
		m_UpdateIndexArray.RemoveAt(0);
	}
	
	return nIndex;
}
//��֤ȡ�ñ�ŵ���Ч��
BOOL CComputerList::IsInvaildIndex(INT_PTR nIndex)
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < m_ComputerArray.GetCount());

	if (nIndex < 0 || nIndex >= m_ComputerArray.GetCount())
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CComputerList::GetStatInfo(CStatInfo & StatInfo)
{
	ASSERT(StatInfo.m_nOnline==0);

	StatInfo.m_nAllCount = m_ComputerArray.GetCount();

	//2015-0824 liyajun ��������ն��������Ϣ
	//�����������PCClassID
	StatInfo.m_PCClassInfoArray.RemoveAll();
	for(INT_PTR i=0; i < CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetCount();++i)
	{
		PCClassInfo ClassInfo;
		ClassInfo.nAvailableDevices = 0;
		ClassInfo.nOccupiedDevices = 0;
		ClassInfo.nPCClassID = CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetAt(i).GetID();
		ClassInfo.nTotalDevices = 0;
		ClassInfo.strPCClassName = CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetAt(i).GetName();
		StatInfo.m_PCClassInfoArray.Add(ClassInfo);		
	}

	////Temp
	////std::map<CString,UINT> PCClassMap =  CLocalServer::GetInstance()->GetPCClassMap();
	std::map<CString,UINT> PCClassMap =  CNetBarConfig::GetInstance()->m_TermIDPCClassMap;
	//for(std::map<CString,UINT>::iterator iter = PCClassMap.begin(); iter != PCClassMap.end(); ++iter)
	//{
	//	TRACE(_T("CommandProcess::m_PCClassMap::first:%s,second:%d\n"),iter->first,iter->second);
	//}

	//ͳ�Ƹ�������ն�����
	for(INT_PTR i = 0; i < StatInfo.m_PCClassInfoArray.GetCount(); ++i)
	{
		UINT nCount = 0;
		for(std::map<CString,UINT>::iterator iter = PCClassMap.begin(); iter != PCClassMap.end(); ++iter)
		{
			if(StatInfo.m_PCClassInfoArray.GetAt(i).nPCClassID == iter->second)
			{
				nCount++;
			}
		}
		StatInfo.m_PCClassInfoArray.GetAt(i).nTotalDevices = nCount;
		StatInfo.m_PCClassInfoArray.GetAt(i).nAvailableDevices = nCount;
		StatInfo.m_PCClassInfoArray.GetAt(i).strPCClassName = CNetBarConfig::GetInstance()->GetPCClassName(StatInfo.m_PCClassInfoArray.GetAt(i).nPCClassID);
	}


	for (INT_PTR i = 0; i < m_ComputerArray.GetCount(); i++)
	{
		if (m_ComputerArray.GetAt(i).HasUser())//ֻͳ�����գ��ڻ���VIP���һ������࣬����״̬
		{
			//2015-0824 liyajun ͳ�Ƹ������ռ�õ��ն˺�δռ�õ��ն���
			for(INT_PTR j = 0; j< StatInfo.m_PCClassInfoArray.GetCount(); ++j)
			{
				if(m_ComputerArray.GetAt(i).GetPCClassID() == StatInfo.m_PCClassInfoArray.GetAt(j).nPCClassID)
				{
					++StatInfo.m_PCClassInfoArray.GetAt(j).nOccupiedDevices;
					--StatInfo.m_PCClassInfoArray.GetAt(j).nAvailableDevices;
				}
			}
			StatInfo.m_nOnline++;
			//////////////��ʱ���ʽ//////////////
			switch(m_ComputerArray.GetAt(i).GetPayType())
			{
			case CActiveMember::EPayType_General:
				{
					StatInfo.m_nCommonPayCount++;
					break;
				}
			case CActiveMember::EPayType_Capped:
			case CActiveMember::EPayType_Withholding:
				{
					StatInfo.m_nBundTimePayCount++;
					break;
				}
			}
			//////////////////////ͳ����ͨ���û�����Ա�û�///////////////////
			CString strUserClassName = m_ComputerArray.GetAt(i).GetMemberType();
			for ( INT k = 0; k < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); k++)
			{
				CString strTmp = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(k).GetClassName();
				//���ü���������ʶ��ͬ�ļ���
				if ( strTmp == strUserClassName )
				{	//�ж��Ƿ�Ϊ��Ա
					if (CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(k).GetIsMember())
					{
						StatInfo.m_nMemberUserCount++;
					} 
					else
					{
						StatInfo.m_nCommonUserCount++;
					}

					break;
				}
			}
			////////////////////�Ա�ͳ��///////////////////
			if (m_ComputerArray.GetAt(i).GetSex() == 0)
			{
				StatInfo.m_ManCount++;
			}
			else
			{
				StatInfo.m_WomanCount++;
			}
		}
		else if (m_ComputerArray.GetAt(i).GetComputerStatus() == CComputerInfo::ECS_LOCKED)
		{
			StatInfo.m_nLock++;
		}
		else if (m_ComputerArray.GetAt(i).GetComputerStatus() == CComputerInfo::ECS_UNLOCK)
		{
			StatInfo.m_nUnLock++;
		}
		else if (m_ComputerArray.GetAt(i).GetComputerStatus() == CComputerInfo::ECS_THIEF)
		{
			StatInfo.m_ThiefCount++;
		}
	}

	return TRUE;
}
//�������м������״̬
void CComputerList::UpdateAllComputerStatus()
{
	for (INT_PTR i = 0; i < m_ComputerArray.GetCount(); i++)
	{
		m_ComputerArray.GetAt(i).CheckStatus();
	}
}

void CComputerList::ClearComputerUserInfo(UINT nMemberId)
{
	// 2014-1-17 - qsc
	int nIndex = Find(nMemberId);

	if (nIndex >= 0) //Զ���»�
	{
		CComputerInfo& computer = GetComputer(nIndex);
		computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
		computer.ClearUserInfo();
		UpdateComputer(nIndex, computer);

		//CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
	}
}

std::string EncodeBase64( const char* Data,int DataByte)
{
	//�����
	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//����ֵ
	std::string strEncode;
	unsigned char Tmp[4]={0};
	for(int i=0;i<(int)(DataByte / 3);i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode+= EncodeTable[Tmp[1] >> 2];
		strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode+= EncodeTable[Tmp[3] & 0x3F];
	}
	//��ʣ�����ݽ��б���
	int Mod=DataByte % 3;
	if(Mod==1)
	{
		Tmp[1] = *Data++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode+= "==";
	}
	else if(Mod==2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode+= "=";
	}

	return strEncode;
}

UINT CComputerList::WorkThread(LPVOID lpParam)
{
	CComputerList* pThis = (CComputerList*)lpParam;
	if (NULL == pThis) return 0;

	UINT uRet = 0;

	const DWORD dwInterval = 1000*3600;
	do 
	{
		//try
		//{
		while (TRUE) 
		{
			//CIBALog::GetInstance()->Write(_T("CComputerList::WorkThread, start report..."));
			uRet = pThis->Run();

			// In order to run per 1 hour
			Sleep(dwInterval);
		}
		//}
		//catch(...) 
		//{
		//	CIBALog::GetInstance()->Write(_T("CComputerList::WorkThread, report thread thow a exception."));
		//}
	}while (TRUE);

	return uRet; 
}

UINT CComputerList::Run()
{
	//2015-0916 liyajun �������ϱ�
	return 1;
	//// ���ó�Ա
	//m_Devices.RemoveAll();

	//	// ��ȡ����
	//CString strLocalIp = CNetBarConfig::GetInstance()->GetLocalServerIP();
	//INT nPos = strLocalIp.ReverseFind(_T('.'));
	//if (nPos < 5) return 0;
	//CString strLocalSeg = strLocalIp.Left(nPos+1);

	//CArray<CString, CString&> vtSegments;
	////GetNetSegments(vtSegments);
	//INT nCount = vtSegments.GetCount();
	//BOOL bLackLocal = TRUE;

	//if (bLackLocal)
	//	vtSegments.Add(strLocalSeg);


	//// ��ȡ���к�
	//m_nSerialNo = CIBAConfig::GetInstance()->GetLocalSerialNo();

	//// ��ʼɨ��
	//nCount = vtSegments.GetCount();
	//for (INT nSeg=0; nSeg<nCount; ++nSeg)
	//{
	//	CString strCheckIp;
	//	for (int i=0; i<255; ++i)
	//	{
	//		strCheckIp.Format(_T("%s%d"), vtSegments.GetAt(nSeg).GetBuffer(), i);
	//		if (0 == strCheckIp.CompareNoCase(strLocalIp))
	//			continue;

	//		EmptyARPBuffer(strCheckIp);

	//		SendARPDgram(strCheckIp);
	//	}
	//}

	//// �ȽϿ��Ƿ����Ѵ��ڵ�
	//INT_PTR nPCCount = m_ComputerArray.GetCount();
	//INT_PTR nDevices = m_Devices.GetCount();

	//INT_PTR nUnknown = 0;
	//INT_PTR nRepeat = 0;
	//for (INT_PTR i=0; i<nPCCount; ++i)
	//{
	//	BOOL bIShutdown = TRUE;
	//	CComputerInfo& info = m_ComputerArray.GetAt(i);
	//	CStringA strIp = CT2A(info.GetComputerIP().GetBuffer());
	//	for (INT_PTR j=0; j<nDevices; ++j)
	//	{
	//		IPMAC& ipcmac = m_Devices.GetAt(j);
	//		if (0 == strIp.CompareNoCase(ipcmac.szIp))
	//		{
	//			// ip ��ͬ
	//			ipcmac.bDerelict = TRUE;

	//			CString strMac = CA2T(ipcmac.szMac);
	//			info.SetMac(strMac);

	//			info.SetCheckTime(ipcmac.nTime);

	//			nRepeat += 1;

	//			// ���ܲ鵽�����ѹػ�
	//			bIShutdown = FALSE;
	//		}
	//		else
	//			bIShutdown = TRUE;
	//	}

	//	info.SetIsShutdown(bIShutdown);

	//	if (!bIShutdown && info.GetComputerStatus() == CComputerInfo::ECS_THIEF)
	//	{
	//		nUnknown += 1;
	//	}
	//}

	//// ����
	//nUnknown = nUnknown + nDevices - nRepeat;
	//if (ReportPCInfo(nUnknown))
	//	return 1;

	//return 0;
}

void CComputerList::StartReportPCInfo()
{
#ifndef _DEBUG
	// ���������߳�
	m_pThread = AfxBeginThread(WorkThread, this);
	if (NULL == m_pThread)
		CIBALog::GetInstance()->Write(_T("CComputerList::StartReportPCInfo, AfxBeginThread failed."));
#endif
}

void CComputerList::EmptyARPBuffer(CString strIP)
{
	CStringA strIPA = CT2A(strIP.GetBuffer());
	strIP.ReleaseBuffer();

	// ɾ��ָ��IP�ı���ARP����
	DWORD dwSize = 0;
	CHAR szTem = 0;
	DWORD dwDelIP = inet_addr(strIPA.GetBuffer());
	strIPA.ReleaseBuffer();
	if (ERROR_INSUFFICIENT_BUFFER == GetIpNetTable((PMIB_IPNETTABLE)&szTem, &dwSize,TRUE))
	{
		CHAR* pBuf = new CHAR[dwSize];	
		ZeroMemory(pBuf, dwSize);
		//try
		//{
		if (ERROR_SUCCESS == GetIpNetTable((PMIB_IPNETTABLE)pBuf, &dwSize, TRUE))
		{
			PMIB_IPNETTABLE pNetTable = (PMIB_IPNETTABLE)pBuf;
			for (UINT i = 0; i < pNetTable->dwNumEntries; i++)
			{
				if (dwDelIP == pNetTable->table[i].dwAddr)
				{
					MIB_IPNETROW EnterAddrRow;
					EnterAddrRow.dwAddr = pNetTable->table[i].dwAddr;
					EnterAddrRow.dwIndex = pNetTable->table[i].dwIndex;
					DeleteIpNetEntry(&EnterAddrRow);
				}
			}
		}
		//}
		//catch (...)
		//{
		//	CIBALog::GetInstance()->WriteFormat(_T("CComputerList::EmptyARPBuffer exception, IP:%s, Error:%d."),
		//		strIP, GetLastError());
		//}
		delete[] pBuf;
	}
}

BOOL CComputerList::SendARPDgram(CString strIP)
{
	CStringA strIPA = CT2A(strIP.GetBuffer());
	strIP.ReleaseBuffer();

	ULONG szMac[2] = {0};  
	ULONG uLen = 6;  

	if (NO_ERROR == SendARP(inet_addr(strIPA.GetBuffer()), 0, szMac, &uLen))
	{
		strIPA.ReleaseBuffer();

		IPMAC ipmac;
		
		strcpy(ipmac.szIp, strIPA.GetBuffer());  

		strIPA.ReleaseBuffer();

		sprintf(ipmac.szMac,"%02x-%02x-%02x-%02x-%02x-%02x",
			(unsigned char)szMac[0],
			(unsigned char)szMac[1],
			(unsigned char)szMac[2],
			(unsigned char)szMac[3],
			(unsigned char)szMac[4],
			(unsigned char)szMac[5]);

		ipmac.nTime = time(NULL);

		m_Devices.Add(ipmac);

		return TRUE;
	}

	return FALSE;
}

void CComputerList::GetNetSegments(CArray<CString, CString&>& vtSegments)
{
	vtSegments.RemoveAll();

	DWORD AdapterInfoSize = 0;   
	if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(0, &AdapterInfoSize)) 
	{
		PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[AdapterInfoSize];
		if (ERROR_SUCCESS == GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) 
		{
			size_t nNumber = -1;
			while (pAdapterInfo)
			{
				IP_ADDR_STRING* pIPAddrString = &pAdapterInfo->IpAddressList;
				while (pIPAddrString)
				{
					CStringA strIpA = pIPAddrString->IpAddress.String;
					CString strIp = CA2T(strIpA.GetBuffer());
					strIpA.ReleaseBuffer();

					BOOL bNeedAdd = TRUE;
					do 
					{
						nNumber = strIp.ReverseFind(_T('.'));
						if (nNumber < 5)
						{
							bNeedAdd = FALSE;
							break;
						}

						strIp = strIp.Left(nNumber+1);
						nNumber = vtSegments.GetCount();

						for (size_t i=0; i<nNumber; ++i)
						{
							if (0 == strIp.Compare(vtSegments.GetAt(i).GetBuffer()))
							{
								vtSegments.GetAt(i).ReleaseBuffer();
								bNeedAdd = FALSE;
								break;
							}
							vtSegments.GetAt(i).ReleaseBuffer();
						}
					} while (0);


					if (bNeedAdd)
						vtSegments.Add(strIp);

					pIPAddrString = pIPAddrString->Next;
				}

				pAdapterInfo = pAdapterInfo->Next;
			}
		}

		delete[] pAdapterInfo;
	}
}

CStringA CComputerList::GetHostName(CStringA strIp)
{
	CStringA strResult("");

	//try
	//{
	struct in_addr addr = { 0 };
	addr.s_addr = inet_addr(strIp.GetBuffer());
	strIp.ReleaseBuffer();
	HOSTENT* lpHostEnt = gethostbyaddr((char*)&addr, 4, AF_INET);
	if (lpHostEnt != NULL)
	{
		strResult = lpHostEnt->h_name;
	}
	else
	{
		char hostname[NI_MAXHOST] = {0};
		char servInfo[NI_MAXSERV] = {0};
		u_short port = 27015;

		// Set up sockaddr_in structure which is passed
		// to the getnameinfo function
		struct sockaddr_in saGNI;
		saGNI.sin_family = AF_INET;
		saGNI.sin_addr.s_addr = inet_addr(strIp.GetBuffer());
		strIp.ReleaseBuffer();
		saGNI.sin_port = htons(port);

		DWORD dwRetval = getnameinfo((struct sockaddr *)&saGNI,
			sizeof(struct sockaddr),
			hostname,
			NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

		if (dwRetval != 0) 
		{
			CIBALog::GetInstance()->WriteFormat(_T("CComputerList::GetHostName error, IP:%s, Error:%d."),
				CA2T(strIp.GetBuffer()), GetLastError());
			strIp.ReleaseBuffer();
		}
		else
		{
			strResult = hostname;
		}
	}
	//}
	//catch (...)
	//{
	//	CIBALog::GetInstance()->WriteFormat(_T("CComputerList::GetHostName exception, IP:%s, Error:%d."),
	//		CA2T(strIp.GetBuffer()), GetLastError());
	//	strIp.ReleaseBuffer();
	//}

	return strResult;
}

BOOL CComputerList::ReportPCInfo(INT_PTR nUnknownDevice)
{
	CComputerInfo* pInfo = NULL;
	CString strData(_T("")), strTemp(_T(""));
	INT_PTR nPCCount = m_ComputerArray.GetCount();
	INT_PTR nIndex = 0;
	for (INT_PTR i=0; i<nPCCount; ++i)
	{
		pInfo = &m_ComputerArray.GetAt(i);
		if (NULL == pInfo) continue;

		// �ػ��Ĳ��ϱ�
		if (pInfo->GetIsShutdown()) continue;
		// ���ǿ��ɵ��ԵĲ��ϱ�
		if (pInfo->GetComputerStatus() != CComputerInfo::ECS_THIEF) continue;

		nIndex += 1;

		// netbarid
		strTemp.Format(_T("%05d"), CNetBarConfig::GetInstance()->GetNetBarId());
		strData += strTemp;

		// netbarcaption
		strTemp = CNetBarConfig::GetInstance()->GetNetBarName();
		CIBAHelpper::FillString(strTemp, 50, _T(' '));
		strData += strTemp;

		// unknowncount
		strTemp.Format(_T("%06d"), nUnknownDevice);
		strData += strTemp;

		// machname
		strTemp = pInfo->GetTerminalID();
		CIBAHelpper::FillString(strTemp, 50, _T(' '));
		strData += strTemp;

		// machip
		strTemp = pInfo->GetComputerIP();
		CIBAHelpper::FillString(strTemp, 15, _T(' '));
		strData += strTemp;

		// machmac
		strTemp = pInfo->GetMac();
		CIBAHelpper::FillString(strTemp, 17, _T(' '));
		strData += strTemp;

		// machtype
		strTemp = _T("PC");
		CIBAHelpper::FillString(strTemp, 10, _T(' '));
		strData += strTemp;

		// onlineTime
		strTemp.Format(_T("%014ld"), pInfo->GetCheckTime());
		strData += strTemp;
		
		// offlinetime
		strTemp.Format(_T("%014ld"), 0);
		strData += strTemp;

		if (nIndex > 0 && 0 == nIndex % 5)
		{
			nIndex -= 5;
			
			strTemp.Format(_T("%03d%s"), 5, strData);
			strData = strTemp;

			DoReport(strData);
		}
	}

	INT_PTR nExists = nIndex;
	LPIPMAC pIpMac = NULL;
	INT_PTR nDeviceCount = m_Devices.GetCount();
	for (INT_PTR i=0; i<nDeviceCount; ++i)
	{
		pIpMac = &m_Devices.GetAt(i);
		if (NULL == pIpMac) continue;

		// �Ѿ��ϱ����Ĳ��ٴ��ϱ�
		if (pIpMac->bDerelict) continue;

		nIndex += 1;

		// netbarid
		strTemp.Format(_T("%05d"), CNetBarConfig::GetInstance()->GetNetBarId());
		strData += strTemp;

		// netbarcaption
		strTemp = CNetBarConfig::GetInstance()->GetNetBarName();
		CIBAHelpper::FillString(strTemp, 50, _T(' '));
		strData += strTemp;

		// unknowncount
		strTemp.Format(_T("%06d"), nUnknownDevice);
		strData += strTemp;

		// machname
		strTemp = CA2T(GetHostName(CStringA(pIpMac->szIp)).GetBuffer());
		GetHostName(CStringA(pIpMac->szIp)).ReleaseBuffer();
		CIBAHelpper::FillString(strTemp, 50, _T(' '));
		strData += strTemp;

		// machip
		strTemp = CA2T(pIpMac->szIp);
		CIBAHelpper::FillString(strTemp, 15, _T(' '));
		strData += strTemp;

		// machmac
		strTemp = CA2T(pIpMac->szMac);
		CIBAHelpper::FillString(strTemp, 17, _T(' '));
		strData += strTemp;

		// machtype
		strTemp = _T("PC");
		CIBAHelpper::FillString(strTemp, 10, _T(' '));
		strData += strTemp;

		// onlineTime
		strTemp.Format(_T("%014ld"), pIpMac->nTime);
		strData += strTemp;

		// offlinetime
		strTemp.Format(_T("%014ld"), 0);
		strData += strTemp;

		if (nIndex > 0 && 0 == nIndex % 5)
		{
			nIndex -= 5;

			strTemp.Format(_T("%03d%s"), 5, strData);
			strData = strTemp;

			DoReport(strData);
		}
	}

	if (nIndex > 0)
	{
		strTemp.Format(_T("%03d%s"), nIndex, strData);
		strData = strTemp;

		DoReport(strData);
	}

	CIBALog::GetInstance()->Write(_T("CComputerList::ReportPCInfo finished."));

	return TRUE;
}

BOOL CComputerList::DoReport(CString& strData)
{
	//yajun 20150721 ����״̬���������Ƿ��ɴ˽ӿڵ��£�ֱ�ӷ���true
	/*
	CMachReportMethod MachReport;
	MachReport.SetData(strData);
	
	BOOL bRet = MachReport.DoReport();
	strData.Empty();

	if (!bRet)
	{
		CIBALog::GetInstance()->Write(_T("CComputerList::DoReport, communication with center failed.*****"));
		return FALSE;
	}

	if (0 == MachReport.GetStatusCode())
	{
		CIBALog::GetInstance()->WriteFormat(_T("CComputerList::DoReport, AddNum:%d, MergeNum:%d, ErrorNum:%d, DelNum:%d, OperateTime:%s"),
			MachReport.GetAddNum(), MachReport.GetMergeNum(), MachReport.GetErrorNum(), MachReport.GetDelNum(), MachReport.GetOperateTime().GetBuffer());
	}
	else
	{
		CIBALog::GetInstance()->WriteFormat(_T("CComputerList::DoReport, Status:%d.*****"), MachReport.GetStatusCode());
	}*/

	return TRUE;
}

//BOOL CComputerList::SendHttpData(CStringA strData, int nPCCount, int nRetryTimes /* = 5 */)
//{
//	CString strRequestIp = _T("");
//#if _DEBUG
//	strRequestIp = _T("192.168.10.253");
//#else
//	strRequestIp = CNetBarConfig::GetInstance()->GetMainCenterIP();
//#endif
//	SHORT nRequestPort = 8080;
//	CString strRequestPath = _T("/netbar/servlet/CommonRequest?command=NonSysDetail&param=");
//	CString strRequestData = CA2T(EncodeBase64(strData.GetBuffer(), strData.GetLength()).c_str());
//
//	CString strUrl(_T(""));
//	strUrl.Format(_T("http://%s:%d%s%s"),
//		strRequestIp.GetBuffer(),
//		nRequestPort,
//		strRequestPath.GetBuffer(),
//		strRequestData.GetBuffer());
//
//	static const LONG CodePage_GB2312 = 936;
//	static const LONG WinHttpRequestOption_URLCodePage = 2;
//
//	int nTempRetryTimes = nRetryTimes;
//	BOOL bOk = TRUE;
//	do 
//	{
//		CWinHttpRequest HttpRequest(FALSE); 
//		try
//		{
//			HttpRequest.Create();
//			HttpRequest.SetOption(WinHttpRequestOption_URLCodePage, COleVariant(CodePage_GB2312));
//			HttpRequest.SetTimeouts(5000, 5000, 1000 * 60, 1000 * 60);
//			HttpRequest.Open(_T("POST"), strUrl.GetBuffer());
//			HttpRequest.Send();
//
//			if (200 == HttpRequest.GetStatus())
//			{
//				CString strResp = HttpRequest.GetResponseText();
//				GxxDictionaryPtr respData = GxxDictionary::create();
//				respData->initWithJsonText(CT2A(strResp.GetBuffer()));
//
//				if (respData->isKeyExist("addNum") && 
//					respData->isKeyExist("mergeNum") &&
//					respData->isKeyExist("errorNum"))
//				{
//					INT_PTR nSucNum = respData->valueForKey("addNum")->intValue();
//					INT_PTR nMergeNum = respData->valueForKey("mergeNum")->intValue();
//					INT_PTR nErrorNum = respData->valueForKey("errorNum")->intValue();
//
//					CIBALog::GetInstance()->WriteFormat(_T("CComputerList::SendHttpData, count:%d, addNum:%d, mergeNum:%d, errorNum:%d"),
//						nPCCount, nSucNum, nMergeNum, nErrorNum);
//
//					if (nSucNum + nMergeNum == nPCCount && 0 == nErrorNum)
//						return TRUE;
//				}
//				else
//				{
//					CIBALog::GetInstance()->WriteFormat(_T("CComputerList::SendHttpData, response data:%s"), strResp.GetBuffer());
//				}
//
//				bOk = TRUE;
//			}
//			else
//			{
//				if (bOk)
//				{
//					CIBALog::GetInstance()->WriteFormat(_T("CComputerList::SendHttpData failed, status:%d."), HttpRequest.GetStatus());
//					bOk = FALSE;
//				}
//				else
//				{
//					CIBALog::GetInstance()->WriteFormat(_T("CComputerList::SendHttpData retry report failed, status:%d."), HttpRequest.GetStatus());
//				}
//			}
//		}
//		catch (...)
//		{
//			if (bOk)
//			{
//				CIBALog::GetInstance()->Write(_T("CComputerList::SendHttpData throw exception!"));
//				bOk = FALSE;
//			}
//			else
//			{
//				CIBALog::GetInstance()->Write(_T("CComputerList::SendHttpData retry report throw exception!"));
//			}
//		}
//	} while (--nTempRetryTimes > 0 && !bOk);
//	
//
//	return FALSE;
//}