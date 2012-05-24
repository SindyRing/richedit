/**
*@file editor.cpp
*@brief editor类
*
*完整的一个editor与系统交互层面逻辑和提供接口调用
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include "editor.h"
#include "drawer.h"
#include "UniConversion.h"
#include "DataObj.h"

/************************************************************************/
/* CreateDataObject                                                     */
/************************************************************************/
HRESULT CreateDataObject (IDataObject **ppDataObject, MY_CHAR* c)
{
	if(ppDataObject == 0)
		return E_INVALIDARG;

	*ppDataObject = new CDataObject(c);

	return (*ppDataObject) ? S_OK: E_OUTOFMEMORY;
}

/************************************************************************/
/* RichEditor		                                                     */
/************************************************************************/

RichEditor::RichEditor()
{
	bTraceMouse_ = true;
	bLBButtonDown_ = false;
	bReadOnly_ = false;
	GetDefaultCharformat(cfDefault_);
	crBkgrd_ = RGB(112,112,112);
	ZeroMemory(&details_, sizeof(details_));
}

void RichEditor::Attach(HWND hWnd)
{
	window_.Attach(hWnd);
	document_.Attach(&window_, &selectRange_);
	document_.SetDetails(&details_);
}

void RichEditor::Release()
{
	delete this;
}

void RichEditor::Paint()
{
	//
	CPaintDC dc(window_.GetWnd());
	CRect rcClient, rcUpdate;
	GetRect(rcClient);

	::GetUpdateRect(window_.GetWnd(), &rcUpdate, FALSE);
	Paint(dc.m_hDC, rcClient, rcClient);	
}

void RichEditor::Paint(HDC dcPaint,RECT& rcClient, RECT& rcUpdate)
{
	CRect rcNeedUpdate;
	::IntersectRect(&rcNeedUpdate, &rcClient, &rcUpdate);

	CRect rcClientTemp = rcNeedUpdate;
	CDCHandle dcMember;
	dcMember.CreateCompatibleDC(dcPaint);

	//rcClientTemp = rcNeedUpdate;
	HBITMAP bitMapMember = ::CreateCompatibleBitmap(dcPaint, rcClientTemp.Width(), rcClientTemp.Height());
	HBITMAP bitMapOld = (HBITMAP)::SelectObject(dcMember, (HGDIOBJ)bitMapMember);
	dcMember.FillSolidRect(rcClientTemp.left, rcClientTemp.top, rcClientTemp.Width(), rcClientTemp.Height(), crBkgrd_);

	LineObj* obj = document_.GetLineFromPos(rcNeedUpdate.top );
	int totalHeight = rcClientTemp.Height();
	int nCurPaintHeight = 0;
	while(obj)
	{
		nCurPaintHeight = obj->yPos_ - details_.yViewPos_;
		if (nCurPaintHeight > totalHeight)
			break;
		LineDrawer lineDraw(obj, &document_, dcMember);
		lineDraw.Paint(&window_, selectRange_);
		obj = document_.GetLineFromIndex(obj->lineIndex_+1);
	}
	BOOL bSuc = ::BitBlt(dcPaint, rcClientTemp.left, rcClientTemp.top,  rcClientTemp.Width(), rcClientTemp.Height(), dcMember, 0, 0, SRCCOPY);
	::SelectObject(dcMember, (HGDIOBJ)bitMapOld);
	::DeleteObject((HGDIOBJ)bitMapMember);
	::DeleteDC(dcMember);
}


void RichEditor::InsertString(const MY_CHAR* c, int pos, int length,CHARFORMAT& cf, MY_CHAR* hyperLinkData)
{
	document_.InsertString(c, pos, length, cf, hyperLinkData);
	UpdateScrollBar(SB_BOTH);
}

void RichEditor::InsertObj(IObj* obj, int pos)
{
	document_.InsertObj(obj, pos);
	UpdateScrollBar(SB_BOTH);
}

void RichEditor::InsertLink(const MY_CHAR* c, int length, int pos, MY_CHAR* hyperLinkData)
{
	//document_.InsertString()
}

