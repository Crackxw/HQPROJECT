// CharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPClient.h"
#include "CharDlg.h"


#include "Graphics16.h"
#include "CharParser.h"
#include "OPClientDlg.h"
#include "OnlineItem.h"


#define TYPE_ARROW			"Arrow"
#define TYPE_BULLET			"Bullet"
#define TYPE_DART			"Dart"
#define TYPE_SWORD			"sword"
#define TYPE_SEPEAR			"spear"
#define TYPE_AXE			"axe"
#define TYPE_FAN			"fan"
#define TYPE_CRAW			"craw"
#define TYPE_BUDDIST		"buddist"
#define TYPE_BEADS			"beads"
#define TYPE_DOUBLESTICK	"DoubleStick"
#define TYPE_SCYTHE			"Scythe"
#define TYPE_SPINSTONE		"SpinStone"
#define TYPE_EMPTY			"empty"
#define TYPE_FREE			"1"
#define TYPE_NONE			"0"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharDlg dialog


CCharDlg::CCharDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharDlg)
	m_nStr = 0;
	m_nDex = 0;
	m_nVit = 0;
	m_nInt = 0;
	m_nHp = 0;
	m_nMp = 0;
	m_nAc = 0;
	m_nSpeed = 0;
	m_nDR = 0;
	m_MR = 0;
	m_nMinD = 0;
	m_nMaxD = 0;
	m_strDesc = _T("");
	m_nCost = 0;
	m_nPercent[0] = 0;
	m_nPercent[1] = 0;
	m_nPercent[2] = 0;
	m_nLift = 0;
	//}}AFX_DATA_INIT

	m_pSpr = new XSPR[MAX_DC];
	ZeroMemory(m_pSpr, sizeof(XSPR) * MAX_DC);
}


void CCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharDlg)
	DDX_Control(pDX, IDC_COMBO_ITEMLIST1, m_ItemList[0]);
	DDX_Control(pDX, IDC_COMBO_ITEMLIST2, m_ItemList[1]);
	DDX_Control(pDX, IDC_COMBO_ITEMLIST3, m_ItemList[2]);
	DDX_Control(pDX, IDC_COMBO_CHAR_NAME, m_CharList);
	DDX_Text(pDX, IDC_EDIT_STR, m_nStr);
	DDX_Text(pDX, IDC_EDIT_DEX, m_nDex);
	DDX_Text(pDX, IDC_EDIT_VIT, m_nVit);
	DDX_Text(pDX, IDC_EDIT_INT, m_nInt);
	DDX_Text(pDX, IDC_EDIT_HP, m_nHp);
	DDX_Text(pDX, IDC_EDIT_MP, m_nMp);
	DDX_Text(pDX, IDC_EDIT_SPEED2, m_nAc);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_nSpeed);
	DDX_Text(pDX, IDC_EDIT_DR, m_nDR);
	DDX_Text(pDX, IDC_EDIT_MR, m_MR);
	DDX_Text(pDX, IDC_EDIT_MIND, m_nMinD);
	DDX_Text(pDX, IDC_EDIT_MAXD, m_nMaxD);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDesc);
	DDX_Text(pDX, IDC_EDIT_COST, m_nCost);
	DDX_Text(pDX, IDC_EDIT_ITEMPER1, m_nPercent[0]);
	DDX_Text(pDX, IDC_EDIT_ITEMPER2, m_nPercent[1]);
	DDX_Text(pDX, IDC_EDIT_ITEMPER3, m_nPercent[2]);
	DDX_Text(pDX, IDC_EDIT_SPEED4, m_nLift);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharDlg, CDialog)
	//{{AFX_MSG_MAP(CCharDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_CHAR_NAME, OnSelchangeComboCharName)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharDlg message handlers

