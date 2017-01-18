#include "BitImageUtils.h"
#include "minilzo.h"

#define MAX_LEN 1024
#define COMPRESS_BUFFER_SIZE 10485760
#define HEAP_ALLOC(var, size) lzo_align_t __LZO_MMODEL var[((size)+(sizeof(lzo_align_t)-1)) / sizeof(lzo_align_t)]
//the wkmem is used for compressed
static HEAP_ALLOC(wkmem, LZO1X_1_MEM_COMPRESS);

static BYTE lzoBuffer[COMPRESS_BUFFER_SIZE];

/*ѹ������*/
int BitMapCompress(BYTE* in, BYTE* out, ULONG inLen, ULONG* outLen){
	if (lzo_init() != LZO_E_OK) {
		return BITIMAGEUTILS_LZO_INIT_ERROR;
	}
	int ret = -1;
	if ((ret = lzo1x_1_compress(in, inLen, out, outLen, wkmem)) != LZO_E_OK) {
		return BITIMAGEUTILS_LZO_COMPRESS_ERROR;
	}
	return BITIMAGEUTILS_NO_ERROR;
}

/*ѹ������*/
BYTE* BitMapCompress(BYTE* in, ULONG inLen, ULONG* outLen, int* error){
	if (inLen <= COMPRESS_BUFFER_SIZE){
		*error = BitMapCompress(in, lzoBuffer, inLen, outLen);
		if (*error != BITIMAGEUTILS_NO_ERROR){
			return nullptr;
		}
		else {
			return lzoBuffer;
		}
	}
	*error = BITIMAGEUTILS_LZO_COMPRESS_ERROR;
	return nullptr;
}

/*��ѹ������*/
int BitMapDecompress(BYTE* in, BYTE* out, ULONG inLen, ULONG* outLen){
	if (lzo_init() != LZO_E_OK) {
		return BITIMAGEUTILS_LZO_INIT_ERROR;
	}
	int ret = -1;
	if ((ret = lzo1x_decompress(in, inLen, out, outLen, NULL)) != LZO_E_OK) {
		return BITIMAGEUTILS_LZO_DECOMPRESS_ERROR;
	}
	return BITIMAGEUTILS_NO_ERROR;
}

/*��ѹ������*/
BYTE* BitMapDecompress(BYTE* in, ULONG inLen, ULONG* outLen,int* error){
	if (inLen <= COMPRESS_BUFFER_SIZE){
		*error = BitMapDecompress(in, lzoBuffer, inLen, outLen);
		if (*error != BITIMAGEUTILS_NO_ERROR){
			return nullptr;
		}
		else {
			return lzoBuffer;
		}
	}
	*error = BITIMAGEUTILS_LZO_DECOMPRESS_ERROR;
	return nullptr;
}

HBITMAP CopyScreenToBitmap(LPRECT lpRect)
{
	HCURSOR hCursor = NULL;
	POINT pt;
	HWND hwndPt;
	DWORD dwTid = 0, dwCurTid = 0;
	HDC hScrDC, hMemDC;
	// ��Ļ���ڴ��豸������
	HBITMAP hBitmap, hOldBitmap;
	// λͼ���
	int nX, nY, nX2, nY2;
	// ѡ����������
	int nWidth, nHeight;
	// λͼ��Ⱥ͸߶�
	int xScrn, yScrn;
	// ��Ļ�ֱ���
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect)){
		return NULL;
	}
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// �����Ļ�ֱ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
	
	//�����
	GetCursorPos(&pt); //��ȡ�������   
	hwndPt = WindowFromPoint(pt); //���������Ҵ��ھ��   
	dwTid = GetWindowThreadProcessId(hwndPt, NULL); //��ȡ�ô����������߳�id   
	dwCurTid = GetCurrentThreadId(); //��ȡ��ǰ�߳�id   
	if (dwTid != dwCurTid)
	{
		AttachThreadInput(dwCurTid, dwTid, TRUE); //�߳����븽��   
		hCursor = GetCursor(); //��ȡ���������   
		AttachThreadInput(dwCurTid, dwTid, FALSE);
	}
	else { 
		hCursor = GetCursor(); 
	}
	DrawIconEx(hMemDC, pt.x, pt.y, hCursor, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT | DI_DEFAULTSIZE); //������껭���� 
	//�õ���Ļλͼ�ľ��
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	return hBitmap;
}

int SaveBitmapToMemory(HBITMAP hBitmap, LBitMapInfo image, BOOL isCompress)
{
	int result = BITIMAGEUTILS_NO_ERROR;
	HDC     hDC;
	//�豸������
	int     iBits;
	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD    wBitCount;
	//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��  λͼ�ļ���С �� д���ļ��ֽ���
	DWORD           dwPaletteSize = 0, dwBmBitsSize, dwDIBSize;
	BITMAP          Bitmap;
	//λͼ���Խṹ
	BITMAPFILEHEADER   bmfHdr;
	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER   bi;
	//λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi;
	//ָ��λͼ��Ϣͷ�ṹ
	HANDLE          hDib, hPal;
	HPALETTE     hOldPal = NULL;
	//�����ļ��������ڴ�������ɫ����
	//����λͼ�ļ�ÿ��������ռ�ֽ���
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	//8λɫ
	wBitCount = 8;
	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount)*sizeof(RGBQUAD);
	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	// ����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	/*�����ݿ�������*/
	
	UINT total = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
	image->src_size = total;
	if (image->size >= total){
		
		UINT index = 0;
		UINT len = sizeof(BITMAPFILEHEADER);
		char* head = (char*)&bmfHdr;
		for (UINT i = 0; i < len; i++){
			image->out[index++] = head[i];
		}
		len = sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
		char* body = (char*)lpbi;
		for (UINT i = 0; i < len; i++){
			image->out[index++] = body[i];
		}
		image->size = total;
		
		if (isCompress){
			index = 0;
			ULONG outw = 0;
			int error = 0;
			BYTE* res = BitMapCompress(image->out, total, &outw, &error);
			if (error != BITIMAGEUTILS_NO_ERROR){
				result = error;
			}
			else {
				for (UINT i = 0; i < outw; i++){
					image->out[index++] = res[i];
				}
			}
			image->size = outw;
		}
		
	}
	else {
		result = BITIMAGEUTILS_BUFFER_ERROR;
	}
	
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	return result;
}
