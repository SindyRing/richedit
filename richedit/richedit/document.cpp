/**
*@file document.cpp
*@brief 文档类
*
*文档数据的相关操作
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include "line.h"
#include "document.h"
#include "CaretPosition.h"

Document::Document()
{
	ident_ = 0;
}

void Document::Attach(Window* wnd, Selection* sel)
{
	window_ = wnd;
	selectionRange_ = sel;
}

void Document::Log()
{
#ifndef USE_LOG
	return ;
#else
	MY_CHAR* dataTemp = new MY_CHAR[data.Length()];
	GetData(dataTemp, 0, data.Length());
	ATLTRACE("\nData:\n%s\n", dataTemp);
	delete[] dataTemp;
#endif
}

void Document::LogAll()
{
#ifndef USE_LOG
	return ;
#else
	Log();
	styleList_.Log();
	LineObjList_.Log();
#endif
}

void Document::InsertString(const MY_CHAR* c, int pos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData)
{
	int begin = 0;
	int begin1 = 0;
	int end = length;
	const MY_CHAR* cTemp = c;
	int nCharLengthTime = 0;
	int nDataTime = 0;
	int nStyleTime = 0;
	int updateLineTime = 0;

	int x = ::GetTickCount();
	charLengthList_.InsertValue(pos, length, 0);
	nCharLengthTime = ::GetTickCount() - x; 

	x = ::GetTickCount();
	data.InsertFromArray(pos, c, 0, length);
	nDataTime = ::GetTickCount() - x; 

	x = ::GetTickCount();
	styleList_.InsertStyle(pos, length, cf, hyperLinkData);
	nStyleTime = ::GetTickCount() - x; 

	x = ::GetTickCount();
	UpdateLineManager_insert(pos, length);
	updateLineTime = ::GetTickCount() - x; 

	ATLTRACE(_T("\n+++++TotalInsert:%d nCharLengthTime:%d nDataTime:%d nStyleTime:%d updateLineTime:%d++++++++++++\n")
		,length, nCharLengthTime, nDataTime, nStyleTime, updateLineTime);
	LineObjList_.Log();
	ReDraw();
}

void Document::InsertObj(IObj* obj, int charPos)
{
	charLengthList_.InsertValue(charPos, 1, 0);
	styleList_.Log();
	styleList_.InsertStyle(charPos, obj);
	styleList_.Log();
	data.Insert(charPos, '%');
	UpdateLineManager_insert(charPos, 1);
	LineObjList_.Log();
	ReDraw();
}

void Document::DeleteRange(int begin, int end)
{
	charLengthList_.DeleteRange(begin, end - begin + 1);
	data.DeleteRange(begin, end - begin + 1);
	UpdateStyleManager_delete(begin, end);
	UpdateLineManager_delete(begin, end);
}

// do not ensure the pos is valid
void Document::DeletePos(int pos)
{
	DeleteRange(pos, pos);
}

LineObj* Document::GetLineFromPos(int yPos)
{
	return LineObjList_.GetLineFromYPos(yPos + details_->yViewPos_);
}

LineObj* Document::GetLineFromIndex(int index)
{
	return LineObjList_.GetLineObjFromIndex(index);
}

LineObj* Document::GetLineFromCharPos(int charPos)
{
	return LineObjList_.GetLineFromCharPos(charPos);
}

LineObj* Document::GetFirstVisbleLine()
{
	return LineObjList_.GetLineFromYPos(details_->yViewPos_);
}

int Document::FindCharPosFromPoint(const POINT& pt, POINT& ptFix, CaretDetail* caret)
{
	LineObj* line = LineObjList_.GetLineFromYPos(pt.y + details_->yViewPos_);
	bool bLeft = true;
	int xPos = 0;	
	if (line)
	{
		int totalWidth = GetSelectionLength(line->beginPos_, line->endPos_, line);
		int nPosBegin = line->beginPos_;
		while(nPosBegin <= line->endPos_)
		{
			int nWidth = line->GetDetail()->ident_;
			xPos = xPos < nWidth ? nWidth : xPos;
			if (nPosBegin == line->beginPos_)
				nWidth += GetPosLength(nPosBegin);
			else
				nWidth += GetSelectionLength(line->beginPos_, nPosBegin, line);
			if ( nWidth > pt.x )
			{
				int leave = abs(pt.x - nWidth);
				int nCharWidth = GetPosLength(nPosBegin);
				if (nCharWidth > leave * 2)
				{
					xPos = nWidth;
					if (nPosBegin == line->endPos_)
						bLeft = false;
					nPosBegin++;
				}
				break;
			}
			else
				xPos = nWidth;
			nPosBegin++;
		}
		ptFix.y = line->yPos_ + line->GetDetail()->maxHeight_ - CARE_HEIGHT;
		ptFix.x = xPos;
		ptFix.x -= details_->xViewPos_;
		ptFix.y -= details_->yViewPos_;
		nPosBegin = nPosBegin > line->endPos_ ? line->endPos_ : nPosBegin;
		if (caret)
		{
			caret->charPos_ = nPosBegin;
			caret->isLeft = (ptFix.x >= totalWidth) ? false : bLeft;
			caret->line_ = line->lineIndex_;
			caret->pt = ptFix;
		}
		return nPosBegin;
	}
	return -1;
}

int Document::GetCaretFromCharPos(int charPos, POINT& ptFix, CaretDetail* caret, bool bLeft)
{
	LineObj* line = LineObjList_.GetLineFromCharPos(charPos);
	if (line)
	{
		ptFix.y = line->yPos_ + line->GetDetail()->maxHeight_ - CARE_HEIGHT - details_->yViewPos_;
		int endPos = bLeft ? (charPos - 1) : charPos;
		if (charPos >= 0)
			ptFix.x = GetSelectionLength(line->beginPos_, endPos, line);
		if (caret)
		{
			caret->charPos_ = charPos;
			caret->isLeft =  (charPos == line->endPos_) ? false : true;
			caret->line_ = line->lineIndex_;
			caret->pt = ptFix;
		}
	}
	return charPos;
}

void Document::SetParagraphIdent(int ident)
{
	ident_ = ident;
}

StyleObj* Document::GetStyleObjFromCharPos(int charPos)
{
	return styleList_.GetStyleObjFromCharPos(charPos);
}

StyleObj* Document::GetStyleObj(int index)
{
	return styleList_.GetStyleObj(index);
}

int Document::FindStyleFromCharPos(int charPos)
{
	return styleList_.FindStylePosFromCharPos(charPos);
}

bool Document::IsHitTestLink(const POINT& pt)
{
	StyleObj* obj = GetStyleObjFromPoint(pt);
	return (obj && obj->IsLink());
}

bool Document::LinkHitTest(const POINT& pt, UINT msg)
{
	StyleObj* obj = GetStyleObjFromPoint(pt);
	if( obj && obj->IsLink())
	{
		selectionRange_->Begin_ = obj->charPos_;
		selectionRange_->End_ = obj->charPos_ + obj->charLength_ - 1;
		window_->ReDraw();
		if (IsLink( obj->link_->context_ ))
			::ShellExecute(0, _T("open"), obj->link_->context_ , 0, 0, SW_SHOWNORMAL);
		else
		{
			ENLINK enLink;
			ZeroMemory(&enLink, sizeof(enLink));
			enLink.nmhdr.code = EN_LINK;
			enLink.nmhdr.hwndFrom = window_->GetWnd();
			enLink.msg = msg;
			enLink.chrg.cpMin = obj->charPos_;
			enLink.chrg.cpMax = obj->charPos_ + obj->charLength_ - 1;
			window_->SendMessage(OCM_NOTIFY, 0, (LPARAM)&enLink);
		}
		selectionRange_->ClearSelection();
		window_->ReDraw();
		return true;
	}
	return false;
}

void Document::MarkAsLink(int nBegin, int nEnd, CHARFORMAT& cf, MY_CHAR* hyperLinkData)
{
	int markLen = nEnd -nBegin + 1;
	charLengthList_.DeleteRange(nBegin, markLen);
	charLengthList_.InsertValue(nBegin, markLen, 0);
	styleList_.DeleteStyle(nBegin, nEnd);
	
	styleList_.InsertStyle(nBegin, markLen, cf, hyperLinkData);
	LineObjList_.Log();
	UpdateLineManager_insert(nBegin, 0);
	LineObjList_.Log();
	window_->ReDraw();
}

StyleObj* Document::GetStyleObjFromPoint(const POINT& pt)
{
	POINT ptFix;
	int charPos = FindCharPosFromPoint(pt, ptFix);
	return styleList_.GetStyleObjFromCharPos(charPos);
}

void Document::GetData(MY_CHAR* cSrc, int pos, int length)
{
	data.GetRange(cSrc, pos, length);
}

int Document::Scroll(int nYPos)
{
	details_->yViewPos_ += nYPos;
	ReDraw();
	return 0;
}

int Document::GetLineCount()
{
	return LineObjList_.GetLength();
}

int Document::GetDataCount()
{
	return data.Length();
}

int Document::GetStyleCount()
{
	return styleList_.GetLength();
}

const DocumentDetails* Document::GetDetails() const
{
	return details_;
}

void Document::SetDetails(DocumentDetails* details)
{
	details_ = details;
}

int Document::GetLogicHeight()
{
	int nLineLenght = LineObjList_.GetLength();
	if (nLineLenght > 0)
	{
		if ( LineObj* line = LineObjList_.GetLineObjFromIndex(nLineLenght - 1) )
			return (line->yPos_ + line->GetDetail()->maxHeight_);
	}
	return 0;
	
}

int Document::GetPosLength(int charPos, StyleObj* style)
{
	if ( data[charPos] == '\n' )
		return 0;
	int width = charLengthList_[charPos];
	if ( width > 0)
		return width;
	StyleObj* styleTemp = style;
	if(!styleTemp)
		styleTemp = styleList_.GetStyleObjFromCharPos(charPos);
	if (styleTemp->IsObj())
	{
		width = styleTemp->obj_->GetSize()->cx;
	}
	else
	{
		width = GetCharLenth(data[charPos], styleTemp->cf_, styleTemp->font_);
	}
	charLengthList_[charPos] = width;
	return width;	
}

int Document::GetCharLenth(MY_CHAR c,CHARFORMAT& cf, HFONT font)
{
	return window_->GetCharLenght(c, cf, font);
}

int Document::GetTextLenth(MY_CHAR* c, int length, CHARFORMAT& cf, HFONT font)
{
	return window_->GetTextLenght(c, length, cf, font);
}

int Document::GetTextLenth(int charpos, int length)
{
	int width = 0;
	for (int i = 0; i< length; i++)
	{
		width += charLengthList_[charpos + i];
	}
	return width;
}

int Document::CalculateSelectionLength(Selection& range)
{
	return GetSelectionLength(range.Begin_, range.End_, NULL);
}

int Document::CalculateSelectionLength(int begin, int end)
{
	return GetSelectionLength(begin, end, NULL);
}

int Document::GetSelectionLength(int begin, int& end, LineObj* objBegin, bool bCalcSplitter)
{
	if (begin == end)
	{
		return GetPosLength(begin);
	}
	else if ( begin > end)
	{
		return 0;
	}

	int realEnd = data.Length();
	if (end < realEnd)
		realEnd = end;

	if (!bCalcSplitter)
		end = realEnd;

	MY_CHAR* dataSelection = new MY_CHAR[realEnd - begin + 1];

	MY_CHAR* orginData = dataSelection;

	GetData(dataSelection, begin, realEnd - begin + 1);

	LineObj* beginObj = objBegin;
	if (!beginObj)
		beginObj = LineObjList_.GetLineFromCharPos(begin);

	int nLength = 0;
	int nbegin1 = begin;
	int nSplitter = begin;
	StyleObj* styleOld = styleList_.GetStyleObjFromCharPos(begin);
	StyleObj* styleNew = styleOld;
	int nDataLength = data.Length();
	while(begin <= realEnd && begin < nDataLength)
	{
		MY_CHAR c = data[begin];
		styleNew = styleList_.GetStyleObjFromCharPos(begin);
		if ( c == '\n')
		{
			nLength += GetTextLenth(nbegin1, begin - nbegin1);//GetTextLenth(dataSelection, begin - nbegin1, styleOld->cf_, styleOld->font_);// the '\n' no use
			dataSelection += (begin - nbegin1 + 1);
			bool bFirstOne = false;
			if (nbegin1 == begin)// if first is '\n', that's mean it must in this line
				bFirstOne = true;

			nbegin1 = begin + 1;
			styleNew = styleList_.GetStyleObjFromCharPos(nbegin1);
			
			if (!bCalcSplitter && !bFirstOne)
			{
				nbegin1 = realEnd+1;
				end = begin;
				break;
			}
		}
		else if ( !styleNew->IsEqual(styleOld))
		{
			if (styleOld->IsObj())
			{
				nLength += styleOld->obj_->GetSize()->cx;
				dataSelection += 1;
			}
			else
			{
				int eatLength = begin - nbegin1;
				if (begin == nbegin1)
					nLength += GetPosLength(begin);
				else
					nLength += GetTextLenth(nbegin1, eatLength);//GetTextLenth(dataSelection, eatLength, styleOld->cf_, styleOld->font_);
				dataSelection += eatLength;
			}
			nbegin1 = begin;
			
		}
		styleOld = styleNew;
		begin++;
	}

	if (nbegin1 <= realEnd)// no find that's mean all in same line
	{
		if (styleOld->IsObj())
			nLength += styleOld->obj_->GetSize()->cx;
		else
			nLength += GetTextLenth(nbegin1, realEnd - nbegin1 + 1);//GetTextLenth( dataSelection , realEnd - nbegin1 + 1, styleNew->cf_, styleNew->font_);
	}

	
	delete[] orginData;
	return nLength;
}

int Document::GetCharHeight(int pos)
{
	MY_CHAR c = data[pos];
	StyleObj* obj = styleList_.GetStyleObjFromCharPos(pos);
	if (c == '%')
	{
		if(obj->type_ == StyleObj::ST_OBJ)
		{
			assert(obj->obj_);
			return obj->obj_->GetSize()->cy;
		}
	}
	assert(obj->type_ != StyleObj::ST_OBJ);
	return obj->cf_.yHeight;
}

void Document::UpdateLinePos(int beginYViewPos, int from, int charLengthIncrease, int indexIncrease)
{
	LineObjList_.UpdateLinePos(beginYViewPos, from, charLengthIncrease, indexIncrease);
}

int Document::FixTheCharEnd(int charTotal, int pos, int maybeNeed, int& realNeed, LineObj* objBegin, bool bCalcSplitter)
{
	LineObj* obj = objBegin;
	if (!obj)
		obj = LineObjList_.GetLineFromCharPos(pos);

	int lastCharPos = pos + maybeNeed ;
	int widthCurLine = GetSelectionLength(objBegin ? objBegin->beginPos_ : pos, lastCharPos, NULL,  bCalcSplitter);

	int totalWidth = details_->rectDoc_.Width();
	if (widthCurLine > totalWidth)
	{		
		while(lastCharPos > pos)
		{
			int lengthChar = GetPosLength(lastCharPos);
			widthCurLine -= lengthChar;
			if ( widthCurLine <= totalWidth)
				break;
			lastCharPos--;
		}
	}
	else
	{
		realNeed = lastCharPos - pos;
		return widthCurLine;
	}

	realNeed = lastCharPos - pos;
	return widthCurLine;
}

int Document::GetPeLineCharCount(int begin, RowObjDetail& detail)
{
	int nTotalCount = data.Length();
	int beginTemp = begin;
	int lengthUse = 0;
	int lineCount = 0;
	int nTotalWidth = details_->rectDoc_.Width() - ident_;
	detail.ident_ = ident_;
	int nStyle = styleList_.FindStylePosFromCharPos(begin);
	StyleObj* style = styleList_.GetStyleObj(nStyle);
	while( beginTemp < nTotalCount )
	{
		if (beginTemp >= (style->charPos_ + style->charLength_))
		{
			nStyle++;
			style = styleList_.GetStyleObj(nStyle);
		}
		lengthUse = GetPosLength(beginTemp, style);
		if (lengthUse == 0 && data[beginTemp] == '\n' && beginTemp != begin)
		{
			break;
		}

		int height = 0;
		if (style->IsObj())
			height = style->obj_->GetSize()->cy;
		else
			height = style->cf_.yHeight;

		if (detail.maxHeight_ < height)
		{
			detail.maxHeight_ = height;
			detail.pos_ = beginTemp;
		}

		nTotalWidth -= lengthUse;
		if (nTotalWidth > 0 )
			lineCount++;
		else
			break;
		beginTemp++;
	}
	return lineCount;
}

int Document::GetBreakLineIndex(int from, int fixCharLength)
{
	LineObj* obj = LineObjList_.GetLineObjFromIndex(from);
	int totalLength = data.Length();
	while(obj)
	{
		if ( obj->beginPos_ + fixCharLength >= 0 
			&& obj->beginPos_ + fixCharLength < totalLength
			&& data[obj->beginPos_ + fixCharLength] == '\n')
			return from;
		from++;
		obj = LineObjList_.GetLineObjFromIndex(from);
	}

	return  -1;
}

void Document::UpdateLineManager_insert(int nPos, int insertLenght)
{
	LineObj* obj = LineObjList_.GetLineFromCharPos(nPos);
	if (!obj)
	{
		// this one test ok!
		int lineYPos = 0;
		int lineIndex = 0;
		int lineTotalLenght = LineObjList_.GetLength();
		int totalCount = data.Length() - nPos;
		int endPos = nPos;
		int curUpdateCount = 0;
		// update from the last line
		if (lineTotalLenght)
		{
			LineObj* objLast = LineObjList_.GetLineObjFromIndex(lineTotalLenght - 1);
			lineYPos = objLast->yPos_ + objLast->GetDetail()->maxHeight_;
			lineIndex = objLast->lineIndex_ + 1;
			if (data[endPos] != '\n')
			{
				RowObjDetail rowDetail;
				ZeroMemory(&rowDetail, sizeof(rowDetail));
				int nLineCount =  GetPeLineCharCount(objLast->beginPos_, rowDetail);
				objLast->endPos_ = objLast->beginPos_ + nLineCount - 1;
				objLast->SetDetail(rowDetail);
				lineYPos = objLast->yPos_ + objLast->GetDetail()->maxHeight_;
				endPos = objLast->endPos_ + 1;
				curUpdateCount += (objLast->endPos_ - nPos + 1);
			}
		}

		//must in the end, it must be insert line
		RowObjDetail rowDetail;
		while (curUpdateCount < totalCount)
		{
			LineObj* objInsert = new LineObj();
			ZeroMemory(&rowDetail, sizeof(rowDetail));
			int nLineCount =  GetPeLineCharCount(endPos, rowDetail);
			curUpdateCount += nLineCount;
			objInsert->beginPos_ = endPos;
			objInsert->endPos_ = objInsert->beginPos_ + nLineCount - 1;
			objInsert->yPos_ = lineYPos;
			objInsert->SetDetail(rowDetail);
			objInsert->lineIndex_ = lineIndex;
			LineObjList_.InsertLine(lineIndex, objInsert);
			lineYPos += objInsert->GetDetail()->maxHeight_;
			endPos = objInsert->endPos_ + 1;
			lineIndex++;
		} 
	}
	else
	{
		int lineYPos = obj->yPos_;
		int lineIndex = obj->lineIndex_;
		int endPos = nPos;
		int curUpdateCount = 0;
		int totalCount = 0;
		RowObjDetail rowDetail;
		ZeroMemory(&rowDetail, sizeof(rowDetail));
		int nLineCount =  GetPeLineCharCount(obj->beginPos_, rowDetail);
		obj->endPos_ = obj->beginPos_ + nLineCount - 1;
		obj->SetDetail(rowDetail);
		lineYPos += obj->GetDetail()->maxHeight_;
		lineIndex++;

		LineObjList_.Log();
		int lineBreak = GetBreakLineIndex(lineIndex, insertLenght);
 		if (lineBreak < 0)
		{
			totalCount = data.Length() - obj->beginPos_;
			LineObjList_.DeleteRight(lineIndex );
		}
		else
		{
			LineObj* objLast = LineObjList_.GetLineObjFromIndex(lineBreak);
			totalCount = objLast->beginPos_ - obj->beginPos_ + insertLenght;
			LineObjList_.DeleteLineRange(lineIndex, lineBreak - 1);
		}

		endPos = obj->endPos_ + 1;
		curUpdateCount += nLineCount;

		//LineObjList_.Log();
		int indexIncrease = 0;
		while (curUpdateCount < totalCount)
		{
			LineObj* objInsert = new LineObj();
			ZeroMemory(&rowDetail, sizeof(rowDetail));
			int nLineCount =  GetPeLineCharCount(endPos, rowDetail);
			curUpdateCount += nLineCount;
			objInsert->beginPos_ = endPos;
			objInsert->endPos_ = objInsert->beginPos_ + nLineCount - 1;
			objInsert->yPos_ = lineYPos;
			objInsert->SetDetail(rowDetail);
			objInsert->lineIndex_ = lineIndex;
			LineObjList_.InsertLine(lineIndex, objInsert);
			lineYPos += objInsert->GetDetail()->maxHeight_;
			endPos = objInsert->endPos_ + 1;
			lineIndex++;
			indexIncrease++;
			//LineObjList_.Log();
		}
		UpdateLinePos(lineYPos, lineIndex, insertLenght, indexIncrease);
		LineObjList_.Log();
	}
}

int Document::GetSelectionDetail(int nBegin, int nEnd, RowObjDetail& detail)
{
	detail.maxHeight_ = -1;
	detail.pos_ = nBegin;
	while(nBegin <= nEnd)
	{
		MY_CHAR c = data[nBegin];
		StyleObj* obj = styleList_.GetStyleObjFromCharPos(nBegin);
		nBegin++;
		if (c == '%' && obj->IsObj())
		{
			if ( detail.maxHeight_ < obj->obj_->GetSize()->cy )
			{
				detail.maxHeight_ = obj->obj_->GetSize()->cy;
				detail.pos_ = nBegin--;
			}
			continue;
		}

		if ( detail.maxHeight_  < obj->cf_.yHeight )
		{
			detail.maxHeight_ = obj->cf_.yHeight;
			detail.pos_ = nBegin--;
		}
	}
	return 0;
}

void Document::UpdateStyleManager_delete(int begin, int end)
{
	styleList_.DeleteStyle(begin, end);
}

void Document::UpdateLineManager_delete(int begin, int end)
{
	// for 3 step
	// step 1: delete all the line full
	LineObj* obj = LineObjList_.GetLineFromCharPos(begin);

	int beginLine = obj->lineIndex_;
	int endLine = LineObjList_.FindLineIndexFromCharPos(end);
	int decrease = 0;
	int decreaseCharLength = 0;

	if ((beginLine + 1) < endLine)
	{
		LineObjList_.DeleteLineRange(beginLine + 1, endLine - 1);
		decrease = endLine - beginLine -1;
	}

	// step 2: update line
	int lineYPos = obj->yPos_;
	int lineIndex = obj->lineIndex_;
	int curUpdateCount = 0;
	int totalCount = 0;
	RowObjDetail rowDetail;
	ZeroMemory(&rowDetail, sizeof(rowDetail));
	int nLineCount =  GetPeLineCharCount(obj->beginPos_, rowDetail);
	obj->endPos_ = obj->beginPos_ + nLineCount - 1;
	obj->SetDetail(rowDetail);
	lineYPos += obj->GetDetail()->maxHeight_;
	decreaseCharLength = end - begin + 1;
	lineIndex++;

	int virtualDecrease = 0;
	int lineBreak = GetBreakLineIndex(lineIndex, -decreaseCharLength);
	if (lineBreak < 0)
	{
		totalCount = data.Length() - obj->beginPos_;
		LineObjList_.DeleteRight(lineIndex );
	}
	else
	{
		LineObj* objLast = LineObjList_.GetLineObjFromIndex(lineBreak);
		totalCount = objLast->beginPos_ - obj->beginPos_ - decreaseCharLength;
		LineObjList_.Log();
		LineObjList_.DeleteLineRange(lineIndex, lineBreak - 1);
		LineObjList_.Log();
		virtualDecrease = lineBreak - lineIndex;
	}

	int endPos = obj->endPos_ + 1;
	curUpdateCount += nLineCount;
	
	while (curUpdateCount < totalCount)
	{
		LineObj* objInsert = new LineObj();
		ZeroMemory(&rowDetail, sizeof(rowDetail));
		int nLineCount =  GetPeLineCharCount(endPos, rowDetail);
		curUpdateCount += nLineCount;
		objInsert->beginPos_ = endPos;
		objInsert->endPos_ = objInsert->beginPos_ + nLineCount - 1;
		objInsert->yPos_ = lineYPos;
		objInsert->SetDetail(rowDetail);
		objInsert->lineIndex_ = lineIndex;
		LineObjList_.InsertLine(lineIndex, objInsert);
		lineYPos += objInsert->GetDetail()->maxHeight_;
		endPos = objInsert->endPos_ + 1;
		lineIndex++;
		virtualDecrease--;
	}

	if (virtualDecrease > 0)
		decrease += virtualDecrease;

	LineObjList_.Log();
	//update the leave line info
	UpdateLinePos(lineYPos, lineIndex, -decreaseCharLength, -decrease);
	LineObjList_.Log();

}

void Document::ReDraw()
{
	window_->ReDraw();
}