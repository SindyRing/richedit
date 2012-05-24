/**
*@file style.cpp
*@brief 样式对象管理类
*
*包含样式对象定义和相关管理逻辑
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include "style.h"
#include "line.h"
#include <assert.h>

StyleObjManager::StyleObjManager()
{
}

void StyleObjManager::InsertStyle(int charPos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData)
{
	int find = FindStylePosFromCharPos(charPos);
	StyleObj* objInsert = NULL;

	StyleObj* styleFind = GetStyleObj(find);
	StyleObj* style = new StyleObj(charPos, cf, NULL);
	if(cf.dwEffects & CFM_LINK)
		style->SetLink(new LinkObj(charPos, hyperLinkData));
	objInsert = style;
	if (styleFind && styleFind->IsSameStyle(style))
	{
		styleFind->AddSameStyle(length);
		objInsert = styleFind;
		delete style;
	}
	else
	{
		if (styleFind && charPos != 0 && charPos > styleFind->charPos_)
			find++;
		style->charLength_ = length;
		InsertStyle(find, style);
		Log();
		find++;
		if ( styleFind 
			&& charPos <= (styleFind->charPos_ + styleFind->charLength_)  
			&& charPos >= styleFind->charPos_)
		{
			
			if (charPos == styleFind->charPos_)
			{
				styleFind->charPos_ = charPos + style->charLength_;
			}
			else if (charPos < styleFind->charPos_ + styleFind->charLength_)
			{
				int oldlenght = styleFind->charLength_;
				styleFind->charLength_ = style->charPos_ - styleFind->charPos_;
				StyleObj* styleSplit = new StyleObj(style->charPos_ + length, styleFind->cf_, NULL);
				if(styleFind->cf_.dwEffects & CFM_LINK)
					styleSplit->SetLink(styleFind->AddRefLink());
				styleSplit->charLength_ = oldlenght - styleFind->charLength_;
				Log();
				InsertStyle(find, styleSplit);
				Log();
			}
		}
	}
	UpdateStylePosFromIndex(find+1, length, NULL);
}

void StyleObjManager::InsertStyle(int charPos, IObj* obj)
{
	int find = FindStylePosFromCharPos(charPos);
	StyleObj* styleFind = GetStyleObj(find);
	CHARFORMAT cf;
	GetDefaultCharformat(cf);
	StyleObj* style = new StyleObj(charPos, cf, obj);
	if (styleFind && styleFind->IsSameStyle(style))
	{
		styleFind->AddSameStyle(1);
		delete style;
	}
	else
	{
 		if (styleFind && charPos != 0 && charPos > styleFind->charPos_)
 			find++;
		int length = 1;
		style->charLength_ = length;
		InsertStyle(find, style);
		find++;
		if ( styleFind 
			&& charPos <= (styleFind->charPos_ + styleFind->charLength_)  
			&& charPos >= styleFind->charPos_)
		{
			if (charPos == styleFind->charPos_)
			{
				styleFind->charPos_ = charPos + style->charLength_;
			}
			else if (charPos < styleFind->charPos_ + styleFind->charLength_)
			{
				int oldlenght = styleFind->charLength_;
				styleFind->charLength_ = style->charPos_ - styleFind->charPos_;
				StyleObj* styleSplit = new StyleObj(style->charPos_ + length, styleFind->cf_, NULL);
				styleSplit->charLength_ = oldlenght - styleFind->charLength_;
				InsertStyle(find, styleSplit);
			}
		}
	}
	UpdateStylePosFromIndex(find + 1, 1, NULL);
}

void StyleObjManager::InsertStyle( int index, StyleObj* obj)
{
	objList_.Insert(index, obj);
}

void StyleObjManager::UpdateStylePosFromIndex(int begin, int charLength, StyleObj* objBegin)
{
	StyleObj* obj = objBegin;
	if (!obj)
		obj = GetStyleObj(begin);
	while(obj)
	{
		if (obj->charPos_ <= 0)
			break;
		begin++;
		obj->charPos_ += charLength;
		obj = GetStyleObj(begin);
	}

}

void StyleObjManager::DeleteStyle(int charPos)
{
	int find = FindStylePosFromCharPos(charPos);
	
	StyleObj* objFind = GetStyleObj(find);
	StyleObj* objNext = GetStyleObj(find+1);

	if (objNext)
	{
		if ( charPos == objFind->charPos_ &&  (objNext->charPos_ == charPos + 1) )
			objList_.Delete(find);
		else
			objFind->charPos_ -= 1;
	}
	else
	{
		if (charPos == objFind->charPos_)
			objList_.Delete(find);
		else
			objFind->charPos_ -= 1;
	}
	UpdateStylePosFromIndex(find, -1, objNext);

}

void StyleObjManager::DeleteStyle(int charBegin, int charEnd)
{
	int nBegin = charBegin;
	int nEnd = charEnd;

	int beginIndex = FindStylePosFromCharPos(charBegin);
	int endIndex = FindStylePosFromCharPos(charEnd);


	// 3 step: we must overwrite it.
	//step 1:
	// delete all the range style.
	Log();

	if ((beginIndex + 1) < endIndex)
	{
		int length = endIndex- beginIndex - 1;
		objList_.DeleteRange(beginIndex + 1, length);
		endIndex -= length;
	}

	Log();

	//step 2:
	//update two style that not delete but char length change.
	int nBeginUpdateIndex = endIndex + 1;
	bool bNeedDeleteBegin = false;
	bool bNeedDeleteEnd = false;
	StyleObj* objBegin = GetStyleObj(beginIndex);
	if ( objBegin->ReMoveRange(charBegin, charEnd) == StyleObj::SRT_ALLDELETE)
	{
		//objList_.DeleteRange(beginIndex, 1);
		bNeedDeleteBegin = true;
		nBeginUpdateIndex = beginIndex;
	}

	Log();

	if (beginIndex != endIndex)
	{
		StyleObj* objEnd = GetStyleObj(endIndex);
		if ( objEnd->ReMoveRange(charBegin, charEnd) == StyleObj::SRT_ALLDELETE)
		{
			bNeedDeleteEnd = true;
			nBeginUpdateIndex = endIndex;
			int nEndNext = endIndex + 1;
			StyleObj* objEndNext = GetStyleObj(nEndNext);// the two style is same, only need one
			if (objEndNext && objBegin->IsFormatEqual(objEnd))
			{
				objBegin->AddSameStyle(objEndNext->charLength_);
				objList_.Delete(nEndNext);
			}
		}
		else
		{			
			if (objBegin && objBegin->IsFormatEqual(objEnd))// the two style is same,only need one
			{
				objBegin->AddSameStyle(objEnd->charLength_);
				bNeedDeleteEnd = true;
				nBeginUpdateIndex = endIndex;
			}
			else
				nBeginUpdateIndex = endIndex + 1;
		}	
	}

	Log();

	if (bNeedDeleteEnd)
		objList_.Delete(endIndex);

	if (bNeedDeleteBegin)
		objList_.Delete(beginIndex);

	Log();
	//step 3:
	//update leave style char begin.

	StyleObj* objUpdate = GetStyleObj(nBeginUpdateIndex);
	while(objUpdate)
	{
		nBeginUpdateIndex++;
		objUpdate->charPos_ -= (charEnd - charBegin + 1);
		objUpdate = GetStyleObj(nBeginUpdateIndex);
	}
	Log();
}

static StyleObj* g_styleObj = NULL;

StyleObj* StyleObjManager::GetStyleObj(int index)
{
	if ( index >= 0 && index < objList_.Length() )
	{
		return objList_[index];
	}
	return NULL;	
}

StyleObj* StyleObjManager::GetStyleObjFromCharPos(int charPos)
{
	if(bBatch_)
	{
		int find = FindStylePosFromCharPos(charPos);
		g_styleObj = GetStyleObj(find);
		return g_styleObj;
	}
	else
	{
		int find = FindStylePosFromCharPos(charPos);
		StyleObj* style = GetStyleObj(find);
		return style;
	}
}

void StyleObjManager::SetBatch(bool bBatch)
{
	bBatch_ = bBatch;
	if(!bBatch_)
		g_styleObj = NULL;
}

int StyleObjManager::FindStylePosFromCharPos(int charPos)
{
	// this method does not correct exactly
	int nLow = 0;
	int nHight = objList_.Length() - 1;//has a end obj
	int nMiddle = (nLow + nHight) / 2;
	int find = -1;
	while(nLow < nHight)
	{
		StyleObj* objMiddle = objList_[nMiddle];
		int nTypeMiddle = objMiddle->GetPosType(charPos);

		switch (nTypeMiddle)
		{
		case LineObj::in:
			find = nMiddle;
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
		if (find >= 0)
			break;
	}

	if (find < 0)
		find = nLow;

	return find;
}

int StyleObjManager::GetLength()
{
	return objList_.Length();
}

void StyleObjManager::Log()
{
#ifndef USE_LOG
	return ;
#else
	int totalLength = objList_.Length();
	ATLTRACE("\nBegin StyleObjManager Log:\n");
	for ( int i = 0; i < totalLength; i++)
	{
		StyleObj* obj = objList_[i];
		ATLTRACE("%d : charPos: %d, charLength:%d, style:%d, endPos: %d \n"
			, i, obj->charPos_, obj->charLength_, obj->type_, obj->charPos_ + obj->charLength_ - 1);
	}
	ATLTRACE("\nEnd StyleObjManager Log!\n");
#endif
}