void RichEditor::SetReadOnly(bool bReadOnly)
{
	bReadOnly_ = bReadOnly;
}

void RichEditor::SetBkgrdColor(COLORREF crbkgrd)
{
	crBkgrd_ = crbkgrd;
}

void RichEditor::SetSelectedColor(COLORREF crSelected)
{
	window_.SetSelectColor(crSelected);
}

void RichEditor::ScrollToEnd()
{
	Scroll(0, document_.GetLogicHeight(), true);
}

void RichEditor::SetParagraphIdent(int ident)
{
	document_.SetParagraphIdent(ident);
}

void RichEditor::GetWindowText(MY_CHAR* src)
{
	document_.GetData(src, 0, document_.GetDataCount());
}

void RichEditor::GetSelText(MY_CHAR* src)
{
	document_.GetData(src, selectRange_.Begin(), selectRange_.End() - selectRange_.End() + 1);
}

void RichEditor::GetText(MY_CHAR* src, int pos, int length)
{
	document_.GetData(src, 0, length);
}

bool RichEditor::IsReadOnly() const
{
	return bReadOnly_;
}

#ifdef USE_TEST

static bool s_x = true;
static bool s_insert = true;
#include "TestObj.h"

void Getformat(CHARFORMAT& cfdefault, COLORREF cr, int yHeight)
{
	cfdefault.cbSize = sizeof(CHARFORMAT);
	cfdefault.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cfdefault.dwEffects = 0;
	cfdefault.yHeight = yHeight;
	cfdefault.yOffset = 0;
	cfdefault.crTextColor = cr;
	GetSystemFontName(cfdefault.szFaceName);
}
#include <atlmisc.h>
#include <atltime.h>
void RichEditor::Test()
{
	if (s_insert)
	{
		SetBkgrdColor(RGB(255,255,255));
		MY_CHAR* name=_T("Sindy:");
		MY_CHAR* nameLink = _T("www.baidu.com");
		
		ATL::CTime tempTime(::GetCurrentTime());
		WTL::CString strTime = tempTime.Format(_T("  %H:%M:%S  \n"));
		MY_CHAR* text1 = _T("广东阶梯电价听证 电厂电网代表卖力喊穷");
		MY_CHAR* textBlue = _T("藍色 ");
		MY_CHAR* textRed= _T("紅色 ");
		MY_CHAR* textGreen = _T("綠色");
		MY_CHAR* text2 = _T("1234567890123456789012345678901234567890123456789012345678901234567890\n2345678901");
		MY_CHAR* splitter = _T("\n\n");

		SetParagraphIdent(5);
		CMyImageObj* obj = new CMyImageObj();
		CHARFORMAT cfName,cfTime, cfColor;
		Getformat(cfName, RGB(0,0,255), 15);
		Getformat(cfTime, RGB(99,99,99), 15);
		cfName.dwEffects |= CFM_LINK;
		cfName.dwMask |= CFM_UNDERLINE;
		SetParagraphIdent(0);
		InsertString(name, document_.GetDataCount(), lstrlen(name), cfName, nameLink);
		InsertString(strTime.GetBuffer(0), document_.GetDataCount(), strTime.GetLength(), cfTime);

		SetParagraphIdent(10);
		InsertString(text1, document_.GetDataCount(), lstrlen(text1), cfDefault_);
		Getformat(cfColor, RGB(0,0,255), 15);
		InsertString(textBlue, document_.GetDataCount(), lstrlen(textBlue), cfColor);
		Getformat(cfColor, RGB(255,0,0), 14);
		InsertString(textRed, document_.GetDataCount(), lstrlen(textRed), cfColor);
		Getformat(cfColor, RGB(0,255,0), 12);
		InsertString(textGreen, document_.GetDataCount(), lstrlen(textGreen), cfColor);
		InsertObj(obj, document_.GetDataCount() - 9);
		InsertString(splitter, document_.GetDataCount(), lstrlen(splitter), cfDefault_);
		//InsertString(splitter, document_.GetDataCount(), lstrlen(splitter), cfText);
		
		s_x = false;

	}
}

