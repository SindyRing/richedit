/**
*@file document.h
*@brief 文档类
*
*文档数据的相关操作
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "define.h"
#include "misc.h"
#include "line.h"
#include "window.h"
#include "style.h"

struct CaretDetail;

///@brief 文档画布规格结构体
///
///包含画布大小，可见坐标
///
typedef struct
{
	CRect rectangleDeflate_;
	CRect rectDoc_;
	int	yViewPos_;
	int xViewPos_;
}DocumentDetails;

///@brief 文档类
///
///包含style，line，data，charlenght四大数据结构体，所有的操作都是基于这
///四个数据结构的结合操作
///
class Document
{
public:
	Document();

	/************************************************************************/
	/*   operate                                                          */
	/************************************************************************/

	///@brief 插入字符串
	///
	///插入字符串
	///@param[in],c:字符串
	///@param[in],pos:插入位置
	///@param[in],length:插入长度
	///@param[in],cf:字符格式
	///@param[in],hyperLinkData:链接回调参数
	///@pre NULL
	///@return void
	void InsertString(const MY_CHAR* c, int pos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData = NULL);
	///@brief 插入对象
	///
	///插入对象
	///@param[in],obj:对象
	///@param[in],pos:插入位置
	///@pre NULL
	///@return void
	void InsertObj(IObj* obj, int pos);
	///@brief 删除
	///
	///删除一段内容
	///@param[in],begin:删除开始位置
	///@param[in],end:删除结束位置
	///@pre NULL
	///@return void
	void DeleteRange(int begin, int end);
	///@brief 删除
	///
	///删除一个字符
	///@param[in],pos:删除位置
	///@pre NULL
	///@return void
	void DeletePos(int pos);

	/************************************************************************/
	/*   line		                                                          */
	/************************************************************************/

	///@brief 获取行
	///
	///根据y轴坐标获取行
	///@param[in],yPos:y轴坐标
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromPos(int yPos);
	///@brief 获取行
	///
	///根据索引获取行
	///@param[in],index:行索引
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromIndex(int index);
	///@brief 获取可见行
	///
	///获取可见行
	///@pre NULL
	///@return LineObj
	LineObj* GetFirstVisbleLine();
	///@brief 获取行
	///
	///根据字符位置获取行
	///@param[in],charPos:字符位置
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromCharPos(int charPos);
	///@brief 获取字符位置
	///
	///根据坐标位置获取字符
	///@param[in],pt:坐标位置
	///@param[out],ptFix:定位到字符后修正的坐标位置
	///@param[out],caret:光标相关细节
	///@pre NULL
	///@return int
	int FindCharPosFromPoint(const POINT& pt, POINT& ptFix, CaretDetail* caret = NULL);
	///@brief 获取字符位置
	///
	///根据坐标位置获取字符
	///@param[in],pt:坐标位置
	///@param[out],ptFix:定位到字符后修正的坐标位置
	///@param[out],caret:光标相关细节
	///@pre NULL
	///@return int
	int GetCaretFromCharPos(int charPos, POINT& ptFix, CaretDetail* caret = NULL, bool bLeft = true);
	///@brief 设置段落
	///
	///设置段落空白大小
	///@param[in],ident:段落空白大小
	///@pre NULL
	///@return void
	void SetParagraphIdent(int ident);

	/************************************************************************/
	/* style                                                                */
	/************************************************************************/

	///@brief 获取样式对象
	///
	///根据字符位置获取样式
	///@param[in],charPos:字符位置
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObjFromCharPos(int charPos);
	///@brief 获取样式对象
	///
	///根据索引获取样式
	///@param[in],index:索引
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObj(int index);
	///@brief 获取样式索引
	///
	///根据字符位置获取样式
	///@param[in],charPos:字符位置
	///@pre NULL
	///@return int
	int		 FindStyleFromCharPos(int charPos);
	///@brief 是否命中Link样式对象
	///
	///是否命中Link样式对象
	///@param[in],pt:坐标
	///@pre NULL
	///@return bool
	bool	 IsHitTestLink(const POINT& pt);
	///@brief 获取样式对象
	///
	///根据坐标获取样式
	///@param[in],pt:坐标
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObjFromPoint(const POINT& pt);
	///@brief Link样式对象命中測試
	///
	///Link样式对象命中測試
	///@param[in],pt:坐标
	///@param[in],msg:消息
	///@pre NULL
	///@return bool
	bool	 LinkHitTest(const POINT& pt, UINT msg);
	///@brief 将选中部分修改为Link样式
	///
	///将选中部分修改为Link样式
	///@param[in],nBegin:字符开始位置
	///@param[in],nEnd:字符结束位置
	///@param[in],cf:样式
	///@param[in],hyperLinkData:超链接参数
	///@pre NULL
	///@return void
	void	MarkAsLink(int nBegin, int nEnd, CHARFORMAT& cf, MY_CHAR* hyperLinkData = NULL);

	/************************************************************************/
	/* data                                                                */
	/************************************************************************/

	///@brief 获取字符串数据
	///
	///获取字符串数据
	///@param[out],cSrc:字符串
	///@param[in],pos:获取字符开始位置
	///@param[in],length:获取长度
	///@pre NULL
	///@return void
	void GetData(MY_CHAR* cSrc, int pos, int length);
	///@brief 输出文字日志
	///
	///输出文字日志
	///@pre NULL
	///@return void
	void Log();
	///@brief 输出所有日志
	///
	///输出所有日志
	///@pre NULL
	///@return void
	void LogAll();

	///@brief 获取文档详细规格
	///
	///获取文档详细规格
	///@pre NULL
	///@return DocumentDetails
	const DocumentDetails* GetDetails() const;
	///@brief 设置文档详细规格
	///
	///设置文档详细规格
	///@param[in],details:文档详细规格
	///@pre NULL
	///@return void
	void SetDetails(DocumentDetails* details);
	///@brief 获取文档逻辑高度
	///
	///获取文档逻辑高度(px)
	///@pre NULL
	///@return int
	int GetLogicHeight();

	/************************************************************************/
	/* property                                                             */
	/************************************************************************/
	///@brief 绑定属性
	///
	///绑定属性
	///@param[in],wnd:窗口
	///@param[in],sel:选中对象
	///@pre NULL
	///@return int
	void Attach(Window* wnd, Selection* sel);
	///@brief 滚动Y轴
	///
	///滚动Y轴
	///@param[in],nYPos:y轴滚动幅度
	///@pre NULL
	///@return int
	int Scroll(int nYPos);
	///@brief 获取总行数
	///
	///获取总行数
	///@pre NULL
	///@return int
	int GetLineCount();
	///@brief 获取总字符数
	///
	///获取总字符数
	///@pre NULL
	///@return int
	int GetDataCount();
	///@brief 获取总样式数
	///
	///获取总样式数
	///@pre NULL
	///@return int
	int GetStyleCount();
	///@brief 计算选中区域长度
	///
	///计算选中区域长度
	///@param[in],range:区域
	///@pre NULL
	///@return int
	int CalculateSelectionLength(Selection& range);
	///@brief 计算选中区域长度
	///
	///计算选中区域长度
	///@param[in],begin:起点位置
	///@param[in],end:终点位置
	///@pre NULL
	///@return int
	int CalculateSelectionLength(int begin, int end);


