#ifndef __ONLINEVILLAGE_PARSER_H
#define __ONLINEVILLAGE_PARSER_H

/********************************************************************************************
	파일명	:	OnlineVillage-Parser.cpp
	작성자	:	정 진 욱
	담당자  :   이 상 원
	작성일	:	2001.06.22
	수정일	:	2001.11.23
********************************************************************************************/


#define		ON_VILLAGE_MAX_ITEM						20
#define		ON_VILLAGE_MAX_BUILDING					10
#define		ON_VILLAGE_MAX_EVENT					3

#define		ON_VILLAGE_CONSUMPTION					0x00000000									// 소모
#define		ON_VILLAGE_PRODUCTION					0x00000001									// 생산


#define		ON_VIL_ST_KIND_UNKNOWN					0											// 알 수 없는 건물이다.
#define		ON_VIL_ST_KIND_BARRACK					1											// 훈련소
#define		ON_VIL_ST_KIND_MARKET					2											// 시장
#define		ON_VIL_ST_KIND_BANK						3											// 은행(전장)
#define		ON_VIL_ST_KIND_HOSPITAL					4											// 병원
#define		ON_VIL_ST_KIND_INN						5											// 주막
#define		ON_VIL_ST_KIND_WHARF					6											// 부두
#define		ON_VIL_ST_KIND_FARM						7											// 농장
#define		ON_VIL_ST_KIND_GOVERNMENT				8											// 관청
#define		ON_VIL_ST_KIND_AUCTION					9											// 경매소
#define		ON_VIL_ST_KIND_WSTORE					10											// 무기 상점
#define		ON_VIL_ST_KIND_FSTORE					11											// 음식 상점
#define		ON_VIL_ST_KIND_DSTORE					12											// 약 상점
#define		ON_VIL_ST_KIND_WEAPON					13											// 무기점
#define		ON_VIL_ST_KIND_ARMOR					14											// 갑주장
#define		ON_VIL_ST_KIND_PASTURE					15											// 목장
#define		ON_VIL_ST_KIND_FACTORY					16											// 공장
#define		ON_VIL_ST_KIND_GBARRACK					17											// 장수훈련소
#define		ON_VIL_ST_KIND_MBARRACK					18											// 몬스터훈련소
#define		ON_VIL_ST_KIND_RING						19											// 반지점


#define		ON_VIL_ST_KIND_CODE_BARRACK			"BAR"											// 훈련소
#define		ON_VIL_ST_KIND_CODE_MARKET			"MAR"											// 시장
#define		ON_VIL_ST_KIND_CODE_BANK			"BAN"											// 은행(전장)
#define		ON_VIL_ST_KIND_CODE_HOSPITAL		"CLI"											// 병원
#define		ON_VIL_ST_KIND_CODE_INN				"INN"											// 주막
#define		ON_VIL_ST_KIND_CODE_WHARF			"WHA"											// 부두
#define		ON_VIL_ST_KIND_CODE_FARM			"FAR"											// 농장
#define		ON_VIL_ST_KIND_CODE_GOVERNMENT		"GOV"											// 관청
#define		ON_VIL_ST_KIND_CODE_WSTORE			"WST"											// 무기상점
#define		ON_VIL_ST_KIND_CODE_FSTORE			"FST"											// 음식상점
#define		ON_VIL_ST_KIND_CODE_DSTORE			"DST"											// 약상점
#define		ON_VIL_ST_KIND_CODE_WEAPON			"WEA"											// 무기점
#define		ON_VIL_ST_KIND_CODE_ARMOR			"ARM"											// 갑옷점
#define		ON_VIL_ST_KIND_CODE_PASTURE			"RAN"											// 목장
#define		ON_VIL_ST_KIND_CODE_FACTORY			"FAC"											// 공장
#define		ON_VIL_ST_KIND_CODE_GBARRACK		"GBA"											// 장수훈련소
#define		ON_VIL_ST_KIND_CODE_MBARRACK		"MBA"											// 조련소
#define		ON_VIL_ST_KIND_CODE_LUMBERING		"LMB"											// 벌목장
#define		ON_VIL_ST_KIND_CODE_MINE			"MIN"											// 광산
#define		ON_VIL_ST_KIND_CODE_RING			"RIN"											// 반지점
#define		ON_VIL_ST_KIND_CODE_GEXBARRACK		"GEX"											// 장수훈련소 안의 2차전직.


