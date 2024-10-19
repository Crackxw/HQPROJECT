#ifndef	_SODBSTRUCTURE_H_
#define	_SODBSTRUCTURE_H_

#include "../AuthServer/_LoginDefine.h"
#include "../SCCommon/GSCDefine.h"

// ����Ʈ ť���� ó�� �Ұ͵� 
//-----------------------------------------------------------------------------------------------------------
#define	SO_DB_INFOKIND_NONE							0			// �ƹ� ��ɵ� �ƴ�
//#define	SO_DB_INFOKIND_LOGOUT					3			// �÷��̾ �α׾ƿ� �ߴٰ� DB�� �����Ѵ�.
//#define	SO_DB_INFOKIND_SAVEPLAYERITEM			4
//#define	SO_DB_INFOKIND_PLAYERITEM				5
//#define	SO_DB_INFOKIND_UNSETCONNECTED				6
#define	SO_DB_INFOKIND_VILLAGEMARKETSELLITEM		7			
#define	SO_DB_INFOKIND_BANKINFO						8
//#define SO_DB_INFOKIND_BANKBALANCE				9
#define	SO_DB_INFOKIND_SAVEMONEY					10			// �÷��̾��� ���� DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_APPENDITEM					11			// �÷��̾��� �κ��丮�� �������� �߰����ش�.
#define	SO_DB_INFOKIND_DELETEITEM					12			// �÷��̾��� �κ��丮���� �������� �����Ѵ�.
#define	SO_DB_INFOKIND_UPDATEITEM					13			// �÷��̾��� �κ��丮���� �������� �������� ������ �����Ѵ�.
#define	SO_DB_INFOKIND_UPDATEVILLAGEITEM			14			// �������� ����� �������� ���� �����Ѵ�.
#define	SO_DB_INFOKIND_SAVEPLAYERPOS				15			// �÷��̾��� ���� ��ġ�� DB�� �����Ѵ�.
//#define	SO_DB_INFOKIND_SETWORLDTIME				16			// ���� ������ �ð��� DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_ALLFIELDITEM					17			// �ʵ忡 ������ �����۵��� DB���� ���´�.
#define	SO_DB_INFOKIND_APPENDFIELDITEM				18			// �ʵ忡 �������� �߰����ش�.			
#define	SO_DB_INFOKIND_DELETEFIELDITEM				19			// �ʵ忡�� �������� �����Ѵ�.
#define	SO_DB_INFOKIND_VILLAGEPOPULATION			20			// ������ �α��� ������ ��´�.
#define	SO_DB_INFOKIND_OPENSAVINGACCOUNT			21
#define	SO_DB_INFOKIND_MYBANKBOOK					22
#define	SO_DB_INFOKIND_DEPOSIT						23
#define	SO_DB_INFOKIND_DRAWING						24
//#define	SO_DB_INFOKIND_CLOSESAVINGACCOUNT		25
//#define	SO_DB_INFOKIND_SETBATTLECREDIT			26			// ���� �ſ뵵�� DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_SETTRADECREDIT				27			// ���� �ſ뵵�� DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_REGISTNOTICE					28			// ����, �Խ����� ���� DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_TOTAL_NOTICESNUM				29			// ����, �Խ��ǿ��� �� ��� ���� �ִ��� ���´�.
#define	SO_DB_INFOKIND_TOTAL_NOTICESHEADER			30			// ����, �Խ��ǿ��� �� �Խù��� ��ȣ���� ���´�.
#define	SO_DB_INFOKIND_GETNOTICESHEADER				31			// ����, �Խ��ǿ��� �� �Խù��� �⺻ ������ ���´�.
#define	SO_DB_INFOKIND_BANKCAPITAL					32
#define	SO_DB_INFOKIND_GETNOTICECONTENT				33			// ����, �Խ��ǿ��� �Խù��� ������ ���´�.
#define	SO_DB_INFOKIND_DELETENOTICE					34			// ����, �Խ��ǿ��� �Խù��� �����Ѵ�.
//#define	SO_DB_INFOKIND_CREATEMONSTERACCOUNT		35
//#define	SO_DB_INFOKIND_SETMONSTERINFO			36
//#define	SO_DB_INFOKIND_SETBATTLECHARINFO		37
//#define	SO_DB_INFOKIND_CREATEBATTLE				38
#define	SO_DB_INFOKIND_COLLATERAL_1					39
#define SO_DB_INFOKIND_BANKBOOK_NUM					40
#define	SO_DB_INFOKIND_COLLATERALLIST				41
#define	SO_DB_INFOKIND_TAKECOLLATERAL				42
//#define	SO_DB_INFOKIND_FOLLOWERINFO				43			// ���ϵ��� ����(�Ķ����).
//#define	SO_DB_INFOKIND_UPDATEMONSTERINFO		44			// ������ �����ϰ� ���͵��� ������ ���´�.
#define	SO_DB_INFOKIND_SAVECAPITAL					45			// ���� ���� �����Ѵ�.
//#define	SO_DB_INFOKIND_FINISHBATTLE				49			// Battle.
#define SO_DB_INFOKIND_AUDIT_TOTAL_1				50			// ��ο��� �� ���(�����ܰ�, �������)
#define SO_DB_INFOKIND_AUDIT_TOTAL_2				51			// ��ο��� �� ���(��ŷ, ����� ����)
//#define SO_DB_ACCOUNT_TOTALNUM					52			// ��ü ������ �� ĳ���ͼ��� ��´�.
#define	SO_DB_INFOKIND_AUDIT_BANKACCOUNT			53			// ��ü ���� ������ ���´�.
#define	SO_DB_INFOKIND_AUDIT_VILLAGE				54			// ��ο��� ���� ����Ʈ�� �޾ƿ´�.
#define SO_DB_INFOKIND_AUDIT_VILLAGEINFO			55			// ��ο� �ִ� �� ������ ���� ������ ���´�.
#define	SO_DB_INFOKIND_FARM_GETDATA					57
#define SO_DB_INFOKIND_FARM_GETINFO					58
#define SO_DB_INFOKIND_FARM_BUY						59
#define SO_DB_INFOKIND_FARM_BUILDSTRUCT				60
#define SO_DB_INFOKIND_FARM_EXPANDSLOT				61
#define SO_DB_INFOKIND_FARM_DESTROYSTRUCT			62
#define SO_DB_INFOKIND_FARM_SETEQUIPMENT			63
#define SO_DB_INFOKIND_CHANGE_WEAR_INFO				64			//	��� ����
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
#define	SO_DB_INFOKIND_USERMAILADDRESS				84			// ������� ���� �ּҸ� ���´�.
#define	SO_DB_INFOKIND_GUILD_CREATE					85			// ����� �����.
#define	SO_DB_INFOKIND_GUILD_DELETE					86			// ����� �����Ѵ�.
#define	SO_DB_INFOKIND_GUILD_JOIN					87			// �÷��̾ ��ܿ� �����Ѵ�.
#define	SO_DB_INFOKIND_GUILD_LEAVE					88			// �÷��̾ ��ܿ��� Ż���Ѵ�.
#define	SO_DB_INFOKIND_GUILD_CHANGE_CLASS			89			// �÷��̾ ��ܿ��� ������ ����Ǿ���.
#define	SO_DB_INFOKIND_GUILD_GETGUILDLIST			90			// ����� ����Ʈ�� ���´�.
#define	SO_DB_INFOKIND_GUILD_GETGUILDMEMBER			91			// ����� ������� ���´�.
//#define	SO_DB_INFOKIND_GUILD_OWNERVILLAGENUM	92				
//#define	SO_DB_INFOKIND_GUILD_MEMBERSNUM			93
//#define	SO_DB_INFOKIND_GUILD_GETGUILDIDBYCN		94
#define	SO_DB_INFOKIND_GUILD_SETRELATION			95			// ����� ���踦 �����Ѵ�.
#define SO_DB_INFOKIND_CHECKLOAN					96
#define	SO_DB_INFOKIND_AUDIT_SETNAME				97
#define	SO_DB_INFOKIND_VILLAGEGUILDID_SET			98			// Ư�� ������ ���� ����� �����Ѵ�.
//#define	SO_DB_INFOKIND_SAVETRADECAPITAL			99
#define SO_DB_INFOKIND_BANKCAPITAL_CALCULATE		100
#define	SO_DB_INFOKIND_GETADDMONEY					101
//#define	SO_DB_INFOKIND_SETSATIETY				102			// �������� �����Ѵ�.
#define SO_DB_INFOKIND_SETVILLAGEITEM				103
#define SO_DB_INFOKIND_ITEM_TOTALNUM				104
#define SO_DB_INFOKIND_ITEM_DBPRICE					105
#define SO_DB_INFOKIND_ITEM_APPEND					106
//#define	SO_DB_INFOKIND_VILLAGE_GUILDCLASS		107			
#define SO_DB_INFOKIND_VILLAGE_SETNATION			108
#define	SO_DB_INFOKIND_VILLAGEHNSNAME_SET			109			// ������ ��� �̸��� �����Ѵ�.
#define	SO_DB_INFOKIND_GUILDCLASS					111			// �÷��̾��� ��� ������ ���´�.
#define	SO_DB_INFOKIND_FARM_SELL_BYFORCE			112
#define SO_DB_INFOKIND_FARM_SETREMAINDAY			113
#define	SO_DB_INFOKIND_VILLAGEDEFENCE_SET			114			// ������ ������ �����Ѵ�.
#define	SO_DB_INFOKIND_GUILD_FLAG					115			// ����� ����� ���� �� ���´�.
#define	SO_DB_INFOKIND_GETVILLAGECAPITAL			116
#define	SO_DB_INFOKIND_NEARESTHVCODE				117			// �÷��̾ ������ ���� ���� �������� �װԵǸ� ���� ������ �ڵ带 �����Ѵ�.
#define	SO_DB_INFOKIND_MODIFYNOTICE					118			// �Խù��� �����Ѵ�.
#define	SO_DB_INFOKIND_ITEMPRODUCTSET				119
#define	SO_DB_INFOKIND_PLANT_CHANGE_PAY				120
#define	SO_DB_INFOKIND_PLANT_CANCEL_PLAN			121
#define	SO_DB_INFOKIND_GETWORLDTIME					122			// ���� ������ �ð��� ���´�.
#define SO_DB_INFOKIND_VILLAGE_GETNATION			125
#define	SO_DB_INFOKIND_VILLAGEGUILDID_GET			126			// ������ ���� ��� ID�� ���´�.
#define	SO_DB_INFOKIND_GUILD_GETRELATION			128			// ����� ���踦 ���´�.
#define	SO_DB_INFOKIND_VILLAGEHNSNAME_GET			129			// ������ ��� �̸��� ���´�.
#define	SO_DB_INFOKIND_VILLAGEDEFENCE_GET			130			// ���� ������ ���´�.
#define	SO_DB_INFOKIND_NOTICEREADCOUNT				131			// �Խù��� ���� Ƚ���� ���´�.
#define	SO_DB_INFOKIND_BUYTICKET					132			// Ticket ���
#define	SO_DB_INFOKIND_GETTICKET					133			// Ticket ���� ���
#define	SO_DB_INFOKIND_RESETTICKET					134			// Ticket �ʱ�ȭ
#define	SO_DB_INFOKIND_VILLAGEENTERPLAYERGET		135			// ������ ���� ����� ���
//#define	SO_DB_INFOKIND_VILLAGEENTERPLAYERSET	136			// ������ ���� ����� ����
#define	SO_DB_INFOKIND_APPENDMERCENARY				137			// ���� �߰�
#define	SO_DB_INFOKIND_SETMERCENARYPARAM			138			// ���� ���� ����
#define	SO_DB_INFOKIND_DELETEMERCENARY				139			// ���� ����
#define	SO_DB_INFOKIND_VILLAGEGETCAPITAL			140			// ���� ���ھ� ���
#define	SO_DB_INFOKIND_VILLAGESETCAPITAL			141			// ���� ���ھ� ����
#define	SO_DB_INFOKIND_INVESTORGETINFO				142			// ���� ������ ���� ���
#define	SO_DB_INFOKIND_INVESTORSETINFO				143			// ���� ������ ���� ����
#define SO_DB_INFOKIND_SAVEALLPARAM					147			// �뺴���� ��� �Ķ���͸� DB�� �����Ѵ�
#define	SO_DB_INFOKIND_UPDATEMERCENARY				148			// ���� �⺻ ���� ����
#define	SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA		150			// �������� ����Ʈ ������ ����
#define	SO_DB_INFOKIND_QUEST_COMPLETEQUEST			151			// ����Ʈ�� �Ϸ��Ѵ�.
#define	SO_DB_INFOKIND_QUEST_CANCELQUEST			152			// ����Ʈ�� ����Ѵ�.
#define	SO_DB_INFOKIND_GIVEFOLLOWER					153			// ����Ʈ�� ����Ѵ�.
//#define SO_DB_INFOKIND_CONCURRENTUSER				155
#define	SO_DB_INFOKIND_TRADE						156
#define	SO_DB_INFOKIND_HEALATVILLAGE				157			// Heal
#define	SO_DB_INFOKIND_INVEST_SETINVESTMENT			158			// ���� ������ ���� ����
#define	SO_DB_INFOKIND_INVEST_GETINVESTMENT			159			// ���� ������ List ���� ���
#define	SO_DB_INFOKIND_INVEST_DELINVESTMENT			160			// ���� ������ ���� �ʱ�ȭ
#define	SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT	161			// ���� ������ List���� ��� 10Percent ����
#define	SO_DB_INFOKIND_MOVEITEM						162			// �κ��丮�� �������� �̵� ��Ų��.
#define SO_DB_INFOKIND_SAVETOBANK                   163         // ���忡   �������ϱ�
#define SO_DB_INFOKIND_WITHDRAWFROMBANK             164         // ���忡�� ������
#define	SO_DB_KIND_BUYITEMINMARKET					165			// �������� ������ ����
#define	SO_DB_KIND_SELLITEMINMARKET					166			// �������� ������ ����
#define	SO_DB_INFOKIND_DROPITEM						167			// �ʵ忡�� �������� ������.
#define	SO_DB_INFOKIND_PICKUPITEM					168			// �ʵ忡�� �������� �ݴ´�.
#define	SO_DB_INFOKIND_CREATEITEM_BYADMIN			169			// ��ڰ� �ʵ忡 �������� �����.
#define	SO_DB_INFOKIND_CHANGEJOBMERCENARY			170			// ���� ����
#define	SO_DB_INFOKIND_BUYMERCENARY					171			// ���� ���
#define SO_DB_INFOKIND_PLANT_CANCEL_STORAGE         172			// â�����
#define SO_DB_INFOKIND_FARM_DESTROY_FACTORY           173
#define SO_DB_INFOKIND_FARM_DESTROY_STORAGE           174
#define SO_DB_INFOKIND_FARM_STORAGE_INPUT_ITEM        175
#define SO_DB_INFOKIND_FARM_STORAGE_OUTPUT_ITEM       176
#define SO_DB_INFOKIND_STORAGE_CHANGE_PAY             177
#define SO_DB_INFOKIND_STORAGE_DELETEITEM             178

