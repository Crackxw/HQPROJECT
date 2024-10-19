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
// Desc: Wav������ ���� Sound Buffer�� ����� Wav Data�� Sound Buffer�� ����. 
//		 Sound Buffer�� Wav Data�� ���� ���� Wav Data�� ���̻� �ʿ����� �ʴ�.
//		szFileName - Open�� ���ϸ�
//-----------------------------------------------------------------------------
BOOL CSoundPlayer::Open(const char* szFileName) 
{
	ISoundData* pSoundData;

	pSoundData = new CWav;
	if( OpenSoundData(pSoundData, szFileName))
	{
		// Wav Data�� Sound Buffer�� ����. 
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
		// Wav Data�� Sound Buffer�� ����. 
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
		// Wav Data�� Sound Buffer�� ����. 
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
	
	// Wav Data�� �о���δ�.
	if( pSoundData->Open(szFileName))
	{
		m_nBytePerSec = pSoundData->GetBytePerSecond();
		// Player�� Sound Buffer�� �����Ѵ�.
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
// Desc: ���� Player���� ������� SoundBuffer�� ���ϴ� ���� ��ŭ�� Wav �����͸� 
//		 �о�鿩 ����. 
//		nPos  - Sound Buffer�� ��ġ
//		nSize - Sound Buffer�� �� Data�� ũ��
//		pWav  - Sound Buffer�� �� Wav Data 
//-----------------------------------------------------------------------------
void CSoundPlayer::WriteWavDataToPlayerSoundBuffer(const int nPos, const int nSize, ISoundData* const pSoundData) const
{
	assert(m_pPlayer);

	void* pLockBuffer = NULL;

#ifdef _MT
	LockThread(0);
#endif
	// Sound Buffer�� Data�� ���� ���� Lock�� �Ǵ�
	pLockBuffer = m_pPlayer->Lock(m_nSndHandle, nPos, nSize);
	assert( pLockBuffer);

	if( !pSoundData)
		ZeroMemory(pLockBuffer, nSize); 
	else
		pSoundData->ReadData(pLockBuffer, nSize); // Wav Data�� pLockBuffer�� ����. 
	
	// Sound Buffer�� �� ����ߴٸ� UnLock �����ش�. 
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
// Desc: �����带 ����ϱ� ���ؼ� �����ص� �ڵ�� 
// ���� ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
// Desc: �����带 ����ϱ� ���ؼ� �����ص� �ڵ�� 
// �� -<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//-----------------------------------------------------------------------------
