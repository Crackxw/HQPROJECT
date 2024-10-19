#ifndef _PREG_H
#define _PREG_H


//-----------------------------------------------------------------
BOOL	GetPath(char *Sp);				// 게임경로명을 알아온다.
BOOL	GetGameVersion(char Str[]);					// 게임의 버젼을 불러온다. 실패하면 FALSE
SI32	GetAdType();					            // 레지스트리에서 광고정보를 불러온다.
void	SetServerVersion();							// 게임의 버젼을 레지스트리에 등록한다.	
BOOL	DeleteData();								// 필요없는 파일을 지운다.

#ifdef	_MYTH
BOOL	DPRegister(char AppName[], char GamePath[], char ExeName[]);
#endif
//-----------------------------------------------------------------
#endif
