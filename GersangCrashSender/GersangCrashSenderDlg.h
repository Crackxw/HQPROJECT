// GersangCrashSenderDlg.h : header file
//

#if !defined(AFX_GERSANGCRASHSENDERDLG_H__29CAE78C_6A44_4269_BDC7_B46D89D12A1B__INCLUDED_)
#define AFX_GERSANGCRASHSENDERDLG_H__29CAE78C_6A44_4269_BDC7_B46D89D12A1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include <winsock2.h>
#include "..\\CrashMailProtocol\\CrashMailProtocol.h"

/////////////////////////////////////////////////////////////////////////////
// CGersangCrashSenderDlg dialog

#define ON_PROCESSOR_INTEL_386     386
#define ON_PROCESSOR_INTEL_486     486
#define ON_PROCESSOR_INTEL_PENTIUM 586
#define ON_PROCESSOR_MIPS_R4000    4000    // incl R4101 & R3910 for Windows CE
#define ON_PROCESSOR_ALPHA_21064   21064
#define ON_PROCESSOR_PPC_601       601
#define ON_PROON_CESSOR_PPC_603    603
#define ON_PROCESSOR_PPC_604       604
#define ON_PROCESSOR_PPC_620       620
#define ON_PROCESSOR_HITACHI_SH3   10003   // Windows CE
#define ON_PROCESSOR_HITACHI_SH3E  10004   // Windows CE
#define ON_PROCESSOR_HITACHI_SH4   10005   // Windows CE
#define ON_PROCESSOR_MOTOROLA_821  821     // Windows CE
#define ON_PROCESSOR_SHx_SH3       103     // Windows CE
#define ON_PROCESSOR_SHx_SH4       104     // Windows CE
#define ON_PROCESSOR_STRONGARM     2577    // Windows CE - 0xA11
#define ON_PROCESSOR_ARM720        1824    // Windows CE - 0x720
#define ON_PROCESSOR_ARM820        2080    // Windows CE - 0x820
#define ON_PROCESSOR_ARM920        2336    // Windows CE - 0x920
#define ON_PROCESSOR_ARM_7TDMI     70001   // Windows CE


class CGersangCrashSenderDlg : public CDialog
{
// Construction
public:
	CGersangCrashSenderDlg(CWnd* pParent = NULL);	// standard constructor
	~CGersangCrashSenderDlg();

	void SetCrashInfo( char* szStartTime, char* szLineAddress, char* szCrashInfo);	

private : 
	void		GetHostIP(char* IP, SOCKET Socket = INVALID_SOCKET);
	const char* GetProcessType(DWORD dwType);
	const char* GetOSType(DWORD dwPlatformId, DWORD dwMinorVersion);
	int			Connect();
	BOOL		SendPacket(const SOCKET Socket, const char* pPacket, int nSize);
	BOOL		IsRecvAble(SOCKET Socket) const;
	
public : 	

// Dialog Data
	//{{AFX_DATA(CGersangCrashSenderDlg)
	enum { IDD = IDD_GERSANGCRASHSENDER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGersangCrashSenderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CString m_strStartTime;
	CString m_strCrashAddress;
	CString m_strLineAddress;
	bool	m_bGetParam;


	ON_REQ_CRASHINFO	m_CrashInfo;
	LONG	m_nCrashAddress;
	LONG	m_nLineAddress;
	STIME	m_sTime;
	char	m_szNationCode[64];
	char	m_szSupposition[256];

	SOCKADDR_IN	SocketAddr;

	// Generated message map functions
	//{{AFX_MSG(CGersangCrashSenderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSendOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GERSANGCRASHSENDERDLG_H__29CAE78C_6A44_4269_BDC7_B46D89D12A1B__INCLUDED_)
