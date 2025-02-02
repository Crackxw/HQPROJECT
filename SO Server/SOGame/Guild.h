// 熱薑 橾濠 : 2002-02-18 螃瞪 11:15:33 - 曄薑賅

#ifndef	_GUILD_HEADER
#define _GUILD_HEADER

#include "SOCommonHeader.h"
#include "GSCDefine.h"
#include "SOVillage.h"
#include "Stack.h"
#include "GuildMemberManager.h"


#define	ON_GUILD_RELATION_ALLY			1			// 翕裊
#define	ON_GUILD_RELATION_ENEMY			2			// 瞳
#define	ON_GUILD_RELATION_NEUTRAL		3			// 醞董			餌辨x
#define	ON_GUILD_RELATION_MINE			4			// 釭曖 鼻欽.	餌辨x
#define ON_GUILD_RELATION_WAR			5			// 奢撩

#define ON_GUILD_UNIT_SWORD				0			// 匐煽
#define ON_GUILD_UNIT_SPEAR				1			// 璽煽
#define ON_GUILD_UNIT_HORSE				2			// 晦葆煽
#define ON_GUILD_UNIT_ARROW				3			// 掙煽
#define ON_GUILD_UNIT_DEFENSE			4			// 熱綠煽
#define ON_GUILD_UNIT_RAMPART			5			// 醱離.
#define ON_GUILD_UNIT_SIEGE				6			// 嫦戮剪.
#define ON_GUILD_UNIT_SUPPLY			7			// 爾晝熱溯.
#define ON_MAX_GUILD_UNIT				8

#define ON_MAX_GUILD_RESULT				20



enum {
	    ON_GUILD_GIVEUP_NORMAL = 0,
        ON_GUILD_GIVEUP_NOT_ANSWER,
//		ON_GUILD_GIVEUP_NOT_SEND,
		ON_GUILD_GIVEUP_OK,
		ON_GUILD_GIVEUP_NO,
};

//鼻欽曖 奢撩嶸棉擊 婦葬ж朝 贗楚蝶.
class   cltGuildUnit				
{
private:
	SI16 siGuildUnit[ON_MAX_GUILD_UNIT];								// ⑷營 陛雖堅 氈朝 奢撩 嶸棉.
public:
	void	InitGuildUnit();
	void	SetGuildUnit(SI16 siUnitKind,SI16 siCount);
	SI16	IncreaseGuildUnit(SI16 siUnitKind,SI16 siCount);	// ⑷營 陛雖堅 氈朝 奢撩 嶸棉擊 隸陛 衛鑑棻.
	SI16	DecreaseGuildUnit(SI16 siUnitKind,SI16 siCount);
	SI16	GetGuildUnit(SI16 uiUnitKind);

};

class	cltGuildResult
{
private:
	SI16 siVillageCode;
	SI16 siPercent;
	
public:
	char szCharName[ON_ID_LENGTH + 1];

	void	SetCharName(char* szName);
	char*   GetCharName();
	void	SetVillageCode(SI16	siVillageCode);
	SI16	GetVillageCode();
	void	SetPercent(SI16	siPer);
	SI16	GetPercent();
	
};

/*class   cltVillageDamageByGuild
{
	SI16 siVillageCode;
	SI32 siSoldierDamage;
};*/

typedef struct tag_giveUpInfo
{
	SI32            uiVillageCode;  //л塊脹 葆擊
	SI16            siGiveUpFlag;   //
	MONEY           mnMoney;        //奢撩ん晦 綠辨

}GIVEUP_INFO, *LP_GIVEUP_INFO;

class	cltGuild
{
private:	
	UI32				m_uiArrayIndex;								// 寡翮 檣策蝶.
	UI32				m_uiGuildID;								// 鼻欽曖 ID
	CHAR				m_szGuildName[ON_GUILDNAME_LENGTH + 1];		// 鼻欽曖 檜葷.
	CHAR				m_szDAIID[ON_ID_LENGTH + 1];				// 渠寞曖 檜葷.

