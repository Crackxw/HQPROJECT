#ifndef	_GSCPROTOCOL_HEADER
#define	_GSCPROTOCOL_HEADER

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 클라이언트와의 프로토콜
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0 - 999			:	서버 접속 및 게임 로그인 관련.
// 1000 - 1999		:	필드 내에서...(전용)		(1900~1999) 저자거리
// 2000 - 2999		:	마을 내에서...(전용)
// 3000 - 3999		:	전투 내에서...(전용)
// 4000 - 4999		:	마을과 전투 공통.(공통)
// 5000 - 5999		:	운영자 명령...
// 6000 - 6999		:	필드와 마을 공통.(공통)
// 7000 - 7499		:	좌판대
// 7500 - 7999		:	거래
// 8000 - 8999		:	퀘스트 관련
// 9000 - 9999		:   스킬(9000~9100), 경매소(9100~9200)  관련 ( 지역 탐사등 ) 특수 NPC관련 대장간(9201~9300)
// 10000 - 10999	:	게임 시스템적인것(시간등)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속 거부
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_RESPONSE_SERVER_FULL					(DWORD(1))	// 사용자가 너무 많아서 접속을 거부한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속하는 클라이언트의 종류를 구분한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define	ON_REQUEST_CLIENTKIND					(DWORD(2))
//#define ON_RESPONSE_CLIENTKIND					(DWORD(3))

//#define	ON_RET_CLIENT_KIND_OK					(DWORD(0))	// 정상적인 클라이언트다.
//#define ON_RET_CLIENT_KIND_CLIENT				(DWORD(1))	// 다른 클라이언트다.
//#define ON_RET_CLIENT_KIND_USER					(DWORD(2))	// 해당 유저용의 클라이언트가 아니다.
//#define	ON_RET_CLIENT_KIND_INVALIDVERSION		(DWORD(3))	// 버전이 맞지 않다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속하는 서버종류를 구분한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_REQUEST_SERVERTYPE					(DWORD(17))
//#define ON_RESPONSE_SERVERTYPE					(DWORD(18))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 서버 상태
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_REQUEST_NUMUSER					    (DWORD(19))
//#define ON_RESPONSE_NUMUSER						(DWORD(20))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그인한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_LOGIN						(DWORD(4))	// 로그인을 요청한다.
#define ON_RESPONSE_LOGIN						(DWORD(5))	// 요청한 로그인에 대한 응답

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐릭터 정보를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_REQUEST_CHARINFO						(DWORD(21))
//#define ON_RESPONSE_CHARINFO					(DWORD(22))

//#define ON_RET_CHARINFO_OK						(DWORD(0))	// 정보를 얻어오는데 성공 
//#define ON_RET_CHARINFO_FAIL					(DWORD(1))	// 정보를 얻어오는데 실패 

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그아웃한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_LOGOUT						(DWORD(6))	// 로그아웃을 요청한다.
#define ON_RESPONSE_LOGOUT						(DWORD(7))	// 요청한 로그아웃에 대한 응답

#define ON_RET_LOGOUT_OK						(DWORD(0))	// 로그 아웃을 허락한다.
//#define ON_RET_LOGOUT_FORCE						(DWORD(1))	// 강제 로그아웃 시킨다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// GameGuard관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_CHECK_GAMEGUARD						(DWORD(40))	// Server->Client 
#define ON_REQUEST_CHECK_GAMEGUARD				(DWORD(41)) // Client->Server
#define ON_RESPONSE_CHECK_GAMEGUARD				(DWORD(42)) // Server->Client 
#define ON_USER_INFO_GAMEGUARD					(DWORD(43)) // Server->Client
// #define ON_RET_BATTLE_NO						(DWORD(11))	// 전투를 거부한다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 새로운 캐릭터를 만든다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_REQUEST_CHENTRY						(DWORD(10))
//#define ON_RESPONSE_CHENTRY						(DWORD(11))

//#define ON_RET_CHENTRY_OK						(DWORD(0))	// 가입을 허락한다.
//#define ON_RET_CHENTRY_ID						(DWORD(1))	// 같은 ID를 쓰는 사람이 존재한다.
//#define ON_RET_CHENTRY_CHAR						(DWORD(2))	// 이미 3명이  다 찼다.
//#define ON_RET_CHENTRY_NO						(DWORD(3))	// 가입 처리를 할 수 없다.
//#define ON_RET_CHENTRY_ABILITY					(DWORD(4))	// 능력치에 이상이 있다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐릭터를 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_REQUEST_DELCHAR						(DWORD(12))
//#define ON_RESPONSE_DELCHAR						(DWORD(13))

//#define ON_RET_DELCHAR_OK						(DWORD(0))	// 가입을 허락한다.
//#define ON_RET_DELCHAR_NO						(DWORD(1))	// 가입 처리를 할 수 없다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 알림 보드를 요청한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_REQUEST_MESSAGE						(DWORD(15))
//#define ON_RESPONSE_MESSAGE						(DWORD(16))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어를 맵상에서 위치를 이동시킨다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_POSITION						(DWORD(1005))		// 클라이언트에서 현재 주인공 캐릭터가 위치한 타일의 위치를 알려준다.
#define ON_RESPONSE_POSITION					(DWORD(1006))		// 서버에서 클라이언트에게 이동한 캐릭터들의 위치를 알려준다.

//#define ON_RET_POSITION_OK						(DWORD(0))	// 이동을 허락한다.
//#define ON_RET_POSITION_NO						(DWORD(1))	// 이동금지
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_BATTLE						(DWORD(1007))	// 전투를 요청한다. 
#define ON_RESPONSE_BATTLE						(DWORD(1008))	// 요청한 전투의 대한 응답
#define ON_RESPONSE_BATTLEPLAYER				(DWORD(1100))	// 전투를 한다고 다른 사용자에게 알림

#define ON_RET_BATTLE_WAIT						(DWORD(0))	// 전투 대기상태로 들어간다.
#define ON_RET_BATTLE_BEGIN						(DWORD(1))	// 전투를 시작한다.
#define ON_RET_BATTLE_NO						(DWORD(2))	// 전투를 거부한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어의 이동시 한줄의 정보를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define ON_RESPONSE_INFO_LINE					(DWORD(1009))

//#define ON_RET_NEW_PLAYERS						(DWORD(0))	// 새로운 플레이어들을 보낸다.
//#define ON_RET_NEW_ITEM							(DWORD(1))	// 새발견된 아이템들을 보낸다.
//#define ON_RET_RELEASE_PLAYERS					(DWORD(2))	// 삭제될 플레이어들을 보낸다.
//#define ON_RET_RELEASE_ONE_PLAYER				(DWORD(3))	// 삭제될 플레이어를 보낸다.
#define ON_RET_BATTLE_NO						(DWORD(2))	// 전투를 거부한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어의 정보를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_PLAYER_INFO					(DWORD(1010))	// 플레이어의 정보를 요청한다.
#define	ON_RESPONSE_PLAYER_INFO					(DWORD(1011))	// 플레이어의 정보를 요청했을때의 응답을 받았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 맵에 아이템을 줍고 버린다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_THROWITEM					(DWORD(1012))
#define ON_RESPONSE_THROWITEM					(DWORD(1013))
#define ON_REQUEST_PICKUPITEM					(DWORD(1014))
#define ON_RESPONSE_PICKUPITEM					(DWORD(1015))
#define	ON_CHANGE_FIELDITEM						(DWORD(1022))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 이동할때 내 주위에서 새로 생기는 캐릭터나 없어지는 캐릭터.
#define	ON_NEWINFOONMOVE						(DWORD(1016))
#define	ON_ADDCHAR								(DWORD(1017))
#define	ON_DELCHAR								(DWORD(1018))
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define	ON_RESPONSE_AOURNDCHARINFO				(DWORD(1019))	// 주위 캐릭터들의 대한 정보.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐릭터의 상세한 정보를 얻어낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_REQUEST_CHAR_DETAILINFO				(DWORD(1020))	// 캐릭터의 상세 정보 요청.
#define	ON_RESPONSE_CHAR_DETAILINFO				(DWORD(1021))	// 캐릭터의 상세 정보 요청.(응답)

#define	ON_REQUEST_MOVEMAP						(DWORD(1030))	// 맵을 이동시에 메시지.
#define	ON_RESPONSE_MOVEMAP						(DWORD(1031))	// 맵을 이동시에 메시지.(응답)


// 배 관련
#define	ON_BOARDSHIP							(DWORD(1040))	// 배가 출항한다.
#define	ON_GETOUTSHIP							(DWORD(1041))	// 배에서 내린다.

// 장부관련 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define	ON_REQUEST_AUDIT_TOTAL					(DWORD(1500))	// 장부 총재산 정보
#define	ON_RESPONSE_AUDIT_TOTAL					(DWORD(1501))	// 장부 총재산 정보 (응답)
#define	ON_REQUEST_AUDIT_ACCOUNT				(DWORD(1502))	// 장부 계좌목록 정보
#define	ON_RESPONSE_AUDIT_ACCOUNT				(DWORD(1503))	// 장부 계좌목록 정보(응답)
#define	ON_REQUEST_AUDIT_OWN					(DWORD(1504))	// 장부 부동산보유 정보
#define	ON_RESPONSE_AUDIT_OWN					(DWORD(1505))	// 장부 부동산보유 정보(응답)
#define	ON_REQUEST_AUDIT_VILLAGE_LIST			(DWORD(1506))	// 장부 투자한 마을 리스트
#define	ON_RESPONSE_AUDIT_VILLAGE_LIST			(DWORD(1507))	// 장부 투자한 마을 리스트(응답)
#define	ON_REQUEST_AUDIT_VILLAGE_INFO			(DWORD(1508))	// 장부 투자한 한 마을 정보
#define	ON_RESPONSE_AUDIT_VILLAGE_INFO			(DWORD(1509))	// 장부 투자한 한 마을 정보(응답)
#define	ON_REQUEST_AUDIT_STATUS					(DWORD(1510))	// 장부 무장 정보
#define	ON_RESPONSE_AUDIT_STATUS				(DWORD(1511))	// 장부 무장 정보(응답)
#define	ON_REQUEST_AUDIT_SETNAME				(DWORD(1512))	// 장부 이름 입력
#define	ON_RESPONSE_AUDIT_SETNAME				(DWORD(1513))	// 장부 이름 입력(응답)
#define ON_REQUEST_AUDIT_FRIEND_LIST			(DWORD(1514))	// 장부 친구 리스트
#define ON_RESPONSE_AUDIT_FRIEND_LIST			(DWORD(1515))	// 장부 친구 리스트(응답)
#define ON_REQUEST_AUDIT_FRIEND_SET_DENIAL		(DWORD(1516))	// 장부 친구 수신거부
#define ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL		(DWORD(1517))	// 장부 친구 수신거부(응답)
#define ON_REQUEST_AUDIT_FRIEND_ADD_ONE			(DWORD(1518))	// 장부 친구 추가
#define ON_RESPONSE_AUDIT_FRIEND_ADD_ONE		(DWORD(1519))	// 장부 친구 추가(응답)
#define ON_REQUEST_AUDIT_FRIEND_REMOVE_SOME		(DWORD(1520))	// 장부 친구 한명 삭제
#define ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME	(DWORD(1521))	// 장부 친구 한명 삭제(응답)
#define ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL		(DWORD(1522))	// 장부 친구 전체 삭제
#define ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL		(DWORD(1523))	// 장부 친구 전체 삭제

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define	ON_REQUEST_AUDIT_PROFIT_STATISTICS		(DWORD(1524))	// 장부 마을 수익 내역
#define ON_RESPONSE_AUDIT_PROFIT_STATISTICS		(DWORD(1525))	// 장부 마을 수익 내역(응답)

