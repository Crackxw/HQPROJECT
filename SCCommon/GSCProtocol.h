#ifndef	_GSCPROTOCOL_HEADER
#define	_GSCPROTOCOL_HEADER

//����������������������������������������������������������������������������
// Ŭ���̾�Ʈ���� ��������
//����������������������������������������������������������������������������
// 0 - 999			:	���� ���� �� ���� �α��� ����.
// 1000 - 1999		:	�ʵ� ������...(����)		(1900~1999) ���ڰŸ�
// 2000 - 2999		:	���� ������...(����)
// 3000 - 3999		:	���� ������...(����)
// 4000 - 4999		:	������ ���� ����.(����)
// 5000 - 5999		:	��� ���...
// 6000 - 6999		:	�ʵ�� ���� ����.(����)
// 7000 - 7499		:	���Ǵ�
// 7500 - 7999		:	�ŷ�
// 8000 - 8999		:	����Ʈ ����
// 9000 - 9999		:   ��ų(9000~9100), ��ż�(9100~9200)  ���� ( ���� Ž��� ) Ư�� NPC���� ���尣(9201~9300)
// 10000 - 10999	:	���� �ý������ΰ�(�ð���)

//����������������������������������������������������������������������������
// ���� �ź�
//����������������������������������������������������������������������������
//#define ON_RESPONSE_SERVER_FULL					(DWORD(1))	// ����ڰ� �ʹ� ���Ƽ� ������ �ź��Ѵ�.
//����������������������������������������������������������������������������
// �����ϴ� Ŭ���̾�Ʈ�� ������ �����Ѵ�.
//����������������������������������������������������������������������������
//#define	ON_REQUEST_CLIENTKIND					(DWORD(2))
//#define ON_RESPONSE_CLIENTKIND					(DWORD(3))

//#define	ON_RET_CLIENT_KIND_OK					(DWORD(0))	// �������� Ŭ���̾�Ʈ��.
//#define ON_RET_CLIENT_KIND_CLIENT				(DWORD(1))	// �ٸ� Ŭ���̾�Ʈ��.
//#define ON_RET_CLIENT_KIND_USER					(DWORD(2))	// �ش� �������� Ŭ���̾�Ʈ�� �ƴϴ�.
//#define	ON_RET_CLIENT_KIND_INVALIDVERSION		(DWORD(3))	// ������ ���� �ʴ�.

//����������������������������������������������������������������������������
// �����ϴ� ���������� �����Ѵ�.
//����������������������������������������������������������������������������
//#define ON_REQUEST_SERVERTYPE					(DWORD(17))
//#define ON_RESPONSE_SERVERTYPE					(DWORD(18))

//����������������������������������������������������������������������������
// ���� ����
//����������������������������������������������������������������������������
//#define ON_REQUEST_NUMUSER					    (DWORD(19))
//#define ON_RESPONSE_NUMUSER						(DWORD(20))

//����������������������������������������������������������������������������
// �α����Ѵ�.
//����������������������������������������������������������������������������
#define ON_REQUEST_LOGIN						(DWORD(4))	// �α����� ��û�Ѵ�.
#define ON_RESPONSE_LOGIN						(DWORD(5))	// ��û�� �α��ο� ���� ����

//����������������������������������������������������������������������������
// ĳ���� ������ ��û�Ѵ�.
//����������������������������������������������������������������������������
//#define ON_REQUEST_CHARINFO						(DWORD(21))
//#define ON_RESPONSE_CHARINFO					(DWORD(22))

//#define ON_RET_CHARINFO_OK						(DWORD(0))	// ������ �����µ� ���� 
//#define ON_RET_CHARINFO_FAIL					(DWORD(1))	// ������ �����µ� ���� 

//����������������������������������������������������������������������������
// �α׾ƿ��Ѵ�.
//����������������������������������������������������������������������������
#define ON_REQUEST_LOGOUT						(DWORD(6))	// �α׾ƿ��� ��û�Ѵ�.
#define ON_RESPONSE_LOGOUT						(DWORD(7))	// ��û�� �α׾ƿ��� ���� ����

#define ON_RET_LOGOUT_OK						(DWORD(0))	// �α� �ƿ��� ����Ѵ�.
//#define ON_RET_LOGOUT_FORCE						(DWORD(1))	// ���� �α׾ƿ� ��Ų��.

//����������������������������������������������������������������������������
// GameGuard����
//����������������������������������������������������������������������������
#define ON_CHECK_GAMEGUARD						(DWORD(40))	// Server->Client 
#define ON_REQUEST_CHECK_GAMEGUARD				(DWORD(41)) // Client->Server
#define ON_RESPONSE_CHECK_GAMEGUARD				(DWORD(42)) // Server->Client 
#define ON_USER_INFO_GAMEGUARD					(DWORD(43)) // Server->Client
// #define ON_RET_BATTLE_NO						(DWORD(11))	// ������ �ź��Ѵ�.

//����������������������������������������������������������������������������
// ���ο� ĳ���͸� �����.
//����������������������������������������������������������������������������
//#define ON_REQUEST_CHENTRY						(DWORD(10))
//#define ON_RESPONSE_CHENTRY						(DWORD(11))

//#define ON_RET_CHENTRY_OK						(DWORD(0))	// ������ ����Ѵ�.
//#define ON_RET_CHENTRY_ID						(DWORD(1))	// ���� ID�� ���� ����� �����Ѵ�.
//#define ON_RET_CHENTRY_CHAR						(DWORD(2))	// �̹� 3����  �� á��.
//#define ON_RET_CHENTRY_NO						(DWORD(3))	// ���� ó���� �� �� ����.
//#define ON_RET_CHENTRY_ABILITY					(DWORD(4))	// �ɷ�ġ�� �̻��� �ִ�.

//����������������������������������������������������������������������������
// ĳ���͸� �����Ѵ�.
//����������������������������������������������������������������������������
//#define ON_REQUEST_DELCHAR						(DWORD(12))
//#define ON_RESPONSE_DELCHAR						(DWORD(13))

//#define ON_RET_DELCHAR_OK						(DWORD(0))	// ������ ����Ѵ�.
//#define ON_RET_DELCHAR_NO						(DWORD(1))	// ���� ó���� �� �� ����.

//����������������������������������������������������������������������������
// �˸� ���带 ��û�Ѵ�.
//����������������������������������������������������������������������������
//#define ON_REQUEST_MESSAGE						(DWORD(15))
//#define ON_RESPONSE_MESSAGE						(DWORD(16))

//����������������������������������������������������������������������������
// �÷��̾ �ʻ󿡼� ��ġ�� �̵���Ų��.
//����������������������������������������������������������������������������
#define ON_REQUEST_POSITION						(DWORD(1005))		// Ŭ���̾�Ʈ���� ���� ���ΰ� ĳ���Ͱ� ��ġ�� Ÿ���� ��ġ�� �˷��ش�.
#define ON_RESPONSE_POSITION					(DWORD(1006))		// �������� Ŭ���̾�Ʈ���� �̵��� ĳ���͵��� ��ġ�� �˷��ش�.

//#define ON_RET_POSITION_OK						(DWORD(0))	// �̵��� ����Ѵ�.
//#define ON_RET_POSITION_NO						(DWORD(1))	// �̵�����
//����������������������������������������������������������������������������
// ����.
//����������������������������������������������������������������������������
#define ON_REQUEST_BATTLE						(DWORD(1007))	// ������ ��û�Ѵ�. 
#define ON_RESPONSE_BATTLE						(DWORD(1008))	// ��û�� ������ ���� ����
#define ON_RESPONSE_BATTLEPLAYER				(DWORD(1100))	// ������ �Ѵٰ� �ٸ� ����ڿ��� �˸�

#define ON_RET_BATTLE_WAIT						(DWORD(0))	// ���� �����·� ����.
#define ON_RET_BATTLE_BEGIN						(DWORD(1))	// ������ �����Ѵ�.
#define ON_RET_BATTLE_NO						(DWORD(2))	// ������ �ź��Ѵ�.
//����������������������������������������������������������������������������
// �÷��̾��� �̵��� ������ ������ ������.
//����������������������������������������������������������������������������
//#define ON_RESPONSE_INFO_LINE					(DWORD(1009))

//#define ON_RET_NEW_PLAYERS						(DWORD(0))	// ���ο� �÷��̾���� ������.
//#define ON_RET_NEW_ITEM							(DWORD(1))	// ���߰ߵ� �����۵��� ������.
//#define ON_RET_RELEASE_PLAYERS					(DWORD(2))	// ������ �÷��̾���� ������.
//#define ON_RET_RELEASE_ONE_PLAYER				(DWORD(3))	// ������ �÷��̾ ������.
#define ON_RET_BATTLE_NO						(DWORD(2))	// ������ �ź��Ѵ�.
//����������������������������������������������������������������������������
// �÷��̾��� ������ ������.
//����������������������������������������������������������������������������
#define ON_REQUEST_PLAYER_INFO					(DWORD(1010))	// �÷��̾��� ������ ��û�Ѵ�.
#define	ON_RESPONSE_PLAYER_INFO					(DWORD(1011))	// �÷��̾��� ������ ��û�������� ������ �޾Ҵ�.
//����������������������������������������������������������������������������
// �ʿ� �������� �ݰ� ������.
//����������������������������������������������������������������������������
#define ON_REQUEST_THROWITEM					(DWORD(1012))
#define ON_RESPONSE_THROWITEM					(DWORD(1013))
#define ON_REQUEST_PICKUPITEM					(DWORD(1014))
#define ON_RESPONSE_PICKUPITEM					(DWORD(1015))
#define	ON_CHANGE_FIELDITEM						(DWORD(1022))

//����������������������������������������������������������������������������
// �̵��Ҷ� �� �������� ���� ����� ĳ���ͳ� �������� ĳ����.
#define	ON_NEWINFOONMOVE						(DWORD(1016))
#define	ON_ADDCHAR								(DWORD(1017))
#define	ON_DELCHAR								(DWORD(1018))
//����������������������������������������������������������������������������
//#define	ON_RESPONSE_AOURNDCHARINFO				(DWORD(1019))	// ���� ĳ���͵��� ���� ����.

//����������������������������������������������������������������������������
// ĳ������ ���� ������ ����.
//����������������������������������������������������������������������������
#define	ON_REQUEST_CHAR_DETAILINFO				(DWORD(1020))	// ĳ������ �� ���� ��û.
#define	ON_RESPONSE_CHAR_DETAILINFO				(DWORD(1021))	// ĳ������ �� ���� ��û.(����)

#define	ON_REQUEST_MOVEMAP						(DWORD(1030))	// ���� �̵��ÿ� �޽���.
#define	ON_RESPONSE_MOVEMAP						(DWORD(1031))	// ���� �̵��ÿ� �޽���.(����)


// �� ����
#define	ON_BOARDSHIP							(DWORD(1040))	// �谡 �����Ѵ�.
#define	ON_GETOUTSHIP							(DWORD(1041))	// �迡�� ������.

