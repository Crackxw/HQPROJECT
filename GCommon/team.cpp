//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <clgame.h>
#include <main.h>
#include <MapInterface.h>
#include "..\Gersang\maindata.h"
#include <etc.h>
#include <item.h>
#include "..\Gersang\music.h"
#include <CharOrg.h>
#include <charinterface.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	_NewID 				ZeroID;
extern _clGame		*pGame;


void _Team::Init()
{
	int i=0, j=0;

	CurrentTeamNumber=0;

	for(i=0;i<MAX_TEAM_NUMBER;i++)
	{
    	Init(i);
	}
}


void _Team::Init(SHORT teamnumber)
{

	SHORT j;

	CharNumber[teamnumber]=0;
	CenterX[teamnumber]=0;
	CenterY[teamnumber]=0;

   for(j=0;j<MAX_SELECTED_CHAR_NUMBER;j++)
	  ID[teamnumber][j].Init();
}

// 팀을 깨끗이 비운다.
void _Team::DeleteTeamAll(SHORT teamnumber)
{
	SHORT i;

	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		ID[teamnumber][i].Init();
	}
	CharNumber[teamnumber]=0;


}

// id를 팀에서 제거한다. 
void _Team::DeleteTeam(_NewID id)
{

	int i, j, k;

	if(GD.CharInfo[id.ID]==0)return ;


	for(i=0;i<MAX_TEAM_NUMBER;i++)
	{
	   for(j=0;j<MAX_SELECTED_CHAR_NUMBER;j++)
	   {
			   if(ID[i][j].IsSame(id))
			   {
                   ID[i][j].Init();
				   clifChar.SetGroup(id.ID, -1);
				   CharNumber[i]--;

				   // 팀원이 모두 소멸하면, 
				   // 팀을 초기화시킨다. 
				   if(CharNumber[i]==0)
				   {
					   Init(i);
				   }
				   else
				   {

					   // 아이디가 순차적으로 배열되도록 만든다. 
				       for(k=j;k<(MAX_SELECTED_CHAR_NUMBER-1);k++)
					   {
					       ID[i][k]=ID[i][k+1];
					   }

				       ID[i][MAX_SELECTED_CHAR_NUMBER-1].Init();

				       GetCenter(i, CenterX[i], CenterY[i]);
				   }
			   }
	   }
	}

}

// 그룹으로 만든다. 
void _Team::AddTeam(SI32 number, _NewID id)
{

	SI32 i;

	if(IsAlive(id.ID)==FALSE)return ;

	// 이미 있으면 또 넣을 필요가 없다. 
	/*for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
       if(ID[number][i].IsSame(id))return ;
	}*/

	// 빈곳을 찾아 넣는다.
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		if(ID[number][i].IsEmpty())
		{
			ID[number][i]=id;
			CharNumber[number]++;
			
			GetCenter(number, CenterX[number], CenterY[number]);
		    clifChar.SetGroup(id.ID, number);
			return ;
		}
	}

}

// 그룹으로 만든다. 
void _Team::AddTeam(SI32 number)
{
	int i, k;

	// 기존 부대에 있던 멤버들은 지운다. 
   for(k=0;k<MAX_SELECTED_CHAR_NUMBER;k++)
   {
		if(ID[number][k].IsEmpty()==FALSE)
		 {
		   if(GD.CharInfo[ ID[number][k].ID ])
		   {
		      clifChar.SetGroup(ID[number][k].ID, -1);
		   }
		   else 
		   {
			   SetMessage("125076");
		   }
           ID[number][k].Init();
		   CharNumber[number]--;
		 }
   }


   // 이미 다른 부대에 있는 것은 아닌지 검사한다. 
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		if(GD.CharInfo[ GD.SelectedID[i].ID ]!=0)
		{

			/*// 중복된 것은 제거한다. 
			for(j=0;j<MAX_TEAM_NUMBER;j++)
			{
			   for(k=0;k<MAX_SELECTED_CHAR_NUMBER;k++)
			   {
				   if(ID[j][k].IsSame(GD.SelectedID[i]))
				   {
                      ID[j][k].Init();
					  CharNumber[j]--;
					  clifChar.SetGroup(GD.SelectedID[i].ID, -1);
				   }
			   }
			}*/

		    ID[number][i].Init();
			ID[number][i]=GD.SelectedID[i];
			CharNumber[number]++;
			clifChar.SetGroup(GD.SelectedID[i].ID, number);

		}

	}
	
}

