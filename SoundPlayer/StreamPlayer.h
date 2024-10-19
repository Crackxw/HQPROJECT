// StreamPlayer.h: interface for the CStreamPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMPLAYER_H__7E65A50A_1CE2_4EB6_A32F_696AA0921267__INCLUDED_)
#define AFX_STREAMPLAYER_H__7E65A50A_1CE2_4EB6_A32F_696AA0921267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const short SOUNDPLAY_STATE_LOOPING = -1;
const short SOUNDPLAY_STATE_STOP    = 0;
const short SOUNDPLAY_STATE_ONEPLAY = 1;
const short SOUNDPLAY_STATE_PAUSE   = 2;

const short MAX_SOUND = 10;

class ISoundData;
class IPlayer;
class CProfile;
class CStreamPlayer : public CSoundPlayer
{
public:
	CStreamPlayer(IPlayer* pPlayer, const int nBufferSize);
	virtual ~CStreamPlayer();

	BOOL Open(const char* szFileName);
	BOOL Open(ISoundData* pSoundData);
	void Play(const BOOL bLoop = FALSE);
	void Pause();
	void Close();
	int  Process();
	inline int GetPlayState() { return m_nPlayState; }
	inline ISoundData* GetSoundData() { return m_pSoundData; }

	static void AllProcess();
	
private:
	void AddProcess();
	void RemoveProcess();

	void ReadStream(const int nReadDataSize);
	BOOL OpenStreamData(ISoundData* pSoundData, const char* szFileName);
	void ContainStreamInfo(ISoundData* pSoundData);

#ifdef _MT
	void CreateThreadEvent();
	void ReleaseThreadEvent();
	static void StreamThreadProc(void*);
#endif

private:
	ISoundData*  m_pSoundData; // Stream을 위해선 Wav Data가 있어야 한다.
	int			 m_nDataSize;
	int			 m_nBufferSize;
	int			 m_nWritePos;
	int		     m_nStartPos;
	int			 m_nPlayPos;  // Play될 Sound의 위치 
	int			 m_nPivotPos; // Buffer를 사용한 크기를 저장해둔다. 
	int 		 m_nPlayState;

	static HANDLE		  m_hProcessStreamProc;
	static CStreamPlayer* m_ListStreamPlayer[MAX_SOUND];
	static int			  m_nStreamPlayerCount;
	static BOOL			  m_bThreadDelete;
};

#endif // !defined(AFX_STREAMPLAYER_H__7E65A50A_1CE2_4EB6_A32F_696AA0921267__INCLUDED_)
