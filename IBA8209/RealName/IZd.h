//------------------------------------------------------
//	����ӿ��࣬��Ҫ���ſƼ��Ķ�̬��
//
//		ijqrnlog.dll
//		
//------------------------------------------------------
#pragma once

#include "..\Singleton.h"
#include <string>
#include <map>


//#include "Communication.h"

#define IK_BASE WM_USER + 1100
#define IK_ID_GETIDCARDDATA		IK_BASE + 10	//��ð󶨿��������
#define IK_ID_OPENUSER			IK_BASE + 11	//ʵ��������
#define IK_ID_CLOSEUSER			IK_BASE + 12	//ʵ�����ػ�
#define IK_ID_LOGINOUT			IK_BASE + 13	//ʵ��������
#define IK_ID_LOGININ			IK_BASE + 14	//ʵ��������
#define ID_ID_TOCLOSEOUT		IK_BASE + 15	//����ͣҵ


class CIZd
{
	SINGLETON_DECLARE(CIZd)	
public:
	static CIZd* GetInstance();

	typedef struct IC_CRC_RE_TAG
	{
		int iRe;					//CRCУ�鷵��ֵ  0��У��ɹ�  -1��CRCУ�����  -2���汾�Ŵ���
		BOOL bIsHeadInputCard;		//�Ƿ������ֹ�������������  TURE������  FALSE��������
		double dDeductMoney;		//����ˢ���۵�Ľ�0.5Ԫ���������
		int iDeductDistantTime;		//ˢ���۵���ʱ�䣬��λ����
		BOOL bIsMacAllInOrOut;		//�Ƿ�����ȫ���ն���������� TURE������  FALSE��������
	}IC_CRC_RE;

	// �����͹ػ����ݽṹ
	typedef struct T_IC_USER_TAG
	{
		TCHAR szikIDNo[21];			//ʵ�������
		TCHAR szikName[31]; 		//����
		int		iCertType;			//֤������
		TCHAR szCertNumber[19];		//֤������
		int   iCardType;			//��������ȡ���� 0������֤ 1���ֻ� 2��ָ�� 3�����ӵǼ�
		TCHAR szMobile[12];			//�ֻ���	
		int	  iValidate;			//��������Ƿ�ɹ�
	}T_IC_USER;

	// �����͹ػ����ݽṹ  
	typedef struct IC_USER_TAG
	{
		char szikIDNo[21];			//ʵ�������
		char szikName[31]; 		//����
		int		iCertType;			//֤������
		char szCertNumber[19];		//֤������
		int   iCardType;			//��������ȡ���� 0������֤ 1���ֻ� 2��ָ�� 3�����ӵǼ�
		char szMobile[12];			//�ֻ���	
		int	  iValidate;			//��������Ƿ�ɹ�
	}IC_USER;

	// �������������ݽṹ
	typedef struct T_IC_LOGIN_TAG
	{
		TCHAR szikICNo[21];			//�û��İ󶨿����
		TCHAR szikIDNo[21];			//ʵ�������
		TCHAR szikIP[16];			//�ͻ���IP��ַ
		TCHAR szikHost[31];			//�ͻ�������
		TCHAR szicPassword[16];		//ʵ��������
	}T_IC_LOGIN;

	// �������������ݽṹ
	typedef struct IC_LOGIN_TAG
	{
		char szikICNo[21];			//�û��İ󶨿����
		char szikIDNo[21];			//ʵ�������
		char szikIP[16];			//�ͻ���IP��ַ
		char szikHost[31];			//�ͻ�������
		char szicPassword[16];		//ʵ��������
	}IC_LOGIN;

	// ��ӻ�Ա���ݽṹ
	typedef struct ID_ADDINSIDR_TAG
	{
		char szikICNo[21];			//�󶨿����
		char szikIDNo[21];			//ʵ�������
		char szikName[31]; 		//����
		int   iIDCardType;			//֤������
		char  szCertificateID[19];	//֤������
		int   iErrCode;				//����ţ�����������0��ʾ�ɹ�
		char  szErrMsg[101];		//�������������Error��Ϊ0�Ļ�
		int   iCardGetType;			//ˢ֤����
		int   iIsCheck;				//�Ƿ���� 0��ʾû��1����ʾδ���2��ʾͨ��
	}ID_ADDINSIDR;

	// �۵��ѯ���ݽṹ
	typedef struct IC_DEDUCT_TAG
	{
		TCHAR szBeginTime[15]; 	//��ʼʱ��
		TCHAR szEndTime[15]; 	//����ʱ��
		TCHAR szCashMacNum[31];	//�����˱��(��Ϊ��)
	}IC_DEDUCT;
	// ˵����ʱ�䶨��14λ����ʽΪ��4λ�꣬2λ�£�2λ�գ�2λСʱ��2λ���ӣ�2λ��

