// MyOgg.h: interface for the CMyOgg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYOGG_H__C4CE94CA_E306_404D_A61E_EED00F24220F__INCLUDED_)
#define AFX_MYOGG_H__C4CE94CA_E306_404D_A61E_EED00F24220F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWav;
class CMyOgg : public ISoundData
{
public:
	CMyOgg();
	virtual ~CMyOgg();

	virtual BOOL Open(const char* szFileName);
	void ReadData(void* pOutData, const int nReadSize);
	BOOL Rewind();

protected:
	OggVorbis_File m_OggVoribsFile;
	vorbis_info*   m_pVorbisInfo;
};

#endif // !defined(AFX_MYOGG_H__C4CE94CA_E306_404D_A61E_EED00F24220F__INCLUDED_)
