#ifndef	_GSCPROTOCOL_HEADER
#define	_GSCPROTOCOL_HEADER

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 贗塭檜樹お諦曖 Щ煎饜屬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 0 - 999			:	憮幗 蕾樓 塽 啪歜 煎斜檣 婦溼.
// 1000 - 1999		:	в萄 頂縑憮...(瞪辨)		(1900~1999) 盪濠剪葬
// 2000 - 2999		:	葆擊 頂縑憮...(瞪辨)
// 3000 - 3999		:	瞪癱 頂縑憮...(瞪辨)
// 4000 - 4999		:	葆擊婁 瞪癱 奢鱔.(奢鱔)
// 5000 - 5999		:	遴艙濠 貲滄...
// 6000 - 6999		:	в萄諦 葆擊 奢鱔.(奢鱔)
// 7000 - 7499		:	謝っ渠
// 7500 - 7999		:	剪楚
// 8000 - 8999		:	蠡蝶お 婦溼
// 9000 - 9999		:   蝶鑒(9000~9100), 唳衙模(9100~9200)  婦溼 ( 雖羲 鬚餌蛔 ) か熱 NPC婦溼 渠濰除(9201~9300)
// 10000 - 10999	:	啪歜 衛蝶蠱瞳檣匙(衛除蛔)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蕾樓 剪睡
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_RESPONSE_SERVER_FULL					(DWORD(1))	// 餌辨濠陛 傘鼠 號嬴憮 蕾樓擊 剪睡и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蕾樓ж朝 贗塭檜樹お曖 謙盟蒂 掘碟и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define	ON_REQUEST_CLIENTKIND					(DWORD(2))
//#define ON_RESPONSE_CLIENTKIND					(DWORD(3))

//#define	ON_RET_CLIENT_KIND_OK					(DWORD(0))	// 薑鼻瞳檣 贗塭檜樹お棻.
//#define ON_RET_CLIENT_KIND_CLIENT				(DWORD(1))	// 棻艇 贗塭檜樹お棻.
//#define ON_RET_CLIENT_KIND_USER					(DWORD(2))	// п渡 嶸盪辨曖 贗塭檜樹お陛 嬴棲棻.
//#define	ON_RET_CLIENT_KIND_INVALIDVERSION		(DWORD(3))	// 幗瞪檜 蜃雖 彊棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蕾樓ж朝 憮幗謙盟蒂 掘碟и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_REQUEST_SERVERTYPE					(DWORD(17))
//#define ON_RESPONSE_SERVERTYPE					(DWORD(18))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 憮幗 鼻鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_REQUEST_NUMUSER					    (DWORD(19))
//#define ON_RESPONSE_NUMUSER						(DWORD(20))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 煎斜檣и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_LOGIN						(DWORD(4))	// 煎斜檣擊 蹂羶и棻.
#define ON_RESPONSE_LOGIN						(DWORD(5))	// 蹂羶и 煎斜檣縑 渠и 擬港

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 議葛攪 薑爾蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_REQUEST_CHARINFO						(DWORD(21))
//#define ON_RESPONSE_CHARINFO					(DWORD(22))

//#define ON_RET_CHARINFO_OK						(DWORD(0))	// 薑爾蒂 橢橫螃朝等 撩奢 
//#define ON_RET_CHARINFO_FAIL					(DWORD(1))	// 薑爾蒂 橢橫螃朝等 褒ぬ 

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 煎斜嬴醒и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_LOGOUT						(DWORD(6))	// 煎斜嬴醒擊 蹂羶и棻.
#define ON_RESPONSE_LOGOUT						(DWORD(7))	// 蹂羶и 煎斜嬴醒縑 渠и 擬港

#define ON_RET_LOGOUT_OK						(DWORD(0))	// 煎斜 嬴醒擊 ъ塊и棻.
//#define ON_RET_LOGOUT_FORCE						(DWORD(1))	// 鬼薯 煎斜嬴醒 衛鑑棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// GameGuard婦溼
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_CHECK_GAMEGUARD						(DWORD(40))	// Server->Client 
#define ON_REQUEST_CHECK_GAMEGUARD				(DWORD(41)) // Client->Server
#define ON_RESPONSE_CHECK_GAMEGUARD				(DWORD(42)) // Server->Client 
#define ON_USER_INFO_GAMEGUARD					(DWORD(43)) // Server->Client
// #define ON_RET_BATTLE_NO						(DWORD(11))	// 瞪癱蒂 剪睡и棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 億煎遴 議葛攪蒂 虜萇棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_REQUEST_CHENTRY						(DWORD(10))
//#define ON_RESPONSE_CHENTRY						(DWORD(11))

//#define ON_RET_CHENTRY_OK						(DWORD(0))	// 陛殮擊 ъ塊и棻.
//#define ON_RET_CHENTRY_ID						(DWORD(1))	// 偽擎 ID蒂 噙朝 餌塋檜 襄營и棻.
//#define ON_RET_CHENTRY_CHAR						(DWORD(2))	// 檜嘐 3貲檜  棻 獺棻.
//#define ON_RET_CHENTRY_NO						(DWORD(3))	// 陛殮 籀葬蒂 й 熱 橈棻.
//#define ON_RET_CHENTRY_ABILITY					(DWORD(4))	// 棟溘纂縑 檜鼻檜 氈棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 議葛攪蒂 餉薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_REQUEST_DELCHAR						(DWORD(12))
//#define ON_RESPONSE_DELCHAR						(DWORD(13))

//#define ON_RET_DELCHAR_OK						(DWORD(0))	// 陛殮擊 ъ塊и棻.
//#define ON_RET_DELCHAR_NO						(DWORD(1))	// 陛殮 籀葬蒂 й 熱 橈棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 憲葡 爾萄蒂 蹂羶и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_REQUEST_MESSAGE						(DWORD(15))
//#define ON_RESPONSE_MESSAGE						(DWORD(16))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫蒂 裘鼻縑憮 嬪纂蒂 檜翕衛鑑棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_POSITION						(DWORD(1005))		// 贗塭檜樹お縑憮 ⑷營 輿檣奢 議葛攪陛 嬪纂и 顫橾曖 嬪纂蒂 憲溥遽棻.
#define ON_RESPONSE_POSITION					(DWORD(1006))		// 憮幗縑憮 贗塭檜樹お縑啪 檜翕и 議葛攪菟曖 嬪纂蒂 憲溥遽棻.

//#define ON_RET_POSITION_OK						(DWORD(0))	// 檜翕擊 ъ塊и棻.
//#define ON_RET_POSITION_NO						(DWORD(1))	// 檜翕旎雖
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 瞪癱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_BATTLE						(DWORD(1007))	// 瞪癱蒂 蹂羶и棻. 
#define ON_RESPONSE_BATTLE						(DWORD(1008))	// 蹂羶и 瞪癱曖 渠и 擬港
#define ON_RESPONSE_BATTLEPLAYER				(DWORD(1100))	// 瞪癱蒂 и棻堅 棻艇 餌辨濠縑啪 憲葡

#define ON_RET_BATTLE_WAIT						(DWORD(0))	// 瞪癱 渠晦鼻鷓煎 菟橫除棻.
#define ON_RET_BATTLE_BEGIN						(DWORD(1))	// 瞪癱蒂 衛濛и棻.
#define ON_RET_BATTLE_NO						(DWORD(2))	// 瞪癱蒂 剪睡и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 檜翕衛 и還曖 薑爾蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_RESPONSE_INFO_LINE					(DWORD(1009))

//#define ON_RET_NEW_PLAYERS						(DWORD(0))	// 億煎遴 Ы溯檜橫菟擊 爾魚棻.
//#define ON_RET_NEW_ITEM							(DWORD(1))	// 億嫦唯脹 嬴檜蠱菟擊 爾魚棻.
//#define ON_RET_RELEASE_PLAYERS					(DWORD(2))	// 餉薯腆 Ы溯檜橫菟擊 爾魚棻.
//#define ON_RET_RELEASE_ONE_PLAYER				(DWORD(3))	// 餉薯腆 Ы溯檜橫蒂 爾魚棻.
#define ON_RET_BATTLE_NO						(DWORD(2))	// 瞪癱蒂 剪睡и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 薑爾蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_PLAYER_INFO					(DWORD(1010))	// Ы溯檜橫曖 薑爾蒂 蹂羶и棻.
#define	ON_RESPONSE_PLAYER_INFO					(DWORD(1011))	// Ы溯檜橫曖 薑爾蒂 蹂羶ц擊陽曖 擬港擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 裘縑 嬴檜蠱擊 鄹堅 幗萼棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_THROWITEM					(DWORD(1012))
#define ON_RESPONSE_THROWITEM					(DWORD(1013))
#define ON_REQUEST_PICKUPITEM					(DWORD(1014))
#define ON_RESPONSE_PICKUPITEM					(DWORD(1015))
#define	ON_CHANGE_FIELDITEM						(DWORD(1022))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 檜翕й陽 頂 輿嬪縑憮 億煎 儅晦朝 議葛攪釭 橈橫雖朝 議葛攪.
#define	ON_NEWINFOONMOVE						(DWORD(1016))
#define	ON_ADDCHAR								(DWORD(1017))
#define	ON_DELCHAR								(DWORD(1018))
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define	ON_RESPONSE_AOURNDCHARINFO				(DWORD(1019))	// 輿嬪 議葛攪菟曖 渠и 薑爾.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 議葛攪曖 鼻撮и 薑爾蒂 橢橫魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_REQUEST_CHAR_DETAILINFO				(DWORD(1020))	// 議葛攪曖 鼻撮 薑爾 蹂羶.
#define	ON_RESPONSE_CHAR_DETAILINFO				(DWORD(1021))	// 議葛攪曖 鼻撮 薑爾 蹂羶.(擬港)

#define	ON_REQUEST_MOVEMAP						(DWORD(1030))	// 裘擊 檜翕衛縑 詭衛雖.
#define	ON_RESPONSE_MOVEMAP						(DWORD(1031))	// 裘擊 檜翕衛縑 詭衛雖.(擬港)


// 寡 婦溼
#define	ON_BOARDSHIP							(DWORD(1040))	// 寡陛 轎ои棻.
#define	ON_GETOUTSHIP							(DWORD(1041))	// 寡縑憮 頂萼棻.

