// IpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "servermonitor.h"
#include "IpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIpDialog dialog


CIpDialog::CIpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpDialog)
	m_IpString = _T("");
	//}}AFX_DATA_INIT
}


void CIpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpDialog)
	DDX_Control(pDX, IDC_COMBO1, m_IpComboBox);
	DDX_CBString(pDX, IDC_COMBO1, m_IpString);
	DDV_MaxChars(pDX, m_IpString, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIpDialog, CDialog)
	//{{AFX_MSG_MAP(CIpDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpDialog message handlers

BOOL CIpDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	FILE *fp;
	TCHAR buffer[256];	
	ZeroMemory(buffer,sizeof(buffer));
	fp = _tfopen(_T("rcip.txt"),_T("rt"));
	if (fp != NULL)
	{
		while (_fgetts(buffer,256,fp) != NULL)
		{	
			buffer[_tcslen(buffer) - 1] = 0;
			m_IpComboBox.AddString(buffer);			
		}
		fclose(fp);
	}
	
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CIpDialog::PreTranslateMessage(MSG* pMsg) 
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
				TCHAR buffer[256];
				TCHAR buffer2[256];
				ZeroMemory(buffer,sizeof(buffer));

				ZeroMemory(buffer2,sizeof(buffer));

				m_IpComboBox.GetWindowText(buffer,256);

				fp = _tfopen(_T("rcip.txt"),_T("r+t"));
				
				if (fp != NULL)
				{
					fseek(fp,0,SEEK_SET);
					int flag = 0;
					while (_fgetts(buffer2,256,fp) != NULL)
					{
						buffer2[_tcslen(buffer2) - 1] = 0;
						if ( !_tcscmp(buffer2,buffer) ) {
						
							
							flag = 1;
						}
					}
					if (!flag)
					{
						_ftprintf(fp,_T("%s\n"),buffer);

					}
				}
				else
				{
					fp = _tfopen(_T("rcip.txt"),_T("wt"));
					_ftprintf(fp,_T("%s\n"),buffer);
					
				}

				fclose(fp);
					
			OnOK();
			}
			return 1;
	
		}
	
		break;
	}
	
	
	return CDialog::PreTranslateMessage(pMsg);
}
