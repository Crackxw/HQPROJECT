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
// Desc: DirectSound를 생성하고 협조레벨을 설정한다
//		hWnd			   - DirectSound를 생성할 HWND
//		dwCooperativeLevel - 협조레벨 값
//-----------------------------------------------------------------------------
BOOL CDxSndPlayer::Init(const HWND hWnd, const DWORD dwCooperativeLevel)
{
	// DirectSound를 생성한다. 
	DirectSoundCreate8(NULL, &m_pDirectSound, NULL);

	if (m_pDirectSound) {
		// DirectSound의 협조레벨을 설정한다. 
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
// Desc: DirectSound에 SoundBuffer를 생성한다
//		nBufferSize   - 생성할 Buffer의 Size
//		nSamplePerSec - 생성할 Buffer의 SamplePerSec(SampleRate)
//		nBitPerSample - 생성할 Buffer의 BitPerSEc(BitRate)
//		nChannel	  - 생성할 Buffer의 Channel
//-----------------------------------------------------------------------------
int CDxSndPlayer::CreateSoundBuffer(const int nBufferSize, const int nSamplePerSec, const int nBitPerSample, const int nChannel)
{
	DSBUFFERDESC  desc;
	PCMWAVEFORMAT pcmFormat;

	for(int nBufferIndex = 1; nBufferIndex < sizeof(m_SndBuffer)/sizeof(*m_SndBuffer); ++nBufferIndex)
	{
		// 만들어진 Sound Buffer가 없다면 새로 Sound Buffer를 생성한다. 
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

			// DirectSound에 SoundBuffer를 생성한다.
			HRESULT hr = m_pDirectSound->CreateSoundBuffer(&desc, &m_SndBuffer[nBufferIndex].pSndBuffer, NULL);
			assert( SUCCEEDED(hr));
			if( SUCCEEDED(hr)) /// hr값이 DS_OK 이면 TRUE
			{
				// Sound Buffer 생성이 성공했다면 Index를 return 하여 준다. 
				return nBufferIndex;
			}
			else
			{
				// 실패했다면 NULL값을 넣어주고 0을 return 하여준다 (0은 실패)
				m_SndBuffer[nBufferIndex].pSndBuffer = NULL;
				return DXSND_ERROR_CREATESOUNDBUFFER;
			}
		}
	}
	/// Sound Buffer 더이상 만들수 없다면(꽉 찾다면) 0을 return 하여준다 (0은 실패) 
	/// _DEBUG 라면 ASSERT를 일으킨다 
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
// Desc: Sound Buffer를 사용하기 위해선 Lock을 걸어주어 Sound Buffer의 ptr를
//		 가져와야 하며 사용을 다했다면 UnLock을 시켜주어야 한다.
//		nSndBufferIndex - Sound Buffer의 Index
//		nLockPos		- Sound Buffer에 어디서부터 Lock 걸것인지에 대한 offset
//		nLockSize		- Sound Buffer에 어디까지 Lock 걸것인지에 대한값
//		return	        - Lock을 건 Sound Buffer의 ptr
//-----------------------------------------------------------------------------
void* CDxSndPlayer::Lock(const int nSndBufferIndex, const int nLockPos, const int nLockSize)
{
	void* pOutData1 = NULL;
	void* pOutData2 = NULL; // 캡춰 버퍼의 2 번째에 Lock 된 부분의 포인터를 받는 변수의 주소
							// 여기에선 필요없는 값
	unsigned long ulLockSize1 = 0;
	unsigned long ulLockSize2 = 0; 

	if( IsSoundBuffer(nSndBufferIndex))
	{
		// DirectSound에 Lock을 건다
		m_SndBuffer[nSndBufferIndex].pSndBuffer->Lock(nLockPos, nLockSize, 
													  &pOutData1, &ulLockSize1,
													  &pOutData2, &ulLockSize2, 0);
		
		_ASSERT(ulLockSize2 == 0 && pOutData2 == 0); 
		_ASSERT(ulLockSize1 == static_cast<unsigned long>(nLockSize));

		// 실제로 사용하는 SoundBuffer만을 가지고 있는다. 
		m_SndBuffer[nSndBufferIndex].pLockData      = pOutData1;
		m_SndBuffer[nSndBufferIndex].ulLockDataSize = ulLockSize1;
		return m_SndBuffer[nSndBufferIndex].pLockData;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Name: UnLock(const int nSndBufferIndex)
// Code: sagolboss (2004-06-17)
// Desc: Sound Buffer를 사용하기 위해선 Lock을 걸어주어 Sound Buffer의 ptr를
//		 가져와야 하며 사용을 다했다면 UnLock을 시켜주어야 한다.
//		nSndBufferIndex - Sound Buffer의 Index
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