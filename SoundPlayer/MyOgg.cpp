// MyOgg.cpp: implementation of the CMyOgg class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string>

#include "SoundPlayer_Common.h"
#include "..\..\GSLib\OggVorbis\vorbisfile.h"
#include "iSoundData.h"
#include "MyOgg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyOgg::CMyOgg() : m_pVorbisInfo(NULL)
{
	memset(&m_OggVoribsFile, 0, sizeof(m_OggVoribsFile));
}

CMyOgg::~CMyOgg()
{
	if( m_pFilePtr)
		fclose(m_pFilePtr);

	ov_clear(&m_OggVoribsFile);
}


BOOL CMyOgg::Open(const char* szFileName)
{
	FILE*	  fp;
	File_Type FileType;

	fp = fopen(szFileName, "rb");
	if( !fp)
		return FALSE;

	fread(&FileType, 1, sizeof(FileType), fp);
	fseek(fp, 0, SEEK_SET);

	if( !memcmp(FileType.id, "OggS", 4))
	{
		if( ov_open(fp, &m_OggVoribsFile, "Ogg", 0) < 0)
		{
			fclose(fp);
			return FALSE;
		}
		m_pVorbisInfo = ov_info(&m_OggVoribsFile, -1);
		if( !m_pVorbisInfo)
		{
			fclose(fp);
			ov_clear(&m_OggVoribsFile);
			return FALSE;
		}

		m_SoundDataHeader.unSamplesPerSec  = m_pVorbisInfo->rate;
		m_SoundDataHeader.unChannels	   = m_pVorbisInfo->channels;
		m_SoundDataHeader.unBitsPerSample  = 16;
		m_SoundDataHeader.ulBlockAlign     = (m_SoundDataHeader.unBitsPerSample / 8) * m_SoundDataHeader.unChannels;
		m_SoundDataHeader.ulAvgBytesPerSec = m_SoundDataHeader.unSamplesPerSec * m_SoundDataHeader.ulBlockAlign;
		
		m_ulDataSize        = static_cast<unsigned long>(ov_pcm_total(&m_OggVoribsFile, -1) * 
							  m_SoundDataHeader.ulBlockAlign);
		m_ulDataStartOffset = static_cast<unsigned long>(m_OggVoribsFile.offset);

		m_pFilePtr    = fp;
		m_strFileName = szFileName;
		return TRUE;
	}
	fclose(fp);
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: ReadData(void* pOutData, const int nReadSize)
// Code: sagolboss (2004-06-21)
// Desc: Ogg Lib 함수를 이용하여 Ogg Data를 Wav Data로 nReadSize 만큼 변환하여 
//		 return 한다.
//		pOutData   - Ogg -> Wav 로 컨버팅된 Sound Data
//		nReadSize  - 읽을 크기
//-----------------------------------------------------------------------------
void CMyOgg::ReadData(void* pOutData, const int nReadSize)
{
	int nCount     = 0;
	int nRet	   = 0;
	int nBitStream = 0;
	char* buffer   = (char*)pOutData;

	// 원하는 길이만큼 읽어들인뒤 break 한다 
	while(nCount < nReadSize)
	{
		// Ogg Lib에 있는 함수 
		// Ogg 를 Wav로 컨버팅 해서 읽어들이고 디코딩된 순수 Data Size를 return 한다  
		nRet = ov_read(&m_OggVoribsFile, buffer + nCount, nReadSize - nCount, 0, 
					   GetSoundHeader()->unBitsPerSample / 8, 1, &nBitStream);
		if( nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
}


//-----------------------------------------------------------------------------
// Name: Rewind()
// Code: sagolboss (2004-06-21)
// Desc: 처음으로 음악을 돌린다.
//-----------------------------------------------------------------------------
BOOL CMyOgg::Rewind()
{	
	ov_pcm_seek(&m_OggVoribsFile, m_ulDataStartOffset);
	return FALSE;
}

