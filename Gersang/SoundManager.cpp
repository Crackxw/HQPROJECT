#include <GSL.h>

#include "SoundManager.h"
#include "BindJXFile.h"
#include "OnlineCommonParser.h"		// sagolboss (2004/08/11 17:05) : ���� �ļ��� �߰�

#ifdef _OGG_PLAY
#include <vector>
#include "BgmMgr_PreHeader.h"
#endif
/*
	PlaySound()�� 3D���带 ���Ľ�Ű������ ����(����)��
	Volumne�� siVolumeX1, Y1�� �Ÿ��� ����޾� ����Ѵ�
	Pan�� �� ��,�� ����Ŀ�� ũ�⸦ 50/siPan �ܰ躰�� ������ �ִ�
*/
void SoundManager::SetEmulate3DValue(SI16 siVolumeX1, SI16 siVolumeY1, SI16 siPan1)
{
	siVolumeX = siVolumeX1;
	siVolumeY = siVolumeY1;
	siPan = siPan1;
}

/*
	EAXSound�� �����Ѵ�
	Parameter : ���� ����Ʈ ����, EAX����, �׼��� ������ ����ƽ ������ ���Լ�, �׼��� ������ ��Ʈ�� ������ ���Լ�
*/
void SoundManager::Create(char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic, SI16 siNStream)
{
	UI16 uiCount;

	if(pEAXSound)
	{
		pEAXSound->Delete();
		delete pEAXSound;
		pEAXSound = NULL;
	}
	pEAXSound = new EAXSound(m_hWnd);

	bSoundDriver = TRUE;

	if(!pEAXSound->Initialize(bEAX, bUsing3D))
		bSoundDriver = FALSE;

	pEAXSound->CreateStaticBufferEnvironment(siNStatic);

	for(uiCount=0; uiCount<MAX_DEFAULT_SOUNDLIST; uiCount++)
	{
		bStaticPlayList[uiCount] = 0;
		uiStaticPlayListIDInfo[uiCount] = 0;
		uiSoundListIDInfo[uiCount] = 0;

		bStreamPlayList[uiCount] = 0;
		uiStreamPlayListIDInfo[uiCount] = 0;
		uiSoundListIDInfo[uiCount] = 0;
	}
	if(uiStaticSlotInfo)
	{
		delete [] uiStaticSlotInfo;
		uiStaticSlotInfo = NULL;
	}
	uiStaticSlotInfo = new UI16[siNStatic+1];
	for(uiCount=0; uiCount<=siNStatic; uiCount++)
		uiStaticSlotInfo[uiCount] = 0;

	if(uiStreamSlotInfo)
	{
		delete [] uiStreamSlotInfo;
		uiStreamSlotInfo = NULL;
	}
	uiStreamSlotInfo = new UI16[siNStream+1];
	for(uiCount=0; uiCount<=siNStream; uiCount++)
		uiStreamSlotInfo[uiCount] = 0;

	uiStaticNum = 1;

	pEAXSound->CreateStreamBufferEnvironment(siNStream);

	uiStreamNum = 1;

	siListenerPosX = 0;
	siListenerPosY = 0;

	ZeroMemory(SoundList, sizeof(SoundList));

	if(pFile)
		LoadPlayList(pFile);
}

/*
	���� �Ŵ����� ������
	Parameter : ���������ڵ�, ���帮��Ʈ ����, EAX��������, 3D��������, ����ƽ ���Լ�, ��Ʈ�� ���Լ�
*/
SoundManager::SoundManager(HWND hWnd, char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic1, SI16 siNStream1)
:	siNStatic(-1),
	siNStream(-1)
{
	m_hWnd = hWnd;
	pEAXSound = NULL;

	DXUtil_Timer(TIMER_START);

	(siNStatic == -1) ? siNStatic = MAX_DEFAULT_STATIC : siNStatic = siNStatic1;
	(siNStream == -1) ? siNStream = MAX_DEFAULT_STREAM : siNStream = siNStream1;

	SetEmulate3DValue(1, 7, 3);

	uiStaticSlotInfo = NULL;
	uiStreamSlotInfo = NULL;

	uiListIDNum = 0;
	uiLastIDNum = 0;

	uiMaxStaticVolume = 0;
	uiMaxStreamVolume = 0;

#ifdef _OGG_PLAY
	m_lStreamSoundVolume = 0;
#endif
	
	Create(pFile, bEAX, bUsing3D, siNStatic, siNStream);
}

/*
	���� �Ŵ��� ��ü�� �����
	Parameter :
*/
void SoundManager::ReCreate(char *pFile, BOOL bEAX, BOOL bUsing3D, SI16 siNStatic1, SI16 siNStream1)
{
	float fNowTime;
	static float fLastTime = 0;
	fNowTime = DXUtil_Timer(TIMER_GETAPPTIME);
	if(fNowTime - fLastTime < 0.5f)
		return;
	fLastTime = fNowTime; 
	StopAllSound();
	Create(pFile, bEAX, bUsing3D, siNStatic1, siNStream1);
}

/*
	�Ҹ���
*/
SoundManager::~SoundManager()
{
	DXUtil_Timer(TIMER_STOP);

	StopAllSound();

	if(pEAXSound)
	{
		pEAXSound->Delete();
		delete pEAXSound;
		pEAXSound = NULL;
	}


	if(uiStaticSlotInfo)
	{
		delete [] uiStaticSlotInfo;
		uiStaticSlotInfo = NULL;
	}
	if(uiStreamSlotInfo)
	{
		delete [] uiStreamSlotInfo;
		uiStreamSlotInfo = NULL;
	}
}

