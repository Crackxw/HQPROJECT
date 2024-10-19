// StreamSound.cpp: implementation of the CStreamSound class.
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
#include "StaticSound.h"
#include "StreamSound.h"

CStreamSound*  CStreamSound::m_ListStreamSound[MAX_SOUND];
int            CStreamSound::m_nStreamSoundCount  = 0;
BOOL		   CStreamSound::m_bThreadDelete	  = FALSE;
HANDLE		   CStreamSound::m_hProcessStreamProc = NULL;

const int MIN_SKIP_BYTES   = 1024;
const int STREAM_SOUND_GAP = 256;

#define SAFE_DELETE(p) { if(p) { delete (p);     (p)=NULL; } }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStreamSound::CStreamSound(IPlayer* pPlayer, const int nBufferSize) : m_nWritePos(0),				
																        m_nStartPos(0),
																	    m_nPlayPos(0),
																	    m_nPivotPos(0),
																	    m_nPlayState(0),
																		m_pSoundData(NULL),
																	    m_nBufferSize(nBufferSize),	
																	    CStaticSound(pPlayer)
																
{
	assert(nBufferSize > 0);

#ifdef _MT
	m_hProcessStreamProc = CreateEvent(NULL, TRUE, TRUE, NULL);
	ResetEvent(m_hProcessStreamProc);
	
	_beginthread(StreamThreadProc, 0, 0);
	OutputDebugString("=========== StreamThread Proc Start! =========== \n");
#endif
}

CStreamSound::~CStreamSound()
{
#ifdef _MT
	CStaticSound::LockThread(1);
#endif
	Close();
#ifdef _MT
	m_bThreadDelete = TRUE;
	SetEvent(m_hProcessStreamProc);
	CStaticSound::UnLockThread(1);
#endif
}

//-----------------------------------------------------------------------------
// Name: Open(const char* szFileName)
// Code: sagolboss (2004-06-22)
// Desc: WavŬ������ ���� �ε� �Ͽ� ���� �����Ѵٸ� Ogg Ŭ������ �ε��Ѵ�. 
//		szFileName - Open�� ���ϸ�
//-----------------------------------------------------------------------------
BOOL CStreamSound::Open(const char* szFileName)
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

