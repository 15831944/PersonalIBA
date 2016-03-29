// DlgCapPhoto.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgCapPhoto.h"
#include ".\dlgcapphoto.h"
#include "CaptureVideo.h"
#include "MainFrm.h"
#include "CapImageFunc.h"
#include <gdiplus.h>
#include "IBAHelpper.h"
using namespace Gdiplus;
// CDlgCapPhoto �Ի���
extern CEvent g_eCapEvent;
#define UM_VERIFYRET WM_USER + 1
IMPLEMENT_DYNAMIC(CDlgCapPhoto, CDialog)
CDlgCapPhoto::CDlgCapPhoto(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCapPhoto::IDD, pParent)
	, m_strName(_T(""))
	, m_strID(_T(""))
{
	m_pCap = new CCaptureVideo;
}

CDlgCapPhoto::~CDlgCapPhoto()
{
	if(m_pCap)
	{
		delete m_pCap ;
	}
}

void CDlgCapPhoto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_static_Pic);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Control(pDX, IDC_COMBO_CAMERADEVICES, m_comboDevices);
}


BEGIN_MESSAGE_MAP(CDlgCapPhoto, CDialog)
	ON_BN_CLICKED(IDC_BTNCAPPHOTO, OnBnClickedBtncapphoto)
	ON_MESSAGE(UM_VERIFYRET, OnVerifyRet)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgCapPhoto ��Ϣ�������

BOOL CDlgCapPhoto::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitFilePath();
	InitCamera();
	CJuBao* pJubao = ((CMainFrame*)AfxGetMainWnd)->GetJubaoInf();
	if(pJubao)
	{	
		pJubao->SetJubaoDelegate(this);
	}
	else
	{
		IBA_LOG0(_T("��ȡ�۱�ָ��Ϊ�� Init"));
		ASSERT(FALSE);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgCapPhoto::InitFilePath()
{
	CString strPath;
	strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("JubaoImages"));
	CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
	m_strCapBmpPath		= strPath + _T("\\jubao.bmp");
	m_strCapBmpPathTmp	= strPath + _T("\\jubaotmp.bmp");
	m_strConvertJpgPath = strPath + _T("\\jubao.jpg");
	m_strJubaoRetPath	= strPath + _T("\\jubaoret.jpg");
}

void CDlgCapPhoto::OnBnClickedBtncapphoto()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	VerifyUI();
	UpdateData();
	m_threadVerify.Create(this, false);
}

void CDlgCapPhoto::VerifyUI()
{
	GetDlgItem(IDC_BTN_CAPPHOTO)->SetWindowText(_T("�۱���֤��"));
	GetDlgItem(IDC_BTN_CAPPHOTO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CHOOSECAMERA)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetReadOnly(TRUE);
	GetDlgItem(IDC_COMBO_CAMERADEVICES)->EnableWindow(FALSE);
}

void CDlgCapPhoto::NormalUI()
{
	GetDlgItem(IDC_BTN_CAPPHOTO)->SetWindowText(_T("���ղ���֤"));
	GetDlgItem(IDC_BTN_CAPPHOTO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CHOOSECAMERA)->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetReadOnly(FALSE);
	((CComboBox*)GetDlgItem(IDC_COMBO_CAMERADEVICES))->EnableWindow(TRUE);
}

LRESULT CDlgCapPhoto::OnVerifyRet(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == lParam)
	{
		AfxMessageBox(_T("��֤ʧ�ܣ�"));
		NormalUI();
	}
	else
	{
		OnOK();
	}
	return 0;
}
void CDlgCapPhoto::CapAPhoto()
{
	m_pCap->GrabOneFrame(TRUE);
}

BOOL CDlgCapPhoto::GetBase64CapPhotoBuf(std::string& stbuf)
{
	BYTE *pBuffer;
	int nSize;
	if(!GetCapPhotoBuf(pBuffer, &nSize))
	{
		return FALSE;
	}
	stbuf= CJuBao::Encode((char*)pBuffer, nSize);
	return TRUE;
}

BOOL CDlgCapPhoto::GetCapPhotoBuf(BYTE *&pBuffer, int *bufLen)
{
	g_eCapEvent.ResetEvent();
	CapAPhoto();
	if( WaitForSingleObject( g_eCapEvent, 5 * 1000 ) != WAIT_OBJECT_0 )
	{
		IBA_LOG0(_T("�豸��ʱ"));
		return FALSE;
	}
	

	BOOL bFlag = FALSE;
	try
	{
		GdiplusStartupInput gdiplusStartupInput; 
		ULONG_PTR gdiplusToken = NULL;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

		BMPScale( m_strCapBmpPathTmp, m_strCapBmpPath, 320, 240 );

		BMP2JPG( m_strCapBmpPathTmp, m_strConvertJpgPath );

		GdiplusShutdown(gdiplusToken);

		Sleep( 50 );
		CFile f;
		CFileStatus fStatus;
		if( f.Open( m_strConvertJpgPath, CFile::modeRead ) )
		{
			f.GetStatus( fStatus );
			pBuffer = new BYTE[(size_t)fStatus.m_size]; 
			f.Read( pBuffer, (UINT)fStatus.m_size );
			*bufLen = (int)fStatus.m_size;
			f.Close();
			bFlag = TRUE;
		}


	}
	catch (...)
	{
		bFlag = FALSE;
		IBA_LOG0(_T("��Ƭת����jpg���ִ���"));
	}
	return bFlag;
}

