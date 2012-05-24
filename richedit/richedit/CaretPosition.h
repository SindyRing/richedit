/**
*@file CaretPosition.h
*@brief 光标位置相关操作
*
*包括光标位置定位，移动，删除
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/

#include "editorInterface.h"

class Window;
class Document;

///@brief 光标属性类
///
///包含光标各种属性
///
struct CaretDetail
{
	CaretDetail()
	{
		ZeroMemory(&pt, sizeof(pt));
		charPos_ = -1;
		line_ = -1;
		isLeft = true;
	}

	CaretDetail& operator = (const CaretDetail& other)
	{
		charPos_ = other.charPos_;
		line_ = other.line_;
		pt = other.pt;
		isLeft = other.isLeft;
		return *this;
	}

	///判断是否相等
	///
	bool IsEqual(const CaretDetail& other)
	{
		if (charPos_ == other.charPos_ 
			&& line_ == other.line_
			&& isLeft == other.isLeft)
		{
			return true;
		}

		return false;
	}
	///日志函数
	///
	void Log(MY_CHAR* name)
	{
#ifndef USE_LOG
		return ;
#else
		ATLTRACE("\n%s: charPos_:%d ; line_:%d; pt:[x:%d, y:%d] isLeft:%d \n"
			, name
			, charPos_
			, line_
			, pt.x
			, pt.y
			, isLeft ? 1 : 0);
#endif
	}

	int charPos_;
	int line_;
	POINT pt;
	bool isLeft;
};

///@brief 光标位置类
///
///维护光标各种属性和操作
class CaretPosition
{
public:
	///光标移动方向，左上右下四个方向
	///
	enum CaretMoveDirection{
		CMD_LEFT = 0,
		CMD_TOP,
		CMD_RIGHT,
		CMD_BOTTOM
	};

	CaretPosition();

	///@brief 初始化相关属性
	///
	///初始化相关属性
	///@param[in],window:绘画窗口
	///@param[in],document:文档
	///@param[in],selectRange:选中区域
	///@pre NULL
	///@return void
	void Attach(Window* window, Document* document, Selection* selectRange);
	///@brief 设置光标位置
	///
	///设置光标位置
	///@param[in],charPos,光标位置
	///@param[in],bLeft,是否为该位置左边
	///@pre NULL
	///@return void
	void SetCaretPos(int charPos, bool bLeft = true);
	///@brief 更新光标位置
	///
	///更新光标位置
	///@param[in],charPos,光标位置
	///@param[in],bLeft,是否为该位置左边
	///@pre NULL
	///@return void
	void UpdateCaretPos(const POINT& pt);
	///@brief 获取光标位置
	///
	///获取光标光标在字符串中位置
	///@pre NULL
	///@return int
	int GetCaretCharPos();
	///@brief 获取可插入位置
	///
	///根据当前光标位置获取可插入位置
	///@pre NULL
	///@return int
	int GetTextInsertPos();
	///@brief 显示光标
	///
	///将光标设置为可见状态
	///@pre NULL
	///@return void
	void ShowCaret();
	///@brief 隐藏光标
	///
	///将光标设置为隐藏状态
	///@pre NULL
	///@return void
	void HideCaret();
	///@brief 更新光标状态
	///
	///根据当前光标位置更新光标状态
	///@pre NULL
	///@return void
	void UpdateCaretStyle();
	///@brief 移动光标
	///
	///移动光标位置
	///@param[in],x,x轴偏移位置(px)
	///@param[in],y,y轴偏移位置(px)
	///@pre NULL
	///@return void
	void MoveCaretPos(int x, int y);
	///@brief 获取光标状态
	///
	///获取光标状态
	///@pre NULL
	///@return bool
	bool IsLeft();
	///@brief 更新插入后光标位置
	///
	///更新插入后光标位置
	///@param[in],length,插入字符数
	///@pre NULL
	///@return void
	void UpdateInsert(int length);

	/************************************************************************/
	/* operator                                                             */
	/************************************************************************/

	///@brief 光标回退删除
	///
	///光标回退删除
	///@param[in],bBack,true为back键，false为Delete键
	///@pre NULL
	///@return void
	void Delete(bool bBack);
	///@brief 更新光标位置
	///
	///根据逻辑坐标更新光标位置并同时更新选中状态
	///@param[in],pt,坐标
	///@pre NULL
	///@return void
	void UpdatePosFromPt(const POINT& pt);
	///@brief 移动光标
	///
	///移动光标位置
	///@param[in],nType,移动方向：左上右下
	///@pre NULL
	///@return void
	void MoveCaret(int nType);

protected:
	void MoveLeft();
	void MoveTop();
	void MoveRight();
	void MoveBottom();
	void GetCaretPointFromCharPos(char pos, int lineIndex, POINT& ptFind, bool bLeft = true);

protected:
	Window*	window_;
	Document* document_;
	Selection* selectRange_;
	CaretDetail caretCurShow_;
};