// 濰睡婦溼 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define	ON_REQUEST_AUDIT_TOTAL					(DWORD(1500))	// 濰睡 識營骯 薑爾
#define	ON_RESPONSE_AUDIT_TOTAL					(DWORD(1501))	// 濰睡 識營骯 薑爾 (擬港)
#define	ON_REQUEST_AUDIT_ACCOUNT				(DWORD(1502))	// 濰睡 啗謝跡煙 薑爾
#define	ON_RESPONSE_AUDIT_ACCOUNT				(DWORD(1503))	// 濰睡 啗謝跡煙 薑爾(擬港)
#define	ON_REQUEST_AUDIT_OWN					(DWORD(1504))	// 濰睡 睡翕骯爾嶸 薑爾
#define	ON_RESPONSE_AUDIT_OWN					(DWORD(1505))	// 濰睡 睡翕骯爾嶸 薑爾(擬港)
#define	ON_REQUEST_AUDIT_VILLAGE_LIST			(DWORD(1506))	// 濰睡 癱濠и 葆擊 葬蝶お
#define	ON_RESPONSE_AUDIT_VILLAGE_LIST			(DWORD(1507))	// 濰睡 癱濠и 葆擊 葬蝶お(擬港)
#define	ON_REQUEST_AUDIT_VILLAGE_INFO			(DWORD(1508))	// 濰睡 癱濠и и 葆擊 薑爾
#define	ON_RESPONSE_AUDIT_VILLAGE_INFO			(DWORD(1509))	// 濰睡 癱濠и и 葆擊 薑爾(擬港)
#define	ON_REQUEST_AUDIT_STATUS					(DWORD(1510))	// 濰睡 鼠濰 薑爾
#define	ON_RESPONSE_AUDIT_STATUS				(DWORD(1511))	// 濰睡 鼠濰 薑爾(擬港)
#define	ON_REQUEST_AUDIT_SETNAME				(DWORD(1512))	// 濰睡 檜葷 殮溘
#define	ON_RESPONSE_AUDIT_SETNAME				(DWORD(1513))	// 濰睡 檜葷 殮溘(擬港)
#define ON_REQUEST_AUDIT_FRIEND_LIST			(DWORD(1514))	// 濰睡 耀掘 葬蝶お
#define ON_RESPONSE_AUDIT_FRIEND_LIST			(DWORD(1515))	// 濰睡 耀掘 葬蝶お(擬港)
#define ON_REQUEST_AUDIT_FRIEND_SET_DENIAL		(DWORD(1516))	// 濰睡 耀掘 熱褐剪睡
#define ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL		(DWORD(1517))	// 濰睡 耀掘 熱褐剪睡(擬港)
#define ON_REQUEST_AUDIT_FRIEND_ADD_ONE			(DWORD(1518))	// 濰睡 耀掘 蹺陛
#define ON_RESPONSE_AUDIT_FRIEND_ADD_ONE		(DWORD(1519))	// 濰睡 耀掘 蹺陛(擬港)
#define ON_REQUEST_AUDIT_FRIEND_REMOVE_SOME		(DWORD(1520))	// 濰睡 耀掘 и貲 餉薯
#define ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME	(DWORD(1521))	// 濰睡 耀掘 и貲 餉薯(擬港)
#define ON_REQUEST_AUDIT_FRIEND_REMOVE_ALL		(DWORD(1522))	// 濰睡 耀掘 瞪羹 餉薯
#define ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL		(DWORD(1523))	// 濰睡 耀掘 瞪羹 餉薯

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define	ON_REQUEST_AUDIT_PROFIT_STATISTICS		(DWORD(1524))	// 濰睡 葆擊 熱櫛 頂羲
#define ON_RESPONSE_AUDIT_PROFIT_STATISTICS		(DWORD(1525))	// 濰睡 葆擊 熱櫛 頂羲(擬港)

#define	ON_RET_AUDIT_OK							(DWORD(0))		// 濰睡縑憮 橾奩瞳檣 撩奢.
#define	ON_RET_AUDIT_NO							(DWORD(1))		// 濰睡縑憮 橾奩瞳檣 褒ぬ.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 嬴檜蠱擊 雜辨, п薯 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_WEARITEM_INFO				(DWORD(1550))			//	雜辨и 嬴檜蠱 薑爾
#define ON_RESPONSE_WEARITEM_INFO				(DWORD(1551))			//	雜辨и 嬴檜蠱 薑爾(擬港)
#define ON_REQUEST_WEARITEM						(DWORD(1552))			//	嬴檜蠱擊 雜辨и棻.
#define ON_RESPONSE_WEARITEM					(DWORD(1553))			//	嬴檜蠱擊 雜辨и棻.(擬港)
#define ON_REQUEST_STRIPITEM					(DWORD(1554))			//	嬴檜蠱擊 п薯и棻.
#define ON_RESPONSE_STRIPITEM					(DWORD(1555))			//	嬴檜蠱擊 п薯и棻.(擬港)

#define ON_CHANGE_CLOTHES						(DWORD(1556))		    // 輿滲餌塋曖 嬴檜蠱 雜辨и啪 夥票擊陽.

#define ON_CHANGE_CHARACTERCODE					(DWORD(1557))		    // 輿滲餌塋曖 議葛攪 囀萄陛 夥票擊陽.
//#define ON_CHANGE_CHARACTERNATION				(DWORD(1558))		    // 措瞳檜 夥船歷擊陽.
//OnChangeClothesMsg


#define ON_RET_WEARITEM_OK						(DWORD(0))				//	瞪奩瞳檣 OK
#define ON_RET_WEARITEM_NO						(DWORD(1))				//	瞪奩瞳檣 NO

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 瞪癱賅萄, ゎ�飛藒� 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_SET_ATTACK_MODE				(DWORD(1560))			// 瞪癱賅萄 蹂羶
#define ON_RESPONSE_SET_ATTACK_MODE				(DWORD(1561))			// 瞪癱賅萄 蹂羶縑 渠и 擬港

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 跨蝶攪陛 濠褐擊 喻葬剪釭, 慇橫陬擊陽
#define ON_TARGETME								(DWORD(1570))	// 跪檜 濠褐擊 喻萵陽.
#define ON_RELEASEME							(DWORD(1571))	// 顫啃縑憮 慇橫陳陽.
// 檜賅じ夔 ル衛
#define ON_REQUEST_IMOTICON						(DWORD(1572))	// 檜賅じ夔 ル薑擊 ц擊陽.
#define ON_RESPONSE_IMOTICON					(DWORD(1573))
// 嬴檜蠱 模賅
#define ON_REQUEST_SPENDITEM					(DWORD(1574))
#define ON_RESPONSE_SPENDITEM					(DWORD(1575))
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊寰頂僥 婦溼 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_REQUEST_WRITE_DESCRIPT				(DWORD(1600))	// 寰頂僥 噹棻朝 詭撮雖(葆擊)
#define	ON_RESPONSE_WRITE_DESCRIPT				(DWORD(1601))	// 寰頂僥 噹棻朝 詭撮雖(擬港)
#define	ON_REQUEST_READ_DESCRIPT				(DWORD(1602))	// 寰頂僥 檗朝 詭撮雖(в萄)
#define	ON_RESPONSE_READ_DESCRIPT				(DWORD(1603))	// 寰頂僥 檗朝 詭撮雖(擬港)

#define ON_RET_DESCRIPT_OK						(DWORD(0))
#define ON_RET_DESCRIPT_NO						(DWORD(1))
#define ON_RET_DESCRIPT_NOTBEST					(DWORD(2))

#define ON_REQUEST_VILLAGE_NATION				(DWORD(1604))
#define ON_RESPONSE_VILLAGE_NATION				(DWORD(1605))

//OnReqVillageGetNation

// 錶Щ 婦溼
#define	ON_REQUEST_WARP							(DWORD(1800))	// 錶Щ
#define	ON_RESPONSE_WARP						(DWORD(1801))	// 錶Щ(擬港)

// 盪濠 剪葬
#define	ON_REQUEST_CHARGEBOOTH					(DWORD(1900))	// 謝っ渠 菟橫陛晦
#define	ON_RESPONSE_CHARGEBOOTH					(DWORD(1901))	// 謝っ渠 菟橫陛晦(擬港)
#define ON_REQUEST_DISPLAYBOOTH					(DWORD(1902))	// 謝っ渠曖 嬴檜蠱菟 爾罹輿晦
#define ON_RESPONSE_DISPLAYBOOTH				(DWORD(1903))
//#define ON_REQUEST_INPUTBOOTH_DESC				(DWORD(1904))	// 謝っ渠 僭勒 薑爾蒂 殮溘
#define ON_RESPONSE_INPUTBOOTH_DESC				(DWORD(1905))
#define ON_REQUEST_GETBOOTHINFO					(DWORD(1906))	// 謝っ渠 僭勒 薑爾蒂 橢橫褥
#define ON_RESPONSE_GETBOOTHINFO				(DWORD(1907))
#define	ON_REQUEST_LEAVEBOOTH					(DWORD(1908))	// 謝っ渠 釭陛晦
#define	ON_RESPONSE_LEAVEBOOTH					(DWORD(1909))	// 謝っ渠 釭陛晦(擬港)
#define	ON_REQUEST_SETBOOTHITEM					(DWORD(1910))	// 謝っ渠 僭勒 場晦
#define	ON_RESPONSE_SETBOOTHITEM				(DWORD(1911))	// 謝っ渠 釭陛晦
#define	ON_REQUEST_CANCELBOOTHITEM				(DWORD(1912))	// 謝っ渠 堡湍 僭勒 鏃模
#define	ON_RESPONSE_CANCELBOOTHITEM				(DWORD(1913))	// 謝っ渠 堡湍 僭勒 鏃模(擬港)
//#define	ON_REQUEST_BUYBOOTHITEM					(DWORD(1914))	// 謝っ渠 堡湍 僭勒 掘殮
#define	ON_RESPONSE_BUYBOOTHITEM				(DWORD(1915))	// 謝っ渠 堡湍 僭勒 掘殮(擬港)
#define	ON_RESPONSE_SELLBOOTHITEM				(DWORD(1916))	// 僭勒檜 つ葬賊 輿檣檜 嫡朝 詭撮雖

#define ON_RET_MARKETPLACE_OK					(DWORD(0))
#define ON_RET_MARKETPLACE_NO					(DWORD(1))
//#define ON_RET_MARKETPLACE_FULL					(DWORD(2))		// 粕雙 鼻鷓, 渦檜鼻 й 熱 橈棻.
#define	ON_RET_MARKETPLACE_EMPTY				(DWORD(3))		// 綴鼻鷓, 輿檣檜 橈棻.
#define	ON_RET_MARKETPLACE_PLAYER				(DWORD(4))		// 棻艇 餌辨濠陛 餌辨醞


#define	ON_REQUEST_ATTACK_VILLAGE				(DWORD(1980))	// 葆擊擊 奢問и棻.
#define	ON_RESPONSE_ATTACK_VILLAGE				(DWORD(1981))	// 葆擊擊 奢問и棻.(擬港)

#define	ON_CHANGED_VILLAGE_DEFENCE				(DWORD(1990))	// 葆擊曖 寞橫溘檜 滲唳腎歷棻.
// 葆擊婦溼 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_ENTER_VILLAGE				(DWORD(2000))	// 葆擊縑 菟橫除棻.
#define ON_RESPONSE_ENTER_VILLAGE				(DWORD(2001))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_LEAVE_VILLAGE				(DWORD(2002))	// 葆擊縑憮 集陪棻.
#define ON_RESPONSE_LEAVE_VILLAGE				(DWORD(2003))


#define ON_RET_LEAVE_VILLAGE_OK					(DWORD(0))	// ъ陛и棻.
#define ON_RET_LEAVE_VILLAGE_NO					(DWORD(1))	// 剪睡и棻.
//#define ON_RET_LEAVE_VILLAGE_PLAYER				(DWORD(2))	// 棻艇Ы溯檜橫陛 葆擊縑憮 集陬棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_LIST_ITEM					(DWORD(2004))	// 衛濰縑憮 僭ヶ 葬蝶お 蹂羶5
#define ON_RESPONSE_LIST_ITEM					(DWORD(2005))
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_BUY_ITEM						(DWORD(2006))	// 僭勒擊 骯棻.
#define ON_RESPONSE_BUY_ITEM					(DWORD(2007))

#define ON_RET_NOITEM							(DWORD(1))	// つ 僭勒檜 橈擊唳辦
#define ON_RET_NOMONEY							(DWORD(2))	// Ы溯檜橫曖 絲檜 賅濠蒂 唳辦
#define	ON_RET_NOSALE							(DWORD(3))	// っ衙ж雖 彊朝 嬴檜蠱檜棻.
#define	ON_RET_BUYZERO							(DWORD(4))	// 0偃蒂 餌溥堅 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_SELL_ITEM					(DWORD(2008))	// 僭勒擊 っ棻.
#define ON_RESPONSE_SELL_ITEM					(DWORD(2009))
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_RESPONSE_CHANGED_ITEMINFO			(DWORD(2012))	// 棻艇 Ы溯檜橫縑曖п 衛濰曖 嬴檜蠱葬蝶お縑 滲翕檜 氈擊唳辦
																// 斜 滲翕脹 嬴檜蠱菟曖 薑爾蒂 瞪殖и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊, 瞪濰縑憮曖 詭衛雖
