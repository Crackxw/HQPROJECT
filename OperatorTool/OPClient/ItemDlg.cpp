// ItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPClient.h"
#include "ItemDlg.h"

#include "Graphics16.h"
#include "OPClientDlg.h"
#include "ItemParser.h"

#include "utility.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

map<int, string, less<int> > g_Unit;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemDlg dialog


CItemDlg::CItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemDlg)
	m_ElementNum[0] = 0;
	m_ElementNum[1] = 0;
	m_ElementNum[2] = 0;
	m_ElementNum[3] = 0;
	m_ElementNum[4] = 0;
	m_nSource = 0;
	m_nID = 0;
	m_nWork = 0;
	m_nHp = 0;
	m_nMp = 0;
	m_nStr = 0;
	m_nDex = 0;
	m_nInt = 0;
	m_nVit = 0;
	m_nAc = 0;
	m_nDamageRegist = 0;
	m_nMagicRegist = 0;
	m_nSound = 0;
	m_nHealth = 0;
	m_nFontL = 0;
	m_nFontS = 0;
	m_nMaxQuantity = 0;
	m_nAttUp = 0;
	m_nDefUp = 0;
	m_nMinD = 0;
	m_nMaxD = 0;
	m_nAttackRange = 0;
	m_nReqLv = 0;
	m_nMagicAttack = 0;
	m_strDescription = _T("");
	m_strMagicDescription = _T("");
	m_nDefence = 0;
	m_nDrawID = 0;
	m_strModify = _T("");
	//}}AFX_DATA_INIT

	m_pSpr = new XSPR;
	m_pSpr->Image = NULL;
}


void CItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemDlg)
	DDX_Control(pDX, IDC_COMBO_MAGIC, m_AttackType);
	DDX_Control(pDX, IDC_COMBO_ATTACK_TYPE, m_Magic);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_Unit);
	DDX_Control(pDX, IDC_COMBO_KIND, m_Kind);
	DDX_Control(pDX, IDC_COMBO_ATT, m_Attribute);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_Group);
	DDX_Control(pDX, IDC_COMBO_IMAGES, m_ImageS);
	DDX_Control(pDX, IDC_COMBO_IMAGEL, m_ImageL);
	DDX_Control(pDX, IDC_COMBO_TOOL3, m_Tool[2]);
	DDX_Control(pDX, IDC_COMBO_TOOL2, m_Tool[1]);
	DDX_Control(pDX, IDC_COMBO_TOOL1, m_Tool[0]);
	DDX_Control(pDX, IDC_COMBO_ELEMENT05, m_Element[4]);
	DDX_Control(pDX, IDC_COMBO_ELEMENT04, m_Element[3]);
	DDX_Control(pDX, IDC_COMBO_ELEMENT03, m_Element[2]);
	DDX_Control(pDX, IDC_COMBO_ELEMENT02, m_Element[1]);
	DDX_Control(pDX, IDC_COMBO_ELEMENT01, m_Element[0]);
	DDX_Control(pDX, IDC_COMBO1, m_ItemName);
	DDX_Text(pDX, IDC_EDIT_ID, m_nID);
	DDX_Text(pDX, IDC_EDIT_ELEMENT1, m_ElementNum[0]);
	DDX_Text(pDX, IDC_EDIT_ELEMENT2, m_ElementNum[1]);
	DDX_Text(pDX, IDC_EDIT_ELEMENT3, m_ElementNum[2]);
	DDX_Text(pDX, IDC_EDIT_ELEMENT4, m_ElementNum[3]);
	DDX_Text(pDX, IDC_EDIT_ELEMENT5, m_ElementNum[4]);
	DDX_Text(pDX, IDC_EDIT_WORK, m_nWork);
	DDX_Text(pDX, IDC_EDIT_HP, m_nHp);
	DDX_Text(pDX, IDC_EDIT_MP, m_nMp);
	DDX_Text(pDX, IDC_EDIT_STR, m_nStr);
	DDX_Text(pDX, IDC_EDIT_DEX, m_nDex);
	DDX_Text(pDX, IDC_EDIT_INT, m_nInt);
	DDX_Text(pDX, IDC_EDIT_VIT, m_nVit);
	DDX_Text(pDX, IDC_EDIT_AC, m_nAc);
	DDX_Text(pDX, IDC_EDIT_DR, m_nDamageRegist);
	DDX_Text(pDX, IDC_EDIT_MR, m_nMagicRegist);
	DDX_Text(pDX, IDC_EDIT_SOUND, m_nSound);
	DDX_Text(pDX, IDC_EDIT_HEALTH, m_nHealth);
	DDX_Text(pDX, IDC_EDIT_IMAGEL_FONT, m_nFontL);
	DDX_Text(pDX, IDC_EDIT_IMAGES_FONT, m_nFontS);
	DDX_Text(pDX, IDC_EDIT_MAXQUANTITY, m_nMaxQuantity);
	DDX_Text(pDX, IDC_EDIT_ATTUP, m_nAttUp);
	DDX_Text(pDX, IDC_EDIT_DEFUP, m_nDefUp);
	DDX_Text(pDX, IDC_EDIT_MINDAMAGE, m_nMinD);
	DDX_Text(pDX, IDC_EDIT_MAXDAMAGE, m_nMaxD);
	DDX_Text(pDX, IDC_EDIT_MAXDAMAGE2, m_nAttackRange);
	DDX_Text(pDX, IDC_EDIT_REQ_LV, m_nReqLv);
	DDX_Text(pDX, IDC_EDIT_MAGIC_ATTACK, m_nMagicAttack);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_DESC2, m_strMagicDescription);
	DDX_Text(pDX, IDC_EDIT_DEFENCE, m_nDefence);
	DDX_Text(pDX, IDC_EDIT_DRAWID, m_nDrawID);
	DDX_Text(pDX, IDC_EDIT_MODIFY, m_strModify);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_nSource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemDlg, CDialog)
	//{{AFX_MSG_MAP(CItemDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeItemName)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SAVEITEM, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_OK, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelchangeComboGroup)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemDlg message handlers

