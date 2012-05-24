/**
*@file EditorWnd.cpp
*@brief Editor窗口类
*
*Editor窗口注册和窗口实现
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include "EditorWnd.h"
#include "define.h"
#include "editor.h"
#include "common.h"
typedef unsigned long uptr_t;
typedef long eptr_t;

#define MAX_CHAR 100

typedef eptr_t (*WndProc)(eptr_t ptr, unsigned int message, uptr_t wparam, eptr_t lpparam);

const MY_CHAR szClassName[] = _T("SEditWnd");


class EditorWnd
{
public:

	HWND m_hWnd;

	EditorWnd(HWND hWnd)
	{
		richeditor = new RichEditor();
		assert(richeditor);
		m_hWnd = hWnd;
		richeditor->Attach(hWnd);
	}
	static bool Register(HINSTANCE hInstance_) ;
	static bool Unregister();
	static eptr_t PASCAL EWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

	void Finalise()
	{
		// you must clear something here
		hInstance_ = NULL;
		delete richeditor;
	}

public:
	virtual eptr_t WndProc(unsigned int iMessage, uptr_t wParam, eptr_t lParam, BOOL& bHandle);

protected:
	
	static HINSTANCE hInstance_;
	RichEditor* richeditor;
};

HINSTANCE EditorWnd::hInstance_ = NULL;

eptr_t EditorWnd::EWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	// Find C++ object associated with window.
	EditorWnd *se = reinterpret_cast<EditorWnd *>(PointerFromWindow(hWnd));
	// sci will be zero if WM_CREATE not seen yet
	if (se == 0) {
		try {
			if (uMsg == WM_CREATE) {
				// Create C++ object associated with window
				se = new EditorWnd(hWnd);
				SetWindowPointer(hWnd, se);
				BOOL bHandle = FALSE;
				eptr_t ret = 0; 
				ret = se->WndProc(uMsg, wParam, lParam,bHandle);
				//if(!bHandle)
					//ret = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
				return ret;
			}
		} catch (...) {
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	} else {
		if (uMsg == WM_NCDESTROY) {
			try {
				se->Finalise();
				delete se;
			} catch (...) {
			}
			::SetWindowLong(hWnd, 0, 0);
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		} else {
			BOOL bHandle = FALSE;
			eptr_t ret = 0;
			//ATLTRACE(_T("\n+++++++uMsg : 0x%0x +++++++\n"), uMsg);
			ret = se->WndProc(uMsg, wParam, lParam, bHandle);
 			if(!bHandle)
 				ret = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
			return ret;
		}
	}
	return 0;
}

eptr_t EditorWnd::WndProc(unsigned int iMessage, uptr_t wParam, eptr_t lParam, BOOL& bHandle)
{
	return richeditor->OnMessage(iMessage, wParam, lParam, bHandle);
}

bool EditorWnd::Register(HINSTANCE hInstance) 
{
	hInstance_ = hInstance;
	bool result;

	// Register the EditorWnd class

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc = EditorWnd::EWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(EditorWnd *);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;
	wndclass.hIconSm = 0;
	result = ::RegisterClassEx(&wndclass) != 0;

	return result;
}

bool EditorWnd::Unregister() 
{
	bool result = ::UnregisterClass(szClassName, EditorWnd::hInstance_) != 0;
	return result;
}


int SEditWnd_RegisterClasses(void *hInstance)
{
	EditorWnd::Register( reinterpret_cast<HINSTANCE>(hInstance) );
	return 0;
}

int SEditWnd_Release()
{
	return EditorWnd::Unregister();
}

IRicheditor* CoCreateRicheditor()
{
	RichEditor* edit = new RichEditor();
	return (IRicheditor*)edit;
}