#define	ON_REQUEST_BANK_INFO					(DWORD(2016))	// 擎ч 薑爾 蹂掘.
#define	ON_RESPONSE_BANK_INFO					(DWORD(2017))
#define	ON_REQUEST_BANK_DEPOSIT					(DWORD(2018))	// 蕨旎
#define ON_RESPONSE_BANK_DEPOSIT				(DWORD(2019))
#define	ON_REQUEST_BANK_DRAWING					(DWORD(2020))	// 轎旎
#define ON_RESPONSE_BANK_DRAWING				(DWORD(2021))
#define	ON_REQUEST_BANK_OPENSAVINGACCOUNT		(DWORD(2022))	// 啗謝蒂 虜萇棻堅 蹂羶и棻.
#define	ON_RESPONSE_BANK_OPENSAVINGACCOUNT		(DWORD(2023))	// 啗謝蒂 虜萇棻堅 蹂羶ц擊陽曖擬港
#define	ON_REQUEST_BANK_BANKBOOK_LIST			(DWORD(2024))	// 鱔濰曖 葬蝶お蒂 殖塭堅 蹂羶и棻.
#define	ON_RESPONSE_BANK_BANKBOOK_LIST			(DWORD(2025))	// 鱔濰曖 葬蝶お蒂 殖塭堅 ц擊陽曖 擬港
#define	ON_REQUEST_BANK_BANKBOOK_LISTCOUNT		(DWORD(2026))	// 鱔濰曖 葬蝶お曖 熱蒂 殖塭堅 蹂羶и棻.

#define ON_REQUEST_BANK_SAVE                    (DWORD(2600))	// 殮旎 蹂羶
#define ON_RESPONSE_BANK_SAVE                   (DWORD(2601))	// 殮旎 擬港
#define ON_REQUEST_BANK_WITHDRAW                (DWORD(2602))	// 轎旎 蹂羶
#define ON_RESPONSE_BANK_WITHDRAW               (DWORD(2603))	// 轎旎 擬港


#define	ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT		(DWORD(2499))	// 鱔濰曖 葬蝶お曖 熱蒂 殖塭堅 ц擊陽曖 擬港

#define	ON_REQUEST_BANK_COLLATERAL				(DWORD(2500))	// 氬爾蒂 蜆曹棻堅 蹂羶и棻.
#define	ON_RESPONSE_BANK_COLLATERAL				(DWORD(2501))	// 氬爾蒂 蜆曹棻堅 蹂羶ц擊陽曖 擬港
#define	ON_REQUEST_BANK_COLLATERALLIST			(DWORD(2502))	// 氬爾List蒂 蹂羶и棻.
#define	ON_RESPONSE_BANK_COLLATERALLIST			(DWORD(2503))	// 氬爾List蒂 蹂羶ц擊陽曖 擬港
#define ON_REQUEST_BANK_TAKECOLLATERAL			(DWORD(2504))	// 氬爾蒂 給溥殖塭堅 蹂羶и棻.
#define ON_RESPONSE_BANK_TAKECOLLATERAL			(DWORD(2505))	// 氬爾蒂 給溥殖塭堅 蹂羶 ц擊陽曖 擬港.
//#define ON_REQUEST_BANK_MAXCREDITLOAN			(DWORD(2506))	//	褐辨氬爾 譆渠 旎擋
#define ON_RESPONSE_BANK_MAXCREDITLOAN			(DWORD(2507))



#define	ON_RET_BANK_OK							(DWORD(0))		// 瞪奩瞳檣 OK
#define	ON_RET_BANK_NO							(DWORD(1))		// 瞪夢瞪檣 NO
#define	ON_RET_BANK_EXISTOPENSAVINGACCOUNT		(DWORD(3))		// п渡 啗謝陛 檜嘐 氈棻.
#define	ON_RET_BANK_FULLSAVINGACCOUNT			(DWORD(4))		// 渦 檜鼻曖 п渡 啗謝蒂 虜菟 熱陛 橈棻.(⑷營 虜菟溥堅 ж朝 啗謝縑 ип憮)
#define	ON_RET_BANK_FULLTOTALSAVINGACCOUNT		(DWORD(5))		// 渦 檜鼻曖 п渡 啗謝蒂 虜菟 熱陛 橈棻.(瞪羹 虜萇 啗謝縑 ип憮.)
//#define	ON_RET_BANK_MAXMONEY					(DWORD(6))		// и啗 擋熱蒂 剩歷棻.
//#define	ON_RET_BANK_MINMONEY					(DWORD(7))		// 譆模 擋熱爾棻 濛棻.


// 葆擊, 堯濰縑憮曖 詭衛雖
#define ON_REQUEST_FARM_INFO					(DWORD(2530))	//	堯濰 薑爾蒂 蹂羶л.
#define	ON_RESPONSE_FARM_INFO					(DWORD(2531))
#define ON_REQUEST_FARM_BUY						(DWORD(2532))	//	堯濰擊 掘殮и棻堅 蹂羶л.
#define ON_RESPONSE_FARM_BUY					(DWORD(2533))
#define ON_REQUEST_FARM_SLOTINFO				(DWORD(2534))	//	蝸煜 薑爾蒂 蹂羶л.
#define ON_RESPONSE_FARM_SLOTINFO				(DWORD(2535))
//#define ON_REQUEST_FARM_STRUCINFO				(DWORD(2536))	//	勒僭 薑爾蒂 蹂羶л.
//#define ON_RESPONSE_FARM_STRUCINFO				(DWORD(2537))
#define ON_REQUEST_FARM_BUILDSTRUCT				(DWORD(2538))	//	勒僭擊 騁朝棻堅 蹂羶л.
#define ON_RESPONSE_FARM_BUILDSTRUCT			(DWORD(2539))
#define ON_REQUEST_FARM_EXPANDSLOT				(DWORD(2540))	//	蝸煜 衛撲 �挫� и棻堅 蹂羶.
#define ON_RESPONSE_FARM_EXPANDSLOT				(DWORD(2541))
#define ON_REQUEST_FARM_DESTROYSTRUCT			(DWORD(2542))	//	勒僭擊 繹剪и棻堅 蹂羶л.
#define ON_RESPONSE_FARM_DESTROYSTRUCT			(DWORD(2543))
#define ON_REQUEST_FARM_SETEQUIPMENT			(DWORD(2544))	//	勒僭縑 睡熱 衛撲僭擊 騁朝棻堅 蹂羶л.
#define ON_RESPONSE_FARM_SETEQUIPMENT			(DWORD(2545))
#define ON_REQUEST_FARM_ITEMPRODUCT				(DWORD(2546))	//	堯濰縑憮 嬴檜蠱擊 儅骯й 匙擊 蹂羶и棻.
#define ON_RESPONSE_FARM_ITEMPRODUCT			(DWORD(2547))
#define ON_REQUEST_FARM_WORK					(DWORD(2548))	//	堯濰縑憮 橾擊 и棻堅 蹂羶и棻.
#define ON_RESPONSE_FARM_WORK					(DWORD(2549))
#define ON_REQUEST_FARM_WORKINFO				(DWORD(2550))	//	堯濰曖 橾羞 薑爾蒂 爾魚棻.
#define ON_RESPONSE_FARM_WORKINFO				(DWORD(2551))
#define ON_REQUEST_FARM_UPGRADESLOT				(DWORD(2552))	//	蝸煜擊 機斜溯檜萄и棻.
#define ON_RESPONSE_FARM_UPGRADESLOT			(DWORD(2553))
#define ON_REQUEST_FARM_WORKSTART				(DWORD(2554))	//	堯濰縑憮 橾擊 ж溥 и棻.
#define ON_RESPONSE_FARM_WORKSTART				(DWORD(2555))
//#define ON_REQUEST_FARM_SELL					(DWORD(2556))	//	堯濰擊 っ棻.
//#define ON_RESPONSE_FARM_SELL					(DWORD(2557))
#define	ON_FARMWORK_COMPLETED					(DWORD(2558))	// ⑷營 堯濰縑憮 濛機菟檜 諫猿腎歷棻.
#define ON_REQUEST_FARM_WORK_HACK				(DWORD(2559))


// 璽堅
#define ON_REQUEST_STORAGE_INFO					(DWORD(2580))	//	璽堅曖 嬴檜蠱擊 橢橫螞棻.
#define ON_RESPONSE_STORAGE_INFO				(DWORD(2581))
#define ON_REQUEST_STORAGE_INPUTITEM			(DWORD(2582))	//	璽堅縑 嬴檜蠱擊 厥朝棻.
#define ON_RESPONSE_STORAGE_INPUTITEM			(DWORD(2583))
#define ON_REQUEST_STORAGE_OUTPUTITEM			(DWORD(2584))	//	璽堅縑憮 嬴檜蠱擊 賓棻.
#define ON_RESPONSE_STORAGE_OUTPUTITEM			(DWORD(2585))

#define ON_REQUEST_PLANT_CHANGE_PAY				(DWORD(2590))	//	歜旎擊 夥羚陽 詭撮雖
#define ON_RESPONSE_PLANT_CHANGE_PAY			(DWORD(2591))
#define ON_REQUEST_PLANT_CANCEL_PLAN			(DWORD(2592))	//	歜旎擊 夥羚陽 詭撮雖
#define ON_RESPONSE_PLANT_CANCEL_PLAN			(DWORD(2593))

// 璽堅 塽 堯濰, 跡濰, 惜骯, 奢濰 縑憮曖 唸婁 高.
//#define ON_RET_FARM_OWNER						(DWORD(2))		//	堯濰 模嶸輿曖 濠問檜棻.
//#define	ON_RET_FARM_WORKER						(DWORD(3))		//	堯濰 橾羞曖 濠問檜棻.
#define	ON_RET_FARM_MATERIAL					(DWORD(4))		//	營猿陛 睡褶ж棻.
#define	ON_RET_FARM_TOOL						(DWORD(5))		//	紫掘陛 橈棻.
#define	ON_RET_FARM_MONEY						(DWORD(6))		//	絲檜 睡褶ж棻.
#define	ON_RET_FARM_NOTOWNER					(DWORD(7))		//	堯濰 輿檣檜 嬴棲晦陽僥縑 й 熱 橈棻.
#define	ON_RET_FARM_ITEMERROR					(DWORD(8))		//	斜楛 嬴檜蠱擎 儅骯й 熱 橈棻.
//#define	ON_RET_FARM_FULLUPGRAGE					(DWORD(9))		//	渦檜鼻 機斜溯檜萄 й 熱 橈棻.

// 葆擊, 輿薄縑憮曖 詭衛雖.
#define	ON_REQUEST_ENTER_INN					(DWORD(2027))	// 葆擊 輿薄縑 菟橫除棻.
#define	ON_RESPONSE_ENTER_INN					(DWORD(2028))	// 葆擊 輿薄縑 菟橫除棻.(擬港)
#define	ON_REQUEST_LEAVE_INN					(DWORD(2029))	// 葆擊 輿薄縑憮 釭除棻
#define	ON_RESPONSE_LEAVE_INN					(DWORD(2030))	// 葆擊 輿薄縑憮 釭除棻.(擬港)


#define	ON_REQUEST_ENTER_VWR					(DWORD(2031))	// 葆擊 輿薄, 渠晦褒縑 菟橫除棻.							
#define	ON_RESPONSE_ENTER_VWR					(DWORD(2032))	// 葆擊 輿薄, 渠晦褒縑 菟橫除棻.(擬港)
#define	ON_REQUEST_VIL_INN_JOIN_CHAT_ROOM		(DWORD(2033))	// 葆擊 輿薄, 渠�食瞈� 菟橫除棻.
#define	ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM		(DWORD(2034))	// 葆擊 輿薄, 渠�食瞈� 菟橫除棻.(擬港)
#define	ON_REQUEST_INN_CREATECHATROOM			(DWORD(2035))	// 葆擊 輿薄, 渠�食磍� 虜萇棻.
#define	ON_RESPONSE_INN_CREATECHATROOM			(DWORD(2036))	// 葆擊 輿薄, 渠�食磍� 虜萇棻.(擬港)
#define	ON_WAITROOM_CHANGE_INFO					(DWORD(2037))	// 葆擊 輿薄, 渠晦褒縑 滲唳脹 薑爾蒂 爾頂遽棻.
#define	ON_RET_INN_OK							(DWORD(0))
#define ON_RET_INN_NO							(DWORD(1))
#define	ON_VWR_CREATEROOM						(DWORD(0))		// 億煎遴 寞檜 偃撲腎歷棻.
#define	ON_VWR_DELETEROOM						(DWORD(1))		// 晦襄曖 寞檜 わ潑 腎歷棻.
#define	ON_VWR_CHANGEPLAYERNUM					(DWORD(2))		// 晦襄曖 寞縑 Ы溯檜橫 熱陛 滲唳腎歷棻.

