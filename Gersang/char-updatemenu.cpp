#include <GSL.h>

#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
#endif

#include <map.h>
#include <CharOrg.h>
#include <OnlineWorld.h>

extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Map 				Map;



// ��� �Ʒ��� �޴��� �߰��� �� �ִ��� ���θ� �˷��ش�. 
BOOL cltCharOrg::CanPushGeneralMakeMenu(SHORT kind)
{
	// ���� �۾��� �ش� �Ʒ��̰�  
	if(CurrentOrder==ORDER_TRAIN && Para1==kind)
	{
		return FALSE;
	}

	// �̹� ����� ���ӿ� �����Ѵٸ�, 
	if(GD.CharNumber[GetPlayerNumber()][kind])return FALSE;

	return TRUE;
}


BOOL cltCharOrg::UpdateMenu(SHORT order)
{

	for(SHORT i=0;i<MAX_MENU_NUMBER;i++)
	{
		MenuItem[i]=0;
	}

	UpdateMenuNormal(order);

	return TRUE;

}




// �޴��� ������Ʈ�Ѵ�. 
BOOL cltCharOrg::UpdateMenuNormal(SHORT order)
{

	UpdateMenuOrder=order;

	MenuNumber=0;
	PushMenu(0, ITEM_STOP);
	PushMenu(1, ITEM_MOVE);
	
	// ���� �Ӽ��� ���� �־�߸� ���� �޴��� �۵��Ѵ�. 
	if(IsAtb(ATB_ATTACK|ATB_ATTACKAIR))
		PushMenu(2, ITEM_ATTACK);

	PushMenu(3, ITEM_HOLD);


	SI32 index= 4;

	if(IsAtb3(ATB3_LIGHTNING ) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_LIGHTNING);
		index++;
	}

	if(IsAtb3(ATB3_EARTHQUAKE) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_EARTHQUAKE);
		index++;
	}

	// ���ڰ�. 
	if(IsAtb3(ATB3_KIMBOMB) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_KIMBOMB);
		index++;
	}

	if(IsAtb3(ATB3_MAGIC5J) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC5J);
		index++;
	}

	// ������� .
	if(IsAtb3(ATB3_MAGIC4 ) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC4C);
		index++;
	}

	// ���ڼ�. 
	if(IsAtb3(ATB3_MAGIC6C) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC6C);
		index++;
	}

	//�̻��� ��� Ư�� ����.
	if(IsAtb3(ATB3_ICE) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_ICE);
		index++;
	}

	// ǳ���. 
	if(IsAtb3(ATB3_TORNADO) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_TORNADO);
		index++;
	}
	// Ȳ��.
	if(IsAtb3(ATB3_YELLOWSAND) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_YELLOWSAND);
		index++;
	}

	// ���ɺм��(��������)
	if(IsAtb3(ATB3_MAGIC4T) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC4T);
		index++;
	}
	// �߼��� ȥ
	if(IsAtb3(ATB3_SOUL) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index, ITEM_MAGIC_SOUL);
		index++;
	}

	//�ͽ�ź.
	if(IsAtb3(ATB3_GHOST) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_GHOST);
	}

	//����ȭ����ǳ.
	if(IsAtb3(ATB3_DARKFIRE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_DARKFIRE);
		index++;
	}

	if(IsAtb3(ATB3_YELLOWSAND) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_YELLOWSAND);
	}

	//�������
	if(IsAtb4(ATB4_IVORY) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_IVORY);
		index++;
	}

	//���û�浵
	if(IsAtb4(ATB4_BLUEDRAGON) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_BLUEDRAGON);
		index++;
	}

	//������ź
	if(IsAtb4(ATB4_RUNNINGFIRE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RUNNINGFIRE);
		index++;
	}

	//��ǳ
	if(IsAtb4(ATB4_ENERGYPA)	== TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_ENERGYPA);
		index++;
	}

	//��������
	if(IsAtb4(ATB4_SNOWATTACK) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_SNOWATTACK);
		index++;
	}
	
	//ȭ������
	if(IsAtb4(ATB4_FIREATTACK) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_FIREATTACK);
		index++;
	}
	//��ȭ��ǳ(���Ȳ���).
	if(IsAtb4(ATB4_FIREENERGYPA) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_FIREENERGYPA);
		index++;
	}
	

	// �нż�. 
	// �н��� ����� �� ����.(HostID.IsEmpty() == TRUE :�н��� �ƴϴ�.)
	if(IsAtb3(ATB3_CLON) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_CLON);
		index++;
	}

	// �нż�. 
	// ������� �ı�,�Ʊ� ��ȯ
	// �Ƹ���簡 �ҷ��� ������ �ı�,�Ʊ͸� ��ȯ�Ҽ� �ִ�.
	if(IsAtb3(ATB3_CLON2) == TRUE )
	{
		PushMenu(index, ITEM_MAGIC_CLON);
		index++;
	}


