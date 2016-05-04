#ifndef  __UI_CHAT__
#define  __UI_CHAT__



#define CHAT_FONT_NAME (_T("微软雅黑"))

using namespace std;
//using namespace DuiLib2;

class CFontInfo				// 字体信息
{
public:
	CFontInfo(void)
	{
		m_nSize = 9;
		m_clrText = RGB(0, 0, 0);
		m_strName = CHAT_FONT_NAME;
		m_bBold = FALSE;
		m_bItalic = FALSE;
		m_bUnderLine = FALSE;
	}
	//~CFontInfo(void);

public:
	int m_nSize;			// 字体大小
	COLORREF m_clrText;		// 字体颜色
	wstring m_strName;		// 字体名称
	BOOL m_bBold;			// 是否加粗
	BOOL m_bItalic;			// 是否倾斜
	BOOL m_bUnderLine;		// 是否带下划线
};

class CUIChat : public IMessageFilterUI, public INotifyUI
{

public:

	CUIChat();
	~CUIChat();

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void Notify(TNotifyUI& msg);
	void init();



	void SetPaintManager(CPaintManagerUI *val);

	bool OnMsg(void *pMsg);

	void NotifyMsg(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);



private:
	CPaintManagerUI *m_PaintManager;
	HWND m_hMainWnd;



};




#endif