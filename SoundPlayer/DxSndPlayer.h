// DxSndPlayer.h: interface for the CDxSndPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXSNDPLAYER_H__1D688ECF_DE05_4A46_B407_E5A77B92836A__INCLUDED_)
#define AFX_DXSNDPLAYER_H__1D688ECF_DE05_4A46_B407_E5A77B92836A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const short DXSND_ERROR_CREATESOUNDBUFFER = 0;
const short SOUNDBUFFER_MAX = 128;
class CDxSndPlayer : public IPlayer
{
public:
	CDxSndPlayer();
	virtual ~CDxSndPlayer();

	BOOL Init(const HWND hWnd, const DWORD dwCooperativeLevel=DSSCL_NORMAL);
	BOOL Init(IDirectSound8* pDirectSound);

public:
	int   CreateSoundBuffer(const int nBufferSize, const int nSamplePerSec, const int nBitPerSample, const int nChannel);
	
	BOOL  Play(const int nSndBufferIndex, const BOOL bLoop);
	BOOL  Pause(const int nSndBufferIndex);
	void  Close(const int nSndBufferIndex);

	void  SetVolume(const long lVolume);

	void* Lock(const int nSndBufferIndex, const int nLockPos, const int nLockSize);
	void  UnLock(const int nSndBufferIndex);

	int   GetCurrentPos(const int nSndBufferIndex) const;

private:
	void ReleaseSoundBuffer(const int nSndBufferIndex);
	BOOL IsSoundBuffer(const int nSndBufferIndex) const;

private:
	struct DX_SOUND_BUFFER
	{
		IDirectSoundBuffer* pSndBuffer;
		void*				pLockData;
		unsigned long       ulLockDataSize;
	} m_SndBuffer[SOUNDBUFFER_MAX];
	
	IDirectSound8*		m_pDirectSound;
	IDirectSoundBuffer* m_pPrimaryBuffer;
	BOOL				m_bBringDirectSoundDevice;
};

#endif // !defined(AFX_DXSNDPLAYER_H__1D688ECF_DE05_4A46_B407_E5A77B92836A__INCLUDED_)
