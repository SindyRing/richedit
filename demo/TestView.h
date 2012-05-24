#pragma once
//#include "richedit/EditorWnd.h"
#include "editorInterface.h"

class TestView
{
public:
	HWND CreateView(HINSTANCE hinstance)
	{
		hWnd_ = ::CreateWindowEx(NULL
			, _T("SEditWnd")
			, _T("TestView")
			, WS_VISIBLE | WS_POPUP | WS_VSCROLL
			, 50, 50, 200, 200
			, NULL
			, NULL
			, hinstance
			,NULL);
		if(!hWnd_)
		{
			assert(false);
		}
		return hWnd_;
	}

public:
	HWND hWnd_;
};