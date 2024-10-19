#ifndef _TALK_H
#define _TALK_H

#define TALKINDEX_WAIT                1     // 대기상태 
#define TALKINDEX_MOVE                2     // 이동 
#define TALKINDEX_HARVEST             3     // 수확 
#define TALKINDEX_DYING               4     // 죽을때 
#define TALKINDEX_RECOVER             5     // 부상에서 회복될때 
#define TALKINDEX_TRAIN               6     // 훈련이 끝나고 나올때 
#define TALKINDEX_BEATTACKEDBROADCAST 7     // 적의 공격을 받고 있음 
#define TALKINDEX_STEALTECH           8     // 적 기술을 빼앗음
#define TALKINDEX_STEALEDTECH         9     // 기술을 빼앗김
#define TALKINDEX_UPGRADE            10     // 업그레이드가 완료되었다. 
#define TALKINDEX_MERCHANT_ARRIVE    11     // 상인이 도착했습니다. 
#define TALKINDEX_MERCHANT_LEAVE     12     // 상인이 떠났습니다.
#define TALKINDEX_LEVELUP            13     // 장수의 레벨이 올랐다. 
#define TALKINDEX_LEVELDOWN          14     // 장수의 레벨이 내려갔다.
#define TALKINDEX_CAPTUREDBUILD      15     // 건물이 점령되었다
#define TALKINDEX_CAPTUREENEMYBUILD  16     // 적의 건물을 점령했다.
#define TALKINDEX_MINE               17
#define TALKINDEX_BUILDDONE          18

#define TALK_TEXT_SIZE              128

class _Talk {
public:
	char TalkText[128];    // 표시해주어야 할 데이터 
	char DrawTalkText[128];
	char TalkSwitch;       
	char TalkStartX, TalkStartY;
	SHORT TalkID;
	DWORD TalkClock;

	void Init();
	void PushText(SHORT id, char * text);
	void PushText(SHORT id);

	BOOL FindProperText(SHORT kind, SHORT index, char* text, char *filename);

};

void InitTalkData();
void DeleteTalkData();

#endif