#define	ON_RET_AUDIT_OK							(DWORD(0))		// 장부에서 일반적인 성공.
#define	ON_RET_AUDIT_NO							(DWORD(1))		// 장부에서 일반적인 실패.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 아이템을 착용, 해제 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_WEARITEM_INFO				(DWORD(1550))			//	착용한 아이템 정보
#define ON_RESPONSE_WEARITEM_INFO				(DWORD(1551))			//	착용한 아이템 정보(응답)
#define ON_REQUEST_WEARITEM						(DWORD(1552))			//	아이템을 착용한다.
#define ON_RESPONSE_WEARITEM					(DWORD(1553))			//	아이템을 착용한다.(응답)
#define ON_REQUEST_STRIPITEM					(DWORD(1554))			//	아이템을 해제한다.
#define ON_RESPONSE_STRIPITEM					(DWORD(1555))			//	아이템을 해제한다.(응답)

#define ON_CHANGE_CLOTHES						(DWORD(1556))		    // 주변사람의 아이템 착용한게 바꼈을때.

#define ON_CHANGE_CHARACTERCODE					(DWORD(1557))		    // 주변사람의 캐릭터 코드가 바꼈을때.
//#define ON_CHANGE_CHARACTERNATION				(DWORD(1558))		    // 국적이 바뀌었을때.
//OnChangeClothesMsg


#define ON_RET_WEARITEM_OK						(DWORD(0))				//	전반적인 OK
#define ON_RET_WEARITEM_NO						(DWORD(1))				//	전반적인 NO

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투모드, 평화모드 설정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_SET_ATTACK_MODE				(DWORD(1560))			// 전투모드 요청
#define ON_RESPONSE_SET_ATTACK_MODE				(DWORD(1561))			// 전투모드 요청에 대한 응답

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 몬스터가 자신을 노리거나, 벗어났을때
#define ON_TARGETME								(DWORD(1570))	// 몹이 자신을 노릴때.
#define ON_RELEASEME							(DWORD(1571))	// 타겟에서 벗어날때.
// 이모티콘 표시
#define ON_REQUEST_IMOTICON						(DWORD(1572))	// 이모티콘 표정을 했을때.
#define ON_RESPONSE_IMOTICON					(DWORD(1573))
// 아이템 소모
#define ON_REQUEST_SPENDITEM					(DWORD(1574))
#define ON_RESPONSE_SPENDITEM					(DWORD(1575))
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을안내문 관련 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_REQUEST_WRITE_DESCRIPT				(DWORD(1600))	// 안내문 쓴다는 메세지(마을)
#define	ON_RESPONSE_WRITE_DESCRIPT				(DWORD(1601))	// 안내문 쓴다는 메세지(응답)
#define	ON_REQUEST_READ_DESCRIPT				(DWORD(1602))	// 안내문 읽는 메세지(필드)
#define	ON_RESPONSE_READ_DESCRIPT				(DWORD(1603))	// 안내문 읽는 메세지(응답)

#define ON_RET_DESCRIPT_OK						(DWORD(0))
#define ON_RET_DESCRIPT_NO						(DWORD(1))
#define ON_RET_DESCRIPT_NOTBEST					(DWORD(2))

#define ON_REQUEST_VILLAGE_NATION				(DWORD(1604))
#define ON_RESPONSE_VILLAGE_NATION				(DWORD(1605))

//OnReqVillageGetNation

// 워프 관련
#define	ON_REQUEST_WARP							(DWORD(1800))	// 워프
#define	ON_RESPONSE_WARP						(DWORD(1801))	// 워프(응답)

// 저자 거리
#define	ON_REQUEST_CHARGEBOOTH					(DWORD(1900))	// 좌판대 들어가기
#define	ON_RESPONSE_CHARGEBOOTH					(DWORD(1901))	// 좌판대 들어가기(응답)
#define ON_REQUEST_DISPLAYBOOTH					(DWORD(1902))	// 좌판대의 아이템들 보여주기
#define ON_RESPONSE_DISPLAYBOOTH				(DWORD(1903))
//#define ON_REQUEST_INPUTBOOTH_DESC				(DWORD(1904))	// 좌판대 물건 정보를 입력
#define ON_RESPONSE_INPUTBOOTH_DESC				(DWORD(1905))
#define ON_REQUEST_GETBOOTHINFO					(DWORD(1906))	// 좌판대 물건 정보를 얻어옴
#define ON_RESPONSE_GETBOOTHINFO				(DWORD(1907))
#define	ON_REQUEST_LEAVEBOOTH					(DWORD(1908))	// 좌판대 나가기
#define	ON_RESPONSE_LEAVEBOOTH					(DWORD(1909))	// 좌판대 나가기(응답)
#define	ON_REQUEST_SETBOOTHITEM					(DWORD(1910))	// 좌판대 물건 놓기
#define	ON_RESPONSE_SETBOOTHITEM				(DWORD(1911))	// 좌판대 나가기
#define	ON_REQUEST_CANCELBOOTHITEM				(DWORD(1912))	// 좌판대 놨던 물건 취소
#define	ON_RESPONSE_CANCELBOOTHITEM				(DWORD(1913))	// 좌판대 놨던 물건 취소(응답)
//#define	ON_REQUEST_BUYBOOTHITEM					(DWORD(1914))	// 좌판대 놨던 물건 구입
#define	ON_RESPONSE_BUYBOOTHITEM				(DWORD(1915))	// 좌판대 놨던 물건 구입(응답)
#define	ON_RESPONSE_SELLBOOTHITEM				(DWORD(1916))	// 물건이 팔리면 주인이 받는 메세지

#define ON_RET_MARKETPLACE_OK					(DWORD(0))
#define ON_RET_MARKETPLACE_NO					(DWORD(1))
//#define ON_RET_MARKETPLACE_FULL					(DWORD(2))		// 꽉찬 상태, 더이상 할 수 없다.
#define	ON_RET_MARKETPLACE_EMPTY				(DWORD(3))		// 빈상태, 주인이 없다.
#define	ON_RET_MARKETPLACE_PLAYER				(DWORD(4))		// 다른 사용자가 사용중


#define	ON_REQUEST_ATTACK_VILLAGE				(DWORD(1980))	// 마을을 공격한다.
#define	ON_RESPONSE_ATTACK_VILLAGE				(DWORD(1981))	// 마을을 공격한다.(응답)

#define	ON_CHANGED_VILLAGE_DEFENCE				(DWORD(1990))	// 마을의 방어력이 변경되었다.
// 마을관련 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_ENTER_VILLAGE				(DWORD(2000))	// 마을에 들어간다.
#define ON_RESPONSE_ENTER_VILLAGE				(DWORD(2001))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_LEAVE_VILLAGE				(DWORD(2002))	// 마을에서 떠난다.
#define ON_RESPONSE_LEAVE_VILLAGE				(DWORD(2003))


#define ON_RET_LEAVE_VILLAGE_OK					(DWORD(0))	// 허가한다.
#define ON_RET_LEAVE_VILLAGE_NO					(DWORD(1))	// 거부한다.
//#define ON_RET_LEAVE_VILLAGE_PLAYER				(DWORD(2))	// 다른플레이어가 마을에서 떠났다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_LIST_ITEM					(DWORD(2004))	// 시장에서 물품 리스트 요청5
#define ON_RESPONSE_LIST_ITEM					(DWORD(2005))
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_BUY_ITEM						(DWORD(2006))	// 물건을 산다.
#define ON_RESPONSE_BUY_ITEM					(DWORD(2007))

#define ON_RET_NOITEM							(DWORD(1))	// 팔 물건이 없을경우
#define ON_RET_NOMONEY							(DWORD(2))	// 플레이어의 돈이 모자를 경우
#define	ON_RET_NOSALE							(DWORD(3))	// 판매하지 않는 아이템이다.
#define	ON_RET_BUYZERO							(DWORD(4))	// 0개를 사려고 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_SELL_ITEM					(DWORD(2008))	// 물건을 판다.
#define ON_RESPONSE_SELL_ITEM					(DWORD(2009))
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_RESPONSE_CHANGED_ITEMINFO			(DWORD(2012))	// 다른 플레이어에의해 시장의 아이템리스트에 변동이 있을경우
																// 그 변동된 아이템들의 정보를 전달한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 마을, 전장에서의 메시지
