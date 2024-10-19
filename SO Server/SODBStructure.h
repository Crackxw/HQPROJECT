#ifndef	_SODBSTRUCTURE_H_
#define	_SODBSTRUCTURE_H_

#include "../AuthServer/_LoginDefine.h"
#include "../SCCommon/GSCDefine.h"

// 게이트 큐에서 처리 할것들 
//-----------------------------------------------------------------------------------------------------------
#define	SO_DB_INFOKIND_NONE							0			// 아무 명령도 아님
//#define	SO_DB_INFOKIND_LOGOUT					3			// 플레이어가 로그아웃 했다고 DB에 저장한다.
//#define	SO_DB_INFOKIND_SAVEPLAYERITEM			4
//#define	SO_DB_INFOKIND_PLAYERITEM				5
//#define	SO_DB_INFOKIND_UNSETCONNECTED				6
#define	SO_DB_INFOKIND_VILLAGEMARKETSELLITEM		7			
#define	SO_DB_INFOKIND_BANKINFO						8
//#define SO_DB_INFOKIND_BANKBALANCE				9
#define	SO_DB_INFOKIND_SAVEMONEY					10			// 플레이어의 돈을 DB에 저장한다.
#define	SO_DB_INFOKIND_APPENDITEM					11			// 플레이어의 인벤토리에 아이템을 추가해준다.
#define	SO_DB_INFOKIND_DELETEITEM					12			// 플레이어의 인벤토리에서 아이템을 삭제한다.
#define	SO_DB_INFOKIND_UPDATEITEM					13			// 플레이어의 인베토리에서 아이템의 수량등의 정보를 수정한다.
#define	SO_DB_INFOKIND_UPDATEVILLAGEITEM			14			// 시전에서 변경된 아이템의 수를 저장한다.
#define	SO_DB_INFOKIND_SAVEPLAYERPOS				15			// 플레이어의 현재 위치를 DB에 저장한다.
//#define	SO_DB_INFOKIND_SETWORLDTIME				16			// 게임 서버의 시간을 DB에 저장한다.
#define	SO_DB_INFOKIND_ALLFIELDITEM					17			// 필드에 떨어진 아이템들을 DB에서 얻어온다.
#define	SO_DB_INFOKIND_APPENDFIELDITEM				18			// 필드에 아이템을 추가해준다.			
#define	SO_DB_INFOKIND_DELETEFIELDITEM				19			// 필드에서 아이템을 삭제한다.
#define	SO_DB_INFOKIND_VILLAGEPOPULATION			20			// 마을의 인구와 수비병을 얻온다.
#define	SO_DB_INFOKIND_OPENSAVINGACCOUNT			21
#define	SO_DB_INFOKIND_MYBANKBOOK					22
#define	SO_DB_INFOKIND_DEPOSIT						23
#define	SO_DB_INFOKIND_DRAWING						24
//#define	SO_DB_INFOKIND_CLOSESAVINGACCOUNT		25
//#define	SO_DB_INFOKIND_SETBATTLECREDIT			26			// 전투 신용도를 DB에 저장한다.
#define	SO_DB_INFOKIND_SETTRADECREDIT				27			// 무역 신용도를 DB에 저장한다.
#define	SO_DB_INFOKIND_REGISTNOTICE					28			// 주점, 게시판의 글을 DB에 저장한다.
#define	SO_DB_INFOKIND_TOTAL_NOTICESNUM				29			// 주점, 게시판에서 총 몇개의 글이 있는지 얻어온다.
#define	SO_DB_INFOKIND_TOTAL_NOTICESHEADER			30			// 주점, 게시판에서 각 게시물의 번호등을 얻어온다.
#define	SO_DB_INFOKIND_GETNOTICESHEADER				31			// 주점, 게시판에서 각 게시물의 기본 정보를 얻어온다.
#define	SO_DB_INFOKIND_BANKCAPITAL					32
#define	SO_DB_INFOKIND_GETNOTICECONTENT				33			// 주점, 게시판에서 게시물의 본문을 얻어온다.
#define	SO_DB_INFOKIND_DELETENOTICE					34			// 주점, 게시판에서 게시물을 삭제한다.
//#define	SO_DB_INFOKIND_CREATEMONSTERACCOUNT		35
//#define	SO_DB_INFOKIND_SETMONSTERINFO			36
//#define	SO_DB_INFOKIND_SETBATTLECHARINFO		37
//#define	SO_DB_INFOKIND_CREATEBATTLE				38
#define	SO_DB_INFOKIND_COLLATERAL_1					39
#define SO_DB_INFOKIND_BANKBOOK_NUM					40
#define	SO_DB_INFOKIND_COLLATERALLIST				41
#define	SO_DB_INFOKIND_TAKECOLLATERAL				42
//#define	SO_DB_INFOKIND_FOLLOWERINFO				43			// 부하들의 정보(파라미터).
//#define	SO_DB_INFOKIND_UPDATEMONSTERINFO		44			// 전투를 종료하고 몬스터들의 정보를 얻어온다.
#define	SO_DB_INFOKIND_SAVECAPITAL					45			// 은행 돈을 저장한다.
//#define	SO_DB_INFOKIND_FINISHBATTLE				49			// Battle.
#define SO_DB_INFOKIND_AUDIT_TOTAL_1				50			// 장부에서 총 재산(전장잔고, 전장대출)
#define SO_DB_INFOKIND_AUDIT_TOTAL_2				51			// 장부에서 총 재산(랭킹, 총재산 저장)
//#define SO_DB_ACCOUNT_TOTALNUM					52			// 전체 계정수 및 캐릭터수를 얻는다.
#define	SO_DB_INFOKIND_AUDIT_BANKACCOUNT			53			// 전체 통장 계정을 얻어온다.
#define	SO_DB_INFOKIND_AUDIT_VILLAGE				54			// 장부에서 마을 리스트를 받아온다.
#define SO_DB_INFOKIND_AUDIT_VILLAGEINFO			55			// 장부에 있는 한 마을에 대한 정보를 얻어온다.
#define	SO_DB_INFOKIND_FARM_GETDATA					57
#define SO_DB_INFOKIND_FARM_GETINFO					58
#define SO_DB_INFOKIND_FARM_BUY						59
#define SO_DB_INFOKIND_FARM_BUILDSTRUCT				60
#define SO_DB_INFOKIND_FARM_EXPANDSLOT				61
#define SO_DB_INFOKIND_FARM_DESTROYSTRUCT			62
#define SO_DB_INFOKIND_FARM_SETEQUIPMENT			63
#define SO_DB_INFOKIND_CHANGE_WEAR_INFO				64			//	장비를 착용
#define SO_DB_INFOKIND_FARM_PRODUCTITEM				67
#define SO_DB_INFOKIND_FARM_WORK					68
#define SO_DB_INFOKIND_FARM_WORKINFO				69
#define SO_DB_INFOKIND_FARM_WORKEND					70
#define SO_DB_INFOKIND_STORAGE_BUILD				71
#define SO_DB_INFOKIND_STORAGE_GETDATA				72
#define SO_DB_INFOKIND_STORAGE_SETDATA				73
#define SO_DB_INFOKIND_VILLAGEDESCRIPT				74
#define	SO_DB_INFOKIND_WRITE_DESCRIPT				75
//#define SO_DB_INFOKIND_SET_PARAM					76
#define	SO_DB_INFOKIND_AUDIT_OWN					77
#define	SO_DB_INFOKIND_FARM_UPGRADESLOT				79
#define SO_DB_INFOKIND_STORAGE_DESTROY				80
#define	SO_DB_INFOKIND_FARM_LEVELUP					81
#define	SO_DB_INFOKIND_STORAGE_LEVELUP				82
#define SO_DB_INFOKIND_STORAGE_WORK					83
#define	SO_DB_INFOKIND_USERMAILADDRESS				84			// 사용자의 메일 주소를 얻어온다.
#define	SO_DB_INFOKIND_GUILD_CREATE					85			// 상단을 만든다.
#define	SO_DB_INFOKIND_GUILD_DELETE					86			// 상단을 삭제한다.
#define	SO_DB_INFOKIND_GUILD_JOIN					87			// 플레이어가 상단에 가입한다.
#define	SO_DB_INFOKIND_GUILD_LEAVE					88			// 플레이어가 상단에서 탈퇴한다.
#define	SO_DB_INFOKIND_GUILD_CHANGE_CLASS			89			// 플레이어가 상단에서 직급이 변경되었다.
#define	SO_DB_INFOKIND_GUILD_GETGUILDLIST			90			// 상단의 리스트를 얻어온다.
#define	SO_DB_INFOKIND_GUILD_GETGUILDMEMBER			91			// 상단의 멤버들을 얻어온다.
//#define	SO_DB_INFOKIND_GUILD_OWNERVILLAGENUM	92				
//#define	SO_DB_INFOKIND_GUILD_MEMBERSNUM			93
//#define	SO_DB_INFOKIND_GUILD_GETGUILDIDBYCN		94
#define	SO_DB_INFOKIND_GUILD_SETRELATION			95			// 상단의 관계를 설정한다.
#define SO_DB_INFOKIND_CHECKLOAN					96
#define	SO_DB_INFOKIND_AUDIT_SETNAME				97
#define	SO_DB_INFOKIND_VILLAGEGUILDID_SET			98			// 특정 마을의 소유 상단을 설정한다.
//#define	SO_DB_INFOKIND_SAVETRADECAPITAL			99
#define SO_DB_INFOKIND_BANKCAPITAL_CALCULATE		100
#define	SO_DB_INFOKIND_GETADDMONEY					101
//#define	SO_DB_INFOKIND_SETSATIETY				102			// 포만감을 저장한다.
#define SO_DB_INFOKIND_SETVILLAGEITEM				103
#define SO_DB_INFOKIND_ITEM_TOTALNUM				104
#define SO_DB_INFOKIND_ITEM_DBPRICE					105
#define SO_DB_INFOKIND_ITEM_APPEND					106
//#define	SO_DB_INFOKIND_VILLAGE_GUILDCLASS		107			
#define SO_DB_INFOKIND_VILLAGE_SETNATION			108
#define	SO_DB_INFOKIND_VILLAGEHNSNAME_SET			109			// 마을의 행수 이름을 지정한다.
#define	SO_DB_INFOKIND_GUILDCLASS					111			// 플레이어의 상단 직급을 얻어온다.
#define	SO_DB_INFOKIND_FARM_SELL_BYFORCE			112
#define SO_DB_INFOKIND_FARM_SETREMAINDAY			113
#define	SO_DB_INFOKIND_VILLAGEDEFENCE_SET			114			// 마을의 방어력을 설정한다.
#define	SO_DB_INFOKIND_GUILD_FLAG					115			// 상단의 깃발을 설정 및 얻어온다.
#define	SO_DB_INFOKIND_GETVILLAGECAPITAL			116
#define	SO_DB_INFOKIND_NEARESTHVCODE				117			// 플레이어가 던전에 들어갈때 만약 던전에서 죽게되면 보낼 마을의 코드를 저장한다.
#define	SO_DB_INFOKIND_MODIFYNOTICE					118			// 게시물을 수정한다.
#define	SO_DB_INFOKIND_ITEMPRODUCTSET				119
#define	SO_DB_INFOKIND_PLANT_CHANGE_PAY				120
#define	SO_DB_INFOKIND_PLANT_CANCEL_PLAN			121
#define	SO_DB_INFOKIND_GETWORLDTIME					122			// 게임 서버의 시간을 얻어온다.
#define SO_DB_INFOKIND_VILLAGE_GETNATION			125
#define	SO_DB_INFOKIND_VILLAGEGUILDID_GET			126			// 마을의 소유 상단 ID를 얻어온다.
#define	SO_DB_INFOKIND_GUILD_GETRELATION			128			// 상단의 관계를 얻어온다.
#define	SO_DB_INFOKIND_VILLAGEHNSNAME_GET			129			// 마을의 행수 이름을 얻어온다.
#define	SO_DB_INFOKIND_VILLAGEDEFENCE_GET			130			// 마을 방어력을 얻어온다.
#define	SO_DB_INFOKIND_NOTICEREADCOUNT				131			// 게시물을 읽은 횟수를 얻어온다.
#define	SO_DB_INFOKIND_BUYTICKET					132			// Ticket 사기
#define	SO_DB_INFOKIND_GETTICKET					133			// Ticket 정보 얻기
#define	SO_DB_INFOKIND_RESETTICKET					134			// Ticket 초기화
#define	SO_DB_INFOKIND_VILLAGEENTERPLAYERGET		135			// 마을에 드어온 사람수 얻기
//#define	SO_DB_INFOKIND_VILLAGEENTERPLAYERSET	136			// 마을에 드어온 사람수 지정
#define	SO_DB_INFOKIND_APPENDMERCENARY				137			// 부하 추가
#define	SO_DB_INFOKIND_SETMERCENARYPARAM			138			// 부하 정보 지정
#define	SO_DB_INFOKIND_DELETEMERCENARY				139			// 부하 삭제
#define	SO_DB_INFOKIND_VILLAGEGETCAPITAL			140			// 마을 투자액 얻기
#define	SO_DB_INFOKIND_VILLAGESETCAPITAL			141			// 마을 투자액 저장
#define	SO_DB_INFOKIND_INVESTORGETINFO				142			// 마을 투자자 정보 얻기
#define	SO_DB_INFOKIND_INVESTORSETINFO				143			// 마을 투자자 정보 저장
#define SO_DB_INFOKIND_SAVEALLPARAM					147			// 용병마다 모든 파라미터를 DB에 저장한다
#define	SO_DB_INFOKIND_UPDATEMERCENARY				148			// 부하 기본 정보 수정
#define	SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA		150			// 진행중인 퀘스트 데이터 저장
#define	SO_DB_INFOKIND_QUEST_COMPLETEQUEST			151			// 퀘스트를 완료한다.
#define	SO_DB_INFOKIND_QUEST_CANCELQUEST			152			// 퀘스트를 취소한다.
#define	SO_DB_INFOKIND_GIVEFOLLOWER					153			// 퀘스트를 취소한다.
//#define SO_DB_INFOKIND_CONCURRENTUSER				155
#define	SO_DB_INFOKIND_TRADE						156
#define	SO_DB_INFOKIND_HEALATVILLAGE				157			// Heal
#define	SO_DB_INFOKIND_INVEST_SETINVESTMENT			158			// 마을 투자자 정보 저장
#define	SO_DB_INFOKIND_INVEST_GETINVESTMENT			159			// 마을 투자자 List 정보 얻기
#define	SO_DB_INFOKIND_INVEST_DELINVESTMENT			160			// 마을 투자자 정보 초기화
#define	SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT	161			// 마을 투자자 List에서 모두 10Percent 삭제
#define	SO_DB_INFOKIND_MOVEITEM						162			// 인벤토리의 아이템을 이동 시킨다.
#define SO_DB_INFOKIND_SAVETOBANK                   163         // 전장에   돈저금하기
#define SO_DB_INFOKIND_WITHDRAWFROMBANK             164         // 전장에서 돈빼기
#define	SO_DB_KIND_BUYITEMINMARKET					165			// 시전에서 아이템 구매
#define	SO_DB_KIND_SELLITEMINMARKET					166			// 시전에서 아이템 구매
#define	SO_DB_INFOKIND_DROPITEM						167			// 필드에서 아이템을 버린다.
#define	SO_DB_INFOKIND_PICKUPITEM					168			// 필드에서 아이템을 줍는다.
#define	SO_DB_INFOKIND_CREATEITEM_BYADMIN			169			// 운영자가 필드에 아이템을 만든다.
#define	SO_DB_INFOKIND_CHANGEJOBMERCENARY			170			// 부하 전직
#define	SO_DB_INFOKIND_BUYMERCENARY					171			// 부하 사기
#define SO_DB_INFOKIND_PLANT_CANCEL_STORAGE         172			// 창고취소
#define SO_DB_INFOKIND_FARM_DESTROY_FACTORY           173
#define SO_DB_INFOKIND_FARM_DESTROY_STORAGE           174
#define SO_DB_INFOKIND_FARM_STORAGE_INPUT_ITEM        175
#define SO_DB_INFOKIND_FARM_STORAGE_OUTPUT_ITEM       176
#define SO_DB_INFOKIND_STORAGE_CHANGE_PAY             177
#define SO_DB_INFOKIND_STORAGE_DELETEITEM             178

