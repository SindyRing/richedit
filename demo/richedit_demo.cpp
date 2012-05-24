// richedit_demo.cpp : main source file for richedit_demo.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <string>
#include "resource.h"
#include "aboutdlg.h"
#include "MainDlg.h"
#include "TestView.h"
#include "GdiPlusGetter.h"

CAppModule _Module;

typedef int (*Register_EditWnd)(void *);
typedef int (*UnRegister_EditWnd)();

static std::wstring getAppPath()
{
	static std::wstring strAppPath;
	if (strAppPath.empty())
	{
		TCHAR tmp[1024];
		tmp[0] = 0;
		if(GetModuleFileName(NULL, tmp, sizeof(tmp)) > 0) {
			TCHAR* dst = NULL;
			TCHAR* p = tmp;
			while (*p) {
				if (*p == _T('\\'))
					dst = p;
				p = CharNext(p);
			}
			if(dst) *dst = 0;
		}
		strAppPath = tmp;
	}

	return strAppPath;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	std::wstring str = getAppPath();
	str += _T("\\richedit.dll");
	
	HMODULE hModule = LoadLibrary(str.c_str());

	Register_EditWnd pRE = NULL;
	UnRegister_EditWnd pURE = NULL;
	if (hModule)
	{
		pRE = (Register_EditWnd)GetProcAddress(hModule,"SEditWnd_RegisterClasses");
		pURE = (UnRegister_EditWnd)GetProcAddress(hModule, "SEditWnd_UnRegisterClasses");
	}

	if (pRE)
		pRE(_Module.GetModuleInstance());
	

	//SEditWnd_RegisterClasses(_Module.GetModuleInstance());
	TestView view;
	view.CreateView(_Module.GetModuleInstance());
	int nRet = theLoop.Run();

	if (pURE)
		pURE();
	//SEditWnd_Release();
	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	OleInitialize(0);
	CGdiPlusGetter gdiplus;

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();
	OleUninitialize();
	return nRet;
}