// ��ΰ��� ó��
//����������������������������������������������������������������������������
//#define	ON_REQUEST_AUDIT_TOTAL					(DWORD(1500))	// ��� ����� ����
#define	ON_RESPONSE_AUDIT_TOTAL					(DWORD(1501))	// ��� ����� ���� (����)
#define	ON_REQUEST_AUDIT_ACCOUNT				(DWORD(1502))	// ��� ���¸�� ����
#define	ON_RESPONSE_AUDIT_ACCOUNT				(DWORD(1503))	// ��� ���¸�� ����(����)
#define	ON_REQUEST_AUDIT_OWN					(DWORD(1504))	// ��� �ε��꺸�� ����
#define	ON_RESPONSE_AUDIT_OWN					(DWORD(1505))	// ��� �ε��꺸�� ����(����)
#define	ON_REQUEST_AUDIT_VILLAGE_LIST			(DWORD(1506))	// ��� ������ ���� ����Ʈ
#define	ON_RESPONSE_AUDIT_VILLAGE_LIST			(DWORD(1507))	// ��� ������ ���� ����Ʈ(����)
#define	ON_REQUEST_AUDIT_VILLAGE_INFO			(DWORD(1508))	// ��� ������ �� ���� ����
#define	ON_RESPONSE_AUDIT_VILLAGE_INFO			(DWORD(1509))	// ��� ������ �� ���� ����(����)
#define	ON_REQUEST_AUDIT_STATUS					(DWORD(1510))	// ��� ���� ����
#define	ON_RESPONSE_AUDIT_STATUS				(DWORD(1511))	// ��� ���� ����(����)
#define	ON_REQUEST_AUDIT_SETNAME				(DWORD(1512))	// ��� �̸� �Է�
#define	ON_RESPONSE_AUDIT_SETNAME				(DWORD(1513))	// ��� �̸� �Է�(����)
#define ON_REQUEST_AUDIT_FRIEND_LIST			(DWORD(1514))	// ��� ģ�� ����Ʈ
#define ON_RESPONSE_AUDIT_FRIEND_LIST			(DWORD(1515))	// ��� ģ�� ����Ʈ(����)
#define ON_REQUEST_AUDIT_FRIEND_SET_DENIAL		(DWORD(1516))	// ��� ģ�� ���Űź�
#define ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL		(DWORD(1517))	// ��� ģ�� ���Űź�(����)
#define ON_REQUEST_AUDIT_FRIEND_ADD_ONE			(DWORD(1518))	// ��� ģ�� �߰�
#define ON_RESPONSE_AUDIT_FRIEND_ADD_ONE		(DWORD(1519))	// ��� ģ�� �߰�(����)
#define ON_REQUEST_AUDIT_FRIEND_REMOVE_SOME		(DWORD(1520))	// ��� ģ�� �Ѹ� ����
#define ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME	(DWORD(1521))	// ��� ģ�� �Ѹ� ����(����)
#define ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL		(DWORD(1522))	// ��� ģ�� ��ü ����
#define ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL		(DWORD(1523))	// ��� ģ�� ��ü ����

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define	ON_REQUEST_AUDIT_PROFIT_STATISTICS		(DWORD(1524))	// ��� ���� ���� ����
#define ON_RESPONSE_AUDIT_PROFIT_STATISTICS		(DWORD(1525))	// ��� ���� ���� ����(����)

#define	ON_RET_AUDIT_OK							(DWORD(0))		// ��ο��� �Ϲ����� ����.
#define	ON_RET_AUDIT_NO							(DWORD(1))		// ��ο��� �Ϲ����� ����.

//����������������������������������������������������������������������������
// �������� ����, ���� �Ѵ�.
//����������������������������������������������������������������������������
#define ON_REQUEST_WEARITEM_INFO				(DWORD(1550))			//	������ ������ ����
#define ON_RESPONSE_WEARITEM_INFO				(DWORD(1551))			//	������ ������ ����(����)
#define ON_REQUEST_WEARITEM						(DWORD(1552))			//	�������� �����Ѵ�.
#define ON_RESPONSE_WEARITEM					(DWORD(1553))			//	�������� �����Ѵ�.(����)
#define ON_REQUEST_STRIPITEM					(DWORD(1554))			//	�������� �����Ѵ�.
#define ON_RESPONSE_STRIPITEM					(DWORD(1555))			//	�������� �����Ѵ�.(����)

#define ON_CHANGE_CLOTHES						(DWORD(1556))		    // �ֺ������ ������ �����Ѱ� �ٲ�����.

#define ON_CHANGE_CHARACTERCODE					(DWORD(1557))		    // �ֺ������ ĳ���� �ڵ尡 �ٲ�����.
//#define ON_CHANGE_CHARACTERNATION				(DWORD(1558))		    // ������ �ٲ������.
//OnChangeClothesMsg


#define ON_RET_WEARITEM_OK						(DWORD(0))				//	�������� OK
#define ON_RET_WEARITEM_NO						(DWORD(1))				//	�������� NO

//����������������������������������������������������������������������������
// �������, ��ȭ��� ����
//����������������������������������������������������������������������������
#define ON_REQUEST_SET_ATTACK_MODE				(DWORD(1560))			// ������� ��û
#define ON_RESPONSE_SET_ATTACK_MODE				(DWORD(1561))			// ������� ��û�� ���� ����

//����������������������������������������������������������������������������
// ���Ͱ� �ڽ��� �븮�ų�, �������
#define ON_TARGETME								(DWORD(1570))	// ���� �ڽ��� �븱��.
#define ON_RELEASEME							(DWORD(1571))	// Ÿ�ٿ��� �����.
// �̸�Ƽ�� ǥ��
#define ON_REQUEST_IMOTICON						(DWORD(1572))	// �̸�Ƽ�� ǥ���� ������.
#define ON_RESPONSE_IMOTICON					(DWORD(1573))
// ������ �Ҹ�
#define ON_REQUEST_SPENDITEM					(DWORD(1574))
#define ON_RESPONSE_SPENDITEM					(DWORD(1575))
//����������������������������������������������������������������������������
// �����ȳ��� ���� ó��
//����������������������������������������������������������������������������
#define	ON_REQUEST_WRITE_DESCRIPT				(DWORD(1600))	// �ȳ��� ���ٴ� �޼���(����)
#define	ON_RESPONSE_WRITE_DESCRIPT				(DWORD(1601))	// �ȳ��� ���ٴ� �޼���(����)
#define	ON_REQUEST_READ_DESCRIPT				(DWORD(1602))	// �ȳ��� �д� �޼���(�ʵ�)
#define	ON_RESPONSE_READ_DESCRIPT				(DWORD(1603))	// �ȳ��� �д� �޼���(����)

#define ON_RET_DESCRIPT_OK						(DWORD(0))
#define ON_RET_DESCRIPT_NO						(DWORD(1))
#define ON_RET_DESCRIPT_NOTBEST					(DWORD(2))

#define ON_REQUEST_VILLAGE_NATION				(DWORD(1604))
#define ON_RESPONSE_VILLAGE_NATION				(DWORD(1605))

//OnReqVillageGetNation

// ���� ����
#define	ON_REQUEST_WARP							(DWORD(1800))	// ����
#define	ON_RESPONSE_WARP						(DWORD(1801))	// ����(����)

// ���� �Ÿ�
#define	ON_REQUEST_CHARGEBOOTH					(DWORD(1900))	// ���Ǵ� ����
#define	ON_RESPONSE_CHARGEBOOTH					(DWORD(1901))	// ���Ǵ� ����(����)
#define ON_REQUEST_DISPLAYBOOTH					(DWORD(1902))	// ���Ǵ��� �����۵� �����ֱ�
#define ON_RESPONSE_DISPLAYBOOTH				(DWORD(1903))
//#define ON_REQUEST_INPUTBOOTH_DESC				(DWORD(1904))	// ���Ǵ� ���� ������ �Է�
#define ON_RESPONSE_INPUTBOOTH_DESC				(DWORD(1905))
#define ON_REQUEST_GETBOOTHINFO					(DWORD(1906))	// ���Ǵ� ���� ������ ����
#define ON_RESPONSE_GETBOOTHINFO				(DWORD(1907))
#define	ON_REQUEST_LEAVEBOOTH					(DWORD(1908))	// ���Ǵ� ������
#define	ON_RESPONSE_LEAVEBOOTH					(DWORD(1909))	// ���Ǵ� ������(����)
#define	ON_REQUEST_SETBOOTHITEM					(DWORD(1910))	// ���Ǵ� ���� ����
#define	ON_RESPONSE_SETBOOTHITEM				(DWORD(1911))	// ���Ǵ� ������
#define	ON_REQUEST_CANCELBOOTHITEM				(DWORD(1912))	// ���Ǵ� ���� ���� ���
#define	ON_RESPONSE_CANCELBOOTHITEM				(DWORD(1913))	// ���Ǵ� ���� ���� ���(����)
//#define	ON_REQUEST_BUYBOOTHITEM					(DWORD(1914))	// ���Ǵ� ���� ���� ����
#define	ON_RESPONSE_BUYBOOTHITEM				(DWORD(1915))	// ���Ǵ� ���� ���� ����(����)
#define	ON_RESPONSE_SELLBOOTHITEM				(DWORD(1916))	// ������ �ȸ��� ������ �޴� �޼���

#define ON_RET_MARKETPLACE_OK					(DWORD(0))
#define ON_RET_MARKETPLACE_NO					(DWORD(1))
//#define ON_RET_MARKETPLACE_FULL					(DWORD(2))		// ���� ����, ���̻� �� �� ����.
#define	ON_RET_MARKETPLACE_EMPTY				(DWORD(3))		// �����, ������ ����.
#define	ON_RET_MARKETPLACE_PLAYER				(DWORD(4))		// �ٸ� ����ڰ� �����


#define	ON_REQUEST_ATTACK_VILLAGE				(DWORD(1980))	// ������ �����Ѵ�.
#define	ON_RESPONSE_ATTACK_VILLAGE				(DWORD(1981))	// ������ �����Ѵ�.(����)

#define	ON_CHANGED_VILLAGE_DEFENCE				(DWORD(1990))	// ������ ������ ����Ǿ���.
// �������� ó��
//����������������������������������������������������������������������������
#define ON_REQUEST_ENTER_VILLAGE				(DWORD(2000))	// ������ ����.
#define ON_RESPONSE_ENTER_VILLAGE				(DWORD(2001))

//����������������������������������������������������������������������������
#define ON_REQUEST_LEAVE_VILLAGE				(DWORD(2002))	// �������� ������.
#define ON_RESPONSE_LEAVE_VILLAGE				(DWORD(2003))