#define	SO_DB_INFOKIND_GETITEMCOUNT_ITEMBANK		  179		// ������ ��ũ���� ������ ���� ������
#define SO_DB_INFOKIND_ITEMLIST_ITEMBANK			  180		// ������ ��ũ���� ������ ����Ʈ ������
#define SO_DB_INFOKIND_PUTITEM_FROMITEMBANK			  181		// ������ ��ũ���� �κ��丮�� ������ ��������
#define SO_DB_INFOKIND_SAVEITEM_ITEMBANK			  193		// ������ ��ũ�� �������� �����Ѵ�

#define SO_DB_INFOKIND_QUEST_PAY_MONEY				  182		// Quest ����� ���� ����
#define SO_DB_INFOKIND_QUEST_PAY_ITEM				  183		// Quest ����� Item�� ����
#define SO_DB_INFOKIND_QUEST_PAY_CREDIT				  184		// Quest ����� Credit�� ����
#define SO_DB_INFOKIND_QUEST_PAY_EXP				  185		// Quest ����� EXP�� ����
#define SO_DB_INFOKIND_QUEST_REQUEST_ITEM			  186		// Quest ����� �ڽ��� Item�� ����
#define SO_DB_INFOKIND_QUEST_REQUEST_MONEY			  187		// Quest ����� �ڽ��� ���� ����


