#ifndef		_SOTHREAD_H_
#define		_SOTHREAD_H_

#include <windows.h>

DWORD	WINAPI	DBThread(void *pMe);

DWORD	WINAPI	DBSendThread(void *pMe);

DWORD	WINAPI	GetDBThread(void *pMe);
#endif	//_SOTHREAD_H_