#define	ON_REQUEST_BANK_INFO					(DWORD(2016))	// 은행 정보 요구.
#define	ON_RESPONSE_BANK_INFO					(DWORD(2017))
#define	ON_REQUEST_BANK_DEPOSIT					(DWORD(2018))	// 예금
#define ON_RESPONSE_BANK_DEPOSIT				(DWORD(2019))
#define	ON_REQUEST_BANK_DRAWING					(DWORD(2020))	// 출금
#define ON_RESPONSE_BANK_DRAWING				(DWORD(2021))
#define	ON_REQUEST_BANK_OPENSAVINGACCOUNT		(DWORD(2022))	// 계좌를 만든다고 요청한다.
#define	ON_RESPONSE_BANK_OPENSAVINGACCOUNT		(DWORD(2023))	// 계좌를 만든다고 요청했을때의응답
#define	ON_REQUEST_BANK_BANKBOOK_LIST			(DWORD(2024))	// 통장의 리스트를 달라고 요청한다.
#define	ON_RESPONSE_BANK_BANKBOOK_LIST			(DWORD(2025))	// 통장의 리스트를 달라고 했을때의 응답
#define	ON_REQUEST_BANK_BANKBOOK_LISTCOUNT		(DWORD(2026))	// 통장의 리스트의 수를 달라고 요청한다.

#define ON_REQUEST_BANK_SAVE                    (DWORD(2600))	// 입금 요청
#define ON_RESPONSE_BANK_SAVE                   (DWORD(2601))	// 입금 응답
#define ON_REQUEST_BANK_WITHDRAW                (DWORD(2602))	// 출금 요청
#define ON_RESPONSE_BANK_WITHDRAW               (DWORD(2603))	// 출금 응답


#define	ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT		(DWORD(2499))	// 통장의 리스트의 수를 달라고 했을때의 응답

#define	ON_REQUEST_BANK_COLLATERAL				(DWORD(2500))	// 담보를 맡긴다고 요청한다.
#define	ON_RESPONSE_BANK_COLLATERAL				(DWORD(2501))	// 담보를 맡긴다고 요청했을때의 응답
#define	ON_REQUEST_BANK_COLLATERALLIST			(DWORD(2502))	// 담보List를 요청한다.
#define	ON_RESPONSE_BANK_COLLATERALLIST			(DWORD(2503))	// 담보List를 요청했을때의 응답
#define ON_REQUEST_BANK_TAKECOLLATERAL			(DWORD(2504))	// 담보를 돌려달라고 요청한다.
#define ON_RESPONSE_BANK_TAKECOLLATERAL			(DWORD(2505))	// 담보를 돌려달라고 요청 했을때의 응답.
//#define ON_REQUEST_BANK_MAXCREDITLOAN			(DWORD(2506))	//	신용담보 최대 금액
#define ON_RESPONSE_BANK_MAXCREDITLOAN			(DWORD(2507))



#define	ON_RET_BANK_OK							(DWORD(0))		// 전반적인 OK
#define	ON_RET_BANK_NO							(DWORD(1))		// 전박전인 NO
#define	ON_RET_BANK_EXISTOPENSAVINGACCOUNT		(DWORD(3))		// 해당 계좌가 이미 있다.
#define	ON_RET_BANK_FULLSAVINGACCOUNT			(DWORD(4))		// 더 이상의 해당 계좌를 만들 수가 없다.(현재 만들려고 하는 계좌에 한해서)
#define	ON_RET_BANK_FULLTOTALSAVINGACCOUNT		(DWORD(5))		// 더 이상의 해당 계좌를 만들 수가 없다.(전체 만든 계좌에 한해서.)
//#define	ON_RET_BANK_MAXMONEY					(DWORD(6))		// 한계 액수를 넘었다.
//#define	ON_RET_BANK_MINMONEY					(DWORD(7))		// 최소 액수보다 작다.


// 마을, 농장에서의 메시지
#define ON_REQUEST_FARM_INFO					(DWORD(2530))	//	농장 정보를 요청함.
#define	ON_RESPONSE_FARM_INFO					(DWORD(2531))
#define ON_REQUEST_FARM_BUY						(DWORD(2532))	//	농장을 구입한다고 요청함.
#define ON_RESPONSE_FARM_BUY					(DWORD(2533))
#define ON_REQUEST_FARM_SLOTINFO				(DWORD(2534))	//	슬롯 정보를 요청함.
#define ON_RESPONSE_FARM_SLOTINFO				(DWORD(2535))
//#define ON_REQUEST_FARM_STRUCINFO				(DWORD(2536))	//	건물 정보를 요청함.
//#define ON_RESPONSE_FARM_STRUCINFO				(DWORD(2537))
#define ON_REQUEST_FARM_BUILDSTRUCT				(DWORD(2538))	//	건물을 짓는다고 요청함.
#define ON_RESPONSE_FARM_BUILDSTRUCT			(DWORD(2539))
#define ON_REQUEST_FARM_EXPANDSLOT				(DWORD(2540))	//	슬롯 시설 확장 한다고 요청.
#define ON_RESPONSE_FARM_EXPANDSLOT				(DWORD(2541))
#define ON_REQUEST_FARM_DESTROYSTRUCT			(DWORD(2542))	//	건물을 철거한다고 요청함.
#define ON_RESPONSE_FARM_DESTROYSTRUCT			(DWORD(2543))
#define ON_REQUEST_FARM_SETEQUIPMENT			(DWORD(2544))	//	건물에 부수 시설물을 짓는다고 요청함.
#define ON_RESPONSE_FARM_SETEQUIPMENT			(DWORD(2545))
#define ON_REQUEST_FARM_ITEMPRODUCT				(DWORD(2546))	//	농장에서 아이템을 생산할 것을 요청한다.
#define ON_RESPONSE_FARM_ITEMPRODUCT			(DWORD(2547))
#define ON_REQUEST_FARM_WORK					(DWORD(2548))	//	농장에서 일을 한다고 요청한다.
#define ON_RESPONSE_FARM_WORK					(DWORD(2549))
#define ON_REQUEST_FARM_WORKINFO				(DWORD(2550))	//	농장의 일꾼 정보를 보낸다.
#define ON_RESPONSE_FARM_WORKINFO				(DWORD(2551))
#define ON_REQUEST_FARM_UPGRADESLOT				(DWORD(2552))	//	슬롯을 업그레이드한다.
#define ON_RESPONSE_FARM_UPGRADESLOT			(DWORD(2553))
#define ON_REQUEST_FARM_WORKSTART				(DWORD(2554))	//	농장에서 일을 하려 한다.
#define ON_RESPONSE_FARM_WORKSTART				(DWORD(2555))
//#define ON_REQUEST_FARM_SELL					(DWORD(2556))	//	농장을 판다.
//#define ON_RESPONSE_FARM_SELL					(DWORD(2557))
#define	ON_FARMWORK_COMPLETED					(DWORD(2558))	// 현재 농장에서 작업들이 완료되었다.
#define ON_REQUEST_FARM_WORK_HACK				(DWORD(2559))


// 창고
#define ON_REQUEST_STORAGE_INFO					(DWORD(2580))	//	창고의 아이템을 얻어온다.
#define ON_RESPONSE_STORAGE_INFO				(DWORD(2581))
#define ON_REQUEST_STORAGE_INPUTITEM			(DWORD(2582))	//	창고에 아이템을 넣는다.
#define ON_RESPONSE_STORAGE_INPUTITEM			(DWORD(2583))
#define ON_REQUEST_STORAGE_OUTPUTITEM			(DWORD(2584))	//	창고에서 아이템을 뺀다.
#define ON_RESPONSE_STORAGE_OUTPUTITEM			(DWORD(2585))

#define ON_REQUEST_PLANT_CHANGE_PAY				(DWORD(2590))	//	임금을 바꿀때 메세지
#define ON_RESPONSE_PLANT_CHANGE_PAY			(DWORD(2591))
#define ON_REQUEST_PLANT_CANCEL_PLAN			(DWORD(2592))	//	임금을 바꿀때 메세지
#define ON_RESPONSE_PLANT_CANCEL_PLAN			(DWORD(2593))

// 창고 및 농장, 목장, 광산, 공장 에서의 결과 값.
//#define ON_RET_FARM_OWNER						(DWORD(2))		//	농장 소유주의 자격이다.
//#define	ON_RET_FARM_WORKER						(DWORD(3))		//	농장 일꾼의 자격이다.
#define	ON_RET_FARM_MATERIAL					(DWORD(4))		//	재료가 부족하다.
#define	ON_RET_FARM_TOOL						(DWORD(5))		//	도구가 없다.
#define	ON_RET_FARM_MONEY						(DWORD(6))		//	돈이 부족하다.
#define	ON_RET_FARM_NOTOWNER					(DWORD(7))		//	농장 주인이 아니기때문에 할 수 없다.
#define	ON_RET_FARM_ITEMERROR					(DWORD(8))		//	그런 아이템은 생산할 수 없다.
//#define	ON_RET_FARM_FULLUPGRAGE					(DWORD(9))		//	더이상 업그레이드 할 수 없다.

// 마을, 주점에서의 메시지.
#define	ON_REQUEST_ENTER_INN					(DWORD(2027))	// 마을 주점에 들어간다.
#define	ON_RESPONSE_ENTER_INN					(DWORD(2028))	// 마을 주점에 들어간다.(응답)
#define	ON_REQUEST_LEAVE_INN					(DWORD(2029))	// 마을 주점에서 나간다
#define	ON_RESPONSE_LEAVE_INN					(DWORD(2030))	// 마을 주점에서 나간다.(응답)


#define	ON_REQUEST_ENTER_VWR					(DWORD(2031))	// 마을 주점, 대기실에 들어간다.							
#define	ON_RESPONSE_ENTER_VWR					(DWORD(2032))	// 마을 주점, 대기실에 들어간다.(응답)
#define	ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM		(DWORD(2033))	// 마을 주점, 대화방에 들어간다.
#define	ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM		(DWORD(2034))	// 마을 주점, 대화방에 들어간다.(응답)
#define	ON_REQUEST_INN_CREATECHATROOM			(DWORD(2035))	// 마을 주점, 대화방을 만든다.
#define	ON_RESPONSE_INN_CREATECHATROOM			(DWORD(2036))	// 마을 주점, 대화방을 만든다.(응답)
#define	ON_WAITROOM_CHANGE_INFO					(DWORD(2037))	// 마을 주점, 대기실에 변경된 정보를 보내준다.
#define	ON_RET_INN_OK							(DWORD(0))
#define ON_RET_INN_NO							(DWORD(1))
#define	ON_VWR_CREATEROOM						(DWORD(0))		// 새로운 방이 개설되었다.
#define	ON_VWR_DELETEROOM						(DWORD(1))		// 기존의 방이 폐쇄 되었다.
#define	ON_VWR_CHANGEPLAYERNUM					(DWORD(2))		// 기존의 방에 플레이어 수가 변경되었다.