/*
	Static ���� �ʱ�ȭ
*/
void SoundManager::ReleaseAllStaticSound(void)
{
	for(UI16 uiID=0; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(SoundList[uiSoundListIDInfo[uiID]].bStatic)
		{
			if(bStaticPlayList[uiID])
			{
				StopStaticSound(uiStaticPlayListIDInfo[uiID], SoundList[uiID].bLoop);
				pEAXSound->ReleaseStaticBuffer(uiStaticPlayListIDInfo[uiID]-1);
				uiStaticPlayListIDInfo[uiID] = 0;
				bStaticPlayList[uiID] = FALSE;
			}
		}
		if((uiID > uiListIDNum) && (uiLastIDNum > 0))
			DelPlayID(uiID);
	}
}

/*
	Stream ���� �ʱ�ȭ
*/
void SoundManager::ReleaseAllStreamSound(void)
{
	for(UI16 uiID=0; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(bStreamPlayList[uiID])
		{
			StopStreamSound(uiStreamPlayListIDInfo[uiID]);
			pEAXSound->ReleaseStreamBuffer(uiStreamPlayListIDInfo[uiID]-1);
			uiStreamPlayListIDInfo[uiID] = 0;
			bStreamPlayList[uiID] = FALSE;
		}
		if((uiID > uiListIDNum) && (uiLastIDNum > 0))
			DelPlayID(uiID);
	}
}

/*
	ID�� �ش��ϴ� ���� �ʱ�ȭ
*/
void SoundManager::ReleaseSound(UI16 uiID)
{
	if(uiID <= 0) return;

	if(SoundList[uiSoundListIDInfo[uiID]].bStatic)
	{
		if(bStaticPlayList[uiID])
		{
			StopStaticSound(uiStaticPlayListIDInfo[uiID], SoundList[uiID].bLoop);
			pEAXSound->ReleaseStaticBuffer(uiStaticPlayListIDInfo[uiID]-1);
			uiStaticPlayListIDInfo[uiID] = 0;
			bStaticPlayList[uiID] = FALSE;
		}
	}
	else
	{
		if(bStreamPlayList[uiID])
		{
			StopStreamSound(uiStreamPlayListIDInfo[uiID]);
			pEAXSound->ReleaseStreamBuffer(uiStreamPlayListIDInfo[uiID]-1);
			uiStreamPlayListIDInfo[uiID] = 0;
			bStreamPlayList[uiID] = FALSE;
		}
	}
	if((uiID > uiListIDNum) && (uiLastIDNum > 0))
		DelPlayID(uiID);
}

/*
	��� ���Կ� �ִ� ���带 �ʱ�ȭ ��Ų��
*/
void SoundManager::ReleaseAllSound(void)
{
	UI16 uiID;

	for(uiID=1; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(SoundList[uiSoundListIDInfo[uiID]].bStatic)
		{
			if(bStaticPlayList[uiID])
			{
				StopStaticSound(uiStaticPlayListIDInfo[uiID], SoundList[uiID].bLoop);
				pEAXSound->ReleaseStaticBuffer(uiStaticPlayListIDInfo[uiID]-1);
				uiStaticPlayListIDInfo[uiID] = 0;
				bStaticPlayList[uiID] = FALSE;
			}
		}
		else
		{
			if(bStreamPlayList[uiID])
			{
				StopStreamSound(uiStreamPlayListIDInfo[uiID]);
				pEAXSound->ReleaseStreamBuffer(uiStreamPlayListIDInfo[uiID]-1);
				uiStreamPlayListIDInfo[uiID] = 0;
				bStreamPlayList[uiID] = FALSE;
			}
		}
		ReleaseSound(uiID);
	}
	//pEAXSound->CreateStaticBufferEnvironment(siNStatic);
	//pEAXSound->CreateStreamBufferEnvironment(siNStream);
}

/*
	���� ID�� �߰�
	Parameter :
*/
UI16 SoundManager::AddPlayID(char *lpstrFile, BOOL bStatic, BOOL bLoop, BOOL bStop, UI08 uiDuplicate, float fDelay, SI64 siEAX, 
							 SI64 siVolume, SI64 siPan, SI64 siX, SI64 siY)
{
	if(uiLastIDNum >= MAX_DEFAULT_SOUNDLIST)
		return 0;

	for(UI16 uiID=uiListIDNum+1; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(SoundList[uiID].bActive == FALSE)
			break;
	}
	if(uiID == MAX_DEFAULT_SOUNDLIST)
		return 0;

	strcpy((char *)SoundList[uiID].pFileName, lpstrFile);
	SoundList[uiID].bStatic			= bStatic;
	SoundList[uiID].nDuplicate		= uiDuplicate;
	SoundList[uiID].fDuplicateDelay	= fDelay;
	SoundList[uiID].siEAX			= siEAX;
	SoundList[uiID].bLoop			= bLoop;
	SoundList[uiID].siVolume		= siVolume;
	SoundList[uiID].siPan			= siPan;
	SoundList[uiID].siPosX			= siX;
	SoundList[uiID].siPosY			= siY;
	SoundList[uiID].bStop			= bStop;
	SoundList[uiID].uiID			= uiID;

	SoundList[uiID].siRPosX			= SoundList[uiID].siPosX;
	SoundList[uiID].siRPosY			= SoundList[uiID].siPosY;
	SoundList[uiID].bActive			= TRUE;

	uiSoundListIDInfo[SoundList[uiID].uiID] = uiID;

	uiLastIDNum++;

	return uiID;
}