BOOL CItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pItemParser = new ItemParser;

	if(AfxMessageBox("Item 로드\nDat 파일로 로드 할까요", MB_YESNO|MB_ICONQUESTION|MB_TOPMOST) == IDNO)
	{
		m_pItemParser->ItemLoad(m_pDlg->GetOnlineText(), "Data\\Item\\weapon.txt",
			"Data\\Item\\armor.txt",
			"Data\\Item\\accessary.txt", 
			"Data\\Item\\Animal.txt", 
			"Data\\Item\\Dress.txt", 
			"Data\\Item\\Element.txt",
			"Data\\Item\\Food.txt", 
			"Data\\Item\\Liquor.txt", 
			"Data\\Item\\Medicine.txt", 
			"Data\\Item\\Tool.txt",
			"Data\\Item\\Trade.txt");
	}
	else
	{
		m_pItemParser->Load(m_pDlg->GetOnlineText(), "Text\\Item.dat");
	}

	for(int i=0; i<m_pItemParser->GetCount(); ++i)
	{
		m_ItemName.AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Element[0].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Element[1].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Element[2].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Element[3].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Element[4].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));

		m_Tool[0].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Tool[1].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
		m_Tool[2].AddString(m_pDlg->GetText(m_pItemParser->GetItemHeader(i)->m_siItemName));
	}


	// 뷰 생성
	RECT rt;
	GetDlgItem(IDC_ITEM_VIEW)->GetWindowRect(&rt);
	ScreenToClient(&rt);

	rt.right  -= 2;
	rt.top	  += 6;
	rt.bottom -= 11;



	m_DC.Create(rt);

	// 단위
	efficientAddOrUpdata(g_Unit, 48, m_pDlg->GetText(48));
	efficientAddOrUpdata(g_Unit, 502, m_pDlg->GetText(502));
	efficientAddOrUpdata(g_Unit, 55, m_pDlg->GetText(55));
	efficientAddOrUpdata(g_Unit, 503, m_pDlg->GetText(503));
	efficientAddOrUpdata(g_Unit, 501, m_pDlg->GetText(501));
	efficientAddOrUpdata(g_Unit, 248, m_pDlg->GetText(248));
	efficientAddOrUpdata(g_Unit, 47, m_pDlg->GetText(47));
	efficientAddOrUpdata(g_Unit, 500, m_pDlg->GetText(500));
	efficientAddOrUpdata(g_Unit, 54, m_pDlg->GetText(54));
	efficientAddOrUpdata(g_Unit, 50, m_pDlg->GetText(50));
	efficientAddOrUpdata(g_Unit, 51, m_pDlg->GetText(51));
	efficientAddOrUpdata(g_Unit, 128, m_pDlg->GetText(128));
	efficientAddOrUpdata(g_Unit, 130, m_pDlg->GetText(130));
	efficientAddOrUpdata(g_Unit, 687, m_pDlg->GetText(687));
	efficientAddOrUpdata(g_Unit, 129, m_pDlg->GetText(129));
	
	for(map<int, string, less<int> >::iterator ix=g_Unit.begin(); ix!=g_Unit.end(); ++ix)
		m_Unit.AddString(ix->second.c_str());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CItemDlg::Create(UINT nID, CWnd* pParentWnd) 
{
	m_pDlg = (COPClientDlg*)pParentWnd;
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CItemDlg::DestroyWindow() 
{
	delete m_pItemParser;	
	delete []m_pSpr->Image;
	delete m_pSpr;	

	g_Unit.clear();
	return CDialog::DestroyWindow();
}

CItemHeader *CItemDlg::GetItem(int id)
{
	return m_pItemParser->GetItem(id);
}

CItemHeader	*CItemDlg::GetItemIndex(int index)
{
	return m_pItemParser->GetItemHeader(index);
}

int	CItemDlg::GetItemNun()
{
	return m_pItemParser->GetCount();
}

void CItemDlg::FillControls(CItemHeader *pItem)
{
	m_nID = pItem->m_uiID;
	m_nHp =	pItem->m_siHpGain;
	m_nMp = pItem->m_siMpGain;
	m_nAc = pItem->m_siAttack;
	

	m_nStr = pItem->m_siStrength;
	m_nDex = pItem->m_siDex;
	m_nInt = pItem->m_siInt;
	m_nVit = pItem->m_siVitality;
	
	m_nDefence	= pItem->m_siDefense;
	m_nSound	= pItem->m_siSound;
	m_nHealth	= pItem->m_siHealth;
	m_nFontL	= pItem->m_siSpriteFont;
	m_nFontS	= pItem->m_siSm_SpriteFont;

	m_nMaxQuantity	= pItem->m_uiWeight;
	m_nDamageRegist = pItem->m_siDamageRegist;
	m_nMagicRegist	= pItem->m_siMagicRegist;


	m_ImageL.SetCurSel(m_ImageL.FindString(0, pItem->m_szSpriteFile));
	m_ImageS.SetCurSel(m_ImageS.FindString(0, pItem->m_szSm_SpriteFile));


	m_strDescription	  = m_pDlg->GetText(pItem->m_siItemInfo);
	m_strMagicDescription = "";


	SetGroup(pItem);


	// 단위
	map<int, string, less<int> >::iterator find = g_Unit.lower_bound(pItem->m_siModule);

	if(find->first == pItem->m_siModule)
		m_Unit.SetCurSel(m_Unit.FindString(0, find->second.c_str()));	

	m_Attribute.SetCurSel(0);
	m_Magic.SetCurSel(-1);
	m_AttackType.SetCurSel(-1);


	if(pItem->m_Attribute & ON_ITEM_ATTACH_BODY_POSSIBLE)	m_Attribute.SetCurSel(1);
	if(pItem->m_Attribute & ON_ITEM_USE_POSSIBLE)			m_Attribute.SetCurSel(2);
		


	m_nAttackRange	= 0;
	m_nMagicAttack	= 0;	
	m_nReqLv		= 0;
	m_nMinD			= 0;
	m_nMaxD			= 0;
	m_nDrawID		= 0;
	m_nSource       = 0;

	for(int i=0; i<5; ++i)
	{
		m_Element[i].SetCurSel(-1);
		m_ElementNum[i] = 0;
	}
	
	for(i=0; i<3; ++i)
		m_Tool[i].SetCurSel(-1);


	// 재료
	if(pItem->m_Attribute & ON_ITEM_NO_TOOL && pItem->m_Attribute & ON_ITEM_NO_MATERIAL)
	{
		GetDlgItem(IDC_COMBO_TOOL1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TOOL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TOOL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WORK)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_ELEMENT01)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ELEMENT02)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ELEMENT03)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ELEMENT04)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ELEMENT05)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_ELEMENT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ELEMENT2)->EnableWindow(FALSE);		
		GetDlgItem(IDC_EDIT_ELEMENT3)->EnableWindow(FALSE);		
		GetDlgItem(IDC_EDIT_ELEMENT4)->EnableWindow(FALSE);		
		GetDlgItem(IDC_EDIT_ELEMENT5)->EnableWindow(FALSE);
		


		for(i=0; i<5; ++i)
		{
			m_Element[i].SetCurSel(-1);
			m_ElementNum[i] = 0;
		}

		for(i=0; i<3; ++i)
			m_Tool[i].SetCurSel(-1);

		m_nWork = 0;
	}
	else
	{
		GetDlgItem(IDC_COMBO_TOOL1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TOOL2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TOOL3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WORK)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_ELEMENT01)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ELEMENT02)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ELEMENT03)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ELEMENT04)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ELEMENT05)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_ELEMENT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ELEMENT2)->EnableWindow(TRUE);		
		GetDlgItem(IDC_EDIT_ELEMENT3)->EnableWindow(TRUE);		
		GetDlgItem(IDC_EDIT_ELEMENT4)->EnableWindow(TRUE);		
		GetDlgItem(IDC_EDIT_ELEMENT5)->EnableWindow(TRUE);


		for(i=0; i<5; ++i)
		{
			if(pItem->pMakeItem->m_uiItemID[i])
			{
				m_Element[i].SetCurSel(m_Element[i].FindString(0, m_pDlg->GetText(m_pItemParser->GetItem(pItem->pMakeItem->m_uiItemID[i])->m_siItemName)));
				m_ElementNum[i] = pItem->pMakeItem->m_siItemNum[i];
			}
		}

		for(i=0; i<3; ++i)
		{
			if(pItem->pMakeItem->m_uiToolID[i])
				m_Tool[i].SetCurSel(m_Tool[i].FindString(0, m_pDlg->GetText(m_pItemParser->GetItem(pItem->pMakeItem->m_uiToolID[i])->m_siItemName)));
		}

		m_nWork = pItem->pMakeItem->m_siLabor;

	}


	if(pItem->m_siGroupCode & ON_ITEM_SWORD)
	{
		m_nAttackRange = pItem->pWeaponHeader->siAttackRange;
		m_nMagicAttack = pItem->pWeaponHeader->siMagicAttack;

		m_nReqLv  = pItem->m_siReqLv;
		m_nMinD	  = pItem->pWeaponHeader->siMinDam;
		m_nMaxD	  = pItem->pWeaponHeader->siMaxDam;
		m_nDrawID = pItem->m_siDrawID;
		m_nSource = pItem->pWeaponHeader->siAttackAtb;

		m_strMagicDescription = m_pDlg->GetText(pItem->pWeaponHeader->siMagicDesc);


		m_AttackType.SetCurSel(pItem->pWeaponHeader->siWeaponType);
		m_Magic.SetCurSel(pItem->pWeaponHeader->uiMagic);

		DisableControl(TRUE);
	}
	else if(pItem->m_siGroupCode & ON_ITEM_DRESS)
	{
		m_nAttUp = pItem->m_siReqStr;
		m_nDefUp = pItem->m_siReqDex;

		DisableControl(FALSE);
	}
	else
	{
		DisableControl(FALSE);
	}

	UpdateData(FALSE);
}

