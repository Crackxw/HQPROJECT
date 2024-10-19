// Wav.h: interface for the CWav class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAV_H__0545A7A4_DA00_4310_A2A2_776863BE3EEC__INCLUDED_)
#define AFX_WAV_H__0545A7A4_DA00_4310_A2A2_776863BE3EEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WAV_FMT_PCM			0x01
#define WAV_FMT_IMA_ADPCM	0x11

class CWav : public ISoundData
{
public:
	CWav();
	virtual ~CWav();

	void Release();
	BOOL Open(const char* filename);
	void ReadData(void* pOutData, const int nReadSize);
	BOOL Rewind();

	static int SeekChunk(FILE* fp, const char* szChunkName);
};	

#endif // !defined(AFX_WAV_H__0545A7A4_DA00_4310_A2A2_776863BE3EEC__INCLUDED_)
