#ifndef _DEFINES_H
#define _DEFINES_H
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蛤幗梵辨 煎斜だ橾儅撩
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define _LOGFILE_

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 檜漸お辨 闡だ橾衛
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define _EVENT_

inline bool IsEvent()
{
#ifdef _EVENT_
	return true;
#endif
	return false;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 4夥檜お曖 ぬ韃 <-> 4偃曖 1夥檜お曖 ぬ韃
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ABYTE(l)							(LOBYTE(LOWORD(l)))
#define	BBYTE(l)							(HIBYTE(LOWORD(l)))
#define	CBYTE(l)							(LOBYTE(HIWORD(l)))
#define	DBYTE(l)							(HIBYTE(HIWORD(l)))
#define MAKE32(a,b,c,d)						(MAKELONG(MAKEWORD(a,b),MAKEWORD(c,d)))

#define	SETSTATUS( a, b)					( a = b )										// 鼻鷓蒂 撲薑и棻.
#define ADDSTATUS(a,b)						(a|=b)											// 鼻鷓蒂 蹺陛и棻.
#define REMOVESTATUS(a,b)					(a^=b)											// 鼻鷓蒂 餉薯и棻.
#define CHECKSTATUS(a,b)					( ( ( ( a ) & ( b ) ) != 0 ) ? TRUE : FALSE )	// 鼻鷓蒂 �挫恉挬�.
#define ResetBuffer(a)						(memset(a, 0, sizeof(a)))	// 幗ぷ蒂 綠遴棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 裘 摹樹 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_MAP_XSIZE						2000						//裘曖 X觼晦
#define ON_MAP_YSIZE						2000						//裘曖 Y觼晦
#define ON_MAX_MAPSIZE						(ON_MAP_XSIZE*ON_MAP_YSIZE)	//裘曖 瞪羹觼晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收 
// 等檜攪漆檜蝶 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//#define ON_DB_SERVER						"211.240.38.31"
//#define	ON_DB_SERVER2						"211.218.145.75"
//#define ON_DB_NAME							"IMJIN_ONLINE_TEST"
//#define	ON_DB_NAME2							"HQNet"
//#define ON_DB_USER							"hqteam_on"
//#define ON_DB_PASS							"ehtlfkr"
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// だ橾 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_ITEM_FILE						".\\OnLine\\Items\\item.ihf"
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 譆渠 熱纂 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_LOGIN_BACKLOG					50						// 蕾樓擊 嬪п 渠晦ж堅 氈朝 餌辨濠 熱

// actdoll (2005/02/23 23:17) : 熱辨й 熱 氈朝 醴喀暮曖 譆渠高
//
//	!!!!!!!!!!! 輿曖 !!!!!!!!!!
//
//	Ы溯檜橫縑 й渡腎朝 幗ぷ蒂 還罹 濛機 樓紫蒂 緒腦啪 ж晦 嬪п 輿煎 憮幗 濛機濠陛 號檜 勒萄葬朝 睡碟檜棻.
//	橾欽 歜衛 寞ら戲煎 蛤だ檣擊 釭散堡戲釭 譆謙 寡ん衛縑朝 曄薹曖 ON_MAX_CONNECTION 高檜 奩萄衛 橾纂п撿 и棻.
//	曄薹 高檜 ぎ葬啪 腆 唳辦 澀跤ж賊 纂貲瞳檣 螃盟(贗塭檜樹お 議葛攪陛 煎斜檣 и �� �飛橉� 匐啪 滲ж朝 蛔曖 幗斜)陛 嫦儅и棻.
//	(議葛攪 蟾晦�亳� (RESPONSE_LOGIN) 睡碟縑憮 澀跤脹 薑爾陛 剩橫螃堆 и棻.)
//	奩萄衛 譆謙瞳戲煎 寡ん 鼻�窕� 嫦儅й 陽朝 憮幗諦 贗塭檜樹お曖 舒 高檜 橾纂п撿 и棻.
//
// "Define.h" 縑憮 ON_MAX_CONNECTION 紫 偽擎 高戲煎 撲薑п 輿橫撿 и棻.
#ifdef	_IMJINONLINESERVER
//	#define	ON_MAX_CONNECTION				    20				// 憮幗 睡碟檜嘎煎 憮幗 濛機濠菟擎 罹晦虜 勒萄萵 匙! 
    #define	ON_MAX_CONNECTION				    6000
#else//!_IMJINONLINESERVER
	#define	ON_MAX_CONNECTION					6000			  	// 贗塭檜樹お 睡碟. 贗塭檜樹お 濛機濠陛 剪曖 勒萄萵 橾檜 橈棻. 褻濛ж雖 蜓匙!
#endif//_IMJINONLINESERVER
																	// GSCDefine.h 縑憮 MAX_PLAYER_NUM紫 偽擎 高戲煎 撲薑п 輿橫撿 и棻.
#define ON_MAX_BATTLE						500						// 譆渠 偃撲 陛棟и 啪歜 熱
#define ON_MAX_VILLAGE						500						// 譆渠 蛔煙й熱 氈朝 葆擊 熱
#define ON_MAX_NPC							10						// NPC曖 熱
#define ON_MAX_LOG_LINE						1024					// 煎斜だ橾縑 譆渠 蛔煙й熱 氈朝 塭檣熱
#define ON_MAX_ITEM							600						// 嬴檜蠱謙盟
#define	ON_MAX_BANKACCOUNT					10						// и 葆擊縑憮 乾堅 氈朝 啗薑 謙盟.
#define ON_MAX_SALEITEM						40						// 葆擊縑憮 だ朝 嬴檜蠱 熱
#define ON_MAX_REMINDER_TIME				20						// 蕾樓�挫� 衛除除問
#define ON_MAX_WAIT_BATTLE_TIME				10000					// 瞪癱渠晦 衛除(塵葬撮鐘萄)
#define ON_MAX_PLAYERHAVEITEM				40						// Ы溯檜橫陛 陛雖堅 棻棍 熱 氈朝 嬴檜蠱 熱
//#define ON_MAX_PACKET_SIZE					30000					// и廓縑 爾頂朝 ぬ韃曖 譆渠觼晦
#define ON_MAX_GAME_DATASIZE				30						// 瞪癱憮幗煎 爾鳥 и 等檜攪曖 譆渠觼晦
#define ON_MAX_GAME_DATANUMBER				1024					// 瞪癱憮幗煎 爾鳥 等檜攪蒂 爾婦й 熱 氈朝 譆渠熱
#define ON_MAX_MARKETPLACE					1						// 盪濠剪葬 偃熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 幗ぷ 觼晦 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_MAX_IN_BUFFER					30000					// 熱褐 幗ぷ 觼晦
#define ON_MAX_OUT_BUFFER					30000					// 歎褐 幗ぷ 觼晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 贗塭檜樹お 謙盟 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_CLIENT_KIND_SYW_ONLINE			1						// 歜霞煙螞塭檣 贗塭檜樹お
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫 謙盟 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_USER_KIND_RELEASE				1						// 薑衝 餌辨濠
#define ON_USER_KIND_DEMO					2						// 等賅 餌辨濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 僥濠翮 望檜 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_ID_LENGTH						10						// ID 望檜
#define	ON_ACCOUNT_LENGTH					12						// 啗薑 ID 望檜
#define ON_PASS_LENGTH						12						// 綠塵廓�� 望檜

//#define	ON_ID_LENGTH						20						// ID 望檜
//#define	ON_ACCOUNT_LENGTH					22						// 啗薑 ID 望檜
//#define ON_PASS_LENGTH						22						// 綠塵廓�� 望檜

#define ON_VILLAGE_LENGTH					10						// 葆擊 檜葷
#define ON_MAX_GUID_LENGTH					36						// GUID 高曖 觼晦 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫 鼻鷓 摹樹 (啪歜 瞪奩瞳檣 鼻鷓)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_STATE_LOGOUT						0
#define ON_STATE_LOGIN						10
#define ON_STATE_CONNECT					20
#define ON_STATE_CHANNEL					30
#define ON_STATE_BATTLE						40
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// ぬ韃 婦溼 摹樹 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_PACK_HEADER_SIZE		3
#define	ON_MAX_LOCALBUFFER		30000
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫 鼻鷓 摹樹 (啪歜 頂曖 鼻鷓)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_PLAYER_STATUS_INFIELD			0x0001	// в萄縑憮 чп雖朝 橾擎 �汝鷊�.(瞪癱勘堅,錶Щ陛棟)
#define ON_PLAYER_STATUS_INVILLAGE			0x0002
#define ON_PLAYER_STATUS_INBATTLE			0x0004
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫陛 氈朝 濰模
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
enum	PlayerPlace{ON_PLAYER_PLACE_NONE, ON_PLAYER_PLACE_FIELD, ON_PLAYER_PLACE_VILLAGE, ON_PLAYER_PLACE_BATTLE};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 薑爾蒂 爾鳥 彰嬪蒂 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_STATUS_AREA_XSIZE				31						
#define	ON_STATUS_AREA_YSIZE				31						
#define ON_STATUS_AREA_XHALF				15						// Ы溯檜橫輿嬪縑 薑爾蒂 爾鳥 X艙羲
#define ON_STATUS_AREA_YHALF				15						// Ы溯檜橫輿嬪縑 薑爾蒂 爾鳥 Y艙羲
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 葆擊曖 觼晦蒂 撲薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_SMALL_VILLAGE					2						// 2x2 觼晦
#define ON_MIDDLE_VILLAGE					3						// 3x3 觼晦
#define ON_LARGE_VILLAGE					4						// 4x4 觼晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 瞪癱鼻鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_BATTLE_CREATEOK					0x0001					// 啪歜擊 虜菟 熱 氈擊陽
#define ON_BATTLE_WAIT						0x0002					// 啪歜縑 霤罹衛鑒 熱 氈擊陽
#define ON_BATTLE_BEGIN						0x0003					// 啪歜檜 衛濛腎歷擊陽
#define ON_BATTLE_END						0x0004					// 啪歜檜 部陬擊陽
#define ON_BATTLE_READY						0x0010					// 瞪癱憮幗曖 遽綠陛 部陬擊陽
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 煎斜檣 憮幗 婦溼 摹樹
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define ON_LOGINSERVER_PORT				2342
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define	ON_ITEM_NOTSELL					0
#define	ON_ITEM_SELL					1

#define MINIGAME_TYPE_1		1
#define MINIGAME_TYPE_2		2
#define MINIGAME_TYPE_3		3

#endif
