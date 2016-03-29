#pragma once

#include "Singleton.h"
#include "IniConfig.h"

class CIBAConfig : private CIniConfig
{
	SINGLETON_DECLARE(CIBAConfig)

public :
	
	static CString GetAppPath();
	static CIBAConfig* GetInstance();

	void LoadConfig();
	void SaveConfig();

public:
	static const INT MAX_TIME_LIMIT = 6;  // ʱ���������������λ��

private :

	UINT m_LocalSerialNo;

	UINT m_LogLevel;

	BOOL m_bPopMsgBox; //!<�Ƿ񵯳�ȷ�Ͽ�

	BOOL m_bUseOurReader;

	BOOL m_nOperationAfterSwipingCard;

public :

	UINT GetLocalSerialNo();

	BOOL GetPopMsgBox() const { return m_bPopMsgBox; }
	void SetPopMsgBox(BOOL newVal) { m_bPopMsgBox = newVal; }

	UINT GetLogLevel() const;

	BOOL GetUseOurReader() const { return m_bUseOurReader; }

	BOOL GetOperationAfterSwipingCard() const { return m_nOperationAfterSwipingCard; }
	void SetOperationAfterSwipingCard(BOOL newVal) { m_nOperationAfterSwipingCard = newVal; }

private :

	BOOL m_bDynamicReserve; //!<��̬���ý�

	UINT m_nDynamicReserveMoney;//���涯̬���ý�

	CString m_strShift1;

	CString m_strShift2;

	CString m_strShift3;

	CString m_strShiftName1;

	CString m_strShiftName2;

	CString m_strShiftName3;

	UINT m_nShiftReportType;
	//�Զ���ӡ�����
	BOOL m_bPrintShiftReport;
	//�Ƿ�ʹ�ܽ�����
	BOOL m_bShowShiftTotalInfo;
	//����༭������֧����
	BOOL m_bEditPayOut;

	INT m_nShiftCount;

public :

	INT GetShiftCount() const { return m_nShiftCount; }
	void SetShiftCount(INT newVal) { m_nShiftCount = newVal; }

	CString GetShiftName3() const { return m_strShiftName3; }
	void SetShiftName3(CString newVal) { m_strShiftName3 = newVal; }

	CString GetShiftName2() const { return m_strShiftName2; }
	void SetShiftName2(CString newVal) { m_strShiftName2 = newVal; }

	CString GetShiftName1() const { return m_strShiftName1; }
	void SetShiftName1(CString newVal) { m_strShiftName1 = newVal; }

	BOOL GetEditPayOut() const { return m_bEditPayOut; }
	void SetEditPayOut(BOOL val) { m_bEditPayOut = val; }

	UINT GetDynamicReserveMoney(); 
	void SetDynamicReserveMoney(UINT newVal); 

	BOOL GetShowShiftTotalInfo() const { return m_bShowShiftTotalInfo; }
	void SetShowShiftTotalInfo(BOOL newVal) { m_bShowShiftTotalInfo = newVal; }

	BOOL GetPrintShiftReport() const { return m_bPrintShiftReport; }
	void SetPrintShiftReport(BOOL newVal) { m_bPrintShiftReport = newVal; }

	CString GetShift1() const { return m_strShift1; }
	void SetShift1(CString newVal) { m_strShift1 = newVal; }

	CString GetShift2() const { return m_strShift2; }
	void SetShift2(CString newVal) { m_strShift2 = newVal; }

	CString GetShift3() const { return m_strShift3; }
	void SetShift3(CString newVal) { m_strShift3 = newVal; }

	BOOL GetDynamicReserve() const { return m_bDynamicReserve; }
	void SetDynamicReserve(BOOL newVal) { m_bDynamicReserve = newVal; }

	UINT GetShiftReportType() const { return m_nShiftReportType; }
	void SetShiftReportType(UINT newVal) { m_nShiftReportType = newVal; }

private :

	INT   m_PrintFontSize;
	
	UINT  m_nScanDrvId;

	BOOL  m_bUploadIDImage;

	BOOL  m_bUploadPersonImage;

	UINT  m_nIDImageDrvId;

	UINT  m_nPersonImageDrvId;

public :