#define	SO_DB_INFOKIND_GETITEMCOUNT_ITEMBANK		  179		// 아이템 뱅크에서 아이템 개수 얻어오기
#define SO_DB_INFOKIND_ITEMLIST_ITEMBANK			  180		// 아이템 뱅크에서 아이템 리스트 얻어오기
#define SO_DB_INFOKIND_PUTITEM_FROMITEMBANK			  181		// 아이템 뱅크에서 인벤토리로 아이템 가져오기
#define SO_DB_INFOKIND_SAVEITEM_ITEMBANK			  193		// 아이템 뱅크에 아이템을 저장한다

#define SO_DB_INFOKIND_QUEST_PAY_MONEY				  182		// Quest 결과로 돈을 얻음
#define SO_DB_INFOKIND_QUEST_PAY_ITEM				  183		// Quest 결과로 Item을 얻음
#define SO_DB_INFOKIND_QUEST_PAY_CREDIT				  184		// Quest 결과로 Credit을 얻음
#define SO_DB_INFOKIND_QUEST_PAY_EXP				  185		// Quest 결과로 EXP을 얻음
#define SO_DB_INFOKIND_QUEST_REQUEST_ITEM			  186		// Quest 결과로 자신의 Item을 삭제
#define SO_DB_INFOKIND_QUEST_REQUEST_MONEY			  187		// Quest 결과로 자신의 돈을 삭제


#define SO_DB_INFOKIND_GET_ITEM_AUCTION				  188		// 아이템 경매 정보를 하나 얻어 온다 
#define SO_DB_INFOKIND_ADD_ITEM_AUCTION				  189		// 아이템 경매를 추가한다 
#define SO_DB_INFOKIND_SUCCESSFULBID_ITEM_AUCTION     190		// 아이템 경매를 낙찰한다 
#define SO_DB_INFOKIND_FAILBID_ITEM_AUCTION			  191		// 아이템 경매를 유찰한다 
#define SO_DB_INFOKIND_DEL_ITEM_AUCTION				  192		// 아이템 경매를 삭제한다 
//													  193       위에서 쓰고 있음

