// TextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPClient.h"
#include "TextDlg.h"

#include "OPClientDlg.h"
#include "Text.h"
#include "utility.h"
#include "TextModify.h"


#include <iostream>
#include <string>
#include <map>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog


CTextDlg::CTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextDlg)
	m_strEdit = _T("");
	//}}AFX_DATA_INIT	
}


void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextDlg)
	DDX_Control(pDX, IDC_EDIT, m_Edit);	
	DDX_Control(pDX, IDC_LIST_DESC, m_ListDesc);	
	DDX_Text(pDX, IDC_EDIT, m_strEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextDlg, CDialog)
	//{{AFX_MSG_MAP(CTextDlg)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, OnButtonText)
	ON_BN_CLICKED(IDC_BUTTON_QUEST, OnButtonQuest)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_MINI, OnButtonMini)
	ON_LBN_DBLCLK(IDC_LIST_DESC, OnDblclkListDesc)
	ON_MESSAGE(WM_RETURN_MSG, OnReturnMsg)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDlg message handlers

BOOL CTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	// 텍스트를 생성 한다.
	m_pText = new CText;
	m_pText->Init();


	// 수정용 에디트 박스
	m_Edit.ShowWindow(SW_HIDE);
	m_Edit.m_hParent = this->m_hWnd;


	m_pModify		= new TextModify;
	

	sortText();
	sortQuest();
	sortHelp();
	sortMiniGame();	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextDlg::OnButtonText() 
{
	m_ListDesc.ResetContent();

	CString TempString;

	for(int i=0; i<ON_MAX_TEXT_NUM; ++i)
	{		
		if(m_pText->Texts[i].uiID == 0)
			m_pText->Texts[i].uiID = m_pText->Texts[i-1].uiID+1;


		TempString.Format("%08d    %s", m_pText->Texts[i].uiID, m_pText->Texts[i].szText);
		m_ListDesc.AddString(TempString);
	}

	m_nNowStuts = TEXT_MODE;
}

void CTextDlg::OnButtonQuest() 
{
	m_ListDesc.ResetContent();
	
	CString TempString;

	for(int i=0; i<ON_MAX_TEXT_NUM_2048; ++i)
	{
		if(m_pText->QuestTexts[i].uiID == 0)
			m_pText->QuestTexts[i].uiID = m_pText->QuestTexts[i-1].uiID+1;

		TempString.Format("%08d    %s", m_pText->QuestTexts[i].uiID, m_pText->QuestTexts[i].szText);
		m_ListDesc.AddString(TempString);
	}
	
	m_nNowStuts = QUEST_MODE;
}

void CTextDlg::OnButtonHelp() 
{
	m_ListDesc.ResetContent();
	
	CString TempString;

	for(int i=0; i<ON_MAX_TEXT_NUM_2048; ++i)
	{
		if(m_pText->HelpTexts[i].uiID == 0)
			m_pText->HelpTexts[i].uiID = m_pText->HelpTexts[i-1].uiID+1;

		TempString.Format("%08d    %s", m_pText->HelpTexts[i].uiID, m_pText->HelpTexts[i].szText);
		m_ListDesc.AddString(TempString);
	}

	m_nNowStuts = HELP_MODE;
}

void CTextDlg::OnButtonMini() 
{
	m_ListDesc.ResetContent();
	
	
	CString TempString;

	for(int i=0; i<ON_MAX_TEXT_MINIGAME_NUM; ++i)
	{
		if(m_pText->MiniGameTexts[i].uiID == 0) continue;
		TempString.Format("%08d    %s", m_pText->MiniGameTexts[i].uiID, m_pText->MiniGameTexts[i].szText);
		m_ListDesc.AddString(TempString);
	}

	m_nNowStuts = MINIGAME_MODE;
}

BOOL CTextDlg::DestroyWindow() 
{
	delete m_pText;	
	delete m_pModify;

	return CDialog::DestroyWindow();
}