#endif

LRESULT RichEditor::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		bHandled = TRUE;
		return 1;
		break;
	case WM_CREATE:
		{
			details_.yViewPos_ = 0;
			details_.xViewPos_ = 0;
			::ShowWindow(window_.GetWnd(), WM_SHOWWINDOW);

			misc::CRect rcClient;
			GetRect(rcClient);
			if (rcClient.IsRectEmpty())
				::SetWindowPos(window_.GetWnd(), NULL, 0, 0, 50, 50, SWP_NOMOVE);

			caretPosition_.Attach(&window_, &document_, &selectRange_);
			caretPosition_.UpdateCaretStyle();
			UpdateScrollBar(SB_BOTH);
		}
		break;
	case WM_PAINT:
		Paint();
		break;
	case WM_GETMINMAXINFO:
		return ::DefWindowProc(window_.GetWnd(), uMsg, wParam, lParam);
	case WM_LBUTTONDOWN:
		OnLBButtonDown(wParam, lParam, bHandled);
		break;
	case WM_LBUTTONUP:
		OnLBButtonUp(wParam, lParam, bHandled);
		break;
	case WM_IME_COMPOSITION:
		OnIMEConPositon(wParam, lParam, bHandled);
		break;
	case WM_IME_CHAR: 
	case WM_CHAR:
		OnChar(wParam, lParam, bHandled);
		break;	
	case WM_SETTEXT:
		break;
	case WM_SIZE:
		OnSize(wParam, lParam, bHandled);
		break;
	case WM_HSCROLL:
		OnHScroll(wParam, lParam, bHandled);
		break;
	case WM_VSCROLL:
		OnVScroll(wParam, lParam, bHandled);
		break;
	case WM_SETFOCUS:
		OnSetFocus(wParam, lParam, bHandled);
		break;
	case WM_KILLFOCUS:
		OnKillFocus(wParam, lParam, bHandled);
		break;
	case WM_SETCURSOR:
		::SetCursor(LoadCursor(NULL, IDC_IBEAM));
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam, bHandled);
		break;
	case WM_MOUSELEAVE:
		OnMouseLeave(wParam, lParam, bHandled);
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam, lParam, bHandled);
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(wParam, lParam, bHandled);
		break;
	case WM_LBUTTONDBLCLK:
		break;
#ifdef USE_TEST
// 	case WM_ACTIVATE:
// 		Test();
		break;
	case WM_MBUTTONDOWN:
		{
			s_insert = true;
			int curTick = ::GetTickCount();
			Test();
			int distance = ::GetTickCount() - curTick;
			ATLTRACE(_T("\n one test use tick count: %d  \n"), distance);
		}	
		break;
#endif
	default:
		break;
	}
	return 0;
}