#define ON_RET_LEAVE_VILLAGE_OK					(DWORD(0))	// �㰡�Ѵ�.
#define ON_RET_LEAVE_VILLAGE_NO					(DWORD(1))	// �ź��Ѵ�.
//#define ON_RET_LEAVE_VILLAGE_PLAYER				(DWORD(2))	// �ٸ��÷��̾ �������� ������.
//����������������������������������������������������������������������������
#define ON_REQUEST_LIST_ITEM					(DWORD(2004))	// ���忡�� ��ǰ ����Ʈ ��û5
#define ON_RESPONSE_LIST_ITEM					(DWORD(2005))
//����������������������������������������������������������������������������
#define ON_REQUEST_BUY_ITEM						(DWORD(2006))	// ������ ���.
#define ON_RESPONSE_BUY_ITEM					(DWORD(2007))

#define ON_RET_NOITEM							(DWORD(1))	// �� ������ �������
#define ON_RET_NOMONEY							(DWORD(2))	// �÷��̾��� ���� ���ڸ� ���
#define	ON_RET_NOSALE							(DWORD(3))	// �Ǹ����� �ʴ� �������̴�.
#define	ON_RET_BUYZERO							(DWORD(4))	// 0���� ����� �Ѵ�.
//����������������������������������������������������������������������������
#define ON_REQUEST_SELL_ITEM					(DWORD(2008))	// ������ �Ǵ�.
#define ON_RESPONSE_SELL_ITEM					(DWORD(2009))
//����������������������������������������������������������������������������
#define ON_RESPONSE_CHANGED_ITEMINFO			(DWORD(2012))	// �ٸ� �÷��̾���� ������ �����۸���Ʈ�� ������ �������
																// �� ������ �����۵��� ������ �����Ѵ�.
//����������������������������������������������������������������������������
// ����, ���忡���� �޽���
#define	ON_REQUEST_BANK_INFO					(DWORD(2016))	// ���� ���� �䱸.
#define	ON_RESPONSE_BANK_INFO					(DWORD(2017))
#define	ON_REQUEST_BANK_DEPOSIT					(DWORD(2018))	// ����
#define ON_RESPONSE_BANK_DEPOSIT				(DWORD(2019))
#define	ON_REQUEST_BANK_DRAWING					(DWORD(2020))	// ���
#define ON_RESPONSE_BANK_DRAWING				(DWORD(2021))
#define	ON_REQUEST_BANK_OPENSAVINGACCOUNT		(DWORD(2022))	// ���¸� ����ٰ� ��û�Ѵ�.
#define	ON_RESPONSE_BANK_OPENSAVINGACCOUNT		(DWORD(2023))	// ���¸� ����ٰ� ��û������������
#define	ON_REQUEST_BANK_BANKBOOK_LIST			(DWORD(2024))	// ������ ����Ʈ�� �޶�� ��û�Ѵ�.
#define	ON_RESPONSE_BANK_BANKBOOK_LIST			(DWORD(2025))	// ������ ����Ʈ�� �޶�� �������� ����
#define	ON_REQUEST_BANK_BANKBOOK_LISTCOUNT		(DWORD(2026))	// ������ ����Ʈ�� ���� �޶�� ��û�Ѵ�.

#define ON_REQUEST_BANK_SAVE                    (DWORD(2600))	// �Ա� ��û
#define ON_RESPONSE_BANK_SAVE                   (DWORD(2601))	// �Ա� ����
#define ON_REQUEST_BANK_WITHDRAW                (DWORD(2602))	// ��� ��û
#define ON_RESPONSE_BANK_WITHDRAW               (DWORD(2603))	// ��� ����


#define	ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT		(DWORD(2499))	// ������ ����Ʈ�� ���� �޶�� �������� ����

#define	ON_REQUEST_BANK_COLLATERAL				(DWORD(2500))	// �㺸�� �ñ�ٰ� ��û�Ѵ�.
#define	ON_RESPONSE_BANK_COLLATERAL				(DWORD(2501))	// �㺸�� �ñ�ٰ� ��û�������� ����
#define	ON_REQUEST_BANK_COLLATERALLIST			(DWORD(2502))	// �㺸List�� ��û�Ѵ�.
#define	ON_RESPONSE_BANK_COLLATERALLIST			(DWORD(2503))	// �㺸List�� ��û�������� ����
#define ON_REQUEST_BANK_TAKECOLLATERAL			(DWORD(2504))	// �㺸�� �����޶�� ��û�Ѵ�.
#define ON_RESPONSE_BANK_TAKECOLLATERAL			(DWORD(2505))	// �㺸�� �����޶�� ��û �������� ����.
//#define ON_REQUEST_BANK_MAXCREDITLOAN			(DWORD(2506))	//	�ſ�㺸 �ִ� �ݾ�
#define ON_RESPONSE_BANK_MAXCREDITLOAN			(DWORD(2507))



#define	ON_RET_BANK_OK							(DWORD(0))		// �������� OK
#define	ON_RET_BANK_NO							(DWORD(1))		// �������� NO
#define	ON_RET_BANK_EXISTOPENSAVINGACCOUNT		(DWORD(3))		// �ش� ���°� �̹� �ִ�.
#define	ON_RET_BANK_FULLSAVINGACCOUNT			(DWORD(4))		// �� �̻��� �ش� ���¸� ���� ���� ����.(���� ������� �ϴ� ���¿� ���ؼ�)
#define	ON_RET_BANK_FULLTOTALSAVINGACCOUNT		(DWORD(5))		// �� �̻��� �ش� ���¸� ���� ���� ����.(��ü ���� ���¿� ���ؼ�.)
//#define	ON_RET_BANK_MAXMONEY					(DWORD(6))		// �Ѱ� �׼��� �Ѿ���.
//#define	ON_RET_BANK_MINMONEY					(DWORD(7))		// �ּ� �׼����� �۴�.


// ����, ���忡���� �޽���
#define ON_REQUEST_FARM_INFO					(DWORD(2530))	//	���� ������ ��û��.
#define	ON_RESPONSE_FARM_INFO					(DWORD(2531))
#define ON_REQUEST_FARM_BUY						(DWORD(2532))	//	������ �����Ѵٰ� ��û��.
#define ON_RESPONSE_FARM_BUY					(DWORD(2533))
#define ON_REQUEST_FARM_SLOTINFO				(DWORD(2534))	//	���� ������ ��û��.
#define ON_RESPONSE_FARM_SLOTINFO				(DWORD(2535))
//#define ON_REQUEST_FARM_STRUCINFO				(DWORD(2536))	//	�ǹ� ������ ��û��.
//#define ON_RESPONSE_FARM_STRUCINFO				(DWORD(2537))
#define ON_REQUEST_FARM_BUILDSTRUCT				(DWORD(2538))	//	�ǹ��� ���´ٰ� ��û��.
#define ON_RESPONSE_FARM_BUILDSTRUCT			(DWORD(2539))
#define ON_REQUEST_FARM_EXPANDSLOT				(DWORD(2540))	//	���� �ü� Ȯ�� �Ѵٰ� ��û.
#define ON_RESPONSE_FARM_EXPANDSLOT				(DWORD(2541))
#define ON_REQUEST_FARM_DESTROYSTRUCT			(DWORD(2542))	//	�ǹ��� ö���Ѵٰ� ��û��.
#define ON_RESPONSE_FARM_DESTROYSTRUCT			(DWORD(2543))
#define ON_REQUEST_FARM_SETEQUIPMENT			(DWORD(2544))	//	�ǹ��� �μ� �ü����� ���´ٰ� ��û��.
#define ON_RESPONSE_FARM_SETEQUIPMENT			(DWORD(2545))
#define ON_REQUEST_FARM_ITEMPRODUCT				(DWORD(2546))	//	���忡�� �������� ������ ���� ��û�Ѵ�.
#define ON_RESPONSE_FARM_ITEMPRODUCT			(DWORD(2547))
#define ON_REQUEST_FARM_WORK					(DWORD(2548))	//	���忡�� ���� �Ѵٰ� ��û�Ѵ�.
#define ON_RESPONSE_FARM_WORK					(DWORD(2549))
#define ON_REQUEST_FARM_WORKINFO				(DWORD(2550))	//	������ �ϲ� ������ ������.
#define ON_RESPONSE_FARM_WORKINFO				(DWORD(2551))
#define ON_REQUEST_FARM_UPGRADESLOT				(DWORD(2552))	//	������ ���׷��̵��Ѵ�.
#define ON_RESPONSE_FARM_UPGRADESLOT			(DWORD(2553))
#define ON_REQUEST_FARM_WORKSTART				(DWORD(2554))	//	���忡�� ���� �Ϸ� �Ѵ�.
#define ON_RESPONSE_FARM_WORKSTART				(DWORD(2555))
//#define ON_REQUEST_FARM_SELL					(DWORD(2556))	//	������ �Ǵ�.
//#define ON_RESPONSE_FARM_SELL					(DWORD(2557))
#define	ON_FARMWORK_COMPLETED					(DWORD(2558))	// ���� ���忡�� �۾����� �Ϸ�Ǿ���.
#define ON_REQUEST_FARM_WORK_HACK				(DWORD(2559))


// â��
#define ON_REQUEST_STORAGE_INFO					(DWORD(2580))	//	â���� �������� ���´�.
#define ON_RESPONSE_STORAGE_INFO				(DWORD(2581))
#define ON_REQUEST_STORAGE_INPUTITEM			(DWORD(2582))	//	â�� �������� �ִ´�.
#define ON_RESPONSE_STORAGE_INPUTITEM			(DWORD(2583))
#define ON_REQUEST_STORAGE_OUTPUTITEM			(DWORD(2584))	//	â���� �������� ����.
#define ON_RESPONSE_STORAGE_OUTPUTITEM			(DWORD(2585))

#define ON_REQUEST_PLANT_CHANGE_PAY				(DWORD(2590))	//	�ӱ��� �ٲܶ� �޼���
#define ON_RESPONSE_PLANT_CHANGE_PAY			(DWORD(2591))
#define ON_REQUEST_PLANT_CANCEL_PLAN			(DWORD(2592))	//	�ӱ��� �ٲܶ� �޼���
#define ON_RESPONSE_PLANT_CANCEL_PLAN			(DWORD(2593))

// â�� �� ����, ����, ����, ���� ������ ��� ��.
//#define ON_RET_FARM_OWNER						(DWORD(2))		//	���� �������� �ڰ��̴�.
//#define	ON_RET_FARM_WORKER						(DWORD(3))		//	���� �ϲ��� �ڰ��̴�.
#define	ON_RET_FARM_MATERIAL					(DWORD(4))		//	��ᰡ �����ϴ�.
#define	ON_RET_FARM_TOOL						(DWORD(5))		//	������ ����.
#define	ON_RET_FARM_MONEY						(DWORD(6))		//	���� �����ϴ�.
#define	ON_RET_FARM_NOTOWNER					(DWORD(7))		//	���� ������ �ƴϱ⶧���� �� �� ����.
#define	ON_RET_FARM_ITEMERROR					(DWORD(8))		//	�׷� �������� ������ �� ����.
//#define	ON_RET_FARM_FULLUPGRAGE					(DWORD(9))		//	���̻� ���׷��̵� �� �� ����.