void CTextDlg::sortText()
{
	map<UINT, string, less<UINT> >	TextSort;
	map<UINT, string, less<UINT> >::iterator Find;

	BOOL bErrorMsg	= FALSE;
	FILE *fp		= fopen("겹쳐.txt", "wt");
	

	for(int i=0; i<ON_MAX_TEXT_NUM; ++i)
	{
		if(m_pText->Texts[i].uiID)
		{
			Find = TextSort.lower_bound(m_pText->Texts[i].uiID);

			if(Find->first == m_pText->Texts[i].uiID)
			{
				fprintf(fp, "%08d    %s\n", m_pText->Texts[i].uiID, m_pText->Texts[i].szText);

				bErrorMsg = bErrorMsg == FALSE ? AfxMessageBox("중복 ID 발견! 겹처.txt로 저장 합니다.") : TRUE;				
			}

			efficientAddOrUpdata(TextSort, m_pText->Texts[i].uiID, m_pText->Texts[i].szText);
		}
	}

	fclose(fp);
	ZeroMemory(m_pText->Texts, sizeof(m_pText->Texts));
	
	int nCount = 0;
	for(map<UINT, string, less<UINT> >::iterator ix=TextSort.begin(); ix!=TextSort.end(); ++ix, ++nCount)
	{
		m_pText->Texts[nCount].uiID = ix->first;
		strcpy(m_pText->Texts[nCount].szText, ix->second.c_str());
	}

	TextSort.clear();
}

void CTextDlg::sortQuest()
{
	map<UINT, string, less<UINT> >	TextSort;

	for(int i=0; i<ON_MAX_TEXT_NUM_2048; ++i)
	{
		if(m_pText->QuestTexts[i].szText[0])
			efficientAddOrUpdata(TextSort, m_pText->QuestTexts[i].uiID, m_pText->QuestTexts[i].szText);
	}
	
	ZeroMemory(m_pText->QuestTexts, sizeof(m_pText->QuestTexts));

	int nCount = 0;
	for(map<UINT, string, less<UINT> >::iterator ix=TextSort.begin(); ix!=TextSort.end(); ++ix, ++nCount)
	{
		m_pText->QuestTexts[nCount].uiID = ix->first;
		strcpy(m_pText->QuestTexts[nCount].szText, ix->second.c_str());
	}

	TextSort.clear();
}

void CTextDlg::sortHelp()
{
	map<UINT, string, less<UINT> >	TextSort;

	for(int i=0; i<ON_MAX_TEXT_NUM_2048; ++i)
	{
		if(m_pText->HelpTexts[i].uiID)
			efficientAddOrUpdata(TextSort, m_pText->HelpTexts[i].uiID, m_pText->HelpTexts[i].szText);
	}

	ZeroMemory(m_pText->HelpTexts, sizeof(m_pText->HelpTexts));

	int nCount = 0;
	for(map<UINT, string, less<UINT> >::iterator ix=TextSort.begin(); ix!=TextSort.end(); ++ix, ++nCount)
	{
		m_pText->HelpTexts[nCount].uiID = ix->first;
		strcpy(m_pText->HelpTexts[nCount].szText, ix->second.c_str());
	}

	TextSort.clear();
}

void CTextDlg::sortMiniGame()
{
	map<UINT, string, less<UINT> >	TextSort;	

	for(int i=0; i<ON_MAX_TEXT_MINIGAME_NUM; ++i)
	{
		if(m_pText->MiniGameTexts[i].uiID)
			efficientAddOrUpdata(TextSort, m_pText->MiniGameTexts[i].uiID, m_pText->MiniGameTexts[i].szText);
	}

	ZeroMemory(m_pText->MiniGameTexts, sizeof(m_pText->MiniGameTexts));

	int nCount = 0;
	for(map<UINT, string, less<UINT> >::iterator ix=TextSort.begin(); ix!=TextSort.end(); ++ix, ++nCount)
	{
		m_pText->MiniGameTexts[nCount].uiID = ix->first;
		strcpy(m_pText->MiniGameTexts[nCount].szText, ix->second.c_str());
	}

	TextSort.clear();
}

void CTextDlg::OnDblclkListDesc() 
{
	RECT rt;

	m_ListDesc.GetItemRect(m_ListDesc.GetCurSel(), &rt);

	
	m_Edit.ShowWindow(SW_SHOW);
	m_Edit.SetFocus();	

	::MoveWindow(m_Edit.m_hWnd, rt.left+64, rt.top+2, rt.right - rt.left-62, rt.bottom - rt.top+2, TRUE);

	switch(m_nNowStuts)
	{
	case TEXT_MODE:
		m_nModifyTextID = m_pText->Texts[m_ListDesc.GetCurSel()].uiID;
		m_Edit.SetWindowText(m_pText->Texts[m_ListDesc.GetCurSel()].szText);				
		break;

	case QUEST_MODE:
		m_nModifyTextID = m_pText->QuestTexts[m_ListDesc.GetCurSel()].uiID;
		m_Edit.SetWindowText(m_pText->QuestTexts[m_ListDesc.GetCurSel()].szText);
		break;

	case HELP_MODE:
		m_nModifyTextID = m_pText->HelpTexts[m_ListDesc.GetCurSel()].uiID;
		m_Edit.SetWindowText(m_pText->HelpTexts[m_ListDesc.GetCurSel()].szText);				
		break;

	case MINIGAME_MODE:
		m_nModifyTextID = m_pText->MiniGameTexts[m_ListDesc.GetCurSel()].uiID;
		m_Edit.SetWindowText(m_pText->MiniGameTexts[m_ListDesc.GetCurSel()].szText);		
		break;
	}	
}

