//--------------------------------------------------------------------
//  ��� : ���°�
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

extern	_MainData			GD;									// ������ �ֵ����� 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
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

// ���� ������ ����.
void _Team::DeleteTeamAll(SHORT teamnumber)
{
	SHORT i;

	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		ID[teamnumber][i].Init();
	}
	CharNumber[teamnumber]=0;


}

// id�� ������ �����Ѵ�. 
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

				   // ������ ��� �Ҹ��ϸ�, 
				   // ���� �ʱ�ȭ��Ų��. 
				   if(CharNumber[i]==0)
				   {
					   Init(i);
				   }
				   else
				   {

					   // ���̵� ���������� �迭�ǵ��� �����. 
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

// �׷����� �����. 
void _Team::AddTeam(SI32 number, _NewID id)
{

	SI32 i;

	if(IsAlive(id.ID)==FALSE)return ;

	// �̹� ������ �� ���� �ʿ䰡 ����. 
	/*for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
       if(ID[number][i].IsSame(id))return ;
	}*/

	// ����� ã�� �ִ´�.
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

// �׷����� �����. 
void _Team::AddTeam(SI32 number)
{
	int i, k;

	// ���� �δ뿡 �ִ� ������� �����. 
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


   // �̹� �ٸ� �δ뿡 �ִ� ���� �ƴ��� �˻��Ѵ�. 
	for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
		if(GD.CharInfo[ GD.SelectedID[i].ID ]!=0)
		{

			/*// �ߺ��� ���� �����Ѵ�. 
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

// �׷��� �����Ѵ�.
void _Team::SelectTeam(SHORT number)
{
	int i;
	int index=0;

	if(GD.SelectedTeam==number)return ;
	
	// ���� ���õ� ĳ���͸� �����Ѵ�.
	Unselect();
	ClearMenu();

	GD.SelectedTeam=number;

    for(i=0;i<MAX_SELECTED_CHAR_NUMBER;i++)
	{
	   // ����ְ� 
	   // �Ʊ��̸�, 
	   if(IsAlive( ID[number][i].ID ) 
	   && clifChar.GetPlayerNumber(ID[number][i].ID) == UserPlayerNumber)
	   {
		   // ���̴� ���¶��, 
		   if(clifChar.GetSeeSwitch(ID[number][i].ID) == TRUE)
		   {
			   // �����ϰ� �����. 
	          clifChar.SetSelect( ID[number][i].ID, TRUE, FALSE);
		      index++;
		   }
	   }
	   // ��ȿ�ϸ�, 
	   else
		   ID[number][i].Init();
	}

	// ������ ĳ���Ͱ� ���� ���¶�� ȿ������ �������� �ʴ´�. 
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

// �׷� �ִ� ������ �̵��Ѵ�.
void _Team::GotoTeam(SHORT number)
{

	// ������ ĳ���Ͱ� ���� ���¶�� ȿ������ �������� �ʴ´�. 
	if(GetCenter(number, CenterX[number], CenterY[number])==FALSE)return ;

   // ȭ���� �ű��. 
	clifMap.SetVirtualPos(CenterX[number], CenterY[number]);
	clifMap.SetDrawForceSwitch(TRUE);

}


// ������ ���� ���Ѵ�. 
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

