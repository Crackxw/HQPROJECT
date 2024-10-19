#ifndef _CHAT_H
#define _CHAT_H

#define MAX_INPUT_CHAR   100           // �Է� ���� �� �ִ� �ִ� ���� ���� 
#define MY_CARET_CODE '_'


class _Chat{
private:

public:
	_Chat();
	void SetInputModeSwitch(int mode, int max_string_size);
	void Init();
	void Initialize();

	// �Է����� ������ ǥ���Ѵ�. 
    void DrawInputText();

};

// �κ񿡼� �Է��� �޴´�. 
void InputOperateInLobby(); 

// �����߿� �Է��� �޴´�.
void InputOperateInGame();

// online���߿� �Է��� �޴´�.
void InputOperateInOnline();

// ����� �̸��� �Է¹޴´�.
void InputOperateInInitMenu();

void ChangeLanguage(BOOL flag);

#endif