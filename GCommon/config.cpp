//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "..\Gersang\maindata.h"
#include <etc.h>
#include <config.h>
#include <SoundManager.h>
#include "..\Gersang\directx.h"
#include <CharOrg.h>

extern	_clGame* pGame;

extern	_MainData		GD;
extern	int				MultiPlayDelayTime;
extern	int				CDStopSwitch;
extern	char			SavePath[];
extern	clock_t			FrameDelay;
extern	SoundManager	*g_pSoundManager;
extern SI32	UserPlayerNumber;
extern _Char   CR[];       // 캐릭터의 포인터 

//--------------------------------------------------------------------------
// Name: Init()
// Desc: config 파일이 존재 하지 않아서 기본값을 설정한다.
//--------------------------------------------------------------------------
void _Config::Init()
{
	SHORT i,j;

	ZeroMemory(this, sizeof(_Config));     // Class변수들을 초기화한다.

	m_uiInterfaceMode         = INTERFACEMODE_2BUTTON;
	m_uiShortCutKeyShowSwitch = TRUE;          // 단축키 버튼을 표시하는지 여부를 알려주는 변수 

	m_siGameSpeed   = 2;						// 게임 속도는 2번으로 
	m_siEffectSoundVolume = 80;					// Direct Sound는 80%로 
	m_siBackgroundMusicVolume = 80;				// 배경 음악 볼륨

	m_uiCDMusicSwitch     = TRUE;              // CD Music은 연주가능
	m_uiPrevCDMusicSwitch = TRUE;              // 예전 CD 연주 상태 TRUE


	m_siGamma       = 10;                      // Gamma 는 10 으로 

	// 팀 정보를 초기화한다. 
	for(j = 0; j < MAX_TEAM_NUMBER; j++)
	{
		for(i = 0;i<ON_MAX_FOLLOWER_NUM;i++)
		{
			m_ssTeamInfo[j][i] = -1;
		}
	}
}

//--------------------------------------------------------------------------
// Name: SetGameSpeed()
// Desc: 게임 속도를 결정한다. 
//--------------------------------------------------------------------------
void _Config::SetGameSpeed(int gamespeed)
{
	m_siGameSpeed=gamespeed;                   // 게임 속도 값을 설정하고 

	FrameDelay=GetRealGameSpeedClock();    // Frame Delay값을 설정한다.
}

//--------------------------------------------------------------------------
// Name: GetRealGameSpeedClock()
// Desc: 실제 프레임당 클럭수를 구한다. 
//--------------------------------------------------------------------------
int _Config::GetRealGameSpeedClock()
{
	if( GD.GetOnlineSpeedSwitch() == TRUE )
	{
		return 60;
	}
	// 멀티에서는 속도가 변하지 않는다.
	else 
	{
		return MultiPlayDelayTime;
	}
}

//--------------------------------------------------------------------------
// Name: SetEffectSoundVolume()
// Desc: Effect Sound Volume을 설정한다.
//--------------------------------------------------------------------------
void _Config::SetEffectSoundVolume(int volume)
{
	m_siEffectSoundVolume = volume;
	g_pSoundManager->SetAllStaticSoundVolume((UI16)(volume * 10));    // Direct Sound의 Volume을 설정한다.
}

//--------------------------------------------------------------------------
// Name: SetBackgroundMusicVolume()
// Desc: 배경 음악 볼륨 설정
//--------------------------------------------------------------------------
void _Config::SetBackgroundMusicVolume(int volume)
{
	m_siBackgroundMusicVolume = volume;
	g_pSoundManager->SetAllStreamSoundVolume((UI16)(volume * 10));    // Direct Sound의 Volume을 설정한다.
}