#define	ON_REQUEST_VIL_INN_LEAVE_CHAT_ROOM		(DWORD(2038))	// 葆擊 輿薄, 渠�食瞈□� 釭除棻.
#define	ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM		(DWORD(2039))	// 葆擊 輿薄, 渠�食瞈□� 釭除棻.(擬港)
	
#define	ON_VIL_INN_JOIN_PLAYER					(DWORD(2040))	// 葆擊 輿薄, Ы溯檜橫陛 蹺陛腎歷棻.
#define	ON_VIL_INN_LEAVE_PLAYER					(DWORD(2041))	// 葆擊 輿薄, Ы溯檜橫陛 餉薯腎歷棻.

#define	ON_VIL_INN_CHATTING						(DWORD(2042))	// 葆擊 輿薄, 渠�食瞈□� 瓣た

#define	ON_REQUEST_ENTER_MARKET					(DWORD(2050))	// 衛濰縑 菟橫除棻.
#define	ON_RESPONSE_ENTER_MARKET				(DWORD(2051))	// 衛濰縑 菟橫除棻.(擬港)
#define	ON_REQUEST_LEAVE_MARKET					(DWORD(2052))	// 衛濰縑憮 釭螞棻.
#define	ON_RESPONSE_LEAVE_MARKET				(DWORD(2053))	// 衛濰縑憮 釭螞棻.(擬港)

#define	ON_REQUEST_ENTER_NOTICEBOARD			(DWORD(2060))	// 啪衛っ縑 菟橫除棻.
#define	ON_RESPONSE_ENTER_NOTICEBOARD			(DWORD(2061))	// 啪衛っ縑憮 釭除棻.(擬港)
#define	ON_REQUEST_NOTICEBOARD_NOTICELIST		(DWORD(2062))	// 啪衛っ縑憮 啪衛僭 葬蝶お蒂 蹂羶и棻.
#define	ON_RESPONSE_NOTICEBOARD_NOTICELIST		(DWORD(2063))	// 啪衛っ縑憮 啪衛僭 葬蝶お蒂 爾魚棻.
#define	ON_REQUEST_REGIST_NOTICE				(DWORD(2064))	// 啪衛っ縑憮 旋擊 蛔煙и棻.
#define	ON_RESPONSE_REGIST_NOTICE				(DWORD(2065))	// 啪衛っ縑憮 旋擊 蛔煙и棻.(擬港)
#define	ON_REQUEST_DELETE_NOTICE				(DWORD(2066))	// 啪衛っ曖 旋擊 餉薯и棻.
#define	ON_RESPONSE_DELETE_NOTICE				(DWORD(2067))	// 啪衛っ曖 旋擊 餉薯и棻.(擬港)
#define	ON_REQUEST_VIEW_NOTICE					(DWORD(2070))	// 啪衛っ曖 旋擊 獄棻.
#define	ON_RESPONSE_VIEW_NOTICE					(DWORD(2071))	// 啪衛っ曖 旋擊 獄棻.(擬港)
#define ON_REQUEST_MODIFY_NOTICE				(DWORD(2072))	// 啪衛っ曖 旋擊 熱薑и棻.
#define ON_RESPONSE_MODIFY_NOTICE				(DWORD(2073))	// 啪衛っ曖 旋擊 熱薑и棻.

#define ON_REQUEST_BARRACK_GETSOLDERLIST			(DWORD(2090))	// 辨煽菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_BARRACK_GETSOLDERLIST			(DWORD(2091))	// 辨煽菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_BARRACK_BUYSOLDER				(DWORD(2092))	// 辨煽擊 堅辨и棻.
#define ON_RESPONSE_BARRACK_BUYSOLDER				(DWORD(2093))	// 辨煽擊 堅辨縑 擬港и棻.
#define ON_REQUEST_BARRACK_GETDISMISSALSOLDERLIST	(DWORD(2094))	// п堅й 辨煽菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST	(DWORD(2095))	// п堅й 辨煽菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_BARRACK_DISMISSALSOLDER			(DWORD(2096))	// 辨煽擊 п堅и棻.
#define ON_RESPONSE_BARRACK_DISMISSALSOLDER			(DWORD(2097))	// 辨煽擊 п堅縑 擬港и棻.

#define ON_REQUEST_GBARRACK_GETSOLDERLIST			(DWORD(2130))	// 濰熱辨煽菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_GBARRACK_GETSOLDERLIST			(DWORD(2131))	// 濰熱辨煽菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_GBARRACK_BUYSOLDER				(DWORD(2132))	// 濰熱辨煽擊 堅辨и棻.
#define ON_RESPONSE_GBARRACK_BUYSOLDER				(DWORD(2133))	// 濰熱辨煽擊 堅辨縑 擬港и棻.
#define ON_REQUEST_GBARRACK_GETCHANGEJOBSOLDERLIST	(DWORD(2134))	// 瞪霜й 濰熱辨煽菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST	(DWORD(2135))	// 瞪霜й 濰熱辨煽菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_GBARRACK_CHANGEJOBSOLDER			(DWORD(2136))	// 濰熱辨煽擊 瞪霜и棻.
#define ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER		(DWORD(2137))	// 濰熱辨煽擊 瞪霜縑 擬港и棻.
#define ON_REQUEST_GBARRACK_GETDISMISSALSOLDERLIST	(DWORD(2138))	// п堅й 濰熱辨煽菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST	(DWORD(2139))	// п堅й 濰熱辨煽菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_GBARRACK_DISMISSALSOLDER			(DWORD(2140))	// 濰熱辨煽擊 п堅и棻.
#define ON_RESPONSE_GBARRACK_DISMISSALSOLDER		(DWORD(2141))	// 濰熱辨煽擊 п堅縑 擬港и棻.

#define ON_REQUEST_MBARRACK_GETMONSTERLIST			(DWORD(2150))	// 跨蝶攪菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_MBARRACK_GETMONSTERLIST			(DWORD(2151))	// 跨蝶攪菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_MBARRACK_BUYMONSTER				(DWORD(2152))	// 跨蝶攪蒂 堅辨и棻.
#define ON_RESPONSE_MBARRACK_BUYMONSTER				(DWORD(2153))	// 跨蝶攪 堅辨縑 擬港и棻.
#define ON_REQUEST_MBARRACK_GETDISMISSMONSTERLIST	(DWORD(2154))	// п堅й 跨蝶攪菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST	(DWORD(2155))	// п堅й 跨蝶攪菟曖 List 蹂羶縑 擬港и棻.
#define ON_REQUEST_MBARRACK_DISMISSMONSTER			(DWORD(2156))	// 跨蝶攪蒂 п堅и棻.
#define ON_RESPONSE_MBARRACK_DISMISSMONSTER			(DWORD(2157))	// 跨蝶攪 п堅縑 擬港и棻.

#define ON_REQUEST_GBARRACK_GET_CHANGE_GENERAL_LIST		(DWORD(2160))	// 2離 瞪霜й 濰熱菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST	(DWORD(2161))	// 2離 瞪霜й 濰熱菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_GBARRACK_CHANGE_GENERAL				(DWORD(2162))	// 2離 濰熱蒂 瞪霜 и棻.
#define ON_RESPONSE_GBARRACK_CHANGE_GENERAL				(DWORD(2163))	// 2離 濰熱 瞪霜縑 擬港 и棻.
#define ON_REQUEST_GBARRACK_GETDISMISSAL_GENERAL_LIST	(DWORD(2164))	// п堅й 2離 濰熱菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST	(DWORD(2165))	// п堅й 2離 濰熱菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_GBARRACK_DISMISSAL_GENERAL			(DWORD(2166))	// 2離 濰熱 п堅и棻.
#define ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL			(DWORD(2167))	// 2離 濰熱 п堅縑 擬港и棻.
#define ON_REQUEST_GBARRACK_GET_GENERAL_LIST			(DWORD(2168))	// 2離 濰熱菟曖 List蒂 蹂羶и棻.
#define ON_RESPONSE_GBARRACK_GET_GENERAL_LIST			(DWORD(2169))	// 2離 濰熱菟曖 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_GBARRACK_BUY_GENERAL					(DWORD(2170))	// 2離 濰熱蒂 堅辨и棻.
#define ON_RESPONSE_GBARRACK_BUY_GENERAL				(DWORD(2171))	// 2離 濰熱 堅辨縑 擬港и棻.


#define ON_REQUEST_HOSPITAL_GETCURELIST			(DWORD(2100))	// 辨煽菟曖 纂猿 List蒂 蹂羶и棻.
#define ON_RESPONSE_HOSPITAL_GETCURELIST		(DWORD(2101))	// 辨煽菟曖 纂猿 List蒂 蹂羶縑 擬港и棻.
#define ON_REQUEST_HOSPITAL_CURESOLDER			(DWORD(2102))	// 辨煽擊 纂猿и棻.
#define ON_RESPONSE_HOSPITAL_CURESOLDER			(DWORD(2103))	// 辨煽擊 纂猿縑 擬港и棻.
#define ON_REQUEST_HOSPITAL_CUREALLSOLDER		(DWORD(2104))	// 賅萇 辨煽擊 纂猿и棻.
#define ON_RESPONSE_HOSPITAL_CUREALLSOLDER		(DWORD(2105))	// 賅萇 辨煽擊 纂猿縑 擬港и棻.
#define ON_RET_AUTOCUREOK						(DWORD(2))		// 濠翕纂猿蒂 ъ陛и棻.

#define ON_REQUEST_WHARF_ENTER					(DWORD(2110))	// 睡舒縑 菟橫除棻.
#define ON_RESPONSE_WHARF_ENTER					(DWORD(2111))	// 睡舒縑 菟橫除棻.(擬港)
#define ON_RESPONSE_WHARF_CHANGETICKETLIST		(DWORD(2112))	// ル曖 List蒂 機等檜お и棻.
#define ON_REQUEST_WHARF_BUYTICKET				(DWORD(2113))	// 睡舒縑憮 ル蒂 骯棻.
#define ON_RESPONSE_WHARF_BUYTICKET				(DWORD(2114))	// 睡舒縑憮 ル蒂 骯棻.(擬港)
#define ON_REQUEST_WHARF_EXIT					(DWORD(2115))	// 睡舒縑憮 釭除棻.
#define ON_RESPONSE_WHARF_EXIT					(DWORD(2116))	// 睡舒縑憮 釭除棻.(擬港)
#define ON_REQUEST_WHARFWAITROOM_ENTER			(DWORD(2117))	// 睡舒渠晦褒縑 菟橫除棻.
#define ON_RESPONSE_WHARFWAITROOM_ENTER			(DWORD(2118))	// 睡舒渠晦褒縑 菟橫除棻.(擬港)(陛霞 Ticket曖 衛除擊 蟾晦ж賊 跤 菟橫陛霞棻.)
#define ON_REQUEST_WHARFWAITROOM_EXIT			(DWORD(2119))	// 睡舒渠晦褒縑憮 釭除棻.
#define ON_RESPONSE_WHARFWAITROOM_EXIT			(DWORD(2120))	// 睡舒渠晦褒縑憮 釭除棻.(擬港)
#define ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP	(DWORD(2121))	// 睡舒渠晦褒縑憮 蝓摹ж棻.
#define ON_RET_NOTUSETICKET						(DWORD(2))		// じ鰍擊 餌辨й熱橈朝 唳辦

// 婦羶
#define ON_REQUEST_GOVERNMENT_MAIN_ENTER					(DWORD(2200))		// 婦羶 菟橫陛晦
#define ON_RESPONSE_GOVERNMENT_MAIN_ENTER					(DWORD(2201))		// 婦羶 菟橫陛晦 擬港
#define ON_REQUEST_GOVERNMENT_MAIN_LEAVE					(DWORD(2202))		// 婦羶 釭陛晦
#define ON_RESPONSE_GOVERNMENT_MAIN_LEAVE					(DWORD(2203))		// 婦羶 釭陛晦 擬港