LRESULT RichEditor::OnIMEConPositon(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (IsReadOnly())
		return 0;
	if (lParam & GCS_RESULTSTR) {
		HIMC hIMC = ::ImmGetContext(window_.GetWnd());
		if (hIMC) {
			const int maxLenInputIME = 200;
			wchar_t wcs[maxLenInputIME];
			LONG bytes = ::ImmGetCompositionStringW(hIMC,
				GCS_RESULTSTR, wcs, (maxLenInputIME-1)*2);
			int wides = bytes / 2;
			if (::IsWindowUnicode(window_.GetWnd())) {
				//char utfval[maxLenInputIME * 3];
				unsigned int len = UTF8Length(wcs, wides);
				//UTF8FromUTF16(wcs, wides, utfval, len);
				wcs[wides] = '\0';
				document_.InsertString(wcs, caretPosition_.GetTextInsertPos(), wides, cfDefault_);
				caretPosition_.UpdateInsert(wides);
				//AddCharUTF(utfval, len);
			} else {
				assert(false);
// 				char dbcsval[maxLenInputIME * 2];
// 				int size = ::WideCharToMultiByte(InputCodePage(),
// 					0, wcs, wides, dbcsval, sizeof(dbcsval) - 1, 0, 0);
// 				for (int i=0; i<size; i++) {
// 					AddChar(dbcsval[i]);
// 				}
			}
			// Set new position after converted
// 			Point pos = PointMainCaret();
// 			COMPOSITIONFORM CompForm;
// 			CompForm.dwStyle = CFS_POINT;
// 			CompForm.ptCurrentPos.x = pos.x;
// 			CompForm.ptCurrentPos.y = pos.y;
// 			::ImmSetCompositionWindow(hIMC, &CompForm);
			::ImmReleaseContext(window_.GetWnd(), hIMC);
		}
		bHandled = TRUE;
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT RichEditor::OnChar(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (IsReadOnly())
		return 0;

	if (((wParam >= 128) || !iscntrl(wParam)) ) {
		CHARFORMAT cf;                                   
		GetDefaultCharformat(cf);
		if (::IsWindowUnicode(window_.GetWnd())) 
		{
			wchar_t wcs[2] = {wParam, 0};
			int len = UTF8Length(wcs, 1);
			InsertString(wcs, caretPosition_.GetTextInsertPos(), 1, cf);
			caretPosition_.UpdateInsert(1);
		}
		else
		{
			assert(false);
			// 				char* wcs = wParam;
			// 				MY_CHAR utfval[2];
			// 				UTF16FromUTF8(wcs, 1, utfval, 1);
			// 				document_.InsertString(utfval, caretPosition_.GetCaretCharPos(), 1, cf);
		}
	}
	return 0;
}

LRESULT RichEditor::OnLBButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bLBButtonDown_ = true;
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	caretPosition_.UpdateCaretPos(pt);
	
	::SetFocus(window_.GetWnd());
	::SetCapture(window_.GetWnd());
	return 0;
}

LRESULT RichEditor::OnLBButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (bLBButtonDown_)
		::ReleaseCapture();

	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	if (!selectRange_.IsSelected())
		document_.LinkHitTest(pt, WM_LBUTTONUP);
	bLBButtonDown_ = false;
	return 0;
}

LRESULT RichEditor::OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(bTraceMouse_)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = window_.GetWnd();
		tme.dwFlags = TME_LEAVE;
		::_TrackMouseEvent(&tme);
		bTraceMouse_ = false;
	}

	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	if(bLBButtonDown_)		
		caretPosition_.UpdatePosFromPt(pt);
	else
	{
		if( document_.IsHitTestLink(pt) )
			::SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	return 0;
}

LRESULT RichEditor::OnMouseLeave(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bTraceMouse_ = true;
	return 0;
}

LRESULT RichEditor::OnSetFocus(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	caretPosition_.ShowCaret();
	return 0;
}

LRESULT RichEditor::OnKillFocus(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//caretPosition_.HideCaret();
	return 0;
}

LRESULT RichEditor::OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nWidth = LOWORD(lParam); 
	int nHeight = HIWORD(lParam);
	BOOL bWidthChange = (details_.rectDoc_.Width() != nWidth);
	details_.rectDoc_.right = details_.rectDoc_.left + nWidth;
	details_.rectDoc_.bottom = details_.rectDoc_.top + nHeight;
	if (bWidthChange)
	{
		//fix_me, we must update the document range
	}
	return 0;
}

