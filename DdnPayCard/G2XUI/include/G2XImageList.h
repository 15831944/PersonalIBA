#pragma once

G2XUI_BEGIN

class G2X_CLASSES CG2XImageList
{
public:
	CG2XImageList();
	virtual ~CG2XImageList();

	//- �������ͼ��
	void Delete();

	//---------------------------------------------------------------------
	//- ����
	//- nIDRes: ��ԴID
	//- nImgWidth: ��ͼ��Ŀ��, ͼ���б����ͼ�����=ͼ��Ŀ��/nImgWidth, ����ȡ��
	//- lpszResType: λͼָ��ΪRT_BITMAP,�Զ������Ҫָ������,����:"PNG"
	//---------------------------------------------------------------------
	BOOL Create(UINT nIDRes, INT nImgWidth, LPCTSTR lpszResType=RT_BITMAP, BOOL bHorizontal=TRUE);

	//---------------------------------------------------------------------
	//- ����
	//- lpstrImgPath: ͼ���·��
	//- nImgWidth: ��ͼ��Ŀ��, ͼ���б����ͼ�����=ͼ��Ŀ��/nImgWidth, ����ȡ��
	//---------------------------------------------------------------------
	BOOL Create(LPCTSTR lpstrImgPath, INT nImgWidth, BOOL bHorizontal=TRUE);

	//---------------------------------------------------------------------
	//- ����
	//- img: ͼ�����
	//- nImgWidth: ��ͼ��Ŀ��, ͼ���б����ͼ�����=ͼ��Ŀ��/nImgWidth, ����ȡ��
	//---------------------------------------------------------------------
	BOOL Create(Image& img, INT nImgWidth,  BOOL bHorizontal=TRUE);
	
	//- ������ͼ���б��¡
	BOOL CreateFrom(const CG2XImageList& imgList);
	
	//- ׷��һ����ͼ��
	BOOL AddImage(LPCTSTR lpwstrPath);

	//- ׷��һ����ͼ��
	BOOL AddImage(Image& img);

	//- ��ȡ��һ��ͼ��Ĵ�С
	SIZE GetImgSize() const;

	//- ��ȡĳ��ͼ��Ĵ�С
	SIZE GetImgSize(INT iImage) const;

	//- �õ���ͼ��ĸ���
	INT GetImgCount() const;

	//- �Ƿ�����ͼ��
	BOOL HasImage();

	//- ������ͼ��
	void DrawImg(Gdiplus::Graphics* pGraphics, INT nIndex, INT nDesX, INT nDesY);

	//- ������ͼ��
	void DrawImg(Gdiplus::Graphics* pGraphics, INT nIndex, LPRECT lpRectDes, BOOL bGray=FALSE);

	//- ����ͼ���͸��ɫ, �ǵĻ���ͼ��ʱ��������͸��ɫָ������ɫ��
	void SetTranparentColor(ARGB clr);


	std::vector<Gdiplus::Bitmap*> m_Images;
};

G2X_END