/*
	���� ID�� ����
	Parameter :
*/
BOOL SoundManager::DelPlayID(UI16 uiID)
{
	if(uiID <= uiListIDNum) return TRUE;
	if(uiID == 0) return TRUE;
	if(uiLastIDNum == 0) return TRUE;

	if(IsPlaying(uiID))
		StopSound(uiID);
	if(SoundList[uiSoundListIDInfo[uiID]].bStatic)
		bStaticPlayList[uiID] = 0;
	else
		bStreamPlayList[uiID] = 0;

	ZeroMemory(&SoundList[uiSoundListIDInfo[uiID]], sizeof(SoundManager::SoundInfo_struct));
	uiLastIDNum--;

	return TRUE;
}

/*
	���� ����Ʈ ������ �о����
*/

/** ������ : �輺��
    ������ : 2004-08-11 ���� 5:00
	��  �� : ������ �ִ� �ļ��� ������� �ʰ� CommonParser�� ����Ѵ� 
*/
BOOL SoundManager::LoadPlayList(char *lpstrFile)
{
/*************************************************************************** 
 *  ���� ������ �ҽ� (2004-08-11 ���� 4:17 ���ĺ���)
 ***************************************************************************/
	FILE* fp = NULL;
	char  szBuffer[128];
	OnlineCommonParser	ocp;

	uiListIDNum = 1;

	if((fp = BindJXFile::GetInstance()->GetFile( lpstrFile )) == NULL)
	{
		MessageBox(NULL, "Can't open Soundlist\"", "Error", MB_OK);
		return FALSE;
	}	

	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", "EffectSound.txt" );
	ocp.SetErrFileName( pszErrName );

	/// �Ľ��� �Ҷ� return �Ǵ� ���� 
	int	 iRet, iErrLine;
	BOOL bLess;

	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "OnlineText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", "EffectSound.txt", iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

/* ������ �ִ� �ҽ�(04-08-23 �ҽ�)
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	
			break;	
	
		/// �Ľ��� �Ҷ� return �Ǵ� ����
		int	 iLineNo, iColNo;
		ocp.GetValue(szBuffer, 128,								   iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].uiID,			   iLineNo, iColNo);
		ocp.GetValue((char*)SoundList[uiListIDNum].pFileName,	   MAX_DEFAULT_FILENAME, iLineNo, iColNo);
		ocp.GetValue(SoundList[uiListIDNum].bStatic,			   iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].nDuplicate,      iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].fDuplicateDelay, iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siEAX,	       iLineNo, iColNo);
		ocp.GetValue(SoundList[uiListIDNum].bLoop,				   iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siVolume,        iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siPan,           iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siPosX,          iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siPosY,          iLineNo, iColNo);
		ocp.GetValue(SoundList[uiListIDNum].bStop,				   iLineNo, iColNo);

		SoundList[uiListIDNum].siRPosX = SoundList[uiListIDNum].siPosX;
		SoundList[uiListIDNum].siRPosY = SoundList[uiListIDNum].siPosY;
		SoundList[uiListIDNum].bActive = TRUE;

		uiSoundListIDInfo[SoundList[uiListIDNum].uiID] = uiListIDNum;

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", "EffectSound.txt", ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;

		uiListIDNum++;
	}
*/
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )	
			break;	
	
		/// �Ľ��� �Ҷ� return �Ǵ� ����
		int	 iLineNo, iColNo;

		ocp.GetValue(szBuffer, 128,								   iLineNo, iColNo);
		ocp.GetValue((char*)SoundList[uiListIDNum].pFileName,	   MAX_DEFAULT_FILENAME, iLineNo, iColNo);
		ocp.GetValue(SoundList[uiListIDNum].bStatic,			   iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].nDuplicate,      iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].fDuplicateDelay, iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siEAX,	       iLineNo, iColNo);
		ocp.GetValue(SoundList[uiListIDNum].bLoop,				   iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siVolume,        iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siPan,           iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siPosX,          iLineNo, iColNo);
		ocp.GetValue((int&)SoundList[uiListIDNum].siPosY,          iLineNo, iColNo);
		ocp.GetValue(SoundList[uiListIDNum].bStop,				   iLineNo, iColNo);

		SoundList[uiListIDNum].siRPosX = SoundList[uiListIDNum].siPosX;
		SoundList[uiListIDNum].siRPosY = SoundList[uiListIDNum].siPosY;
		SoundList[uiListIDNum].bActive = TRUE;

		SoundList[uiListIDNum].uiID = uiListIDNum;
		uiSoundListIDInfo[SoundList[uiListIDNum].uiID] = uiListIDNum;

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "OnlineText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", "EffectSound.txt", ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
			fclose( fp );
			return FALSE;
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;

		uiListIDNum++;
	}

	ocp.Release();
	fclose( fp );
	fp = NULL;

	uiLastIDNum = --uiListIDNum;
	return TRUE;
}

