// DlgIBAFastlane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgIBAFastlane.h"
#include ".\dlgibafastlane.h"
#include "MainFrm.h"
#include ".\RealName\IBARealName.h"
#include "ServletHellper.h"
#include "MemberInfoDlg.h"
#include "IBAView.h"
#include "Socket\LocalServer.h"
#include "NDReturnDlg.h"
#include "IBAHelpper.h"
#include "ComputerListView.h"
#include "ComputerInfo.h"
#include "ComputerList.h"
#include "CreditDlg.h"
#include "NDCreditDlg.h"
#include "Dal\IBADAL.h"
#include "RealName\RZXRealName.h"
#include "GlobalFunctions.h"
#include "LocalCenter\G2XCommonTool.h"
#include "Tts\IBATTS.h"

static CIBAString StringZengshoubao ;

// CDlgIBAFastlane �Ի���

IMPLEMENT_DYNAMIC(CDlgIBAFastlane, CBCGPDialog)
CDlgIBAFastlane::CDlgIBAFastlane(CWnd* pParent /*=NULL*/)
	: CStringLoaderDlg<CBCGPDialog>(CDlgIBAFastlane::IDD, pParent)
{
	m_bBugleActive = FALSE; 
	m_nTurnCount = 0;
	m_nIBAStatus = 0;
}

CDlgIBAFastlane::~CDlgIBAFastlane()
{
}

void CDlgIBAFastlane::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CONTENT, m_stcContent);
	DDX_Control(pDX, IDC_COMBO_FAST, m_combFast);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_2, m_static2);
	DDX_Control(pDX, IDC_PIC_STATUS, m_stcPicStatus);
}


BEGIN_MESSAGE_MAP(CDlgIBAFastlane, CBCGPDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_CBN_SELENDOK(IDC_COMBO_FAST, OnCbnSelendokComboFast)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_FAST, OnCbnSelchangeComboFast)
	ON_CBN_SELENDCANCEL(IDC_COMBO_FAST, OnCbnSelendcancelComboFast)
	ON_MESSAGE(WM_UPDATEROLLTEXT, OnUpdateRollText)
	ON_MESSAGE(WM_UPDATE_IBA_STATUS, OnUpdateIBAStatus)
	ON_STN_CLICKED(IDC_PIC_STATUS, OnStnClickedPicStatus)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDlgIBAFastlane ��Ϣ�������

BOOL CDlgIBAFastlane::OnInitDialog()
{
	//SetStringID(IDC_STATIC_2, IDS_ZSB_GUANGBO);
	//SetStringID(IDC_STATIC_1, IDS_BJTD);
	CStringLoaderDlg<CBCGPDialog>::OnInitDialog();
	StringZengshoubao.LoadString(IDS_ZSB_GOOD);

	GetDlgItem(IDC_PIC_BUGLE)->ShowWindow(SW_HIDE);

	m_static1.SetFont(&globalData.fontRegular);
	m_static2.SetFont(&globalData.fontRegular);

	m_stcContent.SetText(StringZengshoubao,_T('|'));
	m_stcContent.SetFont(&globalData.fontRegular);
	m_stcContent.StartRolling(5000);
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_1));
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_2));
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_3));
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_4));
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_5));
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_6));
	m_combFast.AddString(LOAD_STRING(IDS_BJTD_7));

	/*m_combFast.AddString(L"�����˺�(���ȴ��ڵ���6)              (�����ֵ�˿����)");
	m_combFast.AddString(L"�����˺�(���ȵ���4��5)                (ѡ����Ҫ�˺ź�����ֵ�˿����)");
	m_combFast.AddString(L"�ն˺�(����С��4������ĸ��ʼ)  (�ڻ����б���ѡ�������ҵ��ն�)");
	m_combFast.AddString(L"\\�ն˺�                                             (�ڻ����б���ѡ�������ҵ��ն�)");
	m_combFast.AddString(L"\\ \\IP                                                    (�ڻ����б���ѡ�и���IP�����ҵ����ն�)");
	m_combFast.AddString(L"?�����˺�                                         (�ڻ����б���ѡ�и��������˺������ҵ����ն�)");
	m_combFast.AddString(L"+�����˺� ���                               (��ָ���������˺ų�ֵ����λΪԪ)");*/
	StartBugleTurn(TRUE);
	
	m_nIBAStatus = CIBAGlobal::NetworkStatus;

	m_imgStatus.LoadFromOtherResource(AfxGetInstanceHandle(),IDR_PNG_STATE,_T("PNG"));

	//20160116-liyajun
	m_ToolTip.Create(this);
	m_ToolTip.SetColorBk(RGB(200, 200, 200), RGB(220, 220, 220), RGB(200, 200, 200));
	m_ToolTip.SetEffectBk(12);
	m_ToolTip.SetBorder(RGB(200, 200, 200));

	return TRUE;  
}

