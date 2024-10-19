//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> ����Ʈâ
//	File Name		: OnlineQuestInterface.cpp
//	Birth Date		: 2004. 01. 01.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ����� �ֿܼ��� �ӹ� ���� ��� â�� ����ִ� �κ��Դϴ�.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: ���� ��� ����
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <mouse.h>
#include "OnlineWorld.h"
#include "OnlineQuestInterface.h"
#include "../ImjinOnline/OnlineTradeBook.h"
#include "../ImjinOnline/OnlineVillageSave.h"

#include "OnlineFieldArea.h"
//#include "OnlineClient.h"		// actdoll (2004/02/16 16:58) : ��� �������� �Ǵ� ���. ��¥ �ʿ������ ���� ��� ���� ����Ǿ� �ִ� �����. �̰� ������ ���� �߻��� �ٽ� �ѱ� �ٶ�
#include "OnlineQuest-Parser.h"
#include "OnlineMegaText.h"

#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character

//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: ���� �� �����
//-----------------------------------------------------------------------------
extern _InputDevice		IpD;			// �Է� ��ġ �ܺο� ������ ��

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2004-01-01)
// Desc: Ŭ���� �ʱ�ȭ. ������ �ѹ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Init()
{
	// ���� ���� �ʱ�ȭ
	m_nNoField		= -1;
	m_bAction		= FALSE;
	m_bMousePush	= FALSE;
	pfDraw			= Draw_QuestList;
	pfAction		= Action_QuestList;

	// �̹��� �ε��Ѵ�.
	LoadImage();

	// ��ư�� ����
	m_btOK.Create		( 292 + 80,		155 + 146,								m_Spr_Btn_Common.Header.Xsize,		m_Spr_Btn_Common.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(6300005),	BUTTON_PUT_LEFT, FALSE );
	m_btClose.Create	( 262 + 187,	150 + 156,								m_Spr_Btn_Common.Header.Xsize,		m_Spr_Btn_Common.Header.Ysize,		m_pMyOnlineWorld->pOnlineText->Get(6300005),	BUTTON_PUT_LEFT, FALSE );
	m_btUp.Create		( 493,			155 + 18,								m_Spr_Btn_ScrollUp.Header.Xsize,	m_Spr_Btn_ScrollUp.Header.Ysize,	"",												BUTTON_PUT_LEFT, FALSE );
	m_btDown.Create		( 493,			155 + 18 + m_Spr_Bar_List.Header.Ysize,	m_Spr_Btn_ScrollDown.Header.Xsize,	m_Spr_Btn_ScrollDown.Header.Ysize,	"",												BUTTON_PUT_LEFT, FALSE );
	
	// ����Ʈ �ڽ� ����
	m_LB_QuestList.Create( 292 + 4, 155 + 19, 178, 100, 292+16, 155+30, 178, 100, 170, 20, NULL, &m_Spr_Btn_ScrollHandle, 292 + 207, 155 + 18 + m_Spr_Btn_ScrollDown.Header.Ysize, 155 + 18 + m_Spr_Bar_List.Header.Ysize, TRUE);
	m_LB_QuestList.EnableDrawItemColor( true );	// actdoll (2004/01/13 14:50) : ������ ���򺰷� ������ ���� ǥ���ϰԲ� �Ѵ�.
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: actdoll (2004-01-01)
// Desc: Ŭ���� ����. �Ҹ�� �ѹ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Free()
{
	// �̹��� �����Ѵ�.
	FreeImage();
}

//-----------------------------------------------------------------------------
// Name: LoadImage()
// Code: actdoll (2004-01-01)
// Desc: ��������Ʈ �̹��� �ε�
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::LoadImage()
{
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_1.Spr",			m_Spr_Frm_List );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_2.Spr",			m_Spr_Frm_Info );
	
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_scr_up.Spr",		m_Spr_Btn_ScrollUp );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_scr_down.Spr",	m_Spr_Btn_ScrollDown );
	
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_1_SCRBAR.Spr",	m_Spr_Bar_List );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\QUEST_2_SCRBAR.Spr",	m_Spr_Bar_Info );
	
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_scr_handle.Spr",	m_Spr_Btn_ScrollHandle );
	clGrp.LoadXspr( "Online\\GameSpr\\Quest\\quest_button.Spr",		m_Spr_Btn_Common );	
}