/*
	��� ���带 ����
*/
void SoundManager::StopAllSound(void)
{
	if(!bSoundDriver) return;

	UI16 uiID;

	for(uiID=1; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
		StopSound(uiID);
}

/*
	ID�� �ش��ϴ� ���带 ����
*/
void SoundManager::StopSound(UI16 uiID)
{
	if(!bSoundDriver) return;
	if(uiID <= 0) return;

	if(bStaticPlayList[uiID]) // ���� ����Ʈ���� �ش� ȭ���� �ε�Ǿ����� �˻��Ѵ�
	{
		StopStaticSound(uiStaticPlayListIDInfo[uiID], SoundList[uiID].bLoop);
		bStaticPlayList[uiID] = FALSE;
	}
	if(bStreamPlayList[uiID])
	{
		StopStreamSound(uiStreamPlayListIDInfo[uiID]); 
		bStreamPlayList[uiID] = FALSE;
	}
}

/*
	��� ���带 ���� ���� ��Ų��
*/
void SoundManager::PauseAllSound(void)
{
	if(!bSoundDriver) return;

	UI16 uiID;

	for(uiID=1; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(bStaticPlayList[uiID]) // ����ƽ ���� ����Ʈ���� �ش� ȭ���� �ε�Ǿ����� �˻��Ѵ�
		{
			StopStaticSound(uiStaticPlayListIDInfo[uiID], TRUE);
		}
	}
	// ��Ʈ�� ����� ������Ű�� ���Ѵ�.
	// ��Ʈ���� ���� Play�� Stop���� ������
}

/*
	��� Static������ ������ �����Ѵ�
*/
void SoundManager::SetAllStaticSoundVolume(UI16 uiMaxVolume)
{
	if     (uiMaxVolume==1000)  uiMaxStaticVolume = 0;
	else if(uiMaxVolume>= 900)  uiMaxStaticVolume = 20;
    else if(uiMaxVolume>= 800)  uiMaxStaticVolume = 40;
    else if(uiMaxVolume>= 700)  uiMaxStaticVolume = 60;
    else if(uiMaxVolume>= 600)  uiMaxStaticVolume = 80;
    else if(uiMaxVolume>= 500)  uiMaxStaticVolume = 100;
    else if(uiMaxVolume>= 400)  uiMaxStaticVolume = 120;
    else if(uiMaxVolume>= 300)  uiMaxStaticVolume = 160;
    else if(uiMaxVolume>= 200)  uiMaxStaticVolume = 200;
    else if(uiMaxVolume>= 100)  uiMaxStaticVolume = 300;
    else if(uiMaxVolume>=   0)  uiMaxStaticVolume = 1000;

	//uiMaxStaticVolume = 1000-uiMaxVolume;

	for(UI16 uiID=1; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(SoundList[uiID].bStatic)
		{
			if(bStaticPlayList[uiID])
			{
				if(pEAXSound->m_bUsing3D)
				{
					pEAXSound->SetPositionX(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDS3DBuffer, (float)SoundList[uiSoundListIDInfo[uiID]].siRPosX);
					pEAXSound->SetPositionY(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDS3DBuffer, (float)SoundList[uiSoundListIDInfo[uiID]].siRPosY);
				}
				else
				{
					SI64 siVolume;

					if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
						siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
					else
						siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

					if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
						siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
					else
						siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

					SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
					SetSoundVolume(uiID, 1000+siVolume-uiMaxStaticVolume);
				}
				pEAXSound->SetStaticBufferPan(uiStaticPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siPan);
				pEAXSound->SetStaticBufferVolume(uiStaticPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siVolume);
			}
		}
	}
}

/*
	��� Stream������ ������ �����Ѵ�
*/
void SoundManager::SetAllStreamSoundVolume(UI16 uiMaxVolume)
{
	if     (uiMaxVolume==1000)  uiMaxStreamVolume = 0;
	else if(uiMaxVolume>= 900)  uiMaxStreamVolume = 20;
    else if(uiMaxVolume>= 800)  uiMaxStreamVolume = 40;
    else if(uiMaxVolume>= 700)  uiMaxStreamVolume = 60;
    else if(uiMaxVolume>= 600)  uiMaxStreamVolume = 80;
    else if(uiMaxVolume>= 500)  uiMaxStreamVolume = 100;
    else if(uiMaxVolume>= 400)  uiMaxStreamVolume = 120;
    else if(uiMaxVolume>= 300)  uiMaxStreamVolume = 160;
    else if(uiMaxVolume>= 200)  uiMaxStreamVolume = 200;
    else if(uiMaxVolume>= 100)  uiMaxStreamVolume = 300;
    else if(uiMaxVolume>=   0)  uiMaxStreamVolume = 1000;

#if _OGG_PLAY
	m_lStreamSoundVolume = 1000 - uiMaxStreamVolume;
	m_lStreamSoundVolume = -10000 + m_lStreamSoundVolume*10;
	if( GetBgmMgr())
		GetBgmMgr()->SetVolume( m_lStreamSoundVolume);
#else
	for(UI16 uiID=1; uiID<MAX_DEFAULT_SOUNDLIST; uiID++)
	{
		if(!SoundList[uiID].bStatic)
		{
			if(bStreamPlayList[uiID])
			{
				if(pEAXSound->m_bUsing3D)
				{
					pEAXSound->SetPositionX(pEAXSound->m_plpDS3DStreamBuffer[uiStreamPlayListIDInfo[uiID]-1], (float)SoundList[uiSoundListIDInfo[uiID]].siRPosX);
					pEAXSound->SetPositionY(pEAXSound->m_plpDS3DStreamBuffer[uiStreamPlayListIDInfo[uiID]-1], (float)SoundList[uiSoundListIDInfo[uiID]].siRPosY);
				}
				else
				{
					SI64 siVolume;

					if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
						siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
					else
						siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

					if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
						siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
					else
						siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

					SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
					SetSoundVolume(uiID, 1000+siVolume-uiMaxStreamVolume);
				}

				pEAXSound->SetStreamBufferPan(uiStreamPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siPan);
				pEAXSound->SetStreamBufferVolume(uiStreamPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siVolume);
			}
		}
	}
#endif
}

/*
	��� ������ ������ �����Ѵ�
*/
void SoundManager::SetAllSoundVolume(UI16 uiMaxVolume)
{
	SetAllStaticSoundVolume(uiMaxVolume);
	SetAllStreamSoundVolume(uiMaxVolume);
}

/*
	���ͺ信�� Listener��ġ�� �����Ѵ�
*/
void SoundManager::SetListenerPosition_Quarter(SI64 siX, SI64 siY)
{
	// ���ΰ��� ��ġ�� 0, 0 �̾�� �߽�
	siListenerPosX = (siX - siY)/2;
	siListenerPosY = (siX + siY)/2;

	for(UI16 i=1; i<MAX_DEFAULT_SOUNDLIST; i++)
	{
		if(SoundList[i].bStatic)
		{
			SoundList[i].siRPosX = SoundList[i].siPosX - siListenerPosX;
			SoundList[i].siRPosY = SoundList[i].siPosY - siListenerPosY;
		}
	}
}

/*
	���ͺ信�� Character ��ġ�� �����Ѵ�
*/
void SoundManager::SetCharPosition_Quarter(UI16 uiID, SI64 siX, SI64 siY)
{
	if(uiID <= 0) return;

	SI16 siPosX, siPosY;

	siPosX = (siX - siY)/2;
	siPosY = (siX + siY)/2;

	//SoundList[uiSoundListIDInfo[uiID]].siPosX = siPosX;
	//SoundList[uiSoundListIDInfo[uiID]].siPosY = siPosY;
	SoundList[uiID].siPosX = siPosX;
	SoundList[uiID].siPosY = siPosY;
}

/*
	ž�信�� Listener��ġ�� �����Ѵ�
*/
void SoundManager::SetListenerPosition_Top(SI64 siX, SI64 siY)
{
	// Listener�� ��ġ�� 0, 0 �̾�� �߽�
	siListenerPosX = (SI16)siX;
	siListenerPosY = (SI16)siY;
}

/*
	ž�信�� Character ��ġ�� �����Ѵ�
*/
void SoundManager::SetCharPosition_Top(UI16 uiID, SI64 siX, SI64 siY)
{
	if(uiID <= 0) return;

	SI16 siPosX, siPosY;

	siPosX = (SI16)siX;
	siPosY = (SI16)siY;

	SoundList[uiID].siPosX = siPosX;
	SoundList[uiID].siPosY = siPosY;

	SoundList[uiID].siRPosX = siPosX - siListenerPosX;
	SoundList[uiID].siRPosY = siPosY - siListenerPosY;
}

/*
	ID�� �ش��ϴ� ������ bStop���� �����Ѵ�
*/
BOOL SoundManager::IsStop(UI16 uiID)
{
	return SoundList[uiSoundListIDInfo[uiID]].bStop;
}

/*
	���尡 ���������� �˻��Ѵ�
*/
BOOL SoundManager::IsPlaying(UI16 uiID)
{
	if(!bSoundDriver)
	{
		// ���� ���ֵ� �ð��� üũ�Ѵ�
		DWORD time = (timeGetTime() - SoundList[uiSoundListIDInfo[uiID]].dwPlayTime);
		if(time <= SoundList[uiSoundListIDInfo[uiID]].dwEntireTime)
			return TRUE;

		return FALSE;
	}
	if(uiID <= 0) return FALSE;

	if(SoundList[uiSoundListIDInfo[uiID]].bStatic)
	{
		if(!bStaticPlayList[uiID])
			return FALSE;

		if(pEAXSound->IsBufferPlaying(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDSBuffer))
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(!bStreamPlayList[uiID])
			return FALSE;

		if(pEAXSound->IsBufferPlaying(pEAXSound->m_plpDSStreamBuffer[uiStreamPlayListIDInfo[uiID]-1]))
			return TRUE;
		else
			return FALSE;
	}
}

/*
	ID���带 �����Ѵ�
*/
void SoundManager::PlaySound(UI16 uiID)
{
	HANDLE hFile;

	if(uiID <= 0) return;
	if(!SoundList[uiID].bActive) return;
	if(SoundList[uiSoundListIDInfo[uiID]].pFileName == NULL) return;

	hFile = CreateFile((char*)SoundList[uiSoundListIDInfo[uiID]].pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE || hFile == 0)
		return;
	CloseHandle(hFile);

	//SoundList[uiSoundListIDInfo[uiID]].pFileName;
	//return;

	SI64 siVolume;
	UI08 uiEmptySlot = 1;
	BOOL bLoopInfinite = FALSE;
	float fNowTime;

	if(SoundList[uiSoundListIDInfo[uiID]].bStatic) // ȭ���� ����ƽ ������
	{
		if(siNStatic <= 0)
			return;

		if(bStaticPlayList[uiID]) // ����ƽ ���� ����Ʈ���� �ش� ȭ���� �ε�Ǿ����� �˻��Ѵ�
		{
			if(SoundList[uiSoundListIDInfo[uiID]].bStop)
			{
				pEAXSound->StopStaticBuffer(uiStaticPlayListIDInfo[uiID]-1, TRUE);
			}
			if(!pEAXSound->IsBufferPlaying(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDSBuffer))
			{ // �÷��� ���� �ƴ϶��
				if(pEAXSound->m_bUsing3D)
				{
					pEAXSound->SetPositionX(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDS3DBuffer, 
						(float)SoundList[uiSoundListIDInfo[uiID]].siRPosX/100);
					pEAXSound->SetPositionY(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDS3DBuffer, 
						(float)SoundList[uiSoundListIDInfo[uiID]].siRPosY/100);
				}
				else
				{
					if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
						siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
					else
						siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

					if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
						siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
					else
						siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

					SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
					SetSoundVolume(uiID, 1000+siVolume-uiMaxStaticVolume);
				}
			}
			else
			{ // �÷��̰� �ȵǴ°��� ã�´�
				for(WORD i=0; i<pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].nwDuplicate; i++)
				{
					if(!pEAXSound->IsBufferPlaying(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_plpDSBufferDuplicate[i]))
					{
						if(pEAXSound->m_bUsing3D)
						{
							pEAXSound->SetPositionX(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_plpDS3DBufferDuplicate[i], 
								(float)SoundList[uiSoundListIDInfo[uiID]].siRPosX/100);
							pEAXSound->SetPositionY(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_plpDS3DBufferDuplicate[i], 
								(float)SoundList[uiSoundListIDInfo[uiID]].siRPosY/100);
						}
						else
						{
							if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
								siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
							else
								siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

							if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
								siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
							else
								siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

							SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
							SetSoundVolume(uiID, 1000+siVolume-uiMaxStaticVolume);
						}
						break;
					}
				}
			}

			pEAXSound->SetStaticBufferPan(uiStaticPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siPan);
			pEAXSound->SetStaticBufferVolume(uiStaticPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siVolume);
			if(pEAXSound->m_bEAX)
				pEAXSound->SetEnvironment(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpksPropertySet, 
					SoundList[uiSoundListIDInfo[uiID]].siEAX);

			// ���� �������������� ������ �˻�
			fNowTime = DXUtil_Timer(TIMER_GETAPPTIME);
			if(fNowTime - SoundList[uiSoundListIDInfo[uiID]].fDelay > SoundList[uiSoundListIDInfo[uiID]].fDuplicateDelay)
			{
				SoundList[uiSoundListIDInfo[uiID]].fDelay = fNowTime;
				PlayStaticSound(uiStaticPlayListIDInfo[uiID], SoundList[uiSoundListIDInfo[uiID]].bLoop);
				SoundList[uiSoundListIDInfo[uiID]].dwPlayTime = timeGetTime(); // ������ �ð��� ���
			}
		}
		else // �ε尡 �ȵ��ִٸ� �� ���Կ� ������ �ε��Ѵ�
		{
			// ���� 1������ �÷��̵ǰ� ���� �ʴ� ���� ã�´�
			uiEmptySlot = 1;
			bLoopInfinite = FALSE;
			while(1)
			{
				if(bLoopInfinite)  // ���ѷ����� �ݺ��ȴٸ�
					return;
				if(bSoundDriver)
				{
					if(!pEAXSound->IsBufferPlaying(pEAXSound->m_pDSStaticBuffer[uiEmptySlot-1].m_lpDSBuffer))
						break;
				}
				else
				{
					if((timeGetTime() - SoundList[uiID].dwPlayTime) > SoundList[uiSoundListIDInfo[uiID]].dwEntireTime)
						break;
				}
				uiEmptySlot++;
				if(uiEmptySlot > siNStatic) { uiEmptySlot = 1; bLoopInfinite = TRUE; }
			};

			// ������ uiEmptySlot�� ���Կ� ����ϰ� �ִ� ���� ����
			bStaticPlayList[uiStaticSlotInfo[uiEmptySlot]] = FALSE;
			uiStaticPlayListIDInfo[uiStaticSlotInfo[uiEmptySlot]] = 0;
			SoundList[uiSoundListIDInfo[uiID]].dwEntireTime = 0;

			uiStaticNum = uiEmptySlot;

			bStaticPlayList[uiID] = TRUE; // �ε�Ǿ��ٰ� ����

			uiStaticPlayListIDInfo[uiID] = uiStaticNum; // ������ �ε�� ���� ���

			uiStaticSlotInfo[uiStaticNum] = uiID; // �ش� ���Կ� � ���̵� ���Ǿ����� ����

			LoadStaticSound(uiStaticNum, SoundList[uiSoundListIDInfo[uiID]].pFileName, SoundList[uiSoundListIDInfo[uiID]].nDuplicate);

			uiStaticNum++;

			if(pEAXSound->m_bUsing3D)
			{
				pEAXSound->SetPositionX(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDS3DBuffer, 
					(float)SoundList[uiSoundListIDInfo[uiID]].siRPosX/100);
				pEAXSound->SetPositionY(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpDS3DBuffer, 
					(float)SoundList[uiSoundListIDInfo[uiID]].siRPosY/100);
			}
			else
			{
				if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
					siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
				else
					siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

				if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
					siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
				else
					siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

				SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
				SetSoundVolume(uiID, 1000+siVolume-uiMaxStaticVolume);
			}

			pEAXSound->SetStaticBufferPan(uiStaticPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siPan);
			pEAXSound->SetStaticBufferVolume(uiStaticPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siVolume);
			if(pEAXSound->m_bEAX)
				pEAXSound->SetEnvironment(pEAXSound->m_pDSStaticBuffer[uiStaticPlayListIDInfo[uiID]-1].m_lpksPropertySet, 
					SoundList[uiSoundListIDInfo[uiID]].siEAX);

			fNowTime = DXUtil_Timer(TIMER_GETAPPTIME);
			if(fNowTime - SoundList[uiSoundListIDInfo[uiID]].fDelay > SoundList[uiSoundListIDInfo[uiID]].fDuplicateDelay)
			{
				SoundList[uiSoundListIDInfo[uiID]].fDelay = fNowTime;
				PlayStaticSound(uiStaticPlayListIDInfo[uiID], SoundList[uiSoundListIDInfo[uiID]].bLoop); // Excel������ ������ loop���� ����
				SoundList[uiSoundListIDInfo[uiID]].dwPlayTime = timeGetTime(); // ������ �ð��� ���
			}
		}
	}

	else
	{
		if(siNStream <= 0)
			return;

		if(bStreamPlayList[uiID]) // ��Ʈ�� �÷��� ����Ʈ���� �ش� ȭ���� �ε�Ǿ����� �˻��Ѵ�
		{
			if(pEAXSound->m_bUsing3D) // 3D ����� �����ô� Position�� �������ش�
			{
				pEAXSound->SetPositionX(pEAXSound->m_plpDS3DStreamBuffer[uiStreamPlayListIDInfo[uiID]-1], 
					(float)SoundList[uiSoundListIDInfo[uiID]].siRPosX);
				pEAXSound->SetPositionY(pEAXSound->m_plpDS3DStreamBuffer[uiStreamPlayListIDInfo[uiID]-1], 
					(float)SoundList[uiSoundListIDInfo[uiID]].siRPosY);
			}
			else
			{
				if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
					siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
				else
					siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

				if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
					siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
				else
					siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

				SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
				SetSoundVolume(uiID, 1000+siVolume-uiMaxStreamVolume);
			}
			pEAXSound->SetStreamBufferPan(uiStreamPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siPan);
			pEAXSound->SetStreamBufferVolume(uiStreamPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siVolume);
			PlayStreamSound(uiStreamPlayListIDInfo[uiID], SoundList[uiSoundListIDInfo[uiID]].bLoop); // Excel�� �⺻���� ���
			SoundList[uiSoundListIDInfo[uiID]].dwPlayTime = timeGetTime(); // ������ �ð��� ���
		}
		else // �ε尡 �ȵ��ִٸ� �� ���Կ� ������ �ε��Ѵ�
		{
			uiEmptySlot = 1;
			bLoopInfinite = FALSE;
			while(1)
			{
				if(bLoopInfinite)  // ���ѷ����� �ݺ��ȴٸ� �׳� ���Ͻ��� ������.
					return; 
				if(bSoundDriver)
				{
					if(!pEAXSound->IsBufferPlaying(pEAXSound->m_plpDSStreamBuffer[uiEmptySlot-1]))
						break;
				}
				else
				{
					if((timeGetTime() - SoundList[uiID].dwPlayTime) > SoundList[uiSoundListIDInfo[uiID]].dwEntireTime)
						break;
				}
				uiEmptySlot++;
				if(uiEmptySlot > siNStream) { uiEmptySlot = 1; bLoopInfinite = TRUE; }
			};

			// ������ ������ ������ �ִ� ���� ����
			bStreamPlayList[uiStreamSlotInfo[uiEmptySlot]] = FALSE;
			uiStreamPlayListIDInfo[uiStreamSlotInfo[uiEmptySlot]] = 0;
			SoundList[uiSoundListIDInfo[uiID]].dwEntireTime = 0;

			uiStreamNum = uiEmptySlot;

			bStreamPlayList[uiID] = TRUE; // �ε�Ǿ��ٰ� ����

			uiStreamPlayListIDInfo[uiID] = uiStreamNum; // ������ �ε�� ���� ���
			
			uiStreamSlotInfo[uiStreamNum] = uiID; // �ش� ���Կ� � ���̵� ���Ǿ����� ����

			LoadStreamSound(uiStreamNum, SoundList[uiSoundListIDInfo[uiID]].pFileName, SoundList[uiSoundListIDInfo[uiID]].bLoop);

			uiStreamNum++;

			if(pEAXSound->m_bUsing3D)
			{
				pEAXSound->SetPositionX(pEAXSound->m_plpDS3DStreamBuffer[uiStreamPlayListIDInfo[uiID]-1], 
					(float)SoundList[uiSoundListIDInfo[uiID]].siRPosX);
				pEAXSound->SetPositionY(pEAXSound->m_plpDS3DStreamBuffer[uiStreamPlayListIDInfo[uiID]-1], 
					(float)SoundList[uiSoundListIDInfo[uiID]].siRPosY);
			}
			else
			{
				if(SoundList[uiSoundListIDInfo[uiID]].siRPosY > 0)
					siVolume = -SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;
				else
					siVolume =  SoundList[uiSoundListIDInfo[uiID]].siRPosY*(SI64)siVolumeY;

				if(SoundList[uiSoundListIDInfo[uiID]].siRPosX > 0)
					siVolume -= SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;
				else
					siVolume += SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siVolumeX;

				SetSoundPan(uiID, 50+SoundList[uiSoundListIDInfo[uiID]].siRPosX*(SI64)siPan);
				SetSoundVolume(uiID, 1000+siVolume-uiMaxStreamVolume);
			}
			pEAXSound->SetStreamBufferPan(uiStreamPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siPan);
			pEAXSound->SetStreamBufferVolume(uiStreamPlayListIDInfo[uiID]-1, SoundList[uiSoundListIDInfo[uiID]].siVolume);
			PlayStreamSound(uiStreamPlayListIDInfo[uiID], SoundList[uiSoundListIDInfo[uiID]].bLoop); // Excel�� �⺻���� ���
			SoundList[uiSoundListIDInfo[uiID]].dwPlayTime = timeGetTime(); // ������ �ð��� ���
		}
	}
}

