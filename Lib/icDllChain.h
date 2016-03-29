/////////////�ڲ�ʹ��////////////////
#define WM_IREADER_MESSAGE	WM_USER+200
#define WM_IREADER_CARDIN	WM_IREADER_MESSAGE+1
#define WM_IREADER_CARDOUT	WM_IREADER_MESSAGE+2

#define CMD_READER_MSG					0xfe
#define CMD_READER_MSG_CARDIN			0x01
#define CMD_READER_MSG_CARDOUT			0x02
/////////////////////////////////////

#define OPT_WRITE_CARDINFO		0x00000001			//д����Ϣ[ӡˢ��|����],��Ա��Ϣ[��Ա��|��������|��Ա����], ������һ
#define OPT_WRITE_TEMP			0x00000002			//д����ʱ��Ϣ,�����ζ�
#define OPT_WRITE_BUNDLE		0x00000004			//д����ʱ��Ϣ,��������
#define OPT_WRITE_CLEAR_TEMP	0x00000008			//�����ʱ��Ϣ
#define OPT_WRITE_CLEAR_BUNDLE	0x00000010			//��հ�ʱ��Ϣ



/////////////////////////////////////
#define CUSTOM_DATA_ADDR		0xb8	//len 18
#define MSG_CARD_CHECK_ERR		0x0D	//��֤�������
//////////////4.9֮����//////////////
#define	MSG_EEPROM_ERR			0x04	//EEPROM ����
#define	MSG_EEPROM_WR_ERR		0x05	//EEPROM д����
#define	MSG_EEPROM_RD_ERR		0x06	//EEPROM ������
#define	MSG_EEPROM_CHECKSUM		0x07	//EEPROM У��ϴ���
////////////////////////////////////

#define	MSG_OK					0x01
#define	MSG_ERR					0x02
#define	MSG_CHECKSUM			0x03	//ͨѶ�е�����У��ϴ���
#define	MSG_RECV_DATA_ERR		0x08	//�������ݶ���������

#define	MSG_CARD_PW_ERR			0x0A	//���������
#define	MSG_CARD_DAMGED			0x0B	//���Ѿ���
#define	MSG_CARD_WR_ERR			0x0C	//��д����
#define MSG_UNKNOW_CARDTYPE		0x0E	//����ʶ��Ŀ�����(�����Ѿ�������)

#define	MSG_CARD_DATA_ERR		0x11	//�����ݴ���
#define	MSG_CARD_DATA_CHECKSUM	0x12	//������У��ϴ���
#define	MSG_NO_CARD				0x13	//�޿�
#define	MSG_NO_TIME				0x14	//δ����ʱ��
#define	MSG_CARD_INVALID		0x15	//����Ч
#define	MSG_CARD_ERR_EXCEED		0x16	//�������Ѿ���������
#define	MSG_CARD_FIX_ERROR		0x17	//���޸�����
#define MSG_MEMBERID_NOT_EQU	0x18	//��Ա�Ų�ƥ��
#define MSG_INVALID_COSTTYPE	0x19	//��Ч�ļƷ�ģʽ
#define	MSG_COPYRIGHT_ERR		0x20	//�ް�Ȩ��Ϣ���Ȩ��Ϣ����

#define	MSG_IC_BUSY				0x21	//������æ����3������
#define	MSG_COMMU_CHECKSUM		0x22	//�����ڴ�������г���
#define	MSG_READER_NORESPOND	0x23	//����������Ӧ
#define	MSG_READER_NOT_INIT		0x24	//������δ��ʼ���ɹ�
#define	MSG_READER_NOT_SUPPORT	0x25	//��������֧�ִ������

#define	MSG_PORT_CANNOTOPEN		0x31	//���ڲ��ܴ�
#define	MSG_PORT_ALREADYOPEN	0x32	//�����Ѿ���

#define	MSG_WRITER_NOT_SUPPORT	0x33	//�������汾��֧�ִ˹���

#define	MSG_DATE_FORMAT_ERROR	0x40	//���ڸ�ʽ����

#define	MSG_PARAMETER_ERR		0x21	//��������
#define	MSG_DATA_LEN_ERR		0x22	//���ݳ��ȴ���

bool rGetCopyright();

BOOL WINAPI wReadIdCard(LPSTR pszIdCardNumber,BOOL bForceRead);			//��ID��

BOOL WINAPI wDes(LPSTR pszSource,
									   LPSTR pszDest,
									   LPSTR pszKey,
									   BOOL	bEncrypt);

ULONG WINAPI wIssue(ULONG	ulMemberID,					//��Ա���
										  UCHAR	ucMemberTypeID,				//��Ա���
										  USHORT usNetbarID,				//�������ɱ��
										  ULONG	ulExpire,					//�ӵ�ǰʱ�������˿���Ч 0 = ����ʱ
										  LPSTR	pszOpPass,					//��������
										  LPSTR	pszCardID,					//��ID
										  LPSTR	pszName,					//�û���
										  UCHAR	ucSex,						//�û��Ա�
										  UCHAR	ucIDType,					//֤�����
										  LPSTR	pszIDNumber);				//֤������
