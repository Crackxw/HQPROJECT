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
// Desc: Ogg Lib �Լ��� �̿��Ͽ� Ogg Data�� Wav Data�� nReadSize ��ŭ ��ȯ�Ͽ� 
//		 return �Ѵ�.
//		pOutData   - Ogg -> Wav �� �����õ� Sound Data
//		nReadSize  - ���� ũ��
//-----------------------------------------------------------------------------
void CMyOgg::ReadData(void* pOutData, const int nReadSize)
{
	int nCount     = 0;
	int nRet	   = 0;
	int nBitStream = 0;
	char* buffer   = (char*)pOutData;

	// ���ϴ� ���̸�ŭ �о���ε� break �Ѵ� 
	while(nCount < nReadSize)
	{
		// Ogg Lib�� �ִ� �Լ� 
		// Ogg �� Wav�� ������ �ؼ� �о���̰� ���ڵ��� ���� Data Size�� return �Ѵ�  
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
// Desc: ó������ ������ ������.
//-----------------------------------------------------------------------------
BOOL CMyOgg::Rewind()
{	
	ov_pcm_seek(&m_OggVoribsFile, m_ulDataStartOffset);
	return FALSE;
}