#define	ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM		(DWORD(2038))	// 마을 주점, 대화방에서 나간다.
#define	ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM		(DWORD(2039))	// 마을 주점, 대화방에서 나간다.(응답)
	
#define	ON_VIL_INN_JOIN_PLAYER					(DWORD(2040))	// 마을 주점, 플레이어가 추가되었다.
#define	ON_VIL_INN_LEAVE_PLAYER					(DWORD(2041))	// 마을 주점, 플레이어가 삭제되었다.

#define	ON_VIL_INN_CHATTING						(DWORD(2042))	// 마을 주점, 대화방에서 채팅

#define	ON_REQUEST_ENTER_MARKET					(DWORD(2050))	// 시장에 들어간다.
#define	ON_RESPONSE_ENTER_MARKET				(DWORD(2051))	// 시장에 들어간다.(응답)
#define	ON_REQUEST_LEAVE_MARKET					(DWORD(2052))	// 시장에서 나온다.
#define	ON_RESPONSE_LEAVE_MARKET				(DWORD(2053))	// 시장에서 나온다.(응답)

#define	ON_REQUEST_ENTER_NOTICEBOARD			(DWORD(2060))	// 게시판에 들어간다.
#define	ON_RESPONSE_ENTER_NOTICEBOARD			(DWORD(2061))	// 게시판에서 나간다.(응답)
#define	ON_REQUEST_NOTICEBOARD_NOTICELIST		(DWORD(2062))	// 게시판에서 게시물 리스트를 요청한다.
#define	ON_RESPONSE_NOTICEBOARD_NOTICELIST		(DWORD(2063))	// 게시판에서 게시물 리스트를 보낸다.
#define	ON_REQUEST_REGIST_NOTICE				(DWORD(2064))	// 게시판에서 글을 등록한다.
#define	ON_RESPONSE_REGIST_NOTICE				(DWORD(2065))	// 게시판에서 글을 등록한다.(응답)
#define	ON_REQUEST_DELETE_NOTICE				(DWORD(2066))	// 게시판의 글을 삭제한다.
#define	ON_RESPONSE_DELETE_NOTICE				(DWORD(2067))	// 게시판의 글을 삭제한다.(응답)
#define	ON_REQUEST_VIEW_NOTICE					(DWORD(2070))	// 게시판의 글을 본다.
#define	ON_RESPONSE_VIEW_NOTICE					(DWORD(2071))	// 게시판의 글을 본다.(응답)
#define ON_REQUEST_MODIFY_NOTICE				(DWORD(2072))	// 게시판의 글을 수정한다.
#define ON_RESPONSE_MODIFY_NOTICE				(DWORD(2073))	// 게시판의 글을 수정한다.

#define ON_REQUEST_BARRACK_GETSOLDERLIST			(DWORD(2090))	// 용병들의 List를 요청한다.
#define ON_RESPONSE_BARRACK_GETSOLDERLIST			(DWORD(2091))	// 용병들의 List를 요청에 응답한다.
#define ON_REQUEST_BARRACK_BUYSOLDER				(DWORD(2092))	// 용병을 고용한다.
#define ON_RESPONSE_BARRACK_BUYSOLDER				(DWORD(2093))	// 용병을 고용에 응답한다.
#define ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST	(DWORD(2094))	// 해고할 용병들의 List를 요청한다.
#define ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST	(DWORD(2095))	// 해고할 용병들의 List를 요청에 응답한다.
#define ON_REQUEST_BARRACK_DISMISSALSOLDER			(DWORD(2096))	// 용병을 해고한다.
#define ON_RESPONSE_BARRACK_DISMISSALSOLDER			(DWORD(2097))	// 용병을 해고에 응답한다.

#define ON_REQUEST_GBARRACK_GETSOLDERLIST			(DWORD(2130))	// 장수용병들의 List를 요청한다.
#define ON_RESPONSE_GBARRACK_GETSOLDERLIST			(DWORD(2131))	// 장수용병들의 List를 요청에 응답한다.
#define ON_REQUEST_GBARRACK_BUYSOLDER				(DWORD(2132))	// 장수용병을 고용한다.
#define ON_RESPONSE_GBARRACK_BUYSOLDER				(DWORD(2133))	// 장수용병을 고용에 응답한다.
#define ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST	(DWORD(2134))	// 전직할 장수용병들의 List를 요청한다.
#define ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST	(DWORD(2135))	// 전직할 장수용병들의 List를 요청에 응답한다.
#define ON_REQUEST_GBARRACK_CHANGEJOBSOLDER			(DWORD(2136))	// 장수용병을 전직한다.
#define ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER		(DWORD(2137))	// 장수용병을 전직에 응답한다.
#define ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST	(DWORD(2138))	// 해고할 장수용병들의 List를 요청한다.
#define ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST	(DWORD(2139))	// 해고할 장수용병들의 List를 요청에 응답한다.
#define ON_REQUEST_GBARRACK_DISMISSALSOLDER			(DWORD(2140))	// 장수용병을 해고한다.
#define ON_RESPONSE_GBARRACK_DISMISSALSOLDER		(DWORD(2141))	// 장수용병을 해고에 응답한다.

#define ON_REQUEST_MBARRACK_GETMONSTERLIST			(DWORD(2150))	// 몬스터들의 List를 요청한다.
#define ON_RESPONSE_MBARRACK_GETMONSTERLIST			(DWORD(2151))	// 몬스터들의 List를 요청에 응답한다.
#define ON_REQUEST_MBARRACK_BUYMONSTER				(DWORD(2152))	// 몬스터를 고용한다.
#define ON_RESPONSE_MBARRACK_BUYMONSTER				(DWORD(2153))	// 몬스터 고용에 응답한다.
#define ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST	(DWORD(2154))	// 해고할 몬스터들의 List를 요청한다.
#define ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST	(DWORD(2155))	// 해고할 몬스터들의 List 요청에 응답한다.
#define ON_REQUEST_MBARRACK_DISMISSMONSTER			(DWORD(2156))	// 몬스터를 해고한다.
#define ON_RESPONSE_MBARRACK_DISMISSMONSTER			(DWORD(2157))	// 몬스터 해고에 응답한다.

#define ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST		(DWORD(2160))	// 2차 전직할 장수들의 List를 요청한다.
#define ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST	(DWORD(2161))	// 2차 전직할 장수들의 List를 요청에 응답한다.
#define ON_REQUEST_GBARRACK_CHANGE_GENERAL				(DWORD(2162))	// 2차 장수를 전직 한다.
#define ON_RESPONSE_GBARRACK_CHANGE_GENERAL				(DWORD(2163))	// 2차 장수 전직에 응답 한다.
#define ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST	(DWORD(2164))	// 해고할 2차 장수들의 List를 요청한다.
#define ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST	(DWORD(2165))	// 해고할 2차 장수들의 List를 요청에 응답한다.
#define ON_REQUEST_GBARRACK_DISMISSAL_GENERAL			(DWORD(2166))	// 2차 장수 해고한다.
#define ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL			(DWORD(2167))	// 2차 장수 해고에 응답한다.
#define ON_REQUEST_GBARRACK_GET_GENERAL_LIST			(DWORD(2168))	// 2차 장수들의 List를 요청한다.
#define ON_RESPONSE_GBARRACK_GET_GENERAL_LIST			(DWORD(2169))	// 2차 장수들의 List를 요청에 응답한다.
#define ON_REQUEST_GBARRACK_BUY_GENERAL					(DWORD(2170))	// 2차 장수를 고용한다.
#define ON_RESPONSE_GBARRACK_BUY_GENERAL				(DWORD(2171))	// 2차 장수 고용에 응답한다.


#define ON_REQUEST_HOSPITAL_GETCURELIST			(DWORD(2100))	// 용병들의 치료 List를 요청한다.
#define ON_RESPONSE_HOSPITAL_GETCURELIST		(DWORD(2101))	// 용병들의 치료 List를 요청에 응답한다.
#define ON_REQUEST_HOSPITAL_CURESOLDER			(DWORD(2102))	// 용병을 치료한다.
#define ON_RESPONSE_HOSPITAL_CURESOLDER			(DWORD(2103))	// 용병을 치료에 응답한다.
#define ON_REQUEST_HOSPITAL_CUREALLSOLDER		(DWORD(2104))	// 모든 용병을 치료한다.
#define ON_RESPONSE_HOSPITAL_CUREALLSOLDER		(DWORD(2105))	// 모든 용병을 치료에 응답한다.
#define ON_RET_AUTOCUREOK						(DWORD(2))		// 자동치료를 허가한다.

#define ON_REQUEST_WHARF_ENTER					(DWORD(2110))	// 부두에 들어간다.
#define ON_RESPONSE_WHARF_ENTER					(DWORD(2111))	// 부두에 들어간다.(응답)
#define ON_RESPONSE_WHARF_CHANGETICKETLIST		(DWORD(2112))	// 표의 List를 업데이트 한다.
#define ON_REQUEST_WHARF_BUYTICKET				(DWORD(2113))	// 부두에서 표를 산다.
#define ON_RESPONSE_WHARF_BUYTICKET				(DWORD(2114))	// 부두에서 표를 산다.(응답)
#define ON_REQUEST_WHARF_EXIT					(DWORD(2115))	// 부두에서 나간다.
#define ON_RESPONSE_WHARF_EXIT					(DWORD(2116))	// 부두에서 나간다.(응답)
#define ON_REQUEST_WHARFWAITROOM_ENTER			(DWORD(2117))	// 부두대기실에 들어간다.
#define ON_RESPONSE_WHARFWAITROOM_ENTER			(DWORD(2118))	// 부두대기실에 들어간다.(응답)(가진 Ticket의 시간을 초기하면 못 들어가진다.)
#define ON_REQUEST_WHARFWAITROOM_EXIT			(DWORD(2119))	// 부두대기실에서 나간다.
#define ON_RESPONSE_WHARFWAITROOM_EXIT			(DWORD(2120))	// 부두대기실에서 나간다.(응답)
#define ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP	(DWORD(2121))	// 부두대기실에서 승선하다.
#define ON_RET_NOTUSETICKET						(DWORD(2))		// 티켓을 사용할수없는 경우

