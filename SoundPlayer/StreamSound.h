// StreamSound.h: interface for the CStreamSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMSOUND_H__2D5657D8_1491_4D50_9728_02BB15516F93__INCLUDED_)
#define AFX_STREAMSOUND_H__2D5657D8_1491_4D50_9728_02BB15516F93__INCLUDED_

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
class CStreamSound : public CStaticSound
{
public:
	CStreamSound(IPlayer* pPlayer, const int nBufferSize);
	virtual ~CStreamSound();

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
	ISoundData*  m_pSoundData; // Stream�� ���ؼ� Wav Data�� �־�� �Ѵ�.
	int			 m_nDataSize;
	int			 m_nBufferSize;
	int			 m_nWritePos;
	int		     m_nStartPos;
	int			 m_nPlayPos;  // Play�� Sound�� ��ġ 
	int			 m_nPivotPos; // Buffer�� ����� ũ�⸦ �����صд�. 
	int 		 m_nPlayState;

	static HANDLE		  m_hProcessStreamProc;
	static CStreamSound*  m_ListStreamSound[MAX_SOUND];
	static int			  m_nStreamSoundCount;
	static BOOL			  m_bThreadDelete;
};

#endif // !defined(AFX_STREAMSOUND_H__2D5657D8_1491_4D50_9728_02BB15516F93__INCLUDED_)
