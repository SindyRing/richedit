/**
*@file drawer.h
*@brief 绘画逻辑类
*
*包含行绘画逻辑部分
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "line.h"
#include "document.h"
///@brief 逻辑计算类
///
///逻辑计算类
class Calculator
{
public:
	static void GetTextClientRect(RECT& rcOutput, const LineObj* line, const Document* document)
	{
		rcOutput.left = document->GetDetails()->rectDoc_.left + line->GetDetail()->ident_;
		rcOutput.top = line->yPos_ - document->GetDetails()->yViewPos_;
		rcOutput.right = document->GetDetails()->rectDoc_.right;
		rcOutput.bottom = rcOutput.top + line->GetDetail()->maxHeight_;
	}

};

///@brief 行自动绘画类
///
///行自动绘画类
class LineDrawer
{
public:
	LineDrawer(LineObj* obj, Document* document, HDC dcPaint)
	{
		obj_ = obj;
		document_ = document;
		int datalength = obj_->endPos_ - obj_->beginPos_ + 1;
		data = new MY_CHAR[datalength + 1];
		document_->GetData(data, obj_->beginPos_, datalength);
		dataTemp = data;
		dcPaint_ = dcPaint;
	}

	~LineDrawer()
	{
		delete[] data;
	}

	///@brief 绘画
	///
	///行绘画函数
	///@param[in],window:窗口
	///@param[in],curSelecton:当前选中区域
	///@pre NULL
	///@return void
	void Paint(Window* window, const Selection& curSelecton)
	{
		int styleBegin = document_->FindStyleFromCharPos(obj_->beginPos_);
		int styleEnd = document_->FindStyleFromCharPos(obj_->endPos_);
	
		RECT rcLine;
		Calculator::GetTextClientRect(rcLine, obj_, document_);
		posCurDraw_ = obj_->beginPos_;
		while(styleBegin >= 0 && styleBegin <= styleEnd)
		{
			StyleObj* style = document_->GetStyleObj(styleBegin);
		 	PaintInStyleRange(window, style, rcLine, curSelecton);
			styleBegin++;
		}
	}

protected:
	///@brief 样式绘画
	///
	///以样式为单位的绘画
	///@param[in],window:窗口
	///@param[in],style:当前样式
	///@param[in],rcLine:当前可画区域
	///@param[in],curSelecton:当前选中区域
	///@pre NULL
	///@return void
	void PaintInStyleRange(Window* window, StyleObj* style, RECT& rcLine, const Selection& curSelection)
	{
		CRect rcLineTemp = rcLine;
		if (style->IsObj())// obj
		{
			rcLineTemp.right = rcLineTemp.left + style->obj_->GetSize()->cx;
			window->PaintObj(style->obj_, rcLineTemp, dcPaint_, curSelection.IsBeSelect(style->charPos_));
			rcLine.left += style->obj_->GetSize()->cx ;
			posCurDraw_ += 1;
			dataTemp += 1;
		}
		else // text
		{
			int nBeginPos = MAX(style->charPos_, obj_->beginPos_);
			int nEndPos = MIN((style->charPos_ + style->charLength_ - 1), obj_->endPos_);
			if (curSelection.IsBeSelect(nBeginPos, nEndPos))
			{
				//for 3 step:
				// step 1: 
				int eatLength = 0;
				if (nBeginPos < curSelection.Begin_)
				{
					eatLength = curSelection.Begin_ - nBeginPos;
					dataTemp = data + nBeginPos - obj_->beginPos_;
					window->PaintText(dataTemp, eatLength, rcLineTemp, style->cf_, dcPaint_, style->font_);
					dataTemp += eatLength;
					ATLTRACE("\n never select 1: %s ,length:%d \n", dataTemp, eatLength);
					rcLine.left += rcLineTemp.Width();
					rcLineTemp = rcLine;
				}

				// step 2:
				int maxSelBegin = MAX(nBeginPos, curSelection.Begin_);
				int minSelEnd = MIN(nEndPos, curSelection.End_);
				if (maxSelBegin <= minSelEnd)
				{
					eatLength = minSelEnd - maxSelBegin + 1;
					Window::AutoSelection selectionControler(window);

					window->PaintText(dataTemp, eatLength, rcLineTemp, style->cf_, dcPaint_, style->font_);
					dataTemp += eatLength;
					ATLTRACE("\n select : %s ,length:%d \n", dataTemp, eatLength);
					rcLine.left  += rcLineTemp.Width();
					rcLineTemp = rcLine;
				}

				// step 3:
				if (minSelEnd < nEndPos)
				{
					eatLength = nEndPos - minSelEnd ;
					window->PaintText(dataTemp, eatLength, rcLineTemp, style->cf_, dcPaint_, style->font_);
					rcLine.left  += rcLineTemp.Width();
					dataTemp += eatLength;
					rcLineTemp = rcLine;
					ATLTRACE("\n never select 2: %s ,length:%d \n", dataTemp, eatLength);
				}

			}
			else
			{
				int eatLength = nEndPos - nBeginPos + 1;
				window->PaintText(dataTemp, eatLength, rcLineTemp, style->cf_, dcPaint_, style->font_);
				dataTemp += eatLength;
				rcLine.left += rcLineTemp.Width();
			}

		}
	}

public:
	MY_CHAR* data;
	MY_CHAR* dataTemp;
	LineObj* obj_;
	int posCurDraw_;
	Document* document_;
	HDC dcPaint_;
};