//#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct	VillageHeader
{
	UI16	uiVillageCode;																		// 마을 코드
	UI32	uiPop;																				// 인구수(서버에서 사용 x)
	UI32	uiMinPop,uiMaxPop;																	// 최대,최소 인구수
	SI16	siVillageName;																		// 마을의 이름
	SI16	siX, siY;																			// 마을 좌표
	SI16	siMapIndex;																			// Map Index
	SI16	siType;																				// 마을의 타입
	CHAR	szImageFile[256];																	// 이미지 파일
	SI16	siSize;																				// 마을의 크기
	UI32	uiItemID[ON_VILLAGE_MAX_ITEM];														// 마을에서 파는 아이템 정보
	UI32	siQuantity[ON_VILLAGE_MAX_ITEM];
	SI32	siProperty[ON_VILLAGE_MAX_ITEM];
	CHAR	szVillage[ON_VILLAGE_MAX_BUILDING][50];												// 총 몇개의 건물이 있는지
	CHAR	szConfig[ON_VILLAGE_MAX_BUILDING];													// 마을 정보
	CHAR	szGateImageFileName[50];															// 성문의 이미지 파일
	SI32	siStructureKinds[ON_VILLAGE_MAX_BUILDING];											// 건물의 종류
	CHAR	szStructureKinds[ ON_VILLAGE_MAX_BUILDING ] [ 4 ] ;
	UI08	uiNation;
	CHAR	szSize;

	VillageHeader::VillageHeader() : uiVillageCode(0) { };
	const bool VillageHeader::operator<(const VillageHeader &rhs)	const	{return (rhs.uiVillageCode < this->uiVillageCode ? true : false);  };
	const bool VillageHeader::operator==(const VillageHeader &rhs)	const   {return (this->uiVillageCode == rhs.uiVillageCode ? true : false); };
};


struct	VillageType
{
	SI16	Xpos[10];																			// 건물의 좌표
	SI16	Ypos[10];																			// 건물의 좌표
	SI16	ExitXpos, ExitYpos;																	// 나가기 버튼의 위치
	
	SI16	siType;																				// 타입이름
	SI16	siStateCode;																		// 국가 코드
};



class OnlineVillageParser
{
private:
	vector<VillageHeader>	m_VillageHeader;													// 마을 모든 정보를 가진 구조체
	vector<VillageType>		m_VillageType;														// 마을 타입의 구조체 

	typedef vector<VillageHeader>::iterator VWlter;
	typedef pair<VWlter, VWlter> VWlterPair;
public:
	OnlineVillageParser();
	~OnlineVillageParser();

	BOOL					ReadText( char *pFileName, char *pFileStruct );						// 마을에 관련된 텍스트 파일을 읽는다.
	VOID					Free()	{ m_VillageHeader.clear();  m_VillageType.clear(); };		// 지운다
	SI32					GetTotalVillageNum()	 { return m_VillageHeader.size(); };		// 총 마을수를 알아 온다.
	SI16					GetTotalTypeNum()		 { return m_VillageType.size();   };		// 총 마을 타입 수를 리턴해준다.
	VillageHeader			*GetVillageHeaderIndex( SI16 siIndex )	{ return &m_VillageHeader[siIndex]; };
	VillageHeader			*GetVillageHeaderCode(UI16 uiCode);
	BOOL					IsStructure(SI32 HeaderIndex, UI16 code);							// 해당 건물이 존재하는가?
	VillageType				*GetType( SI16 siType, SI16 StateCode);								// 마을의 타입을 얻는다.
	BOOL					GetStructKind(SI32 siKind, CHAR &szKind);							// 건물종류를 문자열로 바꿔준다.

private:
	SI16					GetType( char *pToken );											// 마을의 타입을 리턴해준다.
	BOOL					LoadType( char *pFileStruct );										// 마을 내의 건물의 위치를 읽어 온다.
	SI16					GetVillageKind( CHAR *pFileName );									// 건물의 종류를 알아 냄
};


#endif