	int GetPrintFontSize() const { return m_PrintFontSize; }
	void SetPrintFontSize(int newVal) { m_PrintFontSize = newVal; }

	INT GetScanId() const { return m_nScanDrvId; }
	void SetScanId(INT newVal) { m_nScanDrvId = newVal; }

	BOOL GetUploadIDImage() const { return m_bUploadIDImage; }
	void SetUploadIDImage(BOOL newVal) { m_bUploadIDImage = newVal; }

	BOOL GetUploadPersonImage() const { return m_bUploadPersonImage; }
	void SetUploadPersonImage(BOOL newVal) { m_bUploadPersonImage = newVal; }

	UINT GetIDImageDrvId() const { return m_nIDImageDrvId; }
	void SetIDImageDrvId(UINT newVal) { m_nIDImageDrvId = newVal; }

	UINT GetPersonImageDrvId() const { return m_nPersonImageDrvId; }
	void SetPersonImageDrvId(UINT newVal) { m_nPersonImageDrvId = newVal; }

private : //ע������

	UINT m_nPasswordType;
	CString m_strDefPassword;
	
	UINT m_nTempPasswordType;
	CString m_strTempDefPassword;

	UINT m_nDefUserClassID;
	UINT m_nDefIDTypeId;

	//ע�����Ƶ�����
	UINT m_nRegisterAge;
	//�������
	UINT m_nRegisterMoney;
	//�û��Դ���
	UINT m_bHasCard;
	
	BOOL m_bCheckIDCardNumber;

public :

	BOOL GetCheckIDCardNumber() const { return m_bCheckIDCardNumber; }
	void SetCheckIDCardNumber(BOOL newVal) { m_bCheckIDCardNumber = newVal; }

	BOOL GetHasCard() const { return m_bHasCard; }
	void SetHasCard(BOOL val) { m_bHasCard = val; }

	UINT GetRegisterMoney() const { return m_nRegisterMoney; }
	void SetRegisterMoney(UINT newVal) { m_nRegisterMoney = newVal; }

	UINT GetRegisterAge() const { return m_nRegisterAge; }
	void SetRegisterAge(UINT newVal) { m_nRegisterAge = newVal; }

	UINT GetPasswordType() const { return m_nPasswordType; }
	void SetPasswordType(UINT newVal) { m_nPasswordType = newVal; }
	CString GetDefPassword() const { return m_strDefPassword; }
	void SetDefPassword(CString newVal) { m_strDefPassword = newVal; }

	UINT GetTempPasswordType() const { return m_nTempPasswordType; }
	void SetTempPasswordType(UINT newVal) { m_nTempPasswordType = newVal; }
	CString GetTempDefPassword() const { return m_strTempDefPassword; }
	void SetTempDefPassword(CString newVal) { m_strTempDefPassword = newVal; }

	UINT GetDefUserClassID() const { return m_nDefUserClassID; }
	void SetDefUserClassID(UINT newVal) { m_nDefUserClassID = newVal; }
	UINT GetDefIDTypeId() const { return m_nDefIDTypeId; }
	void SetDefIDTypeId(UINT newVal) { m_nDefIDTypeId = newVal; }

private :
	
	UINT m_CashRegisterID;
	UINT m_nStoreID;

	BOOL m_bOpenAfterSave;
	BOOL m_bEnmergencyVisible;
	BOOL m_bPauseRevMsg;
	UINT m_nMinMoney;
	BOOL m_bShowAllComputer;
	BOOL m_bShowLastUserNetId;

	BOOL m_bMsgAutoHide;
	BOOL m_bMsgLeftBottom;

	UINT m_nFontType; // 0, 1, 2
	BOOL m_bOpenAfterEnter; //�س��Ƿ񵯳�����

public:

	UINT GetFontType() const { return m_nFontType; }
	void SetFontType(UINT newVal) { m_nFontType = newVal; }

	BOOL GetOpenAfterEnter() const { return m_bOpenAfterEnter; }
	void SetOpenAfterEnter(BOOL newVal) { m_bOpenAfterEnter = newVal; }

	BOOL GetOpenAfterSave() const { return m_bOpenAfterSave; }
	void SetOpenAfterSave(BOOL newVal) { m_bOpenAfterSave = newVal; }