	// ��ȡ�������������ݽṹ
	typedef struct ID_GETCARDPWD_TAG
	{
		char szPWD[33];			//����������(MD5ֵ����BASE64)
		int  iErrCode;			//����ţ�����������0��ʾ�ɹ�
		char szErrMsg[101];		//�������������Error��Ϊ0�Ļ�
	}ID_GETCARDPWD;

	// ����ֵ��Ϣ
	typedef struct ID_RETURN_TAG
	{
		int  iErrCode;			//����ţ�����������0��ʾ�ɹ�
		char szErrMsg[101];		//�������������Error��Ϊ0�Ļ�
	}ID_RETURN;

	// ����ֵ�����ṹ
	typedef struct ID_RETURNS_TAG
	{
		TCHAR szRenturn[101]; 	//����ֵ����
	}ID_RETURNS;

	// �ϻ�����ֵ��Ϣ
	typedef struct ID_LOGINRETURN_TAG
	{
		char szCardId[21];		//��������
		int  iCardType;			//��������ȡ���� 0������֤ 1���ֻ� 2��ָ�� 3�����ӵǼ�
		char szName[31]; 		//����
		int   iCertType;		//֤������
		char szCertNumber[19];	//֤������
		int  iErrCode;			//����ţ�����������0��ʾ�ɹ�
		char szErrMsg[101];		//�������������Error��Ϊ0�Ļ�
	}ID_LOGINRETURN;

	// ����KEYʣ�����
	typedef struct KEYBALANCERETURN_TAG
	{
		int  iKEYBalance;		//KEYʣ�����
		int  iErrCode;		//����ţ�����������0��ʾ�ɹ�
		TCHAR szErrMsg[101];	//�������������Error��Ϊ0�Ļ�
	}ID_KEYBALANCERETURN;

	// ʹ���ն����ݼ����Ϣ
	typedef struct ID_USEMACVERIFY_TAG
	{
		TCHAR szMacName[17];	//��������
		TCHAR szCardId[21];	//��������
		int  iCardType;		//��������ȡ���� 0������֤ 1���ֻ� 2��ָ�� 3�����ӵǼ�
		TCHAR szName[31]; 	//����
		int   iCertType;		//֤������
		TCHAR szCertNumber[19];	//֤������
	}ID_USEMACVERIFY;

	typedef struct IC_CARDDATA_TAG
	{
		TCHAR szikICNo[21]; //�Ʒ�����󶨿����
		int   iICCardType;  //�����ͣ�1�C��ͨ�Ʒѿ���2�C��Ա����3 �C������9 �C������
	}IC_CARDDATA;

	//����˵����
	//DWORD dwCRC		CRCֵ��������Ч��У��
	//char * FeeSoftVer	�Ʒ�����İ汾��
	//����ֵ��
	//����У��������
	typedef IC_CRC_RE (__stdcall *FUN_V2_SetCRC)(DWORD dwCRC, char * FeeSoftVer);




	//�Ʒ�
	//˵����
	//�Ʒ�ϵͳ�ڽ��ܵ�iKeeperϵͳ�ѿ�����Ϣ�󣬵��ýӿڣ�֪ͨikeeperϵͳ���ȴ����سɹ����ٸ��û������Ʒѣ�������ݺ�������ֵ��ʾ����
	//����˵����
	//char * pszikICNo	�û��İ󶨿���ţ�����Ϊ20λ����û����Ϊ��ֵ
	//char * pszikIDNo	�û���ʵ�����ţ�����Ϊ20λ������Ϊ��ֵ
	//char * pszPwd		ʵ�������룬����Ϊ15λ������Ϊ��ֵ
	//char * pszikIP	�ͻ���IP��ַ����ʽ�磺192.168.0.1
	//char * pszDeductDotSign	�Ƿ�۵�ı�־  0�����۵�  1���۵�
	//char * pszCurTime	��ǰʱ��
	//char * pszCashMacNum ��ǰ�����˱�ţ���Ϊ��ֵ
	//pszCurTime 14λ����ʽΪ��4λ�꣬2λ�£�2λ�գ�2λСʱ��2λ���ӣ�2λ��
	//���磺20071107123055->2007��11��7�գ�12��30��55��
	typedef ID_LOGINRETURN (__stdcall *FUN_V2_OpenUserEx)(char * pszikICNo, char * pszikIDNo, char * pszPwd, char * pszikIP, char * pszDeductDotSign, char * pszCurTime, char * pszCashMacNum);



