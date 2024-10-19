// IPlayer.h: interface for the IPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPLAYER_H__98A7F025_FC8E_4D6A_8C92_D73F51E75A73__INCLUDED_)
#define AFX_IPLAYER_H__98A7F025_FC8E_4D6A_8C92_D73F51E75A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IPlayer  
{
public:
	IPlayer();
	virtual ~IPlayer();

	virtual int  CreateSoundBuffer(const int nBufferSize, const int nSamplePerSec, const int nBitPerSample, const int nChannel) = 0;
	virtual void ReleaseSoundBuffer(const int nSndBufferIndex){};

	virtual void* Lock(const int nSndBufferIndex, const int nLockPos, const int nLockSize) = 0;
	virtual void  UnLock(const int nSndBufferIndex) = 0;

	virtual BOOL Play(const int nSndBufferIndex, const BOOL bLoop){ return FALSE;}
	virtual BOOL Pause(const int nSndBufferIndex){ return FALSE;}
	virtual void Close(const int nSndBufferIndex){};

	virtual void SetVolume(const long lVolume) = 0;

	virtual int GetCurrentPos(const int nSndBufferIndex)const {return -1;}
};

#endif // !defined(AFX_IPLAYER_H__98A7F025_FC8E_4D6A_8C92_D73F51E75A73__INCLUDED_)
