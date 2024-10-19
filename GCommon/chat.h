#ifndef _CHAT_H
#define _CHAT_H

#define MAX_INPUT_CHAR   100           // 입력 받을 수 있는 최대 문자 갯수 
#define MY_CARET_CODE '_'


class _Chat{
private:

public:
	_Chat();
	void SetInputModeSwitch(int mode, int max_string_size);
	void Init();
	void Initialize();

	// 입력중인 문장을 표시한다. 
    void DrawInputText();

};

// 로비에서 입력을 받는다. 
void InputOperateInLobby(); 

// 게임중에 입력을 받는다.
void InputOperateInGame();

// online임중에 입력을 받는다.
void InputOperateInOnline();

// 사용자 이름을 입력받는다.
void InputOperateInInitMenu();

void ChangeLanguage(BOOL flag);

#endif