#define	SO_DB_INFOKIND_FARM_LEVELUP_BYNPC		      194
#define	SO_DB_INFOKIND_STORAGE_LEVELUP_BYNPC		  195
#define SO_DB_INFOKIND_FARM_WORKEND_BYNPC			  196
#define SO_DB_INFOKIND_FARM_WORK_BYNPC				  197
#define SO_DB_INFOKIND_STORAGE_WORK_BYNPC			  198

#define SO_DB_INFOKIND_INITMERCENARYPARAM			  199

#define SO_DB_INFOKIND_BOOTHTRADE                     200

#define SO_DB_INFOKIND_VMERCENARYSHOW				  201
#define SO_DB_INFOKIND_VMERCENARYDELETE				  202

#define SO_DB_INFOKIND_SMITH_REFINE_SUCCESS			  203
#define SO_DB_INFOKIND_SMITH_REFINE_NO_CHANGES		  204
#define SO_DB_INFOKIND_SMITH_REFINE_FAILED			  205

#define SO_DB_INFOKIND_CHANGESEX        			  206
#define SO_DB_INFOKIND_CHANGENATION        			  207

#define SO_DB_INFOKIND_UPDURABILITYOFVILLAGE		  208

#define	SO_DB_INFOKIND_REWARDTODAIFORDESTROYVILLAGE	  209

#define SO_DB_INFOKIND_CONSUMEITEM                    210

#define SO_DB_INFOKIND_GETOUTINVERTMENT               211

#define	SO_DB_INFOKIND_GUILD_KICK					  212			// 플레이어가 상단에서 쫓겨난다.
#define	SO_DB_INFOKIND_CONSUMECREDITITEM			  213			// 신용도업 아이템
#define	SO_DB_INFOKIND_GETMAXGUILDID     			  214           // 여태까지 생성된 길드ID의 최대값

#define	SO_DB_INFOKIND_USEPORTAL         			  215           // 포탈사용
#define SO_DB_INFOKIND_GIVEMONEYBYADMIN               216           // 운영자 명령으로 돈생성
#define SO_DB_INFOKIND_DIEATBATTLELOSEMONEY           217           // 전투중 본캐릭터가 죽어서 돈을 잃는다.
#define SO_DB_INFOKIND_DIEATBATTLELOSEITEM            218           // 전투중 죽어서 아이템을 잃는다.
#define SO_DB_INFOKIND_GETITEMATBATTLE                219           // 전투에서 아이템을 얻었다.
#define SO_DB_INFOKIND_ROBITEMATBATTLE                220           // 전투에서 아이템을 뺐었다.
#define SO_DB_INFOKIND_INCREASEMAXGENERAL			  221			// 고용 가능한 장수수 증가
#define SO_DB_INFOKIND_CONSUMEDONKEYITEM			  222			// 당나귀 아이템 사용

#define SO_LOGIN_DB_NEWCHAR								230
#define SO_LOGIN_DB_DELCHAR								231
#define SO_LOGIN_DB_CHARSINFO							232


#define	SO_DB_INFOKIND_BUYMONSTER						233			// 몬스터 고용
#define SO_DB_INFOKIND_DELETEMONSTER					244			// 몬스터 해고
#define SO_DB_INFOKIND_BOOTHFOLLOWERTRADE				245			// 개인 상점에서 용병 거래	

#define SO_DB_INFOKIND_UPGRADEWEAPONSUCCESS				250			// 마석을 사용하여 무기 업그레이드에 성공했다.
#define SO_DB_INFOKIND_UPGRADEWEAPONFAILURE				251			// 마석을 사용하여 무기 업그레이드에 실패했다.
#define SO_DB_INFOKIND_UPGRADEWEAPONBROKEN				252			// 마석을 사용하여 무기 업그레이드중 무기가 파괴되었다.

#define SO_DB_INFOKIND_AUDIT_FRIEND_SET_DENIAL			253			// 장부에서 친구를 수신거부 처리한다.
#define SO_DB_INFOKIND_AUDIT_FRIEND_ADD_ONE				254			// 장부에서 친구를 추가한다.
#define SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_SOME			255			// 장부에서 친구를 삭제한다.
#define SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_ALL			256			// 장부에서 친구 전체를 삭제한다.

#define	SO_DB_INFOKIND_EVENT_EXPO						257			// 엑스포 관련 이벤트에 대한 정보를 얻어온다.
#define	SO_DB_INFOKIND_EVENT_EXPOBATTLERESULT			258			// 엑스포 관련 이벤트에서 전투 결과를 DB에 보내준다.
#define	SO_DB_INFOKIND_EVENT_EXPORANKING				259			// 엑스포 관련 이벤트에서 랭킹을 보내준다.

#define	SO_DB_INFO_KIND_GET_GM_LIST						260			// 게임 운영자들의 정보를 얻어온다.

#define SO_DB_INFO_DOKDO_QUEST_COMPLETED_LIST			261			// 독도 퀘스트를 완료한 사용자를 얻어온다.
#define SO_DB_INFO_DOKDO_QUEST_COMPLETED_SAVE			262			// 독도 퀘스트의 정보를 저장한다.

#define SO_DB_INFO_MACRO_USER_INFO_SAVE					263			// 매크로 사용자 정보를 DB에 저장한다.
#define SO_DB_INFO_GETEVENTITEM							264			// 이벤트 아이템을 얻어온다.
#define SO_DB_INFOKIND_QUEST_PAY_EVENTITEM				265			// 이벤트 아이템에 당첨 되었을 경우 DB에 저장.

#define SO_DB_INFOKIND_IMPERIUM_USE						266			// 소집권을 사용해서 일반유닛->공성유닛으로 변한다.
#define SO_DB_INFOKIND_CHANGECHARACTER					267			// 필드에서 맞아 원래 캐릭터로 돌아간다.

#define SO_DB_INFOKIND_INCREASE_GUILD_UNIT				268			// 상단유닛을 증가시킨다.대방이 상단유닛을 만든다.
#define SO_DB_INFOKIND_DECREASE_GUILD_UNIT				269			// 상단유닛을 감소시킨다.대방이 상단유닛을 가져간다.

#define	SO_DB_INFOKIND_INCREASE_SUPPLY					270			// 상단 보급품수를 증가시킨다.
#define SO_DB_INFOKIND_DECREASE_SUPPLY					271			// 상단 보급품수를 감소시킨다.

#define SO_DB_INFOKIND_INCREASE_DEFENCESOLDIER			272			// 마을 수비병수 증가.

#define SO_DB_INFOKIND_WAR_LOGIN						273			// 공성유닛이 로그인을 한다.
#define SO_DB_INFOKIND_GET_GUILD_ITEM					274			// 공성전과 관련된 상단의 물품을 가져간다.

#define SO_DB_INFOKIND_INCREASE_POPULATION				275			// 마을 인구수,최대 인구수를 저장한다.
#define SO_DB_INFOKIND_DECREASE_POPULATION				276			// 감소한 마을의 인구수를 저장한다.

#define SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_OFFER		277			// 화친메세지를 보낸다.
#define SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_ACCEPT	278			// 화친메세지를 허락한다.
#define SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_CANCEL	279

#define SO_DB_INFOKIND_SET_VILLAGE_DAMAGE				280			// 마을이 받은 데미지를 DB에 저장.
#define SO_DB_INFOKIND_GET_VILLAGE_DAMAGE				281			// 서버가 죽었다가 다시 뜨면 

#define SO_DB_INFOKIND_SET_GUILD_RESULT					282			// 공성 승리시. 결과를 저장한다.(캐릭터 이름제외)
#define SO_DB_INFOKIND_GET_GUILD_RESULT					283			// 서버가 죽었다 다시 뜨면 공성결과 정보를 얻어온다.

#define SO_DB_INFOKIND_SET_DELETE_INVESTMENT			284			// 마을에 투자하면 폭투권한이 사라진다.

#define SO_DB_INFOKIND_SET_GUILD_WAR_END				285			// 10시에 공성이 끝났다.
#define SO_DB_INFOKIND_SET_POWER_INVESTMENT_END			286			// 12시에 폭투권한이 끝났다.

#define SO_DB_INFOKIND_DELETE_CHARACTOR_WAR				287

#define SO_DB_INFOKIND_VILLAGE_INFO						288			// 서버 시작시 마을정보를 저장.
#define SO_DB_INFOKIND_CHARACTER_LIST					289			// 서버 시작시 캐릭터정보를 저장.
#define SO_DB_INFOKIND_VILLAGE_ITEM						290			// 서버 시작시 마을 아이템을 저장.

#define SO_DB_INFOKIND_WAR_PROFIT						291			// 공성 수익금을 저장.
#define SO_DB_INFOKIND_DECREASE_SOLDIER					292			// 마을 수비병 수를 줄인다.

#define SO_DB_INFOKIND_VILLAGEOWNER_DELETE				293			// 


#define SO_DB_INFOKIND_SET_WARGIVEUP                    294         //
#define SO_DB_INFOKIND_GET_WARGIVEUP                    295         //

//-----------------------------------------------------------------------------------------------------------




