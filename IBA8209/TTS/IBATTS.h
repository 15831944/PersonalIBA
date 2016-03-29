#pragma once

#include "..\Singleton.h"
#include "..\IniConfig.h"
#include "SpeechVoice.h"

#define _USETTS

namespace NS_TTS
{

class CIBATTS : private CIniConfig
{
	SINGLETON_DECLARE(CIBATTS)

public :

	static CIBATTS* GetInstance();

	BOOL CreateSpeechVoiceObject();

	void SpeakText(LPCTSTR lpszText);

	void SpeakRegisterResult(INT nCredit, INT nPresent);
	void SpeakBundleTimeRegisterResult(INT nBundleTime, INT nBlance);
	void SpeakCreditResult(INT nCredit, INT nPresent, INT nBalance);
	void SpeakReturnResult(INT nReturned);
	void SpeakBundleTimeResult(INT nBundleTimeMoney);
	void SpeakActivation(INT nBalance, BOOL bHaveBirthday=FALSE);
	void SpeakBalance(INT nBalance);
	void SpeakReturn2Result(INT nReturned);
	//��ֵ��ʱ����
	void SpeakCreditBundTimeResult(INT nCredit, INT nPresent, INT nBalance, INT nBundleTimeMoney);

	//��������
	void SpeakUnlock();

	// 2011/07/05-gxx: ת��ʱ�����
	void SpeakBundleTimePatch(INT nMoney);

	// 2011/09/07-8201-gxx: ����
	void SpeakReminderBalance(CString strTermID);

private :

	void LoadString();
	
	CSpeechVoice m_SpeechVoice;

	CString m_strCreditResult;

	CString m_strCreditResultWithPresent;

	CString m_strRegisterResult;

	CString m_strRegisterResultWithPresent;

	CString m_strBundleTimeRegisterResult;

	CString m_strReturnResult;

	CString m_strReturnResult2;

	CString m_strBundleTimeResult;

	CString m_strActivation;
	
	//��ֵ��ʱ
	CString m_strCreditResultWithBundTime;

	CString m_strBalance;

	// 2011/06/17-gxx: ���ӽ��տ��ֵļ�������
	CString m_strActivation2;

	// 
	CString m_strBundleTimePatchMoney; 

	// �����������ѣ� XX�Ż�������
	CString m_strReminderBalance;

};

}