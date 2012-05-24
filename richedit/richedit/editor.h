/**
*@file editor.h
*@brief editor类
*
*完整的一个editor与系统交互层面逻辑和提供接口调用
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "SplitVector.h"
#include "editorInterface.h"
#include "define.h"
#include "document.h"
#include "CaretPosition.h"

///@brief 编辑器类
///
///包含编辑器相关操作响应和绘画
class RichEditor
	: public IRicheditor
{
public:
	RichEditor();
	/************************************************************************/
	/* behaviour                                                            */
	/************************************************************************/
	///@brief 绑定
	///
	///绑定窗口
	///@param[in],hWnd:窗口句柄
	///@pre NULL
	///@return void
	virtual void Attach(HWND hWnd);
	///@brief 釋放
	///
	///釋放對象
	///@pre NULL
	///@return void
	void Release();
	///@brief 插入字符串
	///
	///插入字符串
	///@param[in],c:源字符串
	///@param[in],length:字符串长度
	///@param[in],pos:插入位置
	///@pre NULL
	///@return void
	virtual void InsertString(const MY_CHAR* c, int pos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData=NULL);
	///@brief 插入对象
	///
	///插入对象
	///@param[in],obj:对象地址
	///@param[in],pos:插入位置
	///@pre NULL
	///@return void
	virtual void InsertObj(IObj* obj, int pos);
	///@brief 插入链接
	///
	///插入链接
	///@param[in],c:源字符串
	///@param[in],length:字符串长度
	///@param[in],pos:插入位置
	///@param[in],hyperLinkData:链接参数
	///@pre NULL
	///@return void
	virtual void InsertLink(const MY_CHAR* c, int length, int pos, MY_CHAR* hyperLinkData);
	///@brief 设置是否只读
	///
	///设置是否只读
	///@param[in],bReadOnly:是否只读
	///@pre NULL
	///@return void
	virtual void SetReadOnly(bool bReadOnly);
	///@brief 设置背景顏色
	///
	///设置背景顏色
	///@param[in],crbkgrd:背景顏色
	///@pre NULL
	///@return void
	virtual void SetBkgrdColor(COLORREF crbkgrd);
	///@brief 设置选中背景颜色
	///
	///设置选中背景颜色
	///@param[in],crSelected:选中背景颜色
	///@pre NULL
	///@return void
	virtual void SetSelectedColor(COLORREF crSelected);
	///@brief 滑动到底部
	///
	///滑动到底部
	///@pre NULL
	///@return void
	virtual void ScrollToEnd();
	///@brief 设置段落缩进大小
	///
	///设置段落缩进大小
	///@param[in],ident:缩进大小
	///@pre NULL
	///@return void
	virtual void SetParagraphIdent(int ident);
	///@brief 获取文档所有内容
	///
	///获取文档所有内容
	///@param[out],src:提供内存块
	///@pre NULL
	///@return void
	virtual void GetWindowText(MY_CHAR* src);
	///@brief 获取文档选中内容
	///
	///获取文档选中内容
	///@param[out],src:提供内存块
	///@pre NULL
	///@return void
	virtual void GetSelText(MY_CHAR* src);
	///@brief 获取文档指定内容
	///
	///获取文档指定内容
	///@param[out],src:提供内存块
	///@param[in],pos:获取起始位置
	///@param[in],length:长度
	///@pre NULL
	///@return void
	virtual void GetText(MY_CHAR* src, int pos, int length);

	/************************************************************************/
	/*  property                                                            */
	/************************************************************************/
	///@brief 是否为只读
	///
	///是否为只读
	///@pre NULL
	///@return bool
	virtual bool IsReadOnly() const;
	///@brief 获取窗口区域
	///
	///获取窗口区域
	///@param[out],rcClient:窗口区域大小
	///@pre NULL
	///@return void
	virtual void GetRect(RECT& rcClient);

	/************************************************************************/
	/*  paint                                                               */
	/************************************************************************/

	///@brief 绘画
	///
	///绘画
	///@pre NULL
	///@return void
	virtual void Paint();
	///@brief 绘画
	///
	///绘画
	///@pre NULL
	///@return void
	virtual void Paint(HDC dcPaint,RECT& rcClient, RECT& rcUpdate);

	/************************************************************************/
	/*  message                                                              */
	/************************************************************************/

	///@brief 消息入口
	///
	///消息入口
	///@pre NULL
	///@return LRESULT
	virtual LRESULT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	LRESULT OnLBButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLBButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnVScroll(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnIMEConPositon(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChar(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

#ifdef USE_TEST
	/************************************************************************/
	/* for test                                                             */
	/************************************************************************/
	void Test();
#endif

protected:
	void CopyData();
	void PasteData();
	void Scroll(int x, int y, bool bForce=false);
	void OnScroll(int nBar, int nOperate);
	BOOL IsScrollVisible(int nBar);
	void UpdateScrollBar(int nBar);

protected:
	bool	bTraceMouse_;
	bool	bLBButtonDown_;
	bool	bReadOnly_;
	COLORREF crBkgrd_;
	Selection selectRange_;
	Document document_;
	DocumentDetails details_;
	Window	window_;
	CHARFORMAT cfDefault_;
	CaretPosition caretPosition_;
};