/*
	���ڽ����������Excel��ʹ��
	����������ױ����������߳������
	�����������̱�����ʱ������ͨ�̱߳��������ᵼ�������̱߳���
*/
#pragma once

class CExportOutputListThread :  public CThread
{
public:
	CExportOutputListThread();
	virtual ~CExportOutputListThread();
protected:
	virtual int Run();
	void ExportExcel();
	// ����Excel���ַ���
	CString m_strExportExcel;
	// Excel�ļ��洢��ȫ·��
	CString m_strFullPath;
	UINT m_nRowCount;
	UINT m_nColCount;
	
public:
	void SetParam(LPCTSTR szExportExcel, LPCTSTR szFullPath, UINT nRowCount, UINT nColCount);
};