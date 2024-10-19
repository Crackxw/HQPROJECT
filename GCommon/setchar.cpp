//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include "..\Gersang\maindata.h"
#include <CharOrg.h>

extern	_MainData	GD;
extern	_Char		CR[];
extern	_KindInfo	KI[];


// 빈 캐릭터 번호를 돌려준다. 
// 리턴값: 0           -> 빈 캐릭터번호가 없다. 
//         0 이외의 값 -> 빈 캐릭터 번호 
SI32 FindEmptyID(SI32 pnumber)
{
   int i;
   int max=0;
   int max_id=0;

   for(i = 1; i <= MAX_CHAR_NUMBER; i++)
   {
	   if(i >= MAX_CHAR_NUMBER)return 0;

	   if(GD.CharInfo[i]==0)
	   {
		   return i;
	   }
   }

   return 0;
}

// 한개의 캐릭터를 삭제한다. 
void DeleteChar(SHORT id)
{
	if(GD.CharInfo[id])
	{
		if(CR[id].GetPlayerNumber() == 0)
			int a = 0 ;
		// 유닛이 설정된 상태로 삭제한다면 죽인다.
 		if(CR[id].SetIDSwitch==TRUE)
		{
			CR[id].DeleteID();
		}

		// 선택되어 있다면 해제한다. 
		if(CR[id].GetSelectedSwitch()==TRUE)
	        CR[id].SetSelect(FALSE, FALSE);

		// 특정 그룹에 속해 있다면 제거한다.
		if(CR[id].Group>=0)
			GD.Player[CR[id].GetPlayerNumber()].Team.DeleteTeam(CR[id].m_NewID);
			//GD.Team.DeleteTeam(CR[id].NewID,CR[id].GetPlayerNumber());


		SHORT work_char_order=CR[id].CharOrderNumber;

		// 사망한 유닛의 자리에 맨 마지막 유닛을 넣는다.
		GD.CharOrder[ work_char_order ]=GD.CharOrder[ GD.CharOrderIndex-1 ];
		
		// 맨 마지막 유닛이 있던 자리를 초기화한다. 
		GD.CharOrder[ GD.CharOrderIndex-1 ]=0;

		// 자리를 옮긴 유닛의 순서 번호를 새로 갱신한다.
		CR[ GD.CharOrder[ work_char_order ] ].CharOrderNumber = work_char_order;

		GD.CharOrderIndex--;



		// 캐릭터 갯수를 줄인다. 
        GD.DecreaseCharNumber(CR[id].GetPlayerNumber(), CR[id].m_cltCharStatus.GetKind());

        if(CR[id].m_cltCharStatus.GetArea()==AREA_AIR)
		{
	      GD.DeleteAirCharID(CR[id].m_NewID);
		}
        else
		{
	      GD.DeleteLandCharID(CR[id].m_NewID);
		}
 
		GD.CharInfo[id]=0;


	}
}

// 모든 캐릭터를 삭제한다.
void DeleteChar()
{
	int i;

	for(i=0;i<MAX_CHAR_NUMBER;i++)
	{
		DeleteChar(i);
	}
}

// 일반 캐릭터를 설정한다. 
BOOL SetChar(SHORT kind, SHORT id, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid)
{
	SI32 file;
	SI32 i;


	// 유효하지 않은 종류의 캐릭터는 만들지 않는다. 
	if( KI[kind].IsKindAtb(KINDATB_HERO) == FALSE && KI[kind].m_clCharAnimation[1].siFile == 0 )
	{
		return FALSE;
	}

	// 캐릭터에 필요한 파일이 없다면 불러온다.
	for( i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		file = KI[kind].m_clCharAnimation[i].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
			{
				LoadImgFile(file);
			}
		}
	}

	GD.ProducedCharNumber++;
    CR[id].CharInit(kind, id, GD.ProducedCharNumber % 16, x, y, dir, makepercent, pnumber, pip, pmw, onlineid);

    return TRUE;
}


// robypark 2004/6/9 12:36
// 일반 캐릭터를 설정한다.
// 변신하는 캐릭터를 위한 함수 
// 변신하는 캐릭터는 캐릭터 종류(Kind)만 달라질 뿐, 전체적인 캐릭터 리스트 등에는 영향을 주어서는 안된다.
// SHORT CharOrderNumber: 유닛이 배열에서 어느 위치에 있는가?
BOOL SetChar(SHORT kind, _NewID idChar, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID/* = -1*/)
{
	SI32 file;
	SI32 i;


	// 유효하지 않은 종류의 캐릭터는 만들지 않는다. 
	if( KI[kind].IsKindAtb(KINDATB_HERO) == FALSE && KI[kind].m_clCharAnimation[1].siFile == 0 )
	{
		return FALSE;
	}

	// 캐릭터에 필요한 파일이 없다면 불러온다.
	for( i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		file = KI[kind].m_clCharAnimation[i].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
			{
				LoadImgFile(file);
			}
		}
	}

//	GD.ProducedCharNumber++;
	// 새로운 캐릭터를 생성(생산)하는 것이 아니라 현재의 캐릭터를 변신(캐릭터 종류)만 하도록 설정해야 한다.
    CR[idChar.ID].CharInit_DISGUISE(kind, idChar.ID, idChar.Unique, x, y, dir, makepercent, pnumber, pip, pmw, onlineid, usCharOrderNumber, siPrevWeaponID);

    return TRUE;
}


