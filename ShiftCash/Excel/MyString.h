//
// Author:guoxuxing
// CreateTime:2009/9/1
// Lastdate:2010/4/26
#pragma once
//
//ʹ��������ʽ�ķ��������붨�� __GXX_REGX
//
//#define __GXX_REGX

#include <vector>
#include <string>

#include <atlstr.h>
#ifndef ASSERT
#define ASSERT(x)
#endif
namespace NS_GXX
{

//
//
//---------------------����˵��---------------------
/*
���캯��--
	CMyStringW()                  Ĭ�Ϲ��캯�����ַ���Ϊ��
	CMyStringW(const long nNum)   ������ת��Ϊ�ַ�����ʮ���Ƶģ�
	CMyStringW(const unsigned long nNum) ������ת��Ϊ�ַ�����ʮ���Ƶģ�
	CMyStringW(const int nNum)
	CMyStringW(const unsigned int nNum)
	CMyStringW(const float fNum)
	CMyStringW(const double dNum)
	CMyStringW(CMyStringW& myStr)
	CMyStringW(CString& str)
	CMyStringW(const wchar_t *lpstr)
	CMyStringW( const char *lpsz )

��������--
	ReAllocate( size_t nNewMaxSize,BOOL bCopyOld = FALSE)  ���·��仺������С
	SetAppendAt(long iAt)             ��iAt�Ժ�(����iAt)���ַ�������
	AppendChar(WCHAR ch)              �ۼ�һ���ַ�
	Append(const wchar_t *lpszwStr)	  �ۼ�һ����\0���ַ���β���ַ���
	Append(CMyStringW& myStr)   �ۼ��ַ�������
	Append(CString& str)              �ۼ�һ��CString����
	AppendFormat(const wchar_t *lpszFormat,...)  �ۼ�һ����ʽ���ַ���
	Insert(int iAt ,IN const wchar_t *lpszwStr)  ��ָ��λ�ò����ַ���
	GetData()                         �õ������ַ���
	GetBuffer(int iAt = 0)			  �õ���ָ��λ�ô�������ַ���
	GetLength()                       �õ��ַ�������
	Format(IN const wchar_t* lpszFormat,...)   ��ʽ����������ԭ�е��ַ���
	MakeUpper()						  ת��Ϊ��д
	MakeLower()                       ת��ΪСд

��������--
	Left(int nCount)                  ����߽�ȡnCount���ȵ��ַ���
	Right(int nCount)                 ���ұ߽�ȡnCount���ȵ��ַ���
	Clip(int iFrom,int iTo)           ��ȡָ��������ַ���,��iFrom��ʼ����iTo������������iTo
	GetAt(int i)					  �õ�ָ��λ�ô����ַ�
	SetAt(int iAt,WCHAR ch)           ����ָ��λ�ô����ַ�
	Remove(int iAt,int nLen)          �Ƴ�һ���ַ�
	Remove(WCHAR ch)                  �Ƴ����е��ַ�ch
	FindFromLeft(const int iAt,const WCHAR ch)     �����ĳ��λ�ÿ�ʼ���ҳ��ֵĵ�һ��ch������λ��
	FindFromLeft(WCHAR ch)            ������߿�ʼ���ҵ�һ�����ֵ�ch���ڵ�λ��
	FindFromLeft(IN const WCHAR * pwsz,const int iFrom=0)    �����ָ��λ�ô����ҵ�һ������ pwsz�ַ���������λ��
	FindFromRight(IN const WCHAR * pwsz,const int iFrom )    ���ұ�ָ��λ�ô����ҵ�һ������ pwsz�ַ���������λ��
	FindString(IN const WCHAR * lpszStr,int iFrom = 0)       ���ұ�ĳ��λ�ÿ�ʼ���ҳ��ֵĵ�һ��ch������λ��
	FindFromRight(WCHAR ch)			  �����ұ߿�ʼ���ҵ�һ�����ֵ�ch���ڵ�λ��
	FindString(IN const WCHAR * lpszStr,int iFrom = 0)   ��ͬ��FindFromLeft(IN const WCHAR * pwsz,const int iFrom=0)
	Replace(const WCHAR chSrc,const WCHAR chNew)         �滻 ����chSrc
	TrimLeft(const WCHAR ch=L' ')                        �������������ߵ�ch
	TrimRight(const WCHAR ch=L' ')                       ������������ұߵ�ch
	TrimLeft(IN const WCHAR * pwsz)                      �������ߵ���pwszƥ����ַ���
	TrimRight(IN const WCHAR * pwsz)					 ������ұߵ���pwszƥ����ַ���
	IsEmpty()                                            �Ƿ�Ϊ�մ�
	Split(IN const WCHAR* lpszSplit,OUT CMyStringW*& pStrArray,OUT int &nCount )
														 ��lpszSplit�ִ����ָ��ַ������������飬��Ҫ������ͷ��ڴ�
	Split(IN const WCHAR ch,OUT CMyStringW*& pStrArray,OUT int &nCount )													 
														 ͨ��һ���ַ����ָ��ַ���
    Reverse()                  ���ַ�����ת
	SortUp()                   ���ַ�����������
	SortDown()                 ���ַ�����������

�������--
	MakeIPv4(BYTE b1,BYTE b2,BYTE b3,BYTE b4)           ����ΪIP��ַ���ַ���
	ParseIPv4(BYTE& b1,BYTE& b2,BYTE& b3,BYTE& b4)      ת��ΪIP��ַ
	IsBinNumString()									�ж��Ƿ�Ϊ �������ַ���
	IsHexNumString()                                    �Ƿ�Ϊ 16�����ַ���

*/
class CMyStringA;
//
// CMyStringW ��WCHAR���ַ��������в���
// 
class CMyStringW
{
	friend class CMyStringW;
public:
#pragma region //--------------���캯��-----------------------------
	