// 서브 큐에서 처리 할것들 
//-----------------------------------------------------------------------------------------------------------
#define	SO_DB_INFOKIND_LOGOUTINFO					300			// 플레이어의 로그아웃 정보를 DB에 저장한다.
#define	SO_DB_INFOKIND_SETWORLDTIME					301			// 게임 서버의 시간을 DB에 저장한다.
#define	SO_DB_INFOKIND_SETSATIETY					302			// 포만감을 저장한다.
#define SO_DB_INFOKIND_CONCURRENTUSER				303			// 현재 동시 접속자수를 저장 한다
#define	SO_DB_INFOKIND_VILLAGEENTERPLAYERSET		304			// 마을에 들어온 사람수 지정
#define SO_DB_INFOKIND_LOGOUTSAVEALLPARAM			305			// 로그아웃시에 모든 파라미터 저장 
#define SO_DB_INFOKIND_LOGOUTSTART					306			// 로그아웃을 시작 했다는 FLAG 를 설정 한다 
#define SO_DB_INFOKIND_WARINFO						307			// 공성전 정보를 저장한다.
//-----------------------------------------------------------------------------------------------------------


// 아이템 
struct DBIKPutItemFromItemBank
{
	SI32			DBAccount;
	UI08			uiSlot;
	UI08			uiFollowerID;
	UI08			uiPosition;
	UI16			siItemID;
	UI16			siQuantity;
};

struct DBIKSaveItemIntoItemBank
{
	SI32			DBAccount;
	UI08			uiSlot;
	UI08			uiFollowerID;
	UI16			siItemID;
	UI16			siQuantity;
};

// 투자
struct stVillageInvestorData
{
	SI32						m_siInvestorDBAccount;
	UI08						m_uiInvestorSlot;

	SI08						m_InvestorType;						// 투자자 Type(0 : 사람, 1 : 길드)
	UI08						m_InvestorID[21];					// 투자자 ID
	float						m_Percent;							// 점유율
	__int64						m_Money;							// 투자 금액
	int							m_UpdateTime;						// 투자 시간
};


struct	DBIKSetName
{
	SI32		DBAccount;
	UI08		uiSlot;
	UI08		uiFollowerID;
	CHAR		cName[ON_ID_LENGTH+1];
};

struct	DBIKSetParam
{
	SI32		DBAccount;
	UI08		uiSlot;
	UI08		uiFollowerID;
	UI08		Option;
	SI32		siValue;
};

struct	DBIKGetVillageBuildInfo
{
	UI16		uiVillageCode;
	UI08		uiPlantID;
};

struct	DBIKSetStorageItem
{
	UI16		uiVillageCode;
	UI08		uiPlantID;
	UI08		uiPartID;
	UI08		uiSlot;
	UI08		uiItemIndex;		//	(0~9 : 창고에 10개의 아이템만 가능하다.)
	UI16		uiItemCode;			//	아이템 코드
	UI16		uiItemQnt;			//	아이템 개수
};

struct	DBIKFarmSetItem
{
	UI16		uiVillageCode;	
	UI08		uiFarmID;
	UI08		uiFactoryID;
	UI08		uiItemIndex;		//	(0~9 : 창고에 10개의 아이템만 가능하다.)
	UI16		uiItemCode;			//	아이템 코드
	UI16		uiItemQnt;			//	아이템 개수
};



struct	DBIKStorageInputItem
{
	SI32		DBAccount;
	UI08		uiSlot;

	
	UI16		uiVillageCode;	
	UI08		uiFarmID;
	UI08		uiFactoryID;

	UI16		uiItemCode;			//	아이템 코드
	UI16		uiItemQnt;			//	아이템 개수
	

	UI08        uiMercenary;
	UI08        uiMercenaryItemPosition;     // 용병의 아이템 위치

	UI08		uiStoreItemPosition;		//	(0~9 : 창고에 10개의 아이템만 가능하다.)
	
	
};

struct	DBIKStorageOutputItem
{
	SI32		DBAccount;
	UI08		uiSlot;
	
	UI16		uiVillageCode;	
	UI08		uiFarmID;
	UI08		uiFactoryID;

	UI16		uiItemCode;			//	아이템 코드
	UI16		uiItemQnt;			//	아이템 개수
	

	UI08        uiMercenary;
	UI08        uiMercenaryItemPosition;     // 용병의 아이템 위치

	UI08		uiStoreItemPosition;		//	(0~9 : 창고에 10개의 아이템만 가능하다.)
	
};




struct  DBIKWearItem
{
	SI32						siDBAccount;
	UI08						uiSlot;
	UI08						uiFollowerID;	
	UI08						uiWearLoc;			//	착용한 위치 (0~7)
	UI16						uiItemID;
	SI16						siItemPosInInventory;
	UI16						uiQuantity;			// 인벤토리 개수
	SI16						siLimitTime;
	SI32						siStartTime;
	SI32						siEndTime;
};

struct	DBIKSetUpgradeSlot
{
	SI32			siDBAccount;
	UI08			uiSlot;	
	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08			uiFactoryID;
	MONEY			moPay; // 임금
	MONEY           moTax; // 세금
    MONEY           moCapital; // 수익
	UI32			Labor; // 노동량
	
};

struct	DBIKChangeFarmPay
{	
	SI32			siDBAccount;
	UI08			uiSlot;	
	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08			uiFactoryID;
	MONEY			moPay;
	MONEY           moDifference;
};

struct  DBIKSetFarmProduct
{

	SI32			siDBAccount;
	UI08			uiSlot;	

	UI16			uiVillageCode;
	UI08			uiFarmID;
	UI08			uiFactoryID;

	UI16			uiItemID;
	UI32			Quantity;

	MONEY			moPay;
	UI32			Labor;

	MONEY           moTax;	
	MONEY           moProfit;
	
};


struct	DBIKFarmEquipData
{
	UI16			uiVillageCode;
	UI08			uiPlantID;
	UI08			uiFarmID;
	UI08			uiSlotID;
	UI08			uiEquipID;
	UI08			uiLevel;
};

struct	DBIKFarmSlotData
{
	UI16			uiVilllageCode;
	UI08			uiPlantID;
	UI08			uiFarmID;
	UI08			uiSlotID;
};

struct	DBIKFarmData
{
	UI16			uiVillageCode;
	UI08			uiPlantID;
	UI08			uiFarmID;
};

struct	DBIKFarmBuildStructInfo
{
	SI32			siAccount;
	UI08			uiSlot;	

	UI16			uiVillageCode;
	UI08			uiFarmID;
	UI08			uiFactoryID;
	
	CHAR			Code[4];
	MONEY			moPay;
	MONEY           moTax;
	MONEY           moProfit;

	UI32            uiWork;                // 작업량
	
};


struct	DBIKBuyFarmInfo
{
	SI32			siAccount;
	UI08			uiSlot;	
	
	UI16			uiVillageCode;	
	UI08			uiFarmID;				//	농장 번호 (0~9)
	MONEY           moMoney;


};

struct	DBIKBuyFarmByForceInfo
{
	SI32			siAccount;
	UI08			uiSlot;	

	UI16			uiVillageCode;	
	UI08			uiFarmID;				//	농장 번호 (0~9)
	//BOOL			bSendMsgToPlayer;		// 플레이어게에 생산 시설이 팔렸다는 것에 대해 메시지를 보내는가? (메시지를 보냈을 경우에는 다음 로그인시 
	
		
	MONEY			mnPrice;
											//																	생산시설 판매금 받지 않도록 해야 한다.)
	SI32			siWorkDay;
};			

struct	DBIKFieldItemInfo
{
	SI32			siAccount;
	UI08			uiSlot;	
	UI16			uiMapID;
	UI16			uiQuantity;
	SI16			siX;
	SI16			siY;
	UI32			uiID;	
};

struct  DBIKConsumeItem
{
	SI32			siDBAccount;
	UI08			uiSlot;

	UI08			uiFollowerID;
	UI16			uiItemID;

	
	


};

struct	DBIKPlayerItemInfo
{
	SI32			siDBAccount;
	UI08			uiSlot;
	UI08			uiFollowerID;
	
	UI16			uiItemID;
	UI16			uiItemPosInInventory;

	UI16			uiNewQuantity;
	UI16			uiNewPosInInventory;	
};

struct	DBIKVillageItemInfo
{
	UI16			uiVillageCode;
	SI16			siID;
	UI16			uiQuantity;
};


struct DBIKTotalAccount
{
	UI32			uiTotalAccount;
	UI32			uiTotalCharac;
};

struct	DBIKAuditPropertyInfo
{
	MONEY			uiSaveTotal;
	MONEY			siLoanTotal;
};

struct	DBIKAuditVillageList
{
	UI32			uiVillageCount;								// 개수
	UI16			uiVillageCode[ON_MAX_AUDIT_VILLAGE_NUM];
};

struct	DBIKAuditFriendSetDenial
{
	SI32			siDBAccount;
	UI08			uiSlot;
	CHAR			szFriendCharID[ON_ID_LENGTH+1];
	BOOL			bDenial;
};

struct	DBIKAuditFriendAddOne
{
	SI32			siDBAccount;
	UI08			uiSlot;
	CHAR			szFriendCharID[ON_ID_LENGTH+1];
};

struct	DBIKAuditFriendRemoveSome
{
	SI32			siDBAccount;
	UI08			uiSlot;
	CHAR			szFriendCharID[ON_ID_LENGTH+1];
};

struct	DBIKAuditFriendRemoveAll
{
	SI32			siDBAccount;
	UI08			uiSlot;
};