	//����
	//˵����
	//�Ʒ�ϵͳ�ڷ���˽���ʱ��֪ͨikeeperϵͳ��������Ϣ���ӿڡ�
	//����˵����
	//char * pszikICNo	�û��İ󶨿���ţ�����Ϊ20λ����û����Ϊ��ֵ
	//char * pszikIDNo	�û���ʵ�����ţ�����Ϊ20λ������Ϊ��ֵ
	//char * pszikIP	�ͻ���IP��ַ����ʽ�磺192.168.0.1
	//char * pszikHost	�ͻ�������
	//char * pszCurTime	��ǰʱ��
	//pszCurTime 14λ����ʽΪ��4λ�꣬2λ�£�2λ�գ�2λСʱ��2λ���ӣ�2λ��
	//���磺20071107123055->2007��11��7�գ�12��30��55��
	typedef ID_RETURN (__stdcall *FUN_V2_CloseUser)(char * pszikICNo, char * pszikIDNo, char * pszikIP, char * pszikHost, char * pszCurTime);


	//
	//�ϻ�
	//˵����
	//�Ʒ�ϵͳ���ն��ϻ������ýӿڣ�֪ͨikeeperϵͳ���ȴ����سɹ����ٸ��û������ϻ���������ݺ�������ֵ��ʾ����
	//
	//����˵����
	//char * pszikICNo	�û��İ󶨿���ţ�����Ϊ20λ����û����Ϊ��ֵ
	//char * pszikIDNo	�û���ʵ�����ţ�����Ϊ20λ������Ϊ��ֵ
	//char * pszikIP	�ͻ���IP��ַ����ʽ�磺192.168.0.1
	typedef ID_RETURN (__stdcall *FUN_V2_Login)(char * pszikICNo, char * pszikIDNo, char * pszikIP);




	//�ϻ��������������
	//˵����
	//�Ʒ�ϵͳ���ն��ϻ������ýӿڣ�֪ͨikeeperϵͳ���ȴ����سɹ����ٸ��û������ϻ���������ݺ�������ֵ��ʾ����
	//
	//����˵����
	//char * pszikICNo	�û��İ󶨿���ţ�����Ϊ20λ����û����Ϊ��ֵ
	//char * pszikIDNo	�û���ʵ�����ţ�����Ϊ20λ������Ϊ��ֵ
	//char * pszPwd		ʵ�������룬����Ϊ15λ������Ϊ��ֵ
	//char * pszikIP	�ͻ���IP��ַ����ʽ�磺192.168.0.1
	typedef ID_LOGINRETURN (__stdcall *FUN_V2_LoginEx)(char * pszikICNo, char * pszikIDNo, char * pszPwd, char * pszikIP);


	//�»�
	//˵����
	//�Ʒ�ϵͳ���»�ʱ��֪ͨikeeperϵͳ��������Ϣ���ӿڡ�
	//
	//
	//����˵����
	//char * pszikICNo	�û��İ󶨿���ţ�����Ϊ20λ����û����Ϊ��ֵ
	//char * pszikIDNo	�û���ʵ�����ţ�����Ϊ20λ������Ϊ��ֵ
	//char * pszikIP	�ͻ���IP��ַ����ʽ�磺192.168.0.1
	typedef ID_RETURN (__stdcall *FUN_V2_Logout)(char * pszikICNo, char * pszikIDNo, char * pszikIP);



	//������ӻ�Ա
	//˵����
	//�Ʒ�ϵͳ��ʵ��ˢ����ͨ��ʵ����̨��֤�ɹ���ʵ�����������û���Ϊ��Ա��֪ͨikeeperϵͳ��������Ϣ���ӿ�
	//
	//
	//����˵����
	//char * pszikICNo		�û��İ󶨿���ţ�����Ϊ20λ
	//char * pszikIDNo		�û���ʵ�����ţ�����Ϊ20λ
	//char * pszicPassword	ʵ���������룬����Ϊ15λ
	//
	//����ֵ��
	//����ʵ������
	typedef ID_ADDINSIDR (__stdcall *FUN_V2_AddInsider)(char * pszikICNo, char * pszikIDNo, char * pszicPassword);


	//����
	//˵����
	//���ɼƷ��ն���ʵ���󶨳��ֲ����ϵ�����£����ýӿ�֪ͨikeeperϵͳ�����ն������������ݺ�������ֵ��ʾ����
	//
	//����˵����
	//char * pszikICNo;	//�û��İ󶨿���ţ�������д
	//char * pszikIP;  	//�򿪿ͻ�����IP��ַ��������д
	//char * pszikHost;	//���û��ͻ����ļ����������Ϊ��
	typedef ID_RETURN (__stdcall *FUN_V2_LockMac)(char * pszikICNo, char * pszikIP, char * pszikHost);



