#ifndef _ONLINEMSG_H
#define _ONLINEMSG_H


#define ONLINEMSG_CONNECTBATTLESERVER '\x10'


// ���� ������ �������� �˷��ִ� �޽���. 
class ConnectBattleServerMsg{
public:
	SI32	Type;				//  '\x10'
	SI32	Channel;
	SI32	UniqueID;
	void	Init(){Type = ONLINEMSG_CONNECTBATTLESERVER ;  Channel = 0; UniqueID = 0;}
};


#endif
