#ifndef _DEFINES_H
#define _DEFINES_H
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// µð¹ö±ë¿ë ·Î±×ÆÄÀÏ»ý¼º
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define _LOGFILE_

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀÌº¥Æ®¿ë ÄÄÆÄÀÏ½Ã
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//#define _EVENT_

inline bool IsEvent()
{
#ifdef _EVENT_
	return true;
#endif
	return false;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// 4¹ÙÀÌÆ®ÀÇ ÆÐÅ¶ <-> 4°³ÀÇ 1¹ÙÀÌÆ®ÀÇ ÆÐÅ¶
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define	ABYTE(l)							(LOBYTE(LOWORD(l)))
#define	BBYTE(l)							(HIBYTE(LOWORD(l)))
#define	CBYTE(l)							(LOBYTE(HIWORD(l)))
#define	DBYTE(l)							(HIBYTE(HIWORD(l)))
#define MAKE32(a,b,c,d)						(MAKELONG(MAKEWORD(a,b),MAKEWORD(c,d)))

#define	SETSTATUS( a, b)					( a = b )										// »óÅÂ¸¦ ¼³Á¤ÇÑ´Ù.
#define ADDSTATUS(a,b)						(a|=b)											// »óÅÂ¸¦ Ãß°¡ÇÑ´Ù.
#define REMOVESTATUS(a,b)					(a^=b)											// »óÅÂ¸¦ »èÁ¦ÇÑ´Ù.
#define CHECKSTATUS(a,b)					( ( ( ( a ) & ( b ) ) != 0 ) ? TRUE : FALSE )	// »óÅÂ¸¦ È®ÀÎÇÑ´Ù.
#define ResetBuffer(a)						(memset(a, 0, sizeof(a)))	// ¹öÆÛ¸¦ ºñ¿î´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¸Ê ¼±¾ð 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_MAP_XSIZE						2000						//¸ÊÀÇ XÅ©±â
#define ON_MAP_YSIZE						2000						//¸ÊÀÇ YÅ©±â
#define ON_MAX_MAPSIZE						(ON_MAP_XSIZE*ON_MAP_YSIZE)	//¸ÊÀÇ ÀüÃ¼Å©±â
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬ 
// µ¥ÀÌÅÍº£ÀÌ½º ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//#define ON_DB_SERVER						"211.240.38.31"
//#define	ON_DB_SERVER2						"211.218.145.75"
//#define ON_DB_NAME							"IMJIN_ONLINE_TEST"
//#define	ON_DB_NAME2							"HQNet"
//#define ON_DB_USER							"hqteam_on"
//#define ON_DB_PASS							"ehtlfkr"
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÆÄÀÏ ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_ITEM_FILE						".\\OnLine\\Items\\item.ihf"
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÃÖ´ë ¼öÄ¡ ¼³Á¤
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define	ON_LOGIN_BACKLOG					50						// Á¢¼ÓÀ» À§ÇØ ´ë±âÇÏ°í ÀÖ´Â »ç¿ëÀÚ ¼ö

// actdoll (2005/02/23 23:17) : ¼ö¿ëÇÒ ¼ö ÀÖ´Â Ä¿³Ø¼ÇÀÇ ÃÖ´ë°ª
//
//	!!!!!!!!!!! ÁÖÀÇ !!!!!!!!!!
//
//	ÇÃ·¹ÀÌ¾î¿¡ ÇÒ´çµÇ´Â ¹öÆÛ¸¦ ÁÙ¿© ÀÛ¾÷ ¼Óµµ¸¦ ºü¸£°Ô ÇÏ±â À§ÇØ ÁÖ·Î ¼­¹ö ÀÛ¾÷ÀÚ°¡ ¸¹ÀÌ °Çµå¸®´Â ºÎºÐÀÌ´Ù.
//	ÀÏ´Ü ÀÓ½Ã ¹æÆíÀ¸·Î µðÆÄÀÎÀ» ³ª´²³ùÀ¸³ª ÃÖÁ¾ ¹èÆ÷½Ã¿¡´Â ¾çÂÊÀÇ ON_MAX_CONNECTION °ªÀÌ ¹Ýµå½Ã ÀÏÄ¡ÇØ¾ß ÇÑ´Ù.
//	¾çÂÊ °ªÀÌ Æ²¸®°Ô µÉ °æ¿ì Àß¸øÇÏ¸é Ä¡¸íÀûÀÎ ¿À·ù(Å¬¶óÀÌ¾ðÆ® Ä³¸¯ÅÍ°¡ ·Î±×ÀÎ ÇÑ ÈÄ È­¸éÀÌ °Ë°Ô º¯ÇÏ´Â µîÀÇ ¹ö±×)°¡ ¹ß»ýÇÑ´Ù.
//	(Ä³¸¯ÅÍ ÃÊ±âÈ­½Ã (RESPONSE_LOGIN) ºÎºÐ¿¡¼­ Àß¸øµÈ Á¤º¸°¡ ³Ñ¾î¿À°ï ÇÑ´Ù.)
//	¹Ýµå½Ã ÃÖÁ¾ÀûÀ¸·Î ¹èÆ÷ »óÈ²ÀÌ ¹ß»ýÇÒ ¶§´Â ¼­¹ö¿Í Å¬¶óÀÌ¾ðÆ®ÀÇ µÎ °ªÀÌ ÀÏÄ¡ÇØ¾ß ÇÑ´Ù.
//
// "Define.h" ¿¡¼­ ON_MAX_CONNECTION µµ °°Àº °ªÀ¸·Î ¼³Á¤ÇØ ÁÖ¾î¾ß ÇÑ´Ù.
#ifdef	_IMJINONLINESERVER
//	#define	ON_MAX_CONNECTION				    20				// ¼­¹ö ºÎºÐÀÌ¹Ç·Î ¼­¹ö ÀÛ¾÷ÀÚµéÀº ¿©±â¸¸ °Çµå¸± °Í! 
    #define	ON_MAX_CONNECTION				    6000
#else//!_IMJINONLINESERVER
	#define	ON_MAX_CONNECTION					6000			  	// Å¬¶óÀÌ¾ðÆ® ºÎºÐ. Å¬¶óÀÌ¾ðÆ® ÀÛ¾÷ÀÚ°¡ °ÅÀÇ °Çµå¸± ÀÏÀÌ ¾ø´Ù. Á¶ÀÛÇÏÁö ¸»°Í!
#endif//_IMJINONLINESERVER
																	// GSCDefine.h ¿¡¼­ MAX_PLAYER_NUMµµ °°Àº °ªÀ¸·Î ¼³Á¤ÇØ ÁÖ¾î¾ß ÇÑ´Ù.
#define ON_MAX_BATTLE						500						// ÃÖ´ë °³¼³ °¡´ÉÇÑ °ÔÀÓ ¼ö
#define ON_MAX_VILLAGE						500						// ÃÖ´ë µî·ÏÇÒ¼ö ÀÖ´Â ¸¶À» ¼ö
#define ON_MAX_NPC							10						// NPCÀÇ ¼ö
#define ON_MAX_LOG_LINE						1024					// ·Î±×ÆÄÀÏ¿¡ ÃÖ´ë µî·ÏÇÒ¼ö ÀÖ´Â ¶óÀÎ¼ö
#define ON_MAX_ITEM							600						// ¾ÆÀÌÅÛÁ¾·ù
#define	ON_MAX_BANKACCOUNT					10						// ÇÑ ¸¶À»¿¡¼­ °®°í ÀÖ´Â °èÁ¤ Á¾·ù.
#define ON_MAX_SALEITEM						40						// ¸¶À»¿¡¼­ ÆÄ´Â ¾ÆÀÌÅÛ ¼ö
#define ON_MAX_REMINDER_TIME				20						// Á¢¼ÓÈ®ÀÎ ½Ã°£°£°Ý
#define ON_MAX_WAIT_BATTLE_TIME				10000					// ÀüÅõ´ë±â ½Ã°£(¹Ð¸®¼¼ÄÁµå)
#define ON_MAX_PLAYERHAVEITEM				40						// ÇÃ·¹ÀÌ¾î°¡ °¡Áö°í ´Ù´Ò ¼ö ÀÖ´Â ¾ÆÀÌÅÛ ¼ö
//#define ON_MAX_PACKET_SIZE					30000					// ÇÑ¹ø¿¡ º¸³»´Â ÆÐÅ¶ÀÇ ÃÖ´ëÅ©±â
#define ON_MAX_GAME_DATASIZE				30						// ÀüÅõ¼­¹ö·Î º¸³¾ ÇÑ µ¥ÀÌÅÍÀÇ ÃÖ´ëÅ©±â
#define ON_MAX_GAME_DATANUMBER				1024					// ÀüÅõ¼­¹ö·Î º¸³¾ µ¥ÀÌÅÍ¸¦ º¸°üÇÒ ¼ö ÀÖ´Â ÃÖ´ë¼ö
#define ON_MAX_MARKETPLACE					1						// ÀúÀÚ°Å¸® °³¼ö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¹öÆÛ Å©±â ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_MAX_IN_BUFFER					30000					// ¼ö½Å ¹öÆÛ Å©±â
#define ON_MAX_OUT_BUFFER					30000					// ¼Û½Å ¹öÆÛ Å©±â
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// Å¬¶óÀÌ¾ðÆ® Á¾·ù ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_CLIENT_KIND_SYW_ONLINE			1						// ÀÓÁø·Ï¿Â¶óÀÎ Å¬¶óÀÌ¾ðÆ®
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÇÃ·¹ÀÌ¾î Á¾·ù ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_USER_KIND_RELEASE				1						// Á¤½Ä »ç¿ëÀÚ
#define ON_USER_KIND_DEMO					2						// µ¥¸ð »ç¿ëÀÚ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¹®ÀÚ¿­ ±æÀÌ ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define	ON_ID_LENGTH						10						// ID ±æÀÌ
#define	ON_ACCOUNT_LENGTH					12						// °èÁ¤ ID ±æÀÌ
#define ON_PASS_LENGTH						12						// ºñ¹Ð¹øÈ£ ±æÀÌ

//#define	ON_ID_LENGTH						20						// ID ±æÀÌ
//#define	ON_ACCOUNT_LENGTH					22						// °èÁ¤ ID ±æÀÌ
//#define ON_PASS_LENGTH						22						// ºñ¹Ð¹øÈ£ ±æÀÌ

#define ON_VILLAGE_LENGTH					10						// ¸¶À» ÀÌ¸§
#define ON_MAX_GUID_LENGTH					36						// GUID °ªÀÇ Å©±â 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÇÃ·¹ÀÌ¾î »óÅÂ ¼±¾ð (°ÔÀÓ Àü¹ÝÀûÀÎ »óÅÂ)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_STATE_LOGOUT						0
#define ON_STATE_LOGIN						10
#define ON_STATE_CONNECT					20
#define ON_STATE_CHANNEL					30
#define ON_STATE_BATTLE						40
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÆÐÅ¶ °ü·Ã ¼±¾ð 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_PACK_HEADER_SIZE		3
#define	ON_MAX_LOCALBUFFER		30000
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÇÃ·¹ÀÌ¾î »óÅÂ ¼±¾ð (°ÔÀÓ ³»ÀÇ »óÅÂ)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_PLAYER_STATUS_INFIELD			0x0001	// ÇÊµå¿¡¼­ ÇàÇØÁö´Â ÀÏÀº È¦¼ö·Î.(ÀüÅõ°É°í,¿öÇÁ°¡´É)
#define ON_PLAYER_STATUS_INVILLAGE			0x0002
#define ON_PLAYER_STATUS_INBATTLE			0x0004
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÇÃ·¹ÀÌ¾î°¡ ÀÖ´Â Àå¼Ò
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
enum	PlayerPlace{ON_PLAYER_PLACE_NONE, ON_PLAYER_PLACE_FIELD, ON_PLAYER_PLACE_VILLAGE, ON_PLAYER_PLACE_BATTLE};

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÇÃ·¹ÀÌ¾îÀÇ Á¤º¸¸¦ º¸³¾ ¹üÀ§¸¦ ¼³Á¤
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define	ON_STATUS_AREA_XSIZE				31						
#define	ON_STATUS_AREA_YSIZE				31						
#define ON_STATUS_AREA_XHALF				15						// ÇÃ·¹ÀÌ¾îÁÖÀ§¿¡ Á¤º¸¸¦ º¸³¾ X¿µ¿ª
#define ON_STATUS_AREA_YHALF				15						// ÇÃ·¹ÀÌ¾îÁÖÀ§¿¡ Á¤º¸¸¦ º¸³¾ Y¿µ¿ª
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¸¶À»ÀÇ Å©±â¸¦ ¼³Á¤
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_SMALL_VILLAGE					2						// 2x2 Å©±â
#define ON_MIDDLE_VILLAGE					3						// 3x3 Å©±â
#define ON_LARGE_VILLAGE					4						// 4x4 Å©±â
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀüÅõ»óÅÂ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_BATTLE_CREATEOK					0x0001					// °ÔÀÓÀ» ¸¸µé ¼ö ÀÖÀ»¶§
#define ON_BATTLE_WAIT						0x0002					// °ÔÀÓ¿¡ Âü¿©½ÃÅ³ ¼ö ÀÖÀ»¶§
#define ON_BATTLE_BEGIN						0x0003					// °ÔÀÓÀÌ ½ÃÀÛµÇ¾úÀ»¶§
#define ON_BATTLE_END						0x0004					// °ÔÀÓÀÌ ³¡³µÀ»¶§
#define ON_BATTLE_READY						0x0010					// ÀüÅõ¼­¹öÀÇ ÁØºñ°¡ ³¡³µÀ»¶§
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ·Î±×ÀÎ ¼­¹ö °ü·Ã ¼±¾ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define ON_LOGINSERVER_PORT				2342
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
#define	ON_ITEM_NOTSELL					0
#define	ON_ITEM_SELL					1

#define MINIGAME_TYPE_1		1
#define MINIGAME_TYPE_2		2
#define MINIGAME_TYPE_3		3

#endif