struct	DBIKOpenSavingAccountInfo
{
	UI16			uiVillageCode;	
	UI32			uiCharacterDBAccount;
	UI08			uiCharacterSlot;
	UI16			uiAccountKind;
	SI16			siInterest;
	MONEY			siFirstDepositMoney;
};

struct	DBIKCollateralList
{
	SI32			siCollateralNum;

	UI08 uiFollowerID[MAX_COLLATERAL_NUM];
	UI16 uiITemCode[MAX_COLLATERAL_NUM];
	UI16 uiNum[MAX_COLLATERAL_NUM];
	SI16 siInvenLoc[MAX_COLLATERAL_NUM];
};





struct	DBIKRegistryNoticeInfo
{
	UI16			uiVillageCode;
	CHAR			szID[ON_ID_LENGTH+1];
	CHAR			*pszTitle;
	CHAR			*pszNotice;
	CHAR			szDate[9];
};

struct	DBIKModifyNoticeInfo
{
	UI16			uiVillageCode;
	UI16			uiNoticeIndex;
	CHAR			*pszTitle;
	CHAR			*pszNotice;
	CHAR			szDate[9];
};

struct	DBIKGetNoticesInfo
{
	UI16			uiVillageCode;
	UI32			uiTotalNoticesNum;
};

struct	DBIKGetNoticesHeaderInfoIn
{
	UI16			uiVillageCode;
	UI16			*puiNoticesIndexArray;
	SI32			siTopIndex;
};

struct	DBIKGetNoticeContentIn
{
	UI16				uiVillageCode;
	UI16				uiNoticeID;
};

struct	DBIKDeleteNotice
{
	UI16				uiVillageCode;
	UI16				uiNoticeIndex;
	CHAR				szWroteID[ON_ID_LENGTH + 1];
};

struct	DBIKSatiety
{
	SI32		DBAccount;
	UI08		uiSlot;
	SI32		siSatiety;
};

struct	DBIKGuildInfo
{
	UI32		uiGuildID;
	SI32		siClassInGuildID;
};

struct	DBIKGuildRelation
{
	UI32		uiGuildID1;
	UI32		uiGuildID2;	
	SI32		siRelation;
	SI32		siTime;
};

struct DBIKGuildFrieldlyMessageOffer
{
	UI32		uiGuildID1;
	UI32		uiGuildID2;
	char		szMessage[81];
};

struct DBIKGuildFriendlyMessageAccept
{
	UI32		uiGuildID1;
	UI32		uiGuildID2;
};

struct DBIKGuildFriendlyMessageCancel
{
	UI32		uiGuildID1;
	UI32		uiGuildID2;
	char		szMessage[81];
};


struct	DBIKGuildClassInVillage
{
	UI16		uiVillageCode;
	UI32		uiGuildID;
	CHAR		szID[ON_ID_LENGTH + 1];
	SI32		siClassInGuildID;
	SI32		siParam;		
};

struct	DBIKNearestHVCode
{
	SI32		DBAccount;
	UI08		uiSlot;
	UI16		uiVillageCode;
};

//-------------------------------------------------------
//	월드 관련
//-------------------------------------------------------
struct DBIKWorldTime
{
	SI32		Year;								// 년도
	SI32		Month;								// 월
	SI32		Day;								// 일
	SI32		Hour;								// 시
};

//----------------------------------------------------
// 플레이어 정보 관련...
//----------------------------------------------------
struct	DBIKPlayerDBInfo
{
	SI32		DBAccount;
	UI08		uiSlot;
};

struct	DBIKPlayerData
{
	SI32		DBAccount;
	UI08		uiSlot;
	SI32		Data;
};

struct	DBIKSaveMoney
{
	SI32		DBAccount;
	UI08		uiSlot;
	MONEY		Money;
};

struct	DBIKPlayerPos
{
	SI32		DBAccount;
	UI08		uiSlot;
	SI16		siX;
	SI16		siY;
	UI16		MapID;
};

struct	DBIKPlayerLogOut
{
	SI32		DBAccount;
	UI08		uiSlot;
	SI16		siX;
	SI16		siY;
	UI16		MapID;
	MONEY		Money;
	SI08		VMercenary;
};

struct	DBIKPlayerCredit
{
	SI32		DBAccount;
	UI08		uiSlot;
	SI32		Credit;
	SI32		DayCredit;
};

//----------------------------------------------
//	용병 정보 관련...
//----------------------------------------------
struct	DBIKFollowerParam
{
	SI32		DBAccount;
	UI08		uiSlot;		
	UI08		uiFollowerSlot;
	SI32		Kind;
	SI32		Exp;
	SI32		Level;
	SI32		Str;
	SI32		Dex;
	SI32		Vit;
	SI32		Int;
	SI32		Life;
	SI32		Mana;
	SI32		AC;
	SI32		Bonus;
	SI32		DmResist;
	SI32		MgResist;
	SI32		EquipMin;
	SI32		EquipMax;
};

//----------------------------------------------
// 필드 정보 관련
//----------------------------------------------
struct	DBIKItemNum
{
	UI16		ItemID;
	SI32		Num;
};

struct	DBIKItem
{
	SI32		ItemID;
	SI32		Price;
	CHAR		Name[32];
	SI16		siUseDay;
};

//----------------------------------------------
//	마을 정보 관련
//----------------------------------------------
struct DBIKVillageInfo
{
	char szName[20];
	SI16  siX; 
	SI16  siY; 
	SI32  siPopulation; 
	UI16  uiViillageCode;
	char  szNation; 
};

struct	DBIKVillageProfit
{
	UI16		VillageCode;
	SI16		Year;
	MONEY		Profit;
	SI32		Option;
};

struct	DBIKVillageNation
{
	UI16		VillageCode;
	CHAR		Code;
};

struct	DBIKVillageDurability
{
	UI16		VillageCode;
	VDEF		Durability;
};

struct	DBIKDecreaseDefenceSoldier
{
	UI16		VillageCode;
	SI32		siDefenceSoldier;
};

struct	DBIKVillageMaster
{
	UI16		VillageCode;
	CHAR		Name[ON_ID_LENGTH+1];
};

struct	DBIKVillageDescript
{
	UI16		VillageCode;
	SI32		Length;
	// 뒤에는 Length 만큼 안내문(CHAR)이 온다
};

struct	DBIKVillageItem
{
	UI16		VillageCode;
	SI08		Slot;
	UI16		uiItemQuantity;
	SI16		ItemID;
};

struct	DBIKVillageCollateralInfo
{
	SI32			siAccount;
	UI08			uiSlot;
	UI16			uiVillageCode;
};

//----------------------------------------------
// 상단 정보 관련..
//----------------------------------------------
struct	DBIKGuildFlag
{

	SI32		GuildID;
	SI32		Flag;

	SI32        siAccount;
	UI08        uiSlot;

	MONEY       moMoney;


	
};

struct	DBIKGuildChangeClassInfo
{
	UI32					uiGuildID;

	SI32                    siDBAccount;
	UI08                    uiSlot;
	
	UI08					uiChangeClass;
};

struct	DBIKGuildMemberInfo
{
	CHAR					szID[ON_ID_LENGTH + 1];
	SI32					siClass;
	MONEY					Money;
	SI32					siRanking;
	SI32					siTradeCreditRanking;				// 무역 신용도 순위
	UI32					uiTradeCredit;						// 무역 신용도
	SI32					siLevel;							// Level
};

struct	DBIKGuildCreateInfo
{
	CHAR					szGuildName[ON_GUILDNAME_LENGTH + 1];

	SI32                    siAccount;
	UI08                    uiSlot;
		
	UI16					uiVillageCode;
	UI32                    uiGuildID;

	MONEY                   moMoney;
};

struct	DBIKGuildJoinInfo
{
	UI32					uiGuildID;

	SI32                    siAccount;
	UI08                    uiSlot;
	
	
	UI08					uiClass;
};

struct	DBIKGuildLeaveInfo
{
	UI32					uiGuildID;

	SI32                    siAccount;
	UI08                    uiSlot;

	UI16                    uiClass;
	
	
};

struct DBIKGuildReward
{
	UI32	uiGuildID;
	SI32	siAccount;
	UI08	uiSlot;
	MONEY	mnMoney;
	SI32	siIncreaseSupply;	
};

struct	DBIKGuildOwnerVillageInfo
{
	UI16					uiVillageCode;	
	UI32					uiGuildID;
};

struct	DBIKGuildOnwerVillageNum
{
	UI32					uiGuildID;
	UI16					uiOwnerVillageNum;
};

struct	DBIKGuildMemeberNum
{
	UI32					uiGuildID;
	UI32					m_uiGuildMemberNum;
};

struct	DBIKNoticeReadCount
{
	UI16					uiVillageCode;
	UI16					uiNoticeIndex;
	DWORD					dwReadCount;
};

struct	DBIKBuyTicket
{
	UI16		uiVillageCode;

	SI32		DBAccount;
	UI08		uiSlot;

	SI32		siTicketKind;
	MONEY		siMoney;

	MONEY		siVillageProfit;
};

struct	DBIKGetTicket
{
	SI32		DBAccount;
	UI08		uiSlot;
};

struct	DBIKGetTicketInfo
{
	SI32		siTicketKind;
};

struct	DBIKResetTicket
{
	SI32		DBAccount;
	UI08		uiSlot;
};

struct	DBIKVillageEnterPlauerGet
{
	UI16		uiVillageCode;
};