	CMyStringW();
	CMyStringW(const long nNum);
	CMyStringW(const unsigned long nNum);
	CMyStringW(const int nNum);
	CMyStringW(const unsigned int nNum);
	CMyStringW(const float fNum);
	CMyStringW(const double dNum);
	CMyStringW(const CMyStringW& myStr);
	CMyStringW(CString& str);
	CMyStringW(const wchar_t *lpsz);
	CMyStringW(const char *lpsz);
	~CMyStringW();


#pragma endregion
public:
#pragma region //--------------------------------��������------------
	//--------------------------------------------------
	// Method:  ReAllocate   [ public ]
	// Description: ���·����С,����һ���ϴ�Ŀռ䣬����Ч������ַ��������ۼӵ�Ч��
	// Returns [BOOL]: 
	// Parameters: 
	//    [ size_t nNewMaxSize ]:  �µ���󳤶�
	//    [ BOOL bCopyOld ]:       �Ƿ񿽱�ǰһ�η���������ַ���
	// Author:  Guoxuxing   
	// Modify-Date:2009/10/22
	//************************************
	BOOL ReAllocate( size_t nNewMaxSize,BOOL bCopyOld = FALSE);
	//
	// ���ô�ָ��λ���ۼ��ַ���,iAt����������ַ������ض�
	BOOL SetAppendAt(long iAt);
	//
	//�ۼ��ַ�
	BOOL AppendChar(WCHAR ch);
	BOOL AppendChar(char ch);
	BOOL Append(const char * lpsz);
	BOOL Append(const wchar_t *lpszwStr);
	BOOL Append(CMyStringW& myStr);
	BOOL Append(CString& str);
	BOOL Append(const int num);
	BOOL Append(const unsigned int num);
	BOOL Append(const long num);
	BOOL Append(const unsigned long num);
	
	/// ���ַ�����ʽ�����ۼӵ�ԭ�ַ���ĩβ
	BOOL AppendFormat(const wchar_t *lpszFormat,...);
	/// ���ַ�����ʽ�����ۼӵ�ԭ�ַ���ĩβ
	BOOL AppendFormat(const char *lpszFormat,...);

	/// ��ָ��λ�ò����ַ���
	BOOL Insert(int iAt ,IN const wchar_t *lpszwStr);
	/// ��ָ��λ�ò����ַ���
	BOOL Insert(int iAt ,IN const char *lpszwStr);
	//
	// �õ������ַ���
	wchar_t *GetData() const;
	//
	// ��ȡ�ַ�������,(���WCHAR)
	long GetLength() const;
	//
	/*��ȡָ��λ�ô����ַ���*/
	wchar_t *GetBuffer(int iAt = 0);
	//��ʽ��
	void Format(IN const wchar_t* lpszFormat,...);
	void Format(IN const char* lpszFormat,...);

