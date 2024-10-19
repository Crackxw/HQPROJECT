//--------------------------------------------------------------------
//  ��� : ���°�
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


// �� ĳ���� ��ȣ�� �����ش�. 
// ���ϰ�: 0           -> �� ĳ���͹�ȣ�� ����. 
//         0 �̿��� �� -> �� ĳ���� ��ȣ 
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

// �Ѱ��� ĳ���͸� �����Ѵ�. 
void DeleteChar(SHORT id)
{
	if(GD.CharInfo[id])
	{
		if(CR[id].GetPlayerNumber() == 0)
			int a = 0 ;
		// ������ ������ ���·� �����Ѵٸ� ���δ�.
 		if(CR[id].SetIDSwitch==TRUE)
		{
			CR[id].DeleteID();
		}

		// ���õǾ� �ִٸ� �����Ѵ�. 
		if(CR[id].GetSelectedSwitch()==TRUE)
	        CR[id].SetSelect(FALSE, FALSE);

		// Ư�� �׷쿡 ���� �ִٸ� �����Ѵ�.
		if(CR[id].Group>=0)
			GD.Player[CR[id].GetPlayerNumber()].Team.DeleteTeam(CR[id].m_NewID);
			//GD.Team.DeleteTeam(CR[id].NewID,CR[id].GetPlayerNumber());


		SHORT work_char_order=CR[id].CharOrderNumber;

		// ����� ������ �ڸ��� �� ������ ������ �ִ´�.
		GD.CharOrder[ work_char_order ]=GD.CharOrder[ GD.CharOrderIndex-1 ];
		
		// �� ������ ������ �ִ� �ڸ��� �ʱ�ȭ�Ѵ�. 
		GD.CharOrder[ GD.CharOrderIndex-1 ]=0;

		// �ڸ��� �ű� ������ ���� ��ȣ�� ���� �����Ѵ�.
		CR[ GD.CharOrder[ work_char_order ] ].CharOrderNumber = work_char_order;

		GD.CharOrderIndex--;



		// ĳ���� ������ ���δ�. 
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

// ��� ĳ���͸� �����Ѵ�.
void DeleteChar()
{
	int i;

	for(i=0;i<MAX_CHAR_NUMBER;i++)
	{
		DeleteChar(i);
	}
}

// �Ϲ� ĳ���͸� �����Ѵ�. 
BOOL SetChar(SHORT kind, SHORT id, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid)
{
	SI32 file;
	SI32 i;


	// ��ȿ���� ���� ������ ĳ���ʹ� ������ �ʴ´�. 
	if( KI[kind].IsKindAtb(KINDATB_HERO) == FALSE && KI[kind].m_clCharAnimation[1].siFile == 0 )
	{
		return FALSE;
	}

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
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
// �Ϲ� ĳ���͸� �����Ѵ�.
// �����ϴ� ĳ���͸� ���� �Լ� 
// �����ϴ� ĳ���ʹ� ĳ���� ����(Kind)�� �޶��� ��, ��ü���� ĳ���� ����Ʈ ��� ������ �־�� �ȵȴ�.
// SHORT CharOrderNumber: ������ �迭���� ��� ��ġ�� �ִ°�?
BOOL SetChar(SHORT kind, _NewID idChar, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID/* = -1*/)
{
	SI32 file;
	SI32 i;


	// ��ȿ���� ���� ������ ĳ���ʹ� ������ �ʴ´�. 
	if( KI[kind].IsKindAtb(KINDATB_HERO) == FALSE && KI[kind].m_clCharAnimation[1].siFile == 0 )
	{
		return FALSE;
	}

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
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
	// ���ο� ĳ���͸� ����(����)�ϴ� ���� �ƴ϶� ������ ĳ���͸� ����(ĳ���� ����)�� �ϵ��� �����ؾ� �Ѵ�.
    CR[idChar.ID].CharInit_DISGUISE(kind, idChar.ID, idChar.Unique, x, y, dir, makepercent, pnumber, pip, pmw, onlineid, usCharOrderNumber, siPrevWeaponID);

    return TRUE;
}


