#ifndef __ONLINEVILLAGE_PARSER_H
#define __ONLINEVILLAGE_PARSER_H

/********************************************************************************************
	���ϸ�	:	OnlineVillage-Parser.cpp
	�ۼ���	:	�� �� ��
	�����  :   �� �� ��
	�ۼ���	:	2001.06.22
	������	:	2001.11.23
********************************************************************************************/


#define		ON_VILLAGE_MAX_ITEM						20
#define		ON_VILLAGE_MAX_BUILDING					10
#define		ON_VILLAGE_MAX_EVENT					3

#define		ON_VILLAGE_CONSUMPTION					0x00000000									// �Ҹ�
#define		ON_VILLAGE_PRODUCTION					0x00000001									// ����


#define		ON_VIL_ST_KIND_UNKNOWN					0											// �� �� ���� �ǹ��̴�.
#define		ON_VIL_ST_KIND_BARRACK					1											// �Ʒü�
#define		ON_VIL_ST_KIND_MARKET					2											// ����
#define		ON_VIL_ST_KIND_BANK						3											// ����(����)
#define		ON_VIL_ST_KIND_HOSPITAL					4											// ����
#define		ON_VIL_ST_KIND_INN						5											// �ָ�
#define		ON_VIL_ST_KIND_WHARF					6											// �ε�
#define		ON_VIL_ST_KIND_FARM						7											// ����
#define		ON_VIL_ST_KIND_GOVERNMENT				8											// ��û
#define		ON_VIL_ST_KIND_AUCTION					9											// ��ż�
#define		ON_VIL_ST_KIND_WSTORE					10											// ���� ����
#define		ON_VIL_ST_KIND_FSTORE					11											// ���� ����
#define		ON_VIL_ST_KIND_DSTORE					12											// �� ����
#define		ON_VIL_ST_KIND_WEAPON					13											// ������
#define		ON_VIL_ST_KIND_ARMOR					14											// ������
#define		ON_VIL_ST_KIND_PASTURE					15											// ����
#define		ON_VIL_ST_KIND_FACTORY					16											// ����
#define		ON_VIL_ST_KIND_GBARRACK					17											// ����Ʒü�
#define		ON_VIL_ST_KIND_MBARRACK					18											// �����Ʒü�
#define		ON_VIL_ST_KIND_RING						19											// ������


#define		ON_VIL_ST_KIND_CODE_BARRACK			"BAR"											// �Ʒü�
#define		ON_VIL_ST_KIND_CODE_MARKET			"MAR"											// ����
#define		ON_VIL_ST_KIND_CODE_BANK			"BAN"											// ����(����)
#define		ON_VIL_ST_KIND_CODE_HOSPITAL		"CLI"											// ����
#define		ON_VIL_ST_KIND_CODE_INN				"INN"											// �ָ�
#define		ON_VIL_ST_KIND_CODE_WHARF			"WHA"											// �ε�
#define		ON_VIL_ST_KIND_CODE_FARM			"FAR"											// ����
#define		ON_VIL_ST_KIND_CODE_GOVERNMENT		"GOV"											// ��û
#define		ON_VIL_ST_KIND_CODE_WSTORE			"WST"											// �������
#define		ON_VIL_ST_KIND_CODE_FSTORE			"FST"											// ���Ļ���
#define		ON_VIL_ST_KIND_CODE_DSTORE			"DST"											// �����
#define		ON_VIL_ST_KIND_CODE_WEAPON			"WEA"											// ������
#define		ON_VIL_ST_KIND_CODE_ARMOR			"ARM"											// ������
#define		ON_VIL_ST_KIND_CODE_PASTURE			"RAN"											// ����
#define		ON_VIL_ST_KIND_CODE_FACTORY			"FAC"											// ����
#define		ON_VIL_ST_KIND_CODE_GBARRACK		"GBA"											// ����Ʒü�
#define		ON_VIL_ST_KIND_CODE_MBARRACK		"MBA"											// ���ü�
#define		ON_VIL_ST_KIND_CODE_LUMBERING		"LMB"											// ������
#define		ON_VIL_ST_KIND_CODE_MINE			"MIN"											// ����
#define		ON_VIL_ST_KIND_CODE_RING			"RIN"											// ������
#define		ON_VIL_ST_KIND_CODE_GEXBARRACK		"GEX"											// ����Ʒü� ���� 2������.