	/// ת��Ϊ��д 
	CMyStringW& MakeUpper();	
	/// ת��ΪСд 
	CMyStringW& MakeLower();

	///�������ݵ����飬������Ч�ַ�����
	int CopyTo(IN char * lpChArray,IN int nCount);
	//
	// �������ݵ����飬������Ч�ַ�����
	int CopyTo(IN WCHAR * lpWChArray,IN int nCount);

#pragma endregion 
#pragma region //------------------------��������------------------------

	CMyStringW Left(int nCount);
	CMyStringW Right(int nCount);
	CMyStringW Clip(int iFrom,int iTo);      // ��ȡ��iFrom <= ���� < iTo;
	WCHAR GetAt(int iAt);
	void SetAt(int iAt,WCHAR ch);
	//
	// �Ƴ�����iAt֮�󣨰���iAt���������ַ�ch
	void Remove(int iAt,int nLen);
	//
	// �Ƴ����е��ַ�ch,�����Ƴ��˵ĸ���
	int Remove(WCHAR ch);
	//
	// �Ƴ�iAtָ��λ��֮������� �ַ���lpwsz
	int Remove(IN const WCHAR* lpwsz /* != NULL */, const int iAt = 0);
	//
	// ����߲����ַ��������ַ�λ��,-1��Ч 
	int FindFromLeft(const int iAt,const WCHAR ch);
		/* ����߲����ַ��������ַ�λ��,-1��Ч */
	int FindFromLeft(const WCHAR ch);
		/* ����߲��ҵ�һ���ַ�����λ��,-1��Ч*/
	int FindFromLeft(IN const WCHAR * pwsz,const int iFrom=0);
	int FindFromRight(IN const WCHAR * pwsz,const int iFrom );
		/* ���ұ߲��ң�-1��Ч */
	int FindFromRight(int iAt,const WCHAR ch);
	int FindFromRight(const WCHAR ch);
		/* �����Ӵ���λ�� */
	int FindString(IN const WCHAR * lpszStr,int iFrom = 0);
	//
	// �ַ��滻,���chNew=L'\0',��ô��ɾ�����е��ַ�chSrc,�����滻�ĸ���
	int Replace(const WCHAR chSrc,const WCHAR chNew);
	//
	// �Ӵ��滻,���lpwsznew = L"",��ô��ɾ�����е��Ӵ��������滻�ĸ���
	int Replace(const WCHAR* lpwszSrc,const WCHAR* lpwszNew,int iFrom=0);

