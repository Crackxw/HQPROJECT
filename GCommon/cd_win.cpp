#include <cd_win.h>
#include <mmsystem.h>
#include <stdio.h>
static  DWORD wDeviceID = NULL;  // ��ġ��ȣ ���� 1���̴�.  ?
static  HWND  hWnd ;             // ������ �ڵ�  

static  BOOL		bCDValid     = FALSE; // CD Audio ���� ���� ���� ��������, CD ������ ���ü� ����.(�� CD �� ����.)
static  BOOL		bPlaying     = FALSE; // ���� CD Audio ���� ���� ���� 
static  BOOL		bWasPlaying  = FALSE; // ���� ���°� �������̾����� ���� (Pause �� Resume����)
static  BOOL		bInitialized = FALSE; // CD ROM �ʱ�ȭ ���� �������� 
static  BOOL		bEnabled     = FALSE; // ���� ���� ON, OFF  �������� 
static  BOOL        bPlayLooping = FALSE; // ��� ���� ���ָ� �Ұ��ΰ� 
static  BOOL        bCDVolume    = TRUE;  // CD Voluem ��  ON, OFF
static  UI32		uiMaxTrack;           // �ִ� Ʈ������ ����Ѵ�.    

//--------------------------------------------------------------------------
// Name: CDAudioInitial()
// Desc: CD ROM DRIVE�� �ʱ�ȭ �Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioInitial(HWND hwnd)        
{
	MCIERROR    	dwReturn;
	MCI_OPEN_PARMS	mciOpenParms;
    MCI_SET_PARMS	mciSetParms;

	// Window handle�� �����Ѵ�.
	hWnd  = hwnd;

	// ����ü�� �ʱ�ȭ�Ѵ�.
	memset(&mciOpenParms,0,sizeof(MCI_OPEN_PARMS)); 
	
	// ��ġ ��ȣ�� ���´�.
	mciOpenParms.lpstrDeviceType = TEXT("cdaudio");
	//mciOpenParms.lpstrDeviceType = MAKEINTRESOURCE(MCI_DEVTYPE_CD_AUDIO);
	if (dwReturn = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_SHAREABLE, (DWORD) (LPVOID) &mciOpenParms))
	{
		return FALSE;       //MCI_OPEN ���� 
	}
	// ��ġ��ȣ ���� 
	wDeviceID = mciOpenParms.wDeviceID;

	// Time Format�� ���Ѵ�.track/minute/second/frame (TMSF).
    mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &mciSetParms))
    {   // MCI_SET_TIME_FORMAT����  
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		return FALSE;
    }

	// ���� �ʱ�ȭ 
	bInitialized = TRUE;        // �ʱ�ȭ �Ǿ����� �����Ѵ�.
	bEnabled     = TRUE;        // ���ְ��� ��ġ �ʱ�ȭ 

	// CD Audio Disk ������ ���´�.
	if (CDAudioGetAudioDiskInfo())
	{
		bCDValid = FALSE;       // CD ������ ���ü� ����.(�� CD �� ����.)
	}

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioShutdown()
// Desc: CD ROM DRIVE Audio�� �����Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioShutdown(void)
{
	// �ʱ�ȭ ���� �ʾ����� ���ư���.
	if (!bInitialized)		return FALSE;
	// �������ΰ��� �����Ѵ�.
	CDAudioStop();
	// MCI Driver �� �����Ѵ�.
	if (mciSendCommand(wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD)NULL))
		return FALSE;
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudio_Eject()
// Desc: CD-ROM DRIVE�� Tray�� Open�Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioEject(void)
{
	MCIERROR	dwReturn;

    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD)NULL))
		return FALSE;         // MCI_SET_DOOR_OPEN ���� 

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioCloseDoor()
// Desc: CD-ROM DRIVE�� Tray�� Close�Ѵ�. 
//--------------------------------------------------------------------------
BOOL CDAudioCloseDoor(void)
{
	MCIERROR	dwReturn;

    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_CLOSED, (DWORD)NULL))
		return FALSE;         // MCI_SET_DOOR_CLOSED ���� 

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: CDAudioGetAudioDiskInfo()
// Desc: CD-ROM �� Audio Disk�� ������ ���´�.
//--------------------------------------------------------------------------
BOOL CDAudioGetAudioDiskInfo(void)
{
	MCIERROR			dwReturn;
	MCI_STATUS_PARMS	mciStatusParms;

	// CD �� ������ �����ϴ� ���� �ʱ�ȭ 
	bCDValid = FALSE;

	memset(&mciStatusParms,0,sizeof(MCI_STATUS_PARMS));

	mciStatusParms.dwItem = MCI_STATUS_READY;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)                   return FALSE; // ���¸� ������ ���ߴ�.
	if (!mciStatusParms.dwReturn)   return FALSE; // DRIVE�� ���� �غ� �ȵǾ���

	// Audio Track ���� ���´�.
	mciStatusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)                   return FALSE; // Track�� ���¸� ����µ� �����Ͽ���.  
	if (mciStatusParms.dwReturn < 1)return FALSE; // ���� Ʈ���� ����.
	
	//������ �����Ѵ�.
	bCDValid = TRUE;                              // CD ������ ���Դ�.         
	uiMaxTrack = mciStatusParms.dwReturn;         // Ʈ���� ���ڸ� ����Ѵ�.

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: CDAudioPlay()
// Desc: CD Audio Track�� ���� �Ѵ�. 
//--------------------------------------------------------------------------
BOOL CDAudioPlay(UI32 uiTrack, BOOL bLooping)
{
	MCIERROR			dwReturn;
    MCI_PLAY_PARMS		mciPlayParms;
	MCI_STATUS_PARMS	mciStatusParms;

	// ���� ���� �����̸� ���ָ� �Ѵ�.
	if (!bEnabled)		return TRUE;
	// CD Audio������ ������ ���Ͽ����� �ٽ� �õ��Ѵ�.
	if (!bCDValid)
	{
		CDAudioGetAudioDiskInfo();
		if (!bCDValid)	return FALSE;
	}

	// 1�� Ʈ������ �۰� �ִ� Ʈ������ ũ�� ERROR
	if (uiTrack < 1 || uiTrack > uiMaxTrack) return FALSE;

	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 

	// Non-Audio Track�� �����ҷ��� ���� ���ϰ� �Ѵ�.
	mciStatusParms.dwItem  = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatusParms.dwTrack = uiTrack;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)       return FALSE;	//  MCI_STATUS  ��� ���� 
	if (mciStatusParms.dwReturn != MCI_CDA_TRACK_AUDIO) return FALSE; // ���� ���� ��ų���� Ʈ���� Audio Track �� �ƴϴ�. 

	// ���ֵ� Track�� ���̸� ���´�.
	mciStatusParms.dwItem  = MCI_STATUS_LENGTH;
	mciStatusParms.dwTrack = uiTrack;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)       return FALSE;	//  MCI_STATUS  ��� ���� 
	// ���� ������ �϶� �ٽ� ���� ��Ű�� ���ָ� �ߴ��Ѵ�.
	if (bPlaying)       CDAudioStop();           

    mciPlayParms.dwFrom = MCI_MAKE_TMSF(uiTrack,  0, 0, 0);
	mciPlayParms.dwTo = (mciStatusParms.dwReturn << 8) | uiTrack;
    mciPlayParms.dwCallback = (DWORD)hWnd;
    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;    // MCI_PLAY ���� 
	
	bPlayLooping = bLooping;    // ���� Track�� �ݺ� ���� 
	bPlaying     = TRUE;        // ���� �������̴�.  

	if (bCDVolume == FALSE)		CDAudioPause ();
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioPlayL()
// Desc: 2��° Ʈ������ ���ָ� �����Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioPlayL(void)
{
	return CDAudioPlayFrom(2, TRUE);
}