void CItemDlg::OnSelchangeItemName() 
{
	if(m_ItemName.GetCurSel() == -1)	return;

	FillControls(m_pItemParser->GetItemHeader(m_ItemName.GetCurSel()));
	DrawItem(m_pItemParser->GetItemHeader(m_ItemName.GetCurSel()));
	
	Invalidate(FALSE);
}

void CItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_DC.GetData())
		m_DC.Show(dc.m_hDC);	
}

void CItemDlg::OnButtonExit() 
{
	CDialog::OnOK();
}

void CItemDlg::SetGroup(CItemHeader *pItem)
{
	if(pItem->m_siGroupCode & ON_ITEM_SWORD)		m_Group.SetCurSel(0);
	if(pItem->m_siGroupCode & ON_ITEM_ACCESSARY)	m_Group.SetCurSel(1);

	if(pItem->m_siGroupCode & ON_ITEM_TOOL)			m_Group.SetCurSel(2);
	if(pItem->m_siGroupCode & ON_ITEM_FOOD)			m_Group.SetCurSel(3);
	if(pItem->m_siGroupCode & ON_ITEM_ELEMENT)		m_Group.SetCurSel(4);
	if(pItem->m_siGroupCode & ON_ITEM_DRESS)		m_Group.SetCurSel(5);
	if(pItem->m_siGroupCode & ON_ITEM_ARMOR)		m_Group.SetCurSel(6);
	if(pItem->m_siGroupCode & ON_ITEM_ANIMAL)		m_Group.SetCurSel(7);
	if(pItem->m_siGroupCode & ON_ITEM_BOOTS)		m_Group.SetCurSel(8);
	if(pItem->m_siGroupCode & ON_ITEM_BELT)			m_Group.SetCurSel(9);
	if(pItem->m_siGroupCode & ON_ITEM_RING)			m_Group.SetCurSel(10);	
	if(pItem->m_siGroupCode & ON_ITEM_HELMAT)		m_Group.SetCurSel(11);
	if(pItem->m_siGroupCode & ON_ITEM_GLOVES)		m_Group.SetCurSel(12);
	if(pItem->m_siGroupCode & ON_ITEM_REVIVE)		m_Group.SetCurSel(13);
	if(pItem->m_siGroupCode & ON_ITEM_TRADE)		m_Group.SetCurSel(14);
	if(pItem->m_siGroupCode & ON_ITEM_TOALL)		m_Group.SetCurSel(22);
	if(pItem->m_siGroupCode & ON_ITEM_ALL_RECOVER)	m_Group.SetCurSel(23);
	if(pItem->m_siGroupCode & ON_ITEM_SPECIAL)
	{		
		switch(pItem->m_siSpecial)
		{
		case ON_ITEM_SPECIAL_INITIALIZE:	m_Group.SetCurSel(15);	break;
		case ON_ITEM_SPECIAL_SHOWMERCENARY:	m_Group.SetCurSel(16);	break;
		case ON_ITEM_SPECIAL_CHANGESEX:		m_Group.SetCurSel(17);	break;
		case ON_ITEM_SPECIAL_CHANGENATION:	m_Group.SetCurSel(18);	break;
		case ON_ITEM_SPECIAL_CREDITUP:		m_Group.SetCurSel(19);	break;
		case ON_ITEM_SPECIAL_TACTICS:		m_Group.SetCurSel(20);	break;
		case ON_ITEM_SPECIAL_DONKEY:		m_Group.SetCurSel(21);	break;
		}
	}
}