	//
	// ��lpwszNew�ַ���ȥ���ǵ� iAtλ�ô�����nLength���ַ���
	// ���lpwszNew����nLengthָ���ĳ��ȣ��Ͷ���lpwszNew��nLength������ַ�
	// ���lpwszNew����С�� nLength,�൱���滻
	// ���lpwszNew == L"",�������κθı�
	// bReplace ���� ��lpwszNew�ĳ���С��nLength,�Ƿ��滻nLength����������ַ���
	CMyStringW& Over(int iAt,int nLength,const WCHAR* lpwszNew,BOOL bReplace=FALSE);
	//
	// ���ͷ���������ո�(�������ַ�)
	CMyStringW& TrimLeft(const WCHAR ch=L' ');
	//
	// ���ͷ���������ո�(�������ַ�) 
	CMyStringW& TrimRight(const WCHAR ch=L' ');
	//
	// ���ͷ���������Ӵ� ���磺"abab�������",����TrimLeft(L"ab") ,���Ϊ:"�������"
	CMyStringW& TrimLeft(IN const WCHAR * pwsz);
	//
	// ���β���������Ӵ� ���磺"�������abab",����TrimRight(L"ab") ,���Ϊ:"�������"
	CMyStringW& TrimRight(IN const WCHAR * pwsz);
	//
	// ����β��,iAtΪβ�����ڵ�λ��
	CMyStringW& CutAway(int iAt);
	//
	// ����β��,��β���ҵ���һ�ַ�ch������ch��ʼ��(����ch)����������ַ�
	CMyStringW& CutAway(const WCHAR ch);
	//
	// �Ƿ�Ϊ�մ�
	BOOL IsEmpty();
	//
	// Method:  Split   [ public ]
	// Description: ��lpszSplit��Ϊ�ֽ��ַ������õ��ָ����ַ�������
	// Returns [void]: 
	// Parameters: 
	//    [ IN const WCHAR * lpszSplit ]: 
	//    [ OUT CMyStringW * & pStrArray ]: �����ַ�������,���ó�����Ҫɾ���ڴ�
	//    [ OUT int & nCount ]:     �ַ�������ĳ���
	//************************************
	void Split(IN const WCHAR* lpszSplit,OUT CMyStringW*& pStrArray,OUT int &nCount );
	//
	// �ο�"Split(IN const WCHAR* lpszSplit..."��ע��
	void Split(IN const WCHAR ch,OUT CMyStringW*& pStrArray,OUT int &nCount );
	//
	// �ַ�����ת
	CMyStringW& Reverse();
	//
	// �ַ�����������
	CMyStringW& SortUp();
	//
	// �ַ�����������
	CMyStringW& SortDown();
	//************************************
	// Method:    RemoveByWChars,����lpwszChars��ָ�����ַ��������Ƴ����߱����ַ�
	//             ����: CMyStringW str=L"abbbccccdddd"; str.RemoveByWChars(L"bd");  ���Ϊ: str=L"acccc".����ɾ�������е� 'b','d'
	//             ����: CMyStringW str=L"abbbccccdddd----"; str.RemoveByWChars(L"bd",FALSE);  ���Ϊ: str=L"bbbdddd".���Ǳ��������е� 'b','d'
	// FullName:  NS_GXX::CMyStringW::RemoveByWChars
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// [Parameter: IN const WCHAR * lpwszChars ] �ַ�����
	// [Parameter: IN const BOOL bRemove ] ָ���� �Ƴ����Ǳ���
	// [Parameter: IN const int iFrom ] ��ָ��λ�ô���ʼ����
	//************************************
	int RemoveByWChars(IN const WCHAR * lpwszChars,
		IN const BOOL bRemove = TRUE ,
		IN const int iFrom = 0);


	//�� lpwszStr�滻��,ƥ��pStrArray�������κ�һ���ַ���,
	// �����������һ���մ������ý���ʧ��
	//
	int ReplaceByWStrings(IN CMyStringW * pStrArray,IN const int nArrayCount ,IN const WCHAR *lpwszStr,IN const int iFrom = 0);
	int CountByWChar(IN const WCHAR ch,IN const int iFrom=0);
	int CountByWString(IN const WCHAR *lpwsz,IN const int iFrom=0);
	//
	//��Excel����ת��Ϊ��Ӧ���ַ���
	//1<=nCol<= 256
	CMyStringW& MakeExcelColumn(int nCol);
#pragma endregion 
#pragma region //----------------------�������-------------
	//
	// ����һ��IPv4�ַ���
	CMyStringW& MakeIPv4(BYTE b1,BYTE b2,BYTE b3,BYTE b4);

	//
	// �����ַ�����ģ���ȫ·��
	// hModΪNULL��ʾ��ǰģ��
	CMyStringW& MakeModuleFullPath(HMODULE hMod = NULL);
	//
	// �����ϵͳ��ʱĿ¼��õ��ַ���
	CMyStringW& MakeFromTempPath();
	//
	// ����Ӽ��������õ��ַ���
	CMyStringW& MakeFromComputerName();
	// 
	// ����һ���ֽ�����16�����ַ���,����this.����ʹ��ParseASCIICode������ת��
	CMyStringW& MakeHexByteStream(IN const BYTE * byteStream,IN const int nLen);