protected:
	int GetCharLenth(MY_CHAR c,CHARFORMAT& cf, HFONT font);
	int GetPosLength(int charPos, StyleObj* style=NULL);
	int GetTextLenth(MY_CHAR* c, int length, CHARFORMAT& cf, HFONT font);
	int GetTextLenth(int charpos, int length);	
	int GetSelectionLength(int begin, int& end, LineObj* objBegin, bool bCalcSplitter = true);
	void UpdateLineManager(int insertWidth, int widthPer, int nPos, bool bInsert);
	void UpdateLineManager_insert(int nPos, int insertLenght);
	int GetBreakLineIndex(int from, int fixCharLength);
	int GetPeLineCharCount(int begin, RowObjDetail& detail);
	void UpdateLineManager_delete(int begin, int end);
	void UpdateStyleManager_delete(int begin, int end);
	void UpdateLinePos(int beginYViewPos,int from,int charLengthIncrease, int indexIncrease);
	int GetCharHeight(int pos);
	int GetSelectionDetail(int nBegin, int nEnd, RowObjDetail& detail);
	int FixTheCharEnd(int charTotal, int pos, int maybeNeed,  int& realNeed, LineObj* objBegin, bool bCalcSplitter = true);
	void ReDraw();
protected:
	
	SplitVector<MY_CHAR,false> data;///if is obj, use the '%' as symbol
	SplitVector<int,false> charLengthList_;/// for every char length.
	StyleObjManager styleList_;
	LineObjManager LineObjList_;
	DocumentDetails* details_;
	int				ident_;
	Window*			window_;
	Selection*		selectionRange_;
};