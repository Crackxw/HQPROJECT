// MyGBM.cpp: implementation of the CMyGBM class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <Winbase.h>
#include <process.h>
#include <string>

#include "SoundPlayer_Common.h"
#include "..\..\GSLib\OggVorbis\vorbisfile.h"
#include "iSoundData.h"
#include "MyOgg.h"
#include "MyGBM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyGBM::CMyGBM()
{
}

CMyGBM::~CMyGBM()
{
}

BOOL CMyGBM::Open(const char* szFileName)
{
	FILE*	  fp;
	File_Type FileType;


	fp = fopen(szFileName, "rb");
	if( !fp)
		return FALSE;

	fread(&FileType, 1, sizeof(FileType), fp);
	fseek(fp, 0, SEEK_SET);

	if( !memcmp(FileType.id, "GBMS", 4))
	{
		if( ov_open(fp, &m_OggVoribsFile, "GBM", 0) < 0)
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

		m_pFilePtr	  = fp;
		m_strFileName = szFileName;
		return TRUE;
	}
	fclose(fp);
	return FALSE;
}