	SI32				m_siFlag;									// 梓嫦.
	SI32				m_siFlagIDInStack;							// 蝶鷗縑憮曖 梓嫦 ID.
	SStack<UI16>		m_Villages;									// 葆擊菟.	

	GIVEUP_INFO         m_GiveUpInfo;
	BOOL                m_bIsWar;                                   // 奢撩瞪擊 й熱氈釭?
	
public:
	
	GuildMemberManager	m_GuildMemberManager;						// ⑷ 鼻欽縑 賅萇 鼻欽錳菟擊 婦葬и棻.
	SI32				m_siLoginedGuildMemeberNum;					// ⑷營 煎斜檣и 鼻欽錳菟曖 熱.

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//奢撩 蝓葬衛曖 掏и.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	cltGuildResult		m_stGuildResult[ON_MAX_GUILD_RESULT];		// 鼻欽檜 奢問и 葆擊曖 VillageCode諦 
																	// ァ癱 Percent陛 菟橫除棻.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//鼻欽 僭ヶ 婦溼.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	cltGuildUnit		m_GuildUnit;								// 鼻欽檜 陛雖堅 氈朝 奢撩 嶸棉.
	SI32				m_siGuildSupply;							// 鼻欽檜 陛雖堅 氈朝 爾晝ヶ曖 偎熱.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//瞳渠 婦溼
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	SI32				m_siGuildRelation[SO_TOTAL_GUILD_NUM];				// 陝 鼻欽婁曖 婦啗.
	SI32				m_siGuildRelationEnemyTime[SO_TOTAL_GUILD_NUM];		// 陝 鼻欽檜 瞳渠婦啗橾 陽 瞳渠蒂 褐羶и 衛除.
																			// 瞳渠 勘堅 3橾�� 奢撩檜 衛濛脹棻.
	char				m_szGuildfriendlyMessage[SO_TOTAL_GUILD_NUM][200];	// �俸�擊 錳й 唳辦 詭撮雖蒂 爾魚棻.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
private:

	static	SOWorld	*m_pMyWorld;
private:
	UI32	FindEmptySlot();						// 詹幗蒂 蹺陛й 綠橫氈朝 蝸煜擊 瓊朝棻.	
	UI32	FindSlot(UI16 uiAccount);				// 詹幗陛 撲薑腎橫 氈朝 蝸煜擊 瓊朝棻.		

	//VOID	UpdateMaxRealMemberNum();				// 爾嶸ж堅 氈朝 葆擊曖 熱蒂 撲薑и棻.
public:
	cltGuild();										// 儅撩濠.
	~cltGuild();									// 模資濠.

	VOID	OnceInit(SI32 siTotalVillageNum);		// и廓虜 蟾晦�� п還匙菟擊 п遽棻.
	VOID	Init();									// 詹幗 滲熱菟擊 蟾晦�倆挬�.	
	VOID	CreateGuild(UI32 uiArrayIndex, 
						UI32 uiGuildID, 
						CHAR *pszGuildName,
						SOPlayer *pPlayer,
						SOVillage *pVillage);			// 鼻欽擊 儅撩и棻.
	VOID	InitGuild(	UI32 uiArrayIndex, 
						UI32 uiGuildID, 
						CHAR *pszGuildName,
						CHAR *pszDAIID,						
						UI32 uiGuildMemeberNum, SI32 siFlag, 
						SI32 siFlagIDInStack);		// DB縑憮 檗橫諦 鼻欽擊 檗橫螞棻.
	BOOL	DeleteGuild();							// 鼻欽擊 餉薯и棻.	

