// iSoundData.h: interface for the iSoundData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISOUNDDATA_H__5E54BA0F_2E9A_457F_8375_CA6B61CA8765__INCLUDED_)
#define AFX_ISOUNDDATA_H__5E54BA0F_2E9A_457F_8375_CA6B61CA8765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;
struct SoundData_Header
{
	unsigned int unSamplesPerSec;
	unsigned int unBitsPerSample;
	unsigned int unChannels;
	unsigned long ulBlockAlign;
	unsigned long ulAvgBytesPerSec;
};

class ISoundData 
{
public:
	ISoundData();
	virtual ~ISoundData();

	virtual BOOL Open(const char* szFileName) = 0;
	virtual BOOL Rewind() = 0;
	virtual void ReadData(void* pOutData, const int nReadSize) = 0;

	inline int GetDataSize() { return m_ulDataSize; }
	inline int GetDataStartOffset() { return m_ulDataStartOffset; };
	int	       GetBytePerSecond();

	inline const char* GetFileName() { return m_strFileName.c_str();}
	SoundData_Header*  GetSoundHeader() { return &m_SoundDataHeader; }

protected:
 	FILE*		  m_pFilePtr;
	unsigned long m_ulDataSize;
    unsigned long m_ulDataStartOffset;
	string        m_strFileName;

	SoundData_Header m_SoundDataHeader;
};

#endif // !defined(AFX_ISOUNDDATA_H__5E54BA0F_2E9A_457F_8375_CA6B61CA8765__INCLUDED_)
