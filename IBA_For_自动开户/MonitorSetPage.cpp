// MonitorSetPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include ".\monitorsetpage.h"


// CSetPageOfMonitor �Ի���

IMPLEMENT_DYNAMIC(CSetPageOfMonitor, CIBAPage)
CSetPageOfMonitor::CSetPageOfMonitor()
: CIBAPage(CSetPageOfMonitor::IDD)
{
}

CSetPageOfMonitor::~CSetPageOfMonitor()
{
}

void CSetPageOfMonitor::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLONLINE, m_CPOnLine);
	DDX_Control(pDX, IDC_BUTTON_CLOFFLINE, m_CPOffLine);
	DDX_Control(pDX, IDC_BUTTON_CLLOCK, m_CPLockScreen);
	DDX_Control(pDX, IDC_BUTTON_CLLM, m_CPLittleMoney);
	DDX_Control(pDX, IDC_BUTTON_CLBLACK, m_CPBlack);
	DDX_Control(pDX, IDC_BUTTON_CLBD, m_CPBirthDay);
	DDX_Control(pDX, IDC_BUTTON_CLTHIEF, m_CPThief);
	DDX_Control(pDX, IDC_BUTTON_CLVIP, m_CPVIP);
	DDX_Control(pDX, IDC_BUTTON_CLSUSPEND, m_CPSuspend);
	DDX_Control(pDX, IDC_BUTTON_CLUNLOCK, m_CPUnLock);
	DDX_Control(pDX, IDC_BUTTON_BUNDTIMECOLOR, m_CPBundTimeColor);
}


BEGIN_MESSAGE_MAP(CSetPageOfMonitor, CIBAPage)
END_MESSAGE_MAP()


// CSetPageOfMonitor ��Ϣ�������

void CSetPageOfMonitor::OnOK()
{
	CIBAConfig::GetInstance()->SetOnLineColor(m_CPOnLine.GetColor());
	CIBAConfig::GetInstance()->SetOffLineColor(m_CPOffLine.GetColor());
	CIBAConfig::GetInstance()->SetSuspendColor(m_CPSuspend.GetColor());
	CIBAConfig::GetInstance()->SetThiefColor(m_CPThief.GetColor());
	CIBAConfig::GetInstance()->SetVIPColor(m_CPVIP.GetColor());
	CIBAConfig::GetInstance()->SetLockScreenColor(m_CPLockScreen.GetColor());
	CIBAConfig::GetInstance()->SetBirthDayColor(m_CPBirthDay.GetColor());
	CIBAConfig::GetInstance()->SetBlackColor(m_CPBlack.GetColor());
	CIBAConfig::GetInstance()->SetUnLockColor(m_CPUnLock.GetColor());
	CIBAConfig::GetInstance()->SetLittleMoneyColor(m_CPLittleMoney.GetColor());
	CIBAConfig::GetInstance()->SetBundTimeColor(m_CPBundTimeColor.GetColor());

	CIBAPage::OnOK();
}


BOOL CSetPageOfMonitor::OnInitDialog()
{
	CIBAPage::OnInitDialog();
	//����״̬��ɫ��ť����
	CIBAString strTmp;
	strTmp.LoadString(IDS_MORE);//�����ִ�������...��

	const UINT ColumnsNumber = 10;//��ť�б�����

	m_CPOnLine.EnableOtherButton(strTmp);//ʹ�ܰ�ť�б���
	m_CPOnLine.SetColumnsNumber(ColumnsNumber);//�����б������
	m_CPOnLine.SetColor(CIBAConfig::GetInstance()->GetOnLineColor());//ȡ�������ļ�����ɫ

	m_CPOffLine.EnableOtherButton(strTmp);
	m_CPOffLine.SetColumnsNumber(ColumnsNumber);
	m_CPOffLine.SetColor(CIBAConfig::GetInstance()->GetOffLineColor());

	m_CPThief.EnableOtherButton(strTmp);
	m_CPThief.SetColumnsNumber(ColumnsNumber);
	m_CPThief.SetColor(CIBAConfig::GetInstance()->GetThiefColor());

	m_CPBlack.EnableOtherButton(strTmp);
	m_CPBlack.SetColumnsNumber(ColumnsNumber);
	m_CPBlack.SetColor(CIBAConfig::GetInstance()->GetBlackColor());

	m_CPBirthDay.EnableOtherButton(strTmp);
	m_CPBirthDay.SetColumnsNumber(ColumnsNumber);
	m_CPBirthDay.SetColor(CIBAConfig::GetInstance()->GetBirthDayColor());

	m_CPSuspend.EnableOtherButton(strTmp);
	m_CPSuspend.SetColumnsNumber(ColumnsNumber);
	m_CPSuspend.SetColor(CIBAConfig::GetInstance()->GetSuspendColor());

	m_CPLittleMoney.EnableOtherButton(strTmp);
	m_CPLittleMoney.SetColumnsNumber(ColumnsNumber);
	m_CPLittleMoney.SetColor(CIBAConfig::GetInstance()->GetLittleMoneyColor());

	m_CPUnLock.EnableOtherButton(strTmp);
	m_CPUnLock.SetColumnsNumber(ColumnsNumber);
	m_CPUnLock.SetColor(CIBAConfig::GetInstance()->GetUnLockColor());

	m_CPVIP.EnableOtherButton(strTmp);
	m_CPVIP.SetColumnsNumber(ColumnsNumber);
	m_CPVIP.SetColor(CIBAConfig::GetInstance()->GetVIPColor());

	m_CPLockScreen.EnableOtherButton(strTmp);
	m_CPLockScreen.SetColumnsNumber(ColumnsNumber);
	m_CPLockScreen.SetColor(CIBAConfig::GetInstance()->GetLockScreenColor());

	//////////////��ʱ��ɫ��ť����///////////////
	m_CPBundTimeColor.EnableOtherButton(strTmp);
	m_CPBundTimeColor.SetColumnsNumber(ColumnsNumber);
	m_CPBundTimeColor.SetColor(CIBAConfig::GetInstance()->GetBundTimeColor());

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