LRESULT RichEditor::OnHScroll(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT RichEditor::OnVScroll(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnScroll(SB_VERT, wParam & 0x0000FFFF);
	return 0;
}

LRESULT RichEditor::OnMouseWheel(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	Scroll(0, -zDelta);
	return 0;
}



LRESULT RichEditor::OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bCtrl = (GetKeyState(VK_CONTROL) & 0x8000);
	BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000);
	if (bCtrl)
	{
		switch(wParam)
		{
		case 'C':
		case 'c':
			CopyData();
			break;
		case 'V':
		case 'v':
			PasteData();
			break;
		default:
			break;
		}
	}
	switch(wParam)
	{
	case VK_BACK:
		if(!IsReadOnly())
			caretPosition_.Delete(true);
		break;
	case VK_DELETE:
		if(!IsReadOnly())
			caretPosition_.Delete(false);
		break;
	case VK_RETURN:
		if(!IsReadOnly())
		{
			InsertString(_T("\n"), caretPosition_.GetTextInsertPos(), 1, cfDefault_);
			caretPosition_.UpdateInsert(1);
		}		
		break;
	case VK_UP:
		caretPosition_.MoveCaret(CaretPosition::CMD_TOP);
		break;
	case VK_DOWN:
		caretPosition_.MoveCaret(CaretPosition::CMD_BOTTOM);
		break;
	case VK_LEFT:
		caretPosition_.MoveCaret(CaretPosition::CMD_LEFT);
		break;
	case VK_RIGHT:
		caretPosition_.MoveCaret(CaretPosition::CMD_RIGHT);
		break;
	default:
		break;
	}
	return 0;
}

void RichEditor::GetRect(RECT& rcClient)
{
	::GetClientRect(window_.GetWnd(), &rcClient);
}


void RichEditor::CopyData()
{
//	OleInitialize (0); use this must initialize OLE
	IDataObject *pDataObject;
	int sel = selectRange_.GetSelectionLenght();
	if ( sel > 0)
	{
		MY_CHAR* c = new MY_CHAR[sel + 1];
		document_.GetData(c, selectRange_.Begin(), sel);
		c[sel] = '\0';
		// create the data object
		if (CreateDataObject(&pDataObject, c) == S_OK)
		{
			// add data to the clipboard
			OleSetClipboard(pDataObject);
			OleFlushClipboard();
			pDataObject->Release();
		}
		delete[] c;
	}

	 //OleUninitialize();
}

extern FORMATETC g_rgFETC[];

void RichEditor::PasteData()
{
	if (IsReadOnly())
		return ;
	IDataObject *pDataObject = NULL;
	OleGetClipboard(&pDataObject);
	if(pDataObject)
	{
		FORMATETC* pFormatEtc = &g_rgFETC[0];
		STGMEDIUM pStgMedium;
		ZeroMemory(&pStgMedium, sizeof(pStgMedium));
		if ( pDataObject->GetData(pFormatEtc, &pStgMedium) == S_OK)
		{
			selectRange_.ClearSelection();
			if (pStgMedium.tymed == TYMED_HGLOBAL)
			{
				switch(pFormatEtc->cfFormat)
				{
				case CF_UNICODETEXT:
					{
						wchar_t* lpStr = (wchar_t*)::GlobalLock(pStgMedium.hGlobal);
						int len = lstrlen(lpStr);
						DWORD xx = ::GetTickCount();
						InsertString(lpStr, len, caretPosition_.GetTextInsertPos(), cfDefault_);
						DWORD use = GetTickCount() - xx;
						ATLTRACE(_T("\n+++++++++++insert char %d use time: %d +++++++++++++\n"), len, use);
						caretPosition_.UpdateInsert(len);
						ScrollToEnd();
					}
					break;
				case CF_TEXT:
					{
						char* lpStr = (char*)::GlobalLock(pStgMedium.hGlobal);
						MY_CHAR* temp = CA2W(lpStr);
						int len = lstrlen(temp);
						InsertString(temp, len, caretPosition_.GetTextInsertPos(), cfDefault_);
						caretPosition_.UpdateInsert(len);
						ScrollToEnd();
					}
					break;
				}
			}
		}
	}
}

void RichEditor::Scroll(int x, int y, bool bForce)
{
	if ( x == 0 && y == 0 )
		return ;

	int fixX = x,fixY = y;

	int totalHeight = document_.GetLogicHeight();

	int maxYViewPos = totalHeight - details_.rectDoc_.Width();
	if (maxYViewPos <= 0)
		return ;

	if ((details_.yViewPos_ + fixY) > maxYViewPos)
		fixY = maxYViewPos - details_.yViewPos_;
	else if (details_.yViewPos_ + fixY < 0)
	{
		fixY = -details_.yViewPos_;
	}


	details_.xViewPos_ += x;
	details_.yViewPos_ += fixY;
	caretPosition_.MoveCaretPos( -x, -fixY);

	UpdateScrollBar(SB_VERT);
	window_.ReDraw();
}