//#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct	VillageHeader
{
	UI16	uiVillageCode;																		// ���� �ڵ�
	UI32	uiPop;																				// �α���(�������� ��� x)
	UI32	uiMinPop,uiMaxPop;																	// �ִ�,�ּ� �α���
	SI16	siVillageName;																		// ������ �̸�
	SI16	siX, siY;																			// ���� ��ǥ
	SI16	siMapIndex;																			// Map Index
	SI16	siType;																				// ������ Ÿ��
	CHAR	szImageFile[256];																	// �̹��� ����
	SI16	siSize;																				// ������ ũ��
	UI32	uiItemID[ON_VILLAGE_MAX_ITEM];														// �������� �Ĵ� ������ ����
	UI32	siQuantity[ON_VILLAGE_MAX_ITEM];
	SI32	siProperty[ON_VILLAGE_MAX_ITEM];
	CHAR	szVillage[ON_VILLAGE_MAX_BUILDING][50];												// �� ��� �ǹ��� �ִ���
	CHAR	szConfig[ON_VILLAGE_MAX_BUILDING];													// ���� ����
	CHAR	szGateImageFileName[50];															// ������ �̹��� ����
	SI32	siStructureKinds[ON_VILLAGE_MAX_BUILDING];											// �ǹ��� ����
	CHAR	szStructureKinds[ ON_VILLAGE_MAX_BUILDING ] [ 4 ] ;
	UI08	uiNation;
	CHAR	szSize;

	VillageHeader::VillageHeader() : uiVillageCode(0) { };
	const bool VillageHeader::operator<(const VillageHeader &rhs)	const	{return (rhs.uiVillageCode < this->uiVillageCode ? true : false);  };
	const bool VillageHeader::operator==(const VillageHeader &rhs)	const   {return (this->uiVillageCode == rhs.uiVillageCode ? true : false); };
};


struct	VillageType
{
	SI16	Xpos[10];																			// �ǹ��� ��ǥ
	SI16	Ypos[10];																			// �ǹ��� ��ǥ
	SI16	ExitXpos, ExitYpos;																	// ������ ��ư�� ��ġ
	
	SI16	siType;																				// Ÿ���̸�
	SI16	siStateCode;																		// ���� �ڵ�
};



class OnlineVillageParser
{
private:
	vector<VillageHeader>	m_VillageHeader;													// ���� ��� ������ ���� ����ü
	vector<VillageType>		m_VillageType;														// ���� Ÿ���� ����ü 

	typedef vector<VillageHeader>::iterator VWlter;
	typedef pair<VWlter, VWlter> VWlterPair;
public:
	OnlineVillageParser();
	~OnlineVillageParser();

	BOOL					ReadText( char *pFileName, char *pFileStruct );						// ������ ���õ� �ؽ�Ʈ ������ �д´�.
	VOID					Free()	{ m_VillageHeader.clear();  m_VillageType.clear(); };		// �����
	SI32					GetTotalVillageNum()	 { return m_VillageHeader.size(); };		// �� �������� �˾� �´�.
	SI16					GetTotalTypeNum()		 { return m_VillageType.size();   };		// �� ���� Ÿ�� ���� �������ش�.
	VillageHeader			*GetVillageHeaderIndex( SI16 siIndex )	{ return &m_VillageHeader[siIndex]; };
	VillageHeader			*GetVillageHeaderCode(UI16 uiCode);
	BOOL					IsStructure(SI32 HeaderIndex, UI16 code);							// �ش� �ǹ��� �����ϴ°�?
	VillageType				*GetType( SI16 siType, SI16 StateCode);								// ������ Ÿ���� ��´�.
	BOOL					GetStructKind(SI32 siKind, CHAR &szKind);							// �ǹ������� ���ڿ��� �ٲ��ش�.

private:
	SI16					GetType( char *pToken );											// ������ Ÿ���� �������ش�.
	BOOL					LoadType( char *pFileStruct );										// ���� ���� �ǹ��� ��ġ�� �о� �´�.
	SI16					GetVillageKind( CHAR *pFileName );									// �ǹ��� ������ �˾� ��
};


#endif