struct	DBIKVillageEnterPlauerGetInfo
{
	UI32		uiPlayerNum;
};

struct	DBIKVillageEnterPlauerSet
{
	UI16		uiVillageCode;
	UI32		uiPlayerNum;
};

struct	DBIKAppendMercenary
{
	SI32		DBAccount;
	UI08		uiSlot;

	SI32		siMercenarySlot;
	BYTE		szName[ON_ID_LENGTH + 1];
	SI32		siMercenaryKind;
};

struct	DBIKUpdateMercenary
{
	SI32		DBAccount;
	UI08		uiSlot;

	SI32		siMercenarySlot;
	BYTE		szName[ON_ID_LENGTH + 1];
	SI32		siMercenaryKind;
};

struct	DBIKSetMercenaryParam
{
	SI32		DBAccount;
	UI08		uiSlot;	
	SI32		siMercenarySlot;

	SI32		siKind;
	SI32		siCurExp;
	SI32		siLevel;
	SI32		siSTR;
	SI32		siDEX;
	SI32		siVIT;
	SI32		siINT;
	SI32		siLife;
	SI32		siMana;
	SI32		siAC;
	SI32		siBonus;
	SI32		siDamageResist;
	SI32		siMagicResist;
	SI32		siEquipMinDamage;
	SI32		siEquipMaxDamage;
};

struct	DBIKDeleteMercenary
{
	SI32		DBAccount;
	UI08		uiSlot;

	UI08		siDeleteMercenarySlot;
	UI16		siVillageCode;
	MONEY		siMoney;
	MONEY		siEmployProfit;
};

struct	DBIKDeleteMonster
{
	SI32		DBAccount;
	UI08		uiSlot;

	UI08		siDeleteMercenarySlot;
	UI16		siVillageCode;
	MONEY		siMoney;
	MONEY		siEmployProfit;
};

struct	DBIKVillageGetCapital
{
	UI16		uiVillageCode;
};

struct	DBIKVillageSetCapital
{
	UI16		uiVillageCode;
	MONEY		moVillageCapital;
};

struct	DBIKInvestorGetInfo
{
	UI16		uiVillageCode;
};

struct	DBIKInvestorGetInfo_Result
{
	BYTE		szRealBestInvestorID[21];
	BYTE		uiRealBestInvestorType;
	SI32		uiRealBestInvestorUpdateTime;

	BYTE		szNowBestInvestorID[21];
	BYTE		uiNowBestInvestorType;
	SI32		uiNowBestInvestorUpdateTime;
};

struct	DBIKInvestorSetInfo
{
	UI16		uiVillageCode;

	BYTE		szRealBestInvestorID[21];
	BYTE		uiRealBestInvestorType;
	SI32		uiRealBestInvestorUpdateTime;

	BYTE		szNowBestInvestorID[21];
	BYTE		uiNowBestInvestorType;
	SI32		uiNowBestInvestorUpdateTime;
};

struct DBIKQuestGetQuestData
{
	SI32			DBAccount;
	UI08			uiSlot;

	SPlayerQuest*	lpSPlayerQuest;	
};

/*struct DBIKQuestEventItem
{
	SI32		DBAccount;
	UI08		uiSlot;

	SI16		siQuestCode;
	SI16		siEventItemID;
};*/

struct DBIKQuestSetPlayQuestData
{
	SI32			DBAccount;
	UI08			uiSlot;

	SI08			QuestIndex;
	SPlayingQuest	QuestData;
};

struct DBIKQuestCompleteQuest
{
	SI32		DBAccount;
	UI08		uiSlot;

	SI08		QuestIndex;
};

struct DBIKQuestCancelQuest
{
	SI32		DBAccount;
	UI08		uiSlot;

	SI08		QuestIndex;
};

struct	DBIKGiveFollower
{
	SI32		siFromDBAccount;
	SI32		siToDBAccount;
	UI16		uiFromFollowerID;
	UI16		uiToFollowerID;
	UI08		uiFromSlot;
	UI08		uiToSlot;
};

struct	DBIKHealAtVillage
{
	SI32		DBAccount;
	UI08		uiSlot;

	MONEY		siMoney;
	MONEY		siVillageCapital;
	UI16		uiVillageCode;
};

struct	DBIKInvestSetInvestment
{
	UI16		uiVillageCode;
	SI32		siInvestorDBAccount;
	UI08		uiInvestorSlot;
	BYTE		szInvestorID[21];
	BYTE		uiInvestorType;
	MONEY		moInvestmentMoney;
	MONEY		moInvestmentSaveMoney;
	SI32		uiInvestorUpdateTime;
};

struct	DBIKInvestGetInvestment
{
	UI16		uiVillageCode;
};

struct	DBIKInvestGetInvestment_Result
{
	SI32						siInvestmentCount;
	stVillageInvestorData*		lpstVillageInvestorData;
};

struct	DBIKInvestDelInvestment
{
	UI16		uiVillageCode;

	SI32		siInvestorDBAccount;
	UI08		uiInvestorSlot;
};

struct	DBIKInvestDelAll10PInvestment
{
	UI16		uiVillageCode;
	SI32		siInvestorDBAccount;
	UI08		uiInvestorSlot;
	SI32        siPercent;
};

struct	DBIKSaveToBank
{
	SI32		DBAccount;
	UI08		uiSlot;

	UI16        uiVillageCode;
	MONEY       moSaveMoney;
	
};

struct	DBIKWithdrawFromBank
{
	SI32		DBAccount;
	UI08		uiSlot;

	UI16        uiVillageCode;
	MONEY       moDrawMoney;
	
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKPlayerInfo
{
	SI32			siDBAccount;
	UI08			uiSlot;
	UI16			uiKind;
	CHAR			cName[ON_ID_LENGTH+1];
	SI16			siX, siY;
	UI16			uiMapID;
	UI32			uiGuildID;
	SI32			siClassInGuildID;
	MONEY			siMoney;	
	UI32			uiBattleCredit;
	UI32			uiTradeCredit;
	SI32			siDailyCredit;
	IOnlineFollower	*pIFollower;
	SI32			siSatiety;
	UI16			uiNearestHVCode;

	SOWorld*		m_lpstWorld;
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKVillageBankBookInfo
{
	UI16			uiVillageCode;
	SOPlayer		*pPlayer;
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct DBIKPopertyRanking
{
	SOPlayer		*pPlayer;
	MONEY			siProperty;
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKAuditVillageInfo
{
	SOPlayer		*pOwner;

	UI16			uiVillageCode;	
	UI32			RecodeItemNum;
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKRetOpenSavingAccount
{
	OnAccountNumber	*pAccountNumber;
	SI32			siErrorRet;
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKBattleMonsterInfo
{	
	SI32					siMonsterDBAccount;
	UI16					uiFollowerKind[ON_MAX_FOLLOWER_NUM];
	_ImportantParameta		ImpotantParameter[ON_MAX_FOLLOWER_NUM];
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKVillageCollateral
{
	UI16				uiVillageCode;
	SOPlayer			*pPlayer;
	MONEY				siMoney;					//빌린 액수
	UI16				uiMonth;					//만기(달)
	DBIKCollateralList	*pCollateralList;
};


class cltIGuild;
//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKGuildGetMemeberListInfo
{
	cltIGuild				*pIGuild;			// In
	UI32					uiGuildID;			// In
	SI32					siClass;			// In
	UI32					uiMemberNum;		// Out
	DBIKGuildMemberInfo		*pMembers;			// Out
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKBankInfo
{	
	UI16			uiVillageCode;
	DBIKPlayerInfo	PlayerInfo;

	SI16			siInterest;
	MONEY			siBankCapital;
	MONEY			siTradeCapital;
};	



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
 struct	DBIKAuditOwnList
{
	SI32			OwnNum;				//개수
	OnAuditOwnInfo	AuditOwn[ON_MAX_AUDIT_OWN];
};



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKBankBookList
{
	SI32			siBankBookNum;
	OnAccountNumber	AccountNumber[ON_MAX_BANKBOOK_NUM];
	MONEY			siMoney[ON_MAX_BANKBOOK_NUM];
};

//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKChangeJobMercenary
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI08			uiMercenarySlot;
	SI32			siChangeKind;
	UI16			siBonus;
	UI16			uiVillageCode;
};

//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
struct	DBIKBuyMercenary
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI08			uiMercenarySlot;
	SI32			siKind;
	UI16			uiVillageCode;
	MONEY			siMoney;
	MONEY			siEmployProfit;
};

struct	DBIKBuyMonster
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI08			uiMercenarySlot;
	SI32			siKind;
	UI16			uiVillageCode;
	MONEY			siMoney;
	MONEY			siEmployProfit;
	UI16			uiItemID;
	SI16			siItemQuantity;
};

struct DBIKFarmCancelPlan
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08			uiFactoryID;
	MONEY			moPay;
	
};

struct DBIKDestroyStorage
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08			uiFactoryID;
	

};

struct DBIKDestroyFactory
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08			uiFactoryID;
	
};

struct DBIKReclamation
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI16			uiVillageCode;	
	UI08			uiFarmID;
	MONEY           moPay;
	MONEY           moCapital;
	
};

struct DBIKFarmWorkInfo
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08            uiFactoryID;

	SI32            Labor;
	MONEY           moPay;
	
	
};