/*
	����ִ� Static Slot�� Sound�� �ε��Ѵ�.
	Parameter : Sound file Slot, wave filename, duplicate�� ���� ��
*/
void SoundManager::LoadStaticSound(UI16 uiSlot, UI08 *pFileName, UI08 uiDuplicate)
{
	if(!bSoundDriver)
	{
		//char debug[1024];
		pEAXSound->CreateStaticBuffer(uiSlot-1, (LPSTR)pFileName, uiDuplicate);
		//wsprintf(debug, "%s\n", pFileName);
		//OutputDebugString(debug);
		SoundList[uiStaticSlotInfo[uiSlot]].dwEntireTime = pEAXSound->m_pStaticWaveData[uiSlot-1].m_dwEntireTime;
		return;
	}

	pEAXSound->CreateStaticBuffer(uiSlot-1, (LPSTR)pFileName, uiDuplicate);
}

/*
	����ִ� Stream Slot�� Sound�� �ε��Ѵ�.
	Parameter : Sound file Slot, wave filename, loop
*/
void SoundManager::LoadStreamSound(UI16 uiSlot, UI08 *pFileName, BOOL bLoop)
{
	if(!bSoundDriver)
	{
		pEAXSound->CreateStreamBuffer(uiSlot-1, (LPSTR)pFileName, bLoop);
		SoundList[uiStreamSlotInfo[uiSlot]].dwEntireTime = pEAXSound->m_pStreamWaveData[uiSlot-1].m_dwEntireTime;
		return;
	}

	pEAXSound->CreateStreamBuffer(uiSlot-1, (LPSTR)pFileName, bLoop);
}