#define ON_REQUEST_GOVERNMENT_INVESTMENT_ENTER				(DWORD(2204))		// 婦羶, 癱濠 菟橫陛晦
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER				(DWORD(2205))		// 婦羶, 癱濠 菟橫陛晦 擬港
#define ON_REQUEST_GOVERNMENT_INVESTMENT_LEAVE				(DWORD(2206))		// 婦羶, 癱濠 釭陛晦
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE				(DWORD(2207))		// 婦羶, 癱濠 釭陛晦 擬港
#define ON_REQUEST_GOVERNMENT_INVESTMENT_GETLIST			(DWORD(2208))		// 婦羶, 癱濠 List橢晦
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST			(DWORD(2209))		// 婦羶, 癱濠 List橢晦 擬港
#define ON_REQUEST_GOVERNMENT_INVESTMENT_SETINVESTMENT		(DWORD(2210))		// 婦羶, 癱濠 ж晦
#define ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT		(DWORD(2211))		// 婦羶, 癱濠 ж晦 擬港
#define	ON_RET_NOTINVESTMENT_MANYMAXINVESTMENT				(DWORD(2))
#define	ON_RET_NOTINVESTMENT_MANYMONEY						(DWORD(3))
#define	ON_RET_NOTINVESTMENT_TIMEOVER						(DWORD(4))

#define ON_REQUEST_GOVERNMENT_VILLAGEINFO					(DWORD(2212))		// 婦羶, 葆擊 薑爾 橢晦 蹂羶
#define ON_RESPONSE_GOVERNMENT_VILLAGEINFO					(DWORD(2213))		// 婦羶, 葆擊 薑爾 橢晦 擬港
#define	ON_REQUEST_PROFIT_STATISTICS						(DWORD(2214))
#define	ON_RESPONSE_PROFIT_STATISTICS						(DWORD(2215))

#define	ON_REQUEST_GOVERNMENT_INVESTMENT_GETOUT				(DWORD(2216))		// 婦羶, 癱濠и絲 陛雖堅 釭陛晦
#define	ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT			(DWORD(2217))		// 婦羶, 癱濠и絲 陛雖堅 釭陛晦 擬港

// 鼻欽
#define	ON_REQUEST_CREATE_GUILD								(DWORD(2300))		// 鼻欽擊 儅撩и棻.(蹂掘)
#define	ON_RESPONSE_CREATE_GUILD							(DWORD(2301))		// 鼻欽擊 儅撩и棻.(擬港)
#define	ON_REQUEST_DELETE_GUILD								(DWORD(2302))		// 鼻欽擊 餉薯и棻.(蹂掘)
#define	ON_RESPONSE_DELETE_GUILD							(DWORD(2303))		// 鼻欽擊 餉薯и棻.(擬港)
#define	ON_REQUEST_JOIN_LEAVE_GUILD							(DWORD(2304))		// 鼻欽縑 陛殮/驍黴и棻.(蹂掘)
#define	ON_RESPONSE_JOIN_LEAVE_GUILD						(DWORD(2305))		// 鼻欽縑 陛殮/驍黴и棻.(擬港)
#define	ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST			(DWORD(2306))		// 鼻欽縑 霤罹ж晦 錳ж朝 霤陛渠晦濠曖 葬蝶お蒂 橢橫螞棻.(蹂掘)
#define	ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST			(DWORD(2307))		// 鼻欽縑 霤罹ж晦 錳ж朝 霤陛渠晦濠曖 葬蝶お蒂 橢橫螞棻.(擬港)
#define	ON_REQUEST_JOINGUILD_OKNO							(DWORD(2308))		// 鼻欽縑 霤罹ж晦 錳ж朝 霤陛 渠晦濠縑啪 OK, NO蒂 爾魚棻.(蹂掘)
#define	ON_RESPONSE_JOINGUILD_OKNO							(DWORD(2309))		// 鼻欽縑 霤罹ж晦 錳ж朝 霤陛 渠晦濠縑啪 OK, NO蒂 爾魚棻.(擬港)
#define	ON_REQUEST_GUILD_MEMBERS_LIST						(DWORD(2310))		// 鼻欽錳 葬蝶お蒂 橢橫螞棻.(蹂掘)
#define	ON_RESPONSE_GUILD_MEMBERS_LIST						(DWORD(2311))		// 鼻欽錳 葬蝶お蒂 橢橫螞棻.(擬港)
#define	ON_REQUEST_GUILD_DISMISSAL							(DWORD(2312))		// 鼻欽錳擊 п堅и棻.(蹂掘)
#define	ON_RESPONSE_GUILD_DISMISSAL							(DWORD(2313))		// 鼻欽錳擊 п堅и棻.(擬港)
#define	ON_REQUEST_GUILD_LIST								(DWORD(2314))		// 鼻欽曖 葬蝶お蒂 橢橫螞棻.(蹂掘)
#define	ON_RESPONSE_GUILD_LIST								(DWORD(2315))		// 鼻欽曖 葬蝶お蒂 橢橫螞棻.(擬港)
#define	ON_GUILD_MSG										(DWORD(2316))		// 鼻欽曖 詭衛雖
#define	ON_REQUEST_GUILD_SET_RELATION						(DWORD(2317))		// 鼻欽除曖 婦啗蒂 撲薑и棻.(蹂掘)
#define	ON_RESPONSE_GUILD_SET_RELATION						(DWORD(2318))		// 鼻欽除曖 婦啗蒂 撲薑и棻.(擬港)
#define ON_REQUEST_GUILD_CREATE_COST						(DWORD(2319))		// 鼻欽 偃撲 綠辨擊 橢橫螞棻(蹂掘)
#define ON_RESPONSE_GUILD_CREATE_COST						(DWORD(2320))		// 鼻欽 偃撲 綠辨擊 橢橫螞棻(擬港)

#define	ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT				(DWORD(2321))		// 葆擊寞橫溘縑 癱濠й熱 氈朝 譆堅 癱濠擋擊 橢朝棻.(蹂掘)
#define	ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT				(DWORD(2322))		// 葆擊寞橫溘縑 癱濠й熱 氈朝 譆堅 癱濠擋擊 橢朝棻.(擬港)
#define	ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT				(DWORD(2323))		// 葆擊寞橫溘縑 癱濠и棻.(蹂掘)
#define	ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT				(DWORD(2324))		// 葆擊寞橫溘縑 癱濠и棻.(擬港)

#define	ON_REQUEST_GUILD_DETAIL_INFO						(DWORD(2350))		// 鼻欽曖 撮撮и 薑爾蒂 橢橫螞棻.(蹂掘)
#define	ON_RESPONSE_GUILD_DETAIL_INFO						(DWORD(2351))		// 鼻欽曖 撮撮и 薑爾蒂 橢橫螞棻.(擬港)
#define ON_REQUEST_GUILD_PROMOTION							(DWORD(2352))		// 鼻欽錳擊 か薑 霜晝戲煎 歜貲и棻.(蹂掘)
#define ON_RESPONSE_GUILD_PROMOTION							(DWORD(2353))		// 鼻欽錳擊 か薑 霜晝戲煎 歜貲и棻.(擬港)
#define ON_REQUEST_GUILD_DEMOTION							(DWORD(2354))		// 鼻欽錳擊 п歜(霜晝縑 渠и 鬼蛔)и棻.(蹂掘)
#define ON_RESPONSE_GUILD_DEMOTION							(DWORD(2355))		// 鼻欽錳擊 п歜(霜晝縑 渠и 鬼蛔)и棻.(擬港)

#define	ON_ATTACK_TYPE_NEARCAPTAIN							(DWORD(0))
#define ON_ATTACK_TYPE_CAPTAIN								(DWORD(1))
#define ON_ATTACK_TYPE_NORMAL								(DWORD(2))

#define ON_REQUEST_FIELDATTACK_READY						(DWORD(2700))		// 奢撩 鼠晦蒂 嫦餌и棻朝 褐�ㄧ� 爾魚棻.
#define ON_RESPONSE_FIELDATTACK_READY						(DWORD(2701))		// 奢撩 鼠晦蒂 嫦餌и棻朝 褐�ㄧ� 嫡懊棻.

#define ON_REQUEST_FIELDATTACK_ACTION						(DWORD(2702))		//
#define ON_RESPONSE_FIELDATTACK_ACTION						(DWORD(2703))		//

#define ON_RET_ATTACK_POWER									(DWORD(0))			// 奢3機
#define ON_RET_DEFENCE_POWER								(DWORD(1))			// 寞機
#define ON_RET_NONE_POWER									(DWORD(2))			// 雄

#define ON_REQUEST_FIELDATTACK_DEAD							(DWORD(2704))		//
#define ON_RESPONSE_FIELDATTACK_DEAD						(DWORD(2705))		//

#define ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY			(DWORD(2706))
#define ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY		(DWORD(2707))
#define ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION		(DWORD(2708))
#define ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION		(DWORD(2709))

#define ON_RET_ATTACK_OK                                    (DWORD(0))         // ok
#define ON_RET_NOT_ATTACK_TIME                              (DWORD(1))         // 奢撩衛除檜 嬴棲棻.
#define ON_RET_NOT_VILLAGE_CODE                             (DWORD(2))         // 葆擊 囀萄陛 ぎ葬棻.
#define ON_RET_NOT_ENEMYGUILD                               (DWORD(3))         // 瞳渠 望萄 葆擊檜 嬴棲棻.
#define ON_RET_NOT_ATTACK_UNITE                             (DWORD(4))         // 奢撩 嶸棉檜 嬴棲棻. 
#define ON_RET_NOT_ATTACK_DELAY_TIME                        (DWORD(5))         // 奢問 裁溯檜 衛除檜 陴懊棻. 
#define ON_RET_ATTACK_NO                                    (DWORD(6))


#define ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT					(DWORD(2731))		// 爾晝 熱溯煎 睡攪 爾晝ヶ擊 雖晝嫡朝棻.
#define ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT				(DWORD(2732))		// (擬港).

#define ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK				(DWORD(2729))		// 葆擊 奢問⑽鷓蒂 夥羚熱 氈朝雖 憲嬴螞棻.
#define ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK				(DWORD(2730))		// (擬港).

#define ON_IMPERIUM_USE										(DWORD(2733))		//	模餵掏 餌辨.

#define ON_RET_ATTACK_TYPE_NOT_AUTH							(DWORD(0))			// 渠寞檜釭 渠ч熱陛 嬴棲棻.
#define ON_RET_ATTACK_TYPE_ACTIVE							(DWORD(1))			// 棻艇 葆擊 奢問顫殮檜 褒ч醞檜棻.
#define ON_RET_ATTACK_TYPE_NO								(DWORD(2))			// 斜夤曖 棻艇 唳辦曖 縑楝菟.
#define ON_RET_ATTACK_TYPE_OK								(DWORD(3))			// 葆擊 AttackType滲唳檜 撲薑腎歷棻.
#define ON_RET_ATTACK_TYPE_DELAY							(DWORD(4))			// 薯и衛除檜 嬴霜 部釭雖 彊懊棻.

#define ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK				(DWORD(2710))		// 葆擊 奢問⑽鷓蒂 晦葷奢問戲煎 夥羞棻.
#define ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK				(DWORD(2711))		// 擬港

#define ON_RET_OIL_OK										(DWORD(0))
#define	ON_RET_OIL_NOT_MONEY								(DWORD(1))
#define	ON_RET_OIL_NO										(DWORD(2))

#define ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK				(DWORD(2712))		// 葆擊 奢問⑽鷓蒂 給奢問戲煎 夥羞棻.
#define ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK				(DWORD(2713))		// 擬港

#define ON_RET_STONE_OK										(DWORD(0))
#define	ON_RET_STONE_NOT_MONEY								(DWORD(1))
#define	ON_RET_STONE_NO										(DWORD(2))

