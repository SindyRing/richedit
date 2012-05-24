//#include "stdafx.h"
#include "editorInterface.h"
#include <GdiPlus.h>
#include <GdiPlusHeaders.h>
//#include <GdiPlusBitmap.h>

class CMyImageObj : public IObj
{
public:

	CMyImageObj()
	{
		ZeroMemory(&size_, sizeof(size_));
		size_.cx = 36;
		size_.cy = 36;
		ref_ = 0;
		hbitmap_ = NULL;
		AddRef();
	}

	~CMyImageObj()
	{
		if (hbitmap_)
		{
			::DeleteObject((HGDIOBJ)hbitmap_);
		}
	}

	/////////////////////Interface///////////////////////////////////////////////
	virtual const SIZE* GetSize() const
	{
		return &size_;
	}

	virtual int	AddRef()
	{
		ref_++;
		return ref_;
	}

	virtual int Release()
	{
		ref_--;
		if (ref_ == 0)
			delete this;
		return ref_;
	}

	virtual void OnDraw(HDC dc, RECT& rcObj)
	{
		if (!hbitmap_)
		{
			Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(L"c:\\pic_question.png");
			COLORREF crbkgrd = RGB(255,255,255);
			bitmap->GetHBITMAP(crbkgrd, &hbitmap_);
			delete bitmap;
		}

		CRect rcObjTemp = rcObj;
		HDC hmemberDC = ::CreateCompatibleDC(dc);
		HBITMAP oldBitmap = (HBITMAP)::SelectObject(hmemberDC, (HGDIOBJ)hbitmap_);
		::StretchBlt(dc, rcObjTemp.left, rcObjTemp.top, rcObjTemp.Width(), rcObjTemp.Height(),
			hmemberDC, 0, 0, size_.cx, size_.cy, SRCCOPY);
		::SelectObject(hmemberDC, (HGDIOBJ)oldBitmap);
		::DeleteObject(hmemberDC);

	}
protected:

	int ref_;
	SIZE size_;
	HBITMAP hbitmap_;
};