struct DBIKFarmWorkEndInfo
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI16			uiVillageCode;	
	UI08			uiFarmID;
	UI08            uiFactoryID;

	SI32            siStorageID;
	SI32            siStoragePos;

	UI16		    uiItemCode;			//	아이템 코드
	UI16		    uiItemQnt;			//	아이템 개수

	UI32            Labor;
	MONEY           moPay;
	
	
};

struct DBIKWarGiveUp
{
	UI32      uiGuildID;
	UI16      uiVillageCode;
	MONEY     mnMoney;
	SI16      siFlag;
};

//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 캐릭터의 기본 정보 구조체 
//--------------------------------------------------------------------------------------------
typedef struct __CharacterBaseInfo
{
	DWORD				account;

	char				name[LOGIN::NAME_MAX_LENGTH];
	WORD				kind;
	DWORD				userGrade;
	WORD				satiety;

	__int64				money;
	__int64             saving;
	__int64				investProfit;
	__int64				additionProfit;

	WORD				x_pos;
	WORD				y_pos;
	WORD				mapID;
    WORD				nearVillage;

	DWORD				credit_trade;
	DWORD				credit_daily;
	
	DWORD				guildID;
	DWORD				guildGrade;

	int					ticketKind;

	char				VMercenary;
	BYTE				maxGeneral;
}CharacterBaseInfo;

typedef struct __CharacterWarInfo
{
	WORD				PrevKind;
	int					Hp;
	BYTE				bCaptain;
	int					Delay;
//	int					Supply;
	int                 Mp;
}CharacterWarInfo;


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 진행중인 퀘스트 정보 
//--------------------------------------------------------------------------------------------
typedef struct __ProgressQuestInfo
{	
	WORD		  questCode;
	DWORD		  questObject1;
	DWORD		  questObject2;

}ProgressQuestInfo;



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 한 용병의 상세 정보 
//--------------------------------------------------------------------------------------------
typedef struct __MercenaryInfo 
{
	WORD		slot;
	char		name[LOGIN::NAME_MAX_LENGTH];
	WORD		kind;

	DWORD		Exp;
	DWORD		Level;
	DWORD		Str;
	DWORD		Dex;
	DWORD		Vit;
	DWORD		Int;
	DWORD		Life;
	DWORD		Mana;
	DWORD		AC;
	DWORD		Bonus;
	DWORD		DamageRegist;
	DWORD		MagicRegist;
	DWORD		EquipMinDamage;
	DWORD		EquipMaxDamage;

}MercenaryInfo;



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 인벤토리내의 아이템 하나의 정보 
//--------------------------------------------------------------------------------------------
typedef struct __ItemInfo
{
	WORD		   itemID;
	WORD		   posInInventory;
	WORD		   quantity;
	UI08		   FollowerSlotID;	
}ItemInfo;



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 장착한 아이템 하나의 정보 
//--------------------------------------------------------------------------------------------
typedef struct __EquipItemInfo
{
	WORD		   id[WEAR_ITEM_NUM];
	SI32		   startTime[WEAR_ITEM_NUM];
	SI32		   endTime[WEAR_ITEM_NUM];
}EquipItemInfo;

struct	DBIKQuestEventItemInfo
{
	SI32	siQuestEventID;
	SI32	siCurItemCount;
	SI32	siPreItemCount;
	SI32	siDelivery;
};

struct DBIKQuestEventItemInfoMgr
{
	SI16					siQuestID;
	DBIKQuestEventItemInfo	stQuestEventItemInfo[20];
};

struct	DBIKQuestPayMoney
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	WORD			uiQuestCode;
	MONEY			siMoney;
};

struct	DBIKQuestPayItem
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI08			uiMercenarySlot;

	WORD			uiQuestCode;
	UI08			uiPosition;
	UI16			siItemID;
	UI16			siQuantity;
};

struct DBQuestCompletedCount
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	WORD			uiQuestCode;
	int				nQuestCompletedCount;
};

struct	DBIKQuestPayCredit
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	WORD			uiQuestCode;
	SI32			siCredit;
};

struct	DBIKQuestPayEXP
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	WORD			uiQuestCode;
	SI32			siEXP;
};

struct DBIKQuestPayEventItem
{
	SI32		siDBAccount;
	UI08		uiDBSlot;

	UI08		uiMercenarySlot;
	UI08		uiPosition;

	WORD		uiQuestCode;
	WORD		uiEventItemID;
	WORD		uiDelivery;
	WORD		uiQuantity;
	WORD		uiItemID;
	char		szdate[32];

};

struct	DBIKQuestRequest_Item
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI08			uiMercenarySlot;

	WORD			uiQuestCode;
	UI08			uiPosition;
	UI16			siItemID;
	UI16			siQuantity;
};

struct	DBIKQuestRequest_Money
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	WORD			uiQuestCode;
	MONEY			siMoney;
};



struct	DBIKInitMercenaryParamByItem
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI16            uiMercenarySlot;
	UI16            uiItemID;


	SI32            siBonus;
	
};

struct	DBIKVMercenaryShow
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI16            uiMercenarySlot;
	UI16            uiItemID;
};

struct	DBIKVMercenaryHide
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
};

struct  DBIKBoothFollowerTrade
{
	SI32			siFromDBAccount;
	UI32			uiFromDBSlot;
	SI32			siFromMercenarySlot;
	MONEY           moFromMoney;

	SI32            siToDBAccount;
	UI32            uiToDBSlot;
	MONEY           moToMoney;	
	SI32			siToMercenarySlot;

	SI32			siQuantity;
};

struct	DBIKBoothTrade
{
	SI32			siFromDBAccount;
	UI08			uiFromDBSlot;
	UI08            uiFromMercenarySlot;
	MONEY           moFromMoney;

	SI32            siToDBAccount;
	UI08            uiToDBSlot;
	UI08            uiToMercenarySlot;
	UI08            uiToItemSlot;
	MONEY           moToMoney;
	
	UI16            uiItemID;
	UI16            uiQuantity;
	
};

struct	DBIKRefineItem
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI08            uiMercenarySlot;
	UI16            uiItemSlot;
	UI16            uiOldItemID;
	UI16            uiNewItemID;
	MONEY           moMoney;

	
};

struct	DBIKChangeSex
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI16            uiNewCharactorCode;
	UI16            uiItemID;

};

struct	DBIKChangeNation
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI16            uiNewCharactorCode;
	UI16            uiNewCharactorNation;
	UI16            uiItemID;

};

struct	DBIKImperumUse
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI16			uiNewKind;		// 새롭게 변하는 Kind(일반유닛->공성유닛)
//	UI16			uiItemID;		// 소집권을 사용하면서 사용한 ItemID
//	UI16			uiQuantity;		// 소집권을 사용하면서 사용한 Item갯수.
	SI16			siHp;			// 공성유닛의 체력.
	SI16            siMp;
	BOOL			bCaptain;		// 대장으로 변하는가 일반 유닛으로 변하는가
//	MONEY			mnMoney;		//소모되는 돈.
//	SI16			siSupply;		// 보급 충전게이지.
};


struct	DBIKChangeCharacter
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	SI32			siDeadTime;
};

struct DBIKMakeGuildUnit
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			//길드 아이디.
	UI16			uiItemID;
	SI16			siQuantity;			//증가시키는 갯수.
//	SI32			siSupply;			//소모되는 상단의 보급량.
	MONEY			mnMoney;			//소모되는 돈.
	UI16			uiVillageCode;		//마을 코드.
	UI32			uiPopulation;		//감소되는 인구수.
};

struct DBIKDecreaseGuildUnit
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			// 길드 아이디.
	UI16			uiItemID;			// 아이템 아이디.
	SI16			siQuantity;			// 감소 시키는 갯수.
//	UI08			uiMercenarySlot;	// 용병 아이디.
//	SI16			siInventoryPos;		// 인벤토리 위치.
};

struct DBIKMakeGuildSupply
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			// 길드 아이디.
	SI32			siQuantity;			// 증가시키는 보급품의 양.
	MONEY			mnMoney;			// 소모되는 돈.
};

struct DBIKDecreaseGuildSupply
{
	SI32			siDBAccount;
	UI08			uiDBSlot;			
	UI32			uiGuildID;			// 길드 아이디.
	SI16			siQuantity;			// 증가시키는 보급품의 양.
};

struct DBIKIncreaseSoldierDefence
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			// 길드 아이디.
	SI16			siVillageCode;		// villageCode
	SI32			siDefenceSoldier;	// 증가되는 수비병 수
	SI32			siSupply;			// 감소하는 Supply
	MONEY			mnMoney;			// 감소하는 돈.	
	UI32			uiPopulation;		//감소되는 인구수.
};	


struct DBIKGetVillageInfo
{
	UI32			uiPopulation;		// 인구
	UI32			uiMaxPopulation;    // 최대 마을 인구.
	SI32			siDefenceSoldier;	// 수비병수.
};

struct DBIKSetVillageInfo
{
	UI16			uiVillageCode;		// villageCode
	UI32			uiPopulation;		// 변화된 마을 인구수.
	UI32			uiMaxPopulation;	// 변화된 마을 최대 인구수.
};

struct DBIKDecreasePopulation
{
	UI16			uiVillageCode;		// uiVillageCode
	UI32			uiPopulation;		// 변화된 마을 인구수.
};

