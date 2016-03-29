#pragma once
#include "GxxString.h"
#include <vector>
#include <map>

typedef CGxxString<TCHAR> CMyString;

using namespace std;

struct StringItem
{
	UINT nID;
	CMyString str;
	CMyString strMark;
	BOOL bDlgItem;

	StringItem()
	{
		nID = 0;
		bDlgItem = FALSE;
	}
};
struct MarkName
{
	CMyString strMark;       // ID�ı�ǣ����磺IDS_STRING1
	CMyString strConstent;   // �ַ���������
	CMyString strDlgMark;
};
struct MarkID
{
	CMyString strMark;
	UINT nID;
};

class CFileAnalyzer
{
public:
	CFileAnalyzer(void);
	~CFileAnalyzer(void);

	BOOL LoadFile(const WCHAR* lpFileName, BOOL bOpenAlways=TRUE);

private:
	void AnalyzeStrings(const WCHAR* lpwResource, const WCHAR* lpwSplit=L"\\0\\0");

public:
	BOOL AddString(StringItem sitem);

	static void LoadFromSource(CString strRCFile, CString strResourceFille,vector<StringItem>& vecItems,BOOL bMenuBefore=TRUE);
	static void LoadFromGrcFile(CString strGrcFile, vector<StringItem>& vecItems);
	
protected:
	static void _AnalyzeDlg(CMyString& strOrg, vector<MarkName>& vecMarks);
	static void _AnalyzeMenu(CMyString& strOrg, vector<MarkName>& vecMarks);
	static void _AnalyzeStringTable(CMyString& strOrg, vector<MarkName>& vecMarks);
	static BOOL IntoFindMenu(CMyString& strBE,vector<MarkName>& vecMarks);

public:
	vector<StringItem> m_arrayStrings;
};
