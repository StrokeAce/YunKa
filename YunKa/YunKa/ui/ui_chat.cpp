#include "..\stdafx.h"

#include "ui_chat.h"


CUIChat::CUIChat()
{

}

CUIChat::~CUIChat()
{

}



LRESULT CUIChat::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{


	return 0;
}


void CUIChat::Notify(TNotifyUI& msg)
{

	NotifyMsg(msg);
}

void CUIChat::init()
{

}



void CUIChat::SetPaintManager(CPaintManagerUI *val)
{
	m_PaintManager = val;
	m_hMainWnd = m_PaintManager->GetPaintWindow();

	init();

}

bool CUIChat::OnMsg(void *pMsg)
{


	return 0;
}


void CUIChat::NotifyMsg(TNotifyUI& msg)
{

}

LRESULT CUIChat::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	return 0;
}

