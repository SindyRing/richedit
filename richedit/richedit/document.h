/**
*@file document.h
*@brief �ĵ���
*
*�ĵ����ݵ���ز���
*
*@author ���µ�(Sindy)<goki123@126.com>
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

///@brief �ĵ��������ṹ��
///
///����������С���ɼ�����
///
typedef struct
{
	CRect rectangleDeflate_;
	CRect rectDoc_;
	int	yViewPos_;
	int xViewPos_;
}DocumentDetails;

///@brief �ĵ���
///
///����style��line��data��charlenght�Ĵ����ݽṹ�壬���еĲ������ǻ�����
///�ĸ����ݽṹ�Ľ�ϲ���
///
class Document
{
public:
	Document();

	/************************************************************************/
	/*   operate                                                          */
	/************************************************************************/

	///@brief �����ַ���
	///
	///�����ַ���
	///@param[in],c:�ַ���
	///@param[in],pos:����λ��
	///@param[in],length:���볤��
	///@param[in],cf:�ַ���ʽ
	///@param[in],hyperLinkData:���ӻص�����
	///@pre NULL
	///@return void
	void InsertString(const MY_CHAR* c, int pos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData = NULL);
	///@brief �������
	///
	///�������
	///@param[in],obj:����
	///@param[in],pos:����λ��
	///@pre NULL
	///@return void
	void InsertObj(IObj* obj, int pos);
	///@brief ɾ��
	///
	///ɾ��һ������
	///@param[in],begin:ɾ����ʼλ��
	///@param[in],end:ɾ������λ��
	///@pre NULL
	///@return void
	void DeleteRange(int begin, int end);
	///@brief ɾ��
	///
	///ɾ��һ���ַ�
	///@param[in],pos:ɾ��λ��
	///@pre NULL
	///@return void
	void DeletePos(int pos);

	/************************************************************************/
	/*   line		                                                          */
	/************************************************************************/

	///@brief ��ȡ��
	///
	///����y�������ȡ��
	///@param[in],yPos:y������
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromPos(int yPos);
	///@brief ��ȡ��
	///
	///����������ȡ��
	///@param[in],index:������
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromIndex(int index);
	///@brief ��ȡ�ɼ���
	///
	///��ȡ�ɼ���
	///@pre NULL
	///@return LineObj
	LineObj* GetFirstVisbleLine();
	///@brief ��ȡ��
	///
	///�����ַ�λ�û�ȡ��
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromCharPos(int charPos);
	///@brief ��ȡ�ַ�λ��
	///
	///��������λ�û�ȡ�ַ�
	///@param[in],pt:����λ��
	///@param[out],ptFix:��λ���ַ�������������λ��
	///@param[out],caret:������ϸ��
	///@pre NULL
	///@return int
	int FindCharPosFromPoint(const POINT& pt, POINT& ptFix, CaretDetail* caret = NULL);
	///@brief ��ȡ�ַ�λ��
	///
	///��������λ�û�ȡ�ַ�
	///@param[in],pt:����λ��
	///@param[out],ptFix:��λ���ַ�������������λ��
	///@param[out],caret:������ϸ��
	///@pre NULL
	///@return int
	int GetCaretFromCharPos(int charPos, POINT& ptFix, CaretDetail* caret = NULL, bool bLeft = true);
	///@brief ���ö���
	///
	///���ö���հ״�С
	///@param[in],ident:����հ״�С
	///@pre NULL
	///@return void
	void SetParagraphIdent(int ident);

	/************************************************************************/
	/* style                                                                */
	/************************************************************************/

	///@brief ��ȡ��ʽ����
	///
	///�����ַ�λ�û�ȡ��ʽ
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObjFromCharPos(int charPos);
	///@brief ��ȡ��ʽ����
	///
	///����������ȡ��ʽ
	///@param[in],index:����
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObj(int index);
	///@brief ��ȡ��ʽ����
	///
	///�����ַ�λ�û�ȡ��ʽ
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return int
	int		 FindStyleFromCharPos(int charPos);
	///@brief �Ƿ�����Link��ʽ����
	///
	///�Ƿ�����Link��ʽ����
	///@param[in],pt:����
	///@pre NULL
	///@return bool
	bool	 IsHitTestLink(const POINT& pt);
	///@brief ��ȡ��ʽ����
	///
	///���������ȡ��ʽ
	///@param[in],pt:����
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObjFromPoint(const POINT& pt);
	///@brief Link��ʽ�������Мyԇ
	///
	///Link��ʽ�������Мyԇ
	///@param[in],pt:����
	///@param[in],msg:��Ϣ
	///@pre NULL
	///@return bool
	bool	 LinkHitTest(const POINT& pt, UINT msg);
	///@brief ��ѡ�в����޸�ΪLink��ʽ
	///
	///��ѡ�в����޸�ΪLink��ʽ
	///@param[in],nBegin:�ַ���ʼλ��
	///@param[in],nEnd:�ַ�����λ��
	///@param[in],cf:��ʽ
	///@param[in],hyperLinkData:�����Ӳ���
	///@pre NULL
	///@return void
	void	MarkAsLink(int nBegin, int nEnd, CHARFORMAT& cf, MY_CHAR* hyperLinkData = NULL);

	/************************************************************************/
	/* data                                                                */
	/************************************************************************/

	///@brief ��ȡ�ַ�������
	///
	///��ȡ�ַ�������
	///@param[out],cSrc:�ַ���
	///@param[in],pos:��ȡ�ַ���ʼλ��
	///@param[in],length:��ȡ����
	///@pre NULL
	///@return void
	void GetData(MY_CHAR* cSrc, int pos, int length);
	///@brief ���������־
	///
	///���������־
	///@pre NULL
	///@return void
	void Log();
	///@brief ���������־
	///
	///���������־
	///@pre NULL
	///@return void
	void LogAll();

	///@brief ��ȡ�ĵ���ϸ���
	///
	///��ȡ�ĵ���ϸ���
	///@pre NULL
	///@return DocumentDetails
	const DocumentDetails* GetDetails() const;
	///@brief �����ĵ���ϸ���
	///
	///�����ĵ���ϸ���
	///@param[in],details:�ĵ���ϸ���
	///@pre NULL
	///@return void
	void SetDetails(DocumentDetails* details);
	///@brief ��ȡ�ĵ��߼��߶�
	///
	///��ȡ�ĵ��߼��߶�(px)
	///@pre NULL
	///@return int
	int GetLogicHeight();

	/************************************************************************/
	/* property                                                             */
	/************************************************************************/
	///@brief ������
	///
	///������
	///@param[in],wnd:����
	///@param[in],sel:ѡ�ж���
	///@pre NULL
	///@return int
	void Attach(Window* wnd, Selection* sel);
	///@brief ����Y��
	///
	///����Y��
	///@param[in],nYPos:y���������
	///@pre NULL
	///@return int
	int Scroll(int nYPos);
	///@brief ��ȡ������
	///
	///��ȡ������
	///@pre NULL
	///@return int
	int GetLineCount();
	///@brief ��ȡ���ַ���
	///
	///��ȡ���ַ���
	///@pre NULL
	///@return int
	int GetDataCount();
	///@brief ��ȡ����ʽ��
	///
	///��ȡ����ʽ��
	///@pre NULL
	///@return int
	int GetStyleCount();
	///@brief ����ѡ�����򳤶�
	///
	///����ѡ�����򳤶�
	///@param[in],range:����
	///@pre NULL
	///@return int
	int CalculateSelectionLength(Selection& range);
	///@brief ����ѡ�����򳤶�
	///
	///����ѡ�����򳤶�
	///@param[in],begin:���λ��
	///@param[in],end:�յ�λ��
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