/**
*@file line.cpp
*@brief 行对象管理类
*
*包含行对象定义和相关管理逻辑
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include "line.h"

/************************************************************************/
/* LineObjManager	                                                  */
/************************************************************************/
LineObjManager::LineObjManager()
{
}

void LineObjManager::InsertLine(int index, LineObj* obj)
{
	LineObjList_.Insert(index, obj);
}

void LineObjManager::DelLine(int pos)
{
	LineObjList_.Delete(pos);
}

void LineObjManager::DeleteLineRange(int begin, int end)
{
	LineObjList_.DeleteRange(begin, end - begin + 1);
}

void LineObjManager::DeleteRight(int begin)
{
	int totallength =LineObjList_.Length();
	if (begin < totallength)
	{
		LineObjList_.DeleteRange(begin, totallength - begin);
	}
}

void LineObjManager::UpdateLinePos(int beginYViewPos, int from, int charLengthIncrease, int indexIncrease)
{
	LineObj* obj = GetLineObjFromIndex(from);
	while(obj)
	{
		
		if (obj->beginPos_ < 0)
			break;
		obj->beginPos_ += charLengthIncrease;
		obj->endPos_ += charLengthIncrease;
		obj->yPos_ = beginYViewPos;
		obj->lineIndex_ = from;
		beginYViewPos = obj->yPos_ + obj->GetDetail()->maxHeight_;
		from++;
		obj = GetLineObjFromIndex(from);
	}

	// clear all leave line
	DeleteRight(from);
}

LineObj* LineObjManager::GetLineObjFromIndex(int nIndex)
{
	int totallength =LineObjList_.Length();
	if (nIndex < totallength && nIndex >= 0)
		return LineObjList_[nIndex];
	return NULL;
}

LineObj* LineObjManager::GetLineFromCharPos(int charPos)
{
	int nLow = 0;
	int nHight = LineObjList_.Length();
	int nMiddle = (nLow + nHight) / 2;
	while(nLow < nHight)
	{
		LineObj* objMiddle = LineObjList_[nMiddle];
		int nTypeMiddle = objMiddle->GetCharPosType(charPos);

		switch (nTypeMiddle)
		{
		case LineObj::in:
			return objMiddle;
			break;
		case  LineObj::low:
			{
				if (nHight == nMiddle)
					nHight--;
				else
					nHight = nMiddle;
			}
			break;
		case  LineObj::beyond:
			{
				if (nLow == nMiddle)
					nLow++;
				else
					nLow = nMiddle;
			}			
			break;
		default:
			assert(false);
			break;
		}
		nMiddle = (nLow + nHight) / 2;
	}

	return NULL;
}

LineObj* LineObjManager::GetLineFromYPos(int yPos)
{
	int nLow = 0;
	int nHight = LineObjList_.Length();
	int nMiddle = (nLow + nHight) / 2;
	while(nLow < nHight)
	{
		LineObj* objMiddle = LineObjList_[nMiddle];
		int nTypeMiddle = objMiddle->GetYPosType(yPos);

		switch (nTypeMiddle)
		{
		case LineObj::in:
			return objMiddle;
			break;
		case  LineObj::low:
			{
				if (nHight == nMiddle)
					nHight--;
				else
					nHight = nMiddle;
			}
			break;
		case  LineObj::beyond:
			{
				if(nMiddle == nLow)
					nLow++;
				else
					nLow = nMiddle;
			}			
			break;
		default:
			assert(false);
			break;
		}
		nMiddle = (nLow + nHight) / 2;
	}

	return NULL;
}

int LineObjManager::FindLineIndexFromCharPos(int charPos)
{
	if ( LineObj* obj = GetLineFromCharPos(charPos) )
		return obj->lineIndex_;
	else
		return -1;

}

int LineObjManager::FindLineIndexFromYPos(int yPos)
{
	if ( LineObj* obj = GetLineFromYPos(yPos) )
		return obj->lineIndex_;
	else
		return -1;
}

int LineObjManager::GetLength()
{
	return LineObjList_.Length();
}

void LineObjManager::Log()
{
#ifndef USE_LOG
	return ;
#else
	int totalLength = LineObjList_.Length();
	ATLTRACE("\nBegin LineObjManager Log:\n");
	for ( int i = 0; i < totalLength; i++)
	{
		LineObj* obj = LineObjList_[i];
		ATLTRACE("%d : begin: %d, end:%d, yViewPos:%d, lineIndex:%d, maxHeight:%d, maxHeightPos:%d \n"
			, i, obj->beginPos_, obj->endPos_, obj->yPos_, obj->lineIndex_
			, obj->GetDetail()->maxHeight_, obj->GetDetail()->pos_);
	}
	ATLTRACE("\nEnd LineObjManager Log!\n");
#endif
}