// ����, ���������� �޽���.
#define	ON_REQUEST_ENTER_INN					(DWORD(2027))	// ���� ������ ����.
#define	ON_RESPONSE_ENTER_INN					(DWORD(2028))	// ���� ������ ����.(����)
#define	ON_REQUEST_LEAVE_INN					(DWORD(2029))	// ���� �������� ������
#define	ON_RESPONSE_LEAVE_INN					(DWORD(2030))	// ���� �������� ������.(����)


#define	ON_REQUEST_ENTER_VWR					(DWORD(2031))	// ���� ����, ���ǿ� ����.							
#define	ON_RESPONSE_ENTER_VWR					(DWORD(2032))	// ���� ����, ���ǿ� ����.(����)
#define	ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM		(DWORD(2033))	// ���� ����, ��ȭ�濡 ����.
#define	ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM		(DWORD(2034))	// ���� ����, ��ȭ�濡 ����.(����)
#define	ON_REQUEST_INN_CREATECHATROOM			(DWORD(2035))	// ���� ����, ��ȭ���� �����.
#define	ON_RESPONSE_INN_CREATECHATROOM			(DWORD(2036))	// ���� ����, ��ȭ���� �����.(����)
#define	ON_WAITROOM_CHANGE_INFO					(DWORD(2037))	// ���� ����, ���ǿ� ����� ������ �����ش�.
#define	ON_RET_INN_OK							(DWORD(0))
#define ON_RET_INN_NO							(DWORD(1))
#define	ON_VWR_CREATEROOM						(DWORD(0))		// ���ο� ���� �����Ǿ���.
#define	ON_VWR_DELETEROOM						(DWORD(1))		// ������ ���� ��� �Ǿ���.
#define	ON_VWR_CHANGEPLAYERNUM					(DWORD(2))		// ������ �濡 �÷��̾� ���� ����Ǿ���.

#define	ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM		(DWORD(2038))	// ���� ����, ��ȭ�濡�� ������.
#define	ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM		(DWORD(2039))	// ���� ����, ��ȭ�濡�� ������.(����)
	
#define	ON_VIL_INN_JOIN_PLAYER					(DWORD(2040))	// ���� ����, �÷��̾ �߰��Ǿ���.
#define	ON_VIL_INN_LEAVE_PLAYER					(DWORD(2041))	// ���� ����, �÷��̾ �����Ǿ���.

#define	ON_VIL_INN_CHATTING						(DWORD(2042))	// ���� ����, ��ȭ�濡�� ä��

#define	ON_REQUEST_ENTER_MARKET					(DWORD(2050))	// ���忡 ����.
#define	ON_RESPONSE_ENTER_MARKET				(DWORD(2051))	// ���忡 ����.(����)
#define	ON_REQUEST_LEAVE_MARKET					(DWORD(2052))	// ���忡�� ���´�.
#define	ON_RESPONSE_LEAVE_MARKET				(DWORD(2053))	// ���忡�� ���´�.(����)

#define	ON_REQUEST_ENTER_NOTICEBOARD			(DWORD(2060))	// �Խ��ǿ� ����.
#define	ON_RESPONSE_ENTER_NOTICEBOARD			(DWORD(2061))	// �Խ��ǿ��� ������.(����)
#define	ON_REQUEST_NOTICEBOARD_NOTICELIST		(DWORD(2062))	// �Խ��ǿ��� �Խù� ����Ʈ�� ��û�Ѵ�.
#define	ON_RESPONSE_NOTICEBOARD_NOTICELIST		(DWORD(2063))	// �Խ��ǿ��� �Խù� ����Ʈ�� ������.
#define	ON_REQUEST_REGIST_NOTICE				(DWORD(2064))	// �Խ��ǿ��� ���� ����Ѵ�.
#define	ON_RESPONSE_REGIST_NOTICE				(DWORD(2065))	// �Խ��ǿ��� ���� ����Ѵ�.(����)
#define	ON_REQUEST_DELETE_NOTICE				(DWORD(2066))	// �Խ����� ���� �����Ѵ�.
#define	ON_RESPONSE_DELETE_NOTICE				(DWORD(2067))	// �Խ����� ���� �����Ѵ�.(����)
#define	ON_REQUEST_VIEW_NOTICE					(DWORD(2070))	// �Խ����� ���� ����.
#define	ON_RESPONSE_VIEW_NOTICE					(DWORD(2071))	// �Խ����� ���� ����.(����)
#define ON_REQUEST_MODIFY_NOTICE				(DWORD(2072))	// �Խ����� ���� �����Ѵ�.
#define ON_RESPONSE_MODIFY_NOTICE				(DWORD(2073))	// �Խ����� ���� �����Ѵ�.

#define ON_REQUEST_BARRACK_GETSOLDERLIST			(DWORD(2090))	// �뺴���� List�� ��û�Ѵ�.
#define ON_RESPONSE_BARRACK_GETSOLDERLIST			(DWORD(2091))	// �뺴���� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_BARRACK_BUYSOLDER				(DWORD(2092))	// �뺴�� ����Ѵ�.
#define ON_RESPONSE_BARRACK_BUYSOLDER				(DWORD(2093))	// �뺴�� ��뿡 �����Ѵ�.
#define ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST	(DWORD(2094))	// �ذ��� �뺴���� List�� ��û�Ѵ�.
#define ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST	(DWORD(2095))	// �ذ��� �뺴���� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_BARRACK_DISMISSALSOLDER			(DWORD(2096))	// �뺴�� �ذ��Ѵ�.
#define ON_RESPONSE_BARRACK_DISMISSALSOLDER			(DWORD(2097))	// �뺴�� �ذ� �����Ѵ�.

#define ON_REQUEST_GBARRACK_GETSOLDERLIST			(DWORD(2130))	// ����뺴���� List�� ��û�Ѵ�.
#define ON_RESPONSE_GBARRACK_GETSOLDERLIST			(DWORD(2131))	// ����뺴���� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_GBARRACK_BUYSOLDER				(DWORD(2132))	// ����뺴�� ����Ѵ�.
#define ON_RESPONSE_GBARRACK_BUYSOLDER				(DWORD(2133))	// ����뺴�� ��뿡 �����Ѵ�.
#define ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST	(DWORD(2134))	// ������ ����뺴���� List�� ��û�Ѵ�.
#define ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST	(DWORD(2135))	// ������ ����뺴���� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_GBARRACK_CHANGEJOBSOLDER			(DWORD(2136))	// ����뺴�� �����Ѵ�.
#define ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER		(DWORD(2137))	// ����뺴�� ������ �����Ѵ�.
#define ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST	(DWORD(2138))	// �ذ��� ����뺴���� List�� ��û�Ѵ�.
#define ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST	(DWORD(2139))	// �ذ��� ����뺴���� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_GBARRACK_DISMISSALSOLDER			(DWORD(2140))	// ����뺴�� �ذ��Ѵ�.
#define ON_RESPONSE_GBARRACK_DISMISSALSOLDER		(DWORD(2141))	// ����뺴�� �ذ� �����Ѵ�.

#define ON_REQUEST_MBARRACK_GETMONSTERLIST			(DWORD(2150))	// ���͵��� List�� ��û�Ѵ�.
#define ON_RESPONSE_MBARRACK_GETMONSTERLIST			(DWORD(2151))	// ���͵��� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_MBARRACK_BUYMONSTER				(DWORD(2152))	// ���͸� ����Ѵ�.
#define ON_RESPONSE_MBARRACK_BUYMONSTER				(DWORD(2153))	// ���� ��뿡 �����Ѵ�.
#define ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST	(DWORD(2154))	// �ذ��� ���͵��� List�� ��û�Ѵ�.
#define ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST	(DWORD(2155))	// �ذ��� ���͵��� List ��û�� �����Ѵ�.
#define ON_REQUEST_MBARRACK_DISMISSMONSTER			(DWORD(2156))	// ���͸� �ذ��Ѵ�.
#define ON_RESPONSE_MBARRACK_DISMISSMONSTER			(DWORD(2157))	// ���� �ذ� �����Ѵ�.

#define ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST		(DWORD(2160))	// 2�� ������ ������� List�� ��û�Ѵ�.
#define ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST	(DWORD(2161))	// 2�� ������ ������� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_GBARRACK_CHANGE_GENERAL				(DWORD(2162))	// 2�� ����� ���� �Ѵ�.
#define ON_RESPONSE_GBARRACK_CHANGE_GENERAL				(DWORD(2163))	// 2�� ��� ������ ���� �Ѵ�.
#define ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST	(DWORD(2164))	// �ذ��� 2�� ������� List�� ��û�Ѵ�.
#define ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST	(DWORD(2165))	// �ذ��� 2�� ������� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_GBARRACK_DISMISSAL_GENERAL			(DWORD(2166))	// 2�� ��� �ذ��Ѵ�.
#define ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL			(DWORD(2167))	// 2�� ��� �ذ� �����Ѵ�.
#define ON_REQUEST_GBARRACK_GET_GENERAL_LIST			(DWORD(2168))	// 2�� ������� List�� ��û�Ѵ�.
#define ON_RESPONSE_GBARRACK_GET_GENERAL_LIST			(DWORD(2169))	// 2�� ������� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_GBARRACK_BUY_GENERAL					(DWORD(2170))	// 2�� ����� ����Ѵ�.
#define ON_RESPONSE_GBARRACK_BUY_GENERAL				(DWORD(2171))	// 2�� ��� ��뿡 �����Ѵ�.


#define ON_REQUEST_HOSPITAL_GETCURELIST			(DWORD(2100))	// �뺴���� ġ�� List�� ��û�Ѵ�.
#define ON_RESPONSE_HOSPITAL_GETCURELIST		(DWORD(2101))	// �뺴���� ġ�� List�� ��û�� �����Ѵ�.
#define ON_REQUEST_HOSPITAL_CURESOLDER			(DWORD(2102))	// �뺴�� ġ���Ѵ�.
#define ON_RESPONSE_HOSPITAL_CURESOLDER			(DWORD(2103))	// �뺴�� ġ�ῡ �����Ѵ�.
#define ON_REQUEST_HOSPITAL_CUREALLSOLDER		(DWORD(2104))	// ��� �뺴�� ġ���Ѵ�.
#define ON_RESPONSE_HOSPITAL_CUREALLSOLDER		(DWORD(2105))	// ��� �뺴�� ġ�ῡ �����Ѵ�.
#define ON_RET_AUTOCUREOK						(DWORD(2))		// �ڵ�ġ�Ḧ �㰡�Ѵ�.

