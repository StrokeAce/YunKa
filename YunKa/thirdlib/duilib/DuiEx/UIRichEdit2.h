#ifndef __UIRICHEDIT2_H__
#define __UIRICHEDIT2_H__

#pragma once
#include <stdio.h>
#include "OleHelper.h"
#include "../Uilib.h"


class CControlUI;
class IMessageFilterUI;
class CContainerUI;



namespace DuiLib2 {



	typedef struct _CRichEditFontFormat
	{
		LPCTSTR  sName;
		int nSize;
		COLORREF refColor;
		BOOL bBold;
		BOOL bItalic;
		BOOL bUnderLine;
		BOOL bIsLink;
		int nStartIndent;

		_CRichEditFontFormat() :sName(_T("微软雅黑")), nSize(9), refColor(RGB(0, 0, 0)), bBold(FALSE), bItalic(FALSE), bUnderLine(FALSE), bIsLink(FALSE) {};
	}CRichEditFontFormat;

class CTxtWinHost2;
class CImageDataObject;
class CGifOleObject;

#define UILIB_API

class UILIB_API CRichEditUI2 : public CContainerUI, public IMessageFilterUI
{
public:
    class IRichEditOleExCallback : public IRichEditOleCallback
    {
    public :
        IRichEditOleExCallback();
        virtual ~ IRichEditOleExCallback();
        int m_iNumStorages;
        IStorage * pStorage;
        DWORD m_dwRef;

        virtual HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE * lplpstg);
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
        virtual ULONG STDMETHODCALLTYPE AddRef();
        virtual ULONG STDMETHODCALLTYPE Release();
        virtual HRESULT STDMETHODCALLTYPE GetInPlaceContext(LPOLEINPLACEFRAME FAR * lplpFrame,
            LPOLEINPLACEUIWINDOW FAR * lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
        virtual HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL fShow);
        virtual HRESULT STDMETHODCALLTYPE QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
        virtual HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj);
        virtual HRESULT STDMETHODCALLTYPE QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat,
            DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
        virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
        virtual HRESULT STDMETHODCALLTYPE GetClipboardData(CHARRANGE FAR * lpchrg, DWORD reco, LPDATAOBJECT FAR * lplpdataobj);
        virtual HRESULT STDMETHODCALLTYPE GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
        virtual HRESULT STDMETHODCALLTYPE GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR * lpchrg, HMENU FAR * lphmenu);
    };

public:
    CRichEditUI2();
    ~CRichEditUI2();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;
    bool IsWantTab();
    void SetWantTab(bool bWantTab = true);
    bool IsWantReturn();
    void SetWantReturn(bool bWantReturn = true);
    bool IsWantCtrlReturn();
    void SetWantCtrlReturn(bool bWantCtrlReturn = true);
    bool IsRich();
    void SetRich(bool bRich = true);
    bool IsReadOnly();
    void SetReadOnly(bool bReadOnly = true);
    bool GetWordWrap();
    void SetWordWrap(bool bWordWrap = true);
    int GetFont();
    void SetFont(int index);
    void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    LONG GetWinStyle();
    void SetWinStyle(LONG lStyle);
    DWORD GetTextColor();
    void SetTextColor(DWORD dwTextColor);
    int GetLimitText();
    void SetLimitText(int iChars);
    long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
    CDuiString GetText() const;
    void SetText(LPCTSTR pstrText);
    bool GetModify() const;
    void SetModify(bool bModified = true) const;
    void GetSel(CHARRANGE &cr) const;
    void GetSel(long& nStartChar, long& nEndChar) const;
    int SetSel(CHARRANGE &cr);
    int SetSel(long nStartChar, long nEndChar);
    void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
    void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
    CDuiString GetSelText() const;
    int SetSelAll();
    int SetSelNone();
    WORD GetSelectionType() const;
    bool GetZoom(int& nNum, int& nDen) const;
    bool SetZoom(int nNum, int nDen);
    bool SetZoomOff();
    bool GetAutoURLDetect() const;
    bool SetAutoURLDetect(bool bAutoDetect = true);
    DWORD GetEventMask() const;
    DWORD SetEventMask(DWORD dwEventMask);
    CDuiString GetTextRange(long nStartChar, long nEndChar) const;
    void HideSelection(bool bHide = true, bool bChangeStyle = false);
    void ScrollCaret();
    int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
	int AppendText(LPCTSTR lpstrText, const CRichEditFontFormat &ref, BOOL bCanUndo = FALSE);
    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
    bool SetDefaultCharFormat(CHARFORMAT2 &cf);
    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
    bool SetSelectionCharFormat(CHARFORMAT2 &cf);
    bool SetWordCharFormat(CHARFORMAT2 &cf);
    DWORD GetParaFormat(PARAFORMAT2 &pf) const;
    bool SetParaFormat(PARAFORMAT2 &pf);
    bool Redo();
    bool Undo();
    void Clear();
    void Copy();
    void Cut();
    void Paste();
    int GetLineCount() const;
    CDuiString GetLine(int nIndex, int nMaxLength) const;
    int LineIndex(int nLine = -1) const;
    int LineLength(int nLine = -1) const;
    bool LineScroll(int nLines, int nChars = 0);
    CPoint GetCharPos(long lChar) const;
    long LineFromChar(long nIndex) const;
    CPoint PosFromChar(UINT nChar) const;
    int CharFromPos(CPoint pt) const;
    void EmptyUndoBuffer();
    UINT SetUndoLimit(UINT nLimit);
    long StreamIn(int nFormat, EDITSTREAM &es);
    long StreamOut(int nFormat, EDITSTREAM &es);

    void DoInit();
    // 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
    // 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
    virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
    IDropTarget* GetTxDropTarget();
    virtual bool OnTxViewChanged();
    virtual void OnTxNotify(DWORD iNotify, void *pv);

    void SetScrollPos(SIZE szPos);
    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();

    SIZE EstimateSize(SIZE szAvailable);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

    // for ole object
    IRichEditOle* GetIRichEditOle();
    BOOL SetOLECallback(IRichEditOleCallback* pCallback);

	BOOL InsertImage(LPCTSTR szFilePath, DWORD dwUserData);
protected:
    IRichEditOleCallback* m_pIRichEditOleCallback;
    CTxtWinHost2* m_pTwh;
    bool m_bVScrollBarFixing;
    bool m_bWantTab;
    bool m_bWantReturn;
    bool m_bWantCtrlReturn;
    bool m_bRich;
    bool m_bReadOnly;
    bool m_bWordWrap;
    DWORD m_dwTextColor;
    int m_iFont;
    int m_iLimitText;
    LONG m_lTwhStyle;
private:
	void SetSelFont(LPCTSTR lpFontName, int nFontSize,
		COLORREF clrText, BOOL bBold, BOOL bItalic,
		BOOL bUnderLine, BOOL bIsLink);

	BOOL InsertNotGIFImage(LPCTSTR szFilePath, DWORD dwUserData);
	BOOL InsertGIFImage(LPCTSTR szFilePath, DWORD dwUserData);
};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
