// SoundPlayer.h: interface for the CSoundPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDPLAYER_H__6C1A18E3_973A_43B7_85EF_19F0A04ED29B__INCLUDED_)
#define AFX_SOUNDPLAYER_H__6C1A18E3_973A_43B7_85EF_19F0A04ED29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const short CS_MAX = 3;
class CS_OBJECTS
{
public:
	CRITICAL_SECTION sCriticalSection[CS_MAX];
	
	CS_OBJECTS(){
		for(int i = 0; i < CS_MAX; ++i)
			InitializeCriticalSection(&sCriticalSection[i]);
	}
	~CS_OBJECTS(){
		for(int i = 0; i < CS_MAX; ++i)
			DeleteCriticalSection(&sCriticalSection[i]);
	}
};

class ISoundData;
class CWav;
class IPlayer;
class CSoundPlayer  
{
public:
	CSoundPlayer(){};
	CSoundPlayer(IPlayer* pPlayer);
	virtual ~CSoundPlayer();

	virtual BOOL Open(const char* szFileName);
	virtual void Play(BOOL bLoop = FALSE);
	virtual void Pause() const;
	virtual void Close() const;

	void SetVolume(const long lVolume);
	int  GetTick() const;
	
protected:
	void WriteWavDataToPlayerSoundBuffer(const int nPos, const int nSize, ISoundData* const pSoundData) const;

private:
	BOOL OpenSoundData(ISoundData* pSoundData, const char* szFileName);

public:
#ifdef _MT
	static CS_OBJECTS m_CSObjects;
#endif
protected:
	int		 m_nSndHandle;
	int		 m_nBytePerSec;
	long     m_lVolume;
	IPlayer* m_pPlayer;
	
//-----------------------------------------------------------------------------
// Code: sagolboss (2004-06-17)
// Desc: 쓰레드를 위해서 마련해둔 코드들 
//		 쓰레드 프로시저가 static이기 때문에 함수나 변수를 static 으로 선언한다
//-----------------------------------------------------------------------------
#ifdef _MT
protected:
	static void LockThread(const int nIndex);
	static void UnLockThread(const int nIndex);
#endif
};

#endif // !defined(AFX_SOUNDPLAYER_H__6C1A18E3_973A_43B7_85EF_19F0A04ED29B__INCLUDED_)
