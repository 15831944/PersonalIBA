#pragma once

G2XUI_BEGIN

//-------------------------------------------------------------------------
//- CG2XDrawMask
//
//- ��ͼ�ɰ�, ��ν�ɰ���ָ�����������͸���ȵ�λͼ��
//- ��λͼ��ָ��ͼ�����Alpha��ϻ�����ض���Ч��
//- ����: Foxmail�ı������ı����վ���һ���Ľ������ʵ���ϱ�����ֻ��һ����ɫ,Foxmail
//-    �ı�����ʱ��ֻ��������ɫ�����ɰ��������κθı䣬ȴ���Բ���һ������Ч���ı�����
//- ����:
//     QQ���ԹܼҵĹ�����������껬����ťʱ�����ֵİ�͸��Բ�Ǿ���Ч���������ﶼ����Ϊ���ɰ�
//-------------------------------------------------------------------------
class G2X_CLASSES CG2XDrawMask
{
protected:
	// ������Ĭ�ϴ���
	CG2XDrawMask();

public:
	virtual ~CG2XDrawMask();

public:

	//---------------------------------------------------------------------
	//- ����һ���������ȵľ����ɰ�
	//-		˵��: �뱳����Ϻ󣬱������ĸ���(��������clrMaskָ������ɫ)
	//
	//- nWidth:  �ɰ�Ŀ��
	//- nHeight: �ɰ�ĸ߶�
	//- nBrightness: Ҫ���ӵ�����, ȡֵ��Χ0~100
	//- clrMask: �������Ⱥ�ͼ��ƫ�����ɫ��Ĭ�ϰ�ɫ�������
	//---------------------------------------------------------------------
	static CG2XDrawMask* CreateMaskIncreaseBrightness(
		INT nWidth,
		INT nHeight,
		BYTE nBrightness,
		COLORREF clrMask=RGB(255,255,255));

	//---------------------------------------------------------------------
	//- ����һ���������ȵľ����ɰ�
	//-		˵��: �뱳����Ϻ󣬱������ĸ���(��������clrMaskָ������ɫ)
	//
	//- nWidth: �ɰ�Ŀ��
	//- nHeight: �ɰ�ĸ߶�
	//- nBrightness: Ҫ���͵�����, ȡֵ��Χ0~100
	//- clrMask: �������Ⱥ�ͼ��ƫ�����ɫ
	//---------------------------------------------------------------------
	static CG2XDrawMask* CreateMaskDecreaseBrightness(
		INT nWidth,
		INT nHeight,
		BYTE nBrightness,
		COLORREF clrMask=RGB(0,0,0));

	//---------------------------------------------------------------------
	//- �����������ȵ��ɰ�,ͼ����clrMask����仯
	//- style: ������,(G2XGradientStyle�Ķ��壬��֪���Ȳ����Ĵ���)
	//- nWidth: �ɰ�Ŀ��
	//- nHeight: �ɰ�ĸ߶�
	//- nBrightness1: �������ʼ����
	//- nBrightness2: ������м����Ȼ�����ֹ����
	//- nBrightness3: ������м����Ȼ�����Ч
	//- nBrightness4: �������ֹ���Ȼ�����Ч
	//- clrMask: �ɰ����ɫ
	//---------------------------------------------------------------------
	static CG2XDrawMask* CreateMaskGradientBrightness(
		INT nWidth,
		INT nHeight,
		G2XGradientStyle style,
		BYTE nBrightness1,
		BYTE nBrightness2,
		BYTE nBrightness3,
		BYTE nBrightness4,
		COLORREF clrMask=RGB(255,255,255)
		);

	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	static CG2XDrawMask* CreateMaskRaisedBirghtness(
		INT nWidth,
		INT nHeight,
		BOOL bHorizontal,
		BYTE nBrightness,
		COLORREF clrMask=RGB(255,255,255)
		);

	static CG2XDrawMask* CreateMaskDepressedBirghtness(
		INT nWidth,
		INT nHeight,
		BOOL bHorizontal,
		BYTE nBrightness,
		COLORREF clrMask=RGB(255,255,255)
		);

	//---------------------------------------------------------------------
	//- ��һ��ͼ�񴴽��ɰ�
	//---------------------------------------------------------------------
	static CG2XDrawMask* CreateMaskFromImage(
		Image* pImage);

public:
	//---------------------------------------------------------------------
	//- ��ȡ�ɰ�ͼ��
	//- �Է��ص�ͼ��ӹ����ɰ�Ҳ�ᷢ����Ӧ�ı仯
	//---------------------------------------------------------------------
	Bitmap* GetBitmap();

	//- ��ȡ�ɰ�Ŀ��
	INT GetWidth();

	//- ��ȡ�ɰ�ĸ߶�
	INT GetHeight();

	//---------------------------------------------------------------------
	//- ���ɰ���ʾ��Graphics����
	//- ���ɰ���ȫ��ʾ��ָ����λ��
	//---------------------------------------------------------------------
	void ShowToGraphics(
		Graphics* pGraphics, 
		int nDesX, 
		int nDesY);
	
	//---------------------------------------------------------------------
	//- ���ɰ���ʾ��Graphics����
	//- ��ʾ�ɰ���ض�����ָ����λ��
	//---------------------------------------------------------------------
	void ShowToGraphics(
		Graphics* pGraphics, 
		int nDesX, 
		int nDesY, 
		int nSrcX,
		int nSrcY, 
		int nSrcWidth, 
		int nSrcHeight);

	//---------------------------------------------------------------------
	//- ���ɰ���ʾ��Graphics����
	//- ���ɰ���ʾ��ָ��������
	//---------------------------------------------------------------------
	void ShowToGraphics(
		Graphics* pGraphics, 
		LPCRECT lpDesRect);

	//---------------------------------------------------------------------
	//- ���ɰ���ʾ��Graphics����
	//- ���ɰ���ض�������ʾ��ָ��������
	//---------------------------------------------------------------------
	void ShowToGraphics(
		Graphics* pGraphics, 
		LPCRECT lpDesRect, 
		LPCRECT lpSrcRect);

private:
	Bitmap* m_pBitmap; // λͼ����ָ�� (�ɰ�Ҳ��λͼ)

public:
	

	
	
	
	
	
	

};

G2X_END