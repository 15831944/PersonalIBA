#pragma once

class CCaptureVideo;
class CIBACaptureImage
{
public:
	CIBACaptureImage(void);
	~CIBACaptureImage(void);

	bool Init(HWND hStaticShow, int nIndex = -1);

	// ����ͼ��ı���·��
	void SetImagesPath(LPCTSTR lpszPath);
	
	// ����һ����Ƭ
	bool CaptureImage(LPCTSTR lpszName);
	bool GetCapArray(CArray<CString, CString> &arCapList);
	void Stop();
private:
	CCaptureVideo* m_pCap;
	CString m_strBmpFilePath1;
	CString m_strBmpFilePath2;
public:
	int m_nDeviceCount;
	bool m_bInitOK;
	CString m_strPath; 
};
