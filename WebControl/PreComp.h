#ifndef PRECOMP_H
#define PRECOMP_H

#include <windows.h>
#include <windowsx.h>
#include <ocidl.h>
#include <shlobj.h>
#include <tchar.h>
#include "CntrEvnt.h"
#include "WebCtrl.h"
#include "resource.h"

#define NUMCHARS(x) (sizeof(x)/sizeof(x[0]))
#define THIS_PROP   _T("this")

class CApp
{
public:

   CApp( HINSTANCE );

private:

   friend int WINAPI _tWinMain( HINSTANCE, HINSTANCE, LPTSTR, int );

   friend BOOL CALLBACK AboutDlgProc( HWND, UINT, WPARAM, LPARAM );
 
   friend LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

   LRESULT WndProc( UINT, WPARAM, LPARAM );

   BOOL AboutDlgProc( HWND, UINT, WPARAM, LPARAM );

   int Main( int );

   HWND m_hwnd;

   HWND m_hwndWebCtrl;

   HINSTANCE m_hinst;

   TCHAR m_szHTML[MAX_PATH];
};

#endif
