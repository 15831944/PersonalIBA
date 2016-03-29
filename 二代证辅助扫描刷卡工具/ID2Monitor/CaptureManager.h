#pragma once

struct CaptureItem
{
	CString strName;  // ϵͳ������
	BOOL bDialog;     // �Ƿ�Ϊ�Ի���
	CString strWindowName; // Ҫ����Ĵ��ڱ���
	CString strClassName;  // Ҫ����Ĵ���������
	UINT nCardId;          // �˺ŵĿؼ�ID
	UINT nNumberId;        // ֤������Ŀؼ�ID
	UINT nNameId;          // �����Ŀؼ�ID
};

struct CaptureItemEx
{
	CString strName;  // ϵͳ������
	BOOL bDialog;     // �Ƿ�Ϊ�Ի���
	CString strWindowName; // Ҫ����Ĵ��ڱ���
	CString strClassName;  // Ҫ����Ĵ���������
	UINT nCardId;          // �˺ŵĿؼ��ڽ����е�˳��
	UINT nNumberId;        // ֤������Ŀؼ��ڽ����е�˳��
	UINT nNameId;          // �����Ŀؼ��ڽ����е�˳��
};


class CCaptureManager
{
public:
	CCaptureManager(void);
	~CCaptureManager(void);

	void ReadFromConfig();

	CaptureItem& GetItem(int iIndex);

	int GetCount();

	vector<CaptureItem> m_items;
};