// 관청
#define ON_REQUEST_GOVERNMENT_MAIN_ENTER					(DWORD(2200))		// 관청 들어가기
#define ON_RESPONSE_GOVERNMENT_MAIN_ENTER					(DWORD(2201))		// 관청 들어가기 응답
#define ON_REQUEST_GOVERNMENT_MAIN_LEAVE					(DWORD(2202))		// 관청 나가기
#define ON_RESPONSE_GOVERNMENT_MAIN_LEAVE					(DWORD(2203))		// 관청 나가기 응답

#define ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER				(DWORD(2204))		// 관청, 투자 들어가기
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER				(DWORD(2205))		// 관청, 투자 들어가기 응답
#define ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE				(DWORD(2206))		// 관청, 투자 나가기
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE				(DWORD(2207))		// 관청, 투자 나가기 응답
#define ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST			(DWORD(2208))		// 관청, 투자 List얻기
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST			(DWORD(2209))		// 관청, 투자 List얻기 응답
#define ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT		(DWORD(2210))		// 관청, 투자 하기
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT		(DWORD(2211))		// 관청, 투자 하기 응답
#define	ON_RET_NOTINVESTMENT_MANYMAXINVESTMENT				(DWORD(2))
#define	ON_RET_NOTINVESTMENT_MANYMONEY						(DWORD(3))
#define	ON_RET_NOTINVESTMENT_TIMEOVER						(DWORD(4))

#define ON_REQUEST_GOVERNMENT_VILLAGEINFO					(DWORD(2212))		// 관청, 마을 정보 얻기 요청
#define ON_RESPONSE_GOVERNMENT_VILLAGEINFO					(DWORD(2213))		// 관청, 마을 정보 얻기 응답
#define	ON_REQUEST_PROFIT_STATISTICS						(DWORD(2214))
#define	ON_RESPONSE_PROFIT_STATISTICS						(DWORD(2215))

#define	ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT				(DWORD(2216))		// 관청, 투자한돈 가지고 나가기
#define	ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT			(DWORD(2217))		// 관청, 투자한돈 가지고 나가기 응답

// 상단
#define	ON_REQUEST_CREATE_GUILD								(DWORD(2300))		// 상단을 생성한다.(요구)
#define	ON_RESPONSE_CREATE_GUILD							(DWORD(2301))		// 상단을 생성한다.(응답)
#define	ON_REQUEST_DELETE_GUILD								(DWORD(2302))		// 상단을 삭제한다.(요구)
#define	ON_RESPONSE_DELETE_GUILD							(DWORD(2303))		// 상단을 삭제한다.(응답)
#define	ON_REQUEST_JOIN_LEAVE_GUILD							(DWORD(2304))		// 상단에 가입/탈퇴한다.(요구)
#define	ON_RESPONSE_JOIN_LEAVE_GUILD						(DWORD(2305))		// 상단에 가입/탈퇴한다.(응답)
#define	ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST			(DWORD(2306))		// 상단에 참여하기 원하는 참가대기자의 리스트를 얻어온다.(요구)
#define	ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST			(DWORD(2307))		// 상단에 참여하기 원하는 참가대기자의 리스트를 얻어온다.(응답)
#define	ON_REQUEST_JOINGUILD_OKNO							(DWORD(2308))		// 상단에 참여하기 원하는 참가 대기자에게 OK, NO를 보낸다.(요구)
#define	ON_RESPONSE_JOINGUILD_OKNO							(DWORD(2309))		// 상단에 참여하기 원하는 참가 대기자에게 OK, NO를 보낸다.(응답)
#define	ON_REQUEST_GUILD_MEMBERS_LIST						(DWORD(2310))		// 상단원 리스트를 얻어온다.(요구)
#define	ON_RESPONSE_GUILD_MEMBERS_LIST						(DWORD(2311))		// 상단원 리스트를 얻어온다.(응답)
#define	ON_REQUEST_GUILD_DISMISSAL							(DWORD(2312))		// 상단원을 해고한다.(요구)
#define	ON_RESPONSE_GUILD_DISMISSAL							(DWORD(2313))		// 상단원을 해고한다.(응답)
#define	ON_REQUEST_GUILD_LIST								(DWORD(2314))		// 상단의 리스트를 얻어온다.(요구)
#define	ON_RESPONSE_GUILD_LIST								(DWORD(2315))		// 상단의 리스트를 얻어온다.(응답)
#define	ON_GUILD_MSG										(DWORD(2316))		// 상단의 메시지
#define	ON_REQUEST_GUILD_SET_RELATION						(DWORD(2317))		// 상단간의 관계를 설정한다.(요구)
#define	ON_RESPONSE_GUILD_SET_RELATION						(DWORD(2318))		// 상단간의 관계를 설정한다.(응답)
#define ON_REQUEST_GUILD_CREATE_COST						(DWORD(2319))		// 상단 개설 비용을 얻어온다(요구)
#define ON_RESPONSE_GUILD_CREATE_COST						(DWORD(2320))		// 상단 개설 비용을 얻어온다(응답)

#define	ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT				(DWORD(2321))		// 마을방어력에 투자할수 있는 최고 투자액을 얻는다.(요구)
#define	ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT				(DWORD(2322))		// 마을방어력에 투자할수 있는 최고 투자액을 얻는다.(응답)
#define	ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT				(DWORD(2323))		// 마을방어력에 투자한다.(요구)
#define	ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT				(DWORD(2324))		// 마을방어력에 투자한다.(응답)

#define	ON_REQUEST_GUILD_DETAIL_INFO						(DWORD(2350))		// 상단의 세세한 정보를 얻어온다.(요구)
#define	ON_RESPONSE_GUILD_DETAIL_INFO						(DWORD(2351))		// 상단의 세세한 정보를 얻어온다.(응답)
#define ON_REQUEST_GUILD_PROMOTION							(DWORD(2352))		// 상단원을 특정 직급으로 임명한다.(요구)
#define ON_RESPONSE_GUILD_PROMOTION							(DWORD(2353))		// 상단원을 특정 직급으로 임명한다.(응답)
#define ON_REQUEST_GUILD_DEMOTION							(DWORD(2354))		// 상단원을 해임(직급에 대한 강등)한다.(요구)
#define ON_RESPONSE_GUILD_DEMOTION							(DWORD(2355))		// 상단원을 해임(직급에 대한 강등)한다.(응답)

#define	ON_ATTACK_TYPE_NEARCAPTAIN							(DWORD(0))
#define ON_ATTACK_TYPE_CAPTAIN								(DWORD(1))
#define ON_ATTACK_TYPE_NORMAL								(DWORD(2))

#define ON_REQUEST_FIELDATTACK_READY						(DWORD(2700))		// 공성 무기를 발사한다는 신호를 보낸다.
#define ON_RESPONSE_FIELDATTACK_READY						(DWORD(2701))		// 공성 무기를 발사한다는 신호를 받았다.

#define ON_REQUEST_FIELDATTACK_ACTION						(DWORD(2702))		//
#define ON_RESPONSE_FIELDATTACK_ACTION						(DWORD(2703))		//

#define ON_RET_ATTACK_POWER									(DWORD(0))			// 공3업
#define ON_RET_DEFENCE_POWER								(DWORD(1))			// 방업
#define ON_RET_NONE_POWER									(DWORD(2))			// 땡

#define ON_REQUEST_FIELDATTACK_DEAD							(DWORD(2704))		//
#define ON_RESPONSE_FIELDATTACK_DEAD						(DWORD(2705))		//

#define ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY			(DWORD(2706))
#define ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY		(DWORD(2707))
#define ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION		(DWORD(2708))
#define ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION		(DWORD(2709))

#define ON_RET_ATTACK_OK                                    (DWORD(0))         // ok
#define ON_RET_NOT_ATTACK_TIME                              (DWORD(1))         // 공성시간이 아니다.
#define ON_RET_NOT_VILLAGE_CODE                             (DWORD(2))         // 마을 코드가 틀리다.
#define ON_RET_NOT_ENEMYGUILD                               (DWORD(3))         // 적대 길드 마을이 아니다.
#define ON_RET_NOT_ATTACK_UNITE                             (DWORD(4))         // 공성 유닛이 아니다. 
#define ON_RET_NOT_ATTACK_DELAY_TIME                        (DWORD(5))         // 공격 딜레이 시간이 남았다. 
#define ON_RET_ATTACK_NO                                    (DWORD(6))


#define ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT					(DWORD(2731))		// 보급 수레로 부터 보급품을 지급받는다.
#define ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT				(DWORD(2732))		// (응답).

#define ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK				(DWORD(2729))		// 마을 공격형태를 바꿀수 있는지 알아온다.
#define ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK				(DWORD(2730))		// (응답).

#define ON_IMPERIUM_USE										(DWORD(2733))		//	소집권 사용.

#define ON_RET_ATTACK_TYPE_NOT_AUTH							(DWORD(0))			// 대방이나 대행수가 아니다.
#define ON_RET_ATTACK_TYPE_ACTIVE							(DWORD(1))			// 다른 마을 공격타입이 실행중이다.
#define ON_RET_ATTACK_TYPE_NO								(DWORD(2))			// 그밖의 다른 경우의 에러들.
#define ON_RET_ATTACK_TYPE_OK								(DWORD(3))			// 마을 AttackType변경이 설정되었다.
#define ON_RET_ATTACK_TYPE_DELAY							(DWORD(4))			// 제한시간이 아직 끝나지 않았다.

#define ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK				(DWORD(2710))		// 마을 공격형태를 기름공격으로 바꾼다.
#define ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK				(DWORD(2711))		// 응답

#define ON_RET_OIL_OK										(DWORD(0))
#define	ON_RET_OIL_NOT_MONEY								(DWORD(1))
#define	ON_RET_OIL_NO										(DWORD(2))

#define ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK				(DWORD(2712))		// 마을 공격형태를 돌공격으로 바꾼다.
#define ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK				(DWORD(2713))		// 응답

