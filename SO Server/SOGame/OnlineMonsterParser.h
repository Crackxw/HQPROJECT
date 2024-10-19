#ifndef	__ONLINE_MONSTER_PARSER_H
#define	__ONLINE_MONSTER_PARSER_H


/*******************************************************************************************************
	���ϸ�	:	OnlineMonsterParser.h
	�ۼ���	:	�� �� ��
	�ۼ���	:	2001.08.03
	������	:	2001.11.19
*******************************************************************************************************/


#define _ON_MAX_GROUP			7


struct	MonsterPatternHeader
{
	UI32					uiMonsterID[_ON_MAX_GROUP];										// ���̵�
	SI16					siRate[_ON_MAX_GROUP];											// ����
	UI16					uiMonsterLv[_ON_MAX_GROUP];										// ���� ����
	SI16					siX, siY;														// ��ǥ
	SI16					siMapIndex;
	SI16					siRadius;														// ����
	SI16					siMaxKind;														// ���� ����
	SI16					siGroupMaxQuantity, siGroupMinQuantity;
	SI16					siMaxQuantity;													// ���� �ִ�
	SI16					siMinQuantity;													// ���� �ּ�
	
};


class OnlineMonsterParser
{
private:
	SI32					siTotalPattern;													// �� ���ϼ�
	MonsterPatternHeader	*pMonsterPatternHeader;											// ���� ���

public:
	OnlineMonsterParser();
	~OnlineMonsterParser();

	BOOL					Init( CHAR *pMonsterText );										// ���� �ؽ��� �д´�.	
	VOID					Free();															// ������.

	SI32					GetTotalPattern();												// �� ���� ������ ���� �Ѵ�.
	MonsterPatternHeader	*GetPatternHeader( UI32 uiIndex );								// ���� �ش��� �����Ѵ�.

private:
	UI32					GetID( CHAR *pStr );											// ���ڿ��� �Ƶ�� ��ȯ
};

#endif