BOOL CDlgIBAFastlane::OnEraseBkgnd(CDC* pDC)
{
	CRect rcRect;
	GetClientRect( rcRect );

	CDC memDC;  // Paint���ڴ�DC
	CBitmap bmp;

	bmp.CreateCompatibleBitmap( pDC,rcRect.Width(),rcRect.Height() );
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(&bmp);

	CBCGPToolBar* pParent = (CBCGPToolBar*)GetParent();
	if( pParent )
	{
		
		WINDOWPLACEMENT wp;
		this->GetWindowPlacement(&wp);

		CRect rcParent;
		pParent->GetClientRect(rcParent);

		// ���Ʊ���
		CDC memParentDC;
		CBitmap bmpParent;
		bmpParent.CreateCompatibleBitmap(pDC,rcParent.Width(),rcParent.Height());
		memParentDC.CreateCompatibleDC(NULL);
		memParentDC.SelectObject(&bmpParent);

		CDC* pParentDC = pParent->GetDC();
		CBCGPVisualManager::GetInstance ()->OnFillBarBackground (&memParentDC, pParent,
			rcParent,rcParent);
		pParent->ReleaseDC(pParentDC);

		CRect rt = wp.rcNormalPosition;

		memDC.BitBlt(0,0,rcRect.Width(),rcRect.Height(),&memParentDC,wp.rcNormalPosition.left,wp.rcNormalPosition.top,
			SRCCOPY);

		pDC->BitBlt(0,0,rt.Width(),rt.Height(),&memDC,0,0,SRCCOPY);
	}

	CBitmap bmpImg;
	BOOL bRes = 0;
	if (m_bBugleActive)
	{
		bRes = bmpImg.LoadBitmap(IDB_BUGLE2);
	}
	else
	{
		bRes = bmpImg.LoadBitmap(IDB_BUGLE1);
	}
	if (bRes)
	{
		BITMAP bt;
		bmpImg.GetBitmap(&bt);

		CStatic *pStc = (CStatic*)GetDlgItem(IDC_PIC_BUGLE);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmpImg);

		CRect rc;
		pStc->GetClientRect(rc);
		WINDOWPLACEMENT wp;
		pStc->GetWindowPlacement(&wp);

		CRect rcClient;
		GetClientRect(rcClient);

		rc.left = wp.rcNormalPosition.left;
		rc.top = rcClient.top + (rcClient.Height()-bt.bmHeight)/2;
		rc.right = rc.left + bt.bmWidth;
		rc.bottom = rc.top + bt.bmHeight;

		pDC->TransparentBlt(rc.left,rc.top,rc.Width(),rc.Height(),&memDC,0,0,rc.Width(),rc.Height(),RGB(255,255,255));
	}
	else
	{
		IBA_ASSERT2(0,"����image ����");
	}

	{
		WINDOWPLACEMENT pm;
		m_stcPicStatus.GetWindowPlacement(&pm);

		int nWidth = 85;
		int nHeight = 21;
		int nStatus = m_nIBAStatus;

		int nX = pm.rcNormalPosition.left;
		int nY = pm.rcNormalPosition.top;
		int nImgX = nStatus * nWidth;
		int nImgY = 0;
		
		BOOL bRes = m_imgStatus.Draw(pDC->GetSafeHdc(), nX, nY, nWidth, nHeight, nImgX, nImgY, nWidth, nHeight);
		pDC->SetBkMode(TRANSPARENT);
		CFont* pOldFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
		CStringArray strArr;
		CIBAHelpper::SplitLine(LOAD_STRING(IDS_IBA_NET_STATUS),strArr);
		CRect rcTest(0,0,0,0);
		CString strText = strArr.GetAt(nStatus);
		pDC->DrawText(strText,rcTest,DT_CALCRECT);
		CRect rcTxt;
		rcTxt.left = nX;
		rcTxt.right = nX + nWidth;
		rcTxt.top = nY + (nHeight - rcTest.Height())/2;
		rcTxt.bottom = rcTxt.top + rcTest.Height();
		pDC->DrawText(strText,rcTxt,DT_CENTER);
		bRes = bRes;
		pDC->SelectObject(pOldFont);
	}
	//return CBCGPDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

HBRUSH CDlgIBAFastlane::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CDlgIBAFastlane::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	if ( IsWindow(m_stcContent.m_hWnd ) )
	{
		CRect rc;
		GetClientRect(rc);
		
		WINDOWPLACEMENT wp;
		m_stcPicStatus.GetWindowPlacement(&wp);
		
		CRect rcOld = wp.rcNormalPosition;
		CRect rcNew = rcOld;
		rcNew.right = rc.right - 3;
		rcNew.left = rcNew.right - rcOld.Width();
		m_stcPicStatus.MoveWindow(rcNew);
		
		GetDlgItem(IDC_PIC_BUGLE)->GetWindowPlacement(&wp);
		rcNew.right = rcNew.left - 5;
		rcNew.left = wp.rcNormalPosition.right + 3;
		
		m_stcContent.GetWindowPlacement(&wp);
		rcNew.top = wp.rcNormalPosition.top;
		rcNew.bottom = wp.rcNormalPosition.bottom;

		m_stcContent.MoveWindow(rcNew,TRUE);
	}
	
}

BOOL CDlgIBAFastlane::PreTranslateMessage(MSG* pMsg)
{
	//20160116-liyajun
	m_ToolTip.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN )
		{
			CWnd *pWnd = GetFocus();
			if( pWnd && pWnd->GetParent()->GetSafeHwnd()== m_combFast.GetSafeHwnd())
			{
				OnCbnSelendokComboFast();
				return TRUE;
			}
		}
	}

	return CBCGPDialog::PreTranslateMessage(pMsg);
}

