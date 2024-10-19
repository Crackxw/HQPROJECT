// DxSndPlayer.cpp: implementation of the CDxSndPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <dsound.h>
#include <crtdbg.h>
#include <assert.h>

#include "IPlayer.h"
#include "DxSndPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDxSndPlayer::CDxSndPlayer() : m_pDirectSound(NULL),
							   m_pPrimaryBuffer(NULL),
							   m_bBringDirectSoundDevice(FALSE)
{
	ZeroMemory(&m_SndBuffer, sizeof(m_SndBuffer));
}
	
CDxSndPlayer::~CDxSndPlayer()
{
	if( !m_bBringDirectSoundDevice)
	{
		for( int i = 0; i < SOUNDBUFFER_MAX; ++i)
			ReleaseSoundBuffer(i);

		if( m_pPrimaryBuffer)
		{
			m_pPrimaryBuffer->Release();
			m_pPrimaryBuffer = NULL;
		}

		if( m_pDirectSound)
		{
			m_pDirectSound->Release();
			m_pDirectSound = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Init(const HWND hWnd, const DWORD dwCooperativeLevel)
// Code: sagolboss (2004-06-17)
// Desc: DirectSound�� �����ϰ� ���������� �����Ѵ�
//		hWnd			   - DirectSound�� ������ HWND
//		dwCooperativeLevel - �������� ��
//-----------------------------------------------------------------------------
BOOL CDxSndPlayer::Init(const HWND hWnd, const DWORD dwCooperativeLevel)
{
	// DirectSound�� �����Ѵ�. 
	DirectSoundCreate8(NULL, &m_pDirectSound, NULL);

	if (m_pDirectSound) {
		// DirectSound�� ���������� �����Ѵ�. 
		m_pDirectSound->SetCooperativeLevel(hWnd, dwCooperativeLevel);
		return TRUE;
	}
	return FALSE;
}

BOOL CDxSndPlayer::Init(IDirectSound8* pDirectSound)
{
	if( !pDirectSound)
		return FALSE;
	
	m_pDirectSound = pDirectSound;
	m_bBringDirectSoundDevice = TRUE;
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: CreateSoundBuffer(const int nBufferSize, const int nSamplePerSec, 
//						   const int nBitPerSample, const int nChannel)
// Code: sagolboss (2004-06-17)
// Desc: DirectSound�� SoundBuffer�� �����Ѵ�
//		nBufferSize   - ������ Buffer�� Size
//		nSamplePerSec - ������ Buffer�� SamplePerSec(SampleRate)
//		nBitPerSample - ������ Buffer�� BitPerSEc(BitRate)
//		nChannel	  - ������ Buffer�� Channel
//-----------------------------------------------------------------------------
int CDxSndPlayer::CreateSoundBuffer(const int nBufferSize, const int nSamplePerSec, const int nBitPerSample, const int nChannel)
{
	DSBUFFERDESC  desc;
	PCMWAVEFORMAT pcmFormat;

	for(int nBufferIndex = 1; nBufferIndex < sizeof(m_SndBuffer)/sizeof(*m_SndBuffer); ++nBufferIndex)
	{
		// ������� Sound Buffer�� ���ٸ� ���� Sound Buffer�� �����Ѵ�. 
		if( !m_SndBuffer[nBufferIndex].pSndBuffer)
		{
			ZeroMemory(&desc, sizeof(desc));
			ZeroMemory(&pcmFormat, sizeof(pcmFormat));

			pcmFormat.wf.wFormatTag		 = WAVE_FORMAT_PCM;
			pcmFormat.wf.nChannels		 = nChannel;
			pcmFormat.wf.nSamplesPerSec	 = nSamplePerSec;
			pcmFormat.wf.nBlockAlign	 = (nChannel * nBitPerSample) / 8; 
			pcmFormat.wf.nAvgBytesPerSec = nSamplePerSec * pcmFormat.wf.nBlockAlign;
			pcmFormat.wBitsPerSample	 = nBitPerSample;

			desc.dwSize		   = sizeof(DSBUFFERDESC);
			desc.dwFlags	   = DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2; 
			desc.dwBufferBytes = nBufferSize;
			desc.lpwfxFormat   = reinterpret_cast<LPWAVEFORMATEX>(&pcmFormat);

			// DirectSound�� SoundBuffer�� �����Ѵ�.
			HRESULT hr = m_pDirectSound->CreateSoundBuffer(&desc, &m_SndBuffer[nBufferIndex].pSndBuffer, NULL);
			assert( SUCCEEDED(hr));
			if( SUCCEEDED(hr)) /// hr���� DS_OK �̸� TRUE
			{
				// Sound Buffer ������ �����ߴٸ� Index�� return �Ͽ� �ش�. 
				return nBufferIndex;
			}
			else
			{
				// �����ߴٸ� NULL���� �־��ְ� 0�� return �Ͽ��ش� (0�� ����)
				m_SndBuffer[nBufferIndex].pSndBuffer = NULL;
				return DXSND_ERROR_CREATESOUNDBUFFER;
			}
		}
	}
	/// Sound Buffer ���̻� ����� ���ٸ�(�� ã�ٸ�) 0�� return �Ͽ��ش� (0�� ����) 
	/// _DEBUG ��� ASSERT�� ����Ų�� 
	assert( nBufferIndex < sizeof(m_SndBuffer)/sizeof(*m_SndBuffer));
	return DXSND_ERROR_CREATESOUNDBUFFER;
}

void CDxSndPlayer::ReleaseSoundBuffer(const int nSndBufferIndex)
{
	if( IsSoundBuffer(nSndBufferIndex))
	{
		m_SndBuffer[nSndBufferIndex].pSndBuffer->Release();
		m_SndBuffer[nSndBufferIndex].pSndBuffer = NULL;
	}
}

BOOL CDxSndPlayer::Play(const int nSndBufferIndex, BOOL bLoop)
{
	if( IsSoundBuffer(nSndBufferIndex))
	{
		m_SndBuffer[nSndBufferIndex].pSndBuffer->SetCurrentPosition(0);
		m_SndBuffer[nSndBufferIndex].pSndBuffer->Play(0, 0, bLoop ? DSBPLAY_LOOPING : 0);
		return TRUE;
	}
	return FALSE;
}

BOOL CDxSndPlayer::Pause(const int nSndBufferIndex)
{
	if( IsSoundBuffer(nSndBufferIndex))
	{
		m_SndBuffer[nSndBufferIndex].pSndBuffer->Stop();
		return TRUE;
	}
	return FALSE;
}

void CDxSndPlayer::Close(const int nSndBufferIndex)
{
	ReleaseSoundBuffer(nSndBufferIndex);
}

BOOL CDxSndPlayer::IsSoundBuffer(const int nSndBufferIndex) const 
{
	if( nSndBufferIndex != 0 && m_SndBuffer[nSndBufferIndex].pSndBuffer)
		return TRUE;
	return FALSE;
}

int CDxSndPlayer::GetCurrentPos(const int nSndBufferIndex) const
{	
	unsigned long ulStatus = 0;
	unsigned long ulCurPos[2];

	if( IsSoundBuffer(nSndBufferIndex))
	{
		m_SndBuffer[nSndBufferIndex].pSndBuffer->GetStatus(&ulStatus);
	
		ulStatus &= DSBSTATUS_PLAYING | DSBSTATUS_BUFFERLOST;
		if( ulStatus == DSBSTATUS_PLAYING)
		{
			m_SndBuffer[nSndBufferIndex].pSndBuffer->GetCurrentPosition(&ulCurPos[0], &ulCurPos[1]);
			return ulCurPos[0];
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// Name: Lock(const int nSndBufferIndex, const int nLockPos, const int nLockSize)
// Code: sagolboss (2004-06-17)
// Desc: Sound Buffer�� ����ϱ� ���ؼ� Lock�� �ɾ��־� Sound Buffer�� ptr��
//		 �����;� �ϸ� ����� ���ߴٸ� UnLock�� �����־�� �Ѵ�.
//		nSndBufferIndex - Sound Buffer�� Index
//		nLockPos		- Sound Buffer�� ��𼭺��� Lock �ɰ������� ���� offset
//		nLockSize		- Sound Buffer�� ������ Lock �ɰ������� ���Ѱ�
//		return	        - Lock�� �� Sound Buffer�� ptr
//-----------------------------------------------------------------------------
void* CDxSndPlayer::Lock(const int nSndBufferIndex, const int nLockPos, const int nLockSize)
{
	void* pOutData1 = NULL;
	void* pOutData2 = NULL; // ĸ�� ������ 2 ��°�� Lock �� �κ��� �����͸� �޴� ������ �ּ�
							// ���⿡�� �ʿ���� ��
	unsigned long ulLockSize1 = 0;
	unsigned long ulLockSize2 = 0; 

	if( IsSoundBuffer(nSndBufferIndex))
	{
		// DirectSound�� Lock�� �Ǵ�
		m_SndBuffer[nSndBufferIndex].pSndBuffer->Lock(nLockPos, nLockSize, 
													  &pOutData1, &ulLockSize1,
													  &pOutData2, &ulLockSize2, 0);
		
		_ASSERT(ulLockSize2 == 0 && pOutData2 == 0); 
		_ASSERT(ulLockSize1 == static_cast<unsigned long>(nLockSize));

		// ������ ����ϴ� SoundBuffer���� ������ �ִ´�. 
		m_SndBuffer[nSndBufferIndex].pLockData      = pOutData1;
		m_SndBuffer[nSndBufferIndex].ulLockDataSize = ulLockSize1;
		return m_SndBuffer[nSndBufferIndex].pLockData;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Name: UnLock(const int nSndBufferIndex)
// Code: sagolboss (2004-06-17)
// Desc: Sound Buffer�� ����ϱ� ���ؼ� Lock�� �ɾ��־� Sound Buffer�� ptr��
//		 �����;� �ϸ� ����� ���ߴٸ� UnLock�� �����־�� �Ѵ�.
//		nSndBufferIndex - Sound Buffer�� Index
//-----------------------------------------------------------------------------
void CDxSndPlayer::UnLock(const int nSndBufferIndex)
{
	if( IsSoundBuffer(nSndBufferIndex))
	{
		m_SndBuffer[nSndBufferIndex].pSndBuffer->Unlock(m_SndBuffer[nSndBufferIndex].pLockData, 
														m_SndBuffer[nSndBufferIndex].ulLockDataSize,
														NULL, 0);
		m_SndBuffer[nSndBufferIndex].pLockData = NULL;
		m_SndBuffer[nSndBufferIndex].ulLockDataSize = 0;
	}
}


void CDxSndPlayer::SetVolume(const long lVolume)
{
	for(int i = 0; i < SOUNDBUFFER_MAX; ++i)
	{
		if( m_SndBuffer[i].pSndBuffer)
			m_SndBuffer[i].pSndBuffer->SetVolume(lVolume);
	}
}