void RichEditor::OnScroll(int nBar, int nOperate)
{
	misc::CRect rcClient;
	if(nBar == SB_VERT)
	{
		switch(nOperate)
		{
		case SB_LINELEFT: Scroll(0, -20); break;
		case SB_LINERIGHT: Scroll(0, 20); break;
		case SB_PAGELEFT: Scroll(0, -document_.GetLogicHeight()); break;
		case SB_PAGERIGHT: Scroll(0, document_.GetLogicHeight()); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				SCROLLINFO si;
				si.cbSize	= sizeof(SCROLLINFO);
				si.fMask	= SIF_ALL;
				::GetScrollInfo(window_.GetWnd(),nBar, &si);
				Scroll(0, si.nTrackPos - document_.GetDetails()->yViewPos_,TRUE);
				/*
				if( m_sizeWindowSize.cy > rcClient.Height() * 15 )
				{
					EnableTimerTickCount(TRUE,TIMER_STATE_DELAY_SCROLL);
				}else
				{
					SCROLLINFO si;
					si.cbSize	= sizeof(SCROLLINFO);
					si.fMask	= SIF_ALL;
					m_spVScroll->GetScrollInfo(&si);
					Scroll(0, si.nTrackPos - m_ptViewPos.y,TRUE);
				}
				*/

				break;
			}
// 		case SB_LEFT: Scroll(0, -m_ptViewPos.y); break;
// 		case SB_RIGHT: Scroll(0, m_sizeWindowSize.cy); break;
		case SB_ENDSCROLL:
		default:
			break;
		}
	}
	else //if(nBar == SB_HORZ)
	{
		switch(nOperate)
		{
		case SB_LINELEFT: Scroll(-20, 0); break;
		case SB_LINERIGHT: Scroll(20, 0); break;
		case SB_PAGELEFT: Scroll(-rcClient.Width(), 0); break;
		case SB_PAGERIGHT: Scroll(rcClient.Width(), 0); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				SCROLLINFO si;
				si.cbSize	= sizeof(SCROLLINFO);
				si.fMask	= SIF_ALL;
				::GetScrollInfo(window_.GetWnd(),nBar, &si);
				Scroll(0, si.nTrackPos - document_.GetDetails()->xViewPos_,TRUE);
				break;
			}
// 		case SB_LEFT: Scroll(-m_ptViewPos.x, 0); break;
// 		case SB_RIGHT: Scroll(m_sizeWindowSize.cx, 0); break;
		case SB_ENDSCROLL:
		default:
			break;
		}
	}
}

void RichEditor::UpdateScrollBar( int nBar)
{
	if(nBar == SB_VERT || nBar == SB_BOTH)
	{
		// now never supporst  SB_HORZ
		SCROLLINFO si = {0};
		misc::CRect rcClient;
		GetRect(rcClient);
		if(IsScrollVisible(SB_VERT))
		{
			si.cbSize	= sizeof(SCROLLINFO);
			si.fMask	= SIF_ALL;
			::GetScrollInfo(window_.GetWnd(), nBar, &si);
			si.nMin		= 0;
			si.nMax		= document_.GetLogicHeight();//m_szFolderViewCtrl.cy;
			si.nPos		= document_.GetDetails()->yViewPos_;
			si.nPage	= rcClient.Height();
			si.fMask	= SIF_PAGE | SIF_RANGE | SIF_POS | SIF_DISABLENOSCROLL;
			::SetScrollInfo(window_.GetWnd(), nBar, &si, true);
		}


	}
}

BOOL RichEditor::IsScrollVisible(int nBar)
{
	return TRUE;
}