BOOL CStreamSound::Open(ISoundData* pSoundData)
{
	assert(pSoundData);

	ContainStreamInfo(pSoundData);
	if( CStaticSound::m_nSndHandle == 0)
		return FALSE;
	
	m_pSoundData = pSoundData;
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: OpenStreamData(ISoundData* pSoundData, const char* szFileName)
// Code: sagolboss (2004-06-22)
// Desc: ISoundData �������̽� Ŭ������ ���� �ش�Ǵ� Ŭ������ Open �Լ��� ȣ��
//       �Ѵ�. �����ߴٸ� �� ���� ���� ������ ������ �־��ش�
//		ISoundData* pSoundData - Sound Data Class 
//		szFileName			   - Open�� ���ϸ�
//-----------------------------------------------------------------------------
BOOL CStreamSound::OpenStreamData(ISoundData* pSoundData, const char* szFileName)
{
	if( pSoundData->Open(szFileName) == TRUE)
	{
		ContainStreamInfo(pSoundData);
		if( CStaticSound::m_nSndHandle != 0)
			return TRUE;
	}
	return FALSE;
}

void CStreamSound::ContainStreamInfo(ISoundData* pSoundData)
{
	m_nDataSize					= pSoundData->GetDataSize();
	CStaticSound::m_nBytePerSec = pSoundData->GetBytePerSecond();
	CStaticSound::m_nSndHandle  = CStaticSound::m_pPlayer->CreateSoundBuffer(
											m_nBufferSize, 
											pSoundData->GetSoundHeader()->unSamplesPerSec, 
											pSoundData->GetSoundHeader()->unBitsPerSample, 
											pSoundData->GetSoundHeader()->unChannels );


}

//-----------------------------------------------------------------------------
// Name: ReadStream(const int nReadDataSize)
// Code: sagolboss (2004-06-22)
// Desc: ���ϴ� ���̸�ŭ�� Sound Data�� �д´�. Looping�� �ؾߵȴٸ� Looping�� 
//       �ϰ� ��δ� �о��ٸ� ����ó���� �Ѵ� (���⼭ �ٷ� ������ ���� ��Ű�°� 
//		 �ƴ϶� �󸶰� ����ó���� �ϰ� ���ڿ� ������ �����Ų��. ������ Sound 
//       Data�� ���� �˾Ƴ��� �ٷ� �����ϱⰡ �� ��Ʊ� �����̴�)
//		const int nReadDataSize - ���� Data ������  
//-----------------------------------------------------------------------------
void CStreamSound::ReadStream(const int nReadDataSize)
{
	int nOffset		   = 0;
	int nNextDataSize  = 0;

	assert(m_pSoundData);

	// ���� �о���� ��ġ�� Sound Data ũ�� ���� ũ�ٸ� 
	if( (m_nWritePos - m_nStartPos) + nReadDataSize > m_nDataSize)
	{
		// ���� �о���� ��ġ���� ���� Data�� ũ�⸦ ���Ѵ�.
		nNextDataSize = m_nDataSize - (m_nWritePos - m_nStartPos);
		
		if( nNextDataSize > 0)
		{
			// ���ȣ���� �̿��Ͽ� ���� Data�� �о���δ�. 
			ReadStream(nNextDataSize);
		}

		// Looping�� �Ͽ��� �Ѵٸ� Rewind ��Ų�� 
		if( m_nPlayState == SOUNDPLAY_STATE_LOOPING )
			m_pSoundData->Rewind();

		// �������� Data ũ�� ��ŭ �Ѵ�. �� if������ �ȵ����� �ϱ� �����̴�. 
		m_nStartPos += m_nDataSize;
		
		// ������ ���� Data�� �о���δ�. 
		if( nReadDataSize - nNextDataSize > 0)
			ReadStream( (nReadDataSize-nNextDataSize) & (~7));
		return ;
	}

	// ���� ���۰� ���ƴ��� ����Ѵ�. 
	nOffset = m_nWritePos % m_nBufferSize;

	// ���۰� ���ƴٸ� 
	if( nOffset + nReadDataSize > m_nBufferSize)
	{
		// ���۰� ���� �縸ŭ �����͸� �о���δ�.
		nNextDataSize = m_nBufferSize - nOffset;
		ReadStream(nNextDataSize);
		// ���ο� ���ۿ� ���� �����Ѵ�. 
		ReadStream(nReadDataSize-nNextDataSize);
	}
	else
	{
		// Looping�� ���� �ʰ� Play �� �����ٸ� �ϴ� ����ó���� �Ѵ�. 
		if( m_nPlayState == SOUNDPLAY_STATE_ONEPLAY && m_nWritePos >= m_nDataSize)
			CStaticSound::WriteWavDataToPlayerSoundBuffer(nOffset, nReadDataSize, NULL);
		else // SoundBuffer�� Sound Data�� ����. 
			CStaticSound::WriteWavDataToPlayerSoundBuffer(nOffset, nReadDataSize, m_pSoundData);

		// �о���� Sound Data ��ġ�� ��������. 
		m_nWritePos += nReadDataSize;
	}
}

void CStreamSound::Play(const BOOL bLoop)
{
	if( m_nPlayState == SOUNDPLAY_STATE_LOOPING && 
		m_nPlayState != SOUNDPLAY_STATE_ONEPLAY)
		Close();

	if( m_nPlayState != SOUNDPLAY_STATE_PAUSE)
	{
		m_nWritePos  = 0;
		m_nStartPos  = 0;
		m_nPlayPos   = 0;
		m_nPivotPos  = 0;
		m_nPlayState = bLoop == FALSE ? SOUNDPLAY_STATE_ONEPLAY : SOUNDPLAY_STATE_LOOPING;
		
		assert(m_pSoundData);
		m_pSoundData->Rewind();

		ReadStream(m_nBufferSize);
	}

#ifdef _MT
	CStaticSound::LockThread(2);
#endif

	CStaticSound::SetVolume(CStaticSound::m_lVolume);
	CStaticSound::m_pPlayer->Play(CStaticSound::m_nSndHandle, TRUE);

#ifdef _MT
	CStaticSound::UnLockThread(2);
#endif

	AddProcess();
}

void CStreamSound::Pause()
{
	m_nPlayState = SOUNDPLAY_STATE_PAUSE;

	RemoveProcess();
	CStaticSound::m_pPlayer->Pause(m_nSndHandle);
}

void CStreamSound::Close()
{
	m_nPlayState = SOUNDPLAY_STATE_STOP;

	RemoveProcess();
	CStaticSound::m_pPlayer->Close(m_nSndHandle);

	SAFE_DELETE( m_pSoundData);
}

//-----------------------------------------------------------------------------
// Name: Process()
// Code: sagolboss (2004-06-22)
// Desc: ��Ʈ���� ���Ѱ͵��� ó���ϴ� �Լ�(��Ʈ���� �ϰ��� �Ѵٸ� �� �Լ���
//       �����峪 Timer�� �̿��Ͽ� ��� �ҷ� �־�� �Ѵ�)
//-----------------------------------------------------------------------------
int CStreamSound::Process()
{
	int nCurrPos = 0;
	int nLen     = 0;

	// ���� Play���� ������ ��ġ�� ���Ѵ�. 
	nCurrPos = CStaticSound::m_pPlayer->GetCurrentPos(CStaticSound::m_nSndHandle);
	
	if( nCurrPos == -1)
		return 1;
	
	// ���� Play���� ������ ��ġ���� [����ѹ��� + Play�� ��������ġ] ���� �۴ٸ�
	// ����� ���۰��� �ÿ��ش�. �ֳ� ���۸� �� ����ޱ� �����̴�. 
	if( m_nPivotPos + nCurrPos < m_nPlayPos)
		m_nPivotPos += m_nBufferSize;

	// ���� Play�� ������ ��ġ�� �����Ͽ� �ش� 
	m_nPlayPos = m_nPivotPos + nCurrPos;
	
	// ����� Buffer�� ���� ���� ũ�� ���� ũ�� Looping�� ���� �ʴ´ٸ� �����. 
	if( m_nPlayState == SOUNDPLAY_STATE_ONEPLAY && m_nPivotPos >= m_nDataSize)
	{
		CStaticSound::m_pPlayer->Close(m_nSndHandle);
		return 1;
	}
	
	// ���� �о���� ���� ���Ѵ�. 
	nLen = m_nWritePos - m_nPlayPos;
	if( m_nBufferSize - nLen > MIN_SKIP_BYTES)
	{
		// �ѹ��� �ʹ� ���� Stream�� �а� �Ǹ� �Ҹ��� ����� �ִ�
		// �׷��� ������ ���� ������ ���� ���� ũ����� �о���δ�. 
		ReadStream(m_nBufferSize - nLen - STREAM_SOUND_GAP); 
	}			
	return 0;
}

#ifdef _MT
void CStreamSound::StreamThreadProc(void*)
{
	while(!m_bThreadDelete)
	{
		WaitForSingleObject(m_hProcessStreamProc, INFINITE);
		AllProcess();
		Sleep(20); // CPU �������� 100�� �ȵǰ� �ϱ����ؼ� ��� ���д�.
	}

	CStaticSound::LockThread(1);
	CloseHandle(m_hProcessStreamProc);
	CStaticSound::UnLockThread(1);

	OutputDebugString("=========== StreamThread Proc End! =========== \n");
}
#endif

void CStreamSound::AllProcess()
{
#ifdef _MT	
	CStaticSound::LockThread(1);
#endif

	for(int i=0; i < m_nStreamSoundCount; ++i) 
	{
		if( m_ListStreamSound[i]->Process() == 1)
		{
			m_ListStreamSound[i] = m_ListStreamSound[m_nStreamSoundCount-1];
			m_nStreamSoundCount -= 1;
			i -= 1;
		}
	}

#ifdef _MT
	CStaticSound::UnLockThread(1);
#endif

}

void CStreamSound::AddProcess()
{
#ifdef _MT
	CStaticSound::LockThread(1);
#endif
	if( m_nStreamSoundCount >= MAX_SOUND)
	{
		assert(m_nStreamSoundCount < MAX_SOUND);
		return ;
	}
	m_ListStreamSound[m_nStreamSoundCount++] = this;
	if( m_nStreamSoundCount > 0)
		SetEvent(m_hProcessStreamProc);

#ifdef _MT
	CStaticSound::UnLockThread(1);
#endif
}

void CStreamSound::RemoveProcess()
{
#ifdef _MT
	CStaticSound::LockThread(1);
#endif
	for(int i=0; i < m_nStreamSoundCount; ++i) 
	{
		if( m_ListStreamSound[i] == this) 
		{
			m_ListStreamSound[i] = m_ListStreamSound[m_nStreamSoundCount - 1];
			m_nStreamSoundCount--;
			assert(m_nStreamSoundCount >= 0);
		}
	}
	if( m_nStreamSoundCount <= 0)
		ResetEvent(m_hProcessStreamProc);

#ifdef _MT
	CStaticSound::UnLockThread(1);
#endif	
}