#define ON_REQUEST_WHARF_ENTER					(DWORD(2110))	// �εο� ����.
#define ON_RESPONSE_WHARF_ENTER					(DWORD(2111))	// �εο� ����.(����)
#define ON_RESPONSE_WHARF_CHANGETICKETLIST		(DWORD(2112))	// ǥ�� List�� ������Ʈ �Ѵ�.
#define ON_REQUEST_WHARF_BUYTICKET				(DWORD(2113))	// �εο��� ǥ�� ���.
#define ON_RESPONSE_WHARF_BUYTICKET				(DWORD(2114))	// �εο��� ǥ�� ���.(����)
#define ON_REQUEST_WHARF_EXIT					(DWORD(2115))	// �εο��� ������.
#define ON_RESPONSE_WHARF_EXIT					(DWORD(2116))	// �εο��� ������.(����)
#define ON_REQUEST_WHARFWAITROOM_ENTER			(DWORD(2117))	// �εδ��ǿ� ����.
#define ON_RESPONSE_WHARFWAITROOM_ENTER			(DWORD(2118))	// �εδ��ǿ� ����.(����)(���� Ticket�� �ð��� �ʱ��ϸ� �� ������.)
#define ON_REQUEST_WHARFWAITROOM_EXIT			(DWORD(2119))	// �εδ��ǿ��� ������.
#define ON_RESPONSE_WHARFWAITROOM_EXIT			(DWORD(2120))	// �εδ��ǿ��� ������.(����)
#define ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP	(DWORD(2121))	// �εδ��ǿ��� �¼��ϴ�.
#define ON_RET_NOTUSETICKET						(DWORD(2))		// Ƽ���� ����Ҽ����� ���

// ��û
#define ON_REQUEST_GOVERNMENT_MAIN_ENTER					(DWORD(2200))		// ��û ����
#define ON_RESPONSE_GOVERNMENT_MAIN_ENTER					(DWORD(2201))		// ��û ���� ����
#define ON_REQUEST_GOVERNMENT_MAIN_LEAVE					(DWORD(2202))		// ��û ������
#define ON_RESPONSE_GOVERNMENT_MAIN_LEAVE					(DWORD(2203))		// ��û ������ ����

#define ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER				(DWORD(2204))		// ��û, ���� ����
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER				(DWORD(2205))		// ��û, ���� ���� ����
#define ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE				(DWORD(2206))		// ��û, ���� ������
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE				(DWORD(2207))		// ��û, ���� ������ ����
#define ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST			(DWORD(2208))		// ��û, ���� List���
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST			(DWORD(2209))		// ��û, ���� List��� ����
#define ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT		(DWORD(2210))		// ��û, ���� �ϱ�
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT		(DWORD(2211))		// ��û, ���� �ϱ� ����
#define	ON_RET_NOTINVESTMENT_MANYMAXINVESTMENT				(DWORD(2))
#define	ON_RET_NOTINVESTMENT_MANYMONEY						(DWORD(3))
#define	ON_RET_NOTINVESTMENT_TIMEOVER						(DWORD(4))

#define ON_REQUEST_GOVERNMENT_VILLAGEINFO					(DWORD(2212))		// ��û, ���� ���� ��� ��û
#define ON_RESPONSE_GOVERNMENT_VILLAGEINFO					(DWORD(2213))		// ��û, ���� ���� ��� ����
#define	ON_REQUEST_PROFIT_STATISTICS						(DWORD(2214))
#define	ON_RESPONSE_PROFIT_STATISTICS						(DWORD(2215))

#define	ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT				(DWORD(2216))		// ��û, �����ѵ� ������ ������
#define	ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT			(DWORD(2217))		// ��û, �����ѵ� ������ ������ ����

// ���
#define	ON_REQUEST_CREATE_GUILD								(DWORD(2300))		// ����� �����Ѵ�.(�䱸)
#define	ON_RESPONSE_CREATE_GUILD							(DWORD(2301))		// ����� �����Ѵ�.(����)
#define	ON_REQUEST_DELETE_GUILD								(DWORD(2302))		// ����� �����Ѵ�.(�䱸)
#define	ON_RESPONSE_DELETE_GUILD							(DWORD(2303))		// ����� �����Ѵ�.(����)
#define	ON_REQUEST_JOIN_LEAVE_GUILD							(DWORD(2304))		// ��ܿ� ����/Ż���Ѵ�.(�䱸)
#define	ON_RESPONSE_JOIN_LEAVE_GUILD						(DWORD(2305))		// ��ܿ� ����/Ż���Ѵ�.(����)
#define	ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST			(DWORD(2306))		// ��ܿ� �����ϱ� ���ϴ� ����������� ����Ʈ�� ���´�.(�䱸)
#define	ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST			(DWORD(2307))		// ��ܿ� �����ϱ� ���ϴ� ����������� ����Ʈ�� ���´�.(����)
#define	ON_REQUEST_JOINGUILD_OKNO							(DWORD(2308))		// ��ܿ� �����ϱ� ���ϴ� ���� ����ڿ��� OK, NO�� ������.(�䱸)
#define	ON_RESPONSE_JOINGUILD_OKNO							(DWORD(2309))		// ��ܿ� �����ϱ� ���ϴ� ���� ����ڿ��� OK, NO�� ������.(����)
#define	ON_REQUEST_GUILD_MEMBERS_LIST						(DWORD(2310))		// ��ܿ� ����Ʈ�� ���´�.(�䱸)
#define	ON_RESPONSE_GUILD_MEMBERS_LIST						(DWORD(2311))		// ��ܿ� ����Ʈ�� ���´�.(����)
#define	ON_REQUEST_GUILD_DISMISSAL							(DWORD(2312))		// ��ܿ��� �ذ��Ѵ�.(�䱸)
#define	ON_RESPONSE_GUILD_DISMISSAL							(DWORD(2313))		// ��ܿ��� �ذ��Ѵ�.(����)
#define	ON_REQUEST_GUILD_LIST								(DWORD(2314))		// ����� ����Ʈ�� ���´�.(�䱸)
#define	ON_RESPONSE_GUILD_LIST								(DWORD(2315))		// ����� ����Ʈ�� ���´�.(����)
#define	ON_GUILD_MSG										(DWORD(2316))		// ����� �޽���
#define	ON_REQUEST_GUILD_SET_RELATION						(DWORD(2317))		// ��ܰ��� ���踦 �����Ѵ�.(�䱸)
#define	ON_RESPONSE_GUILD_SET_RELATION						(DWORD(2318))		// ��ܰ��� ���踦 �����Ѵ�.(����)
#define ON_REQUEST_GUILD_CREATE_COST						(DWORD(2319))		// ��� ���� ����� ���´�(�䱸)
#define ON_RESPONSE_GUILD_CREATE_COST						(DWORD(2320))		// ��� ���� ����� ���´�(����)

#define	ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT				(DWORD(2321))		// �������¿� �����Ҽ� �ִ� �ְ� ���ھ��� ��´�.(�䱸)
#define	ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT				(DWORD(2322))		// �������¿� �����Ҽ� �ִ� �ְ� ���ھ��� ��´�.(����)
#define	ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT				(DWORD(2323))		// �������¿� �����Ѵ�.(�䱸)
#define	ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT				(DWORD(2324))		// �������¿� �����Ѵ�.(����)

#define	ON_REQUEST_GUILD_DETAIL_INFO						(DWORD(2350))		// ����� ������ ������ ���´�.(�䱸)
#define	ON_RESPONSE_GUILD_DETAIL_INFO						(DWORD(2351))		// ����� ������ ������ ���´�.(����)
#define ON_REQUEST_GUILD_PROMOTION							(DWORD(2352))		// ��ܿ��� Ư�� �������� �Ӹ��Ѵ�.(�䱸)
#define ON_RESPONSE_GUILD_PROMOTION							(DWORD(2353))		// ��ܿ��� Ư�� �������� �Ӹ��Ѵ�.(����)
#define ON_REQUEST_GUILD_DEMOTION							(DWORD(2354))		// ��ܿ��� ����(���޿� ���� ����)�Ѵ�.(�䱸)
#define ON_RESPONSE_GUILD_DEMOTION							(DWORD(2355))		// ��ܿ��� ����(���޿� ���� ����)�Ѵ�.(����)

#define	ON_ATTACK_TYPE_NEARCAPTAIN							(DWORD(0))
#define ON_ATTACK_TYPE_CAPTAIN								(DWORD(1))
#define ON_ATTACK_TYPE_NORMAL								(DWORD(2))

#define ON_REQUEST_FIELDATTACK_READY						(DWORD(2700))		// ���� ���⸦ �߻��Ѵٴ� ��ȣ�� ������.
#define ON_RESPONSE_FIELDATTACK_READY						(DWORD(2701))		// ���� ���⸦ �߻��Ѵٴ� ��ȣ�� �޾Ҵ�.

#define ON_REQUEST_FIELDATTACK_ACTION						(DWORD(2702))		//
#define ON_RESPONSE_FIELDATTACK_ACTION						(DWORD(2703))		//

#define ON_RET_ATTACK_POWER									(DWORD(0))			// ��3��
#define ON_RET_DEFENCE_POWER								(DWORD(1))			// ���
#define ON_RET_NONE_POWER									(DWORD(2))			// ��

#define ON_REQUEST_FIELDATTACK_DEAD							(DWORD(2704))		//
#define ON_RESPONSE_FIELDATTACK_DEAD						(DWORD(2705))		//

#define ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY			(DWORD(2706))
#define ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY		(DWORD(2707))
#define ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION		(DWORD(2708))
#define ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION		(DWORD(2709))

#define ON_RET_ATTACK_OK                                    (DWORD(0))         // ok
#define ON_RET_NOT_ATTACK_TIME                              (DWORD(1))         // �����ð��� �ƴϴ�.
#define ON_RET_NOT_VILLAGE_CODE                             (DWORD(2))         // ���� �ڵ尡 Ʋ����.
#define ON_RET_NOT_ENEMYGUILD                               (DWORD(3))         // ���� ��� ������ �ƴϴ�.
#define ON_RET_NOT_ATTACK_UNITE                             (DWORD(4))         // ���� ������ �ƴϴ�. 
#define ON_RET_NOT_ATTACK_DELAY_TIME                        (DWORD(5))         // ���� ������ �ð��� ���Ҵ�. 
#define ON_RET_ATTACK_NO                                    (DWORD(6))


#define ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT					(DWORD(2731))		// ���� ������ ���� ����ǰ�� ���޹޴´�.
#define ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT				(DWORD(2732))		// (����).

#define ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK				(DWORD(2729))		// ���� �������¸� �ٲܼ� �ִ��� �˾ƿ´�.
#define ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK				(DWORD(2730))		// (����).

#define ON_IMPERIUM_USE										(DWORD(2733))		//	������ ���.

#define ON_RET_ATTACK_TYPE_NOT_AUTH							(DWORD(0))			// ����̳� ������� �ƴϴ�.
#define ON_RET_ATTACK_TYPE_ACTIVE							(DWORD(1))			// �ٸ� ���� ����Ÿ���� �������̴�.
#define ON_RET_ATTACK_TYPE_NO								(DWORD(2))			// �׹��� �ٸ� ����� ������.
#define ON_RET_ATTACK_TYPE_OK								(DWORD(3))			// ���� AttackType������ �����Ǿ���.
#define ON_RET_ATTACK_TYPE_DELAY							(DWORD(4))			// ���ѽð��� ���� ������ �ʾҴ�.

