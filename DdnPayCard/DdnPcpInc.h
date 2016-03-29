
#pragma once

//---------------------------------------------------------------------
//- �㿨ƽ̨ V1.0,  2011-10-1 ��ǰ
//---------------------------------------------------------------------

void WINAPI CreatePayCardPlatformA(LPCSTR lpszParams);

void WINAPI CreatePayCardPlatformW(LPCWSTR lpszParams);


#ifdef UNICODE
#define CreatePayCardPlatform  CreatePayCardPlatformW
#else
#define CreatePayCardPlatform  CreatePayCardPlatformA
#endif // !UNICODE

void WINAPI DestroyPayCardPlatform();


//---------------------------------------------------------------------
//- �㿨ƽ̨ V1.1,  2011-11-18
//---------------------------------------------------------------------

/*��ʼ���㿨ƽ̨*/
BOOL WINAPI DdnInitializePcpA(LPCSTR lpszParams);
BOOL WINAPI DdnInitializePcpW(LPCWSTR lpszParams);


/*��ʾ�㿨����ҳ��*/
BOOL WINAPI DdnShowPcp();


/*�رյ㿨����ҳ��*/
BOOL WINAPI DdnHidePcp();


/*��ֹ�㿨ƽ̨*/
BOOL WINAPI DdnTerminatePcp();


#ifdef UNICODE
#define DdnInitializePcp DdnInitializePcpW
#else
#define DdnInitializePcp DdnInitializePcpA
#endif
