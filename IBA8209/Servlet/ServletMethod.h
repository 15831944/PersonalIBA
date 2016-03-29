#pragma once

#include "RespField.h"

using namespace NS_SERVLET::NS_SERVLET_HELPPER;

class CCashier;
class CMainFrame;
class CLocalCashier;
class CThreadHelpper;

namespace NS_SERVLET
{

enum ECENTER
{
	E_LOCAL_CENTER = 0,     //��������
	E_MAIN_CENTER = 1,      //������
	E_BACKUP_CENTER = 2,    //��������
	E_OTHER_CENTER = 3,     //��������(�������)
	E_QUERY_CENTER = 4,     //����ѯ��
	E_QUERY_CENTER2 = 5     //����ѯ��
};

enum ECARDTYPE
{
	ECT_IC,
	ECT_ID
};

enum ESTATUSCODE
{
	ESC_WXYH1        = 1,  //!<1��Ч�û�
	ESC_WXWB2        = 2,  //!<2��Ч����
	ESC_QXDCYH3      = 3,  //!<3���ȵǳ��û�
	ESC_WXBS4        = 4,  //!<4��Ч��ʱ
	ESC_GKYGS5       = 5,  //!<5�ÿ��ѹ�ʧ
	ESC_ZHYEBZ6      = 6,  //!<6�˻�����
	ESC_LSHC7        = 7,  //!<7��ˮ�Ų�ƥ��
	ESC_WUYHJB8      = 8,  //!<8��Ч�û�����
	ESC_YJBYZ9       = 9,  //!<9Ѻ��һ��
	ESC_BFHZBS10     = 10, //!<10������ת��ʱ
	ESC_WSJJH11      = 11, //!<11δ�����ϻ�����
	ESC_YHBZX12      = 12, //!<12�û�������
	ESC_ZJHC13       = 13, //!<13֤�����벻ƥ��
	ESC_WQXF14       = 14, //!<14��Ȩ����
	ESC_WXMM15       = 15, //!<15��Ч����
	ESC_CKHYBSY16    = 16, //!<16�˿����ѱ�ʹ��
	ESC_KNXXBYZ17    = 17, //!<17������Ϣ��һ��
	ESC_KMYBQP18     = 18, //!<18����Կ��ƥ��
	ESC_YHYZX19      = 19, //!<19�û���ע��
	ESC_JEWX20       = 20, //!<20�����Ч
	ESC_YHFSTZ21     = 21, //!<21�û�����͸֧
	ESC_YHMYGS22     = 22, //!<22�û�û�й�ʧ
	ESC_FWQCSC23     = 23, //!<23������������
	ESC_KKJEBZ24     = 24, //!<24��������
	ESC_GKBSDJK25    = 25, //!<25�ÿ����Ƕ��Ῠ
	ESC_WXBFQQ26     = 26, //!<26��Ч��������
	ESC_CGCZYXSJ27   = 27, //!<27������������ʱ��
	ESC_FXWBSX28     = 28, //!<28����������Ч
	ESC_WXZDH29      = 29, //!<29��Ч�ն˺�
	ESC_MACJYC30     = 30, //!<30MACУ���
	ESC_YHYDC31      = 31, //!<31�û��ѵǳ�
	ESC_QGHDYKJB32   = 32, //!<32�������Ӧ����Ŀ�
	ESC_GYHJBBYXTK33 = 33, //!<33���û����������˿�
	ESC_WXKBCH34     = 34, //!<34��Ч�����κ�
	ESC_WXCZKLB35    = 35, //!<35��Ч��ֵ�����
	ESC_JQQYBYZ36    = 36, //!<36��������һ��
	ESC_WJHHYCZ37    = 37, //!<37δ������ѳ�ֵ
	ESC_CZKGQ38      = 38, //!<38��ֵ���ѹ���
	ESC_GKYDJ39      = 39, //!<39�ÿ��Ѷ���
	ESC_CKWZC40      = 40, //!<40�˿�δע��
	ESC_KHBCZ41      = 41, //!<41���Ų�����
	ESC_WXLSLX42     = 42, //!<42��Ч��������
	ESC_WXZFLX43     = 43, //!<43��Ч֧������
	ESC_CKBCZ44      = 44, //!<44�ֿⲻ����
	ESC_WXZCID45     = 45, //!<45��Ч�ʲ�ID
	ESC_WQXF46       = 46, //!<46��Ȩ����
	ESC_KHCW47       = 47, //!<47���Ŵ�λ
	ESC_YSHCW48      = 48, //!<48ӡˢ�Ŵ�λ
	ESC_YHHCW49      = 49, //!<49�û��Ŵ�λ
	ESC_WXSYTID50    = 50, //!<50��Ч����̨ID
	ESC_WXDKQ51      = 51, //!<51��Ч������
	ESC_WXGJSJ54     = 54, //!<54��Ч�һ�ʱ��
	ESC_GJWJS55      = 55, //!<55�һ�δ����
	ESC_WBWBCSJ56    = 56, //!<56����δ��������
	ESC_DQSDBYX57    = 57, //!<57��ǰʱ�β�����
	ESC_BSYJS58      = 58, //!<58��ʱ�ѽ���
	ESC_BSQYSX59     = 59, //!<59��ʱ��������
	ESC_YJBS60       = 60, //!<60���Ѿ���ʱ
	ESC_WXYWLX61     = 61, //!<61��Чҵ������
	ESC_WXZHLX62     = 62, //!<62��Ч�˻�����
	ESC_YHXXWX63     = 63, //!<63�û���Ϣ��Ч
	ESC_YJYXXBCZ64   = 64, //!<64ԭ������Ϣ������
	ESC_XMBNWYW65    = 65, //!<65��������ΪӢ���ַ�
	ESC_ZJHMCF66     = 66, //!<66֤�������ظ�
	ESC_KKSXCX67     = 67, //!<67������������
	ESC_YHBSSM68     = 68, //!<68�û�����ʵ��
	ESC_SJQYSX69     = 69, //!<69�ϻ���������
	ESC_WBDSMK70     = 70, //!<70δ��ʵ����
	ESC_BSRSYM71     = 71, //!<71��ʱ��������
	ESC_YJSB72       = 72, //!<72�Ѿ��ϰ�
	ESC_SWSB73       = 73, //!<73��δ�ϰ�
	ESC_BCCS74       = 74, //!<74��γ�ʱ
	ESC_YHHKYZF75    = 75, //!<75�û���������
	ESC_QXFBSXX76    = 76, //!<76���޸���ʱ��Ϣ
	ESC_QSHZC77      = 77, //!<77���Ժ�����
	ESC_JZKK78       = 78, //!<78��ֹ����
	ESC_SMXLHWX79    = 79, //!<79ʵ�����к���Ч
	ESC_SMSZBQP80    = 80, //!<80ʵ�����ò�ƥ��
	ESC_SMXLHYCZ81   = 81, //!<81ʵ�����к��Ѵ���
	ESC_SHYZX82      = 82, //!<82ʵ��������
	ESC_ZJLXBYX83    = 83, //!<83֤�����Ͳ�����
	ESC_JYSJGQ84     = 84, //!<84����ʱ�����
	ESC_ZXZDCF85     = 85, //!<85�����ն��ظ�
	ESC_CZJETD86     = 86, //!<86��ֵ���̫��
	ESC_CKJZSY87     = 87, //!<87�˿���ֹʹ��
	ESC_SYTYDQ88     = 88, //!<88����̨�ѵ���
	ESC_ZZYEBZ89     = 89, //!<89��ֵ��Ȳ���
	ESC_DHKFCC90     = 90, //!<90�绰�۷ѳ���
	ESC_GNBZC91      = 91, //!<91���ܲ�֧��
	ESC_SYTBBD92     = 92, //!<92����̨�汾̫��
	ESC_JLZHBCZ96    = 96, //!<96�����˻�������
	ESC_WXBW97       = 97, //!<97��Ч����
	ESC_SJKCW98      = 98, //!<98���ݿ����
	ESC_QQSB99       = 99, //!<99����ʧ��
	ESC_QT100        = 100,//!<100����
	ESC_QT101        = 101 //!<101����1
};

class CServletMethod
{
protected :
	CServletMethod(LPCTSTR lpszMethodName);
	virtual ~CServletMethod(void);

public :

