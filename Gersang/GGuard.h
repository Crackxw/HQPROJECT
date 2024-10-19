#ifndef GGUARD_H
#define GGUARD_H

BOOL CreateGameGuard(BOOL bEnable);
void SetHwndToGameGuard(HWND hWnd);
VOID ReleaseGameGuard(VOID);
VOID SetGameGuardKey(DWORD dwGameGuardkey);
BOOL CheckGameGuard(VOID);
VOID SendIDGameGuard(LPCTSTR lpszUserId);

#endif