#define ON_RET_STONE_OK										(DWORD(0))
#define	ON_RET_STONE_NOT_MONEY								(DWORD(1))
#define	ON_RET_STONE_NO										(DWORD(2))

#define ON_REQUEST_VILLAGE_DEFENCE							(DWORD(2735))		// 마을 방어력 향상 메세지를 보낸다.
#define ON_RESPONSE_VILLAGE_DEFENCE							(DWORD(2736))		// 응답

#define ON_RET_DEFENSE_OK									(DWORD(0))			// 오케
#define ON_RET_DEFENSE_NOT_MONEY							(DWORD(1))			// 돈이 부족하다.
#define ON_RET_DEFENSE_NO									(DWORD(2))			// 다른 이유.

#define	ON_CHANGED_VILLAGE_ATTACK_TYPE						(DWORD(2728))		// 시간이 지나서 마을 공격의 형태가 원래대로 돌아왔다고 알려준다.
																				// 마을 주변의 플레이어들에게만 전송.
#define ON_REQUEST_INCREASE_GUILD_UNIT						(DWORD(2714))		// 공성유닛 생성.
#define ON_RESPONSE_INCREASE_GUILD_UNIT						(DWORD(2715))		// (상단의 소집권을 더한다.)

#define ON_REQUEST_DECREASE_GUILD_UNIT						(DWORD(2716))		// 공성유닛을 나눠준다.
#define ON_RESPONSE_DECREASE_GUILD_UNIT						(DWORD(2717))		// (상단의 소집권을 가져간다.)

#define ON_REQUEST_LIST_GUILD_UNIT							(DWORD(2726))		// 길드의 공성 유닛 리스트를 요청한다.
#define ON_RESPONSE_LIST_GUILD_UNIT							(DWORD(2727))		//

#define ON_REQUEST_INCREASE_GUILD_SUPPLY					(DWORD(2718))		// 상단의 보급품을 더한다.
#define ON_RESPONSE_INCREASE_GUILD_SUPPLY					(DWORD(2719))		//	

#define ON_REQUEST_DECREASE_GUILD_SUPPLY					(DWORD(2720))		// 상단의 보급품을 가져간다.
#define ON_RESPONSE_DECREASE_GUILD_SUPPLY					(DWORD(2721))

#define ON_REQUEST_LIST_GUILD_SUPPLY						(DWORD(2724))		// 상단 보급품 리스트를 얻어온다.
#define ON_RESPONSE_LIST_GUILD_SUPPLY						(DWORD(2725))		// 

#define ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER			(DWORD(2722))		// 마을 수비병을 증가 시킨다.
#define ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER		(DWORD(2723))		// 마을 수비병을 증가 시킨다.(응답).

#define ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER				(DWORD(2737))		// 마을 수비병의 수를 얻어온다.
#define ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER				(DWORD(2738))		// 응답.

#define ON_CHANGED_PLAYER_DEAD_PENALTY						(DWORD(2734))		// 공성유닛이 죽어서 생긴 패널티를 원래대로 돌린다.

#define ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER				(DWORD(2739))		// 화친제의 메세지를 보낸다.
#define ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER			(DWORD(2740))		// 응답.

#define ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT			(DWORD(2741))		// 화친제의 허락.
#define ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT			(DWORD(2742))		// 응답.
	
#define ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL			(DWORD(2743))		// 화진제의 씹는다.
#define ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL			(DWORD(2744))		// 화진제의 씹는다.

#define ON_REQUEST_GUILD_VILLAGEATTACK_RESULT				(DWORD(2745))		// 공성결과를 요청한다.(대방)
#define ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT				(DWORD(2746))		// 공성결과에 대한 응답.

#define ON_REQUEST_GUILD_CHOICE_PLAYER						(DWORD(2747))		// 공성결과 점령한 마을에 투자할 유저를 선택한다.
#define ON_RESPONSE_GUILD_CHOICE_PLAYER						(DWORD(2748))		// 응답.

#define ON_REQUEST_VILLAGE_INVESTMENT_POWER					(DWORD(2749))		// 해당 유저가 투자를 할때 해당 마을에 폭투를 할수 있는지 물어본다.
#define ON_RESPONSE_VILLAGE_INVESTMENT_POWER				(DWORD(2750))		// 응답.

#define	ON_CHANGED_WAR_END									(DWORD(2751))		// 캐릭터를 원래 캐릭터로 바꾸고 공서관련 아이템을 지운다.

#define ON_REQUEST_WAR_VILLAGE_DEFENCE						(DWORD(2752))		// 대방 캐릭터가 수성인터페이스로 들어간다
#define ON_RESPONSE_WAR_VILLAGE_DEFENCE						(DWORD(2753))		// 응답.	

#define ON_RET_GUILD_VILLAGE_OFF_LIMITS						(DWORD(2754))		// 응답 메세지중 하나.딴사람이 있어서 출입할수 없다.
																				// 나머지 응답 메세지는 EnterVillage와 같다.
//#define ON_REQUEST_WAR_VILLAGE_IN							(DWORD(2755))
//#define	ON_RESPONSE_WAR_VILLAGE_IN							(DWORD(2756))
#define ON_REQUEST_VILLAGE_DEFENCE_STOP                     (DWORD(2755))       // 수성 인터페이스에서 나간다
#define ON_RESPONSE_VILLAGE_DEFFNCE_STOP                    (DWORD(2756))       // 응답.


#define ON_REQUEST_WAR_WARP									(DWORD(2757))		// 공성전 중 필요없는 캐릭을 버린다.
#define ON_RESPONSE_WAR_WARP								(DWORD(2758))		// 응답.

//일반 캐릭터로 만들어뿐다.
#define ON_REQUEST_NORMAL_CHAR								(DWORD(2759))		// OnReqNormalChar 
#define ON_RESPONSE_NORMAL_CHAR								(DWORD(2760))		// OnResNormalChar

#define ON_REQUEST_CHANGE_WAR                               (DWORD(2761))       //공성유닛으로 변신을 요청 한다.
#define ON_RESPONSE_CHANGE_WAR                              (DWORD(2762))       //응답

#define ON_CHANGE_OK                                        (DWORD(0))          //ok
#define ON_CHANGE_NOT_GUILD                                 (DWORD(1))          //상단에 가입되어있지 않다.
#define ON_CHANGE_ENOUGH_LEVEL                              (DWORD(2))          //레벨이 부족하다. 
#define ON_CHANGE_ENOUGH_MONEY                              (DWORD(3))          //돈이 부족하다.
#define ON_CHANGE_ENOUGH_KIND                               (DWORD(4))          //해당 속성의 유닛이 없다.
#define ON_CHANGE_FAILED                                    (DWORD(5))          //일반적인 실패                                

#define ON_REQUEST_CHARGE_MP                                (DWORD(2763))       //보급수레의 MP가 회복되었다 
#define ON_RESPONSE_CHARGE_MP                               (DWORD(2764))       //응답

#define ON_RET_OK											(DWORD(0))			// 마을 수비병 증가  OK
#define ON_RET_NOT_ENOUGH_MONEY								(DWORD(1))			// 돈이 부족하다.
#define ON_RET_NOT_ENOUGH_SUPPLY							(DWORD(2))			// 보급이 부족하다.
#define ON_RET_NOT_ENOUGH_POPULATION						(DWORD(3))			// 인구수가 부족하다.
#define ON_RET_NOT_LIMIT_POPULATION							(DWORD(4))			// 마을이 가질수 있는 최소 최대 인구수를 넘어선다.
#define ON_RET_NOT											(DWORD(5))			// 그밖의 에러.


#define ON_RESPONSE_NOT_WAR                                 (DWORD(2765))       // 공성이 없어졌다.

#define ON_RESPONE_WAR_TIME                                 (DWORD(2766))       //공성시간이 된것을 알려준다.

#define ON_ASK_GIVEUP                                       (DWORD(2767))       // 대방에게 전투포기 할것인지 물어본다.                                    
#define ON_REQUEST_GIVEUP                                   (DWORD(2768))       // 대방이 전투포기를 요청한다.  
#define ON_RESPONE_GIVEUP                                  (DWORD(2769))       // 응답

#define	ON_REQUEST_ENTER_STRUCTURE							(DWORD(2800))		// 마을의 특정 건물에 들어간다.
#define	ON_RESPONSE_ENTER_STRUCTURE							(DWORD(2801))		// 마을의 특정 건물에 들어간다.(응답)
#define	ON_REQUEST_LEAVE_STRUCTURE							(DWORD(2802))		// 현재 건물에서 나간다.
#define	ON_RESPONSE_LEAVE_STRUCTURE							(DWORD(2803))		// 현재 건물에서 나간다.(응답)
//#define	ON_REQUEST_ENTER_FACTORY							(DWORD(2804))		// 마을 산업 단지의 공장에 들어간다.
//#define	ON_RESPONSE_ENTER_FACTORY							(DWORD(2805))		// 마을 산업 단지의 공장에 들어간다.(응답)
#define	ON_REQUEST_LEAVE_FACTORY							(DWORD(2806))		// 마을 산업 단지의 공장에서 나간다.
#define	ON_RESPONSE_LEAVE_FACTORY							(DWORD(2807))		// 마을 산업 단지의 공장에서 나간다.(응답)

#define ON_REQUEST_AUDIT_GUILD_INFO							(DWORD(2808))		// 상단 정보를 얻어온다.(요구)
#define ON_RESPONSE_AUDIT_GUILD_INFO						(DWORD(2809))		// 상단 정보를 얻어온다.(응답)

#define ON_RET_GUILD_OK										(DWORD(0))			// 상단 정보 얻기 성공.
#define ON_RET_GUILD_NO_CHANGED								(DWORD(1))			// 상단 정보 변화 없음.
#define ON_RET_GUILD_NO_MEMBER								(DWORD(2))			// 상단원이 아님.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 아이템 현금 판매 관련..
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define ON_RESPONSE_ITEMRECEIVED							(DWORD(2900))		// 구입한 아이템 개수를 반환 
																				// 아이템 개수( 2byte )
