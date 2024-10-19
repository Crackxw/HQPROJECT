#ifndef _TALK_H
#define _TALK_H

#define TALKINDEX_WAIT                1     // ������ 
#define TALKINDEX_MOVE                2     // �̵� 
#define TALKINDEX_HARVEST             3     // ��Ȯ 
#define TALKINDEX_DYING               4     // ������ 
#define TALKINDEX_RECOVER             5     // �λ󿡼� ȸ���ɶ� 
#define TALKINDEX_TRAIN               6     // �Ʒ��� ������ ���ö� 
#define TALKINDEX_BEATTACKEDBROADCAST 7     // ���� ������ �ް� ���� 
#define TALKINDEX_STEALTECH           8     // �� ����� ������
#define TALKINDEX_STEALEDTECH         9     // ����� ���ѱ�
#define TALKINDEX_UPGRADE            10     // ���׷��̵尡 �Ϸ�Ǿ���. 
#define TALKINDEX_MERCHANT_ARRIVE    11     // ������ �����߽��ϴ�. 
#define TALKINDEX_MERCHANT_LEAVE     12     // ������ �������ϴ�.
#define TALKINDEX_LEVELUP            13     // ����� ������ �ö���. 
#define TALKINDEX_LEVELDOWN          14     // ����� ������ ��������.
#define TALKINDEX_CAPTUREDBUILD      15     // �ǹ��� ���ɵǾ���
#define TALKINDEX_CAPTUREENEMYBUILD  16     // ���� �ǹ��� �����ߴ�.
#define TALKINDEX_MINE               17
#define TALKINDEX_BUILDDONE          18

#define TALK_TEXT_SIZE              128

class _Talk {
public:
	char TalkText[128];    // ǥ�����־�� �� ������ 
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