#define ON_REQUEST_VILLAGE_DEFENCE							(DWORD(2735))		// 葆擊 寞橫溘 щ鼻 詭撮雖蒂 爾魚棻.
#define ON_RESPONSE_VILLAGE_DEFENCE							(DWORD(2736))		// 擬港

#define ON_RET_DEFENSE_OK									(DWORD(0))			// 螃馨
#define ON_RET_DEFENSE_NOT_MONEY							(DWORD(1))			// 絲檜 睡褶ж棻.
#define ON_RET_DEFENSE_NO									(DWORD(2))			// 棻艇 檜嶸.

#define	ON_CHANGED_VILLAGE_ATTACK_TYPE						(DWORD(2728))		// 衛除檜 雖釭憮 葆擊 奢問曖 ⑽鷓陛 錳楚渠煎 給嬴諮棻堅 憲溥遽棻.
																				// 葆擊 輿滲曖 Ы溯檜橫菟縑啪虜 瞪歎.
#define ON_REQUEST_INCREASE_GUILD_UNIT						(DWORD(2714))		// 奢撩嶸棉 儅撩.
#define ON_RESPONSE_INCREASE_GUILD_UNIT						(DWORD(2715))		// (鼻欽曖 模餵掏擊 渦и棻.)

#define ON_REQUEST_DECREASE_GUILD_UNIT						(DWORD(2716))		// 奢撩嶸棉擊 釭散遽棻.
#define ON_RESPONSE_DECREASE_GUILD_UNIT						(DWORD(2717))		// (鼻欽曖 模餵掏擊 陛螳除棻.)

#define ON_REQUEST_LIST_GUILD_UNIT							(DWORD(2726))		// 望萄曖 奢撩 嶸棉 葬蝶お蒂 蹂羶и棻.
#define ON_RESPONSE_LIST_GUILD_UNIT							(DWORD(2727))		//

#define ON_REQUEST_INCREASE_GUILD_SUPPLY					(DWORD(2718))		// 鼻欽曖 爾晝ヶ擊 渦и棻.
#define ON_RESPONSE_INCREASE_GUILD_SUPPLY					(DWORD(2719))		//	

#define ON_REQUEST_DECREASE_GUILD_SUPPLY					(DWORD(2720))		// 鼻欽曖 爾晝ヶ擊 陛螳除棻.
#define ON_RESPONSE_DECREASE_GUILD_SUPPLY					(DWORD(2721))

#define ON_REQUEST_LIST_GUILD_SUPPLY						(DWORD(2724))		// 鼻欽 爾晝ヶ 葬蝶お蒂 橢橫螞棻.
#define ON_RESPONSE_LIST_GUILD_SUPPLY						(DWORD(2725))		// 

#define ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER			(DWORD(2722))		// 葆擊 熱綠煽擊 隸陛 衛鑑棻.
#define ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER		(DWORD(2723))		// 葆擊 熱綠煽擊 隸陛 衛鑑棻.(擬港).

#define ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER				(DWORD(2737))		// 葆擊 熱綠煽曖 熱蒂 橢橫螞棻.
#define ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER				(DWORD(2738))		// 擬港.

#define ON_CHANGED_PLAYER_DEAD_PENALTY						(DWORD(2734))		// 奢撩嶸棉檜 避橫憮 儅曹 ぬ割じ蒂 錳楚渠煎 給萼棻.

#define ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER				(DWORD(2739))		// �俸�薯曖 詭撮雖蒂 爾魚棻.
#define ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER			(DWORD(2740))		// 擬港.

#define ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT			(DWORD(2741))		// �俸�薯曖 ъ塊.
#define ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT			(DWORD(2742))		// 擬港.
	
#define ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL			(DWORD(2743))		// �俯讔池� 壅朝棻.
#define ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL			(DWORD(2744))		// �俯讔池� 壅朝棻.

#define ON_REQUEST_GUILD_VILLAGEATTACK_RESULT				(DWORD(2745))		// 奢撩唸婁蒂 蹂羶и棻.(渠寞)
#define ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT				(DWORD(2746))		// 奢撩唸婁縑 渠и 擬港.

#define ON_REQUEST_GUILD_CHOICE_PLAYER						(DWORD(2747))		// 奢撩唸婁 薄滄и 葆擊縑 癱濠й 嶸盪蒂 摹鷗и棻.
#define ON_RESPONSE_GUILD_CHOICE_PLAYER						(DWORD(2748))		// 擬港.

#define ON_REQUEST_VILLAGE_INVESTMENT_POWER					(DWORD(2749))		// п渡 嶸盪陛 癱濠蒂 й陽 п渡 葆擊縑 ァ癱蒂 й熱 氈朝雖 僭橫獄棻.
#define ON_RESPONSE_VILLAGE_INVESTMENT_POWER				(DWORD(2750))		// 擬港.

#define	ON_CHANGED_WAR_END									(DWORD(2751))		// 議葛攪蒂 錳楚 議葛攪煎 夥紱堅 奢憮婦溼 嬴檜蠱擊 雖遴棻.

#define ON_REQUEST_WAR_VILLAGE_DEFENCE						(DWORD(2752))		// 渠寞 議葛攪陛 熱撩檣攪む檜蝶煎 菟橫除棻
#define ON_RESPONSE_WAR_VILLAGE_DEFENCE						(DWORD(2753))		// 擬港.	

#define ON_RET_GUILD_VILLAGE_OFF_LIMITS						(DWORD(2754))		// 擬港 詭撮雖醞 ж釭.証餌塋檜 氈橫憮 轎殮й熱 橈棻.
																				// 釭該雖 擬港 詭撮雖朝 EnterVillage諦 偽棻.
//#define ON_REQUEST_WAR_VILLAGE_IN							(DWORD(2755))
//#define	ON_RESPONSE_WAR_VILLAGE_IN							(DWORD(2756))
#define ON_REQUEST_VILLAGE_DEFENCE_STOP                     (DWORD(2755))       // 熱撩 檣攪む檜蝶縑憮 釭除棻
#define ON_RESPONSE_VILLAGE_DEFFNCE_STOP                    (DWORD(2756))       // 擬港.


#define ON_REQUEST_WAR_WARP									(DWORD(2757))		// 奢撩瞪 醞 в蹂橈朝 議葛擊 幗萼棻.
#define ON_RESPONSE_WAR_WARP								(DWORD(2758))		// 擬港.

//橾奩 議葛攪煎 虜菟橫閨棻.
#define ON_REQUEST_NORMAL_CHAR								(DWORD(2759))		// OnReqNormalChar 
#define ON_RESPONSE_NORMAL_CHAR								(DWORD(2760))		// OnResNormalChar

#define ON_REQUEST_CHANGE_WAR                               (DWORD(2761))       //奢撩嶸棉戲煎 滲褐擊 蹂羶 и棻.
#define ON_RESPONSE_CHANGE_WAR                              (DWORD(2762))       //擬港

#define ON_CHANGE_OK                                        (DWORD(0))          //ok
#define ON_CHANGE_NOT_GUILD                                 (DWORD(1))          //鼻欽縑 陛殮腎橫氈雖 彊棻.
#define ON_CHANGE_ENOUGH_LEVEL                              (DWORD(2))          //溯漣檜 睡褶ж棻. 
#define ON_CHANGE_ENOUGH_MONEY                              (DWORD(3))          //絲檜 睡褶ж棻.
#define ON_CHANGE_ENOUGH_KIND                               (DWORD(4))          //п渡 樓撩曖 嶸棉檜 橈棻.
#define ON_CHANGE_FAILED                                    (DWORD(5))          //橾奩瞳檣 褒ぬ                                

#define ON_REQUEST_CHARGE_MP                                (DWORD(2763))       //爾晝熱溯曖 MP陛 �蛹僱Ц�棻 
#define ON_RESPONSE_CHARGE_MP                               (DWORD(2764))       //擬港

#define ON_RET_OK											(DWORD(0))			// 葆擊 熱綠煽 隸陛  OK
#define ON_RET_NOT_ENOUGH_MONEY								(DWORD(1))			// 絲檜 睡褶ж棻.
#define ON_RET_NOT_ENOUGH_SUPPLY							(DWORD(2))			// 爾晝檜 睡褶ж棻.
#define ON_RET_NOT_ENOUGH_POPULATION						(DWORD(3))			// 檣掘熱陛 睡褶ж棻.
#define ON_RET_NOT_LIMIT_POPULATION							(DWORD(4))			// 葆擊檜 陛韓熱 氈朝 譆模 譆渠 檣掘熱蒂 剩橫摹棻.
#define ON_RET_NOT											(DWORD(5))			// 斜夤曖 縑楝.


#define ON_RESPONSE_NOT_WAR                                 (DWORD(2765))       // 奢撩檜 橈橫螺棻.

#define ON_RESPONE_WAR_TIME                                 (DWORD(2766))       //奢撩衛除檜 脹匙擊 憲溥遽棻.

#define ON_ASK_GIVEUP                                       (DWORD(2767))       // 渠寞縑啪 瞪癱ん晦 й匙檣雖 僭橫獄棻.                                    
#define ON_REQUEST_GIVEUP                                   (DWORD(2768))       // 渠寞檜 瞪癱ん晦蒂 蹂羶и棻.  
#define ON_RESPONE_GIVEUP                                  (DWORD(2769))       // 擬港

#define	ON_REQUEST_ENTER_STRUCTURE							(DWORD(2800))		// 葆擊曖 か薑 勒僭縑 菟橫除棻.
#define	ON_RESPONSE_ENTER_STRUCTURE							(DWORD(2801))		// 葆擊曖 か薑 勒僭縑 菟橫除棻.(擬港)
#define	ON_REQUEST_LEAVE_STRUCTURE							(DWORD(2802))		// ⑷營 勒僭縑憮 釭除棻.
#define	ON_RESPONSE_LEAVE_STRUCTURE							(DWORD(2803))		// ⑷營 勒僭縑憮 釭除棻.(擬港)
//#define	ON_REQUEST_ENTER_FACTORY							(DWORD(2804))		// 葆擊 骯機 欽雖曖 奢濰縑 菟橫除棻.
//#define	ON_RESPONSE_ENTER_FACTORY							(DWORD(2805))		// 葆擊 骯機 欽雖曖 奢濰縑 菟橫除棻.(擬港)
#define	ON_REQUEST_LEAVE_FACTORY							(DWORD(2806))		// 葆擊 骯機 欽雖曖 奢濰縑憮 釭除棻.
#define	ON_RESPONSE_LEAVE_FACTORY							(DWORD(2807))		// 葆擊 骯機 欽雖曖 奢濰縑憮 釭除棻.(擬港)

#define ON_REQUEST_AUDIT_GUILD_INFO							(DWORD(2808))		// 鼻欽 薑爾蒂 橢橫螞棻.(蹂掘)
#define ON_RESPONSE_AUDIT_GUILD_INFO						(DWORD(2809))		// 鼻欽 薑爾蒂 橢橫螞棻.(擬港)

#define ON_RET_GUILD_OK										(DWORD(0))			// 鼻欽 薑爾 橢晦 撩奢.
#define ON_RET_GUILD_NO_CHANGED								(DWORD(1))			// 鼻欽 薑爾 滲�� 橈擠.
#define ON_RET_GUILD_NO_MEMBER								(DWORD(2))			// 鼻欽錳檜 嬴椒.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 嬴檜蠱 ⑷旎 っ衙 婦溼..
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

#define ON_RESPONSE_ITEMRECEIVED							(DWORD(2900))		// 掘殮и 嬴檜蠱 偃熱蒂 奩�� 
																				// 嬴檜蠱 偃熱( 2byte )
#define ON_REQUEST_ITEMLIST									(DWORD(2901))		// 掘殮и 嬴檜蠱 葬蝶お蒂 蹂羶 
#define ON_RESPONSE_ITEMLIST								(DWORD(2902))		// 掘殮и 嬴檜蠱 葬蝶お 奩�� 
																				// 嬴檜蠱 偃熱( 2byte ) + { ItemID( 2 byte ) + Quantity( 2byte ) + ... }
