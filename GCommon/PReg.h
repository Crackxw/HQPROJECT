#ifndef _PREG_H
#define _PREG_H


//-----------------------------------------------------------------
BOOL	GetPath(char *Sp);				// ���Ӱ�θ��� �˾ƿ´�.
BOOL	GetGameVersion(char Str[]);					// ������ ������ �ҷ��´�. �����ϸ� FALSE
SI32	GetAdType();					            // ������Ʈ������ ���������� �ҷ��´�.
void	SetServerVersion();							// ������ ������ ������Ʈ���� ����Ѵ�.	
BOOL	DeleteData();								// �ʿ���� ������ �����.

#ifdef	_MYTH
BOOL	DPRegister(char AppName[], char GamePath[], char ExeName[]);
#endif
//-----------------------------------------------------------------
#endif