	//
	// �ж��Ƿ�Ϊ�������ַ��� 
	BOOL IsBinNumString();
	//
	// �ж��Ƿ�Ϊ������16�����ַ���
	BOOL IsHexNumString();
	// 
	// �ж��Ƿ�Ϊ������ʮ�������ַ���*/
	BOOL IsNumString();
	//
	// ��ʮ���������ַ���ת��Ϊ16���Ƶ������ַ�����������ַ���Ϊ�������ַ���������FALSE
	// �Ȱ��ַ���ת��Ϊ���֣���ת��Ϊ16������ʽ
	//
	BOOL ToHexStringFromNum();
	//
	// ��2���ƴ�ת��Ϊ16���ƴ� */
	BOOL ToHexStringFromBinString();
	BOOL ToBinStringFromHexString();
	//
	// ��ʮ�������ַ���ת��Ϊʮ�����ַ���
	//
	BOOL ToNumStringFromHexString();
	//
	// ��2����ת��Ϊʮ���Ƶ��ַ���
	//
	BOOL ToNumStringFromBinString();
	//
	// ��16���ƴ��������ֽ�ת��Ϊ��Ӧ��ASCII���ַ���,�õ��µģ�char���ַ�����
	BOOL ParseASCIICode(IN const int nSize,OUT char *chStream,OUT int& nLength);
	//
	// ��16���ƴ�����˫�ֽ�ת��Ϊ��Ӧ��Unicode���ַ���,�õ��µģ�WCHAR���ַ���
	BOOL ParseUnicodeCode(OUT CMyStringW& strOut);
	//
	// ����ַ���Ϊ"x.x.x.x",�Ϳ��Եõ���Ч��IP����
	void ParseIPv4(BYTE& b1,BYTE& b2,BYTE& b3,BYTE& b4);
	//
	// ת����Ϊ����,����ַ������������ܳ���Խ�������
	int ParseInt();
	//
	// ת����Ϊ����,����ַ������������ܳ���Խ�������
	UINT ParseUINT();
	//
	// ת����Ϊ����,����ַ������������ܳ���Խ�������
	long ParseLong();
	//
	// ת����Ϊ����,����ַ������������ܳ���Խ�������
	DWORD ParseDWORD();
	//
	// ת����Ϊ˫����,����ַ������������ܳ���Խ�������
	double ParseDouble();
	//
	// ת����Ϊ������,����ַ������������ܳ���Խ�������
	float ParseFloat();


#pragma endregion 
#pragma region //------------------------����������----------------------


	WCHAR& operator [](int iAt);
	void operator +=(const WCHAR ch);
	void operator +=(const wchar_t* lpszwStr);
	void operator +=(CString& str);
	void operator +=(CMyStringW& myStr);
	void operator +=(const int nNum);
	void operator +=(const unsigned int nNum);
	void operator +=(const long nNum);
	void operator +=(const unsigned long nNum);
	void operator =(CString& str);
	void operator =(const WCHAR ch);
	void operator =(const wchar_t* lpszwStr);
	void operator =(const int nNum);
	void operator =(const unsigned int nNum);
	void operator =(const long nNum);
	void operator =(const unsigned long nNum);	
	CMyStringW& operator =(const CMyStringW& myStr);
	operator wchar_t* ();


	friend CMyStringW operator+( CMyStringW& myStr,CString& str )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%s%s",myStr.GetData(),str.GetBuffer() );
		return strTemp;
	}
	friend CMyStringW operator+( CMyStringW& myStr,const wchar_t* lpszTxt )
	{
		CMyStringW strTemp;;
		strTemp.Append(myStr);
		strTemp.Append(lpszTxt);
		return strTemp;
	}

	friend CMyStringW operator+( CMyStringW& myStr1,CMyStringW& myStr2 )
	{
		CMyStringW strTemp;
		strTemp.Append(myStr1.GetData());
		strTemp.Append(myStr2.GetData());
		return strTemp;
	}

