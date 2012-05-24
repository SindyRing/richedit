/**
*@file DataObj.cpp
*@brief 剪切板data obj
*
*剪切板data obj 相关操作
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include "DataObj.h"
#define countof(data, type) (sizeof(data) / sizeof(type))

HGLOBAL	GlobalAllocText(const std::wstring& text)
{
	HGLOBAL hRcText = GlobalAlloc(GMEM_DDESHARE,	
		(text.size() + 1) * sizeof(wchar_t) );
	if( !hRcText ) return NULL;

	wchar_t* pText = (wchar_t *)GlobalLock(hRcText);
	if( !pText) {
		return NULL;
	}
	memcpy(pText, text.data(), (text.size() + 1) * sizeof(wchar_t));
	GlobalUnlock(hRcText);
	return hRcText;
}

HGLOBAL	GlobalAllocTextA(const std::string& text)
{
	HGLOBAL hRcText = GlobalAlloc(GMEM_DDESHARE,	
		(text.size() + 1) * sizeof(CHAR) );
	if( !hRcText ) return NULL;

	CHAR* pText = (CHAR *)GlobalLock(hRcText);
	if( !pText) {
		return NULL;
	}
	memcpy(pText, text.data(), (text.size() + 1) * sizeof(CHAR));
	GlobalUnlock(hRcText);
	return hRcText;
}


FORMATETC g_rgFETC[] =
{
	{CF_UNICODETEXT,	NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL},	// Unicode Text
	{CF_TEXT,			NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL},	// Text
	{CF_BITMAP,			NULL, DVASPECT_CONTENT, -1, TYMED_GDI},     //image
};


/************************************************************************/
/*   CDataObject                                                    */
/************************************************************************/

CDataObject::CDataObject(MY_CHAR* c)
{
	USES_CONVERSION;
	// reference count must ALWAYS start at 1
	m_lRefCount  = 1;
	data = c;
}

CDataObject::~CDataObject()
{
}

HRESULT __stdcall CDataObject::QueryGetData(FORMATETC *pFormatEtc)
{
	return (LookupFormatEtc(pFormatEtc) == -1) ? DV_E_FORMATETC : S_OK;
}

int CDataObject::LookupFormatEtc(FORMATETC *pFormatEtc)
{
	int nSupportText = countof(g_rgFETC, FORMATETC);
	// 轮流检查格式看是否能找到匹配的格式
	for(int i = 0; i < nSupportText; i++)
	{
		if((g_rgFETC[i].tymed    &  pFormatEtc->tymed)   &&
			g_rgFETC[i].cfFormat == pFormatEtc->cfFormat &&
			g_rgFETC[i].dwAspect == pFormatEtc->dwAspect)
		{
			// return index of stored format
			return i;
		}
	}

	// error, format not found
	return -1;
}

HRESULT __stdcall CDataObject::GetData (FORMATETC *pFormatEtc, STGMEDIUM *pStgMedium)
{
	int idx;
	// try to match the specified FORMATETC with one of our supported formats
	if((idx = LookupFormatEtc(pFormatEtc)) == -1)
		return DV_E_FORMATETC;
	// found a match - transfer data into supplied storage medium
	pStgMedium->tymed           = g_rgFETC[idx].tymed;
	pStgMedium->pUnkForRelease  = 0;
	// copy the data into the caller's storage medium
	switch(g_rgFETC[idx].tymed)
	{
	case TYMED_HGLOBAL:
		{
			pStgMedium->tymed = TYMED_HGLOBAL;
			if( pFormatEtc->cfFormat ==  CF_UNICODETEXT)
			{
				//std::wstring x = A2W()
				pStgMedium->hGlobal = GlobalAllocText(data);
			}else if( pFormatEtc->cfFormat == CF_TEXT)
			{
				std::string x = CW2A(data.c_str());
				pStgMedium->hGlobal = GlobalAllocTextA(x);
			}
			else{
				return E_NOTIMPL;
			}
			return S_OK;
		}
		break;
	default:
		return DV_E_FORMATETC;
	}
	return S_OK;
}

HRESULT __stdcall CDataObject::EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
	// OLE仅仅支持得到方向成员
	if(dwDirection == DATADIR_GET)
	{
		// 在WIN2K下，你可以调用AIP函数SHCreateStdEnumFmtEtc来完成，但为了支持//所有的window平台，我们需要实现IEnumFormatEtc。
		return CEnumFormatEtc::Create(g_rgFETC, countof(g_rgFETC,FORMATETC), ppEnumFormatEtc);
	}
	else
	{
		// the direction specified is not supported for drag+drop
		return E_NOTIMPL;
	}
}

HRESULT CDataObject::DAdvise (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT CDataObject::DUnadvise (DWORD dwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT CDataObject::EnumDAdvise (IEnumSTATDATA **ppEnumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT CDataObject::GetDataHere (FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
	return DATA_E_FORMATETC;
}

HRESULT CDataObject::GetCanonicalFormatEtc (FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut)
{
	// Apparently we have to set this field to NULL even though we don't do anything else
	pFormatEtcOut->ptd = NULL;
	return E_NOTIMPL;
}

HRESULT CDataObject::SetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease)
{
	return E_NOTIMPL;
}

HRESULT CDataObject::QueryInterface( REFIID riid,void  **ppvObject)
{
	if(!ppvObject)
		return E_INVALIDARG;
	*ppvObject = NULL;


	HRESULT		hResult = E_NOINTERFACE;

	if( IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_IDataObject) )
	{
		*ppvObject = this;
		AddRef();
		hResult = NOERROR;
	}
	return hResult;
}

ULONG CDataObject::AddRef(void){
	return ++m_lRefCount;
}
ULONG CDataObject::Release(void){
	if((--m_lRefCount) == 0){
		delete this;
		return 0;
	}
	return m_lRefCount;
}