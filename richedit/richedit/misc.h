/**
*@file misc.h
*@brief 杂项
*
*包含各种工程中偶尔应用到的一些包装类
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/

NAMESPACE_BEGIN(misc)

class CRect : public RECT
{
public:
	// Constructors
	CRect()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	CRect(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	CRect(const RECT& srcRect)
	{
		::CopyRect(this, &srcRect);
	}

	CRect(LPCRECT lpSrcRect)
	{
		::CopyRect(this, lpSrcRect);
	}

	CRect(POINT point, SIZE size)
	{
		right = (left = point.x) + size.cx;
		bottom = (top = point.y) + size.cy;
	}

	CRect(POINT topLeft, POINT bottomRight)
	{
		left = topLeft.x;
		top = topLeft.y;
		right = bottomRight.x;
		bottom = bottomRight.y;
	}

	// Attributes (in addition to RECT members)
	int Width() const
	{
		return right - left;
	}

	int Height() const
	{
		return bottom - top;
	}


	// convert between CRect and LPRECT/LPCRECT (no need for &)
	operator LPRECT()
	{
		return this;
	}

	operator LPCRECT() const
	{
		return this;
	}

	BOOL IsRectEmpty() const
	{
		return ::IsRectEmpty(this);
	}

	BOOL IsRectNull() const
	{
		return (left == 0 && right == 0 && top == 0 && bottom == 0);
	}

	BOOL PtInRect(POINT point) const
	{
		return ::PtInRect(this, point);
	}

	// Operations
	void SetRect(int x1, int y1, int x2, int y2)
	{
		::SetRect(this, x1, y1, x2, y2);
	}

	void SetRect(POINT topLeft, POINT bottomRight)
	{
		::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	}

	void SetRectEmpty()
	{
		::SetRectEmpty(this);
	}

	void CopyRect(LPCRECT lpSrcRect)
	{
		::CopyRect(this, lpSrcRect);
	}

	BOOL EqualRect(LPCRECT lpRect) const
	{
		return ::EqualRect(this, lpRect);
	}

	void InflateRect(int x, int y)
	{
		::InflateRect(this, x, y);
	}

	void InflateRect(SIZE size)
	{
		::InflateRect(this, size.cx, size.cy);
	}

	void InflateRect(LPCRECT lpRect)
	{
		left -= lpRect->left;
		top -= lpRect->top;
		right += lpRect->right;
		bottom += lpRect->bottom;
	}

	void InflateRect(int l, int t, int r, int b)
	{
		left -= l;
		top -= t;
		right += r;
		bottom += b;
	}

	void DeflateRect(int x, int y)
	{
		::InflateRect(this, -x, -y);
	}

	void DeflateRect(SIZE size)
	{
		::InflateRect(this, -size.cx, -size.cy);
	}

	void DeflateRect(LPCRECT lpRect)
	{
		left += lpRect->left;
		top += lpRect->top;
		right -= lpRect->right;
		bottom -= lpRect->bottom;
	}

	void DeflateRect(int l, int t, int r, int b)
	{
		left += l;
		top += t;
		right -= r;
		bottom -= b;
	}

	void OffsetRect(int x, int y)
	{
		::OffsetRect(this, x, y);
	}
	void OffsetRect(SIZE size)
	{
		::OffsetRect(this, size.cx, size.cy);
	}

	void OffsetRect(POINT point)
	{
		::OffsetRect(this, point.x, point.y);
	}

	void NormalizeRect()
	{
		int nTemp;
		if (left > right)
		{
			nTemp = left;
			left = right;
			right = nTemp;
		}
		if (top > bottom)
		{
			nTemp = top;
			top = bottom;
			bottom = nTemp;
		}
	}

	// absolute position of rectangle
	void MoveToY(int y)
	{
		bottom = Height() + y;
		top = y;
	}

	void MoveToX(int x)
	{
		right = Width() + x;
		left = x;
	}

	void MoveToXY(int x, int y)
	{
		MoveToX(x);
		MoveToY(y);
	}

	void MoveToXY(POINT pt)
	{
		MoveToX(pt.x);
		MoveToY(pt.y);
	}

	// operations that fill '*this' with result
	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{
		return ::IntersectRect(this, lpRect1, lpRect2);
	}

	BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{
		return ::UnionRect(this, lpRect1, lpRect2);
	}

	BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2)
	{
		return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
	}

	// Additional Operations
	void operator =(const RECT& srcRect)
	{
		::CopyRect(this, &srcRect);
	}

	BOOL operator ==(const RECT& rect) const
	{
		return ::EqualRect(this, &rect);
	}

	BOOL operator !=(const RECT& rect) const
	{
		return !::EqualRect(this, &rect);
	}

	void operator +=(SIZE size)
	{
		::OffsetRect(this, size.cx, size.cy);
	}

	void operator +=(LPCRECT lpRect)
	{
		InflateRect(lpRect);
	}

	void operator -=(SIZE size)
	{
		::OffsetRect(this, -size.cx, -size.cy);
	}

	void operator -=(LPCRECT lpRect)
	{
		DeflateRect(lpRect);
	}

	void operator &=(const RECT& rect)
	{
		::IntersectRect(this, this, &rect);
	}

	void operator |=(const RECT& rect)
	{
		::UnionRect(this, this, &rect);
	}


	CRect operator +(LPCRECT lpRect) const
	{
		CRect rect(this);
		rect.InflateRect(lpRect);
		return rect;
	}

	CRect operator +(SIZE size) const
	{
		CRect rect(*this);
		::OffsetRect(&rect, size.cx, size.cy);
		return rect;
	}

	CRect operator -(SIZE size) const
	{
		CRect rect(*this);
		::OffsetRect(&rect, -size.cx, -size.cy);
		return rect;
	}

	CRect operator -(LPCRECT lpRect) const
	{
		CRect rect(this);
		rect.DeflateRect(lpRect);
		return rect;
	}

	CRect operator &(const RECT& rect2) const
	{
		CRect rect;
		::IntersectRect(&rect, this, &rect2);
		return rect;
	}

	CRect operator |(const RECT& rect2) const
	{
		CRect rect;
		::UnionRect(&rect, this, &rect2);
		return rect;
	}

	CRect MulDiv(int nMultiplier, int nDivisor) const
	{
		return CRect(
			::MulDiv(left, nMultiplier, nDivisor),
			::MulDiv(top, nMultiplier, nDivisor),
			::MulDiv(right, nMultiplier, nDivisor),
			::MulDiv(bottom, nMultiplier, nDivisor));
	}
};
NAMESPACE_END(misc)