/*************************************************
Function:         //OnCbnSelendokComboFast
Description:      //���ͨ������Ĵ���:
				  //�������:<2,���һ���ַ���0~9��a~z��A~Z,ֱ�ӷ���
				  //���ַ� - '*' - ���ڵ���,ģ��ˢ�����εĿ���:��Ҫ���������˺�
				  //		'\\' - ��ʾ�ն˺�,����ģʽ��,�����ն���ͼ,��ѡ�񵽸��ն�.
				  //		'\' - ��ʾ�ն�IP.�����ն���ͼ,��ѡ�񵽸��ն�.
				  //		'?' - Ϊ�����˺Ų����ն�,�����ն���ͼ,��ѡ�񵽸��ն�.
				  //		'+' - �����˺Ž��н���ֵ
				  //		others - ��ʾ�˺�,<=3λ��ʾ�ն˺�,3<n<6��ʾ�����˺���Ҫ����ƥ��,>=6��ʾȫ���˺�
*************************************************/
void CDlgIBAFastlane::OnCbnSelendokComboFast()
{
	CString strTmp;
	m_combFast.GetWindowText(strTmp);// ȡͨ���е��Ӵ�
	m_combFast.SetWindowText(_T(""));

	strTmp.Trim();

	int nInputLen = strTmp.GetLength(); //�ִ�����

	if (strTmp.GetLength() < 2) //����2���ַ����˳�
	{
		return;
	}

	CHAR tcLast = strTmp.GetAt(strTmp.GetLength()-1);   //��ȡ���һ���ַ�
	
	if ( !( (_T('0')<=tcLast&&tcLast<=_T('9')) ||       //��0~9��a~z��A~Z�Ķ���Ϊ����Ч��
		(_T('a')<=tcLast&&tcLast<=_T('z')) || 
		(_T('A')<=tcLast&&tcLast<=_T('Z')) ) 
		)
	{
		return;
	}

	BOOL bComputerView = theApp.GetIBAView()->IsCurrentComputerView();   //�ж�Ŀǰ������Ƿ������ͼ

	BOOL bAllowInputActivation = CNetBarConfig::GetInstance()->GetInputActivation();  //�Ƿ�������ͨ������

	//���ݵ�һ���ַ��ж�
	switch (strTmp.GetAt(0))    //�ж�����ĵ�һλ
	{
	//�����ô��룬����ģ��ˢ��
	case '*':
		{
			CString strNetId = strTmp.Mid(1);
			if(strNetId.GetLength() < 5)
			{
				return;
			}
			else if(strNetId.GetLength() < 18 )
			{
				strNetId.Format(_T("%018s"),strTmp.Mid(1));
			}
			//CString strNetId = _T("420114198607072219");
			//CString strIdNumber = _T("420114198607072219");
			CString strIdNumber = strNetId;
			CString strName = _T("��������");

			if (bAllowInputActivation) //�������뼤��
			{
				IBA_LOG(_T("Temp.Simulate sweeping personal IDcard ..."));

				CActiveMemberView *pMemberView = CLocalServer::GetInstance()->GetActiveMemberView();
				int nIndex = pMemberView->FindUserAt(0, strTmp);
				if(0 <= nIndex)   //����û��б����Ѵ��ڸ��û���Ϣ,������Ϊѡ�У�������Ҫ����
				{
					CActiveMember ActiveMember;
					pMemberView->SetSelUser(nIndex,TRUE);
					//����Ѿ�����
					if(pMemberView->GetCurActiveMember(ActiveMember))
					{
						strTmp = ActiveMember.GetNetId();
						//����m_NebBarUser��m_localActiveMember�������û���������Ҫ����
						theApp.GetRealName()->OnRealNameMsg(999, &strTmp, strTmp.GetLength()); 
						return;
					}	
				}

				//CActiveMemberView *pMemberView = CLocalServer::GetInstance()->GetActiveMemberView();
				//pMemberView->SetSelUser(pMemberView->FindUserAt(0, strTmp), TRUE);

				int		nIdType = 12;         // ����֤
				CString strPassword = "123";  // ����

				CRegKey reg;

				if(0 == reg.Create(HKEY_CURRENT_USER, _T("SoftWare\\Sunlike\\IBA\\RealName\\Base\\NewUser")))/*Base\\NewUser"))*/
				{
					reg.SetStringValue(_T("IDNo"), strNetId);
					reg.SetStringValue(_T("CertificateID"), strIdNumber);
					reg.SetDWORDValue(_T("CertificateType"), 12);
					reg.SetStringValue(_T("Name"), strName);
					reg.SetStringValue(_T("Password"), strPassword);

					reg.Close();
				}

				theApp.GetRealName()->OnRealNameMsg(999, &strTmp, strTmp.GetLength());
				return;
			}
			// ����ǻ�����ͼ���Ͳ��ü����û���ͼ
			theApp.GetIBAView()->GotoActiveMember(strTmp, !bComputerView);
		}
	case '\\'://Ϊ'\\'��ʾ�����ն�
		{
			if (strTmp.GetAt(1) == '\\') // - //+xxx��ʾ����ΪIP��ֻ���Ҽ��������ͼ��������������
			{	
				theApp.GetIBAView()->GotoComputerByIP(strTmp.Mid(2).Trim(), !bComputerView);//��IP���ն��б��в���,��ѡ�е�ǰ�ն�

			}
			else // - /+xxx��ʾ����Ϊ�ն˺ţ�ʹ���ն˺Ų���ȫƥ���ڼ����б��в����û���ѡ�и��û�������������ʾ�û���Ϣ�����˿
			{
				theApp.GetIBAView()->GotoComputer(strTmp.Mid(1).Trim(), !bComputerView);  //���ն˺����ն��б��в���,��ѡ�е�ǰ�ն�
			}
			
			CComputerInfo Info;

				//������Ӧ����
			if(CLocalServer::GetInstance()->GetComputerListView()->GetCurComputerInfo(Info))
			{
				ActivateWndByComputerInfo(Info);
			}	
		}

		break;

	case '?'://Ϊ�����˺Ų����նˣ������ն���ͼ��ѡ�и��ն�
		{
			theApp.GetIBAView()->GotoComputerByNetID(strTmp.Mid(1).Trim());

			CComputerInfo Info;

				//������Ӧ����
			if(CLocalServer::GetInstance()->GetComputerListView()->GetCurComputerInfo(Info))
			{
				ActivateWndByComputerInfo(Info);
			}	
		}		
		break;

	case '+'://�����˺Ž��н���ֵ
		{
			CCreditHelpper CreditHelpper;
			CreditHelpper.Execute(strTmp.Mid(1));//�ӱ��ͨ��ȡ���룬��������״���������ĳ�ֵ����򱾵س�ֵ����
			m_combFast.SetFocus();
		}
		break;

	default://������
		{
			//�����λΪ��ĸ
			//CHAR chFirstChar = strTmp.GetAt(0);
			//if( _T('a')<=chFirstChar && chFirstChar<=_T('z') || _T('A')<=chFirstChar && chFirstChar<=_T('Z'))
			//{
			//	CString strTermID = strTmp;
			//	CActiveMember ActiveMember;
			//	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberBest(strTermID,ActiveMember, FALSE);
			//	theApp.GetIBAView()->GotoComputer(strTermID, !bComputerView);
			//	CComputerInfo Info;

			//	//������Ӧ����
			//	if(CLocalServer::GetInstance()->GetComputerListView()->GetCurComputerInfo(Info))
			//	{
			//		ActivateWndByComputerInfo(Info);
			//	}	
			//	return;
			//}

			CActiveMemberView *pMemberView = CLocalServer::GetInstance()->GetActiveMemberView();   //�û���ͼ

			BOOL bFoundInActivedList(FALSE);

			switch (nInputLen)
			{
			//ʵ��1λ������ڿ�ʼ�Ѿ���ɸ����,������Ϊ�˴����Ķ�
			case 1:
			case 2:
			case 3:
				{
					CString strTermID = strTmp;
					CActiveMember ActiveMember;
					CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberBest(strTermID,ActiveMember, FALSE);
					theApp.GetIBAView()->GotoComputer(strTermID, !bComputerView);

					CComputerInfo Info;

						//������Ӧ����
					if(CLocalServer::GetInstance()->GetComputerListView()->GetCurComputerInfo(Info))
					{
						ActivateWndByComputerInfo(Info);
					}	
				}
				break;
			//4λ��5λ,��ʾ�˺�.
			case 4:
			case 5: 
				{
					//�ӱ��ؿ����ƥ��ID,�����û�ѡ��
					CString strSelectedID;
					UINT nMemberId;
					if(FALSE == QueryIDs_PartialMatch(strTmp,strSelectedID)){return;}
					if(FALSE == QueryIDs_PartialMatch(strTmp,nMemberId)){return;}					
					//if(FALSE == CIBADAL::GetInstance()->GetMemberId(strSelectedID,nMemberId)){return;}

					//�Ƿ񼤻�
					CActiveMember ActiveMember;
					BOOL bActivated = CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);

					//�Ѽ���,����
					if(bActivated)
					{
						PopupWindow(ActiveMember);
					}
					else//δ����.����ݺ�̨�����Ƿ����������ж϶���
					{
						if(bAllowInputActivation)//�������,���� + ����
						{							
							if(Activation(strSelectedID,nMemberId))
							{
								CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
								PopupWindow(ActiveMember);
							}
						}
						else//���������,������ֵ����
						{
							PopupWindow_UnActiveMember(strSelectedID);
						}
					}	
				}
				break;
			//6λ������
				//ƥ���Ѽ����û�ʱ,��ͬ�ϲ�ѯ�󵯴�
				//ƥ��δ�����û�ʱ������ģʽ�µ����ر��в����û�
								  //����ģʽ�µ�����У�鿨��
				//����������򵽱��ؿ���в���ȫƥ�䣬����������
			default:
				{
					//�ӱ��ؿ����ƥ��ID,�����û�ѡ��
					CString strSelectedID;
					UINT nMemberId;
					CActiveMember ActiveMember;
					if(QueryActiveUser(strTmp,nMemberId))//�ڼ����б����ҵ�,�򵯳���Ӧ����
					{						
						//CIBADAL::GetInstance()->GetMemberId(strSelectedID,nMemberId);
						CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
						PopupWindow(ActiveMember);
					}
					else//δ����,�Ƿ��������뼤��
					{
						if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)//����ģʽ
						{
							//�����ر��в����û�
							if(FALSE == CIBADAL::GetInstance()->GetMemberId(strTmp,nMemberId))//�Ҳ�����ֱ���˳�
							{
								return;
							}
							if(bAllowInputActivation)//�������,���� + ����
							{
								if(Activation(strTmp,nMemberId))
								{
									CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
									PopupWindow(ActiveMember);
								}
							}
							else//��������ģ�����ȫƥ����ң�����
							{
								if(QueryIDs_PartialMatch(strTmp,strSelectedID))
								{
									PopupWindow_UnActiveMember(strSelectedID);
								}	
							}
						}
						else//����ģʽ��ָ�ģʽ
						{
							//У�鿨��
							CIDCheckCardMethod IDCheckCard;
							IDCheckCard.SetSerialNum(strTmp);
							IDCheckCard.SetSerialNumType(1);
							theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

							if(IDCheckCard.GetMemberId() <= 0)//��û�з����û��ţ�ֱ���˳�
							{
								return;
							}
							//���û��ŵģ��������
							if(bAllowInputActivation)//�������,���� + ����
							{
								if(Activation(strTmp,nMemberId))
								{
									CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
									PopupWindow(ActiveMember);
								}
							}
							else//��������ģ�����
							{
								PopupWindow_UnActiveMember(strTmp);
							}
						}
					}
				}
				
			}	
		}
	}
	return;
}