#define SO_DB_INFOKIND_GET_ITEM_AUCTION				  188		// ������ ��� ������ �ϳ� ��� �´� 
#define SO_DB_INFOKIND_ADD_ITEM_AUCTION				  189		// ������ ��Ÿ� �߰��Ѵ� 
#define SO_DB_INFOKIND_SUCCESSFULBID_ITEM_AUCTION     190		// ������ ��Ÿ� �����Ѵ� 
#define SO_DB_INFOKIND_FAILBID_ITEM_AUCTION			  191		// ������ ��Ÿ� �����Ѵ� 
#define SO_DB_INFOKIND_DEL_ITEM_AUCTION				  192		// ������ ��Ÿ� �����Ѵ� 
//													  193       ������ ���� ����

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

#define	SO_DB_INFOKIND_GUILD_KICK					  212			// �÷��̾ ��ܿ��� �Ѱܳ���.
#define	SO_DB_INFOKIND_CONSUMECREDITITEM			  213			// �ſ뵵�� ������
#define	SO_DB_INFOKIND_GETMAXGUILDID     			  214           // ���±��� ������ ���ID�� �ִ밪

#define	SO_DB_INFOKIND_USEPORTAL         			  215           // ��Ż���
#define SO_DB_INFOKIND_GIVEMONEYBYADMIN               216           // ��� ������� ������
#define SO_DB_INFOKIND_DIEATBATTLELOSEMONEY           217           // ������ ��ĳ���Ͱ� �׾ ���� �Ҵ´�.
#define SO_DB_INFOKIND_DIEATBATTLELOSEITEM            218           // ������ �׾ �������� �Ҵ´�.
#define SO_DB_INFOKIND_GETITEMATBATTLE                219           // �������� �������� �����.
#define SO_DB_INFOKIND_ROBITEMATBATTLE                220           // �������� �������� ������.
#define SO_DB_INFOKIND_INCREASEMAXGENERAL			  221			// ��� ������ ����� ����
#define SO_DB_INFOKIND_CONSUMEDONKEYITEM			  222			// �糪�� ������ ���

#define SO_LOGIN_DB_NEWCHAR								230
#define SO_LOGIN_DB_DELCHAR								231
#define SO_LOGIN_DB_CHARSINFO							232


#define	SO_DB_INFOKIND_BUYMONSTER						233			// ���� ���
#define SO_DB_INFOKIND_DELETEMONSTER					244			// ���� �ذ�
#define SO_DB_INFOKIND_BOOTHFOLLOWERTRADE				245			// ���� �������� �뺴 �ŷ�	

#define SO_DB_INFOKIND_UPGRADEWEAPONSUCCESS				250			// ������ ����Ͽ� ���� ���׷��̵忡 �����ߴ�.
#define SO_DB_INFOKIND_UPGRADEWEAPONFAILURE				251			// ������ ����Ͽ� ���� ���׷��̵忡 �����ߴ�.
#define SO_DB_INFOKIND_UPGRADEWEAPONBROKEN				252			// ������ ����Ͽ� ���� ���׷��̵��� ���Ⱑ �ı��Ǿ���.

#define SO_DB_INFOKIND_AUDIT_FRIEND_SET_DENIAL			253			// ��ο��� ģ���� ���Űź� ó���Ѵ�.
#define SO_DB_INFOKIND_AUDIT_FRIEND_ADD_ONE				254			// ��ο��� ģ���� �߰��Ѵ�.
#define SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_SOME			255			// ��ο��� ģ���� �����Ѵ�.
#define SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_ALL			256			// ��ο��� ģ�� ��ü�� �����Ѵ�.

