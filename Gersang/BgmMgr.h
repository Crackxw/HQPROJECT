
/**************************************************************************************************************
 *  @author   김성준 (sagolboss)
 *  @date     2004-07-25
 *  @brief    이 클래스는 거상에서 BGM을 관리하는 클래스이다. 
 *				
 *            BOOL Init(const HWND hWnd, const int nBufferSize);			 // 직접 Device 생성한다. 
 *			  BOOL Init(IDirectSound8* pDirectSound, const int nBufferSize); // Device Ptr를 가져와 사용한다. 
 *            현재는 EaxSound::LPDIRECTSOUND8 m_lpDS 포인터를 가져와서 사용하고 있다.
 *
 *	@see      EaxSound::LPDIRECTSOUND8 m_lpDS
 *	@warning  현재 EaxSound::LPDIRECTSOUND8 m_lpDS 포인터가 NULL 이라면 에러 메세지를 띄운다. 
 **************************************************************************************************************/

#if !defined(AFX_BGMMGR_H__3F3AA0AE_B024_4080_983E_A80835F0AE60__INCLUDED_)
#define AFX_BGMMGR_H__3F3AA0AE_B024_4080_983E_A80835F0AE60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786) //STL warning 않나게
using namespace std;

enum OGGBGM_FILENAME_LIST
{
	OGGBGM_FILENAME_INVALID = -1,

	OGGBGM_FILENAME_GERSANGTITLE = 0,
	OGGBGM_FILENAME_GERSANGTITLE_JAPAN,

	OGGBGM_FILENAME_KOREAFIELD,
	OGGBGM_FILENAME_JAPANFIELD,
	OGGBGM_FILENAME_CHINAFIELD , 
	OGGBGM_FILENAME_TAIWANFIELD ,

	OGGBGM_FILENAME_KOREAVIALLGE,
	OGGBGM_FILENAME_CHINAVIALLGE,
	OGGBGM_FILENAME_JAPANVIALLGE,
	OGGBGM_FILENAME_TAIWANVIALLGE,

	OGGBGM_FILENAME_WARTITLE    ,
	OGGBGM_FILENAME_MINIGAME    ,

	OGGBGM_FILENAME_WIN,
	OGGBGM_FILENAME_LOSE,

	OGGBGM_FILENAME_NUNOCHON_KYUNGMAE,

	// robypark 2004/12/20 16:14
	// 공성전 중에 들려지는 배경음악 추가
	OGGBGM_FILENAME_SIEGEBATTLEMUSIC,

	OGGBGM_FILENAME_MAX,
};

struct BGM_INFO
{
public:
	BGM_INFO(const char* szFileName, const int nFileListIndex)
	{
		strFileName = szFileName;
		nIndex      = nFileListIndex;
	}
	~BGM_INFO() {};

public:
	string strFileName;
	int    nIndex;
	BOOL   bLoop;
};

class ISoundData;
class CMyGBM;
class CDxSndPlayer;
class CStreamSound;
class CBgmMgr : public CSingleton<CBgmMgr>
{
public:
	CBgmMgr();
	virtual ~CBgmMgr();

	BOOL Init(const HWND hWnd, const int nBufferSize);
	BOOL Init(IDirectSound8* pDirectSound, const int nBufferSize);
	
	BOOL PlayNextBgm(const BOOL bLoop);
	BOOL Play(int nBgmFileNameIndex, const BOOL bLoop);
	BOOL Play(const char* szFileName, const BOOL bLoop);
	void Close();

	BOOL IsPlaying();

	void SetVolume(long lVolume);
	void SetNextBgm(int nNextBgmIndex);
	void SetNextBgm(const char* szFileName);

	inline int  GetCurrentBgmIndex()	{ return m_iCurrentPlayingBgmIndex; }
	inline int  GetNextBgmIndex()		{ return m_nNextPlayBgmIndex; }

#ifdef _DEBUG
	inline int GetPlayCount() { return m_nPlayCount; }
#endif
	
private:
	void Release();

	ISoundData* GetSoundData(const char* szFileName);	
	
private:
	CDxSndPlayer*    m_pDxSndPlayer;
	CStreamSound*    m_pStreamSound;
	int					m_iCurrentPlayingBgmIndex;	// actdoll (2004/11/29 18:04) : 루핑에 필요한 데이터
	int			     m_nNextPlayBgmIndex;
	string			 m_strNextPlayBgmFileName;
	BOOL		     m_bContainSoundData;
		
	vector<BGM_INFO>  m_BgmInfoList;
	queue<int>		  m_QueuePlayBgm;

	DWORD m_nPrevTick;	

#ifdef _DEBUG
	int m_nPlayCount;
#endif
};

inline CBgmMgr *GetBgmMgr()
{
	return (CBgmMgr *)CBgmMgr::GetManagePtr();
}


#endif // !defined(AFX_BGMMGR_H__3F3AA0AE_B024_4080_983E_A80835F0AE60__INCLUDED_)