#define ON_REQUEST_ITEMLIST									(DWORD(2901))		// 구입한 아이템 리스트를 요청 
#define ON_RESPONSE_ITEMLIST								(DWORD(2902))		// 구입한 아이템 리스트 반환 
																				// 아이템 개수( 2byte ) + { ItemID( 2 byte ) + Quantity( 2byte ) + ... }
#define ON_REQUEST_TAKEOUTITEM								(DWORD(2903))		// 아이템을 인벤토리에 넣는다
																				// uiFollowerID( 1 ) + itemID( 2 ) + quantity( 2 )
#define ON_RESPONSE_TAKEOUTITEM								(DWORD(2904))		// 성공(0) 실패(1)


#define ON_REQUEST_SAVEITEM									(DWORD(2905))		// 아이템을 아이템 뱅크로 저장한다 
#define ON_RESPONSE_SAVEITEM								(DWORD(2906))		// 성공(0) 실패(1)


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	전투 관련,
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_RET_MARKET_OK						(DWORD(0))
#define ON_RET_MARKET_NO						(DWORD(1))
#define	ON_REQUEST_RETURNONLINE					(DWORD(3000))	// 전투를 하고 다시 온라인 필드로 돌아간다.
#define	ON_RESPONSE_RETURNONLINE				(DWORD(3001))	// 전투를 하고 다시 온라인으로 돌아갈때의 응답을 받았다.
#define	ON_RET_RETURNONLINE_FIELD				(DWORD(1))		// 필드로 돌아간다
#define	ON_RET_RETURNONLINE_VILLAGE				(DWORD(2))		// 마을로 돌아간다

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 퀘스트 관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_QUEST_SELECTNPC						(DWORD(8000))					// NPC 선택시
#define ON_RESPONSE_QUEST_SELECTNPC						(DWORD(8001))					// NPC 선택시 응답(서버에서 현재 퀘스트 상황에 따라 응답을 보내준다.)
#define ON_REQUEST_QUEST_ACCEPTQUEST					(DWORD(8002))					// NPC 퀘스트 수락
#define ON_RESPONSE_QUEST_ACCEPTQUEST					(DWORD(8003))					// NPC 퀘스트 수락 응답
#define ON_REQUEST_QUEST_CANCELQUEST					(DWORD(8004))					// NPC 퀘스트 중간에 취소
#define ON_RESPONSE_QUEST_CANCELQUEST					(DWORD(8005))					// NPC 퀘스트 중간에 취소 응답
#define ON_REQUEST_QUEST_GETPLAYQUEST					(DWORD(8006))					// NPC 퀘스트 진행중인 퀘스트 리스트 요청
#define ON_RESPONSE_QUEST_GETPLAYQUEST					(DWORD(8007))					// NPC 퀘스트 진행중인 퀘스트 리스트 요청 응답
#define ON_RESPONSE_QUEST_GETPLAYQUESTDATA				(DWORD(8008))					// NPC 퀘스트 진행중인 퀘스트 Data
#define ON_RESPONSE_QUEST_ALLCLEARQUEST					(DWORD(8009))					// NPC 퀘스트 모두 초기화
#define	ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO		(DWORD(8010))					// actdoll (2004/03/30 10:46) : 독도 퀘스트용 완료자 명단 무작위 방송

#define ON_RET_QUEST_SELECTNPC_ERROR					(DWORD(0000))					// NPC 선택시 응답(퀘스트 시작전)
#define ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST			(DWORD(0001))					// NPC 선택시 응답(퀘스트 시작전)
#define ON_RET_QUEST_SELECTNPC_STARTQUEST				(DWORD(0002))					// NPC 선택시 응답(퀘스트 시작)
#define ON_RET_QUEST_SELECTNPC_PLAYINGQUEST				(DWORD(0003))					// NPC 선택시 응답(퀘스트 진행중)
#define ON_RET_QUEST_SELECTNPC_ENDQUEST					(DWORD(0004))					// NPC 선택시 응답(퀘스트 끝)
#define ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST			(DWORD(0005))					// NPC 선택시 응답(퀘스트 끝후)
#define ON_RET_QUEST_SELECTNPC_NOTSAMENTION				(DWORD(0006))					// NPC 선택시 응답(다른 국가 일때)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 아이템의 위치를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define ON_REQUEST_MOVEITEM						(DWORD(4004))
#define	ON_RESPONSE_MOVEITEM					(DWORD(4005))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 운영자 명령 메시지
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_COMMAND								(DWORD(5000))	// 운영자 명령 

// 용병을 추가시킨다. (당나귀 추가시와 같은 경우 서버에서 일방적으로 주는 메세지)
#define ON_ADD_FOLLOWER							(DWORD(5001))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐릭터의 보너스를 사용하여 능력치를 올린다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_REQUEST_INC_ABILITY_BYBONUS			(DWORD(6000))	
#define	ON_RESPONSE_INC_ABILITY_BYBONUS			(DWORD(6001))	

#define	ON_HUNGRY								(DWORD(6002))	// 배고프다.

#define	ON_REQUEST_CHANGE_GUILD_FLAG			(DWORD(6010))	// 상단의 깃발을 변경한다.
#define	ON_RESPONSE_CHANGE_GUILD_FLAG			(DWORD(6011))	// 상단의 깃발을 변경한다.(응답)
#define	ON_RESPONSE_CHANGE_GUILD_FLAG1			(DWORD(6014))	// 상단의 깃발을 변경한다.(응답)
#define	ON_REQUEST_USED_FLAG_LIST				(DWORD(6012))	// 사용된 상단의 깃발을 요구한다.
#define	ON_RESPONSE_USED_FLAG_LIST				(DWORD(6013))	// 사용한 상단의 깃발을 요구한다.(응답)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 다른 캐릭터의 위치를 요구한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	ON_REQUEST_PI_INMINIMAP					(DWORD(6100))	// 미니맵에서 사용자의 정보를 얻어온다.
#define	ON_RESPONSE_PI_INMINIMAP				(DWORD(6101))	// 미니맵에서 사용자의 정보를 얻어온다.(응답)

#define ON_REQUEST_PERSONAL_INFO                (DWORD(6200))
#define ON_RESPONSE_PERSONAL_INFO                (DWORD(6201))

#define	ON_MYVILLAGE_ATTACKED					(DWORD(6300))	// 나의 마을이 공격당하고 있다.
#define	ON_EVENT_USERRANKINGS					(DWORD(6400))	// 사용자들의 랭킹.

#define	ON_REQUEST_PM_OPEN						(DWORD(7000))	// 노점상을 연다.
#define	ON_RESPONSE_PM_OPEN						(DWORD(7001))	// 노점상을 연다.(응답)
#define	ON_REQUEST_PM_CLOSE						(DWORD(7002))	// 노점상을 닫는다.
#define	ON_RESPONSE_PM_CLOSE					(DWORD(7003))	// 노점상을 닫는다.(응답)
#define	ON_REQUEST_PM_BUYITEM					(DWORD(7004))	// 노점상에게 아이템을 산다.
#define	ON_RESPONSE_PM_BUYITEM					(DWORD(7005))	// 노점상에게 아이템을 산다.(응답)
#define	ON_RESPONSE_PM_BUYFOLLOWER				(DWORD(7006))	// 노점상에게 용병을 산다.(응답)
#define	ON_REQUEST_PM_ITEMINFO					(DWORD(7007))	// 노점상이 판매하고 있는 아이템의 정보를 얻어온다.
#define	ON_RESPONSE_PM_ITEMINFO					(DWORD(7008))	// 노점상이 판매하고 있는 아이템의 정보를 변경한다.(응답)
#define	ON_REQUEST_PM_CHANGEITEM				(DWORD(7009))	// 노점상이 판매하고 있는 아이템의 정보를 변경한다.
#define	ON_RESPONSE_PM_CHANGEITEM				(DWORD(7010))	// 노점상이 판매하고 있는 아이템의 정보를 변경한다.
#define	ON_PM_CHANGE_BOOTH_ITEM					(DWORD(7011))	// 노점상이 판매하고 있는 아이템이 변경되었다.


#define	ON_REQUEST_TRADE							(DWORD(7500))	// 거래를 요청한다.
#define	ON_RESPONSE_TRADE							(DWORD(7501))	// 거래를 요청한다.(응답)
#define ON_RECEIVE_ASK_TRADE						(DWORD(7502))	// 해당 클라이언트에게 다른 클라이언트가 거래를 요청했다는 것을 알려준다.
#define	ON_REQUEST_ACCEPT_TRADE						(DWORD(7503))	// 거래 요청을 받은 클라이언트가 거래를 허락했다.
#define ON_RESPONSE_ACCEPT_TRADE					(DWORD(7504))	// 거래 요청을 받은 클라이언트가 거래를 허락했다.(응답)
#define	ON_REQUEST_CHANGE_TRADE_GOODS				(DWORD(7505))	// 거래 중인 품목을 수정한다.
#define	ON_RESPONSE_CHANGE_TRADE_GOODS				(DWORD(7506))	// 거래 중인 품목을 수정한다.(응답)
#define	ON_REQUEST_FINISH_CHOOSE_TRADEITEM			(DWORD(7507))	// 거래를 원하는 아이템을 모두 다 선택했다.
#define	ON_RESPONSE_FINISH_CHOOSE_TRADEITEM			(DWORD(7508))	// 거래를 원하는 아이템을 모두 다 선택했다.(응답)
//#define	ON_RESPONSE_FINISHED_CHOOSE_TRADEITEM		(DWORD(7509))	// 거래를 하고 있는 클라이언트가 거래를 원하는 아이템을 모두 다 선택했다.
#define	ON_REQUEST_DECIDE_TRADE						(DWORD(7510))	// 거래를 하는 것을 수락했다.
#define	ON_RESPONSE_DECIDE_TRADE					(DWORD(7511))	// 거래를 하는 것을 수락했다.(응답)
#define	ON_CHANGE_TRADE_GOODS						(DWORD(7512))	// 거래중인 품목을 변경한다.
#define	ON_REQUEST_CANCEL_TRADE						(DWORD(7512))	// 거래를 취소한다.
#define	ON_RESPONSE_CANCEL_TRADE					(DWORD(7513))	// 거래를 취소한다.(응답)
#define	ON_REQUEST_COMPLETE_TRADE_BOOK				(DWORD(7514))	// 장부 거래가 완료되었다.
#define	ON_RESPONSE_COMPLETE_TRADE_BOOK				(DWORD(7515))	// 장부 거래가 완료되었다.(응답)
#define	ON_COMPLETE_TRADE							(DWORD(7516))	// 거래가 완료되었다.

