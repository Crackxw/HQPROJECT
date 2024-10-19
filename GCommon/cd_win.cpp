#include <cd_win.h>
#include <mmsystem.h>
#include <stdio.h>
static  DWORD wDeviceID = NULL;  // 장치번호 보통 1번이다.  ?
static  HWND  hWnd ;             // 윈도우 핸들  

static  BOOL		bCDValid     = FALSE; // CD Audio 연주 가능 상태 설정변수, CD 정보를 얻어올수 없다.(즉 CD 가 없다.)
static  BOOL		bPlaying     = FALSE; // 현재 CD Audio 연수 상태 설정 
static  BOOL		bWasPlaying  = FALSE; // 전의 상태가 연주중이었는지 설정 (Pause 와 Resume위해)
static  BOOL		bInitialized = FALSE; // CD ROM 초기화 상태 설정변수 
static  BOOL		bEnabled     = FALSE; // 현재 연주 ON, OFF  설정변수 
static  BOOL        bPlayLooping = FALSE; // 계속 다음 연주를 할것인가 
static  BOOL        bCDVolume    = TRUE;  // CD Voluem 값  ON, OFF
static  UI32		uiMaxTrack;           // 최대 트랙수를 기억한다.    

//--------------------------------------------------------------------------
// Name: CDAudioInitial()
// Desc: CD ROM DRIVE를 초기화 한다.
//--------------------------------------------------------------------------
BOOL CDAudioInitial(HWND hwnd)        
{
	MCIERROR    	dwReturn;
	MCI_OPEN_PARMS	mciOpenParms;
    MCI_SET_PARMS	mciSetParms;

	// Window handle을 저장한다.
	hWnd  = hwnd;

	// 구조체를 초기화한다.
	memset(&mciOpenParms,0,sizeof(MCI_OPEN_PARMS)); 
	
	// 장치 번호를 얻어온다.
	mciOpenParms.lpstrDeviceType = TEXT("cdaudio");
	//mciOpenParms.lpstrDeviceType = MAKEINTRESOURCE(MCI_DEVTYPE_CD_AUDIO);
	if (dwReturn = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_SHAREABLE, (DWORD) (LPVOID) &mciOpenParms))
	{
		return FALSE;       //MCI_OPEN 실패 
	}
	// 장치번호 설정 
	wDeviceID = mciOpenParms.wDeviceID;

	// Time Format를 정한다.track/minute/second/frame (TMSF).
    mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &mciSetParms))
    {   // MCI_SET_TIME_FORMAT실패  
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		return FALSE;
    }

	// 변수 초기화 
	bInitialized = TRUE;        // 초기화 되었음을 설정한다.
	bEnabled     = TRUE;        // 연주가능 장치 초기화 

	// CD Audio Disk 정보를 얻어온다.
	if (CDAudioGetAudioDiskInfo())
	{
		bCDValid = FALSE;       // CD 정보를 얻어올수 없다.(즉 CD 가 없다.)
	}

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioShutdown()
// Desc: CD ROM DRIVE Audio를 종료한다.
//--------------------------------------------------------------------------
BOOL CDAudioShutdown(void)
{
	// 초기화 되지 않았으면 돌아간다.
	if (!bInitialized)		return FALSE;
	// 연주중인것을 중지한다.
	CDAudioStop();
	// MCI Driver 를 해제한다.
	if (mciSendCommand(wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD)NULL))
		return FALSE;
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudio_Eject()
// Desc: CD-ROM DRIVE의 Tray를 Open한다.
//--------------------------------------------------------------------------
BOOL CDAudioEject(void)
{
	MCIERROR	dwReturn;

    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD)NULL))
		return FALSE;         // MCI_SET_DOOR_OPEN 실패 

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioCloseDoor()
// Desc: CD-ROM DRIVE의 Tray를 Close한다. 
//--------------------------------------------------------------------------
BOOL CDAudioCloseDoor(void)
{
	MCIERROR	dwReturn;

    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_CLOSED, (DWORD)NULL))
		return FALSE;         // MCI_SET_DOOR_CLOSED 실패 

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: CDAudioGetAudioDiskInfo()
// Desc: CD-ROM 의 Audio Disk의 정보를 얻어온다.
//--------------------------------------------------------------------------
BOOL CDAudioGetAudioDiskInfo(void)
{
	MCIERROR			dwReturn;
	MCI_STATUS_PARMS	mciStatusParms;

	// CD 가 있음을 설정하는 변수 초기화 
	bCDValid = FALSE;

	memset(&mciStatusParms,0,sizeof(MCI_STATUS_PARMS));

	mciStatusParms.dwItem = MCI_STATUS_READY;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)                   return FALSE; // 상태를 얻어오지 못했다.
	if (!mciStatusParms.dwReturn)   return FALSE; // DRIVE가 아직 준비가 안되었다

	// Audio Track 수를 얻어온다.
	mciStatusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)                   return FALSE; // Track의 상태를 얻오는데 실패하였다.  
	if (mciStatusParms.dwReturn < 1)return FALSE; // 음악 트랙이 없다.
	
	//변수를 설정한다.
	bCDValid = TRUE;                              // CD 정보를 얻어왔다.         
	uiMaxTrack = mciStatusParms.dwReturn;         // 트랙의 숫자를 기억한다.

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: CDAudioPlay()
// Desc: CD Audio Track을 연주 한다. 
//--------------------------------------------------------------------------
BOOL CDAudioPlay(UI32 uiTrack, BOOL bLooping)
{
	MCIERROR			dwReturn;
    MCI_PLAY_PARMS		mciPlayParms;
	MCI_STATUS_PARMS	mciStatusParms;

	// 연주 가능 상태이면 연주를 한다.
	if (!bEnabled)		return TRUE;
	// CD Audio정보를 얻어오지 못하였으면 다시 시도한다.
	if (!bCDValid)
	{
		CDAudioGetAudioDiskInfo();
		if (!bCDValid)	return FALSE;
	}

	// 1번 트랙보다 작고 최대 트랙보다 크면 ERROR
	if (uiTrack < 1 || uiTrack > uiMaxTrack) return FALSE;

	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 

	// Non-Audio Track을 연주할려고 하지 못하게 한다.
	mciStatusParms.dwItem  = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatusParms.dwTrack = uiTrack;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)       return FALSE;	//  MCI_STATUS  얻기 실패 
	if (mciStatusParms.dwReturn != MCI_CDA_TRACK_AUDIO) return FALSE; // 현재 연주 시킬려는 트랙이 Audio Track 이 아니다. 

	// 연주될 Track의 길이를 얻어온다.
	mciStatusParms.dwItem  = MCI_STATUS_LENGTH;
	mciStatusParms.dwTrack = uiTrack;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)       return FALSE;	//  MCI_STATUS  얻기 실패 
	// 현재 연주중 일때 다시 연주 시키면 연주를 중단한다.
	if (bPlaying)       CDAudioStop();           

    mciPlayParms.dwFrom = MCI_MAKE_TMSF(uiTrack,  0, 0, 0);
	mciPlayParms.dwTo = (mciStatusParms.dwReturn << 8) | uiTrack;
    mciPlayParms.dwCallback = (DWORD)hWnd;
    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;    // MCI_PLAY 실패 
	
	bPlayLooping = bLooping;    // 연주 Track의 반복 여부 
	bPlaying     = TRUE;        // 현재 연주중이다.  

	if (bCDVolume == FALSE)		CDAudioPause ();
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioPlayL()
// Desc: 2번째 트랙부터 연주를 시작한다.
//--------------------------------------------------------------------------
BOOL CDAudioPlayL(void)
{
	return CDAudioPlayFrom(2, TRUE);
}

