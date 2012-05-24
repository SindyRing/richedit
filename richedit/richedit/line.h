/**
*@file line.h
*@brief �ж��������
*
*�����ж��������ع����߼�
*
*@author ���µ�(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "editorInterface.h"
#include "SplitVector.h"

///@brief �ж���
///
///�����ж���ĸ�������
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

///@brief �ж��������
///
///���ж�����й���
class LineObjManager
{
public:
	LineObjManager();
	///@brief �����ж�������
	///
	///��ʼ���������
	///@param[in],beginYViewPos:��ʼ�����е�Y������
	///@param[in],from:��ʼ���µ�������
	///@param[in],charLengthIncrease:�ַ�������
	///@param[in],indexIncrease:����������
	///@pre NULL
	///@return void
	void UpdateLinePos(int beginYViewPos, int from, int charLengthIncrease, int indexIncrease);
	///@brief �����ж���
	///
	///�����ж���
	///@param[in],pos:������
	///@param[in],obj:�ж���
	///@pre NULL
	///@return void
	void InsertLine(int index, LineObj* obj);
	///@brief ɾ���ж���
	///
	///ɾ���ж���
	///@param[in],pos:������
	///@pre NULL
	///@return void
	void DelLine(int pos);
	///@brief ɾ��ĳ����Χ�ж���
	///
	///ɾ��ĳ����Χ�ж���
	///@param[in],begin:������
	///@param[in],end:������
	///@pre NULL
	///@return void
	void DeleteLineRange(int begin, int end);
	///@brief ɾ��ĳ����Χ�ж���
	///
	///��ĳ��������ʼɾ��������������
	///@param[in],begin:������
	///@pre NULL
	///@return void
	void DeleteRight(int begin);
	///@brief ��ȡ��
	///
	///����������ȡ��
	///@param[in],index:������
	///@pre NULL
	///@return LineObj
	LineObj* GetLineObjFromIndex(int nIndex);
	///@brief ��ȡ��
	///
	///�����ַ�λ�û�ȡ��
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromCharPos(int charPos);
	///@brief ��ȡ��
	///
	///����y�������ȡ��
	///@param[in],yPos:y������
	///@pre NULL
	///@return LineObj
	LineObj* GetLineFromYPos(int yPos);
	///@brief ��ȡ������
	///
	///��ȡ������
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return int
	int		FindLineIndexFromCharPos(int charPos);
	///@brief ��ȡ������
	///
	///��ȡ������
	///@param[in],yPos:y������λ��
	///@pre NULL
	///@return int
	int		FindLineIndexFromYPos(int yPos);
	///@brief ��ȡ������
	///
	///��ȡ������
	///@pre NULL
	///@return int
	int GetLength();
	///@brief ��־����
	///
	///��־����
	///@pre NULL
	///@return void
	void Log();

protected:
	SplitVector<LineObj*> LineObjList_;
};
