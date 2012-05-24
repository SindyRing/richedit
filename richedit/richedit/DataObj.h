/**
*@file DataObj.h
*@brief ¼ôÇÐ°ådata obj
*
*¼ôÇÐ°ådata obj Ïà¹Ø²Ù×÷
*
*@author ÁÖÐÂµÜ(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#include <string>
#include "editorInterface.h"
#include "define.h"
class CDataObject;

/************************************************************************/
/*   CEnumFormatEtc                                                    */
/************************************************************************/

class CEnumFormatEtc : public IEnumFORMATETC
{
public:
	CEnumFormatEtc():m_vFormats(NULL),m_ref(0),m_nCur(0),m_nTotal(0){
	}
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj)
	{
		if(!ppvObj) return E_INVALIDARG;
		*ppvObj = NULL;

		HRESULT		hResult = E_NOINTERFACE;
		if( IsEqualIID(riid, IID_IUnknown) ||
			IsEqualIID(riid, IID_IEnumFORMATETC) )
		{
			*ppvObj = this;
			AddRef();
			hResult = NOERROR;
		}
		return hResult;
	}
	STDMETHOD_(ULONG,AddRef)(void){
		return ++ m_ref;
	}
	STDMETHOD_(ULONG,Release)(void){
		if((--m_ref) == 0){
			if(m_vFormats)
				delete[] m_vFormats;
			delete this;
			return 0;
		}
		return m_ref;
	}

	STDMETHOD(Next) (ULONG celt, FORMATETC *rgelt,
		ULONG *pceltFetched)
	{

		HRESULT		hresult = NOERROR;
		ULONG		cFetched;
		if( pceltFetched == NULL && celt != 1 )
		{
			return E_INVALIDARG;
		}
		if( celt > m_nTotal - m_nCur)
		{
			cFetched = m_nTotal - m_nCur;
			hresult = S_FALSE;
		}else{
			cFetched = celt;
		}
		if( cFetched > 0 ){
			memcpy( rgelt, m_vFormats + m_nCur,
				cFetched * sizeof(FORMATETC) );
		}
		m_nCur += cFetched;

		if( pceltFetched )	{
			*pceltFetched = cFetched;
		}
		return hresult;
	}
	STDMETHOD(Skip) (ULONG celt){

		HRESULT		hresult = NOERROR;
		m_nCur += celt;
		if( m_nCur > m_nTotal ){
			m_nCur = m_nTotal;
			hresult = S_FALSE;
		}
		return hresult;
	}
	STDMETHOD(Reset) (void){
		m_nCur = 0;
		return S_OK;
	}
	STDMETHOD(Clone) (IEnumFORMATETC **ppenum){
		return CEnumFormatEtc::Create(m_vFormats, m_nTotal, ppenum);
	}

	static HRESULT Create( FORMATETC *prgFormats, ULONG cTotal, 
		IEnumFORMATETC **ppenum )
	{
		CEnumFormatEtc *penum = new CEnumFormatEtc();
		if( penum != NULL )
		{
			if( cTotal > 0 )
			{
				penum->m_vFormats = new FORMATETC[cTotal];
				if( penum->m_vFormats )
				{
					penum->m_nTotal = cTotal;
					memcpy(penum->m_vFormats, prgFormats, 
						cTotal * sizeof(FORMATETC));
					return  penum->QueryInterface(IID_IEnumFORMATETC, (void**)ppenum);
				}	
			}
		}
		return E_OUTOFMEMORY;
	}
private:
	ULONG		m_ref;
	ULONG       m_nCur; 	
	ULONG       m_nTotal;   
	FORMATETC * m_vFormats;
};


/************************************************************************/
/*   CDataObject                                                    */
/* http://www.cppblog.com/windcsn/archive/2006/03/03/3668.html */
/************************************************************************/

class CDataObject : public IDataObject
{
public:
	// IUnknown members
	HRESULT __stdcall QueryInterface (REFIID iid, void ** ppvObject);
	ULONG   __stdcall AddRef (void);
	ULONG   __stdcall Release (void);

	// IDataObject members
	HRESULT __stdcall GetData (FORMATETC *pFormatEtc, STGMEDIUM *pmedium);
	HRESULT __stdcall GetDataHere (FORMATETC *pFormatEtc, STGMEDIUM *pmedium);
	HRESULT __stdcall QueryGetData (FORMATETC *pFormatEtc);
	HRESULT __stdcall GetCanonicalFormatEtc (FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut);
	HRESULT __stdcall SetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease);
	HRESULT __stdcall EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
	HRESULT __stdcall DAdvise (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *, DWORD *);
	HRESULT __stdcall DUnadvise (DWORD      dwConnection);
	HRESULT __stdcall EnumDAdvise (IEnumSTATDATA **ppEnumAdvise);

	// Constructor / Destructor
	CDataObject(MY_CHAR* c);
	~CDataObject();
private:
	LONG m_lRefCount;
	int LookupFormatEtc(FORMATETC *pFormatEtc);
	std::wstring data;
};
