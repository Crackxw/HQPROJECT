// BgmMgr.cpp: implementation of the CBgmMgr class.
//
//////////////////////////////////////////////////////////////////////

#include <GSL.h>
#include <windows.h>
#include <stdio.h>
#include <dsound.h>
#include <crtdbg.h>
#include <assert.h>
#include <string>
#include <list>
#include <queue>
#include <process.h>
#include <MyCriticalSection.h>

#include "..\SoundPlayer\SoundPlayer_Common.h"
#include "..\SoundPlayer\iSoundData.h"
#include "..\..\GSLib\OggVorbis\vorbisfile.h"
#include "..\SoundPlayer\Wav.h"
#include "..\SoundPlayer\MyOgg.h"
#include "..\SoundPlayer\MyGBM.h"
#include "..\SoundPlayer\IPlayer.h"
#include "..\SoundPlayer\DxSndPlayer.h"
#include "..\SoundPlayer\StaticSound.h"
#include "..\SoundPlayer\StreamSound.h"

#include <Singleton.h>
#include "BgmMgr.h"

const int CRITICAL_SECTION_MAX = 3;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBgmMgr::CBgmMgr(): m_pDxSndPlayer(NULL),
  			        m_pStreamSound(NULL),
					m_nPrevTick(0),
					m_bContainSoundData(TRUE),
					m_nNextPlayBgmIndex(OGGBGM_FILENAME_INVALID)
{
	CMyCriticalSection::InitializeCriticalSection(CRITICAL_SECTION_MAX);

	m_BgmInfoList.push_back( BGM_INFO( "music\\Et_Login.GBM",		       OGGBGM_FILENAME_GERSANGTITLE));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Et_Login_Japan.GBM",		   m_BgmInfoList.size()));

	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_ChosunField.GBM",		   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_JapanField.GBM",		   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_ChinaField.GBM",         m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_TaiwanField.GBM",		   m_BgmInfoList.size()));

	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_ChosunVillage.GBM",	   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_ChinaVillage.GBM",	   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_JapanVillage.GBM",	   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Fl_TaiwanVillage.GBM",	   m_BgmInfoList.size()));
	
	m_BgmInfoList.push_back( BGM_INFO( "music\\Ba_Battle.GBM",			   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Et_MiniGame.GBM",		   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Ba_Win.GBM",			       m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Ba_Lose.GBM",			   m_BgmInfoList.size()));
	m_BgmInfoList.push_back( BGM_INFO( "music\\Dn_Nunonchon_Kyungmae.GBM", m_BgmInfoList.size()));

	// robypark 2004/12/20 16:12
	// 공성전 중에 들려지는 배경음악 추가
	m_BgmInfoList.push_back( BGM_INFO( "music\\SiegeBattleMusic.GBM",	   m_BgmInfoList.size()));

#ifdef _DEBUG
	m_nPlayCount = 0;
#endif
}

CBgmMgr::~CBgmMgr()
{
	Close();
	Release();
}

BOOL CBgmMgr::Init(const HWND hWnd, const int nBufferSize)
{
	m_pDxSndPlayer  = new CDxSndPlayer;
	if( !m_pDxSndPlayer->Init(hWnd))
	{
#ifdef _DEBUG
		MessageBox(hWnd, "It fails in CDxSndPlayer initialize", "Fail", MB_OK);
#endif
		return FALSE;
	}

	assert(m_pDxSndPlayer);
	m_pStreamSound = new CStreamSound(m_pDxSndPlayer, nBufferSize) ;
	// actdoll (2004/11/29 18:05) : 초기화
	m_iCurrentPlayingBgmIndex	= OGGBGM_FILENAME_INVALID;
	
	return TRUE;
}

BOOL CBgmMgr::Init(IDirectSound8* pDirectSound, const int nBufferSize)
{
	m_pDxSndPlayer  = new CDxSndPlayer;
	if( !m_pDxSndPlayer->Init(pDirectSound))
	{
#ifdef _DEBUG
//		MessageBox(NULL, "It fails in CDxSndPlayer initialize", "Fail", MB_OK);
#endif
		return FALSE;
	}
	m_pStreamSound = new CStreamSound(m_pDxSndPlayer, nBufferSize) ;
	assert(m_pStreamSound);	
	// actdoll (2004/11/29 18:05) : 초기화
	m_iCurrentPlayingBgmIndex	= OGGBGM_FILENAME_INVALID;

	return TRUE;
}

void CBgmMgr::Release()
{
	if( m_pStreamSound)
	{
		delete m_pStreamSound;
		m_pStreamSound = NULL;
	}
	if( m_pDxSndPlayer)
	{
		delete m_pDxSndPlayer;
		m_pDxSndPlayer = NULL;
	}
	m_strNextPlayBgmFileName.erase(m_strNextPlayBgmFileName.begin(), m_strNextPlayBgmFileName.end());
	m_BgmInfoList.clear();
	
	CMyCriticalSection::DeleteCriticalSection();

	// actdoll (2004/11/29 18:05) : 초기화
	m_iCurrentPlayingBgmIndex	= OGGBGM_FILENAME_INVALID;
}

BOOL CBgmMgr::PlayNextBgm(const BOOL bLoop)
{
	if( m_nNextPlayBgmIndex > OGGBGM_FILENAME_INVALID)
		return Play(m_nNextPlayBgmIndex, bLoop);
	else
		return Play(m_strNextPlayBgmFileName.c_str(), bLoop);

	return FALSE;
}


BOOL CBgmMgr::Play(int nBgmFileNameIndex, const BOOL bLoop)
{
	if( nBgmFileNameIndex <= OGGBGM_FILENAME_INVALID && nBgmFileNameIndex >= OGGBGM_FILENAME_MAX)
	{
#ifdef _DEBUG
//		MessageBox(NULL, "There is not a filename to the index which corresponds", "Fail", MB_OK);
#endif
		return FALSE;
	}

	// actdoll (2004/11/29 18:03) : 인덱스가 동일한 사운드를 재생하고 있는 경우 그냥 계속 연주시킨다.
	if( m_pStreamSound->GetPlayState() != SOUNDPLAY_STATE_STOP && m_iCurrentPlayingBgmIndex == nBgmFileNameIndex )	return TRUE;

	if( Play(m_BgmInfoList[nBgmFileNameIndex].strFileName.c_str(), bLoop) )
	{
		m_iCurrentPlayingBgmIndex	= nBgmFileNameIndex;
		return TRUE;
	}

	// 여기까지 오면 연주 실패다.
	m_iCurrentPlayingBgmIndex	= OGGBGM_FILENAME_INVALID;
	return FALSE;
}

BOOL CBgmMgr::Play(const char* szFileName, const BOOL bLoop)
{
	assert(m_pStreamSound);
	 
	if( m_pStreamSound->GetPlayState() != SOUNDPLAY_STATE_STOP)
		Close();

	if( !m_pStreamSound->Open(szFileName))
	{
#ifdef _DEBUG
		char szMsg[256];
		sprintf(szMsg, "It fails in sound buffer creation or is not file or file is the type it will not be able to play", szFileName);
//		MessageBox(NULL, szMsg, "Fail", MB_OK);
#endif
		return FALSE;
	}

	m_pStreamSound->Play(bLoop);

	return TRUE;
}

void CBgmMgr::Close()
{
	assert(m_pStreamSound);
	m_pStreamSound->Close();
	// actdoll (2004/11/29 18:05) : 초기화
	m_iCurrentPlayingBgmIndex	= OGGBGM_FILENAME_INVALID;
}

BOOL CBgmMgr::IsPlaying()
{
	assert(m_pStreamSound);
	
	if( m_pStreamSound && m_pStreamSound->GetPlayState() == SOUNDPLAY_STATE_STOP)
		return FALSE;
	return TRUE;
}

void CBgmMgr::SetVolume(long lVolume)
{
	/// BGM은 15%로 낮추어서 출력한다. (은일씨 요청사항)
//	lVolume = lVolume * 0.85f;
	assert(m_pStreamSound);

	if( m_pStreamSound)
		m_pStreamSound->SetVolume(lVolume);
}

void CBgmMgr::SetNextBgm(int nNextBgmIndex) 
{
	m_nNextPlayBgmIndex = nNextBgmIndex; 
	m_strNextPlayBgmFileName.erase(m_strNextPlayBgmFileName.begin(), m_strNextPlayBgmFileName.end());
}

void CBgmMgr::SetNextBgm(const char* szFileName)
{ 
	m_strNextPlayBgmFileName = szFileName; 
	m_nNextPlayBgmIndex      = OGGBGM_FILENAME_INVALID;
}
