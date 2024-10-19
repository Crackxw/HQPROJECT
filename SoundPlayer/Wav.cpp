// Wav.cpp: implementation of the CWav class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string>

#include "SoundPlayer_Common.h"
#include "iSoundData.h"
#include "Wav.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWav::CWav()   
{
}

CWav::~CWav()
{
	Release();
}

void CWav::Release()
{
	if (m_pFilePtr)
		fclose(m_pFilePtr);
}

//-----------------------------------------------------------------------------
// Name: Open(const char* filename)
// Code: sagolboss (2004-06-16)
// Desc: Wav 파일을 로드 하여 Wav Header랑 Wav Data를 적재한다. 
//-----------------------------------------------------------------------------
BOOL CWav::Open(const char* filename)
{
	File_Type     FileType;
	Wav_FmtChunk* pFmtChunk;
	char		  WavHeader[64];
	int			  nSeekChunkLen = 0;
	FILE*		  fp;
	
	fp = fopen(filename, "rb");
	if( !fp)
		return FALSE;
	
	// "RIFF" 글자를 추츨한다.  
	fread(&FileType, 1, sizeof(FileType), fp);
	if( !memcmp(FileType.id, "RIFF", 4))
	{
		fseek(fp, 4, SEEK_CUR); // "WAVE" 글자 4바이트 건너띄기 
		// fmt Chunk로 건너띈다.
		nSeekChunkLen = SeekChunk(fp, "fmt ");
		
		// Seek를 sizeof(WavHeader)보다 많이 했을때에는 Seek가 잘못되었을경우 이기 때문에 
		// sizeof(WavHeader) 크기 만큼만 읽어온다. 
		fread(WavHeader, 1, nSeekChunkLen > sizeof(WavHeader) ? sizeof(WavHeader) : nSeekChunkLen, fp);
		// Seek를 sizeof(WavHeader)보다 많이 했을때에는 Seek가 잘못되었을경우 이기 때문에 
		// 파일의 위치를 원래 있던 위치로 되돌린다. 
		fseek(fp, nSeekChunkLen > sizeof(WavHeader) ? nSeekChunkLen - sizeof(WavHeader) : 0, SEEK_CUR);

		// char형으로 받아온 FmtChunk 정보를 Wav_FmtChunk 형으로 형 변환한다.
		pFmtChunk = reinterpret_cast<Wav_FmtChunk*>(WavHeader);

		m_strFileName = filename;
		if( pFmtChunk->FormatTag == WAV_FMT_PCM)
		{
			m_pFilePtr = fp;

			// Wav Chunk 정보를 가지고 있는다. 
			m_SoundDataHeader.unBitsPerSample = pFmtChunk->BitsPerSample;
			m_SoundDataHeader.unChannels	  = pFmtChunk->Channels;
			m_SoundDataHeader.unSamplesPerSec = pFmtChunk->SamplesPerSec;

			// 실제 Wav Data의 Offset을 가지고 있는다. 
			m_ulDataSize		= SeekChunk(fp, "data ");
			m_ulDataStartOffset = ftell(fp);
			return TRUE;
		}
	}

	fclose(fp);
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: SeekChunk(FILE* fp, const char* searchString)
// Code: sagolboss (2004-06-16)
// Desc: Wav파일을 Chunk단위로 Seek 한다.
//		fp			  - Wav File Ptr
//		szChunkName   - Chunk Name. 실제적인 데이터는 사용하지 않는다 (확인용)
//-----------------------------------------------------------------------------
int CWav::SeekChunk(FILE* fp, const char* szChunkName)
{
	File_Type FileType;
	ZeroMemory(&FileType, sizeof(FileType));
	
	do
	{
		fseek(fp, FileType.nLength, SEEK_CUR);
		if( fread(&FileType, 1, sizeof(FileType), fp) != sizeof(FileType))
			return -1;
	} while(memcmp(FileType.id, szChunkName, 4));
	
	return FileType.nLength;
}


//-----------------------------------------------------------------------------
// Name: ReadWav(void* pOutData, const int nReadSize)
// Code: sagolboss (2004-06-16)
// Desc: Wav Data를 nReadSize만큼 읽어 pOutData에 return 한다. 
//-----------------------------------------------------------------------------
void CWav::ReadData(void* pOutData, const int nReadSize)
{
	fread(pOutData, 1, nReadSize, m_pFilePtr);
}

BOOL CWav::Rewind()
{
	if( m_pFilePtr)
	{
		fseek(m_pFilePtr, m_ulDataStartOffset, SEEK_SET);
		return TRUE;
	}
	return FALSE;
}