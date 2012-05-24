/**
*@file editorInterface.h
*@brief 接口定义
*
*定义整个editor架构中所有接口信息
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once

#include "define.h"
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <assert.h>
#include <Richedit.h>

NAMESPACE_BEGIN(common)
NAMESPACE_END(common)
/************************************************************************/
/*  Selection                                                           */
/************************************************************************/

struct IBaseObj;
NAMESPACE_USE(common)

struct ILogger
{
	virtual void Log()			= 0;
};

struct Selection
{
	Selection():Begin_(-1), End_(-1)
	{}

	Selection(int nBegin, int nEnd):Begin_(nBegin), End_(nEnd)
	{}

	int Begin()
	{
		return Begin_;
	}

	int End()
	{
		return End_;
	}

	int GetSelectionLenght()
	{
		return (End_ - Begin_ + 1);
	}

	bool IsSelected() const
	{
		if ( Begin_ >= 0 && End_ >= 0 && Begin_ <= End_ )
			return true;
		return false;
	}

	bool IsBeSelect(int pos) const
	{
		if (pos <= End_ && pos >= Begin_)
			return true;
		return false;
	}

	bool IsBeSelect(int begin, int end) const
	{
		if (begin <= End_ && end >= Begin_)
			return true;
		return false;
	}

	void UpdateSelection(int pos)
	{
		if (pos <= Begin_)
		{
			End_ = Begin_;
			Begin_ = pos;
		}
		else
		{
			End_ = pos;
		}
	}

	void ClearSelection()
	{
		Begin_ = -1;
		End_ = -1;
	}

	void Log(MY_CHAR* name)
	{
		ATLTRACE("\n%s: Begin_:%d ; End_:%d;\n"
			, name
			, Begin_
			, End_);
	}

	int Begin_;
	int End_;
};

/************************************************************************/
/*  RowObjDetail													    */
/************************************************************************/

struct RowObjDetail
{
	RowObjDetail()
	{
		maxHeight_ = 0;
		pos_ = -1;
		ident_ = 0;
	}

	RowObjDetail& operator = (const RowObjDetail& other)
	{
		maxHeight_ = other.maxHeight_;
		pos_ = other.pos_;
		ident_ = other.ident_;
		return *this;
	}
	int maxHeight_;
	int pos_;
	int ident_;
};


/************************************************************************/
/*  IBaseObj                                                            */
/************************************************************************/
struct IBaseObj
{
	virtual int	AddRef() = 0;
	virtual int Release() = 0;
};

/************************************************************************/
/*  IObj																*/
/************************************************************************/

struct IObj : public IBaseObj
{
	virtual const SIZE* GetSize() const = 0;
	virtual void OnDraw(HDC dc, RECT& rcObj)  = 0;
};

/************************************************************************/
/*  ContextObj														    */
/************************************************************************/

class ContextObj
	: public IObj
{
public:
	virtual const SIZE* GetSize() const
	{
		return &sz_;
	}
protected:
	SIZE	sz_;

};

/************************************************************************/
/*  ImageObj														    */
/************************************************************************/

class ImageObj
	: public ContextObj
{
public:
	ImageObj():obj_(NULL){}

	void UpdatePos(int pos, int line)
	{
		posInLine_ = pos;
		ownLine_ = line;
	}

protected:
	int posInLine_;
	int ownLine_;
	void* obj_;
};


/************************************************************************/
/*  IRicheditor															*/
/************************************************************************/

struct IRicheditor
{
	//
	// behaviour
	//
	virtual void Attach(HWND hWnd)																= 0;
	virtual void Release()																		= 0;
	virtual void InsertString(const MY_CHAR* c, int pos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData)			= 0;
	virtual void InsertLink(const MY_CHAR* c, int length, int pos, MY_CHAR* hyperLinkData)		= 0;
	virtual void InsertObj(IObj* obj, int pos)													= 0;
	virtual void SetReadOnly(bool bReadOnly)													= 0;
	virtual void SetBkgrdColor(COLORREF crbkgrd)												= 0;
	virtual void SetSelectedColor(COLORREF crSelected)											= 0;
	virtual void ScrollToEnd()																	= 0;
	virtual void SetParagraphIdent(int ident)													= 0;
	virtual void GetWindowText(MY_CHAR* src)													= 0;
	virtual void GetSelText(MY_CHAR* src)														= 0;
	virtual void GetText(MY_CHAR* src, int pos, int length)										= 0;

	//
	//property
	//
	virtual bool IsReadOnly() const																= 0;
	virtual void GetRect(RECT& rcClient)														= 0;

	//
	//paint
	//
	virtual void Paint()																		= 0;
	virtual void Paint(HDC dcPaint,RECT& rcClient, RECT& rcUpdate)								= 0;

	//
	//message
	//
	virtual LRESULT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)			= 0;
};

EXTERN_C __declspec(dllexport) int SEditWnd_RegisterClasses(void *hInstance);
EXTERN_C __declspec(dllexport) int SEditWnd_UnRegisterClasses();
EXTERN_C __declspec(dllexport) IRicheditor* CoCreateRicheditor();


