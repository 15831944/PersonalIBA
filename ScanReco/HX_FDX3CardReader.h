// HX_FDX3CardReader.h: interface for the CHX_FDX3CardReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HX_FDX3CARDREADER_H__F1179ED9_071F_44AB_8466_61E7AF269277__INCLUDED_)
#define AFX_HX_FDX3CARDREADER_H__F1179ED9_071F_44AB_8466_61E7AF269277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


 
//90 	�����ɹ� 	   
//91 	֤/���д��������� 	   
//9F 	Ѱ����֤/���ɹ� 	   
//01 	�˿ڴ�ʧ��/�˿���δ��/�˿ںŲ��Ϸ� 	   
//02 	PC���ճ�ʱ���ڹ涨��ʱ����δ���յ��涨���ȵ����ݡ� 	   
//03 	���ݴ������ 	   
//05 	��SAM_V���ڲ����ã�ֻ��SDT_GetCOMBaudʱ���п��ܷ��� 	   
//10 	����ҵ���ն����ݵ�У��ʹ� 	   
//11 	����ҵ���ն����ݵĳ��ȴ� 	   
//21 	����ҵ���ն˵�������󣬰��������еĸ�����ֵ���߼�������� 	   
//23 	ԽȨ���� 	   
//24 	�޷�ʶ��Ĵ��� 	   
//80 	Ѱ��֤/��ʧ�� 	   
//81 	ѡȡ֤/��ʧ�� 	 


//31 	֤/��֤��SAM_Vʧ�� 	   
//32 	SAM_V��֤֤/��ʧ�� 	   
//33 	��Ϣ��֤���� 	   
//40 	�޷�ʶ��Ŀ����� 	   
//41 	��֤/������ʧ�� 	   
//47 	ȡ�����ʧ�� 	   
//60 	SAM_V�Լ�ʧ�ܣ����ܽ������� 	   
//66 	SAM_Vû������Ȩ,�޷�ʹ�� 	 



typedef int (WINAPI FUN_SDT_OpenPort)( int iPort );
typedef int (WINAPI FUN_SDT_ClosePort)( int iPort );
typedef int (WINAPI FUN_SDT_StartFindIDCard )( int iPort , unsigned char * pucManaInfo, int iIfOpen );
typedef int (WINAPI FUN_SDT_SelectIDCard )( int iPort , unsigned char * pucManaMsg, int iIfOpen );
typedef int (WINAPI FUN_SDT_ReadBaseMsg )( int iPort, unsigned char * pucCHMsg, unsigned int * puiCHMsgLen, unsigned char * pucPHMsg, unsigned int * puiPHMsgLen, int iIfOpen);

typedef int (PASCAL FUN_GetBmp)(char * Wlt_File, int intf);




class CHX_FDX3CardReader  
{
public:
	void Close();
	BOOL OpenPort(int iPort = 1001);
	int ClosePort( int iPort = 1001);
	int StartFindIDCard ( int iPort , unsigned char * pucManaInfo, int iIfOpen );
	int SelectIDCard ( int iPort , unsigned char * pucManaMsg, int iIfOpen );
	int ReadBaseMsg ( int iPort, unsigned char * pucCHMsg, unsigned int * puiCHMsgLen, unsigned char * pucPHMsg, unsigned int * puiPHMsgLen, int iIfOpen);
	int GetBmp(char * Wlt_File, int intf = 2);
	char * GetNationStr(char *pCode = "01");

	
	CHX_FDX3CardReader();
	virtual ~CHX_FDX3CardReader();

private:
	BOOL m_bIsOpen;
	HINSTANCE m_hDll;
	
	FUN_SDT_OpenPort *lpfnOpenPort;
	FUN_SDT_ClosePort *lpfnClosePort;
	FUN_SDT_StartFindIDCard  *lpfnStartFindIDCard;
	FUN_SDT_SelectIDCard *lpfnSelectIDCard;
	FUN_SDT_ReadBaseMsg *lpfnReadBaseMsg;
	FUN_GetBmp *lpfnGetBmp;

public:
	typedef struct tag_TextInfo
	{
		//unsigned char ucUndefine[14];
		unsigned char ucName[30];
		unsigned char ucSex[2];
		unsigned char ucNation[4];
		unsigned char ucBirthDay[16];
		unsigned char ucAddress[70];
		unsigned char ucID[36];
		unsigned char ucIssue[30];
		unsigned char ucExpirStart[16];
		unsigned char ucExpirEnd[16];

	}TAG_TEXT_INFO;

};

#endif // !defined(AFX_HX_FDX3CARDREADER_H__F1179ED9_071F_44AB_8466_61E7AF269277__INCLUDED_)
