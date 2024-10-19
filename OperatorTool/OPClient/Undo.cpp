
#include "stdafx.h"
#include "OPClient.h"

#include "OPClientDlg.h"
#include "Undo.h"
#include "Text.h"

void UnDo::AnalysisUndo(char *pData)
{
	char szBuffer[1024];
	char *pToken = NULL;
	
	strcpy(szBuffer, pData);
	pToken = strtok(szBuffer, " \n");		// �ĺ���
	pToken = strtok(NULL, " \n");

	if(strcmp(pToken, "text") == 0 || strcmp(pToken, "quest") == 0 || strcmp(pToken, "help") == 0)
		UndoText(pToken);	
}

void UnDo::AnalysisRedo(char *pData)
{
	char szBuffer[1024];
	char *pToken = NULL;
	
	strcpy(szBuffer, pData);
	pToken = strtok(szBuffer, " \n");		// �ĺ���
	pToken = strtok(NULL, " \n");

	if(strcmp(pToken, "text") == 0 || strcmp(pToken, "quest") == 0 || strcmp(pToken, "help") == 0)
		RedoText(pToken);
}

void UnDo::UndoText(char *pData)
{
	char *pToken = NULL;
	UINT id;
	int	 index;	

	CString FindString;	

	if(strcmp(pData, "text") == 0)
	{
		index = atoi(strtok(NULL, " \n"));	// �ε���
		id = atoi(strtok(NULL, " \n"));		// ���̵�
		pToken = strtok(NULL, " \n");		// ������ ����Ÿ

		memmove(pToken, &pToken[3], strlen(pToken));
		strcpy(m_pDlg->m_TextDlg.m_pText->Get(id), pToken);	
	}
	else if(strcmp(pData, "quest") == 0)
	{
		index = atoi(strtok(NULL, " \n"));	// �ε���
		id = atoi(strtok(NULL, " \n"));		// ���̵�
		pToken = strtok(NULL, "\n");		// ������ ����Ÿ

		memmove(pToken, &pToken[3], strlen(pToken));
		strcpy(m_pDlg->m_TextDlg.m_pText->GetQuestText(id), pToken);
	}
	else if(strcmp(pData, "help") == 0)
	{
		index = atoi(strtok(NULL, " \n"));	// �ε���
		id = atoi(strtok(NULL, " \n"));		// ���̵�
		pToken = strtok(NULL, "\n");		// ������ ����Ÿ
		
		memmove(pToken, &pToken[3], strlen(pToken));
		strcpy(m_pDlg->m_TextDlg.m_pText->GetHelpText(id), pToken);
	}	

	FindString.Format("%08d    %s", id, pToken);

	m_pDlg->m_TextDlg.m_ListDesc.DeleteString(index);
	m_pDlg->m_TextDlg.m_ListDesc.InsertString(index, FindString);
}

void UnDo::RedoText(char *pData)
{
	char *pToken = NULL;
	UINT id;
	int	 index;

	CString FindString;

	if(strcmp(pData, "text") == 0)
	{
		index = atoi(strtok(NULL, " \n"));	// �ε���
		id = atoi(strtok(NULL, " \n"));		// ���̵�
		pToken = strtok(NULL, " \n");		// ������ ����Ÿ		

		strcpy(m_pDlg->m_TextDlg.m_pText->Get(id), pToken);
	}
	else if(strcmp(pData, "quest") == 0)
	{
		id = atoi(strtok(NULL, " \n"));		// ���̵�
		pToken = strtok(NULL, " \n");		// ������ ����Ÿ
		pToken = strtok(NULL, " \n");		// ������ ����Ÿ

		strcpy(m_pDlg->m_TextDlg.m_pText->GetQuestText(id), pToken);	
	}
	else if(strcmp(pData, "help") == 0)
	{
		id = atoi(strtok(NULL, " \n"));		// ���̵�
		pToken = strtok(NULL, " \n");		// ������ ����Ÿ
		pToken = strtok(NULL, " \n");		// ������ ����Ÿ

		strcpy(m_pDlg->m_TextDlg.m_pText->GetHelpText(id), pToken);	
	}	

	FindString.Format("%08d    %s", id, pToken);
	m_pDlg->m_TextDlg.m_ListDesc.DeleteString(index);
	m_pDlg->m_TextDlg.m_ListDesc.InsertString(index, FindString);
}