// 그룹을 선택한다.
void _Team::SelectTeam(SHORT number)
{
	int i;
	int index=0;

	if(GD.SelectedTeam==number)return ;
	
	// 현재 선택된 캐릭터를 제거한다.
	Unselect();
	ClearMenu();

	GD.SelectedTeam=number;

    for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
	   // 살아있고 
	   // 아군이면, 
	   if(IsAlive( ID[number][i].ID ) 
	   && clifChar.GetPlayerNumber(ID[number][i].ID) == UserPlayerNumber)
	   {
		   // 보이는 상태라면, 
		   if(clifChar.GetSeeSwitch(ID[number][i].ID) == TRUE)
		   {
			   // 선택하게 만든다. 
	          clifChar.SetSelect( ID[number][i].ID, TRUE, FALSE);
		      index++;
		   }
	   }
	   // 무효하면, 
	   else
		   ID[number][i].Init();
	}

	// 선택할 캐릭터가 없는 상태라면 효과음을 연주하지 않는다. 
	if(index==0)return ;


#ifndef _ENGLISH
	if(pGame->LanguageCode == ELANGAGECODE_CHINA)
	{
		switch(number)
		{
		case 1:
			PushEffect(EFFECT_TEAM1_C, 0, 0);
			break;
		case 2:
			PushEffect(EFFECT_TEAM2_C, 0, 0);
			break;
		case 3:
			PushEffect(EFFECT_TEAM3_C, 0, 0);
			break;
		case 4:
			PushEffect(EFFECT_TEAM4_C, 0, 0);
			break;
		case 5:
			PushEffect(EFFECT_TEAM5_C, 0, 0);
			break;
		case 6:
			PushEffect(EFFECT_TEAM6_C, 0, 0);
			break;
		case 7:
			PushEffect(EFFECT_TEAM7_C, 0, 0);
			break;
		case 8:
			PushEffect(EFFECT_TEAM8_C, 0, 0);
			break;
		case 9:
			PushEffect(EFFECT_TEAM9_C, 0, 0);
			break;
		default:
			PushEffect(EFFECT_TEAM10_C, 0, 0);
			break;
		}

	}
	else
	{
		switch(number)
		{
		case 1:
			PushEffect(EFFECT_TEAM1, 0, 0);
			break;
		case 2:
			PushEffect(EFFECT_TEAM2, 0, 0);
			break;
		case 3:
			PushEffect(EFFECT_TEAM3, 0, 0);
			break;
		case 4:
			PushEffect(EFFECT_TEAM4, 0, 0);
			break;
		case 5:
			PushEffect(EFFECT_TEAM5, 0, 0);
			break;
		case 6:
			PushEffect(EFFECT_TEAM6, 0, 0);
			break;
		case 7:
			PushEffect(EFFECT_TEAM7, 0, 0);
			break;
		case 8:
			PushEffect(EFFECT_TEAM8, 0, 0);
			break;
		case 9:
			PushEffect(EFFECT_TEAM9, 0, 0);
			break;
		default:
			PushEffect(EFFECT_TEAM0, 0, 0);
			break;
		}
	}
#endif
}

BOOL _Team::GetCenter(SHORT number, SHORT&x, SHORT &y)
{
	int i;
	int index=0;

	x=0;
	y=0;

    for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
	   if(IsAlive( ID[number][i].ID ) 
		&& clifChar.GetSeeSwitch(ID[number][i].ID) == TRUE)
	   {
		   x += clifChar.GetX(ID[number][i].ID);
		   y += clifChar.GetY(ID[number][i].ID);
		   index++;
	   }
	 }

	if(index==0)return FALSE;

	x/=index;
	y/=index;

	return TRUE;

}

// 그룹 있는 곳으로 이동한다.
void _Team::GotoTeam(SHORT number)
{

	// 선택할 캐릭터가 없는 상태라면 효과음을 연주하지 않는다. 
	if(GetCenter(number, CenterX[number], CenterY[number])==FALSE)return ;

   // 화면을 옮긴다. 
	clifMap.SetVirtualPos(CenterX[number], CenterY[number]);
	clifMap.SetDrawForceSwitch(TRUE);

}


// 팀원의 수를 구한다. 
SHORT _Team::GetTeamMemberCount(SHORT number)
{
	SHORT i;
	SHORT count=0;

    for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
	   if(ID[number][i].IsEmpty()==FALSE)
	   {
		   count++;
	   }
	}

	return count;
}