#define ON_REQUEST_TAKEOUTITEM								(DWORD(2903))		// 嬴檜蠱擊 檣漸饜葬縑 厥朝棻
																				// uiFollowerID( 1 ) + itemID( 2 ) + quantity( 2 )
#define ON_RESPONSE_TAKEOUTITEM								(DWORD(2904))		// 撩奢(0) 褒ぬ(1)


#define ON_REQUEST_SAVEITEM									(DWORD(2905))		// 嬴檜蠱擊 嬴檜蠱 屢觼煎 盪濰и棻 
#define ON_RESPONSE_SAVEITEM								(DWORD(2906))		// 撩奢(0) 褒ぬ(1)


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	瞪癱 婦溼,
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_RET_MARKET_OK						(DWORD(0))
#define ON_RET_MARKET_NO						(DWORD(1))
#define	ON_REQUEST_RETURNONLINE					(DWORD(3000))	// 瞪癱蒂 ж堅 棻衛 螞塭檣 в萄煎 給嬴除棻.
#define	ON_RESPONSE_RETURNONLINE				(DWORD(3001))	// 瞪癱蒂 ж堅 棻衛 螞塭檣戲煎 給嬴陞陽曖 擬港擊 嫡懊棻.
#define	ON_RET_RETURNONLINE_FIELD				(DWORD(1))		// в萄煎 給嬴除棻
#define	ON_RET_RETURNONLINE_VILLAGE				(DWORD(2))		// 葆擊煎 給嬴除棻

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蠡蝶お 婦溼
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_QUEST_SELECTNPC						(DWORD(8000))					// NPC 摹鷗衛
#define ON_RESPONSE_QUEST_SELECTNPC						(DWORD(8001))					// NPC 摹鷗衛 擬港(憮幗縑憮 ⑷營 蠡蝶お 鼻�窒� 評塭 擬港擊 爾頂遽棻.)
#define ON_REQUEST_QUEST_ACCEPTQUEST					(DWORD(8002))					// NPC 蠡蝶お 熱塊
#define ON_RESPONSE_QUEST_ACCEPTQUEST					(DWORD(8003))					// NPC 蠡蝶お 熱塊 擬港
#define ON_REQUEST_QUEST_CANCELQUEST					(DWORD(8004))					// NPC 蠡蝶お 醞除縑 鏃模
#define ON_RESPONSE_QUEST_CANCELQUEST					(DWORD(8005))					// NPC 蠡蝶お 醞除縑 鏃模 擬港
#define ON_REQUEST_QUEST_GETPLAYQUEST					(DWORD(8006))					// NPC 蠡蝶お 霞ч醞檣 蠡蝶お 葬蝶お 蹂羶
#define ON_RESPONSE_QUEST_GETPLAYQUEST					(DWORD(8007))					// NPC 蠡蝶お 霞ч醞檣 蠡蝶お 葬蝶お 蹂羶 擬港
#define ON_RESPONSE_QUEST_GETPLAYQUESTDATA				(DWORD(8008))					// NPC 蠡蝶お 霞ч醞檣 蠡蝶お Data
#define ON_RESPONSE_QUEST_ALLCLEARQUEST					(DWORD(8009))					// NPC 蠡蝶お 賅舒 蟾晦��
#define	ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO		(DWORD(8010))					// actdoll (2004/03/30 10:46) : 絮紫 蠡蝶お辨 諫猿濠 貲欽 鼠濛嬪 寞歎

#define ON_RET_QUEST_SELECTNPC_ERROR					(DWORD(0000))					// NPC 摹鷗衛 擬港(蠡蝶お 衛濛瞪)
#define ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST			(DWORD(0001))					// NPC 摹鷗衛 擬港(蠡蝶お 衛濛瞪)
#define ON_RET_QUEST_SELECTNPC_STARTQUEST				(DWORD(0002))					// NPC 摹鷗衛 擬港(蠡蝶お 衛濛)
#define ON_RET_QUEST_SELECTNPC_PLAYINGQUEST				(DWORD(0003))					// NPC 摹鷗衛 擬港(蠡蝶お 霞ч醞)
#define ON_RET_QUEST_SELECTNPC_ENDQUEST					(DWORD(0004))					// NPC 摹鷗衛 擬港(蠡蝶お 部)
#define ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST			(DWORD(0005))					// NPC 摹鷗衛 擬港(蠡蝶お 部��)
#define ON_RET_QUEST_SELECTNPC_NOTSAMENTION				(DWORD(0006))					// NPC 摹鷗衛 擬港(棻艇 措陛 橾陽)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 嬴檜蠱曖 嬪纂蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_REQUEST_MOVEITEM						(DWORD(4004))
#define	ON_RESPONSE_MOVEITEM					(DWORD(4005))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 遴艙濠 貲滄 詭衛雖
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_COMMAND								(DWORD(5000))	// 遴艙濠 貲滄 

// 辨煽擊 蹺陛衛鑑棻. (渡釭敝 蹺陛衛諦 偽擎 唳辦 憮幗縑憮 橾寞瞳戲煎 輿朝 詭撮雖)
#define ON_ADD_FOLLOWER							(DWORD(5001))

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 議葛攪曖 爾傘蝶蒂 餌辨ж罹 棟溘纂蒂 螢萼棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_REQUEST_INC_ABILITY_BYBONUS			(DWORD(6000))	
#define	ON_RESPONSE_INC_ABILITY_BYBONUS			(DWORD(6001))	

#define	ON_HUNGRY								(DWORD(6002))	// 寡堅Щ棻.

#define	ON_REQUEST_CHANGE_GUILD_FLAG			(DWORD(6010))	// 鼻欽曖 梓嫦擊 滲唳и棻.
#define	ON_RESPONSE_CHANGE_GUILD_FLAG			(DWORD(6011))	// 鼻欽曖 梓嫦擊 滲唳и棻.(擬港)
#define	ON_RESPONSE_CHANGE_GUILD_FLAG1			(DWORD(6014))	// 鼻欽曖 梓嫦擊 滲唳и棻.(擬港)
#define	ON_REQUEST_USED_FLAG_LIST				(DWORD(6012))	// 餌辨脹 鼻欽曖 梓嫦擊 蹂掘и棻.
#define	ON_RESPONSE_USED_FLAG_LIST				(DWORD(6013))	// 餌辨и 鼻欽曖 梓嫦擊 蹂掘и棻.(擬港)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 棻艇 議葛攪曖 嬪纂蒂 蹂掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_REQUEST_PI_INMINIMAP					(DWORD(6100))	// 嘐棲裘縑憮 餌辨濠曖 薑爾蒂 橢橫螞棻.
#define	ON_RESPONSE_PI_INMINIMAP				(DWORD(6101))	// 嘐棲裘縑憮 餌辨濠曖 薑爾蒂 橢橫螞棻.(擬港)

#define ON_REQUEST_PERSONAL_INFO                (DWORD(6200))
#define ON_RESPONSE_PERSONAL_INFO                (DWORD(6201))

#define	ON_MYVILLAGE_ATTACKED					(DWORD(6300))	// 釭曖 葆擊檜 奢問渡ж堅 氈棻.
#define	ON_EVENT_USERRANKINGS					(DWORD(6400))	// 餌辨濠菟曖 楨韁.

#define	ON_REQUEST_PM_OPEN						(DWORD(7000))	// 喻薄鼻擊 翱棻.
#define	ON_RESPONSE_PM_OPEN						(DWORD(7001))	// 喻薄鼻擊 翱棻.(擬港)
#define	ON_REQUEST_PM_CLOSE						(DWORD(7002))	// 喻薄鼻擊 殘朝棻.
#define	ON_RESPONSE_PM_CLOSE					(DWORD(7003))	// 喻薄鼻擊 殘朝棻.(擬港)
#define	ON_REQUEST_PM_BUYITEM					(DWORD(7004))	// 喻薄鼻縑啪 嬴檜蠱擊 骯棻.
#define	ON_RESPONSE_PM_BUYITEM					(DWORD(7005))	// 喻薄鼻縑啪 嬴檜蠱擊 骯棻.(擬港)
#define	ON_RESPONSE_PM_BUYFOLLOWER				(DWORD(7006))	// 喻薄鼻縑啪 辨煽擊 骯棻.(擬港)
#define	ON_REQUEST_PM_ITEMINFO					(DWORD(7007))	// 喻薄鼻檜 っ衙ж堅 氈朝 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
#define	ON_RESPONSE_PM_ITEMINFO					(DWORD(7008))	// 喻薄鼻檜 っ衙ж堅 氈朝 嬴檜蠱曖 薑爾蒂 滲唳и棻.(擬港)
#define	ON_REQUEST_PM_CHANGEITEM				(DWORD(7009))	// 喻薄鼻檜 っ衙ж堅 氈朝 嬴檜蠱曖 薑爾蒂 滲唳и棻.
#define	ON_RESPONSE_PM_CHANGEITEM				(DWORD(7010))	// 喻薄鼻檜 っ衙ж堅 氈朝 嬴檜蠱曖 薑爾蒂 滲唳и棻.
#define	ON_PM_CHANGE_BOOTH_ITEM					(DWORD(7011))	// 喻薄鼻檜 っ衙ж堅 氈朝 嬴檜蠱檜 滲唳腎歷棻.


#define	ON_REQUEST_TRADE							(DWORD(7500))	// 剪楚蒂 蹂羶и棻.
#define	ON_RESPONSE_TRADE							(DWORD(7501))	// 剪楚蒂 蹂羶и棻.(擬港)
#define ON_RECEIVE_ASK_TRADE						(DWORD(7502))	// п渡 贗塭檜樹お縑啪 棻艇 贗塭檜樹お陛 剪楚蒂 蹂羶ц棻朝 匙擊 憲溥遽棻.
#define	ON_REQUEST_ACCEPT_TRADE						(DWORD(7503))	// 剪楚 蹂羶擊 嫡擎 贗塭檜樹お陛 剪楚蒂 ъ塊ц棻.
#define ON_RESPONSE_ACCEPT_TRADE					(DWORD(7504))	// 剪楚 蹂羶擊 嫡擎 贗塭檜樹お陛 剪楚蒂 ъ塊ц棻.(擬港)
#define	ON_REQUEST_CHANGE_TRADE_GOODS				(DWORD(7505))	// 剪楚 醞檣 ヶ跡擊 熱薑и棻.
#define	ON_RESPONSE_CHANGE_TRADE_GOODS				(DWORD(7506))	// 剪楚 醞檣 ヶ跡擊 熱薑и棻.(擬港)
#define	ON_REQUEST_FINISH_CHOOSE_TRADEITEM			(DWORD(7507))	// 剪楚蒂 錳ж朝 嬴檜蠱擊 賅舒 棻 摹鷗ц棻.
#define	ON_RESPONSE_FINISH_CHOOSE_TRADEITEM			(DWORD(7508))	// 剪楚蒂 錳ж朝 嬴檜蠱擊 賅舒 棻 摹鷗ц棻.(擬港)
//#define	ON_RESPONSE_FINISHED_CHOOSE_TRADEITEM		(DWORD(7509))	// 剪楚蒂 ж堅 氈朝 贗塭檜樹お陛 剪楚蒂 錳ж朝 嬴檜蠱擊 賅舒 棻 摹鷗ц棻.
#define	ON_REQUEST_DECIDE_TRADE						(DWORD(7510))	// 剪楚蒂 ж朝 匙擊 熱塊ц棻.
#define	ON_RESPONSE_DECIDE_TRADE					(DWORD(7511))	// 剪楚蒂 ж朝 匙擊 熱塊ц棻.(擬港)
#define	ON_CHANGE_TRADE_GOODS						(DWORD(7512))	// 剪楚醞檣 ヶ跡擊 滲唳и棻.
#define	ON_REQUEST_CANCEL_TRADE						(DWORD(7512))	// 剪楚蒂 鏃模и棻.
#define	ON_RESPONSE_CANCEL_TRADE					(DWORD(7513))	// 剪楚蒂 鏃模и棻.(擬港)
#define	ON_REQUEST_COMPLETE_TRADE_BOOK				(DWORD(7514))	// 濰睡 剪楚陛 諫猿腎歷棻.
#define	ON_RESPONSE_COMPLETE_TRADE_BOOK				(DWORD(7515))	// 濰睡 剪楚陛 諫猿腎歷棻.(擬港)
#define	ON_COMPLETE_TRADE							(DWORD(7516))	// 剪楚陛 諫猿腎歷棻.