void CTextDlg::OnReturnMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData();

	if(m_strEdit.GetLength() == 0)	return;

	int		nNum = m_ListDesc.GetCurSel();		

	switch(m_nNowStuts)
	{
	case TEXT_MODE:
		strcpy(m_pText->Texts[nNum].szText, (LPSTR)(LPCTSTR)m_strEdit);
		m_pModify->AddText(m_pText->Texts[nNum].szText, m_nModifyTextID);
		break;

	case QUEST_MODE:		
		strcpy(m_pText->QuestTexts[nNum].szText, (LPSTR)(LPCTSTR)m_strEdit);
		m_pModify->AddText(m_pText->Texts[nNum].szText, m_nModifyTextID);
		break;

	case HELP_MODE:
		strcpy(m_pText->HelpTexts[nNum].szText, (LPSTR)(LPCTSTR)m_strEdit);
		m_pModify->AddText(m_pText->Texts[nNum].szText, m_nModifyTextID);
		break;

	case MINIGAME_MODE:	
		strcpy(m_pText->MiniGameTexts[nNum].szText, (LPSTR)(LPCTSTR)m_strEdit);
		break;
	}
	
	// 우선 지우고
	m_ListDesc.DeleteString(nNum);
	CString temp;

	// 리스트 박스에 삽입
	temp.Format("%08d    %s", m_nModifyTextID, (LPSTR)(LPCTSTR)m_strEdit);
	m_ListDesc.InsertString(nNum, temp);

}

BOOL CTextDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN) return 0;	
	return CDialog::PreTranslateMessage(pMsg);
}

void CTextDlg::OnOK() 
{
	SetFileName("Text\\Text.txt");
	FILE *fp = fopen("Text\\Text.txt", "wt");

	for(int i=0; i<ON_MAX_TEXT_NUM; ++i)
		fprintf(fp, "%d \"\"%s\"\"\n", m_pText->Texts[i].uiID, m_pText->Texts[i].szText);

	fclose(fp);

	
	SetFileName("Text\\Quest_Text.txt");
	fp = fopen("Text\\Quest_Text.txt", "wt");

	for(i=0; i<ON_MAX_TEXT_NUM_2048; ++i)
		fprintf(fp, "%d \"\"%s\"\"\n", m_pText->QuestTexts[i].uiID, m_pText->QuestTexts[i].szText);
	
	fclose(fp);

	SetFileName("Text\\HelpText.txt");
	fp = fopen("Text\\HelpText.txt", "wt");

	for(i=0; i<ON_MAX_TEXT_NUM_2048; ++i)
		fprintf(fp, "%d \"\"%s\"\"\n", m_pText->HelpTexts[i].uiID, m_pText->HelpTexts[i].szText);
	
	fclose(fp);

	m_pModify->SaveAndFree();

	AfxMessageBox("텍스트 저장 완료");	
}

void CTextDlg::TextSave()
{
	OnOK();
}

void CTextDlg::OnButtonExit() 
{
	CDialog::OnOK();
}

char *CTextDlg::GetText(UINT ID)
{
	return m_pText->Get(ID); 
}

BOOL CTextDlg::Create(UINT nID, CWnd* pParentWnd) 
{
	m_pDlg = (COPClientDlg*)pParentWnd;
	return CDialog::Create(IDD, pParentWnd);
}

UINT CTextDlg::GetEmptyTextID()
{
	for(int i=0; i<ON_MAX_TEXT_NUM; ++i)
		if(m_pText->Texts[i].szText[0] == 0)	return m_pText->Texts[i].uiID;

	return 0;
}

void CTextDlg::ModifySave(char *p, UINT id)
{
	strcpy(m_pText->Get(id), p);
	m_pModify->AddText(p, id);
}
