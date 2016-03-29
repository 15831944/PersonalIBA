#pragma once

#include "..\Socket\BufferProcess.h"

namespace NS_REALNAME
{
	const int LEN_ID				= 30;
	const int LEN_NAME				= 50;
	const int LEN_IMAGE				= 65536 - 4096;

	const unsigned char COM_VER_1	    = 0x01;

	const unsigned char C_REGINFO		= 0x06;		//�ϴ��û�ע����Ϣ
	const unsigned char C_CAPIMG		= 0x07;		//�ϴ��û�ע��ͼƬ
	const unsigned char C_IDIMG			= 0x08;		//�ϴ��û�֤��ͼƬ

#pragma pack(push, 1)

	typedef struct _tag_id_img
	{
		char idtype;					//֤������
		char IdNumber[LEN_ID + 1];		//֤����
		char name[LEN_NAME + 1];		//����
		int imageSize;					//ͼ���С
		char image[LEN_IMAGE];			//ͼ��
	}ID_IMAGE;

#pragma pack(pop)

}