#define ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK				(DWORD(2710))		// ���� �������¸� �⸧�������� �ٲ۴�.
#define ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK				(DWORD(2711))		// ����

#define ON_RET_OIL_OK										(DWORD(0))
#define	ON_RET_OIL_NOT_MONEY								(DWORD(1))
#define	ON_RET_OIL_NO										(DWORD(2))

#define ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK				(DWORD(2712))		// ���� �������¸� ���������� �ٲ۴�.
#define ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK				(DWORD(2713))		// ����

#define ON_RET_STONE_OK										(DWORD(0))
#define	ON_RET_STONE_NOT_MONEY								(DWORD(1))
#define	ON_RET_STONE_NO										(DWORD(2))

#define ON_REQUEST_VILLAGE_DEFENCE							(DWORD(2735))		// ���� ���� ��� �޼����� ������.
#define ON_RESPONSE_VILLAGE_DEFENCE							(DWORD(2736))		// ����

#define ON_RET_DEFENSE_OK									(DWORD(0))			// ����
#define ON_RET_DEFENSE_NOT_MONEY							(DWORD(1))			// ���� �����ϴ�.
#define ON_RET_DEFENSE_NO									(DWORD(2))			// �ٸ� ����.

#define	ON_CHANGED_VILLAGE_ATTACK_TYPE						(DWORD(2728))		// �ð��� ������ ���� ������ ���°� ������� ���ƿԴٰ� �˷��ش�.
																				// ���� �ֺ��� �÷��̾�鿡�Ը� ����.
#define ON_REQUEST_INCREASE_GUILD_UNIT						(DWORD(2714))		// �������� ����.
#define ON_RESPONSE_INCREASE_GUILD_UNIT						(DWORD(2715))		// (����� �������� ���Ѵ�.)

#define ON_REQUEST_DECREASE_GUILD_UNIT						(DWORD(2716))		// ���������� �����ش�.
#define ON_RESPONSE_DECREASE_GUILD_UNIT						(DWORD(2717))		// (����� �������� ��������.)

#define ON_REQUEST_LIST_GUILD_UNIT							(DWORD(2726))		// ����� ���� ���� ����Ʈ�� ��û�Ѵ�.
#define ON_RESPONSE_LIST_GUILD_UNIT							(DWORD(2727))		//

#define ON_REQUEST_INCREASE_GUILD_SUPPLY					(DWORD(2718))		// ����� ����ǰ�� ���Ѵ�.
#define ON_RESPONSE_INCREASE_GUILD_SUPPLY					(DWORD(2719))		//	

#define ON_REQUEST_DECREASE_GUILD_SUPPLY					(DWORD(2720))		// ����� ����ǰ�� ��������.
#define ON_RESPONSE_DECREASE_GUILD_SUPPLY					(DWORD(2721))

#define ON_REQUEST_LIST_GUILD_SUPPLY						(DWORD(2724))		// ��� ����ǰ ����Ʈ�� ���´�.
#define ON_RESPONSE_LIST_GUILD_SUPPLY						(DWORD(2725))		// 

#define ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER			(DWORD(2722))		// ���� ������ ���� ��Ų��.
#define ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER		(DWORD(2723))		// ���� ������ ���� ��Ų��.(����).

#define ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER				(DWORD(2737))		// ���� ������ ���� ���´�.
#define ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER				(DWORD(2738))		// ����.

#define ON_CHANGED_PLAYER_DEAD_PENALTY						(DWORD(2734))		// ���������� �׾ ���� �г�Ƽ�� ������� ������.

#define ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER				(DWORD(2739))		// ȭģ���� �޼����� ������.
#define ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER			(DWORD(2740))		// ����.

#define ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT			(DWORD(2741))		// ȭģ���� ���.
#define ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT			(DWORD(2742))		// ����.
	
#define ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL			(DWORD(2743))		// ȭ������ �ô´�.
#define ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL			(DWORD(2744))		// ȭ������ �ô´�.

#define ON_REQUEST_GUILD_VILLAGEATTACK_RESULT				(DWORD(2745))		// ��������� ��û�Ѵ�.(���)
#define ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT				(DWORD(2746))		// ��������� ���� ����.

#define ON_REQUEST_GUILD_CHOICE_PLAYER						(DWORD(2747))		// ������� ������ ������ ������ ������ �����Ѵ�.
#define ON_RESPONSE_GUILD_CHOICE_PLAYER						(DWORD(2748))		// ����.

#define ON_REQUEST_VILLAGE_INVESTMENT_POWER					(DWORD(2749))		// �ش� ������ ���ڸ� �Ҷ� �ش� ������ ������ �Ҽ� �ִ��� �����.
#define ON_RESPONSE_VILLAGE_INVESTMENT_POWER				(DWORD(2750))		// ����.

#define	ON_CHANGED_WAR_END									(DWORD(2751))		// ĳ���͸� ���� ĳ���ͷ� �ٲٰ� �������� �������� �����.

#define ON_REQUEST_WAR_VILLAGE_DEFENCE						(DWORD(2752))		// ��� ĳ���Ͱ� �����������̽��� ����
#define ON_RESPONSE_WAR_VILLAGE_DEFENCE						(DWORD(2753))		// ����.	

#define ON_RET_GUILD_VILLAGE_OFF_LIMITS						(DWORD(2754))		// ���� �޼����� �ϳ�.������� �־ �����Ҽ� ����.
																				// ������ ���� �޼����� EnterVillage�� ����.
//#define ON_REQUEST_WAR_VILLAGE_IN							(DWORD(2755))
//#define	ON_RESPONSE_WAR_VILLAGE_IN							(DWORD(2756))
#define ON_REQUEST_VILLAGE_DEFENCE_STOP                     (DWORD(2755))       // ���� �������̽����� ������
#define ON_RESPONSE_VILLAGE_DEFFNCE_STOP                    (DWORD(2756))       // ����.


#define ON_REQUEST_WAR_WARP									(DWORD(2757))		// ������ �� �ʿ���� ĳ���� ������.
#define ON_RESPONSE_WAR_WARP								(DWORD(2758))		// ����.

//�Ϲ� ĳ���ͷ� �����Ӵ�.
#define ON_REQUEST_NORMAL_CHAR								(DWORD(2759))		// OnReqNormalChar 
#define ON_RESPONSE_NORMAL_CHAR								(DWORD(2760))		// OnResNormalChar

#define ON_REQUEST_CHANGE_WAR                               (DWORD(2761))       //������������ ������ ��û �Ѵ�.
#define ON_RESPONSE_CHANGE_WAR                              (DWORD(2762))       //����

#define ON_CHANGE_OK                                        (DWORD(0))          //ok
#define ON_CHANGE_NOT_GUILD                                 (DWORD(1))          //��ܿ� ���ԵǾ����� �ʴ�.
#define ON_CHANGE_ENOUGH_LEVEL                              (DWORD(2))          //������ �����ϴ�. 
#define ON_CHANGE_ENOUGH_MONEY                              (DWORD(3))          //���� �����ϴ�.
#define ON_CHANGE_ENOUGH_KIND                               (DWORD(4))          //�ش� �Ӽ��� ������ ����.
#define ON_CHANGE_FAILED                                    (DWORD(5))          //�Ϲ����� ����                                

#define ON_REQUEST_CHARGE_MP                                (DWORD(2763))       //���޼����� MP�� ȸ���Ǿ��� 
#define ON_RESPONSE_CHARGE_MP                               (DWORD(2764))       //����

#define ON_RET_OK											(DWORD(0))			// ���� ���� ����  OK
#define ON_RET_NOT_ENOUGH_MONEY								(DWORD(1))			// ���� �����ϴ�.
#define ON_RET_NOT_ENOUGH_SUPPLY							(DWORD(2))			// ������ �����ϴ�.
#define ON_RET_NOT_ENOUGH_POPULATION						(DWORD(3))			// �α����� �����ϴ�.
#define ON_RET_NOT_LIMIT_POPULATION							(DWORD(4))			// ������ ������ �ִ� �ּ� �ִ� �α����� �Ѿ��.
#define ON_RET_NOT											(DWORD(5))			// �׹��� ����.


#define ON_RESPONSE_NOT_WAR                                 (DWORD(2765))       // ������ ��������.

#define ON_RESPONE_WAR_TIME                                 (DWORD(2766))       //�����ð��� �Ȱ��� �˷��ش�.

#define ON_ASK_GIVEUP                                       (DWORD(2767))       // ��濡�� �������� �Ұ����� �����.                                    
#define ON_REQUEST_GIVEUP                                   (DWORD(2768))       // ����� �������⸦ ��û�Ѵ�.  
#define ON_RESPONE_GIVEUP                                  (DWORD(2769))       // ����

#define	ON_REQUEST_ENTER_STRUCTURE							(DWORD(2800))		// ������ Ư�� �ǹ��� ����.
#define	ON_RESPONSE_ENTER_STRUCTURE							(DWORD(2801))		// ������ Ư�� �ǹ��� ����.(����)
#define	ON_REQUEST_LEAVE_STRUCTURE							(DWORD(2802))		// ���� �ǹ����� ������.
#define	ON_RESPONSE_LEAVE_STRUCTURE							(DWORD(2803))		// ���� �ǹ����� ������.(����)
//#define	ON_REQUEST_ENTER_FACTORY							(DWORD(2804))		// ���� ��� ������ ���忡 ����.
//#define	ON_RESPONSE_ENTER_FACTORY							(DWORD(2805))		// ���� ��� ������ ���忡 ����.(����)
#define	ON_REQUEST_LEAVE_FACTORY							(DWORD(2806))		// ���� ��� ������ ���忡�� ������.
#define	ON_RESPONSE_LEAVE_FACTORY							(DWORD(2807))		// ���� ��� ������ ���忡�� ������.(����)

#define ON_REQUEST_AUDIT_GUILD_INFO							(DWORD(2808))		// ��� ������ ���´�.(�䱸)
#define ON_RESPONSE_AUDIT_GUILD_INFO						(DWORD(2809))		// ��� ������ ���´�.(����)

#define ON_RET_GUILD_OK										(DWORD(0))			// ��� ���� ��� ����.
#define ON_RET_GUILD_NO_CHANGED								(DWORD(1))			// ��� ���� ��ȭ ����.
#define ON_RET_GUILD_NO_MEMBER								(DWORD(2))			// ��ܿ��� �ƴ�.

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������ ���� �Ǹ� ����..
//��������������������������������������������������������������������������������������������������������������������������������������������������������������

#define ON_RESPONSE_ITEMRECEIVED							(DWORD(2900))		// ������ ������ ������ ��ȯ 
																				// ������ ����( 2byte )
#define ON_REQUEST_ITEMLIST									(DWORD(2901))		// ������ ������ ����Ʈ�� ��û 
#define ON_RESPONSE_ITEMLIST								(DWORD(2902))		// ������ ������ ����Ʈ ��ȯ 
																				// ������ ����( 2byte ) + { ItemID( 2 byte ) + Quantity( 2byte ) + ... }