	BOOL GetEnmergencyVisible() const { return m_bEnmergencyVisible; }
	void SetEnmergencyVisible(BOOL newVal) { m_bEnmergencyVisible = newVal; }

	UINT GetMinMoney() const { return m_nMinMoney; }
	void SetMinMoney(UINT newVal) { m_nMinMoney = newVal; }

	UINT GetCashRegisterID() const { return m_CashRegisterID; }
	void SetCashRegisterID(UINT newVal) { m_CashRegisterID = newVal; }

	UINT GetStoreID() const { return m_nStoreID; }
	void SetStoreID(UINT newVal) { m_nStoreID = newVal; }

	BOOL GetPauseRevMsg() const { return m_bPauseRevMsg; }
	void SetPauseRevMsg(BOOL newVal) { m_bPauseRevMsg = newVal; }

	BOOL GetShowAllComputer() const { return m_bShowAllComputer; }
	void SetShowAllComputer(BOOL newVal) { m_bShowAllComputer = newVal; }

	BOOL GetShowLastUserNetId() const { return m_bShowLastUserNetId; }
	void SetShowLastUserNetId(BOOL newVal) { m_bShowLastUserNetId = newVal; }

	BOOL GetMsgAutoHide() const { return m_bMsgAutoHide; }
	void SetMsgAutoHide(BOOL newVal) { m_bMsgAutoHide = newVal; } 

	BOOL GetMsgLeftBottom() const { return m_bMsgLeftBottom; }
	void SetMsgLeftBottom(BOOL newVal) { m_bMsgLeftBottom = newVal; } 

private : //TTS��������

	BOOL m_bEnableTTS;

	INT  m_nTTSVol;

	INT  m_nTTSVoice;

public :

	BOOL GetEnableTTS() const { return m_bEnableTTS; }
	void SetEnableTTS(BOOL newVal) { m_bEnableTTS = newVal; }

	INT GetTTSVol() const { return m_nTTSVol; }
	void SetTTSVol(INT newVal) { m_nTTSVol = newVal; }

	INT GetTTSVoice() const { return m_nTTSVoice; }
	void SetTTSVoice(INT newVal) { m_nTTSVoice = newVal; }

private : //��ֵ�˿�����

	UINT m_nReturnAction;//�س�������0Ϊ�رմ��壬1Ϊ�˿�

public:

	UINT GetReturnAction() const { return m_nReturnAction; }
	void SetReturnAction(UINT newValue) { m_nReturnAction = newValue; }

	GProperty(RefundmentTime, UINT);  // �Զ��˿�ʱ��
	GProperty(AllowNextDutyMoney, BOOL); // ����༭"������һ����"

	GProperty(JXCPayCash, BOOL); // ��Ʒ���ʱ���Ƿ�Ϊ�ֽ�֧����Ĭ��Ϊ�ֽ�֧����
	GProperty(ShowLocalModeButton, BOOL); // �ڵ�¼�����Ƿ���ʾ"���ö���ģʽ"�İ�ť
	
	BOOL GetPrintCreditTicket(BOOL bIsMember);
	BOOL GetPrintReturnTicket(BOOL bIsMember);
	BOOL GetPrintTicketBT(BOOL bIsMember);
	GProperty(PrintTicketCreditMember, BOOL); // �Ƿ��ӡ��Ա�û���ֵСƱ
	GProperty(PrintTicketCreditTemp, BOOL);   // �Ƿ��ӡ��ʱ�û���ֵСƱ
	GProperty(PrintTicketOpenMember, BOOL);   // �Ƿ��ӡ��Ա����СƱ
	GProperty(PrintTicketOpenTemp, BOOL);     // �Ƿ��ӡ��ʱ�û�����СƱ
	GProperty(PrintTicketBTMember, BOOL);     // �Ƿ��ӡ��Ա��ʱ�û�СƱ
	GProperty(PrintTicketBTTemp, BOOL);       // �Ƿ��ӡ��ʱ�û���ʱСƱ
	GProperty(PrintTicketReturnMember,BOOL);  // �Ƿ��ӡ��Ա����СƱ
	GProperty(PrintTicketReturnTemp, BOOL);   // �Ƿ��ӡ��ʱ�û��˿�СƱ

