// iSoundData.cpp: implementation of the iSoundData class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string>

#include "SoundPlayer_Common.h"
#include "iSoundData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISoundData::ISoundData() : m_pFilePtr(NULL), 
						   m_ulDataSize(0), 
						   m_ulDataStartOffset(0)
{
	memset(&m_SoundDataHeader, 0, sizeof(m_SoundDataHeader));
}

ISoundData::~ISoundData()
{

}

int ISoundData::GetBytePerSecond()
{

	// 초당Hz * 초당Bit * 채널 = 초당 비트값 
	// 초당 비트값 / 8 = 초당 바이트값
	return (GetSoundHeader()->unSamplesPerSec * GetSoundHeader()->unBitsPerSample * GetSoundHeader()->unChannels) / 8;
}