#define ON_REQUEST_TAKEOUTITEM								(DWORD(2903))		// �������� �κ��丮�� �ִ´�
																				// uiFollowerID( 1 ) + itemID( 2 ) + quantity( 2 )
#define ON_RESPONSE_TAKEOUTITEM								(DWORD(2904))		// ����(0) ����(1)


#define ON_REQUEST_SAVEITEM									(DWORD(2905))		// �������� ������ ��ũ�� �����Ѵ� 
#define ON_RESPONSE_SAVEITEM								(DWORD(2906))		// ����(0) ����(1)


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	���� ����,
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
#define	ON_RET_MARKET_OK						(DWORD(0))
#define ON_RET_MARKET_NO						(DWORD(1))
#define	ON_REQUEST_RETURNONLINE					(DWORD(3000))	// ������ �ϰ� �ٽ� �¶��� �ʵ�� ���ư���.
#define	ON_RESPONSE_RETURNONLINE				(DWORD(3001))	// ������ �ϰ� �ٽ� �¶������� ���ư����� ������ �޾Ҵ�.
#define	ON_RET_RETURNONLINE_FIELD				(DWORD(1))		// �ʵ�� ���ư���
#define	ON_RET_RETURNONLINE_VILLAGE				(DWORD(2))		// ������ ���ư���

//����������������������������������������������������������������������������
// ����Ʈ ����
//����������������������������������������������������������������������������
#define ON_REQUEST_QUEST_SELECTNPC						(DWORD(8000))					// NPC ���ý�
#define ON_RESPONSE_QUEST_SELECTNPC						(DWORD(8001))					// NPC ���ý� ����(�������� ���� ����Ʈ ��Ȳ�� ���� ������ �����ش�.)
#define ON_REQUEST_QUEST_ACCEPTQUEST					(DWORD(8002))					// NPC ����Ʈ ����
#define ON_RESPONSE_QUEST_ACCEPTQUEST					(DWORD(8003))					// NPC ����Ʈ ���� ����
#define ON_REQUEST_QUEST_CANCELQUEST					(DWORD(8004))					// NPC ����Ʈ �߰��� ���
#define ON_RESPONSE_QUEST_CANCELQUEST					(DWORD(8005))					// NPC ����Ʈ �߰��� ��� ����
#define ON_REQUEST_QUEST_GETPLAYQUEST					(DWORD(8006))					// NPC ����Ʈ �������� ����Ʈ ����Ʈ ��û
#define ON_RESPONSE_QUEST_GETPLAYQUEST					(DWORD(8007))					// NPC ����Ʈ �������� ����Ʈ ����Ʈ ��û ����
#define ON_RESPONSE_QUEST_GETPLAYQUESTDATA				(DWORD(8008))					// NPC ����Ʈ �������� ����Ʈ Data
#define ON_RESPONSE_QUEST_ALLCLEARQUEST					(DWORD(8009))					// NPC ����Ʈ ��� �ʱ�ȭ
#define	ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO		(DWORD(8010))					// actdoll (2004/03/30 10:46) : ���� ����Ʈ�� �Ϸ��� ��� ������ ���

#define ON_RET_QUEST_SELECTNPC_ERROR					(DWORD(0000))					// NPC ���ý� ����(����Ʈ ������)
#define ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST			(DWORD(0001))					// NPC ���ý� ����(����Ʈ ������)
#define ON_RET_QUEST_SELECTNPC_STARTQUEST				(DWORD(0002))					// NPC ���ý� ����(����Ʈ ����)
#define ON_RET_QUEST_SELECTNPC_PLAYINGQUEST				(DWORD(0003))					// NPC ���ý� ����(����Ʈ ������)
#define ON_RET_QUEST_SELECTNPC_ENDQUEST					(DWORD(0004))					// NPC ���ý� ����(����Ʈ ��)
#define ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST			(DWORD(0005))					// NPC ���ý� ����(����Ʈ ����)
#define ON_RET_QUEST_SELECTNPC_NOTSAMENTION				(DWORD(0006))					// NPC ���ý� ����(�ٸ� ���� �϶�)

//����������������������������������������������������������������������������
// �������� ��ġ�� ������.
//����������������������������������������������������������������������������
#define ON_REQUEST_MOVEITEM						(DWORD(4004))
#define	ON_RESPONSE_MOVEITEM					(DWORD(4005))

//����������������������������������������������������������������������������
// ��� ��� �޽���
//����������������������������������������������������������������������������
#define	ON_COMMAND								(DWORD(5000))	// ��� ��� 

// �뺴�� �߰���Ų��. (�糪�� �߰��ÿ� ���� ��� �������� �Ϲ������� �ִ� �޼���)
#define ON_ADD_FOLLOWER							(DWORD(5001))

//����������������������������������������������������������������������������
// ĳ������ ���ʽ��� ����Ͽ� �ɷ�ġ�� �ø���.
//����������������������������������������������������������������������������
#define	ON_REQUEST_INC_ABILITY_BYBONUS			(DWORD(6000))	
#define	ON_RESPONSE_INC_ABILITY_BYBONUS			(DWORD(6001))	

#define	ON_HUNGRY								(DWORD(6002))	// �������.

#define	ON_REQUEST_CHANGE_GUILD_FLAG			(DWORD(6010))	// ����� ����� �����Ѵ�.
#define	ON_RESPONSE_CHANGE_GUILD_FLAG			(DWORD(6011))	// ����� ����� �����Ѵ�.(����)
#define	ON_RESPONSE_CHANGE_GUILD_FLAG1			(DWORD(6014))	// ����� ����� �����Ѵ�.(����)
#define	ON_REQUEST_USED_FLAG_LIST				(DWORD(6012))	// ���� ����� ����� �䱸�Ѵ�.
#define	ON_RESPONSE_USED_FLAG_LIST				(DWORD(6013))	// ����� ����� ����� �䱸�Ѵ�.(����)

//����������������������������������������������������������������������������
// �ٸ� ĳ������ ��ġ�� �䱸�Ѵ�.
//����������������������������������������������������������������������������
#define	ON_REQUEST_PI_INMINIMAP					(DWORD(6100))	// �̴ϸʿ��� ������� ������ ���´�.
#define	ON_RESPONSE_PI_INMINIMAP				(DWORD(6101))	// �̴ϸʿ��� ������� ������ ���´�.(����)

#define ON_REQUEST_PERSONAL_INFO                (DWORD(6200))
#define ON_RESPONSE_PERSONAL_INFO                (DWORD(6201))

#define	ON_MYVILLAGE_ATTACKED					(DWORD(6300))	// ���� ������ ���ݴ��ϰ� �ִ�.
#define	ON_EVENT_USERRANKINGS					(DWORD(6400))	// ����ڵ��� ��ŷ.

#define	ON_REQUEST_PM_OPEN						(DWORD(7000))	// �������� ����.
#define	ON_RESPONSE_PM_OPEN						(DWORD(7001))	// �������� ����.(����)
#define	ON_REQUEST_PM_CLOSE						(DWORD(7002))	// �������� �ݴ´�.
#define	ON_RESPONSE_PM_CLOSE					(DWORD(7003))	// �������� �ݴ´�.(����)
#define	ON_REQUEST_PM_BUYITEM					(DWORD(7004))	// �����󿡰� �������� ���.
#define	ON_RESPONSE_PM_BUYITEM					(DWORD(7005))	// �����󿡰� �������� ���.(����)
#define	ON_RESPONSE_PM_BUYFOLLOWER				(DWORD(7006))	// �����󿡰� �뺴�� ���.(����)
#define	ON_REQUEST_PM_ITEMINFO					(DWORD(7007))	// �������� �Ǹ��ϰ� �ִ� �������� ������ ���´�.
#define	ON_RESPONSE_PM_ITEMINFO					(DWORD(7008))	// �������� �Ǹ��ϰ� �ִ� �������� ������ �����Ѵ�.(����)
#define	ON_REQUEST_PM_CHANGEITEM				(DWORD(7009))	// �������� �Ǹ��ϰ� �ִ� �������� ������ �����Ѵ�.
#define	ON_RESPONSE_PM_CHANGEITEM				(DWORD(7010))	// �������� �Ǹ��ϰ� �ִ� �������� ������ �����Ѵ�.
#define	ON_PM_CHANGE_BOOTH_ITEM					(DWORD(7011))	// �������� �Ǹ��ϰ� �ִ� �������� ����Ǿ���.


#define	ON_REQUEST_TRADE							(DWORD(7500))	// �ŷ��� ��û�Ѵ�.
#define	ON_RESPONSE_TRADE							(DWORD(7501))	// �ŷ��� ��û�Ѵ�.(����)
#define ON_RECEIVE_ASK_TRADE						(DWORD(7502))	// �ش� Ŭ���̾�Ʈ���� �ٸ� Ŭ���̾�Ʈ�� �ŷ��� ��û�ߴٴ� ���� �˷��ش�.
#define	ON_REQUEST_ACCEPT_TRADE						(DWORD(7503))	// �ŷ� ��û�� ���� Ŭ���̾�Ʈ�� �ŷ��� ����ߴ�.
#define ON_RESPONSE_ACCEPT_TRADE					(DWORD(7504))	// �ŷ� ��û�� ���� Ŭ���̾�Ʈ�� �ŷ��� ����ߴ�.(����)
#define	ON_REQUEST_CHANGE_TRADE_GOODS				(DWORD(7505))	// �ŷ� ���� ǰ���� �����Ѵ�.
#define	ON_RESPONSE_CHANGE_TRADE_GOODS				(DWORD(7506))	// �ŷ� ���� ǰ���� �����Ѵ�.(����)
#define	ON_REQUEST_FINISH_CHOOSE_TRADEITEM			(DWORD(7507))	// �ŷ��� ���ϴ� �������� ��� �� �����ߴ�.
#define	ON_RESPONSE_FINISH_CHOOSE_TRADEITEM			(DWORD(7508))	// �ŷ��� ���ϴ� �������� ��� �� �����ߴ�.(����)
//#define	ON_RESPONSE_FINISHED_CHOOSE_TRADEITEM		(DWORD(7509))	// �ŷ��� �ϰ� �ִ� Ŭ���̾�Ʈ�� �ŷ��� ���ϴ� �������� ��� �� �����ߴ�.
#define	ON_REQUEST_DECIDE_TRADE						(DWORD(7510))	// �ŷ��� �ϴ� ���� �����ߴ�.
#define	ON_RESPONSE_DECIDE_TRADE					(DWORD(7511))	// �ŷ��� �ϴ� ���� �����ߴ�.(����)
#define	ON_CHANGE_TRADE_GOODS						(DWORD(7512))	// �ŷ����� ǰ���� �����Ѵ�.
#define	ON_REQUEST_CANCEL_TRADE						(DWORD(7512))	// �ŷ��� ����Ѵ�.
#define	ON_RESPONSE_CANCEL_TRADE					(DWORD(7513))	// �ŷ��� ����Ѵ�.(����)
#define	ON_REQUEST_COMPLETE_TRADE_BOOK				(DWORD(7514))	// ��� �ŷ��� �Ϸ�Ǿ���.
#define	ON_RESPONSE_COMPLETE_TRADE_BOOK				(DWORD(7515))	// ��� �ŷ��� �Ϸ�Ǿ���.(����)
#define	ON_COMPLETE_TRADE							(DWORD(7516))	// �ŷ��� �Ϸ�Ǿ���.

