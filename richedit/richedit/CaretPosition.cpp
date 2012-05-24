/**
*@file CaretPosition.cpp
*@brief 光标位置相关操作
*
*包括光标位置定位，移动，删除
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/

#include "CaretPosition.h"
#include "document.h"
#include "window.h"

CaretPosition::CaretPosition()
{
	window_ = NULL;
	document_ = NULL;
	selectRange_ = NULL;
	caretCurShow_.charPos_ = 0;
	caretCurShow_.isLeft = true;
	caretCurShow_.line_ = 0;
	caretCurShow_.pt.x = 0;
	caretCurShow_.pt.y = 0;
}

void CaretPosition::Attach(Window* window, Document* document, Selection* selectRange)
{
	window_ = window;
	document_ = document;
	selectRange_ = selectRange;
}

void CaretPosition::SetCaretPos(int charPos, bool bLeft)
{
	POINT ptFix = {0, 0};
	CaretDetail caret;
	document_->GetCaretFromCharPos(charPos, ptFix, &caret, bLeft);
	caret.charPos_ = charPos;
	selectRange_->ClearSelection();
	caretCurShow_ = caret;
	caretCurShow_.isLeft = bLeft;
	ATLTRACE(_T("\n[UpdateCaretPos]caretCurShow_: charPos_:%d ; line_:%d; pt:[x:%d, y:%d] isLeft:%d \n")
		, caretCurShow_.charPos_
		, caretCurShow_.line_
		, caretCurShow_.pt.x
		, caretCurShow_.pt.y
		, caretCurShow_.isLeft ? 1 : 0);
	::SetCaretPos(ptFix.x, ptFix.y);
	caretCurShow_.Log(_T("SetCaretPos : caretCurShow_"));
	window_->ReDraw();
}

void CaretPosition::UpdateCaretPos(const POINT& pt)
{
	POINT ptFix;
	CaretDetail caret;
	int charPos = document_->FindCharPosFromPoint(pt, ptFix, &caret);
	if (charPos >= 0)
	{
		selectRange_->ClearSelection();
		caretCurShow_ = caret;
		ATLTRACE("\n[UpdateCaretPos]caretCurShow_: charPos_:%d ; line_:%d; pt:[x:%d, y:%d] isLeft:%d \n"
			, caretCurShow_.charPos_
			, caretCurShow_.line_
			, caretCurShow_.pt.x
			, caretCurShow_.pt.y
			, caretCurShow_.isLeft ? 1 : 0);
		::SetCaretPos(ptFix.x, ptFix.y);
		caretCurShow_.Log(_T("UpdateCaretPos : caretCurShow_"));
		window_->ReDraw();
	}		
}

int CaretPosition::GetCaretCharPos()
{
	return caretCurShow_.charPos_;
}

int CaretPosition::GetTextInsertPos()
{
	if (!caretCurShow_.isLeft)
		return (caretCurShow_.charPos_ + 1);
	return caretCurShow_.charPos_;
}


void CaretPosition::ShowCaret()
{
	::ShowCaret(window_->GetWnd());
}

void CaretPosition::HideCaret()
{
	::DestroyCaret();
}

void CaretPosition::UpdateCaretStyle()
{
	TEXTMETRIC tm;
	Window::AutoDC adc(window_->GetWnd());
	GetTextMetrics(adc.GetDC(),&tm);
	CreateCaret( window_->GetWnd(),NULL,3,tm.tmAscent);
}

void CaretPosition::MoveCaretPos(int x, int y)
{
	caretCurShow_.pt.x += x;
	caretCurShow_.pt.y += y;
	::SetCaretPos(caretCurShow_.pt.x, caretCurShow_.pt.y);
}

bool CaretPosition::IsLeft()
{
	return caretCurShow_.isLeft;
}

void CaretPosition::UpdateInsert(int length)
{
	int pos = GetCaretCharPos() + length;
	SetCaretPos(pos, caretCurShow_.isLeft);
}

void CaretPosition::Delete(bool bBack)
{
	int caretCharPos = 0;
	int totalData = document_->GetDataCount();
	bool bLeft = true;
	if(selectRange_->IsSelected())
	{
		caretCharPos = selectRange_->Begin() - 1;
		if (selectRange_->End() == totalData - 1)
			bLeft = false;
		document_->DeleteRange(selectRange_->Begin(), selectRange_->End());
	}
	else
	{	
		caretCharPos = GetCaretCharPos();
		if (caretCharPos < 0 
			|| (!caretCurShow_.isLeft && (caretCharPos == totalData - 1) && !bBack))
			return ;

		if (caretCharPos == totalData - 1)
			bLeft = false;

		int deletePos = caretCharPos;
		if (bBack && caretCurShow_.isLeft)//for the back operation
			deletePos--;
		if (deletePos < 0)
			return ;

		document_->DeletePos(deletePos);

		if ( bBack )
			caretCharPos--;
	}
	SetCaretPos(caretCharPos, bLeft);
}

void CaretPosition::UpdatePosFromPt(const POINT& pt)
{
	POINT ptFix;
	CaretDetail caret;
	if (pt.x >= 50)
	{
		int gg = 0;
	}
	int charPos = document_->FindCharPosFromPoint(pt, ptFix, &caret);
	if (charPos >= 0)
	{
		//caretCurShow_.Log(_T("caretCurShow_"));
		//caret.Log(_T("caret"));

		//ATLTRACE(_T("\n charPos: %d HitTestPt:pt.x:%d, pt.y:%d\n"), charPos, pt.x, pt.y);

		::SetCaretPos(ptFix.x, ptFix.y);
		if (caretCurShow_.IsEqual(caret))
		{
			//caretCurShow_.Log(_T("caretCurShow_ ,equal"));
			selectRange_->ClearSelection();
		}
		else
		{
			//caretCurShow_.Log(_T("caretCurShow_ ,not equal"));
			selectRange_->Begin_ = caretCurShow_.charPos_;
			if (charPos > selectRange_->Begin_)
				selectRange_->UpdateSelection(caret.isLeft ? (charPos - 1) : charPos);
			else
				selectRange_->UpdateSelection(caret.isLeft ? charPos : (charPos + 1) );
			//selectRange_->Log(_T("selectRange_"));
		}
		window_->ReDraw();
	}
}

void CaretPosition::MoveCaret(int nType)
{
	switch (nType)
	{
	case CMD_LEFT:
		MoveLeft();
		break;
	case CMD_TOP:
		MoveTop();
		break;
	case CMD_RIGHT:
		MoveRight();
		break;
	case CMD_BOTTOM:
		MoveBottom();
		break;
	default:
		break;
	}
}

void CaretPosition::MoveLeft()
{
	int charPos = GetCaretCharPos();
	if (charPos == 0)
		return ;
	
	LineObj* line = document_->GetLineFromCharPos(charPos);
	int newLineIndex = line->lineIndex_;
	int newCharPos = 0;
	bool bLeft = true;
	if (charPos == line->beginPos_ && caretCurShow_.isLeft)//go to the upper line
	{
		bLeft = false;
		newCharPos = line->beginPos_ - 1;
		newLineIndex--;
	}else if (charPos == line->endPos_ && !caretCurShow_.isLeft)// if it is a right set it to left
	{
		newCharPos = charPos;
	}else
	{
		MY_CHAR* dataTemp = new MY_CHAR[3];
		document_->GetData(dataTemp, line->beginPos_, 3);
		if (charPos == (line->beginPos_ + 1) &&  dataTemp[0] == '\n')// go to the upper line
		{
			newCharPos = line->beginPos_ - 1;
			bLeft = false;
			newLineIndex--;
		}
		else
		{
			newCharPos = charPos - 1;
		}

		delete[] dataTemp;
	}

	if (newCharPos < 0)
	{
		assert(false);
		return ;
	}
	
	POINT pt;
	GetCaretPointFromCharPos(newCharPos, newLineIndex, pt, bLeft);
	caretCurShow_.charPos_ = newCharPos;
	caretCurShow_.isLeft = bLeft;
	caretCurShow_.line_ = newLineIndex;
	caretCurShow_.pt = pt;
	::SetCaretPos(pt.x, pt.y);
}

void CaretPosition::MoveTop()
{
	int charPos = GetCaretCharPos();
	LineObj* line = document_->GetLineFromCharPos(charPos);
	bool bLeft = true;
	int lineIndex = line->lineIndex_;
	if (lineIndex == 0)//go to the upper line
		return;

	POINT pt;
	pt.x = document_->CalculateSelectionLength(line->beginPos_, charPos - 1);
	pt.y = line->yPos_ - 2 - document_->GetDetails()->yViewPos_;
	UpdateCaretPos(pt);
}

void CaretPosition::MoveRight()
{
	int charPos = GetCaretCharPos();
	int dataTotalCount = document_->GetDataCount();
	if (charPos == (dataTotalCount - 1) && !caretCurShow_.isLeft)
		return ;

	LineObj* line = document_->GetLineFromCharPos(charPos);
	int newLineIndex = line->lineIndex_;
	int newCharPos = charPos;
	bool bLeft = true;
	if (charPos == line->endPos_ )//go to the upper line
	{
		if (!caretCurShow_.isLeft)
		{
			MY_CHAR* dataTemp = new MY_CHAR[ 2];
			document_->GetData(dataTemp, line->endPos_ + 1, 2);
			if (dataTemp[0] == '\n')// go to the upper line
				newCharPos += 2;
			else
				newCharPos++;
			newLineIndex++;
			delete[] dataTemp;
		}
		else
		{
			bLeft = false;
		}
		
	}
	else
	{
		newCharPos += 1;		
	}

	if (newCharPos >= document_->GetDataCount())
	{
		assert(false);
		return ;
	}

	POINT pt;
	GetCaretPointFromCharPos(newCharPos, newLineIndex, pt, bLeft);
	caretCurShow_.charPos_ = newCharPos;
	caretCurShow_.isLeft = bLeft;
	caretCurShow_.line_ = newLineIndex;
	caretCurShow_.pt = pt;
	::SetCaretPos(pt.x, pt.y);
}

void CaretPosition::MoveBottom()
{
	int charPos = GetCaretCharPos();
	LineObj* line = document_->GetLineFromCharPos(charPos);
	bool bLeft = true;
	int lineIndex = line->lineIndex_;
	if (lineIndex == (document_->GetLineCount() - 1))//go to the upper line
	{
		return;
	}

	POINT pt;
	pt.x = document_->CalculateSelectionLength(line->beginPos_, charPos - 1);
	pt.y = line->yPos_ + line->GetDetail()->maxHeight_ + 2 - document_->GetDetails()->yViewPos_;
	UpdateCaretPos(pt);
}

void CaretPosition::GetCaretPointFromCharPos(char pos, int lineIndex, POINT& ptFind, bool bLeft)
{
	LineObj* line = document_->GetLineFromIndex(lineIndex);
	assert(line);

	if ( bLeft )
		pos -= 1;
	int width = document_->CalculateSelectionLength(line->beginPos_, pos);
	
	ptFind.x = width;
	ptFind.y = line->GetDetail()->maxHeight_ + line->yPos_ - CARE_HEIGHT;

}