BOOL CCharDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pCharParser = new CharParser;
	m_pCharParser->Init("Data\\Char\\Character List.txt");


	// ����Ʈ�� ĳ���� �̸��� �ʾ� �ش�.
	for(int i=0; i<m_pCharParser->GetCharTotalNum(); ++i)
		m_CharList.AddString(m_pDlg->GetText(m_pCharParser->GetCharHeader(i)->uiCharName));


	// �������� �ʾ� �ش�.
	for(i=0; i<m_pDlg->GetItemNun(); ++i)
	{
		m_ItemList[0].AddString(m_pDlg->GetText(m_pDlg->GetItemIndex(i)->m_siItemName));
		m_ItemList[1].AddString(m_pDlg->GetText(m_pDlg->GetItemIndex(i)->m_siItemName));
		m_ItemList[2].AddString(m_pDlg->GetText(m_pDlg->GetItemIndex(i)->m_siItemName));
	}


	// �� �並 ����� �ش�.
	RECT rt;
	for(i=0; i<MAX_DC; ++i)
	{
		GetDlgItem(IDC_CHAR_VIEW+i)->GetWindowRect(&rt);
		ScreenToClient(&rt);

		m_DC[i].Create(rt);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCharDlg::DestroyWindow() 
{
	delete 	m_pCharParser;

	for(int i=0; i<MAX_DC; ++i)
	{
		if(m_pSpr[i].Image)
		delete []m_pSpr[i].Image;
	}

	delete []m_pSpr;
	return CDialog::DestroyWindow();
}

BOOL CCharDlg::Create(UINT nID, CWnd* pParentWnd) 
{
	m_pDlg = (COPClientDlg*)pParentWnd;	
	return CDialog::Create(IDD, pParentWnd);
}

void CCharDlg::OnSelchangeComboCharName() 
{
	if(m_CharList.GetCurSel() == -1)	return;

	FillControl(m_pCharParser->GetCharHeader(m_CharList.GetCurSel()));
}

void CCharDlg::FillControl(CharHead *pHeader)
{
	m_nStr = pHeader->Parameta.GetStr();
	m_nDex = pHeader->Parameta.GetDex();
	m_nInt = pHeader->Parameta.GetInt();
	m_nVit = pHeader->Parameta.GetVit();

	m_nHp	= m_nVit * 4;
	m_nMp	= m_nInt * 4;
	m_nAc	= pHeader->Parameta.GetAC();
	m_nDR	= pHeader->Parameta.DamageResist;
	m_MR	= pHeader->Parameta.MagicResist;
	m_nLift = pHeader->siCon;

	m_nSpeed	= pHeader->siMoveSpeed;
	m_nCost		= pHeader->siCost;
	m_strDesc	= m_pDlg->GetText(pHeader->siDescText);


	// ������ ����
	for(int i=1; i<3; ++i)
	{
		m_nPercent[i-1] = pHeader->siDropPercent[i-1];
		DrawItem(i, pHeader->siItemDrop[i-1]);
	}

	UpdateData(FALSE);	
	Invalidate(FALSE);
}

void CCharDlg::DrawChar(CharHead *pHeader)
{
	
}

void CCharDlg::DrawItem(int index, int nItemID)
{
	m_DC[index].clear();
	m_ItemList[index-1].SetCurSel(-1);

	if(nItemID == 0)	return;

	// SPR �ε�
	loadspr(m_pDlg->GetItem(nItemID)->m_szSpriteFile, m_pSpr[index]);


	// DC�� ���	
	m_DC[index].PutSprite8CT((m_DC[index].GetWidth()-m_pSpr[index].Header.Xsize)/2, 
		(m_DC[index].GetHeight()-m_pSpr[index].Header.Ysize)/2,
		m_pSpr[index].Header.Xsize, m_pSpr[index].Header.Ysize, 
		&m_pSpr[index].Image[m_pSpr[index].Header.Start[m_pDlg->GetItem(nItemID)->m_siSpriteFont]]);	


	// ����Ʈ �ڽ� �̸��� ������ �̸��� ���� �ش�.
	m_ItemList[index-1].SetCurSel(m_ItemList[index-1].FindString(0, m_pDlg->GetText(m_pDlg->GetItem(nItemID)->m_siItemName)));
}

void CCharDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	for(int i=0; i<MAX_DC; ++i)
		m_DC[i].Show(dc.m_hDC);
}

void CCharDlg::OnButtonSave() 
{	
	SetFileName("Text\\Character List.txt");

	FILE *fp = fopen("Text\\Character List.txt", "wt");

	SaveToFile(fp);
}

void CCharDlg::SaveToFile(FILE *fp)
{
	fprintf( fp, "ĳ���͸���Ʈ																																	�Ϲݿ뺴-0\n" );
	fprintf( fp, "�մ�� �ְ�																																	���-1\n" );
	fprintf( fp, "Ver. 3.4			X=����Ұ�																														����-2\n" );
	fprintf( fp, "������	XXX	�����ڵ�	X	X	�������з�	��	��ø	ü��	����	�⺻����	�⺻����	����	��������	��������	�ּҵ�����	�ְ�����	X	X	X	��밡��	��밡�ɷ�	��񹫱�	���ŵ�	�ִ¾�����	Ȯ��	�ִ¾�����	Ȯ��	X	X	X	X	����	����	X\n" );
	fprintf( fp, "Character ID	Name	Name Code	CurEXP	MyEXP	Nation	Str	Dex	Vit	Int	AC	Level	LiftConstant	DamageResist	MagicResist	EquipMinDamage	EquipMaxDamage	MoveSpeed	Life	Mana	Cost	NeedLevel	WeaponType	Need Credit	Item Drop1	Percent1	Item Drop2	Percent2	Item Drop3	Percent3	Gold Drop	Percent4	Desc Text	General	Attack Type\n" );

	
	for( int i=0 ; i<m_pCharParser->GetCharTotalNum(); ++i )
		SaveToFile( fp, m_pCharParser->GetCharHeader(i) );

	fclose(fp);

	AfxMessageBox("���� �Ϸ�");
}