void CItemDlg::GetGroup(CItemHeader *pItem)
{
	switch(m_Group.GetCurSel())
	{
	case 0:		pItem->m_siGroupCode |=  ON_ITEM_SWORD;		break;
	case 1:		pItem->m_siGroupCode |=  ON_ITEM_ACCESSARY;	break;
	case 2:		pItem->m_siGroupCode |=  ON_ITEM_TOOL;		break;
	case 3:		pItem->m_siGroupCode |=  ON_ITEM_FOOD;		break;
	case 4:		pItem->m_siGroupCode |=  ON_ITEM_ELEMENT;	break;
	case 5:		pItem->m_siGroupCode |=  ON_ITEM_DRESS;		break;
	case 6:		pItem->m_siGroupCode |=  ON_ITEM_ARMOR;		break;
	case 7:		pItem->m_siGroupCode |=  ON_ITEM_ANIMAL;	break;
	case 8:		pItem->m_siGroupCode |=  ON_ITEM_BOOTS;		break;
	case 9:		pItem->m_siGroupCode |=  ON_ITEM_BELT;		break;
	case 10:	pItem->m_siGroupCode |=  ON_ITEM_RING;		break;
	case 11:	pItem->m_siGroupCode |=  ON_ITEM_HELMAT;	break;
	case 12:	pItem->m_siGroupCode |=  ON_ITEM_GLOVES;	break;
	case 13:	pItem->m_siGroupCode |=  ON_ITEM_REVIVE;	break;
	case 14:	pItem->m_siGroupCode |=  ON_ITEM_TRADE;		break;
	case 15:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_INITIALIZE;
		break;

	case 16:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_SHOWMERCENARY;
		break;

	case 17:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_CHANGESEX;
		break;

	case 18:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_CHANGENATION;
		break;

	case 19:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_CREDITUP;
		break;

	case 20:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_TACTICS;
		break;

	case 21:
		pItem->m_siGroupCode |= ON_ITEM_SPECIAL;
		pItem->m_siSpecial = ON_ITEM_SPECIAL_DONKEY;
		break;

	case 22:	pItem->m_siGroupCode |= ON_ITEM_TOALL;			break;
	case 23:
		pItem->m_siGroupCode |= ON_ITEM_ALL_RECOVER;	
		pItem->m_siGroupCode |= ON_ITEM_TOALL;
		break;
	}
}


