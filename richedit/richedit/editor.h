/**
*@file editor.h
*@brief editor��
*
*������һ��editor��ϵͳ���������߼����ṩ�ӿڵ���
*
*@author ���µ�(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "SplitVector.h"
#include "editorInterface.h"
#include "define.h"
#include "document.h"
#include "CaretPosition.h"

///@brief �༭����
///
///�����༭����ز�����Ӧ�ͻ滭
class RichEditor
	: public IRicheditor
{
public:
	RichEditor();
	/************************************************************************/
	/* behaviour                                                            */
	/************************************************************************/
	///@brief ��
	///
	///�󶨴���
	///@param[in],hWnd:���ھ��
	///@pre NULL
	///@return void
	virtual void Attach(HWND hWnd);
	///@brief ጷ�
	///
	///ጷŌ���
	///@pre NULL
	///@return void
	void Release();
	///@brief �����ַ���
	///
	///�����ַ���
	///@param[in],c:Դ�ַ���
	///@param[in],length:�ַ�������
	///@param[in],pos:����λ��
	///@pre NULL
	///@return void
	virtual void InsertString(const MY_CHAR* c, int pos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData=NULL);
	///@brief �������
	///
	///�������
	///@param[in],obj:�����ַ
	///@param[in],pos:����λ��
	///@pre NULL
	///@return void
	virtual void InsertObj(IObj* obj, int pos);
	///@brief ��������
	///
	///��������
	///@param[in],c:Դ�ַ���
	///@param[in],length:�ַ�������
	///@param[in],pos:����λ��
	///@param[in],hyperLinkData:���Ӳ���
	///@pre NULL
	///@return void
	virtual void InsertLink(const MY_CHAR* c, int length, int pos, MY_CHAR* hyperLinkData);
	///@brief �����Ƿ�ֻ��
	///
	///�����Ƿ�ֻ��
	///@param[in],bReadOnly:�Ƿ�ֻ��
	///@pre NULL
	///@return void
	virtual void SetReadOnly(bool bReadOnly);
	///@brief ���ñ����ɫ
	///
	///���ñ����ɫ
	///@param[in],crbkgrd:�����ɫ
	///@pre NULL
	///@return void
	virtual void SetBkgrdColor(COLORREF crbkgrd);
	///@brief ����ѡ�б�����ɫ
	///
	///����ѡ�б�����ɫ
	///@param[in],crSelected:ѡ�б�����ɫ
	///@pre NULL
	///@return void
	virtual void SetSelectedColor(COLORREF crSelected);
	///@brief �������ײ�
	///
	///�������ײ�
	///@pre NULL
	///@return void
	virtual void ScrollToEnd();
	///@brief ���ö���������С
	///
	///���ö���������С
	///@param[in],ident:������С
	///@pre NULL
	///@return void
	virtual void SetParagraphIdent(int ident);
	///@brief ��ȡ�ĵ���������
	///
	///��ȡ�ĵ���������
	///@param[out],src:�ṩ�ڴ��
	///@pre NULL
	///@return void
	virtual void GetWindowText(MY_CHAR* src);
	///@brief ��ȡ�ĵ�ѡ������
	///
	///��ȡ�ĵ�ѡ������
	///@param[out],src:�ṩ�ڴ��
	///@pre NULL
	///@return void
	virtual void GetSelText(MY_CHAR* src);
	///@brief ��ȡ�ĵ�ָ������
	///
	///��ȡ�ĵ�ָ������
	///@param[out],src:�ṩ�ڴ��
	///@param[in],pos:��ȡ��ʼλ��
	///@param[in],length:����
	///@pre NULL
	///@return void
	virtual void GetText(MY_CHAR* src, int pos, int length);

	/************************************************************************/
	/*  property                                                            */
	/************************************************************************/
	///@brief �Ƿ�Ϊֻ��
	///
	///�Ƿ�Ϊֻ��
	///@pre NULL
	///@return bool
	virtual bool IsReadOnly() const;
	///@brief ��ȡ��������
	///
	///��ȡ��������
	///@param[out],rcClient:���������С
	///@pre NULL
	///@return void
	virtual void GetRect(RECT& rcClient);

	/************************************************************************/
	/*  paint                                                               */
	/************************************************************************/

	///@brief �滭
	///
	///�滭
	///@pre NULL
	///@return void
	virtual void Paint();
	///@brief �滭
	///
	///�滭
	///@pre NULL
	///@return void
	virtual void Paint(HDC dcPaint,RECT& rcClient, RECT& rcUpdate);

	/************************************************************************/
	/*  message                                                              */
	/************************************************************************/

	///@brief ��Ϣ���
	///
	///��Ϣ���
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