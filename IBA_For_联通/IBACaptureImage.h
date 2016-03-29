#pragma once

#include "CaptureVideo//CapVideoInf.h"
class CIBACaptureImage
{
public:
	CIBACaptureImage(void);
	~CIBACaptureImage(void);

	bool Init(HWND hStaticShow);

	// ����ͼ��ı���·��
	void SetImagesPath(LPCTSTR lpszPath) { m_strPath = lpszPath; }
	
	// ����һ����Ƭ
	bool CaptureImage(LPCTSTR lpszName);

private:
	CCapVideoInf m_capVideo;

public:
	int m_nDeviceCount;
	bool m_bInitOK;
	CString m_strPath; 
};