void CItemDlg::OnSelchangeComboGroup() 
{
	if(m_Group.GetCurSel() == 0) 
	{
		GetDlgItem(IDC_EDIT_REQ_LV)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MINDAMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAXDAMAGE)->EnableWindow(TRUE);		
		GetDlgItem(IDC_COMBO_ATTACK_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAXDAMAGE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DRAWID)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MAGIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAGIC_ATTACK)->EnableWindow(TRUE);


		GetDlgItem(IDC_EDIT_ATTUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEFUP)->EnableWindow(FALSE);
	}
	else if(m_Group.GetCurSel() == 5)
	{
		GetDlgItem(IDC_EDIT_ATTUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEFUP)->EnableWindow(TRUE);


		GetDlgItem(IDC_EDIT_REQ_LV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MINDAMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAXDAMAGE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_COMBO_ATTACK_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAXDAMAGE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DRAWID)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MAGIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAGIC_ATTACK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ATTUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEFUP)->EnableWindow(TRUE);


		GetDlgItem(IDC_EDIT_REQ_LV)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MINDAMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAXDAMAGE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_COMBO_ATTACK_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAXDAMAGE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DRAWID)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MAGIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAGIC_ATTACK)->EnableWindow(FALSE);
	}
}

void CItemDlg::OnButtonSave()
{
	SetFileName("Text\\Item.dat");

	m_pItemParser->Save();
	m_pDlg->TextSave();

	AfxMessageBox("저장 성공");
}

