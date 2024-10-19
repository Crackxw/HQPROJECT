#ifndef	BATTLE_PROTOCOL_HEADER
#define	BATTLE_PROTOCOL_HEADER

#include "SOCommonHeader.h"

// ���� ������ ���� ������(UDP)�� ���� ��Ŷ ��� 

typedef struct
{
	WORD	dataSize;
	BYTE	checkSum;

}HEADER;



// 1. ���� ���� ���� 				   [ ���Ӽ���  -------->  ���� ���� ]
typedef struct 
{
	WORD   command;						// ON_REQUEST_CREATE_BATTLEROOM

	WORD   numPlayer;					// ������ �÷��̾��� �� (NPC(����)����)
	DWORD  uniqID;						// ���� �������� �Ǵ��� ����ũ�� ���� ���̵� 	
}REQUEST_INFOGAME;


// 2. ���� ���� ���� 					[ ���Ӽ���  <--------  ���� ���� ]
typedef struct 
{
	WORD    command;					// ON_RESPONSE_CREATE_BATTLEROOM	
	WORD	battleIndex;				// ��Ʋ���������� ���� �ε���, ���� ������ ���� ���� ��� -1 										
	DWORD	uniqID;						// ���� ������ ������ ���Ӽ��������� ����ũ ���̵�  

}RESPONSE_CREATEGAME;


// 3. ���� ����,����� ����Ÿ �˸�  	[ ���Ӽ���  <--------  ���� ���� ]
typedef struct							
{
	WORD   command;						// ON_RESPONSE_FINISH_BATTLE	
	WORD   numPlayer;					// ������ �Ϻ��� ���� �Ǿ��� ��� +90 �� ���� ���� 
	DWORD  uniqID;						// ���Ӽ��������� UniqID
}RESPONSE_FINISHGAME;


// 1. ������ �ִ� �÷��̾� ���� ��û 	[ ���Ӽ���  -------->  ���� ���� ]
typedef struct 
{
	WORD	command;					// ON_REQUEST_CREATE_BATTLEROOM
	WORD	uiDBSlot;					// DBSlot	
	SI32	siDBAccount;				// DBAccount;		
}REQUEST_BATTLEPLAYERINFO;

// ������ ����Ǿ��ٴ� ���� ���� �������� �˷���
typedef struct 
{
	WORD	command;					// ON_BATTLEROOM_FINISHED
	WORD	battleIndex;				// ��Ʋ���������� ���� �ε���
}BATTLE_ROOMFINISHED;


// ������ ����Ǿ��ٴ� �޽����� �޾Ҵٴ� ���� ���� �������� �ٽ� �˷���
typedef struct 
{
	WORD	command;					// ON_BATTLEROOM_FINISHED_RECV_OK
	WORD	battleIndex;				// ��Ʋ���������� ���� �ε���
}BATTLE_ROOMFINISHED_RECV_OK;

/*
	���� ��Ȳ  

  ������ ���������� ���� ������ ����(��Ʈ�������� ���ڱ� ������)�� ���Ӽ����� ���������� 
  ���� ���� �޽����� ������ ������ ���Ḧ ó���ؾ� �Ѵ�. 
  ������ ������ ������ 30���̻�(����) ���������� ���� ����޽����� �������� ������ ���������� 
  �����û�� �Ҽ��ִ�. 
  ( ���Ӽ��������� ������ �����, �������������� ������ �������, ���Ӽ����� ������������ ���� 
    ������ ������ ���Ḧ �˾��� ��� ) 
 
*/

#endif