struct DBIKVillageDamage
{
	UI32			uiGuildID;
	UI16			siVillageCode;
	SI32			siDamage;
};

struct DBIKGuildResult
{
	UI32			uiGuildID;
	SI16			siVillageCode;
	SI16			siPercent;
	char			szCharName[11];
};

struct DBIKGuildInvestVillage
{
	UI32			uiGuildID;
	UI16			uiVillageCode;
};


struct DBIKGuildFallVillage
{
	UI16           uiVillageCode;   //마을 코드
	UI32           uiGuildID;       //상단 코드
	MONEY		   mnMoney;          //돈
};

struct	DBIKConsumeCreditItem
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI08            uiFollowerID;
	UI32            uiCreditUp;
	UI16            uiItemID;
};

struct	DBIKIncreaseMaxGeneral
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI08			uiFollowerID;
	UI16            uiItemID;
};


struct	DBIKConsumeDonkeyItem
{
	SI32			DBAccount;
	UI08			uiSlot;

	UI08			uiFollowerID;
	UI16			uiItemID;
	UI08			uiDonkeySlot;
	SI32			siKind;
};


struct	DBIKUpgradeWeaponSuccess
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI08            uiFollowerID;
	UI32            uiUpgradeItemID;
	UI16            uiItemID;
};

struct	DBIKUpgradeWeaponFailure
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI08            uiFollowerID;
	UI16            uiItemID;
};

struct	DBIKUpgradeWeaponBroken
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI08            uiFollowerID;
	UI16            uiItemID;
};


struct	DBIKUpDurabilityOfVillage
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	UI16			uiVillageCode;		// 방어력 올릴 마을
	SI32			siUpDurability;		// 올릴 방어력
	SI32			siRate1;			// 비율의 분자
	SI32			siRate2;			// 비율의 분모
};

struct  DBIKGetOutInvestment
{
	UI16			uiVillageCode;		// 방어력 올릴 마을

	SI32			siDBAccount;
	UI08			uiDBSlot;

	MONEY			GetOutInvestment;
	MONEY			Tex;
};

struct  DBIKUsePortal
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	SI32            siPortalNum;
	MONEY           moMoney;
	
};

struct  DBIKGiveMoneyByAdmin
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	MONEY           moMoney;
	
};

struct  DBIKDieAtBattleLoseMoney
{
	SI32			siDBAccount;
	UI08			uiDBSlot;

	MONEY           moMoney;
	
};

struct  DBIKDieAtBattleLoseItem
{
	SI32			siDBAccount;

	UI08			uiDBSlot;
	UI08            uiMercenaryID;
	UI16            uiItemID;
	
};

struct  DBIKGetItemAtBattle
{
	SI32			siDBAccount;

	UI08			uiDBSlot;
	UI08            uiMercenaryID;
	UI16            uiItemID;
	UI16            uiItemSlot;

	UI16			uiVictimCharKind;	
};

struct  DBIKRobItemAtBattle
{
	SI32			siToDBAccount;
	UI08			uiToDBSlot;
	UI08            uiToMercenaryID;
	UI16            uiToItemSlot;

	SI32			siFromDBAccount;
	UI08			uiFromDBSlot;
	UI08            uiFromMercenaryID;
	

	UI16            uiItemID;
	

	
};

struct DBIKEvent_ExpoBattleResult	
{
	SI32			siWinDBAccount;
	SI32			siLoseDBAccount;
	SI32			siScore;
	UI08			uiWinDBSlot;
	UI08			uiLoseDBSlot;	
};

struct	DBIKEvent_ExpoRanking
{
	SI32			siDBAccount;
	SI32			siRanking;	
};

struct DBMacroUserInfo
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
};

struct DBIKDelteWarCharactor
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
};

struct DBIKCharacterList
{
	UI16			uiID;
	char			szChar[20];
	SI16			siLv;
	UI16			uiExp;
	SI16			siNation;
	SI16			siStr;
	SI16			siDex;
	SI16			siVit;
	SI16			siInt;
	SI16			siAc;
	SI16			siDamageResist;
	SI16			siMagicResist;
	SI16			siMinEquip;
	SI16			siMaxEquip;
	SI16			siMoveSpeed;
	SI16			siLife;
	SI16			siMana;
	SI32			siCost;
	SI16			siBonus;
};

//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 로그인시 필요한 필수 정보 
//--------------------------------------------------------------------------------------------
struct LoginInfo
{
	CharacterBaseInfo			charBaseInfo;			
	MercenaryInfo				mercenaryInfos[ON_MAX_FOLLOWER_NUM];
	ItemInfo					mercenaryItemInfos[ON_MAX_FOLLOWER_NUM][ON_MYDATA_MAX_ITEM_NUM];
	EquipItemInfo				mercenaryEquipItemInfos[ON_MAX_FOLLOWER_NUM];
//	EquipItemInfo				TempMercenaryEquipItemInfos[ON_MAX_FOLLOWER_NUM];

	WORD						completeQuests[ON_QUSETDATA_MAXCOMLETEDQUEST];
	ProgressQuestInfo			progressQuests[ON_QUSETDATA_MAXPLAYINGQUEST];

	strFriendInfo				friendInfos[MAX_FRIEND_NUM];
	CharacterWarInfo			charWarInfo;
};


//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 아이템 경매 추가를 DB에 설정 할때 쓰이는 구조체
//--------------------------------------------------------------------------------------------
struct sDB_AddItemAuction
{
	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];
	BYTE		auctioneerCharslot;
	DWORD		auctioneerDBAccount;

	DWORD		minPrice;
	DWORD		maxPrice;
	BYTE		numFail;

	BYTE		articleKind;

	WORD		itemID;
	WORD		itemQuantity;
	BYTE		hasItemFollowerSlot;

	DWORD		feePrice;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 아이템 경매 낙찰을 DB에 설정 할때 쓰이는 구조체
//--------------------------------------------------------------------------------------------
struct sDB_SuccessfulBidItemAuction
{
	DWORD		auctioneerDBAccount;
	BYTE		auctioneerCharSlot;
	DWORD		bidderDBAccount;
	BYTE		bidderCharSlot;
	DWORD		biddingPrice;

	BYTE		articleKind;

	WORD		itemID;
	WORD		itemQuantity;
	WORD		posInInventory;
	BYTE		recvItemFollowerSlot;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 아이템 경매 유찰을 DB에 설정 할때 쓰이는 구조체
//--------------------------------------------------------------------------------------------
struct sDB_FailBidItemAuction
{
	DWORD		auctioneerDBAccount;
	BYTE		auctioneerCharSlot;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 아이템 경매 삭제를 DB에 설정 할때 쓰이는 구조체
//--------------------------------------------------------------------------------------------
struct sDB_DelItemAuction
{
	DWORD		auctioneerDBAccount;
	BYTE		auctioneerCharSlot;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 아이템 경매 정보를 DB에서 얻어올때 쓰이는 구조체 
//--------------------------------------------------------------------------------------------
struct sDB_GetItemAuction
{
	char		strAuctioneer[LOGIN::NAME_MAX_LENGTH];
	BYTE		auctioneerCharSlot;
	DWORD		auctioneerDBAccount;

	DWORD		minPrice;
	DWORD		maxPrice;
	BYTE		numFail;

	WORD		itemID;
	WORD		itemQuantity;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 
//--------------------------------------------------------------------------------------------
struct sDB_LogoutStart
{
	DWORD		dbAccount;
	char		id			[LOGIN::ID_MAX_LENGTH];
	char		password	[LOGIN::PW_MAX_LENGTH];
	char		ip			[16];
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : 
//--------------------------------------------------------------------------------------------
struct sDB_LogoutInfo
{	
	DWORD		dbAccount;
	BYTE		slot;
	WORD		xpos;
	WORD		ypos;
	WORD		mapID;
	char		IP[ 16 ];
};

struct sDB_LogoutWarInfo
{
	DWORD				dbAccount;
	BYTE				slot;
	WORD				PrevKind;
	int					Hp;
	BYTE				bCaptain;
	int					siDelay;
	int					MP;
};

//

struct sDB_NewChar
{
	DWORD		dbAccount;
	BYTE		slot;	
	
	char		name [LOGIN::NAME_MAX_LENGTH];	// 캐릭터의 이름  
	WORD		kind;							// 캐릭터의 종류 
	
	WORD		Str;							// 힘		 
	WORD		Dex;							// 민첩성			
	WORD		Vit;							// 체력				
	WORD		Int;							// 지력

};



struct sDB_DelChar
{
	DWORD		dbAccount;
	BYTE		slot;
};

struct sDB_CharsInfo
{
	DWORD		dbAccount;
};

enum	{	SO_DB_SETPARAM_CUREXP, SO_DB_SETPARAM_STR, SO_DB_SETPARAM_DEX, SO_DB_SETPARAM_VIT, SO_DB_SETPARAM_INT,
			SO_DB_SETPARAM_LIFE, SO_DB_SETPARAM_MANA, SO_DB_SETPARAM_AC, SO_DB_SETPARAM_DAMAGEREGIST, SO_DB_SETPARAM_MAGICREGIST,
			SO_DB_SETPARAM_EQUIPMINDAMAGE, SO_DB_SETPARAM_EQUIPMAXDAMAGE, SO_DB_SETPARAM_BONUS, SO_DB_SETPARAM_LEVEL = 14};


#endif	//_SODBSTRUCTURE_H_