#define	SO_DB_INFOKIND_EVENT_EXPO						257			// ������ ���� �̺�Ʈ�� ���� ������ ���´�.
#define	SO_DB_INFOKIND_EVENT_EXPOBATTLERESULT			258			// ������ ���� �̺�Ʈ���� ���� ����� DB�� �����ش�.
#define	SO_DB_INFOKIND_EVENT_EXPORANKING				259			// ������ ���� �̺�Ʈ���� ��ŷ�� �����ش�.

#define	SO_DB_INFO_KIND_GET_GM_LIST						260			// ���� ��ڵ��� ������ ���´�.

#define SO_DB_INFO_DOKDO_QUEST_COMPLETED_LIST			261			// ���� ����Ʈ�� �Ϸ��� ����ڸ� ���´�.
#define SO_DB_INFO_DOKDO_QUEST_COMPLETED_SAVE			262			// ���� ����Ʈ�� ������ �����Ѵ�.

#define SO_DB_INFO_MACRO_USER_INFO_SAVE					263			// ��ũ�� ����� ������ DB�� �����Ѵ�.
#define SO_DB_INFO_GETEVENTITEM							264			// �̺�Ʈ �������� ���´�.
#define SO_DB_INFOKIND_QUEST_PAY_EVENTITEM				265			// �̺�Ʈ �����ۿ� ��÷ �Ǿ��� ��� DB�� ����.

#define SO_DB_INFOKIND_IMPERIUM_USE						266			// �������� ����ؼ� �Ϲ�����->������������ ���Ѵ�.
#define SO_DB_INFOKIND_CHANGECHARACTER					267			// �ʵ忡�� �¾� ���� ĳ���ͷ� ���ư���.

#define SO_DB_INFOKIND_INCREASE_GUILD_UNIT				268			// ��������� ������Ų��.����� ��������� �����.
#define SO_DB_INFOKIND_DECREASE_GUILD_UNIT				269			// ��������� ���ҽ�Ų��.����� ��������� ��������.

#define	SO_DB_INFOKIND_INCREASE_SUPPLY					270			// ��� ����ǰ���� ������Ų��.
#define SO_DB_INFOKIND_DECREASE_SUPPLY					271			// ��� ����ǰ���� ���ҽ�Ų��.

#define SO_DB_INFOKIND_INCREASE_DEFENCESOLDIER			272			// ���� ���񺴼� ����.

#define SO_DB_INFOKIND_WAR_LOGIN						273			// ���������� �α����� �Ѵ�.
#define SO_DB_INFOKIND_GET_GUILD_ITEM					274			// �������� ���õ� ����� ��ǰ�� ��������.