//--------------------------------------------------------------------------
// Name: CDAudioPlayLFrom()
// Desc: CD Audio Track을 연주 한다. 
//--------------------------------------------------------------------------
BOOL CDAudioPlayLFrom(UI32 uiTrack)
{
	return CDAudioPlayFrom(uiTrack, TRUE);
}


//--------------------------------------------------------------------------
// Name: CDAudioPlayFrom()
// Desc: 음악연주를 StartTrack 부터 끝까지 연주한다.
//--------------------------------------------------------------------------
BOOL CDAudioPlayFrom(UI32 uiStartTrack, BOOL bLooping)
{
	MCI_PLAY_PARMS mciPlayParms;
	MCIERROR       dwReturn;

	// 연주 가능 상태이면 연주를 한다.
	if (!bEnabled)		return TRUE;
	// CD Audio정보를 얻어오지 못하였으면 다시 시도한다.
	if (!bCDValid)
	{
		CDAudioGetAudioDiskInfo();
		if (!bCDValid)	return FALSE;
	}

	// 1번 트랙보다 작고 최대 트랙보다 크면 ERROR
	if (uiStartTrack < 1 || uiStartTrack > uiMaxTrack) return FALSE;

	// 현재 연주중 일때 다시 연주 시키면 연주를 중단한다.
	if (bPlaying)       CDAudioStop();           

	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 

	mciPlayParms.dwFrom = MCI_MAKE_TMSF(uiStartTrack,0,0,0);
	mciPlayParms.dwCallback = (DWORD)hWnd;
    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM , (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;    // MCI_PLAY 실패 

	bPlayLooping = bLooping;    // 연주 Track의 반복 여부 
	bPlaying     = TRUE;        // 현재 연주중이다.  

	if (bCDVolume == FALSE)		CDAudioPause ();
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioPlayFromAndTo()
// Desc: 음악연주를 StartTrack 부터 EndTrack전 까지 연주한다. 
//       EndTrack은 연주를 못한다. 또한 Maximum Track까지 연주를 못한다.
//--------------------------------------------------------------------------
BOOL CDAudioPlayFromAndTo(UI32 uiStartTrack,UI32 uiEndTrack, BOOL bLooping)
{
	MCI_PLAY_PARMS mciPlayParms;
	MCIERROR       dwReturn;
	

	// 연주 가능 상태이면 연주를 한다.
	if (!bEnabled)		return TRUE;
	// CD Audio정보를 얻어오지 못하였으면 다시 시도한다.
	if (!bCDValid)
	{
		CDAudioGetAudioDiskInfo();
		if (!bCDValid)	return FALSE;
	}

	// 1번 트랙보다 작고 최대 트랙보다 크면 ERROR
	if (uiStartTrack < 1 || uiStartTrack > uiMaxTrack) return FALSE;

	// 1번 트랙보다 작고 최대 트랙보다 크면 ERROR
	if (uiEndTrack < 1 || uiEndTrack > uiMaxTrack) return FALSE;

	// 현재 연주중 일때 다시 연주 시키면 연주를 중단한다.
	if (bPlaying)       CDAudioStop();           
		
	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 

	mciPlayParms.dwFrom = MCI_MAKE_TMSF(uiStartTrack,0,0,0);
	mciPlayParms.dwTo   = MCI_MAKE_TMSF(uiEndTrack,0,0,0);
	mciPlayParms.dwCallback = (DWORD)hWnd;

    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;    // MCI_PLAY 실패 

	bPlayLooping = bLooping;    // 연주 Track의 반복 여부 
	bPlaying     = TRUE;        // 현재 연주중이다.  

	if (bCDVolume == FALSE)		CDAudioPause ();
	return TRUE;

}

//--------------------------------------------------------------------------
// Name: CDAudioStop()
// Desc: CD Audio Track연주를 중지한다.
//--------------------------------------------------------------------------
BOOL CDAudioStop(void)
{
	MCIERROR	dwReturn;

	// 현재 연주 가능 상태가 OFF 이면 
	if (!bEnabled)		return FALSE;
	// 현재 연주중이 아니면 
	if (!bPlaying)		return FALSE;
	// 연주 중지 
    if (dwReturn = mciSendCommand(wDeviceID, MCI_STOP, 0, (DWORD)NULL))
		return FALSE;
		
	bWasPlaying  = FALSE;      // Pause된 상태가 아니다.
	bPlaying     = FALSE;      // 연주하고 있는 상태가 아니다.
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioPause()
// Desc: CD 연주를 잠시 중단 시킨다.
//--------------------------------------------------------------------------
BOOL CDAudioPause(void)
{
	MCIERROR			dwReturn;
	MCI_GENERIC_PARMS	mciGenericParms;

	// 현재 연주 가능 상태가 OFF 이면 
	if (!bEnabled)		return FALSE;
	// 현재 연주중이 아니면 
	if (!bPlaying)		return FALSE;

	memset(&mciGenericParms,0,sizeof(MCI_GENERIC_PARMS)); 
	// 연주를 잠시 멈춘다.
	mciGenericParms.dwCallback = (DWORD)hWnd;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_PAUSE, 0, (DWORD)(LPVOID) &mciGenericParms))
		return FALSE;

	bWasPlaying = bPlaying;      // 현재의 연주상태를 저장 
	bPlaying    = FALSE;         // 현재 연주상태가 아니다.
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioResume()
// Desc: 잠시 Pause 시킨 연주를 계속 재개한다.
//--------------------------------------------------------------------------
BOOL CDAudioResume(void)
{
	MCIERROR		dwReturn;
    MCI_PLAY_PARMS	mciPlayParms;
	// 현재 연주 가능 상태가 OFF 이면 
	if (!bEnabled)		return FALSE;
	// 연주 가능 상태를 얻어오지 못하면 
	if (!bCDValid)		return FALSE;
	// 전에 연주 
	if (!bWasPlaying)	return FALSE;
	
	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 
	// 연주 계속 
	mciPlayParms.dwCallback = (DWORD)hWnd;
    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;   //  MCI_PLAY 실패 
		
	bPlaying = TRUE;    // 현재 연주중 이다.   
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioFunction()
// Desc: CD Audio의 각 기능을 한다.
//--------------------------------------------------------------------------
BOOL CDAudioFunction(UI32 Command, UI32 uiParam)
{
	
/*	case CD_ON:		// CD Audio 연주 ON  설정 
	case CD_OFF:	// CD Audio 연주 OFF 설정 
	case CD_RESET:	// CD Audio 연주 RESET 
	case CD_CLOSE:  // CD Drive를 닫는다.
	case CD_REJECT: // CD Drive를 연다.
	case CD_PLAY:   // CD Audio를 연주한다.
	case CD_LOOP:   // CD Audio를 반복 연주한다.
	case CD_STOP:   // CD Audio의 연주를 중단한다.
	case CD_PAUSE:  // CD Audio의 연주를 잠시 멈춘다.
	case CD_RESUME: // CD Audio의 연주를 다시 갱신한다.
	case CD_INFO:
  
	switch(Command)
	{
	case CD_ON:		// CD Audio 연주 ON  설정 
		bEnabled = true;
		break;
	case CD_OFF:	// CD Audio 연주 OFF 설정 
		if (bPlaying) CDAudioStop();	bEnabled = FALSE;
		break;
	case CD_RESET:	// CD Audio 연주 RESET 
		if (bPlaying) CDAudioStop();                 // 연주중이면 STOP
		CDAudioGetAudioDiskInfo();                   // CD Audio 정보를 얻어온다.
		bEnabled = TRUE;                              // 연주 가능 상태 설정 
		break;
	case CD_CLOSE:  // CD Drive를 닫는다.
		CDAudioCloseDoor();
		break;
	case CD_REJECT: // CD Drive를 연다.
		if (bPlaying)	CDAudioStop();
		CDAudioEject();
		bCDValid = FALSE;
		break;
	case CD_PLAY:   // CD Audio를 연주한다.
		CDAudioPlay(uiParam, FALSE);
		break;
	case CD_LOOP:   // CD Audio를 반복 연주한다.
		CDAudioPlay(uiParam, TRUE);
		break;
	case CD_STOP:   // CD Audio의 연주를 중단한다.
		CDAudioStop();
		break;
	case CD_PAUSE:  // CD Audio의 연주를 잠시 멈춘다.
		CDAudioPause();
		break;
	case CD_RESUME: // CD Audio의 연주를 다시 갱신한다.
		CDAudioResume();
		break;
	case CD_INFO:
		// uiMaxTrack, bWasPlaying, bPlaying, uiPlayTrack, bCDVolume
		break;
	}
 */
	if (!bCDValid)  // 연주 가능한 CD정보를 얻지 못했으면 
	{
		CDAudioGetAudioDiskInfo();  //  정보를 얻어와서 
		if (!bCDValid)               //  다시 CD Audio정보를 얻지 못했으면 
		{
			return FALSE;			 //  No CD 
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: 
// Desc: Window Procedure에서 처리한다.
//--------------------------------------------------------------------------
BOOL CDAudioMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ((UI32)lParam != wDeviceID)	return FALSE;

	switch (wParam)
	{
		case MCI_NOTIFY_SUCCESSFUL:
			if (bPlaying)             // 현재 연주중이면 
			{
				bPlaying = FALSE;     // 연주중의 변수를 설정하고
				// Looping 이 설정 되어 있으면 계속 연주시킨다.
				if (bPlayLooping)     CDAudioPlayL(); 
			}
			break;

		case MCI_NOTIFY_ABORTED:
		case MCI_NOTIFY_SUPERSEDED:
			break;

		case MCI_NOTIFY_FAILURE:     
			CDAudioStop();          // 연주를 중지시키고 
			bCDValid = FALSE;         // CD Audio 연주 가능정보 변수 설정 
			break;

		default:
			return FALSE;
	}

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: CDAudioGetTrackLength()
// Desc: 정해진 트랙의 길이를 얻어온다.
//--------------------------------------------------------------------------
DWORD CDAudioGetTrackLength(UI32 uiTrack)
{
	
	MCI_STATUS_PARMS mciStatusParms;
	MCIERROR         dwReturn;
	DWORD            TempMin,TempSec,TempFrame,Frame;

	memset(&mciStatusParms,0,sizeof(MCI_STATUS_PARMS));

	// 연주될 Track의 길이를 얻어온다.
	mciStatusParms.dwItem  = MCI_STATUS_LENGTH;
	mciStatusParms.dwTrack = uiTrack;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)       return 0;	//  MCI_STATUS  얻기 실패 

	TempMin = MCI_MSF_MINUTE(mciStatusParms.dwReturn);
	TempSec = MCI_MSF_SECOND(mciStatusParms.dwReturn);
	TempFrame = MCI_MSF_FRAME(mciStatusParms.dwReturn);

	Frame = (TempMin*60 + TempSec)*75 + TempFrame;
	return Frame;
}

//--------------------------------------------------------------------------
// Name: CDAudioGetCurrentCDTrack()
// Desc: 함수는 현재의 트랙위치를 알아낸다.
//--------------------------------------------------------------------------
DWORD CDAudioGetCurrentCDTrack(void)
{
	MCI_STATUS_PARMS mciStatusParms;
	MCIERROR         dwReturn;
	
	memset(&mciStatusParms,0,sizeof(MCI_STATUS_PARMS));

	mciStatusParms.dwItem = MCI_STATUS_CURRENT_TRACK;
	dwReturn = mciSendCommand(wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mciStatusParms);

	if(dwReturn) return 0;

	return mciStatusParms.dwReturn;
}



