//--------------------------------------------------------------------
//  ��� : ���°�
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
extern _Char   CR[];       // ĳ������ ������ 

//--------------------------------------------------------------------------
// Name: Init()
// Desc: config ������ ���� ���� �ʾƼ� �⺻���� �����Ѵ�.
//--------------------------------------------------------------------------
void _Config::Init()
{
	SHORT i,j;

	ZeroMemory(this, sizeof(_Config));     // Class�������� �ʱ�ȭ�Ѵ�.

	m_uiInterfaceMode         = INTERFACEMODE_2BUTTON;
	m_uiShortCutKeyShowSwitch = TRUE;          // ����Ű ��ư�� ǥ���ϴ��� ���θ� �˷��ִ� ���� 

	m_siGameSpeed   = 2;						// ���� �ӵ��� 2������ 
	m_siEffectSoundVolume = 80;					// Direct Sound�� 80%�� 
	m_siBackgroundMusicVolume = 80;				// ��� ���� ����

	m_uiCDMusicSwitch     = TRUE;              // CD Music�� ���ְ���
	m_uiPrevCDMusicSwitch = TRUE;              // ���� CD ���� ���� TRUE


	m_siGamma       = 10;                      // Gamma �� 10 ���� 

	// �� ������ �ʱ�ȭ�Ѵ�. 
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
// Desc: ���� �ӵ��� �����Ѵ�. 
//--------------------------------------------------------------------------
void _Config::SetGameSpeed(int gamespeed)
{
	m_siGameSpeed=gamespeed;                   // ���� �ӵ� ���� �����ϰ� 

	FrameDelay=GetRealGameSpeedClock();    // Frame Delay���� �����Ѵ�.
}

//--------------------------------------------------------------------------
// Name: GetRealGameSpeedClock()
// Desc: ���� �����Ӵ� Ŭ������ ���Ѵ�. 
//--------------------------------------------------------------------------
int _Config::GetRealGameSpeedClock()
{
	if( GD.GetOnlineSpeedSwitch() == TRUE )
	{
		return 60;
	}
	// ��Ƽ������ �ӵ��� ������ �ʴ´�.
	else 
	{
		return MultiPlayDelayTime;
	}
}

//--------------------------------------------------------------------------
// Name: SetEffectSoundVolume()
// Desc: Effect Sound Volume�� �����Ѵ�.
//--------------------------------------------------------------------------
void _Config::SetEffectSoundVolume(int volume)
{
	m_siEffectSoundVolume = volume;
	g_pSoundManager->SetAllStaticSoundVolume((UI16)(volume * 10));    // Direct Sound�� Volume�� �����Ѵ�.
}

//--------------------------------------------------------------------------
// Name: SetBackgroundMusicVolume()
// Desc: ��� ���� ���� ����
//--------------------------------------------------------------------------
void _Config::SetBackgroundMusicVolume(int volume)
{
	m_siBackgroundMusicVolume = volume;
	g_pSoundManager->SetAllStreamSoundVolume((UI16)(volume * 10));    // Direct Sound�� Volume�� �����Ѵ�.
}

//--------------------------------------------------------------------------
// Name: Save() 
// Desc: Config File�� �����Ѵ�.
//--------------------------------------------------------------------------
BOOL _Config::Save()
{
	FILE* fp;
	char buffer[FILE_NAME_SIZE];

	// ��ο� �����̸��� �ռ��Ͽ� 
	GetFileNamePath("config.ln", SavePath, buffer);

	// config ������ ���� 
	fp=fopen(buffer, "wb");
	if(fp==NULL)	return FALSE;
	
	// �����Ѵ�.
	fwrite(this, sizeof(_Config), 1, fp);

	fclose(fp);

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: Load()
// Desc: Config.hq ������ �о� ���� �����Ѵ�.
//--------------------------------------------------------------------------
BOOL _Config::Load()
{
	FILE* fp;
	char buffer[FILE_NAME_SIZE];

	// ��θ� �������� 
	GetFileNamePath("config.ln", SavePath, buffer);

	// ������ ���� 
	fp=fopen(buffer, "rb");
	if(fp==NULL)	return FALSE;

	// Class�� ����� ������ ũ�⸸ŭ �о�´�.
	SI32 data = fread(this, 1, sizeof(_Config), fp);
	
	if(data != sizeof(_Config))
	{
		clGrp.Error("Config Error", "Not Proper Config File.");
		Init();
	}

	// ������ �ݴ´�.
	fclose(fp);

	// ������ ���� �����Ѵ�.
	GammaFunction(m_siGamma);

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: GammaFunction()
// Desc: ������ ���� �����Ѵ�.
//--------------------------------------------------------------------------
BOOL _Config::GammaFunction(SHORT mount)
{

	// ������ ������ ���� �ʰ� �Ѵ�. 

	SHORT i;
	SHORT temp_pal[768];

	// Original Palette ���� �������� 
	for(i=0;i<768;i++)
	{
		temp_pal[i]=pGame->OriginalGamePalette[i];
	}


	// �־��� ����ŭ �ȷ�Ʈ�� ���� �����Ѵ�.
	m_siGamma = mount;

	// �ȷ�Ʈ ���� 
	for(i=0;i<256;i++)
	{
		clGrp.Palette[i * 3]     = min(63, max(0, temp_pal[i*3]+(m_siGamma-10)*1)    );
		
		clGrp.Palette[i * 3 + 1] = min(63, max(0, temp_pal[i*3+1]+(m_siGamma-10)*1)  );
		
		clGrp.Palette[i * 3 + 2] = min(63, max(0, temp_pal[i*3+2]+(m_siGamma-10)*1)  );
	}

	// ������ �ȷ�Ʈ�� �����ȷ�Ʈ�� �����Ѵ�.
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

	// �� ������ �ʱ�ȭ�Ѵ�. 
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