#define SO_DB_INFOKIND_INCREASE_POPULATION				275			// ���� �α���,�ִ� �α����� �����Ѵ�.
#define SO_DB_INFOKIND_DECREASE_POPULATION				276			// ������ ������ �α����� �����Ѵ�.

#define SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_OFFER		277			// ȭģ�޼����� ������.
#define SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_ACCEPT	278			// ȭģ�޼����� ����Ѵ�.
#define SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_CANCEL	279

#define SO_DB_INFOKIND_SET_VILLAGE_DAMAGE				280			// ������ ���� �������� DB�� ����.
#define SO_DB_INFOKIND_GET_VILLAGE_DAMAGE				281			// ������ �׾��ٰ� �ٽ� �߸� 

#define SO_DB_INFOKIND_SET_GUILD_RESULT					282			// ���� �¸���. ����� �����Ѵ�.(ĳ���� �̸�����)
#define SO_DB_INFOKIND_GET_GUILD_RESULT					283			// ������ �׾��� �ٽ� �߸� ������� ������ ���´�.

#define SO_DB_INFOKIND_SET_DELETE_INVESTMENT			284			// ������ �����ϸ� ���������� �������.

#define SO_DB_INFOKIND_SET_GUILD_WAR_END				285			// 10�ÿ� ������ ������.
#define SO_DB_INFOKIND_SET_POWER_INVESTMENT_END			286			// 12�ÿ� ���������� ������.

#define SO_DB_INFOKIND_DELETE_CHARACTOR_WAR				287

#define SO_DB_INFOKIND_VILLAGE_INFO						288			// ���� ���۽� ���������� ����.
#define SO_DB_INFOKIND_CHARACTER_LIST					289			// ���� ���۽� ĳ���������� ����.
#define SO_DB_INFOKIND_VILLAGE_ITEM						290			// ���� ���۽� ���� �������� ����.

#define SO_DB_INFOKIND_WAR_PROFIT						291			// ���� ���ͱ��� ����.
#define SO_DB_INFOKIND_DECREASE_SOLDIER					292			// ���� ���� ���� ���δ�.

#define SO_DB_INFOKIND_VILLAGEOWNER_DELETE				293			// 


#define SO_DB_INFOKIND_SET_WARGIVEUP                    294         //
#define SO_DB_INFOKIND_GET_WARGIVEUP                    295         //

//-----------------------------------------------------------------------------------------------------------




// ���� ť���� ó�� �Ұ͵� 
//-----------------------------------------------------------------------------------------------------------
#define	SO_DB_INFOKIND_LOGOUTINFO					300			// �÷��̾��� �α׾ƿ� ������ DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_SETWORLDTIME					301			// ���� ������ �ð��� DB�� �����Ѵ�.
#define	SO_DB_INFOKIND_SETSATIETY					302			// �������� �����Ѵ�.
#define SO_DB_INFOKIND_CONCURRENTUSER				303			// ���� ���� �����ڼ��� ���� �Ѵ�
#define	SO_DB_INFOKIND_VILLAGEENTERPLAYERSET		304			// ������ ���� ����� ����
#define SO_DB_INFOKIND_LOGOUTSAVEALLPARAM			305			// �α׾ƿ��ÿ� ��� �Ķ���� ���� 
#define SO_DB_INFOKIND_LOGOUTSTART					306			// �α׾ƿ��� ���� �ߴٴ� FLAG �� ���� �Ѵ� 
#define SO_DB_INFOKIND_WARINFO						307			// ������ ������ �����Ѵ�.
//-----------------------------------------------------------------------------------------------------------


// ������ 
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

// ����
struct stVillageInvestorData
{
	SI32						m_siInvestorDBAccount;
	UI08						m_uiInvestorSlot;

	SI08						m_InvestorType;						// ������ Type(0 : ���, 1 : ���)
	UI08						m_InvestorID[21];					// ������ ID
	float						m_Percent;							// ������
	__int64						m_Money;							// ���� �ݾ�
	int							m_UpdateTime;						// ���� �ð�
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
	UI08		uiItemIndex;		//	(0~9 : â�� 10���� �����۸� �����ϴ�.)
	UI16		uiItemCode;			//	������ �ڵ�
	UI16		uiItemQnt;			//	������ ����
};

struct	DBIKFarmSetItem
{
	UI16		uiVillageCode;	
	UI08		uiFarmID;
	UI08		uiFactoryID;
	UI08		uiItemIndex;		//	(0~9 : â�� 10���� �����۸� �����ϴ�.)
	UI16		uiItemCode;			//	������ �ڵ�
	UI16		uiItemQnt;			//	������ ����
};