void CDlgIBAFastlane::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==EVENT_TIME_BUGLE)
	{
		UpdateBugle();
		m_bBugleActive = !m_bBugleActive;

		if ( (--m_nTurnCount) <= 0 )
		{
			StopBugleTurn(); // ֹͣ���Ƚ���
		}
	}

	CBCGPDialog::OnTimer(nIDEvent);
}

void CDlgIBAFastlane::UpdateBugle()
{
	CWnd *pBugle = GetDlgItem(IDC_PIC_BUGLE);

	WINDOWPLACEMENT wp;
	pBugle->GetWindowPlacement(&wp);

	InvalidateRect(&wp.rcNormalPosition);
}

void CDlgIBAFastlane::StartBugleTurn(BOOL bStartActive/*=TRUE*/)
{
	m_nTurnCount = COUNT_TURN;
	m_bBugleActive = bStartActive;
	SetTimer(EVENT_TIME_BUGLE,TIME_BUGLE,0);
}

void CDlgIBAFastlane::StopBugleTurn(BOOL bLastActive/*=FALSE*/)
{
	m_nTurnCount = 0;
	m_bBugleActive = bLastActive;
	UpdateBugle();
	KillTimer(EVENT_TIME_BUGLE);
}

void CDlgIBAFastlane::OnCbnSelchangeComboFast()
{
	m_combFast.SetWindowText(_T(""));  // �����ʾ���ֿ�
}
void CDlgIBAFastlane::OnCbnSelendcancelComboFast()
{
	m_combFast.SetWindowText(_T(""));
}

LRESULT CDlgIBAFastlane::OnUpdateRollText(WPARAM wParam, LPARAM lParam)
{
	CDlgIBAFastlane* pDlg = this;
	pDlg->m_stcContent.StopRolling();
	pDlg->m_stcContent.ClearAll();
	
	CString strs = LPCTSTR(wParam);
	if (!strs.IsEmpty())
	{
		pDlg->m_stcContent.SetText(strs,_T('|'));
	}
	else
	{
		// ���û�л�ȡ�����Ѽ�¼���Ͳ���ʹ�����ձ��ĺô�
		pDlg->m_stcContent.SetText(StringZengshoubao,_T('|'));
	}

	pDlg->StartBugleTurn();
	pDlg->m_stcContent.StartRolling(5000);
	return 0;
}
//ȡ��������Գ�������
void CDlgIBAFastlane::OnStnClickedPicStatus()
{
	CIBAString strToolTip;
	strToolTip.LoadString(IDS_IBA_STATUS_TEXT);
	//strToolTip = _T("<h3>��ţ����̨</h3><br></br><h4>����ģʽ</h4><br></br><p>����̨��Ʒ������������ӣ����й��ܶ�������ʹ�á�</p>	<br></br><h4>Ӧ��ģʽ</h4><br></br><p>����̨��Ʒ�����ʧȥ���ӣ�ֻ�ܿ������û�����ֵ�˿���޸����룬���ܽ��ࡣ</p><br></br><h4>�ָ�ģʽ</h4><br></br><p>����̨��Ʒ������������ӣ����б����û���û�н����˿��ʱ�����ܽ��ࡣ</p>");

	CRect rc(0,0,0,0);
	GetDlgItem(IDC_PIC_STATUS)->GetWindowRect(rc);

	//20160116-liyajun
	m_ToolTip.ShowHelpTooltip(&rc.TopLeft(), strToolTip );

	SetFocus();

	// 2015/5/18 tmelody
	// �������Ƕ���״̬����ô�������Իָ�
	//// {
	//IBA_LOG0(_T("�����ť��ͼ�ָ�����"));
	//if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	//{
	//	CGetTimeMethod time;
	//	time.Execute();

	//	int nStatus = time.GetStatusCode();
	//	if (nStatus >= 0)
	//	{
	//		IBA_LOG0(_T("�����ʱ�ѻָ�"));
	//		CBrokenNetwork::EnableNetwork();
	//		//�ӳ���ģʽ�ָ�������ģʽʱ��û�б���ģʽ���ݣ���˲���Ҫ��������ָ��߳�
	//		//CBrokenNetwork::GetInstance()->m_threadRestore->Start();
	//	}
	//}
	// }
}