	//���Ͱ󶨿�����
	//˵����
	//�Ʒ�ϵͳ���յ� IK_ID_GETIDCARDDATA ������Ϣ����WM_COPYDATA��Ϣ����ǰ�����󶨿������ݷ��͸�ikeeperϵͳ
	//
	//����˵����
	//IC_CARDDATA * pstICCardData		�û��İ󶨿�����
	typedef ID_RETURN (__stdcall *FUN_V2_SendICCardData)(IC_CARDDATA * pstICCardData);


	//���Ϳ۵����ݲ�ѯ
	//˵����
	//�Ʒ�ϵͳ�ڲ�ѯ�۵�����ʱ��֪ͨikeeperϵͳ��������Ϣ���ӿڡ�
	//
	//
	//����˵����
	//IC_DEDUCT * pstDeductData    ��ѯ����
	//
	//����ֵ��
	//-1��ʧ��
	//�Ǹ���ֵ����۳�����
	typedef int (__stdcall *FUN_V2_SendDeductData)(IC_DEDUCT * pstDeductData);

	//У�鿨���Ƿ���ȷ
	//˵����
	//У��37λ��39λ�ſ���10λID���Ƿ�Ϊ�����ÿ���У��ɹ���pszAvailableCard�����з�����ȷ���š�
	//
	//����˵����
	//pszSourceCard			��У���Դ����(37��39λ�ſ���10λID��)
	//pszAvailableCard		����У����ȷ�Ŀ���(10λ����)
	//
	//����ֵ���Ƿ�У��ɹ�
	//FALSE��ʧ��
	//TURE���ɹ�
	typedef BOOL (__stdcall *FUN_V2_VerifyCard)(char * pszSourceCard, char * pszAvailableCard);

	//��ȡʵ��������
	//˵����
	//��ȡָ��ʵ�����ڱ����ݴ�����룬������ΪMD5����ֵ��
	//
	//
	//����˵����
	//char * pszikIDNo		�û���ʵ�����ţ�����Ϊ20λ
	//
	//����ֵ��
	//���ؽ������
	typedef ID_GETCARDPWD (__stdcall *FUN_V2_GetCardPWD)(char * pszikIDNo);


	//��ȡ��̨�������ϵ�ʵ��������
	//˵����
	//��ȡָ��ʵ�����ں�̨��������������룬������ΪMD5����ֵ��
	//
	//����˵����
	//char * pszikIDNo		�û���ʵ�����ţ�����Ϊ20λ
	//
	//����ֵ��
	//���ؽ������
	//
	//�ӿڶ��壺
	typedef ID_GETCARDPWD (__stdcall *FUN_V2_GetCardPWDEx)(char * pszikIDNo);

	//��ȡʵ����Ϣ
	//˵����
	//�Ʒ�ϵͳ����Ҫ��֤���룬�����ȡʵ����Ϣ��
	//
	//
	//����˵����
	//char * pszikICNo		�û��İ󶨿���ţ�����Ϊ20λ
	//char * pszikIDNo		�û���ʵ�����ţ�����Ϊ20λ
	//
	//����ֵ��
	//����ʵ������
	typedef ID_ADDINSIDR (__stdcall *FUN_V2_GetCustomer)(char * pszikICNo, char * pszikIDNo);

	//��ȡ�ֻ�ˢ����������֤��
	//˵����
	//ʹ���ֻ�ˢ������ʱ����Ҫ��ȡ�������֤�룬���ö��ŷ��͵�ָ�����ֻ������С�
	//
	//����˵����
	//pszMobile				�ֻ���
	//pszSMSSource			��֤�������Դ
	//0:�ն˻���ȡ
	//1:�Ʒѳ�����Ҽ��˵���ȡ
	//2:�Ʒѳ���ˢ��ȷ��ʱ��ȡ
	//
	//����ֵ�����Ͷ����Ƿ�ɹ�
	//FALSE��ʧ��
	//TURE���ɹ�
	typedef BOOL (__stdcall *FUN_V2_GetMobileVerificationCode)(char * pszMobile, char * pszSMSSource);


	//��ȡ��֤�뷢�ʹ���ֵ
	//˵����
	//������֤��󣬻�ȡ���ͷ���ֵ
	//
	//����˵����
	//pszMobile				�ֻ���
	//����ֵ�����Ͷ��ŷ���ֵ�����д����򷵻������Ĵ�������
	typedef ID_RETURNS (__stdcall *FUN_V2_GetMobileVerificationCodeReturn)(char * pszMobile);