void CDlgCapPhoto::GetNameID( CString& strName, CString& strID )
{
	strName = m_strName; 
	strID = m_strID;
}

BOOL CDlgCapPhoto::InitCamera()
{
	CArray<CString, CString> arCapList;
	m_pCap->EnumDevices (arCapList);
	if(0 == arCapList.GetCount())
	{
		AfxMessageBox(_T("����ͷ��ʼ��ʧ�ܣ�"));
		return FALSE;
	}
	else
	{
		for(int i = 0; i < arCapList.GetCount(); i++)
		{
			m_comboDevices.AddString(arCapList.GetAt(i));
		}
	}
	UINT nCameraIndex = CIBAConfig::GetInstance()->GetCameraIndex();
	if(nCameraIndex > m_comboDevices.GetCount())
	{
		nCameraIndex = 0;
		CIBAConfig::GetInstance()->SetCameraIndex(nCameraIndex);
	}
	m_comboDevices.SetCurSel (nCameraIndex); 

	HWND hWnd	= m_static_Pic.GetSafeHwnd() ; 
	HRESULT hr	= m_pCap->Init(nCameraIndex,hWnd);

	m_pCap->SetSaveFilePath(m_strCapBmpPath);
	return TRUE;
}

void CDlgCapPhoto::DoCheckEnd( char* username,char* idcardno ,DWORD checkresult,char * transNumber,char * bitmap )
{
	/*����username����ʾ��֤�ӿڴ��������û����� 
	����idcardno����ʾ��֤�ӿڴ����������֤�ţ� 
	����checkresult����ʾ��֤�Ľ�� 
	����transNo����ʾ��ˮ�� */
	// ��ȡ������Ϣ 

	CString strName(username);
	CString strID(idcardno);
	CString strTransNum(transNumber);

	IBA_LOG0(strName); 
	IBA_LOG0(strID); 
	IBA_LOG0(strTransNum); 

	CString strRet("�����:");
	switch(checkresult) 
	{ 
		// 0����֤�ɹ���֤����Ч�� 
		// 1��һ�� 
		// 2��Ч֤�� 
		// 3������������ 
		// 4��������ʱ 
		// 5������Ƭ������ 
		// 6���и��˲��߳�δ��ֹ 
		case 0: 
			strRet += _T("һ��"); 
			break; 
		case 1: 
			strRet += _T("��һ��"); 
			break; 
		case 2: 
			strRet += _T("��Ч֤��"); 
			break; 
		case 3: 
			strRet += _T("������������"); 
			break; 
		case 4: 
			strRet += _T("��������ʱ"); 
			break; 
		case 5: 
			strRet += _T("������Ƭ������"); 
			break; 
		case 6: 
			strRet += _T("���и��˲��߳�δ��ֹ"); 
			break;
		default:
			strRet += _T("δ֪");
	} 
	IBA_LOG0(strRet);

	// ��ȡͼƬ 
	int OutByte=0; 
	std::string strTmpResult = CJuBao::Decode(bitmap,strlen(bitmap),OutByte); 
	int i,len = strTmpResult.length(); 

	BYTE *buf = new BYTE[len]; 

	for (i=0;i<len;++i) 
	{ 
		buf[i] = strTmpResult[i]; 
	} 

	FILE * pFile; 
	pFile = fopen ( CT2A(m_strJubaoRetPath) , "wb" ); 
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);} 
	fwrite(buf,1,len,pFile);

	PostMessage(UM_VERIFYRET, 0, (checkresult == 0) ? TRUE : FALSE);
}

void CDlgCapPhoto::didRun( CG2XThread* pThread )
{
	if(pThread)
	{
		CMainFrame* pMainWnd = ((CMainFrame*)theApp.GetMainWnd());
		CJuBao* pJubao = NULL;
		if(pMainWnd)
		{	
			pJubao = pMainWnd->GetJubaoInf();	
		}

		int nRet;
		if(pJubao)
		{
			char *pName = CT2A(m_strName);
			char *pID = CT2A(m_strID);
			std::string stPhoto;
			if(!GetBase64CapPhotoBuf(stPhoto))
			{
				IBA_LOG0(_T("��ȡBase64��Ƭʧ��"));
				nRet = FALSE;
			}

			if(0 != (nRet =  pJubao->VerifyUserInfo(pName, pID, stPhoto)))
			{
				IBA_LOG(_T("�۱���֤�ӿڷ���ʧ��:%d"), nRet);
				nRet = FALSE;
			}
			else
			{
				nRet = TRUE;
			}
		}
		else
		{
			IBA_LOG0(_T("��ȡ�۱�ָ��Ϊ��"));
			ASSERT(FALSE);
			nRet = FALSE;
		}
	}	
}



void CDlgCapPhoto::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pCap->Stop();
	__super::OnClose();
}

void CDlgCapPhoto::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pCap->Stop();
	__super::OnOK();
}