	friend class CCashier;
	friend class CLocalCashier;
	friend class CThreadHelpper;

	INT GetStatusCode();				//!< ���ķ��ص�״̬��

	CString GetStatusMessage();			//!< ���ķ��ص�״̬���Ӧ���ַ�����Ϣ

	void SetPSK(CString newVal);		//!< ���ü�����Կ
	
	CString GetPSK() const;				//!< ��ȡ������Կ

	ECARDTYPE GetCardType() const;		//!< ��ȡ��ǰ������

	void SetCardType(ECARDTYPE newVal); //!< ���ÿ�����

	UINT GetSerialNo();					//!< ��ȡ���ķ��ص���ˮ��

	UINT GetLocalSerialNo() const;		//!< ��ȡ���ز����ı�����ˮ��

	CString GetUrlFromIndex(UINT nIndex) ;
protected :

	BOOL Execute();						//!< ִ������
	void AddParams();					//!< ��Ӳ���������ɸ���
	void AddRespFields();				//!< ��ӷ����ֶΣ�����ɸ���
	CString MakeParamsString();			//!< ƴ�Ӳ���������
	void WriteLog(LPCTSTR lpszInfo);	//!< д��־�ļ�

protected :
	
	virtual CString GetMethodName();					//!< ���ص�ǰ�������ƣ�����д
	virtual CString GetRequestAddr(UINT nIndex);		//!< ��ȡ���ĵ�ַ������д
	virtual CString MergeParams();						//!< �ϲ�����������д
	virtual CString EncodeParams(CString strParams);	//!< ���ܲ���������д
	virtual UINT	MakeLocalSerialNo();				//!< ��ȡ������ˮ��
	virtual CString MakeMacString(CString strParams);	//!< ���ɲ���MAC������д
	virtual BOOL	VerifyResp(CString strResp);		//!< У�鷵�ؼ������ݣ�����д
	virtual BOOL    DecodeResp(CString strResp);		//!< ��������ֵ������д
	virtual BOOL	GetHttpData(LPCTSTR lpszURL, CString& strResp);	//!< �����Ľ���������д
	virtual BOOL	GetCenterData(CString strParams, CString& strResp);
	virtual BOOL	OnBegin();							//!< ִ������ǰ
	virtual void	OnEnd(BOOL bRet);					//!< ִ�������

protected :
	
	CInputParamList m_Params;		//!< �����б�
    CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	const CString m_strMethodName;	//!< ��������
	CString m_strPSK;				//!< ��Կ
	
	ECARDTYPE m_nCardType;			//!< ������

	UINT m_nLocalSerialNo;			//!< ������ˮ��

protected:

	UINT m_nNetbarId;				//!< ����ID

	BOOL m_bUseQueryHost;			//!< ʹ�ò�ѯ��

	UINT m_nRevTimeout;				//!< ���ճ�ʱ(��λ:��)

	static UINT m_nFirstCenterIndex;		//!< �������ӵ�����

	static UINT m_nFirstQueryCenterIndex;	//!< �������ӵĲ�ѯ����

	friend CMainFrame;
};

}