//-----------------------------------------------------------------------------
// Name: FreeImage()
// Code: actdoll (2004-01-01)
// Desc: ��������Ʈ �̹��� ����
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::FreeImage()
{
	clGrp.FreeXspr( m_Spr_Frm_List );
	clGrp.FreeXspr( m_Spr_Frm_Info );

	clGrp.FreeXspr( m_Spr_Btn_ScrollUp );
	clGrp.FreeXspr( m_Spr_Btn_ScrollDown );
	
	clGrp.FreeXspr( m_Spr_Bar_List );
	clGrp.FreeXspr( m_Spr_Bar_Info );
	
	clGrp.FreeXspr( m_Spr_Btn_ScrollHandle );
	clGrp.FreeXspr( m_Spr_Btn_Common );	
}

//-----------------------------------------------------------------------------
// Name: Draw_QuestList()
// Code: actdoll (2004-01-01)
// Desc: ����Ʈ â ��� - ����Ʈ ����Ʈ
//		pSurface	- ���� ȭ���� ���ǽ�
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Draw_QuestList( LPDIRECTDRAWSURFACE7 pSurface )
{	
	// �׸�
	if(clGrp.LockSurface(pSurface))
	{
		// ������ ���
		clGrp.PutSpriteJin(292, 155, m_Spr_Frm_List.Header.Xsize, m_Spr_Frm_List.Header.Ysize, &m_Spr_Frm_List.Image[m_Spr_Frm_List.Header.Start[0]]);
		clGrp.PutSpriteT(292, 155, m_Spr_Frm_List.Header.Xsize, m_Spr_Frm_List.Header.Ysize, &m_Spr_Frm_List.Image[m_Spr_Frm_List.Header.Start[1]]);

		// OK��ư ���
		m_btOK.Put(&m_Spr_Btn_Common, 0, 0, 1, BUTTON_PUT_NOMOVE );

//		if(IsShowMainScroll())
		{
			// ��ũ�� �� ���
			clGrp.PutSpriteT(495, 173, m_Spr_Bar_List.Header.Xsize, m_Spr_Bar_List.Header.Ysize, m_Spr_Bar_List.Image);

			// ���Ʒ� ��ũ�� ��ư ���
			m_btUp.Put(&m_Spr_Btn_ScrollUp, 0, 0, 1, BUTTON_PUT_NOMOVE);
			m_btDown.Put(&m_Spr_Btn_ScrollDown, 0, 0, 1, BUTTON_PUT_NOMOVE);
		}

		clGrp.UnlockSurface(pSurface);
	}

	// �۾�
	HDC hDC;
	if(pSurface->GetDC(&hDC) == DD_OK)
	{		
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 297, 159, 206, 13, m_pMyOnlineWorld->pOnlineText->Get(8000567),RGB(245, 245, 245));	// ���� ǥ����

		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));		
		m_btOK.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pSurface->ReleaseDC(hDC);
	}

//	if(IsShowMainScroll())
	// ����Ʈ ����Ʈ ���
	m_LB_QuestList.Draw( pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), RGB(64, 64, 255), RGB(0, 0, 0), true, m_SelectColor, RGB(200, 200, 200) );
}

