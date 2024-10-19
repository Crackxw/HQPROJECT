// PatchToolDlg.h : header file
//

#if !defined(AFX_PATCHTOOLDLG_H__82E34C52_0B2F_4631_8648_696C5CB58AE6__INCLUDED_)
#define AFX_PATCHTOOLDLG_H__82E34C52_0B2F_4631_8648_696C5CB58AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPatchToolDlg dialog

#include "DDListCtrl.h"
#include "afxwin.h"
#include "TNDBConfig.h"
#include "afxcmn.h"
#include "TextProgressCtrl.h"
#include <stack>
#include <vector>
#include "DBJobRecord.h"
#include "Struct.h"

// Forward Declaration
class CSelectInfo;
class CListSelectData;

#define SOURCE_FILE_LIST_ID		0
#define DESTINATION_FILE_LIST	1

#define SERVER_PATCH_TIMER_ID	 WM_USER+1
#define SERVER_REFRESH_TIMER_ID  WM_USER+2

class CPatchToolDlg : public CDialog, public CTNDBConfig
{
// Construction
public:
	CPatchToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPatchToolDlg)
	enum { IDD = IDD_PATCHTOOL_DIALOG };
	CStatic	m_FileTransferName;	
	CListBox	m_MsgDisplayList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchToolDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private : 
	struct _S_PATCH_INFO{
		char szDir[_MAX_DIR];
		char szSettingDir[_MAX_DIR];
		char szServerName[SERVER_NAME_MAX_SIZE];
	};

	CListSelectData*	m_pData;
	CSelectInfo*		m_pSelectInfo;
	CDDListCtrl			m_SourceFileList;
	CDDListCtrl			m_TargetFileList;
	CDDListCtrl			m_WasteFileList;

	CString				m_SourceDirectoryPath;
	
	int					m_nPatchCount;
	_S_PATCH_INFO*		m_pDesPatch;
	_S_PATCH_INFO		m_SrcPatch;

	std::stack<CString>	m_stackDirectoryPath;

	CString				m_strCurTime;
	BYTE				m_byNewVersion;
	CString				m_strAttachPath;
	
	CTNDBConfig			m_DestPathConfig;
	CString				m_pDestPath[32];
	WORD				m_wDestPathCount;
	DWORD				m_dwTickCount;

//	CDBJobRecord		m_DBRecord;

private : 
	void Initialize();
	void FileListDraw();
	void Init_DestinationPath();	
	void TransmissionPercentFile();

	BOOL OnCopyDirectorys();
	BOOL CopyDirectorys( LPCTSTR strSourceDirectory, 
		LPCTSTR strDestinationDirectory, 
		int& nCount, 
		int nPatchCount);
	BOOL CopySettingFile(int n, CString strDestinationDirectory);
	BOOL OnDeleteDirectorys();
	BOOL DeleteFiles(CString strDestinationDirectory);
	BOOL RemoveDirectorys();


	int GetRealPatchCount();	
	void EnableWindowCtrl(BOOL bEnable);
	BOOL IsCopyFile(CString strFileName);
	void JobProgressBar(int& nCount, int nPatchCount);

	BOOL IsPatchEnable();
	void SetDestinationPathSave(BYTE byRetVal, CString strDestPath);	
	
public : 
	void MsgPrint(const TCHAR* szFormat, ...);

	WORD GetSelectedServer(WORD* pServerList);
	void Init_PatchDestinationPath();

	void FileTransferState
		(
			int nGropNum,  
			const char* pFileName
		);
	
	void AddServer();
	
// Implementation
protected:
	HICON m_hIcon;

	CEdit		m_SourceDirectoryPathEditCtrl;
	CEdit		m_FileListEditCtrl;
	CEdit		m_JobSubstanceEditCtrl;
	CEdit		m_JobMakerEditCtrl;
	CCheckListBox m_SelectListBox;
	CTextProgressCtrl m_CopyFileProgressCtrl;
	
	// Generated message map functions
	//{{AFX_MSG(CPatchToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSourcedirectorypathbtn();
	afx_msg void OnAllFileAddBtn();
	afx_msg void OnAllFileRemoveBtn();
	afx_msg void OnAllpatchchoicebtn();
	afx_msg void OnChoicepatchcancelbtn();
	afx_msg void OnDeletepatchbtn();	
	afx_msg void OnStartpatchbtn();
	virtual void OnCancel();
	afx_msg void OnAbout();
	afx_msg void OnAttachradio();
	afx_msg void OnNewvisionradio();
	afx_msg void OnMasterServerConnect();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHTOOLDLG_H__82E34C52_0B2F_4631_8648_696C5CB58AE6__INCLUDED_)
