/**
*@file CaretPosition.h
*@brief ���λ����ز���
*
*�������λ�ö�λ���ƶ���ɾ��
*
*@author ���µ�(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/

#include "editorInterface.h"

class Window;
class Document;

///@brief ���������
///
///��������������
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

	///�ж��Ƿ����
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
	///��־����
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

///@brief ���λ����
///
///ά�����������ԺͲ���
class CaretPosition
{
public:
	///����ƶ��������������ĸ�����
	///
	enum CaretMoveDirection{
		CMD_LEFT = 0,
		CMD_TOP,
		CMD_RIGHT,
		CMD_BOTTOM
	};

	CaretPosition();

	///@brief ��ʼ���������
	///
	///��ʼ���������
	///@param[in],window:�滭����
	///@param[in],document:�ĵ�
	///@param[in],selectRange:ѡ������
	///@pre NULL
	///@return void
	void Attach(Window* window, Document* document, Selection* selectRange);
	///@brief ���ù��λ��
	///
	///���ù��λ��
	///@param[in],charPos,���λ��
	///@param[in],bLeft,�Ƿ�Ϊ��λ�����
	///@pre NULL
	///@return void
	void SetCaretPos(int charPos, bool bLeft = true);
	///@brief ���¹��λ��
	///
	///���¹��λ��
	///@param[in],charPos,���λ��
	///@param[in],bLeft,�Ƿ�Ϊ��λ�����
	///@pre NULL
	///@return void
	void UpdateCaretPos(const POINT& pt);
	///@brief ��ȡ���λ��
	///
	///��ȡ��������ַ�����λ��
	///@pre NULL
	///@return int
	int GetCaretCharPos();
	///@brief ��ȡ�ɲ���λ��
	///
	///���ݵ�ǰ���λ�û�ȡ�ɲ���λ��
	///@pre NULL
	///@return int
	int GetTextInsertPos();
	///@brief ��ʾ���
	///
	///���������Ϊ�ɼ�״̬
	///@pre NULL
	///@return void
	void ShowCaret();
	///@brief ���ع��
	///
	///���������Ϊ����״̬
	///@pre NULL
	///@return void
	void HideCaret();
	///@brief ���¹��״̬
	///
	///���ݵ�ǰ���λ�ø��¹��״̬
	///@pre NULL
	///@return void
	void UpdateCaretStyle();
	///@brief �ƶ����
	///
	///�ƶ����λ��
	///@param[in],x,x��ƫ��λ��(px)
	///@param[in],y,y��ƫ��λ��(px)
	///@pre NULL
	///@return void
	void MoveCaretPos(int x, int y);
	///@brief ��ȡ���״̬
	///
	///��ȡ���״̬
	///@pre NULL
	///@return bool
	bool IsLeft();
	///@brief ���²������λ��
	///
	///���²������λ��
	///@param[in],length,�����ַ���
	///@pre NULL
	///@return void
	void UpdateInsert(int length);

	/************************************************************************/
	/* operator                                                             */
	/************************************************************************/

	///@brief ������ɾ��
	///
	///������ɾ��
	///@param[in],bBack,trueΪback����falseΪDelete��
	///@pre NULL
	///@return void
	void Delete(bool bBack);
	///@brief ���¹��λ��
	///
	///�����߼�������¹��λ�ò�ͬʱ����ѡ��״̬
	///@param[in],pt,����
	///@pre NULL
	///@return void
	void UpdatePosFromPt(const POINT& pt);
	///@brief �ƶ����
	///
	///�ƶ����λ��
	///@param[in],nType,�ƶ�������������
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