	// 2011/10/20-8206-gxx: 
	GProperty(OperationAfterEnter, INT);      // �û���ͼ����Enter�������ķ�Ӧ
	GProperty(OperationAfterDClick, INT);     // �û���ͼ���˫���������ķ�Ӧ
	GProperty(MaxCreditMoney, INT);           // ÿ�ʳ�ֵ�������, Ϊ0��ʾû������

private :

	COLORREF m_OnLineColor;
	COLORREF m_OffLineColor;
	COLORREF m_LockScreenColor;
	COLORREF m_BirthDayColor;
	COLORREF m_ThiefColor;
	COLORREF m_VIPColor;
	COLORREF m_BlackColor;
	COLORREF m_LittleMoneyColor;
	COLORREF m_NoRecordColor;
	COLORREF m_ConflictColor;
	COLORREF m_SuspendColor;
	COLORREF m_UnLockColor;
	COLORREF m_BundTimeColor;

public :
	
	//�û�������ɫ����
	COLORREF GetUserTypeColor(UINT nClassID);
	void SetUserTypeColor(UINT nClassID, COLORREF newVal); 
	
	//�����������
	UINT GetShowPCClassType(UINT nClassID);
	void SetShowPCClassType(UINT nClassID, UINT newVal);
	
	//״̬��ɫ����
	COLORREF GetUnLockColor() const { return m_UnLockColor; }
	void SetUnLockColor(COLORREF newVal) { m_UnLockColor = newVal; }

	COLORREF GetSuspendColor() const { return m_SuspendColor; }
	void SetSuspendColor(COLORREF newVal) { m_SuspendColor = newVal; }

	COLORREF GetConflictColor() const { return m_ConflictColor; }
	void SetConflictColor(COLORREF newVal) { m_ConflictColor = newVal; }

	COLORREF GetNoRecordColor() const { return m_NoRecordColor; }
	void SetNoRecordColor(COLORREF newVal) { m_NoRecordColor = newVal; }

	COLORREF GetLittleMoneyColor() const { return m_LittleMoneyColor; }
	void SetLittleMoneyColor(COLORREF newVal) { m_LittleMoneyColor = newVal; }

	COLORREF GetBlackColor() const { return m_BlackColor; }
	void SetBlackColor(COLORREF newVal) { m_BlackColor = newVal; }

	COLORREF GetVIPColor() const { return m_VIPColor; }
	void SetVIPColor(COLORREF newVal) { m_VIPColor = newVal; }

	COLORREF GetThiefColor() const { return m_ThiefColor; }
	void SetThiefColor(COLORREF newVal) { m_ThiefColor = newVal; }

	COLORREF GetBirthDayColor() const { return m_BirthDayColor; }
	void SetBirthDayColor(COLORREF newVal) { m_BirthDayColor = newVal; }

	COLORREF GetLockScreenColor() const { return m_LockScreenColor; }
	void SetLockScreenColor(COLORREF newVal) { m_LockScreenColor = newVal; }

	COLORREF GetOffLineColor() const { return m_OffLineColor; }
	void SetOffLineColor(COLORREF newVal) { m_OffLineColor = newVal; }

	COLORREF GetOnLineColor() const { return m_OnLineColor; }
	void SetOnLineColor(COLORREF newVal) { m_OnLineColor = newVal; }
	
	//��ʱ�û���ɫ����
	COLORREF GetBundTimeColor() const { return m_BundTimeColor; }
	void SetBundTimeColor(COLORREF newVal) { m_BundTimeColor = newVal; }

private:

	CString m_BitmapPath;//��λͼ·��

	UINT m_nDisplayType;//��ʾ��λͼ

public:
	CString GetSeatBmpPath() const { return m_BitmapPath; }
	void SetSeatBmpPath(CString newValue) { m_BitmapPath = newValue; }

	UINT GetDisplayType() const { return m_nDisplayType; }
	void SetDisplayType(UINT newValue) { m_nDisplayType = newValue; }

	// for- ����
	// ��ȡ����ͼ���·��
	CString GetImagesPath();
};