void CItemDlg::DrawItem(CItemHeader *pItem)
{
	loadspr(pItem->m_szSpriteFile, *m_pSpr);

	m_DC.clear();
	m_DC.PutSprite8CT((m_DC.GetWidth()-m_pSpr->Header.Xsize)/2, (m_DC.GetHeight()-m_pSpr->Header.Ysize)/2,
		m_pSpr->Header.Xsize, m_pSpr->Header.Ysize, &m_pSpr->Image[m_pSpr->Header.Start[pItem->m_siSpriteFont]]);		
}

void CItemDlg::OnButtonModify()
{
	if(CheckAll() == FALSE) return;

	CItemHeader *pItem = m_pItemParser->GetItem(m_nID);


	UpdateData();


	// 아이템 이름 수정
	CString Temp;
	m_ItemName.GetWindowText(Temp);

	int nModifyText = m_ItemName.GetCurSel();

	m_ItemName.GetWindowText(Temp);


	if(strcmp(m_pDlg->GetText(pItem->m_siItemName), (LPSTR)(LPCTSTR)Temp) != 0)
		m_pDlg->ModifySave((LPSTR)(LPCTSTR)Temp, pItem->m_siItemName);

	if(strcmp(m_pDlg->GetText(pItem->m_siItemInfo), (LPSTR)(LPCTSTR)m_strDescription) != 0)
		m_pDlg->ModifySave((LPSTR)(LPCTSTR)m_strDescription, pItem->m_siItemInfo);


	m_ItemName.DeleteString(nModifyText);
	m_ItemName.InsertString(nModifyText, Temp);


	pItem->m_siHpGain = m_nHp;
	pItem->m_siMpGain = m_nMp;
	pItem->m_siAttack = m_nAc;
	

	pItem->m_siStrength = m_nStr;
	pItem->m_siDex = m_nDex;
	pItem->m_siInt = m_nInt;
	pItem->m_siVitality = m_nVit;
	
	pItem->m_siDefense = m_nDefence;
	pItem->m_siSound = m_nSound;
	pItem->m_siHealth = m_nHealth;
	pItem->m_siSpriteFont = m_nFontL;
	pItem->m_siSm_SpriteFont = m_nFontS;

	pItem->m_uiWeight = m_nMaxQuantity;
	pItem->m_siDamageRegist = m_nDamageRegist;
	pItem->m_siMagicRegist = m_nMagicRegist;


//	CString Temp;

	m_ImageL.GetLBText(m_ImageL.GetCurSel(), Temp);
	strcpy(pItem->m_szSpriteFile, (LPSTR)(LPCTSTR)Temp);

	m_ImageS.GetLBText(m_ImageS.GetCurSel(), Temp);
	strcpy(pItem->m_szSm_SpriteFile, (LPSTR)(LPCTSTR)Temp);

	GetGroup(pItem);

	int nCount = 0;
	for(map<int, string, less<int> >::iterator ix=g_Unit.begin(); ix!=g_Unit.end(); ++ix, ++nCount)
	{
		m_Unit.GetLBText(m_Unit.GetCurSel(), Temp);

		if(strcmp((LPSTR)(LPCTSTR)Temp, ix->second.c_str()) == 0)
		{
			pItem->m_siModule = ix->first;
			break;
		}
	}


	switch(m_Attribute.GetCurSel())
	{
	case 0:		pItem->m_Attribute |= ON_ITEM_NONE;					break;
	case 1:		pItem->m_Attribute |= ON_ITEM_ATTACH_BODY_POSSIBLE;	break;
	case 2:		pItem->m_Attribute |= ON_ITEM_USE_POSSIBLE;			break;
	}


	if(m_ElementNum[0] == 0)			pItem->m_Attribute |= ON_ITEM_NO_MATERIAL;
	if(m_Tool[0].GetCurSel() == -1)		pItem->m_Attribute |= ON_ITEM_NO_TOOL;


	for(int i=0; i<5; ++i)
	{		
		if(m_Element[i].GetCurSel() != -1)
		{
			pItem->pMakeItem->m_uiItemID[i]		= m_pItemParser->GetItemHeader(m_Element[i].GetCurSel())->m_uiID;
			pItem->pMakeItem->m_siItemNum[i]	= m_ElementNum[i];
		}
	}
	
	for(i=0; i<3; ++i)
	{
		if(m_Tool[i].GetCurSel() != -1)
			pItem->pMakeItem->m_uiToolID[i] = m_pItemParser->GetItemHeader(m_Tool[i].GetCurSel())->m_uiID;
	}

	if(pItem->m_siGroupCode & ON_ITEM_SWORD)
	{
		pItem->pWeaponHeader->siAttackRange = m_nAttackRange;
		pItem->pWeaponHeader->siMagicAttack = m_nMagicAttack;

		pItem->m_siReqLv = m_nReqLv;
		pItem->pWeaponHeader->siMinDam = m_nMinD;
		pItem->pWeaponHeader->siMaxDam = m_nMaxD;
		pItem->m_siDrawID			   = m_nDrawID;
		pItem->m_Attribute			   = m_nSource;

		if(m_strMagicDescription.GetLength() != 0)
		{
			pItem->pWeaponHeader->siMagicDesc = m_pDlg->GetEmptyTextID();
			strcpy(m_pDlg->GetText(pItem->pWeaponHeader->siMagicDesc), (LPSTR)(LPCTSTR)m_strMagicDescription);
		}
	}
	else if(pItem->m_siGroupCode & ON_ITEM_DRESS)
	{
		pItem->m_siReqStr = m_nAttUp; 
		pItem->m_siReqDex = m_nDefUp;
	}
	else
	{
		delete pItem->pWeaponHeader;
		pItem->pWeaponHeader = NULL;
	}


	int nAddPos = m_ItemName.GetCurSel();
	m_ItemName.DeleteString(nAddPos);
	m_ItemName.InsertString(nAddPos, m_pDlg->GetText(pItem->m_siItemName));
}

