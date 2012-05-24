/**
*@file window.h
*@brief 窗口类
*
*负责实际的窗口相关逻辑应用，比如计算长度，绘画，刷新等
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
class Window
{
public:

	class AutoSelection
	{
	public:
		AutoSelection(Window* window)
		{
			window_ = window;
			window_->SetSelectionMode(true);
		}

		~AutoSelection()
		{
			window_->SetSelectionMode(false);
		}
	protected:
		Window* window_;
	};

	class AutoDC
	{
	public:
		AutoDC(HWND hwnd)
		{
			hWnd_ = hwnd;
			hDC_ = ::GetDC(hWnd_);
		}

		HDC GetDC()
		{
			return hDC_;
		}

		~AutoDC()
		{
			::ReleaseDC(hWnd_, hDC_);
		}

	protected:
		HWND hWnd_;
		HDC hDC_;
	};
public:
	Window()
	{
		hWnd_ = NULL;
		m_brushFrame = NULL;
		m_brushSelect = NULL;
		bSeletionMode_ = false;
		bDoubleBuffering_ = false;
		crSelected_ = RGB(224, 229, 226);
	}
	~Window()
	{
		if(m_brushFrame)
		{
			::DeleteObject((HGDIOBJ)m_brushFrame);
			m_brushFrame = NULL;
		}	
		ClearSelectedBrush();
	}

	void ClearSelectedBrush()
	{
		if(m_brushSelect)
		{
			::DeleteObject((HGDIOBJ)m_brushSelect);
			m_brushSelect = NULL;
		}
	}


	void Attach(HWND hWnd)
	{
		hWnd_ = hWnd;
		bSeletionMode_ = false;
		m_brushFrame = ::CreateSolidBrush(RGB(0,0,0));
		m_brushSelect = ::CreateSolidBrush(crSelected_);
	}

	void Detch()
	{
	}

	void SetSelectColor(COLORREF crSel)
	{
		if (crSelected_ == crSel)
			return ;
		crSelected_ = crSel;
		ClearSelectedBrush();
		m_brushSelect = ::CreateSolidBrush(crSelected_);
	}

	int GetCharLenght(MY_CHAR c, CHARFORMAT& cf, HFONT font)
	{
		AutoDC adc(hWnd_);
		HFONT fontOld = (HFONT)::SelectObject(adc.GetDC(), (HGDIOBJ)font);
		RECT rcText;
		ZeroMemory(&rcText, sizeof(rcText));
		::DrawText(adc.GetDC(), &c, 1, &rcText, DT_CALCRECT | DT_SINGLELINE | DT_LEFT | DT_BOTTOM );
		::SelectObject(adc.GetDC(), (HGDIOBJ)fontOld);
		return (rcText.right - rcText.left);
	}

	int GetTextLenght(MY_CHAR* c, int length, CHARFORMAT& cf, HFONT font)
	{
		AutoDC adc(hWnd_);
		HFONT fontOld = (HFONT)::SelectObject(adc.GetDC(), (HGDIOBJ)font);
		RECT rcText;
		ZeroMemory(&rcText, sizeof(rcText));
		::DrawText(adc.GetDC(), c, length, &rcText, DT_CALCRECT | DT_SINGLELINE );
		::SelectObject(adc.GetDC(), (HGDIOBJ)fontOld);
		return (rcText.right - rcText.left);
	}

	void PaintText(MY_CHAR* c, int length, RECT& rcText, CHARFORMAT& cf, HDC hdcPaint, HFONT font)
	{
		HFONT hOldFont = (HFONT)::SelectObject(hdcPaint, (HGDIOBJ)font);
		::SetBkMode(hdcPaint, TRANSPARENT);
		CRect rcTextDraw;
		::DrawText(hdcPaint, c, length, &rcTextDraw, DT_CALCRECT | DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
		rcText.right = rcText.left + rcTextDraw.Width();
		if (bSeletionMode_)
		{			
			::FillRect(hdcPaint, &rcText, m_brushSelect);
			::SetTextColor(hdcPaint, RGB(255,255,255));
		}
		else
		{
			::SetTextColor(hdcPaint, cf.crTextColor);
		}
		::DrawText(hdcPaint, c, length, &rcText, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
		rcText = rcTextDraw;
		::SelectObject(hdcPaint, (HGDIOBJ)hOldFont);
	}

	void PaintObj(IObj* obj, RECT& rcObj, HDC hdcPaint, bool bSelect = false)
	{
		obj->OnDraw(hdcPaint, rcObj);
		if (bSelect)
			::FrameRect(hdcPaint, &rcObj, m_brushFrame);
	}

	void SetSelectionMode(bool bSele)
	{
		bSeletionMode_ = bSele;
	}


	void ReDraw()
	{
		RECT rcClient;
		::GetClientRect(hWnd_, &rcClient);
		::InvalidateRect(hWnd_, &rcClient,FALSE);
	}

	HWND GetWnd()
	{
		return hWnd_;
	}

	LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return ::SendMessage(hWnd_, Msg, wParam, lParam);
	}

	BOOL PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return ::PostMessage(hWnd_, Msg, wParam, lParam);
	}

protected:
	HWND	hWnd_;
	CHARFORMAT	cfCur_;
	bool	bSeletionMode_;
	bool	bDoubleBuffering_;
	COLORREF crSelected_;

	HBRUSH	m_brushFrame;
	HBRUSH	m_brushSelect;
	
};