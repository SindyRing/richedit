
#include <GdiPlus.h>

using namespace Gdiplus;

class CGdiPlusGetter
{
public: 
	CGdiPlusGetter()
	{
		Status ret = GdiplusStartup(&m_token, &m_pStartupInput, NULL);
		assert(ret == Ok);
	}

	~CGdiPlusGetter()
	{
		GdiplusShutdown(m_token);
	}

protected:
	GdiplusStartupInput m_pStartupInput;

	ULONG_PTR		m_token;

};