//-----------------------------------------------------------------------------
// Name: Draw_QuestInfo()
// Code: actdoll (2004-01-01)
// Desc: ����Ʈ â ��� - ����Ʈ ������
//		pSurface	- ���� ȭ���� ���ǽ�
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Draw_QuestInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	// �׸�
	if(clGrp.LockSurface(pSurface))
	{
		// ������ ���
		clGrp.PutSpriteJin(262, 150, m_Spr_Frm_Info.Header.Xsize, m_Spr_Frm_Info.Header.Ysize, &m_Spr_Frm_Info.Image[m_Spr_Frm_Info.Header.Start[0]]);
		clGrp.PutSpriteT(262, 150, m_Spr_Frm_Info.Header.Xsize, m_Spr_Frm_Info.Header.Ysize, &m_Spr_Frm_Info.Image[m_Spr_Frm_Info.Header.Start[1]]);

		// ��ư ���
		m_btOK.Put(&m_Spr_Btn_Common, 0, 0, 1, BUTTON_PUT_NOMOVE);
		m_btClose.Put(&m_Spr_Btn_Common, 0, 0, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	// �۾�
	HDC hDC;
	char	pszTemp[512], pszBuff[512], pszCount[64] = {0,};	
	if(pSurface->GetDC(&hDC) == DD_OK)
	{		
		SetBkMode(hDC, TRANSPARENT);
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 271, 153, 267, 13, m_pMyOnlineWorld->pOnlineText->Get(8000568),RGB(245, 245, 245));		// ����ǥ����
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 266, 170, 74, 18, m_pMyOnlineWorld->pOnlineText->Get(8000093), RGB(245, 245, 245));		// "�ӹ�"
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 266, 191, 74, 18, m_pMyOnlineWorld->pOnlineText->Get(8000402), RGB(245, 245, 245));		// "�ӹ������"

		// ������ �������� ����Ʈ �ڵ带 �ް�
		SQuestHeader *pQH = (SQuestHeader*)m_LB_QuestList.GetData(m_siSelect);
		if( !pQH )
		{
			pSurface->ReleaseDC(hDC);
			return;
		}
		SI32	siSelectedQuestCode = pQH->siQuestCode;

