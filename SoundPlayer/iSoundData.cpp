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

	// �ʴ�Hz * �ʴ�Bit * ä�� = �ʴ� ��Ʈ�� 
	// �ʴ� ��Ʈ�� / 8 = �ʴ� ����Ʈ��
	return (GetSoundHeader()->unSamplesPerSec * GetSoundHeader()->unBitsPerSample * GetSoundHeader()->unChannels) / 8;
}

