#pragma once

#ifdef DDN_CAPTURE_VIDEO
#define DDN_DLLAPI _declspec(dllexport)
#else
#define DDN_DLLAPI _declspec(dllimport)
#endif

class CCaptureVideo;

// lpDeviceName: �豸����
// pParam: ���ûص�����ʱ����Ĳ���
typedef void (*DDNEnumDevicesCallback)(const char* lpDeviceName, void* pParam);

//=========================CCapVideoInf������===============================
class DDN_DLLAPI CCapVideoInf
{
public:
	CCapVideoInf();
	~CCapVideoInf();
	
	int EnumDevices(DDNEnumDevicesCallback lpCallback, void* pParam); 

	// �����ʼ��
	// iDevice����Ҫʹ�õ���������
	// ��ֱ����EnumDevicesö�ٵ�˳��������ΪiDeviceID, (>=0)
	HRESULT Init(int iDeviceID,HWND hVideoWnd); 
	
	// ֹͣԤ��
	BOOL Stop();

	// ��ȡһ��jpgͼƬ������Ĳ�����Ҫ�������Ƭ���ļ���
	// ����Ҫ�����ļ���׺
	BOOL CapOneImage(const char* szFileName);

	// ���ý�ȡ���ͼƬ�ı���·��
	void SetBitmapSavePath( const char* szBitPath );

	void ResizeVideoPos();
		
	// �����Ƿ��
	BOOL IsOpened();

	// ����һ��ȫ·��������Ƿ����
	BOOL FileIsExist( const char* szFileName);

private:
	CCaptureVideo *m_pCapVideo;
};
