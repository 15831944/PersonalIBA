#pragma once

#include "tts\SpeechVoice.h"
// CSetPageOfTTS �Ի���

class CSetPageOfTTS : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfTTS)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfTTS();
	virtual ~CSetPageOfTTS();

// �Ի�������
	enum { IDD = IDD_SET_TTS };

private:

	BOOL m_bEnableTTS;
	
	CString m_strText;

	CBCGPComboBox m_cboTTS;
	
	CBCGPSliderCtrl m_sldVol;

	CSpeechVoice m_SpeechVoice;

	BOOL m_bAutoReturn; // �Զ��˿����

protected:

	BOOL IsTTSInstalled();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonTts();
	afx_msg void OnBnClickedCheckTts();
	
public:
	BOOL m_bReminderBalance;
};
