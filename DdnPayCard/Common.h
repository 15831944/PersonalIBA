#pragma once

enum E_WNDMODE
{
	e_GeneralMode	= 1, //��ͨ����
	e_QQMode = 2	//	QQ����

};

typedef struct Tips_ShowParam
{
	size_t length; //����ṹ��Ĵ�С

	INT    nPublicID;
	
	SYSTEMTIME startTime; //
	SYSTEMTIME endTime; //
	
	INT		nShowWay;			//����������˸
	BOOL    bFullScreenShow;	//ȫ���Ƿ񵯳�+

	INT     nDelayDisplayTime;      //��¼�Ժ��ӳ���ʾʱ��
	INT     nDelayDisappearTime;	//������ʱ�� 

                                //������û���ݵ�����
	INT     nContentType;       //�������ͣ�URL���ӻ����ļ�����
	RECT    DistanceRect; //������߽���������

	E_WNDMODE eWndMode;     
}TIPS_SHOWPARAM, *PTIPS_SHOWPARAM;


typedef struct Tips_ManagerParam
{
	size_t length; //����ṹ��Ĵ�С


}TIPS_MANAGERPARAM, *PTIPS_MANAGERPARAM;