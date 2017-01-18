#ifndef BITIMAGEUTILS
#define BITIMAGEUTILS

#define BITIMAGEUTILS_NO_ERROR 0
#define BITIMAGEUTILS_BUFFER_ERROR 1
#define BITIMAGEUTILS_LZO_INIT_ERROR 2
#define BITIMAGEUTILS_LZO_COMPRESS_ERROR 3
#define BITIMAGEUTILS_LZO_DECOMPRESS_ERROR 4


#include "stdafx.h"

/*�ýṹ���ڸ�����йܴ�����н�����ʹ��*/
typedef struct{
	//��ʾλͼ����
	BYTE* out;
	//��ʾλͼ��С
	unsigned int size;
	unsigned int src_size;
} BitMapInfo, *LBitMapInfo;

/*����ѹ��*/
int BitMapCompress(BYTE* in, BYTE* out, ULONG inLen, ULONG* outLen);
/*����ѹ��*/
BYTE* BitMapCompress(BYTE* in, ULONG inLen, ULONG* outLen, int* error);
/*���ݽ�ѹ��*/
int BitMapDecompress(BYTE* in, BYTE* out, ULONG inLen, ULONG* outLen);
/*���ݽ�ѹ��*/
BYTE* BitMapDecompress(BYTE* in, ULONG inLen, ULONG* outLen, int* error);


/*�÷������ڽ���Ļ�ض������ͼ��HBITMAP*/
HBITMAP CopyScreenToBitmap(LPRECT lpRect);

/*�÷������ڽ�HBITMAPװ���ڴ�*/
int SaveBitmapToMemory(HBITMAP hBitmap, LBitMapInfo image, BOOL isCompress = false);

#endif