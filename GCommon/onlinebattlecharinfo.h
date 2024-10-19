#ifndef _ONLINEBATTLECHARINFO_H
#define _ONLINEBATTLECHARINFO_H

#define MAX_ONLINE_CHAR_NUMBER_PER_PLAYER		10    // �÷��̾�� �ִ� ĳ���� ����. 

// ���� ĳ������ ����. 
class _OnlineBattleCharInfo {
public:
	UI08 CharKind;	// ĳ������ ����.
	UI08 Reserved[3];
};

// �� �÷��̾��� ����. 
class _OnlineBattlePlayerInfo{
public:
	SI32 CharNumber;	// �÷��̾� �Ҽ��� ĳ���� ����. 
	_OnlineBattleCharInfo * pOBCI[MAX_ONLINE_CHAR_NUMBER_PER_PLAYER];
};

#endif

