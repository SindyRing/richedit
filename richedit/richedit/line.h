/**
*@file line.h
*@brief 行对象管理类
*
*包含行对象定义和相关管理逻辑
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "editorInterface.h"
#include "SplitVector.h"

///@brief 行对象
///
///包含行对象的各种属性
struct LineObj : public IBaseObj
{

	enum PosType{
		low = 0,
		in,
		beyond
	};

	LineObj():beginPos_(-1), endPos_(-1), width_(0),lineIndex_(0)
	{
		nRef_ = 0;
		AddRef();
	}

	virtual int	AddRef()
	{
		nRef_++;
		return nRef_;
	}

	virtual int Release()
	{
		nRef_--;
		if (nRef_ == 0)
			delete this;
		return nRef_;
	}

	void ReSetBegin(int nBegin)
	{
		beginPos_ = nBegin;
	}

	void ReSetEnd(int nEnd)
	{
		endPos_ = nEnd;
	}

	void SetDetail(RowObjDetail& other)
	{
		Details = other;
	}

	const RowObjDetail* GetDetail() const 
	{
		return &Details;
	}

	int GetBottomYPos()
	{
		return (yPos_ + Details.maxHeight_);
	}

	void SetLineIndex(int nIndex)
	{
		lineIndex_ = nIndex;
	}

	int GetYPosType(int yPos)
	{
		if (yPos >= yPos_)
		{
			if (yPos < GetBottomYPos())
				return in;
			else
				return beyond;
		}
		else
			return low;
	}

	int GetCharPosType(int charPos)
	{
		if (charPos < beginPos_)
		{
			return low;
		}
		else if (charPos > endPos_)
		{
			return beyond;
		}
		else
			return in;
	}

	int GetWidth()
	{
		return width_;
	}

	int beginPos_;
	int endPos_;
	int width_;
	int lineIndex_;
	bool bNewLine_;

	RowObjDetail Details;
	int yPos_;

	int nRef_;
};

///@brief 行对象管理类
///
///对行对象进行管理
class LineObjManager
{
public:
	LineObjManager();
	///@brief 更新行对象属性
	///
	///初始化相关属性
	///@param[in],beginYViewPos:开始更新行的Y轴坐标
	///@param[in],from:开始更新的行索引
	///@param[in],charLengthIncrease:字符增长数
	///@param[in],indexIncrease:索引增长数
	///@pre NULL
	///@return void
	void UpdateLinePos(int beginYViewPos, int from, int charLengthIncrease, int indexIncrease);
	///@brief 插入行对象
	///
	///插入行对象
	///@param[in],pos:行索引
	///@param[in],obj:行对象
	///@pre NULL
	///@return void
	void InsertLine(int index, LineObj* obj);
	///@brief 删除行对象
	///
	///删除行对象
	///@param[in],pos:行索引
	///@pre NULL
	///@return void
	void DelLine(int pos);
	///@brief 删除某个范围行对象
	///
	///删除某个范围行对象
	///@param[in],begin:行索引
	///@param[in],end:行索引
	///@pre NULL
	///@return void
	void DeleteLineRange(int begin, int end);
	///@brief 删除某个范围行对象
	///
	///从某个索引开始删除掉后面所有行
	///@param[in],begin:行索引
	///@pre NULL
	///@return void
	void DeleteRight(int begin);
	///@brief 获取行
	///
	///根据索引获取行
	///@param[in],index:行索引
	///@pre NULL
	///@return LineObj
	LineObj* GetLineObjFromIndex(int nIndex);
	///@brief 获取行
	///
	///根据字符位置获取行
	///@param[in],charPos:字符位置
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromCharPos(int charPos);
	///@brief 获取行
	///
	///根据y轴坐标获取行
	///@param[in],yPos:y轴坐标
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromYPos(int yPos);
	///@brief 获取行索引
	///
	///获取行索引
	///@param[in],charPos:字符位置
	///@pre NULL
	///@return int
	int		FindLineIndexFromCharPos(int charPos);
	///@brief 获取行索引
	///
	///获取行索引
	///@param[in],yPos:y轴坐标位置
	///@pre NULL
	///@return int
	int		FindLineIndexFromYPos(int yPos);
	///@brief 获取行总数
	///
	///获取行总数
	///@pre NULL
	///@return int
	int GetLength();
	///@brief 日志函数
	///
	///日志函数
	///@pre NULL
	///@return void
	void Log();

protected:
	SplitVector<LineObj*> LineObjList_;
};
