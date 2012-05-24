/**
*@file style.h
*@brief ��ʽ���������
*
*������ʽ���������ع����߼�
*
*@author ���µ�(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#pragma once
#include "SplitVector.h"
#include "editorInterface.h"
#include "common.h"
#include <WinGDI.h>

///@brief �����ʽ��չ��
///
///�����ʽ��չ��
class CharFormat
	: public CHARFORMAT
{
public:
	CharFormat()
	{
	}

	CHARFORMAT& operator = (CHARFORMAT& cf)
	{
		bCharSet = cf.bCharSet;
		bPitchAndFamily = cf.bPitchAndFamily;
		cbSize = cf.cbSize;
		crTextColor = cf.crTextColor;
		dwEffects = cf.dwEffects;
		dwMask = cf.dwMask;
		lstrcpy(szFaceName, cf.szFaceName);
		yHeight = cf.yHeight;
		yOffset = cf.yOffset;
		return *this;
	}
};


///@brief ���Ӷ���
///
///���Ӷ������Ժ���ز���
struct LinkObj
	: public IBaseObj
{
	int charPos_;
	int nRef_;
	MY_CHAR* context_;


	LinkObj(int charPos, MY_CHAR* context)
	{
		int len = lstrlen(context);
		context_ = new MY_CHAR[len + 1];
		lstrcpy(context_, context);
		charPos_ = charPos;
		nRef_ = 0;
		AddRef();
	}

	~LinkObj()
	{
		ClearContext();
	}

	void ResetContext(MY_CHAR* context)
	{
		ClearContext();
		context_ = context;
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

protected:
	void ClearContext()
	{
		if(context_)
			delete context_;
	}
};


///@brief ��ʽ����
///
///��ʽ�������Ժ���ز���
struct StyleObj : public IBaseObj
{
	enum StyleType
	{
		ST_CHAR = 0, 
		ST_OBJ
	};

	enum StyleRemoveType
	{
		SRT_SUCCEED = 0,
		SRT_ALLDELETE,
		SRT_NOINRANGE
	};

	StyleObj(int nCharPos, CHARFORMAT& cf, IObj* obj = NULL)
	{
		type_ = ST_CHAR;
		if (obj)
			type_ = ST_OBJ;
		charPos_ = nCharPos;
		ZeroMemory(&cf_,sizeof(cf_));
		cf_ = cf;
		obj_ = obj;
		charLength_ = 0;
		nRef_ = 0;
		link_ = NULL;
		font_ = NULL;

		AddRef();
		if (obj_)
			obj->AddRef();
		else
			InitFont();
	}

	void InitFont()
	{
		font_ = ::CreateFont(-cf_.yHeight, 0, 0, 0
			, cf_.dwMask & CFM_BOLD ? FW_BOLD : FW_NORMAL
			, cf_.dwMask & CFM_ITALIC ? 1 : 0
			, cf_.dwMask & CFM_UNDERLINE ? 1 : 0
			, FALSE
			, DEFAULT_CHARSET
			, OUT_OUTLINE_PRECIS
			, CLIP_DEFAULT_PRECIS
			, CLEARTYPE_QUALITY
			, VARIABLE_PITCH
			, cf_.szFaceName);
	}

	void SetLink(LinkObj* link)
	{
		link_ = link;
	}

	LinkObj* AddRefLink()
	{
		assert(link_);
		if (link_)
			link_->AddRef();
		return link_;
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

	~StyleObj()
	{
		if(obj_)
			obj_->Release();

		if (link_)
			link_->Release();

		if(font_)
		{
			::DeleteObject((HGDIOBJ)font_);
			font_ = NULL;
		}
	}

	enum PosType{
		low = 0,
		in,
		beyond
	};

	bool IsObj() const
	{
		if (type_ == ST_CHAR)
			return false;
		return true;
	}

	bool IsEqual(const StyleObj* other)
	{
		if (!other)
			return false;

		if ( charPos_ != other->charPos_ )
			return false;
		return true;
	}

	bool IsSameStyle(const StyleObj* other)
	{
		if (!other)
			return false;

		if (other->IsObj())
			return false;

		if (other->link_ && link_)
		{
			if (lstrcmp(other->link_->context_, link_->context_) != 0)
				return false;
		}

		if (IsCharFormatEqual(cf_, other->cf_) 
			&& other->charPos_ >= charPos_ 
			&& other->charPos_ <= (charPos_ + charLength_) )
		{
			return true;
		}
		return false;
	}

	bool IsFormatEqual(const StyleObj* other)
	{
		if (IsCharFormatEqual(cf_, other->cf_) )
			return true;
		return false;
	}

	int GetPosType(int charPos)
	{
		if (charPos > (charPos_ + charLength_ - 1))
			return beyond;
		else if (charPos < charPos_)
			return low;
		else
			return in;
	}

	void AddSameStyle(int length)
	{
		charLength_ += length;
		assert(charLength_ >= 0);
	}

	void RemoveSameStyle(int length)
	{
		charLength_ -= length;
		assert(charLength_ >= 0);
	}

	int ReMoveRange(int begin, int end)
	{
		int beginRange = MAX(begin, charPos_);
		int charEndPos = charPos_ + charLength_ -1;
		int endRange = MIN(end, charEndPos);
		if (beginRange > charEndPos)
		{
			return SRT_NOINRANGE;
		}
		else if(beginRange == charPos_ && endRange == charEndPos)
		{
			return SRT_ALLDELETE;
		}
		else
		{
			RemoveSameStyle(endRange - beginRange + 1);
			if (begin < charPos_)
				charPos_ = begin;
			return SRT_SUCCEED;
		}
	}

	bool IsLink()
	{
		if (cf_.dwEffects & CFM_LINK)
			return true;
		else
			return false;
	}


	int type_;
	int charPos_;
	int charLength_;
	CharFormat cf_;
	IObj* obj_;
	LinkObj* link_;
	HFONT font_;

	int nRef_;
};

///@brief ��ʽ���������
///
///��ʽ����������ز���
class StyleObjManager
{
public:
	StyleObjManager();
	//@brief ������ʽ
	///
	///������ʽ
	///@param[in],charPos:�ַ�λ��
	///@param[in],length:���볤��
	///@param[in],cf:�����ʽ
	///@param[in],hyperLinkData:���Ӳ���
	///@pre NULL
	///@return void
	void InsertStyle(int charPos, int length, CHARFORMAT& cf, MY_CHAR* hyperLinkData = NULL);
	//@brief ������ʽ
	///
	///������ʽ
	///@param[in],charPos:�ַ�λ��
	///@param[in],obj:����
	///@pre NULL
	///@return void
	void InsertStyle(int charPos, IObj* obj);
	//@brief ������ʽ
	///
	///������ʽ
	///@param[in],index:��ʽ����
	///@param[in],obj:����
	///@pre NULL
	///@return void
	void InsertStyle(int index, StyleObj* obj);
	//@brief ɾ����ʽ
	///
	///ɾ����ʽ
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return void
	void DeleteStyle(int charPos);
	//@brief ɾ����ʽ
	///
	///ɾ����ʽ
	///@param[in],charBegin:�ַ�λ��
	///@param[in],charEnd:�ַ�λ��
	///@pre NULL
	///@return void
	void DeleteStyle(int charBegin, int charEnd);
	//@brief ������ʽ
	///
	///������ʽ
	///@param[in],begin:�ַ�λ��
	///@param[in],charLength:�ַ�����
	///@param[in],objBegin:��ʽ����
	///@pre NULL
	///@return void
	void UpdateStylePosFromIndex(int begin, int charLength, StyleObj* objBegin);
	///@brief ��ȡ��ʽ����
	///
	///����������ȡ��ʽ
	///@param[in],index:����
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObj(int index);
	//@brief ��ȡ��ʽ����
	///
	///�����ַ�λ�û�ȡ��ʽ
	///@param[in],charPos:�ַ�λ��
	///@pre NULL
	///@return StyleObj
	StyleObj* GetStyleObjFromCharPos(int charPos);
	///@brief ��ȡ��ʽ����
	///
	///�����ַ�λ�û�ȡ��ʽ����
	///@pre NULL
	///@return int
	void SetBatch(bool bBatch);
	///@brief ��ȡ��ʽ����
	///
	///�����ַ�λ�û�ȡ��ʽ����
	///@pre NULL
	///@return int
	int FindStylePosFromCharPos(int charPos);
	///@brief ��ȡ��ʽ����
	///
	///��ȡ��ʽ����
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
	SplitVector<StyleObj*> objList_;
	int					totalChar_;
	bool				bBatch_;
};
