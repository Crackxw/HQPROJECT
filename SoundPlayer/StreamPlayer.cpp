// StreamPlayer.cpp: implementation of the CStreamPlayer class.
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
#include "Wav.h"
#include "..\..\GSLib\OggVorbis\vorbisfile.h"

#include "MyOgg.h"
#include "MyGBM.h"
#include "SoundPlayer.h"
#include "StreamPlayer.h"

CStreamPlayer* CStreamPlayer::m_ListStreamPlayer[MAX_SOUND];
int            CStreamPlayer::m_nStreamPlayerCount = 0;
BOOL		   CStreamPlayer::m_bThreadDelete	   = FALSE;
HANDLE		   CStreamPlayer::m_hProcessStreamProc = NULL;

const int MIN_SKIP_BYTES   = 1024;
const int STREAM_SOUND_GAP = 256;

#define SAFE_DELETE(p) { if(p) { delete (p);     (p)=NULL; } }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStreamPlayer::CStreamPlayer(IPlayer* pPlayer, const int nBufferSize) : m_nWritePos(0),				
																        m_nStartPos(0),
																	    m_nPlayPos(0),
																	    m_nPivotPos(0),
																	    m_nPlayState(0),
																		m_pSoundData(NULL),
																	    m_nBufferSize(nBufferSize),	
																	    CSoundPlayer(pPlayer)
																
{
	assert(nBufferSize > 0);

#ifdef _MT
	m_hProcessStreamProc = CreateEvent(NULL, TRUE, TRUE, NULL);
	ResetEvent(m_hProcessStreamProc);
	
	_beginthread(StreamThreadProc, 0, 0);
	OutputDebugString("=========== StreamThread Proc Start! =========== \n");
#endif
}

CStreamPlayer::~CStreamPlayer()
{
	CSoundPlayer::LockThread(1);
	m_bThreadDelete = TRUE;
	SetEvent(m_hProcessStreamProc);
	CSoundPlayer::UnLockThread(1);
}

//-----------------------------------------------------------------------------
// Name: Open(const char* szFileName)
// Code: sagolboss (2004-06-22)
// Desc: Wav클래스로 먼저 로드 하여 보고 실패한다면 Ogg 클래스로 로드한다. 
//		szFileName - Open할 파일명
//-----------------------------------------------------------------------------
BOOL CStreamPlayer::Open(const char* szFileName)
{
	m_pSoundData = new CMyOgg;
	if( OpenStreamData(m_pSoundData, szFileName))
		return TRUE;
	else
		SAFE_DELETE(m_pSoundData);
	
	m_pSoundData = new CMyGBM;
	if( OpenStreamData(m_pSoundData, szFileName))
		return TRUE;
	else
		SAFE_DELETE(m_pSoundData);

	m_pSoundData = new CWav;
	if( OpenStreamData(m_pSoundData, szFileName))
		return TRUE;
	else
		SAFE_DELETE(m_pSoundData);
	return FALSE;
}