//--------------------------------------------------------------------------
// Name: CDAudioPlayLFrom()
// Desc: CD Audio Track�� ���� �Ѵ�. 
//--------------------------------------------------------------------------
BOOL CDAudioPlayLFrom(UI32 uiTrack)
{
	return CDAudioPlayFrom(uiTrack, TRUE);
}


//--------------------------------------------------------------------------
// Name: CDAudioPlayFrom()
// Desc: ���ǿ��ָ� StartTrack ���� ������ �����Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioPlayFrom(UI32 uiStartTrack, BOOL bLooping)
{
	MCI_PLAY_PARMS mciPlayParms;
	MCIERROR       dwReturn;

	// ���� ���� �����̸� ���ָ� �Ѵ�.
	if (!bEnabled)		return TRUE;
	// CD Audio������ ������ ���Ͽ����� �ٽ� �õ��Ѵ�.
	if (!bCDValid)
	{
		CDAudioGetAudioDiskInfo();
		if (!bCDValid)	return FALSE;
	}

	// 1�� Ʈ������ �۰� �ִ� Ʈ������ ũ�� ERROR
	if (uiStartTrack < 1 || uiStartTrack > uiMaxTrack) return FALSE;

	// ���� ������ �϶� �ٽ� ���� ��Ű�� ���ָ� �ߴ��Ѵ�.
	if (bPlaying)       CDAudioStop();           

	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 

	mciPlayParms.dwFrom = MCI_MAKE_TMSF(uiStartTrack,0,0,0);
	mciPlayParms.dwCallback = (DWORD)hWnd;
    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM , (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;    // MCI_PLAY ���� 

	bPlayLooping = bLooping;    // ���� Track�� �ݺ� ���� 
	bPlaying     = TRUE;        // ���� �������̴�.  

	if (bCDVolume == FALSE)		CDAudioPause ();
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioPlayFromAndTo()
// Desc: ���ǿ��ָ� StartTrack ���� EndTrack�� ���� �����Ѵ�. 
//       EndTrack�� ���ָ� ���Ѵ�. ���� Maximum Track���� ���ָ� ���Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioPlayFromAndTo(UI32 uiStartTrack,UI32 uiEndTrack, BOOL bLooping)
{
	MCI_PLAY_PARMS mciPlayParms;
	MCIERROR       dwReturn;
	

	// ���� ���� �����̸� ���ָ� �Ѵ�.
	if (!bEnabled)		return TRUE;
	// CD Audio������ ������ ���Ͽ����� �ٽ� �õ��Ѵ�.
	if (!bCDValid)
	{
		CDAudioGetAudioDiskInfo();
		if (!bCDValid)	return FALSE;
	}

	// 1�� Ʈ������ �۰� �ִ� Ʈ������ ũ�� ERROR
	if (uiStartTrack < 1 || uiStartTrack > uiMaxTrack) return FALSE;

	// 1�� Ʈ������ �۰� �ִ� Ʈ������ ũ�� ERROR
	if (uiEndTrack < 1 || uiEndTrack > uiMaxTrack) return FALSE;

	// ���� ������ �϶� �ٽ� ���� ��Ű�� ���ָ� �ߴ��Ѵ�.
	if (bPlaying)       CDAudioStop();           
		
	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 

	mciPlayParms.dwFrom = MCI_MAKE_TMSF(uiStartTrack,0,0,0);
	mciPlayParms.dwTo   = MCI_MAKE_TMSF(uiEndTrack,0,0,0);
	mciPlayParms.dwCallback = (DWORD)hWnd;

    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_TO, (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;    // MCI_PLAY ���� 

	bPlayLooping = bLooping;    // ���� Track�� �ݺ� ���� 
	bPlaying     = TRUE;        // ���� �������̴�.  

	if (bCDVolume == FALSE)		CDAudioPause ();
	return TRUE;

}

//--------------------------------------------------------------------------
// Name: CDAudioStop()
// Desc: CD Audio Track���ָ� �����Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioStop(void)
{
	MCIERROR	dwReturn;

	// ���� ���� ���� ���°� OFF �̸� 
	if (!bEnabled)		return FALSE;
	// ���� �������� �ƴϸ� 
	if (!bPlaying)		return FALSE;
	// ���� ���� 
    if (dwReturn = mciSendCommand(wDeviceID, MCI_STOP, 0, (DWORD)NULL))
		return FALSE;
		
	bWasPlaying  = FALSE;      // Pause�� ���°� �ƴϴ�.
	bPlaying     = FALSE;      // �����ϰ� �ִ� ���°� �ƴϴ�.
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioPause()
// Desc: CD ���ָ� ��� �ߴ� ��Ų��.
//--------------------------------------------------------------------------
BOOL CDAudioPause(void)
{
	MCIERROR			dwReturn;
	MCI_GENERIC_PARMS	mciGenericParms;

	// ���� ���� ���� ���°� OFF �̸� 
	if (!bEnabled)		return FALSE;
	// ���� �������� �ƴϸ� 
	if (!bPlaying)		return FALSE;

	memset(&mciGenericParms,0,sizeof(MCI_GENERIC_PARMS)); 
	// ���ָ� ��� �����.
	mciGenericParms.dwCallback = (DWORD)hWnd;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_PAUSE, 0, (DWORD)(LPVOID) &mciGenericParms))
		return FALSE;

	bWasPlaying = bPlaying;      // ������ ���ֻ��¸� ���� 
	bPlaying    = FALSE;         // ���� ���ֻ��°� �ƴϴ�.
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioResume()
// Desc: ��� Pause ��Ų ���ָ� ��� �簳�Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioResume(void)
{
	MCIERROR		dwReturn;
    MCI_PLAY_PARMS	mciPlayParms;
	// ���� ���� ���� ���°� OFF �̸� 
	if (!bEnabled)		return FALSE;
	// ���� ���� ���¸� ������ ���ϸ� 
	if (!bCDValid)		return FALSE;
	// ���� ���� 
	if (!bWasPlaying)	return FALSE;
	
	memset(&mciPlayParms,0,sizeof(MCI_PLAY_PARMS)); 
	// ���� ��� 
	mciPlayParms.dwCallback = (DWORD)hWnd;
    dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID) &mciPlayParms);
	if (dwReturn)       return FALSE;   //  MCI_PLAY ���� 
		
	bPlaying = TRUE;    // ���� ������ �̴�.   
	return TRUE;
}

//--------------------------------------------------------------------------
// Name: CDAudioFunction()
// Desc: CD Audio�� �� ����� �Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioFunction(UI32 Command, UI32 uiParam)
{
	
/*	case CD_ON:		// CD Audio ���� ON  ���� 
	case CD_OFF:	// CD Audio ���� OFF ���� 
	case CD_RESET:	// CD Audio ���� RESET 
	case CD_CLOSE:  // CD Drive�� �ݴ´�.
	case CD_REJECT: // CD Drive�� ����.
	case CD_PLAY:   // CD Audio�� �����Ѵ�.
	case CD_LOOP:   // CD Audio�� �ݺ� �����Ѵ�.
	case CD_STOP:   // CD Audio�� ���ָ� �ߴ��Ѵ�.
	case CD_PAUSE:  // CD Audio�� ���ָ� ��� �����.
	case CD_RESUME: // CD Audio�� ���ָ� �ٽ� �����Ѵ�.
	case CD_INFO:
  
	switch(Command)
	{
	case CD_ON:		// CD Audio ���� ON  ���� 
		bEnabled = true;
		break;
	case CD_OFF:	// CD Audio ���� OFF ���� 
		if (bPlaying) CDAudioStop();	bEnabled = FALSE;
		break;
	case CD_RESET:	// CD Audio ���� RESET 
		if (bPlaying) CDAudioStop();                 // �������̸� STOP
		CDAudioGetAudioDiskInfo();                   // CD Audio ������ ���´�.
		bEnabled = TRUE;                              // ���� ���� ���� ���� 
		break;
	case CD_CLOSE:  // CD Drive�� �ݴ´�.
		CDAudioCloseDoor();
		break;
	case CD_REJECT: // CD Drive�� ����.
		if (bPlaying)	CDAudioStop();
		CDAudioEject();
		bCDValid = FALSE;
		break;
	case CD_PLAY:   // CD Audio�� �����Ѵ�.
		CDAudioPlay(uiParam, FALSE);
		break;
	case CD_LOOP:   // CD Audio�� �ݺ� �����Ѵ�.
		CDAudioPlay(uiParam, TRUE);
		break;
	case CD_STOP:   // CD Audio�� ���ָ� �ߴ��Ѵ�.
		CDAudioStop();
		break;
	case CD_PAUSE:  // CD Audio�� ���ָ� ��� �����.
		CDAudioPause();
		break;
	case CD_RESUME: // CD Audio�� ���ָ� �ٽ� �����Ѵ�.
		CDAudioResume();
		break;
	case CD_INFO:
		// uiMaxTrack, bWasPlaying, bPlaying, uiPlayTrack, bCDVolume
		break;
	}
 */
	if (!bCDValid)  // ���� ������ CD������ ���� �������� 
	{
		CDAudioGetAudioDiskInfo();  //  ������ ���ͼ� 
		if (!bCDValid)               //  �ٽ� CD Audio������ ���� �������� 
		{
			return FALSE;			 //  No CD 
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------------
// Name: 
// Desc: Window Procedure���� ó���Ѵ�.
//--------------------------------------------------------------------------
BOOL CDAudioMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ((UI32)lParam != wDeviceID)	return FALSE;

	switch (wParam)
	{
		case MCI_NOTIFY_SUCCESSFUL:
			if (bPlaying)             // ���� �������̸� 
			{
				bPlaying = FALSE;     // �������� ������ �����ϰ�
				// Looping �� ���� �Ǿ� ������ ��� ���ֽ�Ų��.
				if (bPlayLooping)     CDAudioPlayL(); 
			}
			break;

		case MCI_NOTIFY_ABORTED:
		case MCI_NOTIFY_SUPERSEDED:
			break;

		case MCI_NOTIFY_FAILURE:     
			CDAudioStop();          // ���ָ� ������Ű�� 
			bCDValid = FALSE;         // CD Audio ���� �������� ���� ���� 
			break;

		default:
			return FALSE;
	}

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: CDAudioGetTrackLength()
// Desc: ������ Ʈ���� ���̸� ���´�.
//--------------------------------------------------------------------------
DWORD CDAudioGetTrackLength(UI32 uiTrack)
{
	
	MCI_STATUS_PARMS mciStatusParms;
	MCIERROR         dwReturn;
	DWORD            TempMin,TempSec,TempFrame,Frame;

	memset(&mciStatusParms,0,sizeof(MCI_STATUS_PARMS));

	// ���ֵ� Track�� ���̸� ���´�.
	mciStatusParms.dwItem  = MCI_STATUS_LENGTH;
	mciStatusParms.dwTrack = uiTrack;
    dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms);
	if (dwReturn)       return 0;	//  MCI_STATUS  ��� ���� 

	TempMin = MCI_MSF_MINUTE(mciStatusParms.dwReturn);
	TempSec = MCI_MSF_SECOND(mciStatusParms.dwReturn);
	TempFrame = MCI_MSF_FRAME(mciStatusParms.dwReturn);

	Frame = (TempMin*60 + TempSec)*75 + TempFrame;
	return Frame;
}

//--------------------------------------------------------------------------
// Name: CDAudioGetCurrentCDTrack()
// Desc: �Լ��� ������ Ʈ����ġ�� �˾Ƴ���.
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