/*
	�ش� Static ID�� �����Ѵ�
	Parameter : Sound file Slot, ���� ���� �÷���
*/
void SoundManager::PlayStaticSound(UI16 uiSlot, BOOL bLoop)
{
	if(!bSoundDriver) return;

	pEAXSound->PlayStaticBuffer(uiSlot-1, bLoop);
}

/*
	�ش� Stream ID�� �����Ѵ�
	Parameter : Sound file Slot, ���� ���� �÷���
*/
void SoundManager::PlayStreamSound(UI16 uiSlot, BOOL bLoop)
{
	if(!bSoundDriver) return;

	pEAXSound->PlayStreamBuffer(uiSlot-1, bLoop);
}

/*
	�ش� Static ID ���ָ� ������Ų��
	Parameter : Sound file Slot, ���� ���ֽ� ���� �κк��� �÷��� �Ұ����ΰ��� �÷���
*/
void SoundManager::StopStaticSound(UI16 uiSlot, BOOL fPosition)
{
	if(!bSoundDriver) return;

	uiStaticSlotInfo[uiSlot] = 0;
	pEAXSound->StopStaticBuffer(uiSlot-1, fPosition);
}

/*
	�ش� Stream ID ���ָ� ������Ų��
	Parameter : Sound file Slot
*/
void SoundManager::StopStreamSound(UI16 uiSlot)
{
	if(!bSoundDriver) return;

	uiStreamSlotInfo[uiSlot] = 0;
	pEAXSound->StopStreamBuffer(uiSlot-1);
}

