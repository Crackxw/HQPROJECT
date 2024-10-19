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
// Desc: Wav ������ �ε� �Ͽ� Wav Header�� Wav Data�� �����Ѵ�. 
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
	
	// "RIFF" ���ڸ� �����Ѵ�.  
	fread(&FileType, 1, sizeof(FileType), fp);
	if( !memcmp(FileType.id, "RIFF", 4))
	{
		fseek(fp, 4, SEEK_CUR); // "WAVE" ���� 4����Ʈ �ǳʶ�� 
		// fmt Chunk�� �ǳʶ��.
		nSeekChunkLen = SeekChunk(fp, "fmt ");
		
		// Seek�� sizeof(WavHeader)���� ���� ���������� Seek�� �߸��Ǿ������ �̱� ������ 
		// sizeof(WavHeader) ũ�� ��ŭ�� �о�´�. 
		fread(WavHeader, 1, nSeekChunkLen > sizeof(WavHeader) ? sizeof(WavHeader) : nSeekChunkLen, fp);
		// Seek�� sizeof(WavHeader)���� ���� ���������� Seek�� �߸��Ǿ������ �̱� ������ 
		// ������ ��ġ�� ���� �ִ� ��ġ�� �ǵ�����. 
		fseek(fp, nSeekChunkLen > sizeof(WavHeader) ? nSeekChunkLen - sizeof(WavHeader) : 0, SEEK_CUR);

		// char������ �޾ƿ� FmtChunk ������ Wav_FmtChunk ������ �� ��ȯ�Ѵ�.
		pFmtChunk = reinterpret_cast<Wav_FmtChunk*>(WavHeader);

		m_strFileName = filename;
		if( pFmtChunk->FormatTag == WAV_FMT_PCM)
		{
			m_pFilePtr = fp;

			// Wav Chunk ������ ������ �ִ´�. 
			m_SoundDataHeader.unBitsPerSample = pFmtChunk->BitsPerSample;
			m_SoundDataHeader.unChannels	  = pFmtChunk->Channels;
			m_SoundDataHeader.unSamplesPerSec = pFmtChunk->SamplesPerSec;

			// ���� Wav Data�� Offset�� ������ �ִ´�. 
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
// Desc: Wav������ Chunk������ Seek �Ѵ�.
//		fp			  - Wav File Ptr
//		szChunkName   - Chunk Name. �������� �����ʹ� ������� �ʴ´� (Ȯ�ο�)
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
// Desc: Wav Data�� nReadSize��ŭ �о� pOutData�� return �Ѵ�. 
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