BOOL CStreamPlayer::Open(ISoundData* pSoundData)
{
	assert(pSoundData);

	ContainStreamInfo(pSoundData);
	if( CSoundPlayer::m_nSndHandle == 0)
		return FALSE;
	
	m_pSoundData = pSoundData;
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: OpenStreamData(ISoundData* pSoundData, const char* szFileName)
// Code: sagolboss (2004-06-22)
// Desc: ISoundData 인터페이스 클래스를 통해 해당되는 클래스의 Open 함수를 호출
//       한다. 성공했다면 몇 가지 사운드 정보를 변수에 넣어준다
//		ISoundData* pSoundData - Sound Data Class 
//		szFileName			   - Open할 파일명
//-----------------------------------------------------------------------------
BOOL CStreamPlayer::OpenStreamData(ISoundData* pSoundData, const char* szFileName)
{
	if( pSoundData->Open(szFileName) == TRUE)
	{
		
		ContainStreamInfo(pSoundData);
		if( CSoundPlayer::m_nSndHandle !=0)
			return TRUE;
	}
	return FALSE;
}

void CStreamPlayer::ContainStreamInfo(ISoundData* pSoundData)
{
	m_nDataSize					= pSoundData->GetDataSize();
	CSoundPlayer::m_nBytePerSec = pSoundData->GetBytePerSecond();
	CSoundPlayer::m_nSndHandle  = CSoundPlayer::m_pPlayer->CreateSoundBuffer(
											m_nBufferSize, 
											pSoundData->GetSoundHeader()->unSamplesPerSec, 
											pSoundData->GetSoundHeader()->unBitsPerSample, 
											pSoundData->GetSoundHeader()->unChannels );
}

//-----------------------------------------------------------------------------
// Name: ReadStream(const int nReadDataSize)
// Code: sagolboss (2004-06-22)
// Desc: 원하는 길이만큼의 Sound Data를 읽는다. Looping을 해야된다면 Looping을 
//       하고 모두다 읽었다면 묵음처리를 한다 (여기서 바로 음악을 종료 시키는게 
//		 아니라 얼마간 묵음처리를 하고 난뒤에 음악을 종료시킨다. 이유는 Sound 
//       Data의 끝을 알아내어 바로 종료하기가 더 어렵기 때문이다)
//		const int nReadDataSize - 읽을 Data 사이즈  
//-----------------------------------------------------------------------------
void CStreamPlayer::ReadStream(const int nReadDataSize)
{
	int nOffset		   = 0;
	int nNextDataSize  = 0;

	assert(m_pSoundData);

	// 현재 읽어들일 위치가 Sound Data 크기 보다 크다면 
	if( (m_nWritePos - m_nStartPos) + nReadDataSize > m_nDataSize)
	{
		// 현재 읽어들일 위치에서 남은 Data의 크기를 구한다.
		nNextDataSize = m_nDataSize - (m_nWritePos - m_nStartPos);

		// 재귀호출을 이용하여 남은 Data를 읽어들인다. 
		ReadStream(nNextDataSize);

		// Looping을 하여야 한다면 Rewind 시킨다 
		if( m_nPlayState == SOUNDPLAY_STATE_LOOPING )
			m_pSoundData->Rewind();


		// 시작점을 Data 크기 만큼 한다. 이 if문으로 안들어오게 하기 위함이다. 
		m_nStartPos += m_nDataSize;
		
		// 마지막 남은 Data를 읽어들인다. 
		if( nReadDataSize - nNextDataSize > 0)
			ReadStream( (nReadDataSize-nNextDataSize) & (~7));
		return ;
	}

	// 현재 버퍼가 넘쳤는지 계산한다. 
	nOffset = m_nWritePos % m_nBufferSize;

	// 버퍼가 넘쳤다면 
	if( nOffset + nReadDataSize > m_nBufferSize)
	{
		// 버퍼가 남은 양만큼 데이터를 읽어들인다.
		nNextDataSize = m_nBufferSize - nOffset;
		ReadStream(nNextDataSize);
		// 새로운 버퍼에 쓰기 시작한다. 
		ReadStream(nReadDataSize-nNextDataSize);
	}
	else
	{
		// Looping을 하지 않고 Play 가 끝났다면 일단 묵음처리를 한다. 
		if( m_nPlayState == SOUNDPLAY_STATE_ONEPLAY && m_nWritePos >= m_nDataSize)
			CSoundPlayer::WriteWavDataToPlayerSoundBuffer(nOffset, nReadDataSize, NULL);
		else // SoundBuffer에 Sound Data를 쓴다. 
			CSoundPlayer::WriteWavDataToPlayerSoundBuffer(nOffset, nReadDataSize, m_pSoundData);

		// 읽어들인 Sound Data 위치를 갱신힌다. 
		m_nWritePos += nReadDataSize;
	}
}

void CStreamPlayer::Play(const BOOL bLoop)
{
	if( m_nPlayState)
		Close();

	m_nWritePos  = 0;
	m_nStartPos  = 0;
	m_nPlayPos   = 0;
	m_nPivotPos  = 0;
	m_nPlayState = bLoop == FALSE ? SOUNDPLAY_STATE_ONEPLAY : SOUNDPLAY_STATE_LOOPING;
	
	assert(m_pSoundData);
	m_pSoundData->Rewind();

	ReadStream(m_nBufferSize);

#ifdef _MT
	CSoundPlayer::LockThread(2);
#endif

	CSoundPlayer::SetVolume(CSoundPlayer::m_lVolume);
	CSoundPlayer::m_pPlayer->Play(CSoundPlayer::m_nSndHandle, TRUE);

#ifdef _MT
	CSoundPlayer::UnLockThread(2);
#endif

	AddProcess();
}

void CStreamPlayer::Pause()
{
	m_nPlayState = SOUNDPLAY_STATE_PAUSE;

	RemoveProcess();
	CSoundPlayer::m_pPlayer->Pause(m_nSndHandle);
}

void CStreamPlayer::Close()
{
	m_nPlayState = SOUNDPLAY_STATE_STOP;

	RemoveProcess();
	CSoundPlayer::m_pPlayer->Close(m_nSndHandle);

	SAFE_DELETE( m_pSoundData);
}

//-----------------------------------------------------------------------------
// Name: Process()
// Code: sagolboss (2004-06-22)
// Desc: 스트림을 위한것들을 처리하는 함수(스트림을 하고자 한다면 이 함수를
//       쓰레드나 Timer를 이용하여 계속 불러 주어야 한다)
//-----------------------------------------------------------------------------
int CStreamPlayer::Process()
{
	int nCurrPos = 0;
	int nLen     = 0;

	// 현재 Play중인 버퍼의 위치를 구한다. 
	nCurrPos = CSoundPlayer::m_pPlayer->GetCurrentPos(CSoundPlayer::m_nSndHandle);
	
	if( nCurrPos == -1)
		return 1;
	
	// 현재 Play중인 사운드의 위치보다 [사용한버퍼 + Play될 버퍼의위치] 보다 작다면
	// 사용한 버퍼값을 늘여준다. 왜냐 버퍼를 더 사용햇기 때문이다. 
	if( m_nPivotPos + nCurrPos < m_nPlayPos)
		m_nPivotPos += m_nBufferSize;

	// 현재 Play될 사운드의 위치를 갱신하여 준다 
	m_nPlayPos = m_nPivotPos + nCurrPos;
	
	// 사용한 Buffer의 양이 사운드 크기 보다 크고 Looping을 하지 않는다면 멈춘다. 
	if( m_nPlayState == SOUNDPLAY_STATE_ONEPLAY && m_nPivotPos >= m_nDataSize)
	{
		CSoundPlayer::m_pPlayer->Close(m_nSndHandle);
		return 1;
	}
	
	// 실제 읽어들일 값을 구한다. 
	nLen = m_nWritePos - m_nPlayPos;
	if( m_nBufferSize - nLen > MIN_SKIP_BYTES)
	{
		// 한번에 너무 많은 Stream을 읽게 되면 소리가 끊길수 있다
		// 그렇기 때문에 버퍼 사이즈 보다 작은 크기들을 읽어들인다. 
		ReadStream(m_nBufferSize - nLen - STREAM_SOUND_GAP); 
	}			
	return 0;
}

#ifdef _MT
void CStreamPlayer::StreamThreadProc(void*)
{
	while(!m_bThreadDelete)
	{
		WaitForSingleObject(m_hProcessStreamProc, INFINITE);
		AllProcess();
		Sleep(20); // CPU 점유율이 100이 안되게 하기위해서 잠깐 재어둔다.
	}

	CSoundPlayer::LockThread(1);
	CloseHandle(m_hProcessStreamProc);
	CSoundPlayer::UnLockThread(1);

	OutputDebugString("=========== StreamThread Proc End! =========== \n");
}
#endif

void CStreamPlayer::AllProcess()
{
#ifdef _MT	
	CSoundPlayer::LockThread(1);
#endif

	for(int i=0; i < m_nStreamPlayerCount; ++i) 
	{
		if( m_ListStreamPlayer[i]->Process() == 1)
		{
			m_ListStreamPlayer[i] = m_ListStreamPlayer[m_nStreamPlayerCount-1];
			m_nStreamPlayerCount -= 1;
			i -= 1;
		}
	}

#ifdef _MT
	CSoundPlayer::UnLockThread(1);
#endif

}

void CStreamPlayer::AddProcess()
{
#ifdef _MT
	CSoundPlayer::LockThread(1);
#endif
	if( m_nStreamPlayerCount >= MAX_SOUND)
	{
		assert(m_nStreamPlayerCount < MAX_SOUND);
		return ;
	}
	m_ListStreamPlayer[m_nStreamPlayerCount++] = this;
	if( m_nStreamPlayerCount > 0)
		SetEvent(m_hProcessStreamProc);

#ifdef _MT
	CSoundPlayer::UnLockThread(1);
#endif
}

void CStreamPlayer::RemoveProcess()
{
#ifdef _MT
	CSoundPlayer::LockThread(1);
#endif
	for(int i=0; i < m_nStreamPlayerCount; ++i) 
	{
		if( m_ListStreamPlayer[i] == this) 
		{
			m_ListStreamPlayer[i] = m_ListStreamPlayer[m_nStreamPlayerCount - 1];
			m_nStreamPlayerCount--;
			assert(m_nStreamPlayerCount >= 0);
		}
	}
	if( m_nStreamPlayerCount <= 0)
		ResetEvent(m_hProcessStreamProc);

#ifdef _MT
	CSoundPlayer::UnLockThread(1);
#endif	
}

