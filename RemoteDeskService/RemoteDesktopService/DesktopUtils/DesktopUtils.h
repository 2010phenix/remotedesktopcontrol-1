// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DESKTOPUTILS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DESKTOPUTILS_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef DESKTOPUTILS_EXPORTS
#define DESKTOPUTILS_API __declspec(dllexport)
#else
#define DESKTOPUTILS_API __declspec(dllimport)
#endif

#include "BitImageUtils.h"
#define ENABLE_MOUSE_EVENT
#define ENABLE_KEYBOARD_EVENT
#define ENABLE_SCREEN_IMAGE
#define ENABLE_SYSTEM_EVENT


#ifdef ENABLE_MOUSE_EVENT
EXTERN_C DESKTOPUTILS_API int MouseMove(int x,int y);
EXTERN_C DESKTOPUTILS_API int LBMouseDown();
EXTERN_C DESKTOPUTILS_API int LBMouseUp();
EXTERN_C DESKTOPUTILS_API int RBMouseDown();
EXTERN_C DESKTOPUTILS_API int RBMouseUp();
#endif

#ifdef ENABLE_KEYBOARD_EVENT
EXTERN_C DESKTOPUTILS_API int KeyBoardDown(int ch);
EXTERN_C DESKTOPUTILS_API int KeyBoardUp(int ch);
#endif

#ifdef ENABLE_SCREEN_IMAGE

EXTERN_C DESKTOPUTILS_API int GetBitImageByFullScreen(LBitMapInfo info);

EXTERN_C DESKTOPUTILS_API int DecompressImage(LBitMapInfo info);

EXTERN_C DESKTOPUTILS_API void GetScreenSize(int* width, int*height);

#endif

#ifdef ENABLE_SYSTEM_EVENT

EXTERN_C DESKTOPUTILS_API int LogoutWindows(bool force);

EXTERN_C DESKTOPUTILS_API int CloseWindows(bool force);

EXTERN_C DESKTOPUTILS_API int GetCpuUsageRate(int* rate);

EXTERN_C DESKTOPUTILS_API int GetGlobalMemoryUsageRate(int* rate);

EXTERN_C DESKTOPUTILS_API int SetAuthCode(LPCWSTR value);

EXTERN_C DESKTOPUTILS_API int SetAuthLock(LPCWSTR value);

EXTERN_C DESKTOPUTILS_API int SetMHID(LPCWSTR value);

EXTERN_C DESKTOPUTILS_API int SetLoginName(LPCWSTR value);

EXTERN_C DESKTOPUTILS_API int SetSupperCode(LPCWSTR value);

EXTERN_C DESKTOPUTILS_API int CreateUserProcess(LPWSTR app);

EXTERN_C DESKTOPUTILS_API int ShowMessage(LPWSTR title,LPWSTR message);

#endif


