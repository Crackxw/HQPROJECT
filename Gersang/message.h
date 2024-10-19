#ifndef _MESSAGE_H
#define _MESSAGE_H

#define MAX_MESSAGE_NUMBER     8
#define MAX_MESSAGE_DELAY      140         // �޽����� �󸶳� ���� ǥ���ϴ°�?
#define MAX_MESSAGE2_DELAY     55         // �޽����� �󸶳� ���� ǥ���ϴ°�?

#define MAX_MESSAGE_TEXT_LENGTH    150


#define MESSAGEMODE_ALL        0
#define MESSAGEMODE_ALLY       1  


// �޽������� �ʱ�ȭ�Ѵ�. 
void InitMessage();

// �޽����� ǥ���Ѵ�. p_number�� -1�̰ų� ������ ��쿡�� ǥ���Ѵ�. 
void SetMessage(char* message,  SHORT p_number=-1,  SHORT effect=0, COLORREF rgb = NORMAL_TEXT_COLOR);
// �޽����� ǥ���Ѵ�. p_number�� -1�̰ų� ������ ��쿡�� ǥ���Ѵ�. 
void SetMessage2(char* message, SHORT p_number=-1, SHORT effect=0);
// ���� ���� ǥ�ÿ� �޽����� ǥ���Ѵ�. 
void SetMessage3(char* message, SHORT effect);
// robypark 2005/1/12 13:51
// �޽����� ǥ���Ѵ�. �޽����� ���̰� �� ��쿡 ���� �������� �����ش�. p_number�� -1�̰ų� ������ ��쿡�� ǥ���Ѵ�. 
void SetMessage4(char* pString, HDC hDC, SHORT p_number=-1,  SHORT effect=0, COLORREF rgb = NORMAL_TEXT_COLOR);

void SetMessage3Off();

void ScrollMessage();

void ShowMessage(SI32 main_message_start_x, SI32 main_message_start_y, LPDIRECTDRAWSURFACE7 surface);

// ���콺 �ؿ� �ִ� ��� ������ �����ش�.
void ShowCropInfo(LPDIRECTDRAWSURFACE7 surface);

// ���� �ð��� �����Ѵ�.
void PassPlayTime();

// ���� �ð��� ǥ���Ѵ�. 
void ShowPlayTime(SI32 x, SI32 y, LPDIRECTDRAWSURFACE7 surface);

//���� �ð��� ���´�.
BOOL GetPlayTime();

#endif