//		pQH->sQuestReqConditional.m_QuestConditionalQuantity;
//		m_QuestData.m_sPlayingQuest[TempCount]
		char	*pTextTitle	= m_pMyOnlineWorld->pOnlineText->GetQuestText(pQH->siSubject);
		char	*pTextNPC	= m_pMyOnlineWorld->pOnlineText->Get(m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetQuestNPCAllMap(pQH->siNPCCode)->siName);
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 262+88, 150+23, pTextTitle, RGB(245, 245, 245));	// �ӹ� �����
		m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, 262+88, 150+43, pTextNPC, RGB(245, 245, 245));		// ��� NPC �̸�

		// �ӹ� ���� ���� ����
		char			*pText;
		SI32			siLimitCount, siCurrentCount;		// ������ ���߱����� ���� ��ġ�� ���� ��ġ
		SI32			siRealSelIndex;
		COLORREF		textColor;
		SI32			siObjectCountForFinish;				// 
		SQuestHeader*	lpSQuestHeader;
		BOOL			bOpen		= false;
		BOOL			bIsPlaying	= false;

		// ������ ����Ʈ�� �÷��������� �ذ��� ������ ã�´�.
		for( SI32 i=0; i<ON_QUSETDATA_MAXPLAYINGQUEST; i++ )
		{
			// ���� �÷������� ����Ʈ���
			if( m_QuestData.m_sPlayingQuest[i].m_QuestCode == siSelectedQuestCode )		
			{
				siRealSelIndex	= i;		// �迭 ��ȣ �־��ְ�
				bIsPlaying		= true;		// �������̴�.
				break;
			}
		}

		// ������ ����ش�.
		strcpy( pszTemp, m_pMyOnlineWorld->pOnlineText->GetQuestText(pQH->m_siDesc) );	// ���� ���

		// ����Ʈ ���� ��´�.
		if( bIsPlaying )		// �������̶��
		{
			if( siRealSelIndex >= 0 )
			{
				// ����Ʈ ��� �ް�
				lpSQuestHeader = m_pMyOnlineWorld->pQuestParser->GetQuest( m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestCode );
				
				// ����Ʈ ���Ḧ ���� �ʿ��� ��� ������ ������.
				for( siObjectCountForFinish = 0; siObjectCountForFinish < ON_QUSETDATA_MAXQUESTENDREQOBJECT; siObjectCountForFinish++ )
				{
					// ���� ������ ����ִ� ����� �ް�
					SQuestObject	*pEndQuestObject = &lpSQuestHeader->sQuestEndReqObject[siObjectCountForFinish];
					// �ش� Ÿ���� ������ ��
					if( pEndQuestObject->m_QuestObjectType != QUESTOBJECTTYPE_NONE )
					{
						if( !bOpen )	// ù���϶��� ���� ���ְ�
						{
							strcat( pszTemp, "\n\n[" );
							bOpen = true;
						}
						else			// �׷��� ���� ���� ������ ���ش�.
						{
							strcat( pszTemp, ", " );
						}
						
						siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;		// �ش� ���ǿ� ���� ������ �� �ִ� ��ġ �ް�
						siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
						switch(pEndQuestObject->m_QuestObjectType)	// �䱸�ϴ� ������ üũ�Ѵ�.
						{
						case QUESTOBJECTTYPE_ITEM:		// �������� ��������� �䱸
							{
								// �ش� �����۸� : ���簹�� / ���ް���
								CItemHeader		*lpCItemHeader;
								lpCItemHeader	= m_pMyOnlineWorld->pItemsInfo->GetItem( pEndQuestObject->m_QuestObjectCode );
								
								pText			= m_pMyOnlineWorld->pOnlineText->Get( lpCItemHeader->m_siItemName );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetItemCount( pEndQuestObject->m_QuestObjectCode );
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_WORK:		// ���� �϶�� �䱸
							{
								// �ش� �۾��� : ���� �۾��� / ���� �۾���
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_WORKDATA );
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_HEALTH:		// ü���� ä����(?)
							{
								// �ش� �۾��� : ���� ü�� / �ִ� ü��
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_WORKDATA );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife( 0 );
								siLimitCount	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife( 0 );
								
								sprintf( pszBuff, " %s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
							
						case QUESTOBJECTTYPE_BANKBOOK:		// ���� �����
							{
								// �ش� �۾��� : ���� ���¼� / �ִ� ���¼�
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_BANKACCOUNT );
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %I64d/%d", pText, m_pMyOnlineWorld->pOnlineTradeBook->Quest_GetBankBookCount(), siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_PORTAL:		// ����Ÿ� ���ƴ���
							{
								// �ش� �۾��� : ���� ��  / �ִ� ��
								pText			= m_pMyOnlineWorld->pOnlineText->Get(8000569);
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_NPC:			// NPC ������
							{
								// �ش� �۾��� : ���� �� / �ִ� ��
								pText			= m_pMyOnlineWorld->pOnlineText->Get(8000570);
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_BOOK:			// ������ �湮�ϱ�
							{	
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_TRADEBOOK );
								siCurrentCount	= m_pMyOnlineWorld->pOnlineVillageSave->GetVillageNum();
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_MERCENARY:		// �뺴 ����ϱ�
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_SOILDERCOUNT );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetFollowerNum() - 1;
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_MONSTER:		// ���� ���
							{
								CharHead*		lpCharHead;
								lpCharHead		= m_pMyOnlineWorld->pIOnlineCharKI->GetCharHeader( pEndQuestObject->m_QuestObjectCode );
								
								pText			= m_pMyOnlineWorld->pOnlineText->Get( lpCharHead->uiCharName );
								siCurrentCount	= m_QuestData.m_sPlayingQuest[siRealSelIndex].m_QuestObject[siObjectCountForFinish];
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
							
						case QUESTOBJECTTYPE_SATIETY:		// ������ ä���
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_HUNGRY );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetHealth();
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_LEVEL:			// ���� �ø���
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INFO_LV );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( 0 );
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff,"%s : %d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
							
						case QUESTOBJECTTYPE_QUEST:			// ����Ʈ ����
							{
								SQuestHeader*	lpTemp;
								lpTemp			= m_pMyOnlineWorld->pQuestParser->GetQuest( siLimitCount );
								
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_QUESTINFO_QUESTNO );
								for( SI32 j=0; j<ON_QUSETDATA_MAXCOMLETEDQUEST; j++ )
								{
									if( m_QuestData.m_siCompletedQuest[j] > 0 )
									{
										if( m_QuestData.m_siCompletedQuest[j] >= siLimitCount )
										{
											pText	= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_QUESTINFO_QUESTOK );
											break;
										}
									}
								}
								sprintf( pszBuff, pText, m_pMyOnlineWorld->pOnlineText->GetQuestText( lpTemp->siSubject ) );
							}
							break;
							
						case QUESTOBJECTTYPE_MONEY:			// ��
							{
								pText			= m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_QUESTINFO_MONEY );
								siCurrentCount	= m_pMyOnlineWorld->pMyData->GetMoney();
								siLimitCount	= pEndQuestObject->m_QuestObjectQuantity;
								
								sprintf( pszBuff, "%s : %I64d/%d", pText, siCurrentCount, siLimitCount );
							}
							break;
						}// eof switch()
						// ������ ������
						strcat( pszTemp, pszBuff );
					}
				}
			}
			
			// ��ȣ�� �Ϸ�� ���¿��� Ƣ��Դٸ� �ݾƶ�
			if( bOpen )
			{
				strcat( pszTemp, "]" );
			}
			textColor = RGB( 255, 128, 128 );	
		}
		else		// ���� ���̶��
		{
			strcat( pszTemp, "\n\n" );
			strcat( pszTemp, m_pMyOnlineWorld->pOnlineText->Get(8000571) );			// �۾��Ϸ�
			textColor = RGB( 128, 128, 255 );		
		}