/*
wRead ����: MSG_CARD_DATA_CHECKSUM ʱ��������Ȼ���أ����п����Ǵ���ġ�
��Ҫ���������д���ߵ�����Ӧ�Ľӿ��޸����ݡ�
*/
ULONG WINAPI wRead(
										 LPSTR pszMemberID,					//��Ա���
										 LPSTR pszMemberTypeID,				//��Ա���
										 LPSTR pszNetbarID,					//�������ɱ��
										 LPSTR pszPSK,						//PSK
										 LPSTR pszCardID,					//��ID
										 LPSTR pszFreezeMoney,				//������
										 LPSTR pszUsedMoney,				//ʹ���˶��ٽ��
										 LPSTR pszRemain,					//����ʣ����
										 LPSTR pszRefNo,					//ϵͳ�ο���
										 LPSTR pszSerialNo,					//������ˮ��
										 LPSTR pszCheckinDate,				//����ʱ��
										 LPSTR pszEndUsingDate,				//���ʹ��ʱ��
										 //��ʱ
										 LPSTR pszBundleTimeId,				//��ʱID
										 LPSTR pszBundleNetbarId,			//��ʱ����ID
										 LPSTR pszBundlePcClass,			//��ʱ��������
										 LPSTR pszBundlePayType,			//��ʱ����
										 LPSTR pszBundleAmountTotal,		//��ʱ�ܽ��
										 LPSTR pszBundleAmountUsed,	 		//�����Ѱ�ʱ���
										 LPSTR pszBundleStartTime,			//��ʱ��ʼʱ��
										 LPSTR pszBundleEndTime,			//��ʱ����ʱ��
										 LPSTR pszCardSerialNumber=NULL);	//��ӡˢ��ˮ��,����Ϊ 15 ���ֽ�(����������)��

ULONG WINAPI wEmptyTemp(ULONG	ulMemberID,				//��Ա���
										  LPSTR	pszOpPass,					//��������
										  LPSTR	pszUID);					//��ƬUID

ULONG WINAPI wEmpty(ULONG	ulMemberID,					//��Ա���
										  LPSTR	pszOpPass,					//��������
										  LPSTR	pszUID);					//��ƬUID

ULONG WINAPI wFix(ULONG ulMemberID,					//��Ա���  (δʹ��)
										LPSTR pszOpPass,					//��������  (δʹ��)
										LPSTR pszUID,						//��ƬUID   (δʹ��)
  									    LPSTR pszCardSerialNumber=NULL);	//��д��Ƭӡˢ��ˮ�뵽������ (������ 4.6 �汾����)

ULONG WINAPI wReCalChecksum(ULONG	ulMemberID,			//��Ա���
										LPSTR pszOpPass,					//��������
										LPSTR pszUID,						//��ƬUID
										BOOL  bForce,						//ǿ���޸�
										ULONG ulUsed,						//��ʹ�ý��
										LPSTR pszLastTime);					//���ʱ�� ��ʽ (2004-01-01 00:00:00)

ULONG WINAPI wCalMac(LPSTR pszBuffer,					//���� ��Ҫ���������
										   LPSTR pszMac,					//��� MACУ����	8 * 2 ���ֽ�
										   LPSTR pszKey,					//��� ������Կ		8 * 2 ���ֽ�
										   ULONG ulLenght);					///���� ��Ҫ��������ݵĳ���(ֵΪ NULL ���Զ�����)

void WINAPI wCalMd5(LPSTR pszIn,						//���� ��Ҫ������ַ���
										  LPSTR pszMd5Hash);				//��� HASH	ֵ��32 ���ֽڡ�NULL ������

int WINAPI wSetDate(LPSTR pszNewDate);				//���ڸ�ʽ 20041018200101

VOID rGetMsgByrCode(ULONG ulRetCode,					//��Ҫ���͵ķ���ֵ
										  LPSTR pszRetMessage);				//����ֵ�Ľ����ִ�

ULONG WINAPI  wInit(ULONG ulPort);					//�򿪴��ڲ���ʼ��
VOID WINAPI	wClose(VOID);							//�رմ���
BOOL WINAPI	VerifyIdCardNo(char * pszIdCardNo);		//18λ���֤У�����


BOOL WINAPI	wRewriteBundleInfo(ULONG	ulMemberId,
													   USHORT	usTimeId,		//��ʱID
													   ULONG	ulSerialNo,		//��ʱ�ն���ˮ��
													   ULONG	ulRefNo,		//��ʱϵͳ�ο���
													   USHORT	usNetbarId,		//��ʱ����ID
													   UCHAR	ucPcClass,		//��ʱ��������
													   UCHAR	ucPayType,		//��ʱ����
													   ULONG	ulAmountTotal,	//��ʱ�ܽ��
													   ULONG	ulAmountUsed,	//�����Ѱ�ʱ���
													   ULONG	ulFrozenMoney,	//�����Ѷ�����
													   ULONG	ulStartTime,	//��ʱ��ʼʱ��
													   ULONG	ulEndTime);		//��ʱ����ʱ��

ULONG WINAPI	ConverTime(LPSTR pszDateString);
USHORT WINAPI	wGetVersion();
BOOL WINAPI	wSetRecvWnd(HWND hRecvWindow);
BOOL WINAPI	wRewritePskCardId(const char * pszCardPsk,	//����̬��Կ
													  unsigned long ulCardId);	//��ID
//BOOL WINAPI	wChangeInsertMsg(BOOL bSendMsg);			//����̬��Կ

ULONG WINAPI wWrite42(const UCHAR *pucBuffer, ULONG ulPos, ULONG ulLen);
ULONG WINAPI wRead42(UCHAR *pucBuffer, ULONG ulPos, ULONG ulLen);
ULONG WINAPI wChangeInsertMsg(BOOL bSendInsertMessage);

ULONG WINAPI wReadCustom(char *pszBuffer);				//���Զ������� ���� 16 ���ֽڡ�ע�⻺�������� 16 ���ֽ�
ULONG WINAPI wWriteCustom(const char *pszBuffer);			//д�Զ������� ���� 16 ���ֽڡ�ע�⻺�������� 16 ���ֽ�

/*���÷���
1.�޸� ӡˢ��ˮ��/��ID/��Ա��/��Ա���/�������ɱ��
	pszPrintNo/ulCardID/ulMemberId/ucMemberTypeID/usIssueNetbarID ����һ������ʱ����д��Ϊ�յ�ֵ��
	pszPrintNo ���ַ��������ж�,���ȴ���0ʱ��Ϊ��Чֵ��

2.�޸� [��ʱ�ۿ���Ϣ, ��Ҫ�ṩ���� [�����ζ�] �������ֶ�
3.�޸� [��ʱ��Ϣ],��Ҫ�ṩ [�����ζ�] �� [��������] �������ֶ�, ���� [ucCostType] ����Ϊ 1 or 2, ���޸���ʱ [ucCostType] = 0

*�� pszPrintNo/pszPSK���ַ���ֵ֮�⣬���඼�Ƕ����Ƹ�ʽ
*pszPSK �ֽ׶�δʹ��.

//дѡ��
#define OPT_WRITE_CARDINFO		0x00000001			//д����Ϣ[ӡˢ��|����],��Ա��Ϣ[��Ա��|��������|��Ա����], ������һ
#define OPT_WRITE_TEMP			0x00000002			//д����ʱ��Ϣ,�����ζ�
#define OPT_WRITE_BUNDLE		0x00000004			//д����ʱ��Ϣ,��������
#define OPT_WRITE_CLEAR_TEMP	0x00000008			//�����ʱ��Ϣ
#define OPT_WRITE_CLEAR_BUNDLE	0x00000010			//��հ�ʱ��Ϣ

��: ulWriteOption = OPT_WRITE_CARDINFO | OPT_WRITE_TEMP | OPT_WRITE_BUNDLE

*/
ULONG WINAPI wWriteCardData(ULONG		ulWriteOption,	//дѡ��
												  //������һ,����Ϣ �� OPT_WRITE_CARDINFO ѡ��
												  LPSTR		pszPrintNo,		//ӡˢ��ˮ��
												  ULONG		ulCardID,		//��ID
												  ULONG		ulMemberId,		//��Ա��
												  ULONG		ucMemberTypeID,	//��Ա���
												  ULONG		usIssueNetbarID,//�������ɱ��
												  LPSTR		pszPSK,			//PSK
												  //�����ζ�,��ʱ�ۿ���Ϣ	//�� OPT_WRITE_TEMP ѡ��
												  ULONG		ucCostType,		//�ۿʽ
												  ULONG		usNetbarID,		//�ϻ����ɱ��
												  ULONG		ucPcClass,		//��������
												  ULONG		ulFreezeMoney,	//������
												  ULONG		ulRemainMoney,	//ʣ����
												  ULONG		ulUsedMoney,	//ʹ���˶��ٽ��
												  ULONG		ulRefNo,		//ϵͳ�ο���
												  ULONG		ulSerialNo,		//������ˮ��
												  ULONG		ulStartTime,	//����ʱ��
												  ULONG		ulEndTime,		//���ʹ��ʱ��
												  //��������,��ʱ��Ϣ		//�� OPT_WRITE_BUNDLE ѡ��
												  ULONG		usTimeId,			//��ʱID
												  ULONG		usBundleNetbarId,	//��ʱ����ID
												  ULONG		ucBundlePcClass,	//��ʱ��������
												  ULONG		ucPayType,			//��ʱ����
												  ULONG		ulAmountTotal,		//��ʱ�ܽ��
												  ULONG		ulAmountUsed,		//�����Ѱ�ʱ���
												  ULONG		ulBundleStartTime,	//��ʱ��ʼʱ��
												  ULONG		ulBundleEndTime);	//��ʱ����ʱ��