	UI32	GetGuildID();							// 鼻欽曖 ID蒂 橢橫螞棻.
	CHAR*	GetGuildName();							// 鼻欽曖 檜葷擊 橢橫螞棻.	
	CHAR*	GetDAIID();								// 渠寞曖 ID(僥濠翮)擊 橢橫螞棻.
	UI16	GetOwnerVillageNum();					// 模嶸ж堅 氈朝 葆擊曖 熱蒂 剩啖遽棻.
	UI32	GetGuildMemberNum();					// 詹幗 �蛾虃鷏� 剩啖遽棻.
	BOOL	GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass);		// 議葛攪 檜葷擊 餌辨ж罹 鼻欽曖 ID諦 望萄 霜晝擊 橢橫螞棻.


	BOOL	AddGuildMember(CHAR *pszName);										// 鼻欽縑 鼻欽 詹幗陛 蹺陛腎歷棻.
	BOOL	DelGuildMember(CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick);		// 鼻欽縑憮 鼻欽 詹幗陛 餉薯腎歷棻. bKick檜 撢腎橫氈戲賊 覲嬴魚匙歜
	BOOL	AddCandidate(SOPlayer *pPlayer);									// 雖錳濠蒂 蹺陛и棻.	
	VOID	AddVillage(SOVillage *pVillage, BOOL bInit);						// 鼻欽縑 葆擊檜 蹺陛腎歷棻.
	BOOL	DelVillage(SOVillage *pVillage, BOOL bDeleteGuild);					// 鼻欽縑憮 葆擊檜 餉薯腎歷棻.

	BOOL	IsValidGuild();													// 嶸�褲� 望萄檣雖 憲嬴螞棻.
	BOOL	Notice(CHAR *pszName, CHAR *pszNotice);							// 奢雖餌о擊 爾頂遽棻.	

	UI32	GetArrayIndex();						// 寡翮 檣策蝶蒂 橢橫褥.

	BOOL	ChangeFlag(SI32 siFlag);				// 梓嫦擊 撲薑и棻.
	SI32	GetFlag();								// 梓嫦擊 橢橫螞棻.

	BOOL	SetHNS(SOVillage *pVillage);									// ч熱蒂 雖薑и棻.

	static	VOID InitStaticVariable(SOWorld *pWorld);	// 薑瞳 滲熱蒂 蟾晦�� и棻.

	VOID	SetFlagIDInStack(SI32 siFlagIDInStack);		// 蝶鷗縑憮曖 梓嫦 ID蒂 撲薑и棻.
	SI32	GetFlagIDInStack();							// 蝶鷗縑憮曖 梓嫦 ID蒂 橢橫螞棻.

	BOOL	ChangeClass( CHAR *pszName, SI32 siNewClass );	// 霜晝擊 滲唳衛鑑棻.
	VOID	GetOwnerVillages(UI16 *puiOwnerVillageNum, UI16 *puiVillageCode);								// ⑷營 檜 葆擊縑 模樓脹 葆擊曖 囀萄蒂 橢橫螞棻.
	VOID	GetOwnerGuildVillageInfo( UI16 *puiOwnerVillageNum, OnGuildVillageInfo *pGuildVillageInfo );	// ⑷營 檜 葆擊縑 模樓脹 葆擊曖 晦獄 薑爾蒂 橢橫螞棻.

	DWORD	GetRMTotalPageNum();															// 瞪羹 薑衝 鼻欽錳擊 �飛橦� 爾罹輿晦 嬪п в蹂и む檜雖熱蒂 橢橫螞棻.
	DWORD	GetCMTotalPageNum();															// 瞪羹 雖錳濠蒂 �飛橦� 爾罹輿晦 嬪п в蹂и む檜雖熱蒂 橢橫螞棻.
	UI32	GetRMList(DWORD dwPage, strGuildMemberInfo *pGuildMemeberInfo);					// 薑衝 鼻欽錳曖 葬蝶お蒂 橢橫螞棻.
	UI32	GetCMList(DWORD dwPage, strCandidateInfo *pCandidateInfo);						// 雖錳濠 葬蝶お蒂 橢橫螞棻.

	BOOL	SetMemberInfoFromDB();															// DB煎 睡攪 賅萇 鼻欽錳曖 薑爾蒂 橢橫螞棻.

	VOID	SendMsgToGuildMemebers(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL);	// 蕾樓п 氈朝 鼻欽錳縑啪 詭衛雖蒂 爾頂遽棻.		

	VOID	GuildMemberLogin();
	VOID	GuildMemberLogout();

	void	GuildInfoChanged();
	BOOL	GetAuditGuildInfo(SOPlayer* pPlayer);

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 奢撩 婦溼 蹺陛,熱薑 л熱菟
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID	InitRelation(UI32 uiArrayIndex);												// 鼻欽曖 婦啗蒂 蟾晦�� и棻.
	VOID	SetRelation(cltGuild *pGuild, SI32 siRelation, BOOL bInit,SI32 siDbTime);		// 鼻欽曖 婦啗蒂 撲薑и棻.
	SI32	GetRelation(UI32 uiArrayIndex);													// 鼻欽曖 婦啗蒂 橢橫螞棻.
	BOOL	IsEnemyGuild(UI32 uiArrayIndex);												// 瞳渠瞳檣 婦啗檣雖 憲嬴螞棻.
	BOOL	IsWarGuild(UI32 uiArrayIndex);													// 奢撩 醞檣雖 憲嬴螞棻.
	void	SetRelationReLoad();															// ж瑞縑 и廓噶 鼻欽除曖 婦啗蒂 營撲薑 п遽棻.

	SI32    IncreaseGuildSupply(SI32 siSupply);							// 鼻欽曖 爾晝ヶ 隸陛.
	SI32	DecreaseGuildSupply(SI32 siSupply);							// 鼻欽曖 爾晝ヶ 馬模.
	SI32	GetGuildSupply();
	void	SetGuildSupply(SI32	siSupply);

	
	char*   GetFrieldlyMessage(UI32 uiArrayIndex);
	BOOL	SetFriendlyMessage(cltGuild *pGuild,CHAR* pszFriendlyMsg);	// �俸�薯曖蒂 и棻.
	BOOL	SetFriendlyMessageAccept(cltGuild*	pGuild);				// �俸�薯曖 熱塊.
	BOOL	SetFriendlyMessageCancel(cltGuild*	pGuild);				// �俸�薯曖 剪瞰.
	BOOL	SetMessage(cltGuild* pGuild,CHAR* pszMsg);					

	BOOL	SetPowerInvestmentEnd();									// ァ癱掏и 餉薯.
	BOOL	SendGuildItemDeleteMessage();								// 鼻欽檜 橈橫雖賊 鼻欽曖 奢撩 婦溼 等檜攪蒂 陳萼棻.