//		m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine( hDC, 262+16, 150+69, 239, pszTemp, textColor );	// �ӹ� ����
		m_pMyOnlineWorld->pOnlineMegaText->DrawTextInBox( hDC, 262+16, 150+69, 242, 292, NULL, pszTemp, textColor, DT_VCENTER );
		// ��ư �ؽ�Ʈ
		SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
		m_btOK.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_btClose.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hDC);
	}
}

//-----------------------------------------------------------------------------
// Name: Action_QuestList()
// Code: actdoll (2004-01-01)
// Desc: ����Ʈ â �۵� - ����Ʈ ����Ʈ
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Action_QuestList()
{
	// ��ư ���ȳ� Ȯ��
	if( m_btOK.Process	( m_bMousePush ) )		OnButtonPush();				// OK
	if( m_btUp.Process	( m_bMousePush ) )		m_LB_QuestList.PrevSell();	// �� ��ũ��
	if( m_btDown.Process	( m_bMousePush ) )	m_LB_QuestList.NextSell();	// �Ʒ� ��ũ��

	// ����Ʈ �ڽ� ���� ü
	m_LB_QuestList.Action(IpD.Mouse_X, IpD.Mouse_Y, m_bMousePush, &m_siSelect);

	// ������ ���� ���
	if(m_siSelect > -1)
	{
		// ������ �Լ� ������ �����ְ�
		pfDraw   = Draw_QuestInfo;
		pfAction = Action_QuestInfo;
		
		// ��ư ��ǥ ����
		m_btOK.SetX(262+32);
		m_btOK.SetY(150+156);

		// ��ư�� ��� üũ
		m_btOK.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(1000114));
		m_btClose.SetAction(TRUE);

		// actdoll (2004/01/16 17:31) : ������ �������� ����Ʈ �ڵ带 �ް�
		SQuestHeader *pQH = (SQuestHeader*)m_LB_QuestList.GetData(m_siSelect);
		if( !pQH )	return;
		
		// ������ ����Ʈ�� �÷��������� �ذ��� ������ ã�´�.
		m_SelectColor	= RGB( 128, 128, 255 );
		for( SI32 i=0; i<ON_QUSETDATA_MAXPLAYINGQUEST; i++ )
		{
			// ���� �÷������� ����Ʈ���
			if( m_QuestData.m_sPlayingQuest[i].m_QuestCode == pQH->siQuestCode )		
			{
				m_SelectColor	= RGB( 255, 128, 128 );
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Action_QuestInfo()
// Code: actdoll (2004-01-01)
// Desc: ����Ʈ â �۵� - ����Ʈ ����
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::Action_QuestInfo()
{
	BOOL	bPushOK		= m_btOK.Process(m_bMousePush);
	BOOL	bPushClose	= m_btClose.Process(m_bMousePush);

	// ��ư �ϰ͵� �ȴ��ȴٸ� ����
	if( !bPushOK && !bPushClose )		return;

	// ��ġ ����
	m_btOK.SetX(292+80);
	m_btOK.SetY(155+146);
	
	// ��ư�� ����
	m_btOK.SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(6300005));
	m_btClose.SetAction(FALSE);
	
	// �Լ� ������ ����
	pfDraw   = Draw_QuestList;
	pfAction = Action_QuestList;		

	// �ݱ� ��ư�� ���� ���̶�� ó��
	if( bPushClose )		OnButtonPush();
}

//-----------------------------------------------------------------------------
// Name: SetQuestList()
// Code: actdoll (2004-01-01)
// Desc: ����Ʈ ����Ʈ â�� �ش� ����Ʈ���� ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::SetQuestList()
{
	int		TempCount, nCount = 1;
	char	szTemp[512];

	// �ϴ� Ȧ�� �����
	m_LB_QuestList.DelAllSell();
	m_LB_QuestList.ClearItemColor();

	// ���� �����ϰ� �ִ� ����Ʈ ����Ʈ ���� ����
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			// �ش� ���� ����Ʈ ����� �ް�
			m_QuestData.m_sPlayingQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode);
			sprintf(szTemp, "%d. %s", nCount, m_pMyOnlineWorld->pOnlineText->GetQuestText(m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode)->siSubject) );
			m_LB_QuestList.AddSell(m_QuestData.m_sPlayingQuestHeader[TempCount],szTemp);
			m_LB_QuestList.SetItemColor( nCount - 1, RGB( 255, 64, 64 ) );
			nCount++;
		}
	}

	// �̹� ���� ����Ʈ ����Ʈ�� ����
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] > 0)
		{
			m_QuestData.m_siCompletedQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_siCompletedQuest[TempCount]);

			sprintf(szTemp, "%d. %s", nCount, m_pMyOnlineWorld->pOnlineText->GetQuestText(m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_siCompletedQuest[TempCount])->siSubject));
			m_LB_QuestList.AddSell(m_QuestData.m_siCompletedQuestHeader[TempCount],szTemp);
			nCount++;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-01-01)