void CCharDlg::SaveToFile(FILE *fp, CharHead *pHeader)
{
//					  1     2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33
		fprintf(fp, "%c%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",

		LOBYTE(pHeader->uiID),HIBYTE(pHeader->uiID),							// ID						1
		m_pDlg->GetText(pHeader->uiCharName ),									// ĳ���� �̸�				2
		pHeader->uiCharName,													// Name Code				3
		0,0,																	// 2�� ��� ����ȵ�		4,5
		pHeader->siNation,														// ����						6
		pHeader->Parameta.GetStr(),												// ��						7
		pHeader->Parameta.GetDex(),												// ��ø						8
		pHeader->Parameta.GetVit(),												// ����						9
		pHeader->Parameta.GetInt(),												// ����						10
		pHeader->Parameta.GetAC(),												// �⺻ ����				11
		pHeader->siLv,															// �⺻ ����				12
		pHeader->siCon,															// ����						13
		pHeader->Parameta.DamageResist,											// ���� ���				14
		pHeader->Parameta.MagicResist,											// ���� ���				15
		pHeader->Parameta.EquipMinDamage,										// �ּ� ������				16
		pHeader->Parameta.EquipMaxDamage,										// �ִ� ������				17
		pHeader->siMoveSpeed,													// �̵��ӵ�					18
		0, 0,																	// ��� ���� ����			19,20
		pHeader->siCost,														// ��� ����				21
		pHeader->siNeedLv,														// ��� ���� ����			22
		GetWeaponType(pHeader->uiWeaponType),									// ���� Ÿ��				23
		pHeader->siNeedCredit,													// ��� ���� �ſ� ���		24
		pHeader->siItemDrop[0],													// ������ ������ 1			25
		pHeader->siDropPercent[0],												// ������ 1�� Ȯ��			26
		pHeader->siItemDrop[1],													// ������ ������ 2			27
		pHeader->siDropPercent[1],												// ������ 2�� Ȯ��			28
		pHeader->siItemDrop[2],													// ������ ������ 3			29
		pHeader->siDropPercent[2],												// ������ 3�� Ȯ��			30
		pHeader->siItemDrop[3],													// �ִ� ��					31
		pHeader->siDropPercent[3],												// �ִ� ���� Ȯ��			32
		pHeader->siDescText,													// ����						33
		pHeader->siGeneral,														// ��� ����				34
		0 );																	// ��� X					35
}

char *CCharDlg::GetWeaponType(int type)
{
	switch( type )
	{
	case ON_ITEM_MISSLE_TYPE_ARROW:			return TYPE_ARROW;
	case ON_ITEM_MISSLE_TYPE_BULLET:		return TYPE_BULLET;
	case ON_ITEM_MISSLE_TYPE_DART:			return TYPE_DART;
	case ON_ITEM_MISSLE_TYPE_SWORD:			return TYPE_SWORD;
	case ON_ITEM_MISSLE_TYPE_SEPEAR:		return TYPE_SEPEAR;
	case ON_ITEM_MISSLE_TYPE_AXE:			return TYPE_AXE;
	case ON_ITEM_MISSLE_TYPE_FAN:			return TYPE_FAN;
	case ON_ITEM_MISSLE_TYPE_CRAW:			return TYPE_CRAW;
	case ON_ITEM_MISSLE_TYPE_BUDDIST:		return TYPE_BUDDIST;
	case ON_ITEM_MISSLE_TYPE_BEADS:			return TYPE_BEADS;
	case ON_ITEM_MISSLE_TYPE_DOUBLESTICK:	return TYPE_DOUBLESTICK;
	case ON_ITEM_MISSLE_TYPE_SCYTHE:		return TYPE_SCYTHE;
	case ON_ITEM_MISSLE_TYPE_SPINSTONE:		return TYPE_SPINSTONE;
	case ON_ITEM_MISSLE_TYPE_EMPTY:			return TYPE_EMPTY;
	case ON_ITEM_MISSLE_TYPE_FREE:			return TYPE_FREE;
	}

	return TYPE_NONE;
}

void CCharDlg::OnButtonModify() 
{
	if(AfxMessageBox("���� ������ �ӽ÷� ���� �ϰڽ��ϱ�?", MB_ICONASTERISK) == IDNO)	return;

	UpdateData();

	CharHead *pHeader = m_pCharParser->GetCharHeader(m_CharList.GetCurSel());

	pHeader->Parameta.SetStr(m_nStr);
	pHeader->Parameta.SetDex(m_nDex);
	pHeader->Parameta.SetInt(m_nInt);
	pHeader->Parameta.SetVit(m_nVit);
	pHeader->Parameta.SetAC(m_nAc);


	pHeader->Parameta.DamageResist	= m_nDR;
	pHeader->Parameta.MagicResist	= m_MR;
	pHeader->siCon					= m_nLift;
	pHeader->siMoveSpeed			= m_nSpeed;
	pHeader->siCost					= m_nCost;

	// ������ ����
	for(int i=0; i<3; ++i)
	{
		pHeader->siDropPercent[i]	= m_nPercent[i];
		pHeader->siItemDrop[i]		= m_ItemList[i].GetCurSel() != -1 ?  m_pDlg->GetItemIndex(m_ItemList[i].GetCurSel())->m_uiID : 0;
	}
}