LRESULT CDlgIBAFastlane::OnUpdateIBAStatus(WPARAM wParam, LPARAM lParam)
{
	m_nIBAStatus = wParam;
	Invalidate();
	return 0;
}
void CDlgIBAFastlane::OnLButtonDown(UINT nFlags, CPoint point)
{
	WINDOWPLACEMENT pm;
	m_stcPicStatus.GetWindowPlacement(&pm);
	CRect rc = pm.rcNormalPosition;
	if (rc.PtInRect(point))
	{
		OnStnClickedPicStatus();
	}

	CStringLoaderDlg<CBCGPDialog>::OnLButtonDown(nFlags, point);
}

BOOL CDlgIBAFastlane::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CStringLoaderDlg<CBCGPDialog>::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgIBAFastlane::OnMouseMove(UINT nFlags, CPoint point)
{
	WINDOWPLACEMENT pm;
	m_stcPicStatus.GetWindowPlacement(&pm);
	CRect rc = pm.rcNormalPosition;
	if (rc.PtInRect(point))
	{
		SetCursor(::LoadCursor(NULL,IDC_HAND));
	}

	CStringLoaderDlg<CBCGPDialog>::OnMouseMove(nFlags, point);
}

/*************************************************
Function:       //LocalQueryActiveUser
Description:    //����ȫ�˺�ƥ������Ѽ����û�
				//������ActiveMember���в��Ҷ�Ӧ�û�,����ҵ����ƥ���û�,�򵯳����ڹ��û�ѡ��.����ѡ�е��û�,�������Ӧ����
Calls:          //GotoActiveMember
Table Accessed: //ActiveMember
Input:          //strNetId - �����û��Ĳ���
Return:         //û���ҵ��û�������false
Others:         //��LocalQueryUser����ֻ�����Ѽ���
*************************************************/
BOOL CDlgIBAFastlane::QueryActiveUser(CString strNetId,UINT &nMemberId)
{
	//strResultNetdId.Empty();

	CDlgMemberInfo MemberInfoDlg;
	MemberInfoDlg.SetQueryStyle(5); //��ѯ�����Ѽ����û�-ActiveMember��
	MemberInfoDlg.SetNetId(strNetId);

	if (MemberInfoDlg.DoModal() == IDOK) 
	{
		m_combFast.SetFocus(); //����Ǳ���ģ�������ܻ������
		//IBA_LOG(_T("QueryActiveUser::NetId:%s"),MemberInfoDlg.GetNetId());
		//strResultNetdId = MemberInfoDlg.GetNetId(); //ȡ���û���ǰѡ�е��˺�
		//IBA_LOG(_T("QueryActiveUser::strResultNetId:%s"),strResultNetdId);
		nMemberId = MemberInfoDlg.GetMemberId();

		if(0 >= nMemberId)
		{
			return FALSE;
		}
		else
		{
			//bRet = theApp.GetIBAView()->GotoActiveMember(strNetID,false);
			return TRUE;
		}
	}
	else
	{
		//���û�������ý��㵽���������Ȼ(���֮ǰ��ѡ��ĳ���ն˵Ļ�)�ᵯ����ֵ�Ի���
		m_combFast.SetFocus();
		//����Ի�����Ŀ����Ϊ0����ʾ�û�û��ѡ�񣬵���ʵ���Ѿ����ҵ���
		//if(MemberInfoDlg.GetItemCount() != 0) 
		//{
		//	bRet = true;
		//}
		return FALSE;
	}	
}
/*************************************************
Function:       //ActivateWndByComputerInfo
Description:    //�����ն���ϢcomputerInfo�������б����ҵ��û���Ȼ�󵯳�����
Input:          //Info - �ն���Ϣ
*************************************************/
void CDlgIBAFastlane::ActivateWndByComputerInfo(CComputerInfo& Info)
{
	CString strNetId = Info.GetNetId();
	UINT nMemberId = Info.GetMemberID();

	CActiveMember ActiveMemberActivated;

	if(!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMemberActivated))
	{
		IBA_LOG(_T("Abnormal.CDlgIBAFastlane::OnCbnSelecdokComboFast. Couldn't find ActiveMember for specified computer!"));
		return ;
	}
	
	PopupWindow(ActiveMemberActivated);
	//if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 0) //�������κζԻ���
	//{
	//	return ;
	//}

	//if (AfxGetMainWnd()->IsIconic())//�жϵ�ǰ����С����
	//{
	//	AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);//���
	//}

	//AfxGetMainWnd()->BringWindowToTop();
	//AfxGetMainWnd()->SetForegroundWindow();

	//CCurrentNetBarUser::GetInstance()->SetNetId(strNetId);

	////������ֵ�˿�
	//if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 1)
	//{
	//	if (CCreditDlg::m_StaticPrehWnd != NULL) //�����Ѿ���������
	//	{
	//		BOOL bRet = ::SendMessage(CCreditDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)(LPCTSTR)strNetId);
	//		if (!bRet) //��ʾ���ڳ�ֵ
	//		{
	//			//DoNothing
	//		}
	//		else
	//		{
	//			::SetForegroundWindow(CCreditDlg::m_StaticPrehWnd);
	//			::SetFocus(CCreditDlg::m_StaticPrehWnd);
	//		}
	//	}
	//	else
	//	{
	//		CCreditDlg dlg;
	//		dlg.SetLocalActiveMember(ActiveMemberActivated);
	//		dlg.DoModal();	
	//	}
	//}
	//else if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 2) //ʹ���µĳ�ֵ/���˶Ի���
	//{
	//	if (AfxGetMainWnd()->IsWindowEnabled())
	//	{
	//		//���жϳ�ֵ���ֵ�˿��δ�������������˵�����ڳ�ֵ��������������
	//		BOOL bIsNDCredit(FALSE);
	//		LPTSTR lpszWndCaption = new TCHAR[30];
	//		memset(lpszWndCaption,0,30);
	//		HWND hwnd = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();
	//		if(::GetWindowText(hwnd,lpszWndCaption,30) > 0)
	//		{
	//			CString strWndCation(lpszWndCaption);
	//			strWndCation.Trim();
	//			bIsNDCredit = !strWndCation.CompareNoCase(_T("��ֵ"));
	//		}
	//		if(bIsNDCredit)  //��ֵ�����Ѿ�����
	//		{
	//			//DoNothing
	//		}
	//		else
	//		{
	//			CNDReturnDlg dlg;
	//			dlg.SetLocalActiveMember(ActiveMemberActivated);
	//			dlg.DoModal();
	//		}
	//	}
	//}
	//else
	//{
	//	//...����ʾ�κζԻ�����DoNothing
	//}
}