	//��ȡ����Ʒ�������ʺ�
	//˵����
	//��ȡ����Ʒ�������ʺ����ݡ� 
	//
	//����˵����
	//pszAccounts				�ʺ�
	typedef ID_RETURNS (__stdcall *FUN_V2_WXAccounts) (char * pszAccounts);



	//��ȡKEYʣ�����
	//˵����
	//��ȡKEY�е�ʣ������� 
	typedef ID_KEYBALANCERETURN (__stdcall *FUN_V2_KEYBalance) ();


	//ʹ���ն����ݼ��
	//˵����
	//�Ʒѳ����Ѿ���¼����ʹ�õ��ն˵���Ϣ�������ݽṹ�б�ķ�ʽ���͵���ȫ������Խ������ݼ�顣 
	//����˵����
	//pstUseMacVerfy		ʹ���ն˵���Ϣ����Ϊ��������
	//iNum				�ն���Ϣ����
	typedef ID_KEYBALANCERETURN (__stdcall *FUN_V2_UseMacVerfy) (ID_USEMACVERIFY * pstUseMacVerfy, int iNum);

public:
	BOOL Load();
public:
	FUN_V2_OpenUserEx						m_pfn_V2_OpenUserEx;
	FUN_V2_Login							m_pfn_V2_Login;
	FUN_V2_LoginEx							m_pfn_V2_LoginEx;
	FUN_V2_Logout							m_pfn_V2_Logout;
	FUN_V2_AddInsider						m_pfn_V2_AddInsider;
	FUN_V2_LockMac							m_pfn_V2_LockMac;
	FUN_V2_SendICCardData					m_pfn_V2_SendICCardData;
	FUN_V2_SendDeductData					m_pfn_V2_SendDeductData;
	FUN_V2_VerifyCard						m_pfn_V2_VerifyCard;
	FUN_V2_GetCardPWD						m_pfn_V2_GetCardPWD;
	FUN_V2_GetCardPWDEx						m_pfn_V2_GetCardPWDEx;
	FUN_V2_GetCustomer						m_pfn_V2_GetCustomer;
	FUN_V2_GetMobileVerificationCode		m_pfn_V2_GetMobileVerificationCode;
	FUN_V2_GetMobileVerificationCodeReturn	m_pfn_V2_GetMobileVerificationCodeReturn;
	FUN_V2_WXAccounts						m_pfn_V2_WXAccounts;
	FUN_V2_KEYBalance						m_pfn_V2_KEYBalance;
	FUN_V2_UseMacVerfy						m_pfn_V2_UseMacVerfy;
	FUN_V2_SetCRC							m_pfn_V2_SetCRC;
	FUN_V2_CloseUser						m_pfn_V2_CloseUser;
	
	
	BOOL IsInit()	{ return m_bInitSuccess; }
	BOOL IsLoad()	{ return m_bLoadSuccess; }
	void SetInit(BOOL newVal) { m_bInitSuccess = newVal; }

	// ����ת������
	static void Conver_IC_USER( const IC_USER* pOrg, T_IC_USER *pDest );
	static void Conver_IC_LOGIN(IC_LOGIN* pOrg, T_IC_LOGIN *pDest );

	CIZd::ID_LOGINRETURN V2_LoginEx( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szPwd, LPCSTR szIP);
	CIZd::ID_RETURN V2_Logout( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szIP);
	CIZd::ID_RETURN V2_CloseUser( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szIP, LPCSTR szTermId);
	// ���һ�������ǿ۵��־ 0�����۵�  1���۵�
	CIZd::ID_LOGINRETURN V2_OpenUserEx( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szPwd, LPCSTR szIP, LPCSTR szDeductDotSign);
	CIZd::ID_ADDINSIDR V2_AddInsider( LPCSTR szICNO, LPCSTR szIDNO, LPCSTR szPwd);
	CIZd::ID_GETCARDPWD V2_GetCardPWDEx(LPCSTR szIDNO);
	bool IsIDPwdCorrect(LPCSTR szID, LPCSTR szPwd);
	static int SwitchZd2Ddn(int nSwitch);
private:
	HMODULE m_hIKCardID;
	BOOL m_bLoadSuccess;
	BOOL m_bInitSuccess;
	static std::map<int, std::string> m_mapI2S;
	void GetErrorString(int nError, std::string& strError);
	void FmtErrorString();
	bool IsErrorCodeExist(int nError);
	void DealErrorMsg(int nError, char *szError);
	static std::string Encode(LPCSTR szPWD);
	
};
