// MyGBM.h: interface for the CMyGBM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGBM_H__DF36F1AE_53F8_462C_BA1D_7FE1CCC86DE0__INCLUDED_)
#define AFX_MYGBM_H__DF36F1AE_53F8_462C_BA1D_7FE1CCC86DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyGBM : public CMyOgg
{
public:
	CMyGBM();
	virtual ~CMyGBM();

	BOOL Open(const char* szFileName);
};

#endif // !defined(AFX_MYGBM_H__DF36F1AE_53F8_462C_BA1D_7FE1CCC86DE0__INCLUDED_)