//	VOID	RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode ,SI16 siRewardSupply);	//渠寞縑啪 絲婁 癱濠掏и擊 遽棻.
	VOID	RewardToGuildDAIForDestroyVillage( MONEY mnReward, UI16 uiVillageCode );	//渠寞縑啪 絲婁 癱濠掏и擊 遽棻.
	
	BOOL    IsWar() { return m_bIsWar; }                                //鼻欽檜 奢撩醞檣陛....                                                                      
	VOID    SetWarFlag(bool bFlag) { m_bIsWar = bFlag; }

	BOOL    CheckWar();
	BOOL    CheckDay(SI16 siYear, SI16 siMon, SI16 siDay); //奢撩擊 й熱氈朝雖 匐餌 и棻.
	BOOL    CheckDay2(SI16 siYear, SI16 siMon, SI16 siDay);
	
	void    SendWarTime();
	BOOL    CheckVillage(UI16 uiVillageCode);
	
//奢撩 ん晦 婦溼	
	VOID    SetGiveUpInfo(UI16 uiVillageCode, MONEY mnMoney, SI16 siFlag);
	VOID    GetGiveUpInfo(UI16 *puiVillageCode, MONEY *pmnMoney);
	
	VOID    SetGiveUpFlag(SI16 siFlag);
	SI16    GetGiveUpFlag();

	int     GetgiveUpVillageCode() { return m_GiveUpInfo.uiVillageCode;	}

	VOID    GiveUp();
	void    SendGiveUpMsg();

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
};

#endif
