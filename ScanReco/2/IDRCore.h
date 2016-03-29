
#ifndef _IDRCore_H_
#define _IDRCore_H_

/* ���֤ʶ�� */
struct HY_RECT_S
{
	long lLeft;
	long lRight;
	long lTop;
	long lBottom;
};
struct ID_CARD_S
{
	char szText[10][256];
	HY_RECT_S idRect[10];
};
/* ����ʶ�� */
struct JL_ITEM
{
	char pzTxt[256];
	long left;
	long top;
	long right;
	long bottom;
};
/* ����֤������Ϣ */
struct TERMB_ITEM
{
	char pzTxt[70][128];
};

typedef long (WINAPI Fun_Device_Open)(void);
typedef void (WINAPI Fun_Device_Close)(void);
typedef long (WINAPI Fun_Get_IdcPic)(int iCardType, char* szFileNameIn);
typedef long (WINAPI Fun_Get_MiscPic)(int iCardType, char* szFileNameIn);
typedef long (WINAPI Fun_Get_ColorPic)(int iCardType, char* szFileNameIn, char *szUnknow);
typedef long (WINAPI Fun_Get_IdcData)(int iCardType, char* szFileNameIn, char *szFullFileNameOut, char *szHeadFileNameOut, ID_CARD_S* pstOut);
typedef long (WINAPI Fun_Get_MiscData)(int iCardType, char* szFileNameIn, char* szFullFileNameOut, char* szHeadFileNameOut, JL_ITEM* pstOut);
typedef long (WINAPI Fun_Format_ErrMsg)(int iErrCodeIn, char** pErrMsgOut);
typedef long (WINAPI Fun_BmpToJpg)(char* szFileNameSrc, char* szFileNameDest);
typedef long (WINAPI Fun_Get_TermbData)(char *szFileNameIn, TERMB_ITEM* pstOut);

/*
extern "C"
{
	//���豸
	long __stdcall Device_Open(void);
	//�ر��豸
	void __stdcall Device_Close(void);
	//�������֤ͼƬ
	long __stdcall Get_IdcPic(int iCardType, char* szFileNameIn);
	//��������֤��ͼƬ
	long __stdcall Get_MiscPic(int iCardType, char* szFileNameIn);
	//�������֤��Ϣ
	long __stdcall Get_IdcData(int iCardType, char* szFileNameIn, char
			*szFullFileNameOut, char *szHeadFileNameOut, ID_CARD_S* pstOut);
	//��������֤����Ϣ
	long __stdcall Get_MiscData(int iCardType, char* szFileNameIn, char*
			szFullFileNameOut, char* szHeadFileNameOut, JL_ITEM* pstOut);
	//��������֤
	long __stdcall Get_TermbData(char *szFileNameIn, TERMB_ITEM* pstOut);
	//��ʾ���ô���
	long __stdcall Show_ConfigWindow(HANDLE *handle, int iCardType);
	//��ʾ������Ϣ
	long __stdcall Format_ErrMsg(int iErrCodeIn, char** pErrMsgOut);
	//Bmpת����Jpg
	long __stdcall BmpToJpg(char* szFileNameSrc, char* szFileNameDest);
}
*/

#endif