// Desc: ���� ó��
//-----------------------------------------------------------------------------
void OnlineQuestInterface::Action()
{
	if(!IsAction())		return;

	(this->*pfAction)();

	m_bMousePush = IpD.LeftPressSwitch;
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-01-01)
// Desc: ���� ���
//-----------------------------------------------------------------------------
void OnlineQuestInterface::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!IsAction())		return;

	(this->*pfDraw)(pSurface);
}

//-----------------------------------------------------------------------------
// Name: OnButtonPush()
// Code: actdoll (2004-01-01)
// Desc: �� �ܼ��� ȣ���ϴ� ��ư�� ���ȴ�!
//-----------------------------------------------------------------------------
void OnlineQuestInterface::OnButtonPush()
{
	ToggleAction();

//	FreeImage();

	m_btOK.SetAction(IsAction());
	m_btUp.SetAction(IsAction());
	m_btDown.SetAction(IsAction());


	if(m_nNoField != -1)	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_nNoField);
	m_nNoField = -1;

	if(IsAction())
	{
		m_pMyOnlineWorld->pOnlineClient->SendRequestQuestGetBankBookCount();

//		LoadImage();

		m_nNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(292, 155, 292+m_Spr_Frm_List.Header.Xsize, 155+m_Spr_Frm_List.Header.Ysize);
	}
}

//-----------------------------------------------------------------------------
// Name: GetCompletedQuestCount()
// Code: actdoll (2004-01-01)
// Desc: �Ϸ�� ����Ʈ�� ������ �޴´�.
//-----------------------------------------------------------------------------
SI16	OnlineQuestInterface::GetCompletedQuestCount()
{
	SI16		TempCount;
	SI16		TempQuestCount;

	TempQuestCount = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] > 0)
		{
			TempQuestCount++;
		}
	}

	return TempQuestCount;
}

//-----------------------------------------------------------------------------
// Name: GetCompletedQuestCount()
// Code: actdoll (2004-01-01)
// Desc: �������� ����Ʈ�� ������ �޴´�.
//-----------------------------------------------------------------------------
SI16 OnlineQuestInterface::GetPlayingQuestCount()
{
	SI16		TempCount;
	SI16		TempQuestCount;

	TempQuestCount = 0;
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			TempQuestCount++;
		}
	}

	return TempQuestCount;
}




