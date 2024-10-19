// SoundPlayer.cpp: implementation of the CSoundPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#ifdef _MT
#include <process.h>
#endif

#include "IPlayer.h"
#include "iSoundData.h"
#include "SoundPlayer.h"
#include "Wav.h"
#include "..\..\GSLib\OggVorbis\vorbisfile.h"
#include "MyOgg.h"
#include "MyGBM.h"

#ifdef _MT
CS_OBJECTS CSoundPlayer::m_CSObjects;
#endif

#define SAFE_DELETE(p) { if(p) { delete (p);     (p)=NULL; } }
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundPlayer::CSoundPlayer(IPlayer* pPlayer) : m_nSndHandle(0),
											   m_nBytePerSec(0),
											   m_lVolume(0),
											   m_pPlayer(pPlayer)
{
	assert(pPlayer);
}

CSoundPlayer::~CSoundPlayer()
{

}

//-----------------------------------------------------------------------------
// Name: Open(const char* szFileName)
// Code: sagolboss (2004-06-17)
// Desc: Wav파일을 열어 Sound Buffer를 만들어 Wav Data를 Sound Buffer에 쓴다. 
//		 Sound Buffer에 Wav Data를 쓰고 나면 Wav Data는 더이상 필요하지 않다.
//		szFileName - Open할 파일명
//-----------------------------------------------------------------------------
BOOL CSoundPlayer::Open(const char* szFileName) 
{
	ISoundData* pSoundData;

	pSoundData = new CWav;
	if( OpenSoundData(pSoundData, szFileName))
	{
		// Wav Data를 Sound Buffer에 쓴다. 
		WriteWavDataToPlayerSoundBuffer(0, pSoundData->GetDataSize(), pSoundData);
		SAFE_DELETE(pSoundData);
		return TRUE;
	}
	else
	{
		SAFE_DELETE(pSoundData);
	}

	pSoundData = new CMyOgg;
	if( OpenSoundData(pSoundData, szFileName))
	{
		// Wav Data를 Sound Buffer에 쓴다. 
		WriteWavDataToPlayerSoundBuffer(0, pSoundData->GetDataSize(), pSoundData);
		SAFE_DELETE(pSoundData);
		return TRUE;
	}
	else
	{
		SAFE_DELETE(pSoundData);
	}
	
	pSoundData = new CMyGBM;
	if( OpenSoundData(pSoundData, szFileName))
	{
		// Wav Data를 Sound Buffer에 쓴다. 
		WriteWavDataToPlayerSoundBuffer(0, pSoundData->GetDataSize(), pSoundData);
		SAFE_DELETE(pSoundData);
		return TRUE;
	}
	else
	{
		SAFE_DELETE(pSoundData);
	}
	return FALSE;
}

BOOL CSoundPlayer::OpenSoundData(ISoundData* pSoundData, const char* szFileName)
{
	assert(pSoundData);
	
	// Wav Data를 읽어들인다.
	if( pSoundData->Open(szFileName))
	{
		m_nBytePerSec = pSoundData->GetBytePerSecond();
		// Player의 Sound Buffer를 생성한다.
		m_nSndHandle = m_pPlayer->CreateSoundBuffer(pSoundData->GetDataSize(), 
													pSoundData->GetSoundHeader()->unSamplesPerSec, 
													pSoundData->GetSoundHeader()->unBitsPerSample, 
													pSoundData->GetSoundHeader()->unChannels);
		if(m_nSndHandle != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: WriteWavDataToPlayer(const int nPos, const int nSize, CWav* const pWav) 
// Code: sagolboss (2004-06-17)
// Desc: 현재 Player에서 사용중인 SoundBuffer에 원하는 길이 만큼의 Wav 데이터를 
//		 읽어들여 쓴다. 
//		nPos  - Sound Buffer의 위치
//		nSize - Sound Buffer에 쓸 Data의 크기
//		pWav  - Sound Buffer에 쓸 Wav Data 
//-----------------------------------------------------------------------------
void CSoundPlayer::WriteWavDataToPlayerSoundBuffer(const int nPos, const int nSize, ISoundData* const pSoundData) const
{
	assert(m_pPlayer);

	void* pLockBuffer = NULL;

#ifdef _MT
	LockThread(0);
#endif
	// Sound Buffer에 Data를 쓰기 위해 Lock을 건다
	pLockBuffer = m_pPlayer->Lock(m_nSndHandle, nPos, nSize);
	assert( pLockBuffer);

	if( !pSoundData)
		ZeroMemory(pLockBuffer, nSize); 
	else
		pSoundData->ReadData(pLockBuffer, nSize); // Wav Data를 pLockBuffer에 쓴다. 
	
	// Sound Buffer를 다 사용했다면 UnLock 시켜준다. 
	m_pPlayer->UnLock(m_nSndHandle);

#ifdef _MT
	UnLockThread(0);
#endif
}

void CSoundPlayer::Play(BOOL bLoop) 
{
	assert(m_pPlayer);

	SetVolume(m_lVolume);
	m_pPlayer->Play(m_nSndHandle, bLoop);
}

void CSoundPlayer::Pause() const
{
	assert(m_pPlayer);
	m_pPlayer->Pause(m_nSndHandle);
}

void CSoundPlayer::Close() const
{
	assert(m_pPlayer);
	m_pPlayer->Close(m_nSndHandle);
}

int CSoundPlayer::GetTick() const
{
	assert(m_pPlayer);
	int nCurrPos = m_pPlayer->GetCurrentPos(m_nSndHandle);
	if( nCurrPos == -1)
		return -1;

	return nCurrPos * 1000 / m_nBytePerSec;
}

void CSoundPlayer::SetVolume(const long lVolume)
{
	assert(m_pPlayer);
	m_lVolume = lVolume;
	m_pPlayer->SetVolume(m_lVolume);
}

//-----------------------------------------------------------------------------
// Code: sagolboss (2004-06-17)
// Desc: 쓰레드를 사용하기 위해서 마련해둔 코드들 
// 시작 ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-----------------------------------------------------------------------------
#ifdef _MT
void CSoundPlayer::LockThread(const int nIndex) 
{
	assert(nIndex < CS_MAX);
	EnterCriticalSection(&m_CSObjects.sCriticalSection[nIndex]);
}

void CSoundPlayer::UnLockThread(const int nIndex) 
{
	assert(nIndex < CS_MAX);
	LeaveCriticalSection(&m_CSObjects.sCriticalSection[nIndex]);
}
#endif
//-----------------------------------------------------------------------------
// Code: sagolboss (2004-06-17)
// Desc: 쓰레드를 사용하기 위해서 마련해둔 코드들 
// 끝 -<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//-----------------------------------------------------------------------------
