// SelectDirectoryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "servermonitor.h"
#include "ServerMonitorDlg.h"
#include "SelectDirectoryDialog.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDirectoryDialog dialog


CSelectDirectoryDialog::CSelectDirectoryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDirectoryDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDirectoryDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectDirectoryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDirectoryDialog)
	DDX_Control(pDX, IDC_COMBO1, m_DirectoryComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDirectoryDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectDirectoryDialog)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDirectoryDialog message handlers

void CSelectDirectoryDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CSelectDirectoryDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
				
				FILE *fp;
				char buffer[256];
				char buffer2[256];

#ifdef _UNICODE
				{
					TCHAR tstr[256];
					ZeroMemory(tstr,sizeof(tstr));
					m_DirectoryComboBox.GetWindowText(tstr,256);
					wcstombs(buffer,tstr,256);
				}
#else
				m_DirectoryComboBox.GetWindowText(buffer,256);
#endif


				
				fp = fopen("rcdirectory.txt","r+t");
				
				if (fp != NULL)
				{
					fseek(fp,0,SEEK_SET);
					int flag = 0;
					while (fgets(buffer2,256,fp) != NULL)
					{
						buffer2[strlen(buffer2) - 1] = 0;
						if ( !strcmp(buffer2,buffer) ) {
						
							
							flag = 1;
						}
					}
					if (!flag)
					{
						fprintf(fp,"%s\n",buffer);
					}
				}
				else
				{
					fp = fopen("rcdirectory.txt","wt");
					fprintf(fp,"%s\n",buffer);
				}

				fclose(fp);

#ifdef _UNICODE
			TCHAR dirtemp[256];
			ZeroMemory(dirtemp,sizeof(dirtemp));
			m_DirectoryComboBox.GetWindowText(dirtemp,256);

			wcstombs(g_SavePath,dirtemp,256);

#else
				
            m_DirectoryComboBox.GetWindowText(g_SavePath,256);
#endif
	//		AfxMessageBox("¾Æ½Î");
			OnOK();
			}
			return 1;
	
		}
	
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSelectDirectoryDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	
	FILE *fp;
	char buffer[256];
	WCHAR wbuffer[512];
	ZeroMemory(wbuffer,sizeof(wbuffer));
	fp = fopen("rcdirectory.txt","rt");
	if (fp != NULL)
	{
		while (fgets(buffer,256,fp) != NULL)
		{

	
			buffer[strlen(buffer) - 1] = 0;

#ifdef _UNICODE
			mbstowcs(wbuffer,buffer,strlen(buffer));
			m_DirectoryComboBox.AddString(wbuffer);
			
			
			
#else
			m_DirectoryComboBox.AddString(buffer);
#endif
			
			
		}
		fclose(fp);
	}
	
	m_DirectoryComboBox.MoveWindow(20,20,300,120);

	TCHAR dir[256];
	GetCurrentDirectory(256,dir);
	SetCurrentDirectory(dir);
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
