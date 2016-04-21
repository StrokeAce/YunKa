// em client lib.h: 定义的一些通用函数的头文件
//
//////////////////////////////////////////////////////////////////////
#if !defined _EMLIB_EMC
#define _EMLIB_EMC

DWORD CALLBACK EditStreamOutCallbackEx(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb);
DWORD CALLBACK EditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb);
DWORD CALLBACK EditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb);

DWORD CALLBACK EditStreamInCallbackFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb);
DWORD CALLBACK EditStreamOutCallbackFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb);

BOOL InsertBitmapToRichEditCtrl(CRichEditCtrl *pRichEdit, HBITMAP hBitmap, DWORD dwValue=0);
BOOL InsertFileToRichEditCtrl(CRichEditCtrl *pRichEdit, CString strFile);

HBITMAP CreateGradientBMP(int nAreaNum, RECT *rt, COLORREF *clrBegin, BOOL bVert=true);
void DrawGradientRect(HDC hDC, RECT rt, COLORREF clrBegin, COLORREF clrEnd, BOOL bVert=true);

//旋转图象的函数
HBITMAP GetRotatedBitmapNT( HBITMAP hBitmap, float radians, COLORREF clrBack );
HBITMAP GetRotatedBitmap( HBITMAP hBitmap, float radians, COLORREF clrBack );
HANDLE GetRotatedBitmap( HANDLE hDIB, float radians, COLORREF clrBack );

COLORREF GetMiddleColor(COLORREF color1, COLORREF color2);

BOOL CreateGrandientBitmapAndBrush(int nGrandientType, RECT rt, COLORREF topColor, COLORREF bottomColor
								   , CBitmap *&pBitmap, CBrush *&pBrush, BOOL bVert = true);
void DrawGradientBack(CDC *pDC, int nGrandientType, RECT rt, COLORREF topColor, COLORREF bottomColor, BOOL bVert = true);
void DrawBitmapBack(CDC *pDC, CRect rt, CBitmap *pBitmap);

//读取gif bmp jpeg文件
HBITMAP LoadAnImage(char* FileName);

#endif