#define	ON_REQUEST_VILLAGE_INFO						(DWORD(7600))	// か薑 葆擊曖 薑爾蒂 蹂羶и棻.
#define	ON_RESPONSE_VILLAGE_INFO					(DWORD(7601))	// か薑 葆擊曖 薑爾蒂 蹂羶и棻.(擬港)

#define ON_REQUEST_PARTY							(DWORD(7700))	// だじ蒂 蹂羶и棻.
#define ON_RESPONSE_PARTY							(DWORD(7701))	// だじ蒂 蹂羶и棻.(擬港)

#define	ON_RECEIVE_ASK_PARTY						(DWORD(7702))   // п渡 贗塭檜樹お縑啪 棻艇 贗塭檜樹お陛 だじ蒂 蹂羶ц棻朝 匙擊 憲溥遽棻.
//#define ON_REQUEST_ASK_PARTY						ON_RECEIVE_ACK_PARTY

#define ON_REQUEST_ACCEPT_PARTY						(DWORD(7703))	// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻. 翕曖ж替傍 葆替傍.
#define ON_RESPONSE_ACCEPT_PARTY					(DWORD(7704))	// だじ 蹂羶擊 嫡擎 贗塭檜樹お陛 だじ縑 渠и 擬港擊 и棻.(擬港) 翕曖 唸婁縑 評塭.
#define ON_REQUEST_LEAVE_PARTY					    (DWORD(7705))	// 贗嬴檜樹お陛 だじ蒂 集陪棻.(贗塭檜樹お -> 憮幗)
#define ON_RESPONSE_LEAVE_PARTY						(DWORD(7706))	// 贗塭檜樹お だじ蒂 集陪棻曖 擬港 詭衛雖
//#define ON_RESPONSE_PARTY_LIST						(DWORD(7707))	// 贗塭檜樹お陛 だじ蒂 集陪棻.(憮幗 -> だじ 霤陛濠)
#define	ON_REQUEST_CANCEL_REQUESTING_PARTY			(DWORD(7708))	// 瞪縑 ц湍 だじ 蹂羶擊 鏃模и棻.
#define	ON_RESPONSE_CANCEL_REQUESTING_PARTY			(DWORD(7709))	// 瞪縑 ц湍 だじ 蹂羶擊 鏃模и棻朝 詭衛雖曖 擬港
#define	ON_CANCEL_REQUESTED_PARTY					(DWORD(7710))	// だじ蒂 蹂羶ц湍 Ы溯檜橫陛 だじ 蹂羶擊 鏃模ц棻.
#define	ON_PARTY_DELETED							(DWORD(7711))	// だじ陛 橈橫螺棻.
//#define	ON_PARTY_MEMBER_CHANGED						(DWORD(7713))	// だじ錳檜 滲唳腎歷棻.

#define ON_RELOAD_WEAR_ITEM							(DWORD(7712))	// actdoll (2004/07/19 13:26) : = 衛除薯 嬴檜蠱 = 嬴檜蠱 衛除 偵褐 顫檜墅(BroadCast)

// 10000 - 10999	:	啪歜 衛蝶蠱瞳檣匙(衛除蛔)
#define	ON_CHANGED_TIME							(DWORD(10001))
#define	ON_CHANGED_MONEY						(DWORD(10002))
#define	ON_TEXT_MSG								(DWORD(10003))
#define	ON_HEARTBEAT							(DWORD(10004))
#define	ON_OFFICIAL_ANNOUNCEMENT				(DWORD(10005))		// 奢雖 餌о
#define ON_CHANGE_WEATHER						(DWORD(10006))
#define	ON_REQUEST_RUN_PROGRAM_LIST				(DWORD(10007))
#define	ON_RESPONSE_RUN_PROGRAM_LIST			(DWORD(10008))
#define ON_MAKE_THUNDER							(DWORD(10009))		// 廓偃蒂 儅撩衛鑑棻.
#define ON_FRIEND_LOGGEDINOUT					(DWORD(10010))		// 耀掘陛 煎斜檣 ц棻.

// Response.
#define	ON_RET_OK								(DWORD(0))
#define	ON_RET_NO								(DWORD(1))

//#define ON_RET_PARTY_FULL						(DWORD(1200))
//#define ON_RET_PARTY_DIFPARTY					(DWORD(1201))
//#define ON_RET_NO_PARTY_CAP						(DWORD(1202))

#define	ON_RET_GLD_JOIN_OK						(DWORD(1000))
#define	ON_RET_GLD_JOIN_NO						(DWORD(1001))
#define	ON_RET_GLD_LEAVE_OK						(DWORD(1002))
#define	ON_RET_GLD_LEAVE_OK_DELETEGLD			(DWORD(1003))		// 鼻欽擊 集釭堅 望萄陛 餉薯腎歷棻.
#define	ON_RET_GLD_LEAVE_NO						(DWORD(1004))
#define	ON_RET_GLD_DELETEGLD					(DWORD(1005))		// 望萄陛 餉薯腎歷棻.
#define	ON_RET_GLD_CHANGECLASS					(DWORD(1006))		// 霜晝檜 滲唳腎歷棻.
#define	ON_RET_GLD_DISMISSAL					(DWORD(1007))		// 鼻欽縑憮 п堅腎歷棻.
#define	ON_RET_GLD_ALLY							(DWORD(1008))		// 鼻欽縑 翕裊擊 撲薑ц棻
#define	ON_RET_GLD_ENEMY						(DWORD(1009))		// 鼻欽縑 摹瞪ん堅蒂 ц棻.
#define	ON_RET_GLD_NEUTRAL						(DWORD(1010))		// 鼻欽婁 嬴鼠 婦啗紫 嬴棲棻.
#define	ON_RET_GLD_MINE							(DWORD(1011))		// 辦葬曖 鼻欽檜棻.
#define ON_RET_GLD_WAR							(DWORD(1012))

#define	ON_RET_TEXTMSG_WHISPER					(DWORD(1050))		// 敏樓蜓檜棻.
#define	ON_RET_TEXTMSG_GUILD					(DWORD(1051))		// 鼻欽 詭衛雖檜棻.
#define	ON_RET_TEXTMSG_PARTYCHAT				(DWORD(1052))		// だじ 瓣た 詭衛雖檜棻.

#define	ON_RET_HUR_CHANGE_HEALTH				(DWORD(2000))		// 寡堅Ь 詭衛雖縑憮 Health陛 馬模腎歷棻.
#define	ON_RET_HUR_CHANGE_HP					(DWORD(2001))		// 寡堅Ь 詭衛雖縑憮 羹溘檜 馬模腎歷棻.

#define	ON_RET_ENEMY_GUILD_VILLAGE				(DWORD(2002))		// 瞳 鼻欽曖 葆擊檜棻.
 
#define	ON_RET_NOT_CONNECTED					(DWORD(2100))		// 蕾樓腎橫 氈雖 彊棻.
#define	ON_RET_ON_FIELD							(DWORD(2101))		// в萄 嬪縑 氈棻.
#define	ON_RET_IN_VILLAGE						(DWORD(2102))		// 葆擊 寰縑 氈棻.
#define	ON_RET_IN_BATTLE						(DWORD(2103))		// 瞪癱 醞縑 氈棻.

#define	ON_RET_OA_TEXT							(DWORD(3000))		// 奢雖餌о - 橾奩瞳檣 臢蝶お 
#define	ON_RET_OA_VILL_ATTACKED					(DWORD(3001))		// 奢雖餌о - 葆擊檜 奢問擊 渡ж堅 氈棻.
#define	ON_RET_OA_VILL_OCCUPIED					(DWORD(3002))		// 奢雖餌о - 葆擊檜 薄滄腎歷棻.
#define	ON_RET_OA_MAXINVESTMENT_CHANGED			(DWORD(3003))		// 奢雖餌о - 譆堅 癱濠濠陛 滲唳 腎歷棻.
#define	ON_RET_OA_GUILD_CREATED					(DWORD(3004))		// 奢雖餌о - 鼻欽檜 虜菟橫螺棻.
#define	ON_RET_OA_GUILD_DELETED					(DWORD(3005))		// 奢雖餌о - 鼻欽檜 餌塭螺棻
#define	ON_RET_OA_PAYING_BUSINESS				(DWORD(3006))		// 奢雖餌о - 檜櫛檜 陴朝 濰餌曖 薑爾
#define	ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE	(DWORD(3007))		// 奢雖餌о - 葆擊擊 だ惚ж罹 渠寞檜 絲擊 橢歷棻.
#define	ON_RET_OA_SMS_MSG						(DWORD(3008))		// 奢雖餌о - SMS Message
#define ON_RET_OA_VILL_SURRENDER                (DWORD(3009))       // 葆擊 л塊
#define ON_RET_ON_GUILD_GIVEUP                  (DWORD(3010))       // 鼻欽檜 奢撩擊 ん晦


#define	ON_RET_SEND_TRADE_BOOK					(DWORD(4000))		// 濰睡蒂 瞪歎и棻.
#define	ON_RET_CANCEL_DECIDE_TRADE				(DWORD(4001))		// 鼻渠寞 難縑憮 '熱塊'幗が擊 揚毓湍 匙擊 鏃模ц棻.

//#define	ON_RET_CHNGEMONEY_NONE					(DWORD(5000))		// 欽雖 ⑷營 絲擊 爾頂遽棻.
#define	ON_RET_CHNGEMONEY_PRODUCTFACILITY		(DWORD(5001))		// 儅骯 勒僭檜 つ溥憮 絲擊 嫡懊棻.
#define	ON_RET_CHNGEMONEY_FROMADMIN				(DWORD(5002))		// 遴艙濠縑啪 絲擊 嫡懊棻.
#define	ON_RET_CHNGEMONEY_INVESTMENT			(DWORD(5003))		// 癱濠 寡渡旎擊 嫡懊棻.

#define ON_REQUEST_VMERCENARY_DELETE			(DWORD(8100))		// 評塭棻棲朝 辨煽 餉薯 蹂羶
#define ON_RESPONSE_VMERCENARY_CHANGE			(DWORD(8101))		// 評塭棻棲朝 辨煽 掖羹 擬港
#define ON_RESPONSE_VMERCENARY_CHANGEMAIN		(DWORD(8102))		// 評塭棻棲朝 辨煽 掖羹 擬港(獄檣縑啪 陛朝 詭撮雖)

#define ON_REQUEST_REFINE_ITEM                  (DWORD(9201))       // 嬴檜蠱 薯溼 蹂羶
#define ON_RESPONSE_REFINE_ITEM                 (DWORD(9202))       // 嬴檜蠱 薯溼 擬港

#define ON_NOTICE_ENCRYPTION_KEY				(DWORD(10555))		// 憮幗陛 贗塭檜樹お縑 懍��酈蒂 瞪歎и棻
//#define ON_RECEIVED_KEY							(DWORD(10556))		// 贗塭檜樹お陛 憮幗縑 懍��酈蒂 熱褐 ц擠擊 憲萼棻

#define ON_RET_NO_GLD_ADMIT_FULL				(DWORD(11000))		// 鼻欽錳檜 粕雙 鼻鷓塭憮 渠寞檜 雖錳濠蒂 殮欽 ъ辨й 熱 橈棻.
#define ON_RET_NO_GLD_ADMIT_MAXINVESTOR			(DWORD(11001))		// 葆擊擊 陛雖堅 氈晦 陽僥縑 雖錳濠蒂 殮欽 ъ辨й 熱 橈棻.(奢撩衛除渠縑)

#define ON_RESOPNSE_REAL_TIME					(DWORD(11002))




#define ON_REQUEST_MACRO_USING_USER_INFO		(DWORD(20000))		// 衙觼煎 餌辨濠 薑爾蒂 爾魚棻.

#endif