//--------------------------------------------------------------------------
// Name: Save() 
// Desc: Config File을 저장한다.
//--------------------------------------------------------------------------
BOOL _Config::Save()
{
	FILE* fp;
	char buffer[FILE_NAME_SIZE];

	// 경로와 파일이름을 합성하여 
	GetFileNamePath("config.ln", SavePath, buffer);

	// config 파일을 열어 
	fp=fopen(buffer, "wb");
	if(fp==NULL)	return FALSE;
	
	// 저장한다.
	fwrite(this, sizeof(_Config), 1, fp);

	fclose(fp);

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: Load()
// Desc: Config.hq 파일을 읽어 값을 설정한다.
//--------------------------------------------------------------------------
BOOL _Config::Load()
{
	FILE* fp;
	char buffer[FILE_NAME_SIZE];

	// 경로를 설정한후 
	GetFileNamePath("config.ln", SavePath, buffer);

	// 파일을 열어 
	fp=fopen(buffer, "rb");
	if(fp==NULL)	return FALSE;

	// Class에 선언된 변수의 크기만큼 읽어온다.
	SI32 data = fread(this, 1, sizeof(_Config), fp);
	
	if(data != sizeof(_Config))
	{
		clGrp.Error("Config Error", "Not Proper Config File.");
		Init();
	}

	// 파일을 닫는다.
	fclose(fp);

	// 감마의 양을 설정한다.
	GammaFunction(m_siGamma);

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: GammaFunction()
// Desc: 감마의 양을 설정한다.
//--------------------------------------------------------------------------
BOOL _Config::GammaFunction(SHORT mount)
{

	// 감마의 영향을 받지 않게 한다. 

	SHORT i;
	SHORT temp_pal[768];

	// Original Palette 값을 복사한후 
	for(i=0;i<768;i++)
	{
		temp_pal[i]=pGame->OriginalGamePalette[i];
	}


	// 주어진 값만큼 팔레트의 색을 변경한다.
	m_siGamma = mount;

	// 팔래트 설정 
	for(i=0;i<256;i++)
	{
		clGrp.Palette[i * 3]     = min(63, max(0, temp_pal[i*3]+(m_siGamma-10)*1)    );
		
		clGrp.Palette[i * 3 + 1] = min(63, max(0, temp_pal[i*3+1]+(m_siGamma-10)*1)  );
		
		clGrp.Palette[i * 3 + 2] = min(63, max(0, temp_pal[i*3+2]+(m_siGamma-10)*1)  );
	}

	// 설정된 팔래트로 게임팔래트를 설정한다.
	SetGamePalette(clGrp.Palette);


	return TRUE;

}


void _Config::SetTeamInfo(SI16 siTeam)
{

	SI32 i, j;
	_NewID newid;
	_NewID ZeroID;

	ZeroID.ID = -1;
	ZeroID.Unique = -1;

	// 팀 정보를 초기화한다. 
	for(i = 0;i<ON_MAX_FOLLOWER_NUM;i++)
	{
		m_ssTeamInfo[siTeam][i]		=	-1;
	}


	for(j =0;j<ON_MAX_FOLLOWER_NUM;j++)
	{
		newid = GD.Player[UserPlayerNumber].Team.ID[siTeam][j];

		if(IsAlive(newid) == TRUE
		&& CR[newid.ID].m_siOnlineID >=0)
		{
			m_ssTeamInfo[siTeam][ j ] = CR[newid.ID].m_siOnlineID;
		}
	}

}

void _Config::GetTeamInfo()
{	
	for(int j = 0 ; j < MAX_TEAM_NUMBER; j++)
	{
		for(int i = 0 ; i < ON_MAX_FOLLOWER_NUM; i++)
		{
			for(int k = 1 ; k < 200 ; k++)
			{
				if(m_ssTeamInfo[j][i] == -1)
					break;

				if(IsAlive(k) && CR[k].GetPlayerNumber() == UserPlayerNumber
				&& m_ssTeamInfo[j][i] >=0  && (m_ssTeamInfo[j][i] == CR[k].m_siOnlineID))
				{
					GD.Player[UserPlayerNumber].Team.AddTeam(j, CR[k].m_NewID);
					break;
				}
			}
		}
	}
}