/*	// �����. 
	// �н��� ����� �� ����.(HostID.IsEmpty() == TRUE :�н��� �ƴϴ�.)
	if(IsAtb3(ATB3_ABSORBLIFE) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_ABSORB);
		index++;
	}
	*/
	// ��. 
	if(IsAtb3(ATB3_SHIELDMAGIC) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_SHIELD);
		index++;
	}
	
	// ��ȭ�� 
	// �н��� ����� �� ����.(HostID.IsEmpty() == TRUE :�н��� �ƴϴ�.)
	if(IsAtb3(ATB3_EXTINGUISH) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_EXTINGUISH);
		index++;
	}


	// ������ 
	// �н��� ����� �� ����.(HostID.IsEmpty() == TRUE :�н��� �ƴϴ�.)
	if(IsAtb3(ATB3_GROUNDEXP) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_GROUNDEXP);
		index++;
	}

	// ������
	if(IsAtb3(ATB3_POISONBULLET) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC2T);
		index++;
	}
	//���� �׽�Ʈ 
	//////////////////////////////////////////////////////////////////////
	if(IsAtb3(ATB3_MAGICTEST) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_MAGICTEST);
		index++;
	}
	//////////////////////////////////////////////////////////////////////
	if(IsAtb3(ATB3_MAGIC_RECALL_PANDA) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RECALL_PANDA);
		index++;
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RECALL_MONKEY);
		index++;
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RECALL_MONKEY);
		index++;
	}
	if(IsAtb3(ATB3_MINE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MINE);
		index++;
	}
	//������.
	/*
	if(IsAtb4(ATB4_MANABURN) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_MANABURN);
		index++;
	}
	*/
	// �ְ˳���
	if(IsAtb4(ATB4_DOUBLE_SWORD) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_DOUBLE_SWORD);
		index++;
	}
	// ��â��ȯ
	if(IsAtb4(ATB4_LONG_SPEAR) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_LONG_SPEAR);
		index++;
	}		
	// ������
	if(IsAtb4(ATB4_ICE_BARRIER) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_ICE_BARRIER);
		index++;
	}
	// ��õȭ��
	if(IsAtb4(ATB4_FLOWER_NEEDLE_RAIN) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_FLOWER_NEEDLE_RAIN);
		index++;
	}
	// ���Ǽ�
	if(IsAtb4(ATB4_SOUL_CHANGE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_SOUL_CHANGE);
		index++;
	}		
	// ������
	if(IsAtb4(ATB4_STONE_MAGIC) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_STONE_MAGIC);
		index++;
	}
	//������
	if(IsAtb4(ATB4_MANABURN) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_MANABURN);
		index++;
	}

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:55) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	// ����������(����-�ź���, �̼��� 2�� ���� ���)
	if ((IsAtb4(ATB4_FIRE_ROUND_DANCE) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_FIRE_ROUND_DANCE);
		index++;
	}

	// ����(����-������, ���� 2�� ���� ���)
	if ((IsAtb4(ATB4_NOVA) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_NOVA);
		index++;
	}

	// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
	if ((IsAtb4(ATB4_FURY_OF_VOLCANO) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_FURY_OF_VOLCANO);
		index++;
	}

	// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
	if ((IsAtb4(ATB4_POISONING_HEAVENS) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_POISONING_HEAVENS);
		index++;
	}

	// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
	if ((IsAtb4(ATB4_BOMB_OF_HEAVEN) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_BOMB_OF_HEAVEN);
		index++;
	}

	// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
	if ((IsAtb4(ATB4_SIEGE_MODE) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_SIEGE_MODE);
		index++;
	}

	// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
	if ((IsAtb4(ATB4_SIEGE_MODE_UNDO) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_SIEGE_MODE_UNDO);
		index++;
	}

	// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
	if ((IsAtb4(ATB4_ONSLAUGHT_FIREBALL) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_ONSLAUGHT_FIREBALL);
		index++;
	}

	// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
	if ((IsAtb4(ATB4_FIRERAIN_OF_GOD) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_FIRERAIN_OF_GOD);
		index++;
	}

	return TRUE;
}