#define	ON_REQUEST_VILLAGE_INFO						(DWORD(7600))	// Ư�� ������ ������ ��û�Ѵ�.
#define	ON_RESPONSE_VILLAGE_INFO					(DWORD(7601))	// Ư�� ������ ������ ��û�Ѵ�.(����)

#define ON_REQUEST_PARTY							(DWORD(7700))	// ��Ƽ�� ��û�Ѵ�.
#define ON_RESPONSE_PARTY							(DWORD(7701))	// ��Ƽ�� ��û�Ѵ�.(����)

#define	ON_RECEIVE_ASK_PARTY						(DWORD(7702))   // �ش� Ŭ���̾�Ʈ���� �ٸ� Ŭ���̾�Ʈ�� ��Ƽ�� ��û�ߴٴ� ���� �˷��ش�.
//#define ON_REQUEST_ASK_PARTY						ON_RECEIVE_ACK_PARTY

#define ON_REQUEST_ACCEPT_PARTY						(DWORD(7703))	// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�. �����ϴ��� ������.
#define ON_RESPONSE_ACCEPT_PARTY					(DWORD(7704))	// ��Ƽ ��û�� ���� Ŭ���̾�Ʈ�� ��Ƽ�� ���� ������ �Ѵ�.(����) ���� ����� ����.
#define ON_REQUEST_LEAVE_PARTY					    (DWORD(7705))	// Ŭ���̾�Ʈ�� ��Ƽ�� ������.(Ŭ���̾�Ʈ -> ����)
#define ON_RESPONSE_LEAVE_PARTY						(DWORD(7706))	// Ŭ���̾�Ʈ ��Ƽ�� �������� ���� �޽���
//#define ON_RESPONSE_PARTY_LIST						(DWORD(7707))	// Ŭ���̾�Ʈ�� ��Ƽ�� ������.(���� -> ��Ƽ ������)
#define	ON_REQUEST_CANCEL_REQUESTING_PARTY			(DWORD(7708))	// ���� �ߴ� ��Ƽ ��û�� ����Ѵ�.
#define	ON_RESPONSE_CANCEL_REQUESTING_PARTY			(DWORD(7709))	// ���� �ߴ� ��Ƽ ��û�� ����Ѵٴ� �޽����� ����
#define	ON_CANCEL_REQUESTED_PARTY					(DWORD(7710))	// ��Ƽ�� ��û�ߴ� �÷��̾ ��Ƽ ��û�� ����ߴ�.
#define	ON_PARTY_DELETED							(DWORD(7711))	// ��Ƽ�� ��������.
//#define	ON_PARTY_MEMBER_CHANGED						(DWORD(7713))	// ��Ƽ���� ����Ǿ���.

#define ON_RELOAD_WEAR_ITEM							(DWORD(7712))	// actdoll (2004/07/19 13:26) : = �ð��� ������ = ������ �ð� ���� Ÿ�̹�(BroadCast)

// 10000 - 10999	:	���� �ý������ΰ�(�ð���)
#define	ON_CHANGED_TIME							(DWORD(10001))
#define	ON_CHANGED_MONEY						(DWORD(10002))
#define	ON_TEXT_MSG								(DWORD(10003))
#define	ON_HEARTBEAT							(DWORD(10004))
#define	ON_OFFICIAL_ANNOUNCEMENT				(DWORD(10005))		// ���� ����
#define ON_CHANGE_WEATHER						(DWORD(10006))
#define	ON_REQUEST_RUN_PROGRAM_LIST				(DWORD(10007))
#define	ON_RESPONSE_RUN_PROGRAM_LIST			(DWORD(10008))
#define ON_MAKE_THUNDER							(DWORD(10009))		// ������ ������Ų��.
#define ON_FRIEND_LOGGEDINOUT					(DWORD(10010))		// ģ���� �α��� �ߴ�.

// Response.
#define	ON_RET_OK								(DWORD(0))
#define	ON_RET_NO								(DWORD(1))

//#define ON_RET_PARTY_FULL						(DWORD(1200))
//#define ON_RET_PARTY_DIFPARTY					(DWORD(1201))
//#define ON_RET_NO_PARTY_CAP						(DWORD(1202))

#define	ON_RET_GLD_JOIN_OK						(DWORD(1000))
#define	ON_RET_GLD_JOIN_NO						(DWORD(1001))
#define	ON_RET_GLD_LEAVE_OK						(DWORD(1002))
#define	ON_RET_GLD_LEAVE_OK_DELETEGLD			(DWORD(1003))		// ����� ������ ��尡 �����Ǿ���.
#define	ON_RET_GLD_LEAVE_NO						(DWORD(1004))
#define	ON_RET_GLD_DELETEGLD					(DWORD(1005))		// ��尡 �����Ǿ���.
#define	ON_RET_GLD_CHANGECLASS					(DWORD(1006))		// ������ ����Ǿ���.
#define	ON_RET_GLD_DISMISSAL					(DWORD(1007))		// ��ܿ��� �ذ�Ǿ���.
#define	ON_RET_GLD_ALLY							(DWORD(1008))		// ��ܿ� ������ �����ߴ�
#define	ON_RET_GLD_ENEMY						(DWORD(1009))		// ��ܿ� �������� �ߴ�.
#define	ON_RET_GLD_NEUTRAL						(DWORD(1010))		// ��ܰ� �ƹ� ���赵 �ƴϴ�.
#define	ON_RET_GLD_MINE							(DWORD(1011))		// �츮�� ����̴�.
#define ON_RET_GLD_WAR							(DWORD(1012))

#define	ON_RET_TEXTMSG_WHISPER					(DWORD(1050))		// �ӼӸ��̴�.
#define	ON_RET_TEXTMSG_GUILD					(DWORD(1051))		// ��� �޽����̴�.
#define	ON_RET_TEXTMSG_PARTYCHAT				(DWORD(1052))		// ��Ƽ ä�� �޽����̴�.

#define	ON_RET_HUR_CHANGE_HEALTH				(DWORD(2000))		// ����� �޽������� Health�� ���ҵǾ���.
#define	ON_RET_HUR_CHANGE_HP					(DWORD(2001))		// ����� �޽������� ü���� ���ҵǾ���.

#define	ON_RET_ENEMY_GUILD_VILLAGE				(DWORD(2002))		// �� ����� �����̴�.
 
#define	ON_RET_NOT_CONNECTED					(DWORD(2100))		// ���ӵǾ� ���� �ʴ�.
#define	ON_RET_ON_FIELD							(DWORD(2101))		// �ʵ� ���� �ִ�.
#define	ON_RET_IN_VILLAGE						(DWORD(2102))		// ���� �ȿ� �ִ�.
#define	ON_RET_IN_BATTLE						(DWORD(2103))		// ���� �߿� �ִ�.

#define	ON_RET_OA_TEXT							(DWORD(3000))		// �������� - �Ϲ����� �ؽ�Ʈ 
#define	ON_RET_OA_VILL_ATTACKED					(DWORD(3001))		// �������� - ������ ������ ���ϰ� �ִ�.
#define	ON_RET_OA_VILL_OCCUPIED					(DWORD(3002))		// �������� - ������ ���ɵǾ���.
#define	ON_RET_OA_MAXINVESTMENT_CHANGED			(DWORD(3003))		// �������� - �ְ� �����ڰ� ���� �Ǿ���.
#define	ON_RET_OA_GUILD_CREATED					(DWORD(3004))		// �������� - ����� ���������.
#define	ON_RET_OA_GUILD_DELETED					(DWORD(3005))		// �������� - ����� �������
#define	ON_RET_OA_PAYING_BUSINESS				(DWORD(3006))		// �������� - ������ ���� ����� ����
#define	ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE	(DWORD(3007))		// �������� - ������ �ı��Ͽ� ����� ���� �����.
#define	ON_RET_OA_SMS_MSG						(DWORD(3008))		// �������� - SMS Message
#define ON_RET_OA_VILL_SURRENDER                (DWORD(3009))       // ���� �Զ�
#define ON_RET_ON_GUILD_GIVEUP                  (DWORD(3010))       // ����� ������ ����


#define	ON_RET_SEND_TRADE_BOOK					(DWORD(4000))		// ��θ� �����Ѵ�.
#define	ON_RET_CANCEL_DECIDE_TRADE				(DWORD(4001))		// ���� ������ '����'��ư�� ������ ���� ����ߴ�.

//#define	ON_RET_CHNGEMONEY_NONE					(DWORD(5000))		// ���� ���� ���� �����ش�.
#define	ON_RET_CHNGEMONEY_PRODUCTFACILITY		(DWORD(5001))		// ���� �ǹ��� �ȷ��� ���� �޾Ҵ�.
#define	ON_RET_CHNGEMONEY_FROMADMIN				(DWORD(5002))		// ��ڿ��� ���� �޾Ҵ�.
#define	ON_RET_CHNGEMONEY_INVESTMENT			(DWORD(5003))		// ���� ������ �޾Ҵ�.

#define ON_REQUEST_VMERCENARY_DELETE			(DWORD(8100))		// ����ٴϴ� �뺴 ���� ��û
#define ON_RESPONSE_VMERCENARY_CHANGE			(DWORD(8101))		// ����ٴϴ� �뺴 ��ü ����
#define ON_RESPONSE_VMERCENARY_CHANGEMAIN		(DWORD(8102))		// ����ٴϴ� �뺴 ��ü ����(���ο��� ���� �޼���)

#define ON_REQUEST_REFINE_ITEM                  (DWORD(9201))       // ������ ���� ��û
#define ON_RESPONSE_REFINE_ITEM                 (DWORD(9202))       // ������ ���� ����

#define ON_NOTICE_ENCRYPTION_KEY				(DWORD(10555))		// ������ Ŭ���̾�Ʈ�� ��ȣŰ�� �����Ѵ�
//#define ON_RECEIVED_KEY							(DWORD(10556))		// Ŭ���̾�Ʈ�� ������ ��ȣŰ�� ���� ������ �˸���

#define ON_RET_NO_GLD_ADMIT_FULL				(DWORD(11000))		// ��ܿ��� ���� ���¶� ����� �����ڸ� �Դ� ����� �� ����.
#define ON_RET_NO_GLD_ADMIT_MAXINVESTOR			(DWORD(11001))		// ������ ������ �ֱ� ������ �����ڸ� �Դ� ����� �� ����.(�����ð��뿡)

#define ON_RESOPNSE_REAL_TIME					(DWORD(11002))




#define ON_REQUEST_MACRO_USING_USER_INFO		(DWORD(20000))		// ��ũ�� ����� ������ ������.

#endif