/*
	Static ID ũ�⸦ �����Ѵ�
	Parameter : Sound file Slot, Volume(0 ~ 1000)
*/
void SoundManager::SetSoundVolume(SI16 siID, long siVolume1000)
{
	long siVolume;

	if(siVolume1000 < 0) siVolume1000 = 0;
	if(siVolume1000 > 1000) siVolume1000 = 1000;
	
	siVolume = -10000 + siVolume1000*10;

	SoundList[uiSoundListIDInfo[siID]].siVolume = siVolume;
}

/*
	Static ID �� ����
	Parameter : Sound file Slot, Pan(0 ~ 100)
*/
void SoundManager::SetSoundPan(UI16 uiID, long lPan100)
{
	long lPan;

	if(lPan100 < 0) lPan100 = 0;
	if(lPan100 > 100) lPan100 = 100;
	
	lPan = ((lPan100-50)*200)/5;

	SoundList[uiSoundListIDInfo[uiID]].siPan = lPan;
}

/*

*/
void SoundManager::SetEAXEnvironment(UI16 uiID, DWORD dwValue) // 0~26
{
	if(!bSoundDriver) return;

	if(SoundList[uiSoundListIDInfo[uiID]].bStatic == TRUE && pEAXSound->m_bEAX == TRUE)
		SoundList[uiSoundListIDInfo[uiID]].siEAX = dwValue;
}