//===================================================================================================
//	�������� ���� ���� �� ó��	- START
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: RecvMsg_SetQuestData()
// Code: actdoll (2004-01-01)
// Desc: �������� ���� ���� - �ش� ĳ������ ����Ʈ ���� ������ �����Ѵ�. �� ĳ���ͷ� �ε��� ���� ��������� ��������.
//		PlayingQuestCount		- �������� ����Ʈ�� ����
//		CompletedQuestCount		- ����� ����Ʈ�� ����
//		lpPlayingQuestData		- �������� ����Ʈ�� �� ������ ����ִ� ������
//		lpCompletedQuestData	- ����� ����Ʈ�� �ε����� ����ִ� ������
//		return					- ������ true ����
//-----------------------------------------------------------------------------
BOOL	OnlineQuestInterface::RecvMsg_SetQuestData(SI16 PlayingQuestCount, SI16 CompletedQuestCount, SPlayingQuest* lpPlayingQuestData, SI16* lpCompletedQuestData)
{
	SI32	TempCount;
	BOOL	bTempQuestComplete;

	bTempQuestComplete = FALSE;
	ZeroMemory(&m_QuestData, sizeof(SPlayerQuest));

	// ���� �����ϰ� �ִ� ����Ʈ ����Ʈ�� ������ �޾ƿ� ���ۿ� �����Ѵ�.
	CopyMemory(m_QuestData.m_sPlayingQuest, lpPlayingQuestData, sizeof(SPlayingQuest) * PlayingQuestCount);
	for(TempCount = 0; TempCount < PlayingQuestCount; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode > 0)
		{
			m_QuestData.m_sPlayingQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode);
			bTempQuestComplete = TRUE;
		}
	}

	// ���� �Ϸ�� ����Ʈ ����Ʈ�� ������ �޾ƿ� ���ۿ� �����Ѵ�.
	CopyMemory(m_QuestData.m_siCompletedQuest, lpCompletedQuestData, sizeof(SI16) * CompletedQuestCount);
	for(TempCount = 0; TempCount < CompletedQuestCount; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] > 0)
		{
			m_QuestData.m_siCompletedQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(m_QuestData.m_siCompletedQuest[TempCount]);
			bTempQuestComplete = TRUE;
		}
	}

	// ���ۿ� ����ִ� ����Ʈ ������ ����Ʈ �ڽ��� ǥ���Ѵ�.
	SetQuestList();

	// ����Ʈ ����Ʈ�� �ϳ��� ���� ��� �� �������� �޽���â�� ���
	// ��� ���۵����� ���� �޽����� ����ڿ��� �����ش�.
	if(!bTempQuestComplete)
	{
		UI32	uiCharID	= m_pMyOnlineWorld->pMyData->GetFollowerCharID( 0 );
		// �ѱ� ĳ������ ���(��ö��, �̸�ȭ)
		if		( ( uiCharID == KIND_ON_KEON )		|| ( uiCharID == KIND_ON_MYUNGHWA ) )	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000210));
		// �Ϻ� ĳ������ ���(�ø��� ��, �ճ� �ϳ�����)
		else if	( ( uiCharID == KIND_ON_RYO )		|| ( uiCharID == KIND_ON_HANAHINE ) )	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000211));
		// �븸 ĳ������ ���(�ռҷ�, �չ̷�)
		else if	( ( uiCharID == KIND_ON_SORYUNG )	|| ( uiCharID == KIND_ON_MIRYUNG ) )	m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000214));
		// �߱� ĳ������ ���(��ȣ��, �����)
		else if	( ( uiCharID == KIND_ON_DANG )		|| ( uiCharID == KIND_ON_SUYOUN ) )		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(1000215));	
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_SetPlayingQuestData()
// Code: actdoll (2004-01-01)
// Desc: �������� ���� ���� - �������� ����Ʈ�� ������ ������� ��� ��������.
//		lpPlayingQuestData		- �������� ����Ʈ�� �� ������ ����ִ� ������
//		return					- ������ true ����
//-----------------------------------------------------------------------------
BOOL	OnlineQuestInterface::RecvMsg_SetPlayingQuestData(SPlayingQuest* lpPlayingQuestData)
{
	SI32		TempCount;

	// �������� ����Ʈ�� �˻��ؼ� ��ġ�ϴ� �ε����� �ִٸ� ���� ������ ������ �������ش�.
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode == lpPlayingQuestData->m_QuestCode)
		{
			CopyMemory(&m_QuestData.m_sPlayingQuest[TempCount], lpPlayingQuestData, sizeof(SPlayingQuest));
			break;
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_ClearAllQuest()
// Code: actdoll (2004-01-01)
// Desc: �������� ���� ���� - ���� ����ִ� ��� ����Ʈ ������ ����������.
//-----------------------------------------------------------------------------
void	OnlineQuestInterface::RecvMsg_ClearAllQuest()
{
	ZeroMemory(m_QuestData.m_siCompletedQuest,       sizeof(SI16) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	ZeroMemory(m_QuestData.m_siCompletedQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXCOMLETEDQUEST);
	
	ZeroMemory(m_QuestData.m_sPlayingQuest,       sizeof(SPlayingQuest) * ON_QUSETDATA_MAXPLAYINGQUEST);
	ZeroMemory(m_QuestData.m_sPlayingQuestHeader, sizeof(SQuestHeader*) * ON_QUSETDATA_MAXPLAYINGQUEST);

	m_LB_QuestList.DelAllSell();
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_QuestStarted()
// Code: actdoll (2004-01-01)
// Desc: �������� ���� ���� - ������� ���� ����Ʈ�� �������� ���� ó��
//		QuestCode	- ������ ����Ʈ�� �ڵ�
//		return		- ������ true
//-----------------------------------------------------------------------------
BOOL OnlineQuestInterface::RecvMsg_QuestStarted(SI16 QuestCode)
{
	SI32		TempCount;

	// ���� ���� ����Ʈ ������ �����Ѵ�.
	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode <= 0)
		{
			ZeroMemory(&m_QuestData.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
			m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode = QuestCode;
			m_QuestData.m_sPlayingQuestHeader[TempCount]       = m_pMyOnlineWorld->pQuestParser->GetQuest(QuestCode);
			break;
		}
	}

	// ����Ʈ ����Ʈ ����
	SetQuestList();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_QuestFinished()
// Code: actdoll (2004-01-01)
// Desc: �������� ���� ���� - ����Ʈ�� ��� Ȥ�� �Ϸ� ������ �������� ���� ó��
//		QuestCode	- ����� ����Ʈ�� �ڵ�
//		return		- ������ true
//-----------------------------------------------------------------------------
BOOL OnlineQuestInterface::RecvMsg_QuestFinished(SI16 QuestCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXPLAYINGQUEST; TempCount++)
	{
		if(m_QuestData.m_sPlayingQuest[TempCount].m_QuestCode == QuestCode)
		{
			ZeroMemory(&m_QuestData.m_sPlayingQuest[TempCount], sizeof(SPlayingQuest));
			m_QuestData.m_sPlayingQuestHeader[TempCount]       = NULL;
			break;
		}
	}

	// ����Ʈ ����Ʈ ����
	SetQuestList();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: RecvMsg_AddCompletedQuest()
// Code: actdoll (2004-01-01)
// Desc: �������� ���� ���� - �Ϸ��� ����Ʈ�� �ڵ带 ó��
//		QuestCode	- �Ϸ�� ����Ʈ�� �ڵ�
//		return		- ������ true
//-----------------------------------------------------------------------------
BOOL OnlineQuestInterface::RecvMsg_AddCompletedQuest(SI16 QuestCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < ON_QUSETDATA_MAXCOMLETEDQUEST; TempCount++)
	{
		if(m_QuestData.m_siCompletedQuest[TempCount] <= 0)
		{
			m_QuestData.m_siCompletedQuest[TempCount]       = QuestCode;
			m_QuestData.m_siCompletedQuestHeader[TempCount] = m_pMyOnlineWorld->pQuestParser->GetQuest(QuestCode);
			break;
		}
	}

	// ����Ʈ ����Ʈ ����
	SetQuestList();

	return TRUE;
}

//===================================================================================================
//	�������� ���� ���� �� ó��	- END
//===================================================================================================