void CItemDlg::OnButtonInsert() 
{
	CItemHeader Item;

	ZeroMemory(&Item, sizeof(CItemHeader));


	Item.pMakeItem = new MakeItem;
	Item.pWeaponHeader = new WeaponHeader;

	
	ZeroMemory(Item.pMakeItem, sizeof(MakeItem));
	ZeroMemory(Item.pWeaponHeader, sizeof(WeaponHeader));

	CItemHeader *pTempItem = m_pItemParser->GetItemHeader(m_pItemParser->GetCount()-1);
	
	m_nID		= pTempItem->m_uiID + 1;

	Item.m_uiID = m_nID;

	Item.m_siItemName = m_pDlg->GetEmptyTextID();
	strcpy(m_pDlg->GetText(Item.m_siItemName), "이름을 입력해 주세요");
	

	Item.m_siItemInfo = m_pDlg->GetEmptyTextID();
	strcpy(m_pDlg->GetText(Item.m_siItemInfo), "아이템 설명을 입력해 주세요");
	

	// 아이템 삽입
	m_pItemParser->AddItem(Item);


	FillControls(&Item);


	// 아이템 이름 추가
	m_ItemName.AddString(m_pDlg->GetText(Item.m_siItemName));
	m_ItemName.SetCurSel(m_ItemName.GetCount()-1);

	// 뷰 클리어
	m_DC.clear();
	Invalidate(FALSE);
}

