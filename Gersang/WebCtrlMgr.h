// WebCtrlMgr.h: interface for the CWebCtrlMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBCTRLMGR_H__F782D5F6_1694_4896_9CEC_52642F851ADF__INCLUDED_)
#define AFX_WEBCTRLMGR_H__F782D5F6_1694_4896_9CEC_52642F851ADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IOnlineMouse;
class CWebCtrlMgr  
{
public:
	CWebCtrlMgr();
	virtual ~CWebCtrlMgr();

	BOOL Create(HWND hWndParent);
	
	inline IOnlineMouse*	   GetMousePtr() { return m_pOnlineMouse; }
	inline void				   SetMousePtr(IOnlineMouse *pIOnlineMouse) { m_pOnlineMouse = pIOnlineMouse; }
	inline IDirectDrawClipper* GetClipper() { return m_ddClipper;}

	static BOOL CALLBACK WebCtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void DoSystemPaletteUse();
	void DetermineShowCursor();

private:
	
	HWND	  m_hWndParent;
	HINSTANCE m_hInstParent;
	
	IDirectDrawClipper* m_ddClipper;
	IOnlineMouse*	m_pOnlineMouse;

public:
	HWND			    m_hDlg;
	static CWebCtrlMgr* m_pThisPointer;
};

inline CWebCtrlMgr* GetWebControl()
{
	if( CWebCtrlMgr::m_pThisPointer)
		return CWebCtrlMgr::m_pThisPointer;
	return NULL;
}

#endif // !defined(AFX_WEBCTRLMGR_H__F782D5F6_1694_4896_9CEC_52642F851ADF__INCLUDED_)
