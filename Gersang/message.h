#ifndef _MESSAGE_H
#define _MESSAGE_H

#define MAX_MESSAGE_NUMBER     8
#define MAX_MESSAGE_DELAY      140         // 메시지를 얼마나 오래 표시하는가?
#define MAX_MESSAGE2_DELAY     55         // 메시지를 얼마나 오래 표시하는가?

#define MAX_MESSAGE_TEXT_LENGTH    150


#define MESSAGEMODE_ALL        0
#define MESSAGEMODE_ALLY       1  


// 메시지들을 초기화한다. 
void InitMessage();

// 메시지를 표시한다. p_number가 -1이거나 유저인 경우에만 표시한다. 
void SetMessage(char* message,  SHORT p_number=-1,  SHORT effect=0, COLORREF rgb = NORMAL_TEXT_COLOR);
// 메시지를 표시한다. p_number가 -1이거나 유저인 경우에만 표시한다. 
void SetMessage2(char* message, SHORT p_number=-1, SHORT effect=0);
// 게임 목적 표시용 메시지를 표시한다. 
void SetMessage3(char* message, SHORT effect);
// robypark 2005/1/12 13:51
// 메시지를 표시한다. 메시지의 길이가 길 경우에 여러 라인으로 나눠준다. p_number가 -1이거나 유저인 경우에만 표시한다. 
void SetMessage4(char* pString, HDC hDC, SHORT p_number=-1,  SHORT effect=0, COLORREF rgb = NORMAL_TEXT_COLOR);

void SetMessage3Off();

void ScrollMessage();

void ShowMessage(SI32 main_message_start_x, SI32 main_message_start_y, LPDIRECTDRAWSURFACE7 surface);

// 마우스 밑에 있는 곡물의 정보를 보여준다.
void ShowCropInfo(LPDIRECTDRAWSURFACE7 surface);

// 게임 시간을 진행한다.
void PassPlayTime();

// 남은 시간을 표시한다. 
void ShowPlayTime(SI32 x, SI32 y, LPDIRECTDRAWSURFACE7 surface);

//남은 시간을 얻어온다.
BOOL GetPlayTime();

#endif