#define	ON_REQUEST_VILLAGE_INFO						(DWORD(7600))	// 특정 마을의 정보를 요청한다.
#define	ON_RESPONSE_VILLAGE_INFO					(DWORD(7601))	// 특정 마을의 정보를 요청한다.(응답)

#define ON_REQUEST_PARTY							(DWORD(7700))	// 파티를 요청한다.
#define ON_RESPONSE_PARTY							(DWORD(7701))	// 파티를 요청한다.(응답)

#define	ON_RECEIVE_ASK_PARTY						(DWORD(7702))   // 해당 클라이언트에게 다른 클라이언트가 파티를 요청했다는 것을 알려준다.
//#define ON_REQUEST_ASK_PARTY						ON_RECEIVE_ACK_PARTY

#define ON_REQUEST_ACCEPT_PARTY						(DWORD(7703))	// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다. 동의하느냐 마느냐.
#define ON_RESPONSE_ACCEPT_PARTY					(DWORD(7704))	// 파티 요청을 받은 클라이언트가 파티에 대한 응답을 한다.(응답) 동의 결과에 따라.
#define ON_REQUEST_LEAVE_PARTY					    (DWORD(7705))	// 클아이언트가 파티를 떠난다.(클라이언트 -> 서버)
#define ON_RESPONSE_LEAVE_PARTY						(DWORD(7706))	// 클라이언트 파티를 떠난다의 응답 메시지
//#define ON_RESPONSE_PARTY_LIST						(DWORD(7707))	// 클라이언트가 파티를 떠난다.(서버 -> 파티 참가자)
#define	ON_REQUEST_CANCEL_REQUESTING_PARTY			(DWORD(7708))	// 전에 했던 파티 요청을 취소한다.
#define	ON_RESPONSE_CANCEL_REQUESTING_PARTY			(DWORD(7709))	// 전에 했던 파티 요청을 취소한다는 메시지의 응답
#define	ON_CANCEL_REQUESTED_PARTY					(DWORD(7710))	// 파티를 요청했던 플레이어가 파티 요청을 취소했다.
#define	ON_PARTY_DELETED							(DWORD(7711))	// 파티가 없어졌다.
//#define	ON_PARTY_MEMBER_CHANGED						(DWORD(7713))	// 파티원이 변경되었다.

#define ON_RELOAD_WEAR_ITEM							(DWORD(7712))	// actdoll (2004/07/19 13:26) : = 시간제 아이템 = 아이템 시간 갱신 타이밍(BroadCast)

// 10000 - 10999	:	게임 시스템적인것(시간등)
#define	ON_CHANGED_TIME							(DWORD(10001))
#define	ON_CHANGED_MONEY						(DWORD(10002))
#define	ON_TEXT_MSG								(DWORD(10003))
#define	ON_HEARTBEAT							(DWORD(10004))
#define	ON_OFFICIAL_ANNOUNCEMENT				(DWORD(10005))		// 공지 사항
#define ON_CHANGE_WEATHER						(DWORD(10006))
#define	ON_REQUEST_RUN_PROGRAM_LIST				(DWORD(10007))
#define	ON_RESPONSE_RUN_PROGRAM_LIST			(DWORD(10008))
#define ON_MAKE_THUNDER							(DWORD(10009))		// 번개를 생성시킨다.
#define ON_FRIEND_LOGGEDINOUT					(DWORD(10010))		// 친구가 로그인 했다.

// Response.
#define	ON_RET_OK								(DWORD(0))
#define	ON_RET_NO								(DWORD(1))

//#define ON_RET_PARTY_FULL						(DWORD(1200))
//#define ON_RET_PARTY_DIFPARTY					(DWORD(1201))
//#define ON_RET_NO_PARTY_CAP						(DWORD(1202))

#define	ON_RET_GLD_JOIN_OK						(DWORD(1000))
#define	ON_RET_GLD_JOIN_NO						(DWORD(1001))
#define	ON_RET_GLD_LEAVE_OK						(DWORD(1002))
#define	ON_RET_GLD_LEAVE_OK_DELETEGLD			(DWORD(1003))		// 상단을 떠나고 길드가 삭제되었다.
#define	ON_RET_GLD_LEAVE_NO						(DWORD(1004))
#define	ON_RET_GLD_DELETEGLD					(DWORD(1005))		// 길드가 삭제되었다.
#define	ON_RET_GLD_CHANGECLASS					(DWORD(1006))		// 직급이 변경되었다.
#define	ON_RET_GLD_DISMISSAL					(DWORD(1007))		// 상단에서 해고되었다.
#define	ON_RET_GLD_ALLY							(DWORD(1008))		// 상단에 동맹을 설정했다
#define	ON_RET_GLD_ENEMY						(DWORD(1009))		// 상단에 선전포고를 했다.
#define	ON_RET_GLD_NEUTRAL						(DWORD(1010))		// 상단과 아무 관계도 아니다.
#define	ON_RET_GLD_MINE							(DWORD(1011))		// 우리의 상단이다.
#define ON_RET_GLD_WAR							(DWORD(1012))

#define	ON_RET_TEXTMSG_WHISPER					(DWORD(1050))		// 귓속말이다.
#define	ON_RET_TEXTMSG_GUILD					(DWORD(1051))		// 상단 메시지이다.
#define	ON_RET_TEXTMSG_PARTYCHAT				(DWORD(1052))		// 파티 채팅 메시지이다.

#define	ON_RET_HUR_CHANGE_HEALTH				(DWORD(2000))		// 배고픔 메시지에서 Health가 감소되었다.
#define	ON_RET_HUR_CHANGE_HP					(DWORD(2001))		// 배고픔 메시지에서 체력이 감소되었다.

#define	ON_RET_ENEMY_GUILD_VILLAGE				(DWORD(2002))		// 적 상단의 마을이다.
 
#define	ON_RET_NOT_CONNECTED					(DWORD(2100))		// 접속되어 있지 않다.
#define	ON_RET_ON_FIELD							(DWORD(2101))		// 필드 위에 있다.
#define	ON_RET_IN_VILLAGE						(DWORD(2102))		// 마을 안에 있다.
#define	ON_RET_IN_BATTLE						(DWORD(2103))		// 전투 중에 있다.

#define	ON_RET_OA_TEXT							(DWORD(3000))		// 공지사항 - 일반적인 텍스트 
#define	ON_RET_OA_VILL_ATTACKED					(DWORD(3001))		// 공지사항 - 마을이 공격을 당하고 있다.
#define	ON_RET_OA_VILL_OCCUPIED					(DWORD(3002))		// 공지사항 - 마을이 점령되었다.
#define	ON_RET_OA_MAXINVESTMENT_CHANGED			(DWORD(3003))		// 공지사항 - 최고 투자자가 변경 되었다.
#define	ON_RET_OA_GUILD_CREATED					(DWORD(3004))		// 공지사항 - 상단이 만들어졌다.
#define	ON_RET_OA_GUILD_DELETED					(DWORD(3005))		// 공지사항 - 상단이 사라졌다
#define	ON_RET_OA_PAYING_BUSINESS				(DWORD(3006))		// 공지사항 - 이익이 남는 장사의 정보
#define	ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE	(DWORD(3007))		// 공지사항 - 마을을 파괴하여 대방이 돈을 얻었다.
#define	ON_RET_OA_SMS_MSG						(DWORD(3008))		// 공지사항 - SMS Message
#define ON_RET_OA_VILL_SURRENDER                (DWORD(3009))       // 마을 함락
#define ON_RET_ON_GUILD_GIVEUP                  (DWORD(3010))       // 상단이 공성을 포기


#define	ON_RET_SEND_TRADE_BOOK					(DWORD(4000))		// 장부를 전송한다.
#define	ON_RET_CANCEL_DECIDE_TRADE				(DWORD(4001))		// 상대방 측에서 '수락'버튼을 눌렀던 것을 취소했다.

//#define	ON_RET_CHNGEMONEY_NONE					(DWORD(5000))		// 단지 현재 돈을 보내준다.
#define	ON_RET_CHNGEMONEY_PRODUCTFACILITY		(DWORD(5001))		// 생산 건물이 팔려서 돈을 받았다.
#define	ON_RET_CHNGEMONEY_FROMADMIN				(DWORD(5002))		// 운영자에게 돈을 받았다.
#define	ON_RET_CHNGEMONEY_INVESTMENT			(DWORD(5003))		// 투자 배당금을 받았다.

#define ON_REQUEST_VMERCENARY_DELETE			(DWORD(8100))		// 따라다니는 용병 삭제 요청
#define ON_RESPONSE_VMERCENARY_CHANGE			(DWORD(8101))		// 따라다니는 용병 교체 응답
#define ON_RESPONSE_VMERCENARY_CHANGEMAIN		(DWORD(8102))		// 따라다니는 용병 교체 응답(본인에게 가는 메세지)

#define ON_REQUEST_REFINE_ITEM                  (DWORD(9201))       // 아이템 제련 요청
#define ON_RESPONSE_REFINE_ITEM                 (DWORD(9202))       // 아이템 제련 응답

#define ON_NOTICE_ENCRYPTION_KEY				(DWORD(10555))		// 서버가 클라이언트에 암호키를 전송한다
//#define ON_RECEIVED_KEY							(DWORD(10556))		// 클라이언트가 서버에 암호키를 수신 했음을 알린다

#define ON_RET_NO_GLD_ADMIT_FULL				(DWORD(11000))		// 상단원이 꽉찬 상태라서 대방이 지원자를 입단 허용할 수 없다.
#define ON_RET_NO_GLD_ADMIT_MAXINVESTOR			(DWORD(11001))		// 마을을 가지고 있기 때문에 지원자를 입단 허용할 수 없다.(공성시간대에)

#define ON_RESOPNSE_REAL_TIME					(DWORD(11002))




#define ON_REQUEST_MACRO_USING_USER_INFO		(DWORD(20000))		// 매크로 사용자 정보를 보낸다.

#endif