struct	DBIKStorageInputItem
{
	SI32		DBAccount;
	UI08		uiSlot;

	
	UI16		uiVillageCode;	
	UI08		uiFarmID;
	UI08		uiFactoryID;

	UI16		uiItemCode;			//	������ �ڵ�
	UI16		uiItemQnt;			//	������ ����
	

	UI08        uiMercenary;
	UI08        uiMercenaryItemPosition;     // �뺴�� ������ ��ġ

	UI08		uiStoreItemPosition;		//	(0~9 : â�� 10���� �����۸� �����ϴ�.)
	
	
};

struct	DBIKStorageOutputItem
{
	SI32		DBAccount;
	UI08		uiSlot;
	
	UI16		uiVillageCode;	
	UI08		uiFarmID;
	UI08		uiFactoryID;

	UI16		uiItemCode;			//	������ �ڵ�
	UI16		uiItemQnt;			//	������ ����
	

	UI08        uiMercenary;
	UI08        uiMercenaryItemPosition;     // �뺴�� ������ ��ġ

	UI08		uiStoreItemPosition;		//	(0~9 : â�� 10���� �����۸� �����ϴ�.)
	
};




struct  DBIKWearItem
{
	SI32						siDBAccount;
	UI08						uiSlot;
	UI08						uiFollowerID;	
	UI08						uiWearLoc;			//	������ ��ġ (0~7)
	UI16						uiItemID;
	SI16						siItemPosInInventory;
	UI16						uiQuantity;			// �κ��丮 ����
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
	MONEY			moPay; // �ӱ�
	MONEY           moTax; // ����
    MONEY           moCapital; // ����
	UI32			Labor; // �뵿��
	
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

	UI32            uiWork;                // �۾���
	
};


struct	DBIKBuyFarmInfo
{
	SI32			siAccount;
	UI08			uiSlot;	
	
	UI16			uiVillageCode;	
	UI08			uiFarmID;				//	���� ��ȣ (0~9)
	MONEY           moMoney;


};

struct	DBIKBuyFarmByForceInfo
{
	SI32			siAccount;
	UI08			uiSlot;	

	UI16			uiVillageCode;	
	UI08			uiFarmID;				//	���� ��ȣ (0~9)
	//BOOL			bSendMsgToPlayer;		// �÷��̾�Կ� ���� �ü��� �ȷȴٴ� �Ϳ� ���� �޽����� �����°�? (�޽����� ������ ��쿡�� ���� �α��ν� 
	
		
	MONEY			mnPrice;
											//																	����ü� �Ǹű� ���� �ʵ��� �ؾ� �Ѵ�.)
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
	UI32			uiVillageCount;								// ����
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
//	���� ����
//-------------------------------------------------------
struct DBIKWorldTime
{
	SI32		Year;								// �⵵
	SI32		Month;								// ��
	SI32		Day;								// ��
	SI32		Hour;								// ��
};

//----------------------------------------------------
// �÷��̾� ���� ����...
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
//	�뺴 ���� ����...
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
// �ʵ� ���� ����
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
//	���� ���� ����
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
	// �ڿ��� Length ��ŭ �ȳ���(CHAR)�� �´�
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
// ��� ���� ����..
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
	SI32					siTradeCreditRanking;				// ���� �ſ뵵 ����
	UI32					uiTradeCredit;						// ���� �ſ뵵
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
	MONEY				siMoney;					//���� �׼�
	UI16				uiMonth;					//����(��)
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
	SI32			OwnNum;				//����
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

	UI16		    uiItemCode;			//	������ �ڵ�
	UI16		    uiItemQnt;			//	������ ����

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
//	Desc : ĳ������ �⺻ ���� ����ü 
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
//	Desc : �������� ����Ʈ ���� 
//--------------------------------------------------------------------------------------------
typedef struct __ProgressQuestInfo
{	
	WORD		  questCode;
	DWORD		  questObject1;
	DWORD		  questObject2;

}ProgressQuestInfo;



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �뺴�� �� ���� 
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
//	Desc : �κ��丮���� ������ �ϳ��� ���� 
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
//	Desc : ������ ������ �ϳ��� ���� 
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
	UI16			uiNewKind;		// ���Ӱ� ���ϴ� Kind(�Ϲ�����->��������)
//	UI16			uiItemID;		// �������� ����ϸ鼭 ����� ItemID
//	UI16			uiQuantity;		// �������� ����ϸ鼭 ����� Item����.
	SI16			siHp;			// ���������� ü��.
	SI16            siMp;
	BOOL			bCaptain;		// �������� ���ϴ°� �Ϲ� �������� ���ϴ°�
//	MONEY			mnMoney;		//�Ҹ�Ǵ� ��.
//	SI16			siSupply;		// ���� ����������.
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
	UI32			uiGuildID;			//��� ���̵�.
	UI16			uiItemID;
	SI16			siQuantity;			//������Ű�� ����.
//	SI32			siSupply;			//�Ҹ�Ǵ� ����� ���޷�.
	MONEY			mnMoney;			//�Ҹ�Ǵ� ��.
	UI16			uiVillageCode;		//���� �ڵ�.
	UI32			uiPopulation;		//���ҵǴ� �α���.
};

