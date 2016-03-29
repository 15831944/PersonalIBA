// DlgCapture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgCapture.h"
#include ".\dlgcapture.h"
#include "IBAHelpper.h"


// CDlgCapture �Ի���

IMPLEMENT_DYNAMIC(CDlgCapture, CIBADialog)

CDlgCapture::CDlgCapture(LPCTSTR szPath, LPCTSTR lpszImageName, CWnd* pParent /*= NULL*/)
	: CIBADialog(CDlgCapture::IDD, pParent)
{

	m_strImageName = lpszImageName;

	//CIBAHelpper::FillSuitableString(m_strImageName, 10, _T('0'));

	EnableVisualManagerStyle(TRUE);
	m_bOK = FALSE;

	m_capImage.SetImagesPath(szPath);
}

CDlgCapture::~CDlgCapture()
{
}

void CDlgCapture::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	//Ҫ��ʾ��Ƭ�ٻָ� DDX_Control(pDX, IDC_STATIC_PIC, m_picShow);
	DDX_Control(pDX, IDC_COMBO_CAMERADEVICES, m_comboDevices);
}



BEGIN_MESSAGE_MAP(CDlgCapture, CIBADialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CHOOSECAMERA, OnBnClickedBtnChoosecamera)
END_MESSAGE_MAP()


// CDlgCapture ��Ϣ�������

BOOL CDlgCapture::OnInitDialog()
{
	CIBADialog::OnInitDialog();
	
	// 2015/5/14 tmelody �����豸��һ���ӳ٣����԰�ť���н���
	// {
	EnableControls(FALSE);
	SetTimer(1000, 10, NULL);
	// }

	return TRUE;  
}

void CDlgCapture::OnTimer(UINT nIDEvent)
{
	if (1000 == nIDEvent)
	{
		KillTimer(nIDEvent);

		{
			CArray<CString, CString> arCapList;
			m_capImage.GetCapArray(arCapList);

		for(int i = 0; i < arCapList.GetCount(); i++)
		{
			// 2014-7-23 - qsc ��AddString�ĳ�InsertString
			m_comboDevices.InsertString(-1, arCapList.GetAt(i));
		}
		UINT nCameraIndex = CIBAConfig::GetInstance()->GetCameraIndex();
		if(nCameraIndex > m_comboDevices.GetCount())
		{
			nCameraIndex = 0;
			CIBAConfig::GetInstance()->SetCameraIndex(nCameraIndex);
		}
		m_comboDevices.SetCurSel (nCameraIndex);

		if (!m_capImage.Init(GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd(), -1))
		{
			// 2014-11-26 - qsc
			++nCameraIndex;
			CIBAConfig::GetInstance()->SetCameraIndex(nCameraIndex);
			if(nCameraIndex > m_comboDevices.GetCount())
			{
				nCameraIndex = 0;
				CIBAConfig::GetInstance()->SetCameraIndex(nCameraIndex);
			}

				IBA_LOG0(_T("����ͷ��ʼ��ʧ��,û�ҵ����õ�����ͷ"));
				EndDialog(IDCANCEL);
			}
		}

		OnBnClickedBtnChoosecamera();
		
		EnableControls();

		UpdateData(FALSE);
	}
}

void CDlgCapture::OnBnClickedOk()
{
	long nRes = m_capImage.CaptureImage(m_strImageName);

	if (nRes == 1)
	{
		m_bOK = TRUE;

		CString str = CIBAHelpper::GetZcImgPath() + m_strImageName + _T(".jpg");
		//Ҫ��ʾ��Ƭ�ٻָ� m_picShow.ShowPic( CIBAHelpper::GetZcImgPath() + m_strImageName + _T(".jpg"));
		EndDialog(IDOK);
		
		/*if (!m_strImageName.IsEmpty())
		{
			theApp.AppendPhoto(m_strImageName);
		}*/
	}
	else if(nRes == -1)
	{
		// �豸��ʱ
	}
	else
	{
		m_bOK = FALSE;
		theApp.IBAMsgBox(LOAD_STRING(IDS_VEDIO_CAPTURE_FAILED));
	}
}

void CDlgCapture::OnBnClickedCancel()
{
	if (m_bOK)
	{
		// �����̣߳��ϴ���Ƭ�����ġ�

		

		EndDialog(IDOK);
		
	}
	else
	{
		EndDialog(IDCANCEL);
	}
}

void CDlgCapture::OnBnClickedBtnChoosecamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_comboDevices.GetCurSel();
	if(0 <= nIndex)
	{
		CIBAConfig::GetInstance()->SetCameraIndex(nIndex);
		m_capImage.Stop();
		if (!m_capImage.Init(GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd(), nIndex))
		{
			IBA_LOG0(_T("����ͷ��ʼ��ʧ��"));
			EndDialog(IDCANCEL);
		}
	}
	
}

void CDlgCapture::EnableControls(BOOL bEnable /* = TRUE */)
{
	GetDlgItem(IDC_COMBO_CAMERADEVICES)->EnableWindow(bEnable);
	GetDlgItem(IDOK)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CHOOSECAMERA)->EnableWindow(bEnable);
}
