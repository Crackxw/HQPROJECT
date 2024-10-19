/************************************************************************************************
	ÆÄÀÏ¸í : Wansung2Johap.h
	´ã´çÀÚ : ÀÌÀ±¼®
************************************************************************************************/
#if !defined(AFX_WANSUNG2JOHAP_H__DDC7DCA4_5C4C_4155_847D_E1CFF8857410__INCLUDED_)
#define AFX_WANSUNG2JOHAP_H__DDC7DCA4_5C4C_4155_847D_E1CFF8857410__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string.h"
#include "stdio.h"

static unsigned char h_buffer[] = {
"­¡­¢­£­¤­¥­¦­§­¨­©­ª­«­¬­­­®­¯­°­±­²­³­´­µ­¶­·­¸­¹­º­»­¼­½­¾"
"­¿­À­Á­Â­Ã­Ä­Å­Æ­Ç­È­É­Ê­Ë­Ì­Í­Î­Ï­Ð­Ñ­Ò­Ó­Ô­Õ­Ö­×­Ø­Ù­Ú­Û­Ü"
"­Ý­Þ­ß­à­á­â­ã­ä­å­æ­ç­è­é­ê­ë­ì­í­î­ï­ð­ñ­ò­ó­ô­õ­ö­÷­ø­ù­ú"
"­û­ü­ý­þ®¡®¢®£®¤®¥®¦®§®¨®©®ª®«®¬®­®®®¯®°®±®²®³®´®µ®¶®·®¸®¹®º"
"®»®¼®½®¾®¿®À®Á®Â"};
static unsigned char i_h_buffer[] = { 
"ˆaˆbˆeˆhˆiˆjˆkˆqˆsˆtˆuˆvˆwˆxˆyˆ{ˆ|ˆ}ˆˆ‚ˆ…ˆ‰ˆ‘ˆ“ˆ•ˆ–ˆ—ˆ¡ˆ¢ˆ¥ˆ©ˆµˆ·ˆÁˆÅ"
"ˆÉˆáˆâˆåˆèˆéˆëˆñˆóˆõˆöˆ÷ˆøˆûˆüˆý‰A‰E‰I‰Q‰S‰U‰V‰W‰a‰b‰c‰e‰h‰i‰q‰s‰u‰v‰w"
"‰{‰‰…‰‰‰“‰•‰¡‰¢‰¥‰¨‰©‰«‰­‰°‰±‰³‰µ‰·‰¸‰Á‰Â‰Å‰É‰Ë‰Ñ‰Ó‰Õ‰×‰á‰å‰é‰ñ‰ö‰÷ŠA"
"ŠBŠEŠIŠQŠSŠUŠWŠaŠeŠiŠsŠuŠŠ‚Š…ŠˆŠ‰ŠŠŠ‹ŠŠ‘Š“Š•Š—Š˜Š¡Š¢Š¥Š©Š¶Š·ŠÁŠÕŠáŠâ"
"ŠåŠéŠñŠóŠõ‹A‹E‹I‹a‹b‹e‹h‹i‹j‹q‹s‹u‹w‹‹¡‹¢‹¥‹¨‹©‹«‹±‹³‹µ‹·‹¸‹¼ŒaŒbŒcŒe"
"ŒiŒkŒqŒsŒuŒvŒwŒ{ŒŒ‚Œ…Œ‰Œ‘Œ“Œ•Œ–Œ—Œ¡Œ¢Œ©ŒáŒâŒãŒåŒéŒñŒóŒõŒöŒ÷ABEQU"
"Waeiuv{¡¢¥§©±³µ·¸¹ÁÂÉÖ×áâ÷ŽAŽEŽIŽQŽSŽWŽaŽ"
"Ž‚Ž…Ž‰ŽŽ‘Ž“Ž•Ž—Ž˜Ž¡Ž©Ž¶Ž·ŽÁŽÂŽÅŽÉŽÑŽÓŽÖŽáŽåŽéŽñŽóAabegikpqs"
"uw{¡¢¥©±³µ·abcehijkqsuvwxy{}‚…‰‘“•"
"–—¡¢¥©±·áâäåéëìñóõö÷ý‘A‘B‘E‘I‘Q‘S‘U‘V‘W‘a‘b‘e‘i‘q"
"‘s‘v‘w‘z‘‘…‘¡‘¢‘¥‘©‘«‘±‘³‘µ‘·‘¼‘½‘Á‘Å‘É‘Ö’A’E’I’Q’S’U’a’b’e’i’s’u’w’"
"’‚’…’ˆ’‰’‘’“’•’—’¡’¶’Á’á’å’é’ñ’ó“A“B“I“Q“S“W“a“b“e“i“j“k“q“s“u“w“x“|“"
"“…“‰“¡“¢“¥“©“¯“±“³“µ“·“¼”a”b”c”e”h”i”j”k”l”p”q”s”u”v”w”x”y”}””‚”…”‰”‘"
"”“”•”–”—”¡”á”â”ã”å”è”é”ë”ì”ñ”ó”õ”÷”ù”ü•A•B•E•I•Q•S•U•V•W•a•e•i•v•w••…"
"•¡•¢•¥•¨•©•«•­•±•³•µ•·•¹•»•Á•Å•É•á•ö–A–E–I–Q–S–U–a––‚–…–‰–‘–“–•–—–¡–¶"
"–Á–×–á–å–é–ó–õ–÷—A—E—I—Q—W—a—b—e—h—i—k—q—s—u—w——¡—¢—¥—¨—©—±—³—µ—¶—·—¸"
"˜a˜b˜e˜i˜q˜s˜u˜v˜w˜}˜˜‚˜…˜‰˜‘˜“˜•˜–˜—˜á˜â˜å˜é˜ë˜ì˜ñ˜ó˜õ˜ö˜÷˜ý™A™B™E™I"
"™Q™S™U™V™W™a™v™¡™¢™¥™©™·™Á™É™ášAšEšš‚š…š‰šš‘š—šÁšášåšéšñšóš÷›a›b›e›h"
"›i›q›s›u››…›‰›‘›“›¡›¥›©›±›³›µ›·œaœbœeœiœqœsœuœvœwœxœ|œ}œœ‚œ…œ‰œ‘œ“œ•"
"œ–œ—œ¡œ¢œ¥œµœ·œáœâœåœéœñœóœõœöœ÷œýABEIQSUWabeiqsuvw"
"…“•¡¢¥©±³µ·ÁÅ×öžAžEžIžQžSžUžWžažežižsžužwžž‚ž…ž‰ž‘ž“ž•"
"ž—ž¡ž¶žÁžážâžåžéžñžõž÷ŸAŸBŸEŸIŸQŸSŸUŸWŸaŸbŸeŸiŸqŸsŸuŸwŸxŸ{Ÿ|Ÿ¡Ÿ¢Ÿ¥Ÿ©Ÿ±"
"Ÿ³ŸµŸ· a b e g h i j k q s u w x { }  ‚ … ‰ ‘ “ • – — ˜ ¡ ¢ © · á â å"
" é ë ñ ó õ ÷ ø ý¡A¡B¡E¡I¡Q¡S¡U¡V¡W¡a¡b¡e¡i¡u¡v¡w¡y¡¡¡¡¢¡¤¡¥¡©¡«¡±¡³¡µ"
"¡·¡Á¡Å¡Ö¡×¢A¢E¢I¢S¢U¢W¢a¢e¢i¢s¢u¢¢‚¢ƒ¢…¢ˆ¢‰¢Š¢‹¢‘¢“¢•¢—¢›¢¢¡¢¥¢©¢³¢µ"
"¢Á¢á¢å¢é£A£E£I£Q£U£a£e£i£q£u£¡£¢£¥£¨£©£«£±£³£µ£¶£·£¹£»¤a¤b¤c¤d¤e¤h¤i¤j"
"¤k¤l¤q¤s¤u¤w¤{¤¤‚¤…¤‰¤‘¤“¤•¤–¤—¤›¤¡¤¢¤¥¤³¤á¤â¤å¤è¤é¤ë¤ñ¤ó¤õ¤÷¤ø¥A¥B¥E"
"¥H¥I¥Q¥S¥U¥V¥W¥a¥b¥e¥i¥s¥u¥v¥w¥{¥¥…¥¡¥¢¥£¥¥¥©¥±¥³¥µ¥·¥Á¥Å¥Ö¥á¥ö¦A¦B¦E"
"¦I¦Q¦S¦a¦e¦¦‚¦…¦ˆ¦‰¦Š¦‹¦‘¦“¦•¦—¦›¦œ¦¡¦©¦¶¦Á¦á¦â¦å¦é¦÷§A§E§I§Q§U§W§a§b"
"§e§i§q§s§u§¡§¢§¥§©§«§±§³§µ§·§¸§¹¨a¨b¨e¨i¨k¨q¨s¨u¨v¨w¨}¨¨‚¨…¨‰¨‘¨“¨•¨–"
"¨—¨¡¨¢¨±¨á¨â¨å¨è¨é¨ñ¨õ¨ö¨÷©A©W©a©b©q©s©u©v©w©¡©¢©¥©©©±©³©·ªAªaªwªª‚ª…"
"ª‰ª‘ª•ª—«A«W«a«e«i«q«s«¡«¢«¥«©«±«³«µ«·¬a¬b¬d¬e¬h¬i¬j¬k¬q¬s¬u¬v¬w¬{¬¬‚"
"¬…¬‰¬‘¬“¬•¬–¬—¬¡¬¢¬¥¬©¬±¬³¬µ¬·¬Á¬Å¬É¬Ñ¬×¬á¬â¬ã¬ä¬å¬è¬é¬ë¬ì¬ñ¬ó¬õ¬ö¬÷¬ü"
"­A­B­E­I­Q­S­U­V­W­a­b­e­i­q­s­u­v­w­­…­‰­—­¡­¢­£­¥­©­«­±­³­µ­·­»­Á­Â"
"­Å­É­×­á­å­é­ñ­õ­ö®A®E®I®Q®S®U®a®b®e®i®q®s®u®w®®‚®…®ˆ®‰®‘®“®•®—®™®›®œ"
"®¡®¶®Á®Â®Å®É®Ñ®×®á®â®å®é®ñ®ó®õ®÷¯A¯B¯I¯Q¯U¯W¯a¯b¯e¯i¯j¯q¯s¯u¯w¯¡¯¢¯¥¯¨"
"¯©¯°¯±¯³¯µ¯·¯¼°a°b°d°e°i°q°s°v°w°}°°‚°…°‰°‘°“°–°—°·°á°â°å°é°ë°ñ°ó°ö°÷"
"±A±E±I±W±¡±¢±¥±¨±©±«±±±³±·±Á±Â±Å±Ö±á±ö²A²E²I²Q²S²a²²‚²…²‰²‘²“²—²¡²¶²Á"
"²á²å³W³a³b³e³i³k³p³q³s³³…³‰³‘³¡³¢³¥³©³±³³³µ³·´a´b´e´f´g´i´j´k´p´q´s´u"
"´v´w´{´|´´‚´…´‰´‘´“´•´–´—´¡´¢´¥´©´¬´±´³´µ´·´»´½´Á´Å´É´Ó´á´â´å´æ´è´é´ê"
"´ë´ñ´ó´ô´õ´ö´÷´ø´ú´üµAµBµEµIµQµSµUµWµaµbµcµeµiµkµlµqµsµtµuµvµwµ{µ|µ}µ"
"µ…µ‰µ‘µ“µ•µ–µ¡µ¢µ¥µ©µªµ«µ­µ°µ±µ³µµµ·µ¹µÁµÂµÅµÉµÑµÓµÕµÖµ×µáµâµåµñµõµ÷¶A"
"¶B¶E¶I¶Q¶S¶U¶W¶a¶b¶e¶i¶q¶s¶u¶w¶¶‚¶…¶‰¶Š¶‹¶‘¶“¶•¶—¶¡¶¢¶¥¶©¶±¶³¶¶¶·¶Á¶Â"
"¶Å¶É¶Ñ¶Ó¶×¶á¶â¶å¶é¶ñ¶ó¶õ¶÷·A·B·E·I·Q·S·U·W·Y·a·b·e·i·o·q·s·u·w·x·y·z·{"
"·|·}··…·‰·‘·•·¡·¢·¥·©·ª·«·°·±·³·µ·¶···¸·¼¸a¸b¸e¸g¸h¸i¸k¸q¸s¸u¸v¸w¸x¸"
"¸‚¸…¸‰¸‘¸“¸•¸–¸—¸¡¸¢¸¥¸§¸©¸±¸·¸Á¸Å¸É¸á¸â¸å¸é¸ë¸ñ¸ó¸õ¸÷¸ø¹A¹B¹E¹I¹Q¹S¹U"
"¹W¹a¹e¹i¹q¹s¹v¹w¹¹¡¹¢¹¥¹©¹«¹±¹³¹µ¹·¹¸¹¹¹½¹Á¹Â¹É¹Ó¹Õ¹×¹á¹ö¹÷ºAºEºIºQºS"
"ºUºWºaºbºeºwºº‚º…º‰ºŠº‹º‘º“º•º—º¡º¶ºÁºáºâºåºéºñºóºõ»A»E»I»Q»a»b»e»i»q"
"»s»u»w»¡»¢»¥»¨»©»«»±»³»µ»·»¸»»»¼¼a¼b¼e¼g¼i¼l¼q¼s¼u¼v¼w¼¼‚¼…¼‰¼‘¼“¼•¼–"
"¼—¼¡¼¥¼·¼á¼â¼å¼é¼ñ¼ó¼õ¼ö¼÷½A½W½a½v½¡½¢½¥½©½±½³½µ½·½¹½Á½Â½É½Ö½á½ö¾A¾E¾I"
"¾Q¾S¾w¾¾‚¾…¾‰¾‘¾“¾—¾¡¾¶¾·¾á¿A¿a¿q¿u¿w¿¡¿¢¿¥¿©¿±¿³¿·¿¸¿½ÀaÀbÀeÀgÀiÀqÀs"
"ÀuÀvÀwÀxÀÀ‚À…À‰À‘À“À•À–À—À¡À¥À§À©À±À·ÀáÀâÀåÀéÀñÀóÀõÀöÀ÷ÁAÁBÁEÁIÁQÁSÁU"
"ÁWÁaÁeÁvÁÁ…Á—Á¡Á¢Á¥Á©Á±Á³ÁµÁ·ÁÁÁÅÁÉÁ×ÂAÂEÂIÂQÂSÂUÂWÂaÂqÂÂ‚Â…Â‰Â‘Â“Â•"
"Â—Â¡Â¶ÂÁÂÅÂáÂåÂéÂñÂóÂõÂ÷ÃAÃEÃIÃQÃWÃaÃbÃeÃiÃqÃsÃuÃwÃ¡Ã¢Ã¥Ã¨Ã©ÃªÃ±Ã³ÃµÃ·"
"ÄaÄbÄeÄiÄqÄsÄuÄwÄÄ‚Ä…Ä‰Ä‘Ä“Ä•Ä–Ä—Ä¡Ä¢Ä·ÄáÄâÄåÄèÄéÄñÄóÄõÄöÄ÷ÅAÅBÅEÅIÅQ"
"ÅSÅUÅWÅaÅeÅiÅqÅsÅuÅvÅwÅÅ¡Å¢Å¥Å©Å±Å³ÅµÅ·ÅÁÅÂÅÅÅÉÅÑÅ×ÅáÅ÷ÆAÆIÆaÆÆ‚Æ…Æ‰"
"Æ‘Æ“Æ•Æ—Æ¡Æ¥Æ©Æ·ÆÁÆ×ÆáÆâÆåÆéÆñÆóÆõÆ÷ÇAÇEÇIÇQÇaÇbÇeÇiÇqÇsÇwÇ¡Ç¢Ç¥Ç©Ç±Ç³"
"ÇµÇ·ÈaÈbÈeÈiÈjÈqÈsÈuÈvÈwÈÈ‚È…È‰È‘È“È•È–È—È¡È·ÈáÈâÈåÈéÈëÈñÈóÈõÈöÈ÷ÉAÉB"
"ÉEÉIÉQÉSÉUÉWÉaÉeÉvÉÉ…É¡É¢É¥É©É±É³ÉµÉ·É¼ÉÁÉÅÉáÊAÊEÊUÊWÊaÊÊ‚Ê…Ê‰Ê‘Ê“Ê•"
"Ê—Ê¡Ê¶ÊÁÊáÊâÊåÊéÊñÊóÊ÷ËAËEËIËQËWËaËbËeËhËiËkËqËsËuËË…Ë‰Ë‘Ë“Ë¡Ë¢Ë¥Ë©Ë±"
"Ë³ËµË·ÌaÌbÌcÌeÌiÌkÌqÌsÌuÌvÌwÌ{ÌÌ‚Ì…Ì‰Ì‘Ì“Ì•Ì–Ì—Ì¡Ì¢ÌáÌâÌåÌéÌñÌóÌõÌöÌ÷"
"ÍAÍBÍEÍIÍQÍSÍUÍWÍaÍeÍiÍqÍsÍvÍwÍÍ‰Í“Í•Í¡Í¢Í¥Í©Í±Í³ÍµÍ·ÍÁÍ×ÎAÎEÎaÎeÎiÎs"
"ÎuÎÎ‚Î…ÎˆÎ‰Î‹Î‘Î“Î•Î—Î¡Î·ÎáÎåÎéÎñÎõÏAÏEÏIÏQÏUÏWÏaÏeÏiÏqÏsÏuÏ¡Ï¢Ï¥Ï©Ï±"
"Ï³ÏµÏ·ÐaÐbÐeÐiÐnÐqÐsÐuÐwÐÐ‚Ð…Ð‰Ð‘Ð“Ð•Ð–Ð—Ð¡Ð·ÐáÐâÐåÐéÐëÐñÐóÐõÐ÷ÑAÑBÑE"
"ÑIÑQÑSÑUÑWÑaÑbÑeÑiÑqÑsÑuÑvÑwÑÑ…Ñ‰Ñ“Ñ¡Ñ¢Ñ¥Ñ©Ñ®Ñ±Ñ³ÑµÑ·Ñ»ÑÁÑÂÑÅÑÉÑÕÑ×Ñá"
"ÑâÑåÑõÑ÷ÒAÒBÒEÒIÒSÒUÒWÒaÒeÒiÒsÒuÒÒ‚Ò…Ò‰ÒŽÒ‘Ò•Ò—Ò¡Ò¥Ò©Ò±Ò·ÒÁÒÂÒÅÒÉÒ×Òá"
"ÒâÒåÒéÒñÒóÒõÒ÷ÓAÓBÓEÓIÓQÓUÓWÓaÓbÓeÓgÓhÓiÓjÓqÓsÓuÓwÓ{ÓÓ…Ó‰Ó‘Ó“Ó—Ó¡Ó¢Ó¥"
"Ó©Ó±Ó³ÓµÓ·"
};

class wansung2johap  
{
public:
	unsigned char * Input_Wansung(unsigned char *hangul_string);
	int Convert_Start(unsigned char *hg);
	wansung2johap();
	virtual ~wansung2johap();

};

#endif // !defined(AFX_WANSUNG2JOHAP_H__DDC7DCA4_5C4C_4155_847D_E1CFF8857410__INCLUDED_)