struct DBIKDecreaseGuildUnit
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			// ��� ���̵�.
	UI16			uiItemID;			// ������ ���̵�.
	SI16			siQuantity;			// ���� ��Ű�� ����.
//	UI08			uiMercenarySlot;	// �뺴 ���̵�.
//	SI16			siInventoryPos;		// �κ��丮 ��ġ.
};

struct DBIKMakeGuildSupply
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			// ��� ���̵�.
	SI32			siQuantity;			// ������Ű�� ����ǰ�� ��.
	MONEY			mnMoney;			// �Ҹ�Ǵ� ��.
};

struct DBIKDecreaseGuildSupply
{
	SI32			siDBAccount;
	UI08			uiDBSlot;			
	UI32			uiGuildID;			// ��� ���̵�.
	SI16			siQuantity;			// ������Ű�� ����ǰ�� ��.
};

struct DBIKIncreaseSoldierDefence
{
	SI32			siDBAccount;
	UI08			uiDBSlot;
	UI32			uiGuildID;			// ��� ���̵�.
	SI16			siVillageCode;		// villageCode
	SI32			siDefenceSoldier;	// �����Ǵ� ���� ��
	SI32			siSupply;			// �����ϴ� Supply
	MONEY			mnMoney;			// �����ϴ� ��.	
	UI32			uiPopulation;		//���ҵǴ� �α���.
};	


struct DBIKGetVillageInfo
{
	UI32			uiPopulation;		// �α�
	UI32			uiMaxPopulation;    // �ִ� ���� �α�.
	SI32			siDefenceSoldier;	// ���񺴼�.
};

struct DBIKSetVillageInfo
{
	UI16			uiVillageCode;		// villageCode
	UI32			uiPopulation;		// ��ȭ�� ���� �α���.
	UI32			uiMaxPopulation;	// ��ȭ�� ���� �ִ� �α���.
};

struct DBIKDecreasePopulation
{
	UI16			uiVillageCode;		// uiVillageCode
	UI32			uiPopulation;		// ��ȭ�� ���� �α���.
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
	UI16           uiVillageCode;   //���� �ڵ�
	UI32           uiGuildID;       //��� �ڵ�
	MONEY		   mnMoney;          //��
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

	UI16			uiVillageCode;		// ���� �ø� ����
	SI32			siUpDurability;		// �ø� ����
	SI32			siRate1;			// ������ ����
	SI32			siRate2;			// ������ �и�
};

struct  DBIKGetOutInvestment
{
	UI16			uiVillageCode;		// ���� �ø� ����

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
//	Desc : �α��ν� �ʿ��� �ʼ� ���� 
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
//  Desc : ������ ��� �߰��� DB�� ���� �Ҷ� ���̴� ����ü
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
//  Desc : ������ ��� ������ DB�� ���� �Ҷ� ���̴� ����ü
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
//  Desc : ������ ��� ������ DB�� ���� �Ҷ� ���̴� ����ü
//--------------------------------------------------------------------------------------------
struct sDB_FailBidItemAuction
{
	DWORD		auctioneerDBAccount;
	BYTE		auctioneerCharSlot;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : ������ ��� ������ DB�� ���� �Ҷ� ���̴� ����ü
//--------------------------------------------------------------------------------------------
struct sDB_DelItemAuction
{
	DWORD		auctioneerDBAccount;
	BYTE		auctioneerCharSlot;
};



//--------------------------------------------------------------------------------------------
//  Name :
//  Desc : ������ ��� ������ DB���� ���ö� ���̴� ����ü 
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
	
	char		name [LOGIN::NAME_MAX_LENGTH];	// ĳ������ �̸�  
	WORD		kind;							// ĳ������ ���� 
	
	WORD		Str;							// ��		 
	WORD		Dex;							// ��ø��			
	WORD		Vit;							// ü��				
	WORD		Int;							// ����

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