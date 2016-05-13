//-------------------------------------------------------------------------
//  GifRichEdit - RichEdit Plus Control
//--------------------------------------------------------------------------
#pragma once

#include <richole.h>
#include <GdiPlus.h>
#include <Tom.h>
#include <atlstr.h>
#include "UIlib.h"

#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;

namespace DuiLib2 {

	// 除GIF图像以外的基本图像格式的实现类
	class CImageDataObject : IDataObject
	{
	public:
		CImageDataObject();
		~CImageDataObject();

		HRESULT WINAPI QueryInterface(REFIID iid, void ** ppvObject);
		ULONG WINAPI AddRef(void);
		ULONG WINAPI Release(void);

		HRESULT WINAPI GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
		HRESULT WINAPI GetDataHere(FORMATETC* pformatetc, STGMEDIUM*  pmedium);
		HRESULT WINAPI QueryGetData(FORMATETC*  pformatetc);
		HRESULT WINAPI GetCanonicalFormatEtc(FORMATETC*  pformatectIn, FORMATETC* pformatetcOut);
		HRESULT WINAPI SetData(FORMATETC* pformatetc, STGMEDIUM*  pmedium, BOOL  fRelease);
		HRESULT WINAPI EnumFormatEtc(DWORD  dwDirection , IEnumFORMATETC** ppenumFormatEtc);
		HRESULT WINAPI DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
		HRESULT WINAPI DUnadvise(DWORD dwConnection);
		HRESULT WINAPI EnumDAdvise(IEnumSTATDATA **ppenumAdvise);

		void SetBitmap(HBITMAP hBitmap);
		IOleObject *GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage);
		BOOL InsertNotGIFImage( IRichEditOle* pRichEditOle, LPCTSTR szFilePath, DWORD dwUserData);

	private:
		TCHAR m_szNotGifFile[MAX_PATH];
		ULONG	m_ulRefCnt;
		BOOL	m_bRelease;

		STGMEDIUM m_stgmed;
		FORMATETC m_fromat;
	};

	// CGifOleObject
	//
	// GIF OLE对象实现类
	class CGifOleObject : public IOleObject, public IViewObject2
	{
	public:
		CGifOleObject();
		~CGifOleObject();

		// IUnknown接口
		virtual HRESULT WINAPI QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG   WINAPI AddRef(void);
		virtual ULONG   WINAPI Release(void);

		// IOleObject接口
		virtual HRESULT WINAPI SetClientSite(IOleClientSite *pClientSite);
		virtual HRESULT WINAPI GetClientSite(IOleClientSite **ppClientSite);
		virtual HRESULT WINAPI SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj);
		virtual HRESULT WINAPI Close(DWORD dwSaveOption);
		virtual HRESULT WINAPI SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk);
		virtual HRESULT WINAPI GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk);
		virtual HRESULT WINAPI InitFromData(IDataObject *pDataObject, BOOL fCreation, DWORD dwReserved);
		virtual HRESULT WINAPI GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject);
		virtual HRESULT WINAPI DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
		virtual HRESULT WINAPI EnumVerbs(IEnumOLEVERB **ppEnumOleVerb);
		virtual HRESULT WINAPI Update(void);
		virtual HRESULT WINAPI IsUpToDate(void);
		virtual HRESULT WINAPI GetUserClassID(CLSID *pClsid);
		virtual HRESULT WINAPI GetUserType(DWORD dwFormOfType, LPOLESTR *pszUserType);
		virtual HRESULT WINAPI SetExtent(DWORD dwDrawAspect, SIZEL *psizel);
		virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, SIZEL *psizel);
		virtual HRESULT WINAPI Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection);
		virtual HRESULT WINAPI Unadvise(DWORD dwConnection);
		virtual HRESULT WINAPI EnumAdvise(IEnumSTATDATA **ppenumAdvise);
		virtual HRESULT WINAPI GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus);
		virtual HRESULT WINAPI SetColorScheme(LOGPALETTE *pLogpal);

		// IViewObject接口
		virtual HRESULT WINAPI Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
			HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
			BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue);
		virtual HRESULT WINAPI GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
			DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet);
		virtual HRESULT WINAPI Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze);
		virtual HRESULT WINAPI Unfreeze(DWORD dwFreeze);
		virtual HRESULT WINAPI SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
		virtual HRESULT WINAPI GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);

		// IViewObject2接口
		virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel);

		void SetRichEditHwnd(HWND hWnd);
		HWND GetRichEditHwnd();

		BOOL LoadGifFromFile(const TCHAR *lpszGifFile, const LPCTSTR sType = NULL);
		void GetGifSize(LPSIZE lpSize);
		UINT GetFrameCount();
		void OnTimer(UINT_PTR idEvent);
		void GetOleObjectRect(LPRECT lpRect);
		void DrawFrame(HDC hDC, LPRECT lpRect);

		static BOOL InsertGifRichEdit(CControlUI* owner, HWND hWnd, IRichEditOle* pRichEditOle, 
			                                          const TCHAR *lpGifPath, DWORD dwUserData, LPCTSTR sType = NULL);
		static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	protected:
		void DeleteGif();
		BOOL IsAnimatedGIF();

	protected:
		CControlUI* m_pOwner;
		ULONG m_ulRef;
		TCHAR m_szGifFile[MAX_PATH];
		Gdiplus::Image *m_pGifImage;
		UINT *m_puFrameElapse;
		UINT m_nFrameCount;
		UINT m_nFramePosition;
		IOleClientSite *m_pOleClientSite;
		HWND m_hWndRichEdit;
	};
}