//-----------------------------------------------------------------------------
// Name: DXUtil_Timer()
// Desc: Performs timer opertations. Use the following commands:
//          TIMER_RESET           - to reset the timer
//          TIMER_START           - to start the timer
//          TIMER_STOP            - to stop (or pause) the timer
//          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//          TIMER_GETABSOLUTETIME - to get the absolute system time
//          TIMER_GETAPPTIME      - to get the current time
//          TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
//                                  TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------
float SoundManager::DXUtil_Timer(SoundManager::TIMER_COMMAND command)
{
    static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF         = FALSE;
    static LONGLONG m_llQPFTicksPerSec  = 0;

    // Initialize the timer
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
        // not supported, we will timeGetTime() which returns milliseconds.
        LARGE_INTEGER qwTicksPerSec;
        m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
        if( m_bUsingQPF )
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if( m_bUsingQPF )
    {
        static LONGLONG m_llStopTime        = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime        = 0;
        double fTime;
        double fElapsedTime;
        LARGE_INTEGER qwTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {
            fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
            m_llLastElapsedTime = qwTime.QuadPart;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
            return (FLOAT) fAppTime;
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_llBaseTime        = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            m_llBaseTime += qwTime.QuadPart - m_llStopTime;
            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            m_llStopTime = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_llStopTime += m_llQPFTicksPerSec/10;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
    else
    {
        // Get the time using timeGetTime()
        static double m_fLastElapsedTime  = 0.0;
        static double m_fBaseTime         = 0.0;
        static double m_fStopTime         = 0.0;
        double fTime;
        double fElapsedTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            fTime = m_fStopTime;
        else
            fTime = timeGetTime() * 0.001;
    
        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {   
            fElapsedTime = (double) (fTime - m_fLastElapsedTime);
            m_fLastElapsedTime = fTime;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            return (FLOAT) (fTime - m_fBaseTime);
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_fBaseTime         = fTime;
            m_fLastElapsedTime  = fTime;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            m_fBaseTime += fTime - m_fStopTime;
            m_fStopTime = 0.0f;
            m_fLastElapsedTime  = fTime;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            m_fStopTime = fTime;
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_fStopTime += 0.1f;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
}

