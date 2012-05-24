/**
*@file common.h
*@brief 公共函数集合
*
*包括IsCharFormatEqual，GetSystemFont，GetSystemFontName，GetDefaultCharformat，IsBeginWith，IsLink，InputCodePage
*，PointerFromWindow，SetWindowPointer
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "define.h"
#include "editorInterface.h"

NAMESPACE_BEGIN(common)

///@brief 判断两种字体格式是否相同
///
///判断两种字体格式是否相同
///@param[in],cf1:字体格式1
///@param[in],cf2:字体格式2
///@pre NULL
///@return bool
inline bool IsCharFormatEqual(const CHARFORMAT& cf1, const CHARFORMAT& cf2)
{
	if (cf1.bCharSet != cf2.bCharSet 
		|| cf1.bPitchAndFamily != cf2.bPitchAndFamily
		|| cf1.cbSize != cf2.cbSize
		|| cf1.crTextColor != cf2.crTextColor
		|| cf1.dwEffects != cf2.dwEffects
		|| cf1.dwMask != cf2.dwMask
		|| (lstrcmp(cf1.szFaceName, cf2.szFaceName) != 0)
		|| cf1.yHeight != cf2.yHeight
		|| cf1.yOffset != cf2.yOffset)
	{
		return false;
	}

	return true;
}

///@brief 获取系统字体
///
///获取系统字体
///@pre NULL
///@return HFONT
inline HFONT GetSystemFont()
{
	NONCLIENTMETRICS nm;
	nm.cbSize = sizeof (NONCLIENTMETRICS);
	SystemParametersInfo (SPI_GETNONCLIENTMETRICS,0,&nm,0);
	LOGFONT fl = nm.lfMenuFont;
	WTL::CFont font; 
	font.CreateFontIndirect(&fl);
	return font.Detach();

}

///@brief 获取系统字体名称
///
///获取系统字体名称
///@param[out],c:字符串地址
///@pre NULL
///@return void
inline void GetSystemFontName(MY_CHAR* c)
{
	static MY_CHAR fontName[50]=_T("\0");
	if (fontName[0] == '\0')
	{
		WTL::CFont font =  GetSystemFont();
		LOGFONT lf; 
		font.GetLogFont(lf);
		lstrcpy(fontName, lf.lfFaceName);
	}
	lstrcpy(c, fontName);
}
///@brief 获取默认样式格式
///
///获取默认样式格式
///@param[out],cfdefault:字体格式引用
///@pre NULL
///@return void
static void GetDefaultCharformat(CHARFORMAT& cfdefault)
{
	cfdefault.cbSize = sizeof(CHARFORMAT);
	cfdefault.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	cfdefault.dwEffects = 0;
	cfdefault.yHeight = 12;
	cfdefault.yOffset = 0;
	GetSystemFontName(cfdefault.szFaceName);
}

///@brief 判断某字符串是否以特定的字符串开头
///
///判断某字符串是否以特定的字符串开头
///@param[in],lpString:源字符串
///@param[in],lphead:特定字符串
///@pre NULL
///@return bool
inline bool IsBeginWith(LPCTSTR lpString,LPCTSTR lphead)
{
	if( lpString == NULL || lphead == NULL )
		return false;

	for (int i=0; lpString[i]; i++)
	{
		if( lphead[i] == NULL )
			return true;
		if( lphead[i] != lpString[i] && lphead[i] != ::tolower(lpString[i]) )
			return false;
	}
	return false;
}

///@brief 判断某字符串是否符合link属性
///
///判断某字符串是否符合link属性
///@param[in],strLink:源字符串
///@pre NULL
///@return bool
inline bool IsLink(LPCTSTR strLink)
{
	LPCTSTR szLinkPattern[] = { _T("http://"),_T("https://"), _T("www."),_T("update."), _T("api.") };
	for (int i=0; i < _countof(szLinkPattern); i++)
	{
		if( IsBeginWith(strLink,szLinkPattern[i]) )
			return true;
	}
	return false;
}

///@brief 获取输入状态的codepage
///
///获取输入状态的codepage
///@pre NULL
///@return int
static int InputCodePage() {
	HKL inputLocale = ::GetKeyboardLayout(0);
	LANGID inputLang = LOWORD(inputLocale);
	char sCodePage[10];
	int res = ::GetLocaleInfoA(MAKELCID(inputLang, SORT_DEFAULT),
		LOCALE_IDEFAULTANSICODEPAGE, sCodePage, sizeof(sCodePage));
	if (!res)
		return 0;
	return atoi(sCodePage);
}

#ifdef GetWindowLongPtr
static void *PointerFromWindow(HWND hWnd) {
	return reinterpret_cast<void *>(::GetWindowLongPtr(hWnd, 0));
}
static void SetWindowPointer(HWND hWnd, void *ptr) {
	::SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(ptr));
}
#else
static void *PointerFromWindow(HWND hWnd) {
	return reinterpret_cast<void *>(::GetWindowLong(hWnd, 0));
}
static void SetWindowPointer(HWND hWnd, void *ptr) {
	::SetWindowLong(hWnd, 0, reinterpret_cast<LONG>(ptr));
}
#endif
NAMESPACE_END(common)