	friend CMyStringW operator+( CString& str ,CMyStringW& myStr )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%s%s",str.GetBuffer() ,myStr.GetData());
		return strTemp;
	}

	friend CMyStringW operator+( const wchar_t* lpszTxt,CMyStringW& myStr )
	{
		CMyStringW strTemp;
		strTemp.Append(lpszTxt);
		strTemp.Append(myStr);
		return strTemp;
	}

	friend CMyStringW operator+( const unsigned int num,CMyStringW& myStr )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%d%s",num,myStr.GetData());
		return strTemp;
	}

	friend CMyStringW operator+( const unsigned long num,CMyStringW& myStr )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%d%s",num,myStr.GetData());
		return strTemp;
	}

	friend CMyStringW operator+( CMyStringW& myStr,const int num )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%s%d",myStr.GetData(),num);
		return strTemp;
	}

	friend CMyStringW operator+( const long num,CMyStringW& myStr )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%d%s",num,myStr.GetData());
		return strTemp;
	}

	friend CMyStringW operator+( CMyStringW& myStr,const long num )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%s%d",myStr.GetData(),num);
		return strTemp;
	}

	friend CMyStringW operator+( const int num,CMyStringW& myStr )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%d%s",num,myStr.GetData());
		return strTemp;
	}

	friend CMyStringW operator+( CMyStringW& myStr,const unsigned int num )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%s%d",myStr.GetData(),num);
		return strTemp;
	}

	friend CMyStringW operator+( CMyStringW& myStr,const unsigned long num )
	{
		CMyStringW strTemp;
		strTemp.Format(L"%s%d",myStr.GetData(),num);
		return strTemp;
	}

	friend BOOL operator==( CMyStringW& myStr,const unsigned long num )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CMyStringW& myStr,const long num )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CMyStringW& myStr,const unsigned int num )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CMyStringW& myStr,const int num )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CMyStringW& myStr,CString& str )
	{
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CMyStringW& myStr,const WCHAR* str )
	{
		size_t nLen1 = myStr.GetLength();
		size_t nLen2 = wcslen(str);
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CMyStringW& myStr,CMyStringW& str )
	{
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( const unsigned long num,CMyStringW& myStr )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( const long num,CMyStringW& myStr )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( const unsigned int num,CMyStringW& myStr )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( const int num,CMyStringW& myStr )
	{
		CMyStringW str = num;
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( CString& str,CMyStringW& myStr )
	{
		int nLen1 = myStr.GetLength();
		int nLen2 = str.GetLength();
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	friend BOOL operator==( const WCHAR* str,CMyStringW& myStr )
	{
		size_t nLen1 = myStr.GetLength();
		size_t nLen2 = wcslen(str);
		if( nLen1 != nLen2 )
		{
			return FALSE;
		}
		for(int i = 0 ; i < nLen1 ; i ++ )
		{
			if( myStr[i] != str[i] )
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	
	friend BOOL operator >( const WCHAR* str,CMyStringW& myStr )
	{
		if( str == 0 )
		{
			return FALSE;
		}
		if( str[0] == L'\0')
		{
			return FALSE;
		}
		size_t nLen1 = wcslen(str);
		size_t nLen2 = wcslen(myStr);
		int nMin = nLen1 > nLen2 ?nLen2:nLen1;
		int i = 0;
		for(; i < nMin ; i ++ )
		{
			if( str[i] < myStr[i] )
			{
				return FALSE;  //��һ���ַ�С�ھͷ���FALSE
			}
			else if( str[i] > myStr[i] )
			{
				return TRUE;
			}
		}
		if( nLen1 > nLen2 )// <= nMin���ַ�����ȣ����жϳ����ǲ��Ǵ���nLen2
		{
			return TRUE;
		}
		return FALSE;  
	}
	friend BOOL operator <( const WCHAR* str,CMyStringW& myStr )
	{
		if( str == 0 )
		{
			return FALSE;
		}
		if( str[0] == L'\0')
		{
			return FALSE;
		}
		size_t nLen1 = wcslen(str);
		size_t nLen2 = wcslen(myStr);
		int nMin = nLen1 > nLen2 ?nLen2:nLen1;
		int i = 0;
		for(; i < nMin ; i ++ )
		{
			if( str[i] > myStr[i] )
			{
				return FALSE;  //��һ���ַ����ھͷ���FALSE
			}
			else if( str[i] < myStr[i] )
			{
				return TRUE;
			}
		}
		if( nLen1 < nLen2 )  // <= nMin���ַ�����ȣ����жϳ����ǲ���С��nLen2
		{
			return TRUE;
		}
		return FALSE;
	}
	friend BOOL operator >( CMyStringW& myStr,const WCHAR* str )
	{
		return str < myStr;
	}
	friend BOOL operator <( CMyStringW& myStr,const WCHAR* str )
	{
		return str > myStr;
	}
	friend BOOL operator >( CMyStringW& myStr1,CMyStringW& myStr2)
	{
		return (const WCHAR*)myStr1.GetData() > myStr2;
	}
	friend BOOL operator <( CMyStringW& myStr1,CMyStringW& myStr2)
	{
		return myStr2 > myStr1;
	}
	friend BOOL operator >(CMyStringW& myStr1,CString& str)
	{
		return (const WCHAR*)str.GetBuffer() < myStr1;
	}
	friend BOOL operator >(CString& str,CMyStringW& myStr1)
	{
		return (const WCHAR*)str.GetBuffer() > myStr1;
	}
	friend BOOL operator <(CMyStringW& myStr1,CString& str)
	{
		return (const WCHAR*)str.GetBuffer() > myStr1;
	}
	friend BOOL operator <(CString& str,CMyStringW& myStr1)
	{
		return (const WCHAR*)str.GetBuffer() < myStr1;
	}
#pragma endregion 
#pragma region //------------------------������ʽ���⴦��------------------------
	//ʹ��std::tr1��������ʽ

#ifdef __GXX_REGX

	//
	//lpszReg Ϊ���ʽ�ַ���
	//�����ȫƥ��Ļ�������TRUE ��or FALSE
	//
	BOOL RegexMatch(const WCHAR* lpszReg);	

	// ���������з��ϱ��ʽ���Ӵ�
	// return: �Ӵ��ĸ���
	int  RegexSearchAll(const wchar_t* lpszReg,std::vector<CMyStringW>& strResults);
	int  RegexSearchAll(const wchar_t* lpszReg,std::vector<std::wstring>& strResults);

	// ������һ������������ʽ���Ӵ���-1û���ҵ��� >=0�Ӵ�������λ��
	int  RegexSearch(const wchar_t* lpszReg,CMyStringW& strResult);
	// ��iFromָ����λ�ó���ʼ������һ������������ʽ���Ӵ�
	// -1û���ҵ��� >=0�Ӵ�������λ��
	int  RegexSearch(int iFrom,const wchar_t* lpszReg,CMyStringW& strResult);

	// �ָ��ַ���
	int  RegexSplit(const wchar_t* lpszRegx,std::vector<CMyStringW>& vecResults);
	int  RegexSplit(const wchar_t* lpszRegx,std::vector<std::wstring>& vecResults);

	// ��lpszFmt�滻lpszRegx���Ӵ�,����һ���µ��ַ���,�������µ��﷨�Է��ϱ��ʽ�����ݽ����ٴ�����
	//*  �ַ�����ʽ�﷨��
	//	*  $1: What matches the first capture group 
	//	*  $2: What matches the second capture group 
	//	*  $&: What matches the whole regular expression 
	//	*  $`: What appears before the whole regex 
	//	*  $': What appears after the whole regex 
	//	*  $$: $ 
	// eg:
	//   �滻���е�"ing" Ϊ "ed" ,RegexReplace(L"ing",L"ed");
	//   �����е�a,e,i,o,,u��ͷ�ĵ���ǰ������η�"a" �滻�� "an", RegexReplace(L"([\\s]a[\\s])([aeiou])",L" an $2")
	CMyStringW  RegexReplace(const wchar_t* lpszRegx,const wchar_t* lpszFmt);

	// ��lpszNewStr�ڷ��ϱ��ʽlpszRegx�Ӵ��� ����
	CMyStringW  RegexInsert(const wchar_t* lpszRegx,const wchar_t* lpszNewStr,bool bBefore=true);

	// 
	bool Regex_IsDouble(bool bSigned=true);

	//
	bool Regex_IsInt(bool bSigned = true);

#endif
#pragma endregion 
private:
	wchar_t *m_pData;
	size_t m_nMaxSize;
	long  m_iAppendAt;
};
//============================================================================
//
// CMyStringAû���ṩ��CMyStringW��ô��ķ���,������ת�޸�CMyStringW
class CMyStringA
{
public:
	CMyStringA();
	CMyStringA( const char* lpsz );
	CMyStringA( CMyStringW& str );
	~CMyStringA();
	char* SetData(const char* lpsz );
	char* SetData( CMyStringW& str);
	operator char*();
	char* GetData();
	int GetLength();
	int CopyTo( IN char * lpChArray,IN int nCount );
private:
	char *m_pData ;
	int m_nMaxSize;
	int m_iAppendAt;
};


}
