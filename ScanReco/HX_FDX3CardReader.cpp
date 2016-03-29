// HX_FDX3CardReader.cpp: implementation of the CHX_FDX3CardReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HX_FDX3CardReader.h"


const char *g_cNation[][2] = 
{
"01", "��",
"02", "��",
"03", "��",
"04", "��",
"05", "��",
"06", "����",
"07", "����",
"08", "������",
"09", "��",
"10", "��",
"11", "����",
"12", "��",
"13", "�",
"14", "��ɽ",
"15", "����",
"16", "ˮ",
"17", "����",
"18", "����",
"19", "����",
"20", "�¶�����",
"21", "��",
"22", "���Ӷ�",
"23", "����",
"24", "Ǽ",
"25", "����",
"26", "����",
"27", "ë��",
"28", "����",
"29", "����",
"30", "����",
"31", "����",
"32", "������",
"33", "ŭ",
"34", "���α��",
"35", "����˹",
"36", "���¿�",
"37", "����",
"38", "����",
"39", "ԣ��",
"40", "��",
"41", "������",
"42", "����",
"43", "���״�",
"44", "����",
"45", "�Ű�",
"46", "���",
"47", "��ŵ",
"48", "�ɹ�",
"49", "��",
"50", "��",
"51", "ά���",
"52", "��",
"53", "��",
"54", "׳",
"55", "����",
"56", "����",
"57", "����",
"58", "���Ѫͳ",
0, 0,
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHX_FDX3CardReader::CHX_FDX3CardReader()
{
	m_hDll = NULL;
	m_bIsOpen = FALSE;

	lpfnOpenPort = NULL;
	lpfnClosePort = NULL;
	lpfnStartFindIDCard = NULL;
	lpfnSelectIDCard = NULL;
	lpfnReadBaseMsg = NULL;
	lpfnGetBmp = NULL;

}

CHX_FDX3CardReader::~CHX_FDX3CardReader()
{
	Close();
}

BOOL CHX_FDX3CardReader::OpenPort(int iPort)
{
	if(m_bIsOpen)
	{
		return FALSE;
	}

	m_hDll = LoadLibrary("SDTAPI.DLL");
	if(m_hDll == NULL)
	{
		return FALSE;
	}


	
	lpfnOpenPort = (FUN_SDT_OpenPort *)GetProcAddress(m_hDll, "SDT_OpenPort");
	lpfnClosePort = (FUN_SDT_ClosePort *)GetProcAddress(m_hDll, "SDT_ClosePort");
	lpfnStartFindIDCard = (FUN_SDT_StartFindIDCard  *)GetProcAddress(m_hDll, "SDT_StartFindIDCard");
	lpfnSelectIDCard = (FUN_SDT_SelectIDCard *)GetProcAddress(m_hDll, "SDT_SelectIDCard");
	lpfnReadBaseMsg = (FUN_SDT_ReadBaseMsg *)GetProcAddress(m_hDll, "SDT_ReadBaseMsg");

	if(	lpfnOpenPort == NULL ||
		lpfnClosePort == NULL ||
		lpfnStartFindIDCard == NULL ||
		lpfnSelectIDCard == NULL ||
		lpfnReadBaseMsg == NULL )
	{
		FreeLibrary(m_hDll);

		return FALSE;
	}

	m_bIsOpen = TRUE;

	if(lpfnOpenPort(iPort) != 0x90)
	{
		Close();
		m_bIsOpen = FALSE;

	}

	return TRUE;
}

int CHX_FDX3CardReader::ClosePort(int iPort)
{
	if(!m_bIsOpen)
	{
		return 0x01;
	}

	int iError = lpfnClosePort(iPort);
	if(iError != 0x90)
	{
	}
	Close();

	return iError;
}

void CHX_FDX3CardReader::Close()
{
	if(m_bIsOpen)
	{
		if(m_hDll != NULL)
		{
			FreeLibrary(m_hDll);
			
			m_hDll = NULL;
		}
		m_bIsOpen = FALSE;
		
		lpfnOpenPort = NULL;
		lpfnClosePort = NULL;
		lpfnStartFindIDCard = NULL;
		lpfnSelectIDCard = NULL;
		lpfnReadBaseMsg = NULL;
	}

}

int CHX_FDX3CardReader::StartFindIDCard ( int iPort , unsigned char * pucManaInfo, int iIfOpen )
{
	if(!m_bIsOpen)
	{
		return 0x01;
	}

	return lpfnStartFindIDCard(iPort, pucManaInfo, iIfOpen );
}

int CHX_FDX3CardReader::SelectIDCard ( int iPort , unsigned char * pucManaMsg, int iIfOpen )
{
	if(!m_bIsOpen)
	{
		return 0x01;
	}

	return lpfnSelectIDCard(iPort, pucManaMsg, iIfOpen );
}

int CHX_FDX3CardReader::ReadBaseMsg ( int iPort, unsigned char * pucCHMsg, unsigned int * puiCHMsgLen, unsigned char * pucPHMsg, unsigned int * puiPHMsgLen, int iIfOpen)
{
	if(!m_bIsOpen)
	{
		return 0x01;
	}

	return lpfnReadBaseMsg(iPort, pucCHMsg, puiCHMsgLen, pucPHMsg, puiPHMsgLen, iIfOpen);
}


char * CHX_FDX3CardReader::GetNationStr(char *pCode)
{
	if(IsBadReadPtr(pCode, 2) || strlen(pCode) < 2)
		return "����";

	for(int i = 0; g_cNation[i][0] != 0; i++)
	{
		if(strncmp(g_cNation[i][0], pCode, 2) == 0)
		{
			return (char*)g_cNation[i][1];
		}
	}

	return "����";
}

int CHX_FDX3CardReader::GetBmp(char *Wlt_File, int intf)
{
	if(IsBadCodePtr((FARPROC)lpfnGetBmp))
	{
		HINSTANCE hDll = LoadLibrary("WltRS.dll");
		if(hDll != NULL)
		{
			lpfnGetBmp = (FUN_GetBmp*)GetProcAddress(hDll, "GetBmp");
			if(!IsBadCodePtr((FARPROC)lpfnGetBmp))
			{
				return lpfnGetBmp(Wlt_File, intf);
			}
		}
	}
	else
	{
		return lpfnGetBmp(Wlt_File, intf);
	}

	return -5;
}