/***************************************************************************************************
Function:       //PopupWindow_UnActiveMember
Description:    //Ϊδ�����û�������ֵ����
Input:          //strNetId - ���������ڵ��û��˺�
Others:         //��Ϊδ����,ֻ�ܽ��г�ֵ����,Ҫ����������ť.
********************************************************************************************************/
void CDlgIBAFastlane::PopupWindow_UnActiveMember(CString strNetId)
{
	CActiveMember Am;
	if(CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(strNetId,Am))
	{
		return;
	}
	
	//�����ǰ��������С���ڣ������
	if (AfxGetMainWnd()->IsIconic())
	{
		AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);
	}

	AfxGetMainWnd()->BringWindowToTop();
	AfxGetMainWnd()->SetForegroundWindow();

	CCurrentNetBarUser::GetInstance()->SetNetId(strNetId);

	//������ֵ����
	if (AfxGetMainWnd()->IsWindowEnabled())
	{
		CNDCreditDlg dlg;
		dlg.SetNetId(strNetId);	
		dlg.DoModal();
	}
	return;
}

/***************************************************************************************************
Function:       //PopupWindow
Description:    //��������̨���õ�������
Input:          //ActiveMember - ���������ڵ��û�����
********************************************************************************************************/
void CDlgIBAFastlane::PopupWindow(CActiveMember& ActiveMember)
{
	CString strNetId = ActiveMember.GetNetId();
	
	//�����ǰ��������С���ڣ������
	if (AfxGetMainWnd()->IsIconic())
	{
		AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);
	}

	AfxGetMainWnd()->BringWindowToTop();
	AfxGetMainWnd()->SetForegroundWindow();

	CCurrentNetBarUser::GetInstance()->SetNetId(strNetId);

	int nPopupFlag = CIBAConfig::GetInstance()->GetOperationAfterSwipingCard();
	/* nPopupFlag == 0 �������κδ���
       nPopupFlag == 1 ������ֵ�˿
	   nPopupFlag == 2 ��Ա������ֵ��,��ʱ�û��������˴�*/  //��������δ�����û�,ֻ�����ǻ�Ա,��ʱ�û��鲻��

	switch (nPopupFlag)
	{
	case 0:
		return;
	case 1:
		if (CCreditDlg::m_StaticPrehWnd != NULL) //�����Ѿ���������
		{
			BOOL bRet = ::SendMessage(CCreditDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)(LPCTSTR)strNetId);
			if (bRet) //��ʾû���ڳ�ֵ
			{
				::SetForegroundWindow(CCreditDlg::m_StaticPrehWnd);
				::SetFocus(CCreditDlg::m_StaticPrehWnd);
			}
		}
		else
		{
			CCreditDlg dlg;			
			dlg.SetNetId(strNetId, TRUE);			
			dlg.DoModal();	
		}
		return;
	case 2:
		if (AfxGetMainWnd()->IsWindowEnabled())
		{
			//���жϳ�ֵ���ֵ�˿��δ�������������˵�����ڳ�ֵ��ֱ�ӷ���
			BOOL bIsNDCredit(FALSE);
			LPTSTR lpszWndCaption = new TCHAR[30];
			memset(lpszWndCaption,0,30);
			HWND hwnd = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();
			if(::GetWindowText(hwnd,lpszWndCaption,30) > 0)
			{
				CString strWndCation(lpszWndCaption);
				strWndCation.Trim();
				bIsNDCredit = !strWndCation.CompareNoCase(_T("��ֵ"));
			}
			if(bIsNDCredit)
			{
				return;
			}


			//��Ա������ֵ��,��ʱ�û��������˴�
			bool bIsMember = ActiveMember.IsMember();
			if(bIsMember)//��Ա
			{
				CNDCreditDlg dlg;
				dlg.SetNetId(strNetId);
				dlg.DoModal();
			}
			else//��ʱ�û�
			{
				CNDReturnDlg dlg;
				dlg.SetNetId(strNetId);
				dlg.DoModal();
			}
		}
		return;
	default:
		return;
	}
}

/*************************************************
Function:       //QueryIDs_PartialMatch
Description:    //����ȫ�˺�ƥ�������ע���û�,����ҵ����ƥ���û�,�򵯳����ڹ��û�ѡ��
Table Accessed: //Member
Input:          //strNetId - �����û��Ĳ���
Output:			//strResultNetId - �ҵ����û�ID,���û�ѡ��ID
Return:         //û���ҵ�������false
*************************************************/
BOOL CDlgIBAFastlane::QueryIDs_PartialMatch(CString strNetId,CString &strResultNetId)
{
	strResultNetId.Empty();

	//�ӱ��ؿ��������
	CDlgMemberInfo MemberInfoDlg;
	MemberInfoDlg.SetQueryStyle(1); //�ӱ��ز�ѯ��Ч�û�
	MemberInfoDlg.SetNetId(strNetId);

	//ѡ�����û���ֻ��һ���û�,����TRUE.���򷵻�FALSE.
	if (MemberInfoDlg.DoModal() == IDOK)   //��member����ȡ�õ�һ��ƥ�����Ч�û�
	{
		m_combFast.SetFocus(); //����Ǳ���ģ�������ܻ������
		strResultNetId = MemberInfoDlg.GetNetId();

		if(strResultNetId.IsEmpty())
		{
			return FALSE;
		}
		else
		{
			return TRUE;			
		}
	}
	else//δѡ���û�,����FALSE
	{
		//���û�������ý��㵽���������Ȼ(���֮ǰ��ѡ��ĳ���ն˵Ļ�)�ᵯ����ֵ�Ի���
		m_combFast.SetFocus();
		////����Ի�����Ŀ����Ϊ0����ʾ�û�û��ѡ�񣬵���ʵ���Ѿ����ҵ���
		//if(MemberInfoDlg.GetItemCount() != 0) 
		//{
		//	return TRUE;
		//}
		return FALSE;
	}	
}