BOOL CItemDlg::CheckAll()
{
	if(m_ImageL.GetCurSel() == -1)
	{
		AfxMessageBox("빈 칸이 있습니다.\n빈 칸으로 포커스를 이동 합니다.", MB_ICONERROR);
		m_ImageL.SetFocus();
		return FALSE;
	}

	if(m_ImageS.GetCurSel() == -1)
	{
		AfxMessageBox("빈 칸이 있습니다.\n빈 칸으로 포커스를 이동 합니다.", MB_ICONERROR);
		m_ImageS.SetFocus();
		return FALSE;
	}

	if(m_Unit.GetCurSel() == -1)
	{
		AfxMessageBox("빈 칸이 있습니다.\n빈 칸으로 포커스를 이동 합니다.", MB_ICONERROR);
		m_Unit.SetFocus();
		return FALSE;
	}

	if(m_Group.GetCurSel() == 0 )
	{
		if(m_AttackType.GetCurSel() == -1)
		{
			AfxMessageBox("빈 칸이 있습니다.\n빈 칸으로 포커스를 이동 합니다.", MB_ICONERROR);
			m_AttackType.SetFocus();
			return FALSE;
		}
	}

	CString Temp;
	m_ItemName.GetWindowText(Temp);

	if(Temp.GetLength() == 0)
	{
		AfxMessageBox("빈 칸이 있습니다.\n빈 칸으로 포커스를 이동 합니다.", MB_ICONERROR);
		m_ItemName.SetFocus();
		return FALSE;
	}
	return TRUE;
}

void CItemDlg::DisableControl(BOOL b)
{
	GetDlgItem(IDC_EDIT_REQ_LV)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_MINDAMAGE)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_MAXDAMAGE)->EnableWindow(b);		
	GetDlgItem(IDC_COMBO_ATTACK_TYPE)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_MAXDAMAGE2)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_DRAWID)->EnableWindow(b);
	GetDlgItem(IDC_COMBO_MAGIC)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_MAGIC_ATTACK)->EnableWindow(b);
	GetDlgItem(IDC_EDIT_SOURCE)->EnableWindow(b);		
	GetDlgItem(IDC_EDIT_DESC2)->EnableWindow(b);	


	GetDlgItem(IDC_EDIT_ATTUP)->EnableWindow(!b);
	GetDlgItem(IDC_EDIT_DEFUP)->EnableWindow(!b);
}

void CItemDlg::OnButtonDelete()
{
	if(m_ItemName.GetCurSel() == -1) return;

	m_pItemParser->DeleteItem(m_ItemName.GetCurSel());
	m_ItemName.DeleteString(m_ItemName.GetCurSel());
}

void CItemDlg::SetReadOnlyMode(BOOL b)
{
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_SAVEITEM)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_INSERT_OK)->EnableWindow(b);
}