BOOL CDlgIBAFastlane::QueryIDs_PartialMatch(CString strNetId,UINT &nMemberId)
{
	//�ӱ��ؿ��������
	CDlgMemberInfo MemberInfoDlg;
	MemberInfoDlg.SetQueryStyle(1); //�ӱ��ز�ѯ��Ч�û�
	MemberInfoDlg.SetNetId(strNetId);

	//ѡ�����û���ֻ��һ���û�,����TRUE.���򷵻�FALSE.
	if (MemberInfoDlg.DoModal() == IDOK)   //��member����ȡ�õ�һ��ƥ�����Ч�û�
	{
		m_combFast.SetFocus(); //����Ǳ���ģ�������ܻ������
		nMemberId = MemberInfoDlg.GetMemberId();

		if(0 >= nMemberId)
		{
			return FALSE;
		}
		else
		{
			return TRUE;			
		}
	}
	else//δѡ���û�,����FALSE
	{
		//���û�������ý��㵽���������Ȼ(���֮ǰ��ѡ��ĳ���ն˵Ļ�)�ᵯ����ֵ�Ի���
		m_combFast.SetFocus();
		////����Ի�����Ŀ����Ϊ0����ʾ�û�û��ѡ�񣬵���ʵ���Ѿ����ҵ���
		//if(MemberInfoDlg.GetItemCount() != 0) 
		//{
		//	return TRUE;
		//}
		return FALSE;
	}	
}
/*************************************************
Function:       //Activation
Description:    //�����û�
Table Accessed: //Member
Input:          //strNetId - �����û��Ĳ���
Output:			//nMemberId - �û���
Return:         //�ɹ�����TRUE
*************************************************/
BOOL CDlgIBAFastlane::Activation(CString strNetId, UINT &MemberID)
{
	IBA_LOG(_T("���ͨ������..."));
	CActiveMember ActiveMember;
	INT nAllMoney = 0;

	//����ģʽ
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		//�����û�
		GxxDictionaryPtr memberPtr = CIBADAL::GetInstance()->GetMemberFromMember(strNetId);

		//��Member����У��NetId,δע���˻�����ʱ�˺ž�������ע��
		CString sql ;
		if(!memberPtr){return FALSE;}
		if(FALSE == CNetBarConfig :: GetInstance()-> GetIsMember ( memberPtr ->intValueForKey ("classId"))){return FALSE;}

		//������ʵ���Ӹ�����Ƭ����
		if(1 == CNetBarConfig::GetInstance()->GetIsShowRzxPhoto())
		{
			CString strPhotoPath;
			std ::wstring idNum = G2XUtility ::CG2XCommonTool :: Utf8_to_WideChar( std ::string ( memberPtr-> valueForKey ("idNumber")->stringValue ()));
			CString stridNum(idNum .c_str());   
			if(NS_REALNAME::CRZXRealName::GetPhotoPath(strPhotoPath,stridNum))
			{	
				CString strPath, strNewFile;
				strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("UserImages"));
				CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
				strNewFile.Format(_T("%s\\%s.jpg"), strPath, CIBAHelpper::FormatNetId(strNetId));
				IBA_LOG(_T("��ȡ��������Ƭ·��%s, ����·��%s"), strPhotoPath, strNewFile);
				if(!CopyFile(strPhotoPath, strNewFile,FALSE))
				{
					IBA_LOG(_T("������������Ƭʧ��"));
				}
			}	
			else
			{
				IBA_LOG(_T("��ȡ��������Ƭʧ��"));
			}
		}

		//Ϊ��Ա�û�������ʱ������������ڱ���ģʽ���ϻ�����ʼ�������Ϊ0.
		IBA_TRACE ("%s", memberPtr -> describe_d(0, false ).c_str ());
		ActiveMember .SetNetbarID ( memberPtr-> valueForKey ("netBarId")->intValue ());                       //NetbarID
		ActiveMember .SetMemberID ( memberPtr-> valueForKey ("memberId")->intValue ());                       //MemberID
		ActiveMember .SetClassId ( memberPtr-> valueForKey ("classId")->intValue ());                         //ClassID
	/*	std ::wstring name = G2XUtility ::CG2XCommonTool :: Utf8_to_WideChar( std ::string ( memberPtr-> valueForKey ("name")->stringValue ()));
		ActiveMember .SetUserName ( CString( name .c_str ()));        */                                        //UserName
		CString strUserName;
		if(CIBADAL::GetInstance()->GetUserNameFromMember(strNetId,strUserName))
		{
			ActiveMember.SetUserName(strUserName);
		}
		else
		{
			std ::wstring name = G2XUtility ::CG2XCommonTool :: Utf8_to_WideChar( std ::string ( memberPtr-> valueForKey ("name")->stringValue ()));
			ActiveMember .SetUserName ( CString( name .c_str ()));                             //UserName
		}
		ActiveMember .SetNetId ( CString( CA2T (memberPtr -> valueForKey("serialNum")-> stringValue ())));       //NetId
		ActiveMember .SetIdType ( memberPtr-> valueForKey ("idType")->intValue ());                              //idType
		ActiveMember .SetPersonalID ( CString( CA2T (memberPtr -> valueForKey("idNumber")-> stringValue ())));   //PersonalID
		ActiveMember .SetSex ( memberPtr-> valueForKey ("sex")->intValue ());           //Sex
		ActiveMember .SetAvailavleBalance (0);                                          //AvailavleBalance
		ActiveMember .SetAdvancePayment (0);                                            //AdvancePayment
		ActiveMember .SetAmount (0);                                                    //Amount
		ActiveMember .SetCreditProcess ( ActiveMember. GetAdvancePaymentAsString ());   //CreditProcess start with AdvancePayment
		ActiveMember .SetCasherID ( CIBAConfig:: GetInstance ()->GetCashRegisterID ()); //CashierID
		CString strNetId = ActiveMember. GetNetId ();
		strNetId = strNetId . Right(8);
		strNetId .TrimLeft ( _T('0')); // ���˵���� ��0
		ActiveMember .SetNetIdAlias ( strNetId);                             //AliasNetId
		ActiveMember .SetActivationTime ( COleDateTime:: GetCurrentTime ()); //ActivationTime = now
		ActiveMember .SetActive ( TRUE);                                   //Active=TRUE
		ActiveMember .SetStatus ( CActiveMember:: EStatus_Waiting );       //Status = Waiting���ϻ�
		ActiveMember .SetIsOnLine(FALSE);                     //IsOnline
		//ActiveMember.SetRemark(_T("LocalActivated"));       //Remark:����Ǳ��ؼ����
		//ActiveMember.SetTerminalID(_T("''"));               //TerminalId��CheckoutInfo�������������ΪNULL(activeMember��Ĭ��ΪNULL����������Ҫ��ֵ)

		//��ӵ������б�
		CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(ActiveMember);
	}
	else//����ģʽ/�ָ�ģʽ
	{
		//�����̨�������Զ���ȡ�˺ţ���֤������Ϊ11��12ʱ(һ�������֤)����ȡ֤�������12λΪstrNetId������ͬ�����Ա���
		//if(1 == CNetBarConfig::GetInstance()->GetEnableAccount12())
		//{
		//	CString strCID = m_NetBarUser.GetCertificateID();
		//	if(12 < strCID.GetLength() && 
		//		(m_NetBarUser.GetCertificateType() == 11 ||
		//		m_NetBarUser.GetCertificateType() == 12))
		//	{
		//		m_NetBarUser.SetNetId(strCID.Right(12));
		//		strNetId = strCID.Right(12);
		//	}
		//}

		//CheckCard-У�鿨��
		UINT nMemberId;
		UINT nCardId;

		CIDCheckCardMethod IDCheckCard;
		IDCheckCard.SetSerialNum(strNetId);
		IDCheckCard.SetSerialNumType(1);
		theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

		if(0 != IDCheckCard.GetStatusCode()){return FALSE;}
		nCardId = IDCheckCard.GetCardId();		
		nMemberId = IDCheckCard.GetMemberId();

		
		//QueryBalance-��ѯ���
		CCurrentNetBarUser::GetInstance()->Balance.SetMemberId(nMemberId);
		CCurrentNetBarUser::GetInstance()->Balance.SetCardId(nCardId);
		CCurrentNetBarUser::GetInstance()->Balance.SetRound(0);//ȡ��
		theApp.GetCurCashier()->DoBalance(CCurrentNetBarUser::GetInstance()->Balance);

		if (0 != CCurrentNetBarUser::GetInstance()->Balance.GetStatusCode()){return FALSE;}

		// ������ʵ���Ӹ�����Ƭ����
		if(1 == CNetBarConfig::GetInstance()->GetIsShowRzxPhoto())
		{
			CString strPhotoPath;
			if(NS_REALNAME::CRZXRealName::GetPhotoPath(strPhotoPath, CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId()))
			{	
				CString strPath, strNewFile;
				strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("UserImages"));
				CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
				strNewFile.Format(_T("%s\\%s.jpg"), strPath, CIBAHelpper::FormatNetId(strNetId));
				IBA_LOG(_T("��ȡ��������Ƭ·��%s, ����·��%s"), strPhotoPath, strNewFile);
				if(!CopyFile(strPhotoPath, strNewFile,FALSE))
				{
					IBA_LOG(_T("������������Ƭʧ��"));
				}
			}	
			else
			{
				IBA_LOG(_T("��ȡ��������Ƭʧ��"));
			}
		}

		//����
		CBalanceMethod & Balance = CCurrentNetBarUser::GetInstance()->Balance;
		//��ܼ��������ϴ���Ƭ���ϴ���Ƭ
		if(0 != CNetBarConfig::GetInstance()->GetSoftSfreg())
		{
			CString strDest = CIBAHelpper::GetUserZjImgFilePath(Balance.GetIdType(), Balance.GetPersonalId());
			// 2014-8-13 - qsc ÿ�ζ����� if(!PathFileExists(strDest))
			{
				const CString strSource = theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg");
				CopyFile(strSource, strDest, FALSE);
				// 2014-7-4 - qsc ���������±���,��ɾ��ԭ�ļ�
				CString strBak = theApp.GetWorkPath() + _T("\\IBATemp\\ID_bak.jpg");
				CopyFile(strSource, strBak, FALSE);
				DeleteFile(strSource);
			}		
		}
		else if(2 == CNetBarConfig::GetInstance()->GetSoftSfreg())
		{
			CString strDest = CIBAHelpper::GetUserZcImgFilePath(Balance.GetIdType(), Balance.GetPersonalId());
			if(!PathFileExists(strDest))
			{
				if(!CIBAHelpper::TakePhoto(CIBAHelpper::GetUserZcImgPath(Balance.GetIdType(), Balance.GetPersonalId()), Balance.GetPersonalId()))
				{
					IBA_LOG(_T("CRealNameBase::ActivationMember Result is FALSE caused by taking photo failed- for MemberID = %d"),Balance.GetMemberId());
					return FALSE; 
				}
			}		
		}

		//����ģʽ���Ϊ0
		nAllMoney = Balance.GetTotalBalacne();
		if (FALSE == Balance.GetNeedActivation()){return FALSE;}
		if (FALSE == CServletHellper::ActivationMember(nMemberId)) {return FALSE;}

		//��ɾ���������½��棬�����ɱ�֤��ʾ����ǰ��
		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMemberDBPostActivation(Balance.GetMemberId());
		CLocalServer::GetInstance()->ActiveMemberList.AddActiveMemberPostActivation(Balance);

		//��ȡActiveMember������ʾ��¼
		CLocalServer::GetInstance()->GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember);
	}

	//�������
	CIBATTS::GetInstance()->SpeakBalance(nAllMoney);
	//��ʾ�����¼
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		::SendMessage (CMainFrame :: m_hOperationLog, WM_OPERATIONLOG , 1, (LPARAM)&ActiveMember );
	}
	else
	{
		::SendMessage (CMainFrame :: m_hOperationLog, WM_OPERATIONLOG , 1, NULL );
	}
	
	//ˢ���û���ͼ
	CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
	//����CurrentNetBarUser
	CCurrentNetBarUser::GetInstance()->SetNetId(strNetId);

	MemberID = ActiveMember.GetMemberID();
	return TRUE;
}