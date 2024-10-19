#ifndef _ROCKET_NEW_TILE_MAP

#ifndef _FILE_H
#define _FILE_H   1

#include <cltString.h>

#define MAX_MAP_TILE_SPR_NUMBER				61	// 지도 타일용 스프라이트의 갯수

// (주의) 추가할 때는 맨 끝에 추가할 것

//----------------------------------------------
// <임진록2> 위 define 중에서 사용되는 define
#define FILE_GAME_MOUSE         			0
#define FILE_ITEMBOX_SELECTED_LINE			1
#define FILE_GAME_MISSILE1					2
#define FILE_GAME_CANNON0       			3
//#define FILE_GAME_EXP0           			4
//#define FILE_GAME_EXP1           			5
//#define FILE_GAME_EXP2           			6
//#define FILE_GAME_EXP3           			7
//#define FILE_GAME_EXP4           			8
//#define FILE_GAME_EXP5           			9
//#define FILE_GAME_EXP6           			10
//#define FILE_GAME_EXP7           			11
//#define FILE_GAME_EXP8           			12
//#define FILE_GAME_EXP9           			13
//#define FILE_GAME_EXP10          			14
//#define FILE_GAME_EXP11          			15
//#define FILE_GAME_EXP12          			16
//#define FILE_GAME_EXP13          			17
//#define FILE_SINGERMGHOST3				18
#define FILE_CROP0              			19
#define FILE_CROP1              			20
//#define FILE_TREE0							21
#define FILE_MOVEMARK           			22
//#define FILE_IMJIN2						23
#define FILE_RAIN0              			24
#define FILE_RAIN1              			25
#define FILE_TEAMNUMBER         			26
#define FILE_SELECTBOX          			27
#define FILE_HEAL               			28
//#define FILE_BUDAFIRE						29
//#define FILE_BUDAFIRE2						30
//#define FILE_BUDAFIRE3						31
//#define FILE_ARROW							32
//#define FILE_NEEDLE							33
#define FILE_MONEYNUMBER        			34
#define FILE_SMALLNUMBER        			35
#define FILE_RESOURCE           			36
#define FILE_SMOKE1             			41
#define FILE_ELECTRON           			47
#define FILE_EARTHQUAKE         			50
//#define FILE_MAGICBALL           			51
#define FILE_LIGHTNING1         			52
//#define FILE_LIGHTNINGEXP                 53    
#define FILE_TREASURE           			54
#define FILE_PROTECTOR          			55
#define FILE_TEXT1              			56
#define FILE_CREW               			57
#define FILE_FIREHOUSESMOKE     			58
#define FILE_SHORTCUTKEY        			59
//#define FILE_PLAYERSTART        			60
#define FILE_MAGICK             			61
#define FILE_MAGICJ             			62
#define FILE_HELPRESOURCE       			63
#define FILE_LEVEL              			64
//#define FILE_BULLET          				65
#define FILE_BUILDEXP1          			66
#define FILE_STEALEXP           			67
#define FILE_CLOCK              			68
#define FILE_RUIN0              			69
#define FILE_RUIN1              			70
#define FILE_RUIN2              			71
#define FILE_RUIN3              			72
#define FILE_EVENTMARK          			73
#define FILE_BUILDRUIN          			74
#define FILE_SHIELD             			75
#define FILE_GENERAL2KFIRE      			76
#define FILE_SATFX							77
//#define FILE_BOMBEXP1						78
//#define FILE_BOMBEXP2						79
//#define FILE_BOMBEXP3						80
//#define FILE_TORNADO						81
#define FILE_TEAMFLAG           			82
#define FILE_PORTRAITTEAMNUMBER 			83

// 임진록2 사용되는 define			
//----------------------------------------------
			
	
// 캐릭터 화일 			
//#define FILE_CHAR_SWORD_K       			84
//#define FILE_CHAR_HARUBANG       			85
//#define FILE_CHAR_CRAB	      			86
//#define FILE_AXET			     			87
//#define FILE_AXEUPT		    			88
//#define FILE_CHAR_FARMERK       			89
//#define FILE_ICETREE           			90
//#define FILE_BADMERCHANT         			91
//#define FILE_CHAR_HORSEARCHERK  			92
//#define FILE_MADBULL		      			93
//#define FILE_BUDAUPT		       			94
//#define FILE_CHAR_BUDAK         			95
//#define FILE_CHAR_GUNJ1         			96
//#define FILE_CHAR_GUNJ2         			97
//#define FILE_CHAR_GUNJ3         			98
//#define FILE_CHAR_HORSESWORDJ  			99
//#define FILE_CHAR_GHOSTCAT				100		// 귀묘 
//#define FILE_SNOWLIZARD       			101
//#define FILE_CHAR_STABLEK       			102
//#define FILE_CHAR_HEAVYMAKERK   			103
//#define FILE_CHAR_HEAVYMAKERJ   			104
//#define FILE_SKYNINE	         			105
//#define FILE_CHAR_ADVBUDAJ      			106
//#define FILE_GUNUPT		     			107
//#define FILE_CHAR_AIRMAKERK     			108
//#define FILE_AMAZONUPT        			109
//#define FILE_CHAR_TOWERK        			110
//#define FILE_CHAR_TEMPLEK       			111
//#define FILE_BIGTREE		     			112
//#define FILE_CHAR_SHIPYARDK     			113
//#define FILE_CHAR_SHIPYARDK2    			114
//#define FILE_GENERAL4T	     			115
//#define FILE_CHAR_NINJAJ        			116
//#define FILE_CHAR_HQC           			117
//#define FILE_ELEBARRACKC	      			118
//#define FILE_CHAR_FIREHOUSEC    			119
//#define FILE_GENERAL5T		   			120
//#define FILE_CHAR_SHIPYARDC     			121
//#define FILE_CHAR_AIRMAKERC     			122
//#define FILE_CHAR_HQK           			123
//#define FILE_FARMERT      				124
//#define FILE_CHAR_MILLC         			125
//#define FILE_CHAR_FARMERJ       			126
//#define FILE_CHAR_MILLK         			127
//#define FILE_SWORDT		     			128
//#define FILE_HOOK		         			129
//#define FILE_GUNT          				130
//#define FILE_MBANDIT2         			131
//#define FILE_CHAR_GENERALK11    			132
//#define FILE_CHAR_GENERALK12    			133
//#define FILE_CHAR_GENERALK13    			134
//#define FILE_CHAR_GENERALK2    			135
//#define FILE_CHAR_GENERALK3    			136
//#define FILE_CHAR_GENERALK4     			137
//#define FILE_CHAR_GENERALK5    			138
//#define FILE_LEECH						139
//#define FILE_SEAHORSE						140
//#define FILE_CHAR_GENERALK6     			141
//#define FILE_CHAR_GENERALJ1    			142
//#define FILE_THIEF						143
//#define FILE_SEAKING						144
//#define FILE_CHAR_GENERALJ2    			145
//#define FILE_CHAR_GENERALJ3    			146
//#define FILE_CHAR_GENERALJ4    			147
//#define FILE_CHAR_GENERALJ5     			148
//#define FILE_CHAR_GENERALC11    			149                                                                                                                                                                                                                                                                                                                                                                                                                                             
//#define FILE_CHAR_GENERALC12    			150
//#define FILE_CHAR_GENERALC13    			151
//#define FILE_CHAR_GENERALC2     			152
//#define FILE_CHAR_GENERALC31    			153
//#define FILE_CHAR_GENERALC32    			154                          
//#define FILE_CHAR_GENERALC33    			155                          
//#define FILE_CHAR_GENERALC41    			156
//#define FILE_CHAR_GENERALC42    			157                           
//#define FILE_CHAR_GENERALC43    			158                           
//#define FILE_CHAR_GENERALC51    			159
//#define FILE_SWORDUPT		     			160
//#define FILE_CHAR_TANKC         			161
//#define FILE_CHAR_ADVTOWERK     			162
//#define FILE_PIRATES		    			163
//#define FILE_CHAWOO		     			164
//#define FILE_CHAR_ASHIPK        			165
//#define FILE_CHAR_TOWERC        			166
//#define FILE_BANARIANSHAMAN    			167
//#define FILE_AMAZONT          			168
//#define FILE_SCORPION		    			169
//#define FILE_CHAR_PSHIPK        			170
//#define FILE_SNAKE	         			171
//#define FILE_BABARIAN	        			172
//#define FILE_CHAR_CARJ          			173
//#define FILE_CHAR_TEMPLEC       			174
//#define FILE_CROCODILE         			175
//#define FILE_BUDAT			   			176
//#define FILE_MBANDIT1	        			177
//#define FILE_BIGSWORD	        			178
//#define FILE_GENERAL1T	       			179
//#define FILE_CHAR_ADVTEMPLEK    			180
//#define FILE_PANDER	        			181
//#define FILE_BADDEER			   			182
//#define FILE_GENERAL2T        			183
//#define FILE_CHAR_ADVTOWERJ     			184
//#define FILE_MAGICIAN		    			185
//#define FILE_GENERAL3T		   			186
//#define FILE_CHAR_MILLLABC      			187
//#define FILE_CHAR_KOREANKING    			188
//#define FILE_CHAR_FLAGMAN					189
//#define FILE_IMJIN2PLUS					190
//#define FILE_CHAR_GENERALK7				191
//#define FILE_CHAR_GENERALJ6				192
//#define FILE_CHAR_CANNON2K1				193
//#define FILE_CHAR_CANNON2K2				194
//#define FILE_SMALLGANG					195
//#define FILE_CHAR_GENERALK8     			196
//#define FILE_HORSEMAN						197
//#define FILE_CHAR_GENERALJ7				198
//#define FILE_CHAR_EAGLEC3					199
//#define FILE_TREE_PLUS					200
//#define FILE_CHAR_GENERALJ8				201
#define FILE_GROUNDFIRE  					202
//#define FILE_CHAR_GENERALC7				203
//#define FILE_CHAR_GENERALC6				204
//#define FILE_CHAR_HEAVYGUNC				205
//#define FILE_ADVTOWERKEXP					206
#define FILE_CLOCK_PLUS						207
#define FILE_GAME_CLOUD						208
#define FILE_GAME_WIND						209
//#define FILE_CHAR_JUSTICEK				210
//#define FILE_GAME_GENERAL7K_BOMB			211
#define FILE_GAME_ITEMLIGHT					212
//#define FILE_CHAR_NINJAJ2        			213
//#define FILE_GAME_DART        			214
//#define FILE_MAGIC8K						215
//#define FILE_MAGIC8K_EXP					216
//#define FILE_MAMMOTH						217
//#define FILE_WILDBOAR						218
//#define FILE_MAGIC6C						219
#define FILE_ITEMBOX_LIST					220
#define FILE_ITEMBOX_CANCEL					221
#define FILE_ITEMBOX_SCROLL					222
#define FILE_ITEMBOX_UPDOWN					223
//#define FILE_MAGIC4C						224
//#define FILE_MAGIC4CEXP					225
//#define FILE_MAGIC6CBULLET				226
#define FILE_MAGIC6CSTICK					227
#define FILE_GROUNDFIRESMOKE				228
#define FILE_ITEMBOX_GRAY_LINE				229
//#define FILE_SINGERMGHOSTEXP				230
//#define FILE_BURNARROW					231
//#define FILE_ARROWTRAIL					232
//#define FILE_SPECIAL						233
//#define FILE_HORSEARCHERK_PLUS			234
//#define FILE_WATEREFFECT					235
//#define FILE_DUST							236
//#define FILE_SINGERMGHOST2				237
//#define FILE_MAGIC5J						238
#define FILE_RUIN4              			239
//#define FILE_VOLCANO_BULLET             	240
//#define FILE_CHAR_HQK_PLUS				241
//#define FILE_CHAR_SWORD_K_PLUS			242
//#define FILE_CHAR_ARCHER_K_PLUS			243
//#define FILE_ON_CHAR_KEON_1HS				244
//#define FILE_IMJINONLINE					245
//#define FILE_ON_CHAR_EATERDEMON			246
//#define FILE_ON_CHAR_WEIRDPLANT			247
//#define FILE_ON_CHAR_HUNGRYDEMON			248
//#define FILE_ON_CHAR_TIGER				249
//#define FILE_ON_CHAR_DOGDEMON				250
//#define FILE_ON_CHAR_ONGHWA				251
//#define FILE_ON_CHAR_BANDIT1				252
#define FILE_BUTTON_PLUS           			253
//#define FILE_DETECTMINEEFFECT				254
//#define FILE_CHAR_TOWERK_PLUS      		255
//#define FILE_ON_CHAR_BANDIT2				256
//#define FILE_ON_CHAR_BANDIT3				257
//#define FILE_ON_CHAR_BANDIT4				258
//#define FILE_CACTUS						259
//#define FILE_FIREHOUSESMOKE_PLUS 			260
//#define FILE_CHAR_MILLK_PLUS     			261
//#define FILE_ON_CHAR_GACKDAGUI   			262
//#define FILE_ON_CHAR_GATPHA    			263
//#define FILE_ON_CHAR_PORTERK     			264
#define FILE_ON_CHARINFO					265
#define FILE_ON_LEVEL						266
#define FILE_ON_LEVEL_NUM					267
#define FILE_ON_BUTTMENU					268
#define FILE_ON_GAGEBAR						269
#define FILE_ON_BUTTON           			270
//#define FILE_ON_CHAR_BOGY					271
//#define FILE_ON_CHAR_FOX   				272
//#define FILE_ON_CHAR_STARFISH   			273
#define FILE_ON_ALLY_PANNEL        			274
#define FILE_ON_ALLY_BUTT          			275
//#define FILE_ON_CHAR_SHIP1				276
//#define FILE_ON_CHAR_ONI					277
#define FILE_ON_CHARINFO_ICON				278
#define FILE_SNOWMAGIC1						279
#define FILE_SNOWMAGIC2						280
#define FILE_ON_PORTRAIT					281
#define FILE_RUIN_CASTLETOWER     			282
#define FILE_RUIN_DESTROYEDCASTLE			283
#define FILE_RUIN_CASTLEGATE         		284
#define FILE_RUIN_CASTLETILE         		285
#define FILE_RUIN_CASTLELARGEGATE        	286
//#define FILE_ON_CHAR_SNOWDEMON			287
//#define FILE_ON_CHAR_FURHEADDEMON			288
//#define FILE_ON_CHAR_FIREDRAGON			289
//#define FILE_ON_CHAR_BAT					290
//#define FILE_ON_CHAR_TOAD					291
//#define FILE_ON_CHAR_CENTIPEDE			292
//#define FILE_ON_CHAR_DUEOKSINI			293
//#define FILE_ON_CHAR_GANGSI1   			294
//#define FILE_ON_CHAR_GANGSI2   			295
//#define FILE_ON_CHAR_GANGSI3   			296
//#define FILE_GATPHAPOISON					297
//#define FILE_ON_CHAR_RACCOONDOG  			298
//#define FILE_ON_CHAR_DEER   				299
//#define FILE_ON_CHAR_SWORDK  				300
//#define FILE_ON_CHAR_ARCHERK 				301
//#define FILE_ON_CHAR_SWORDJ  				302
//#define FILE_ON_CHAR_GUNJ  				303
//#define FILE_ON_CHAR_SWORDPIRATE			304
//#define FILE_ON_CHAR_GUNPIRATE			305
#define FILE_ON_EFFECT_HURT1				306		// 공격당했을 때 출혈 효과1
#define FILE_ON_EFFECT_HURT2				307		// 공격당했을 때 출혈 효과2
#define FILE_ON_EFFECT_FOXCLON_APPEAR		308		// 구미호 분신이 나타날 때 효과
#define FILE_ON_EFFECT_FOXCLON_DIE			309		// 구미호 분신이 죽을 때 효과
//#define FILE_ON_CHAR_SNOWWOMAN			310		// 설녀
#define FILE_ON_LEVELUP						311		// 레벨업시 효과
//#define FILE_ON_CHAR_BUDAK				312		// 조선 파계승
#define FILE_ON_POISONSMOKE					313		// 독무(두꺼비, 지네의 독에 당했을 때)
//#define FILE_ON_CHAR_GOLDDRAGON			314		// 금룡 제자리
//#define FILE_ON_CHAR_GOLDDRAGON2			315		// 금룡 공격1
//#define FILE_ON_CHAR_GOLDDRAGON3			316		// 금룡 공격2
//#define FILE_ON_CHAR_GOLDDRAGON4			317		// 금룡 죽기
#define FILE_ON_GOLDDRAGON_CLOUD			318		// 금룡 주변의 구름 효과
#define FILE_ON_GOLDDRAGON_BULLET			319		// 금룡 발사체 효과
#define FILE_ON_GOLDDRAGON_SMOKE			320		// 금룡에게 폭파당했을때 효과
#define FILE_ON_EFFECT_POISON				321		// 두꺼비, 지네의 정지 애니메이션시 독효과
#define FILE_ON_FURHEADDEMON_BULLET			322		// 털가면이 던지는  얼음
#define FILE_ON_FURHEADDEMON_SMOKE			323		// 털가면이 던진 얼음에 당했을 때
//#define FILE_BULLET_ONGHWA				324     // 온천 원숭이 해골. 	
//#define FILE_BULLET_GHOSTCAT				325     // 귀묘 음파 공격. 
//#define FILE_SMOKE_GHOSTCAT				326     // 귀묘 음파 공격 폭파. . 
//#define FILE_BULLET_DOLL1					327     // 무명인형. 
//#define FILE_BULLET_DOLL2					328     // 하카다인형. 
//#define FILE_MANAGER1						329     // 운영자1
//#define FILE_WHITETIGER					330     // 백호
//#define FILE_TUTTLE						331     // 바다거북이
//#define FILE_BUTTERFLY					332     // 독나방
//#define FILE_WIDOW						333     // 주귀
//#define FILE_BULLET_BUTTERFLYBULLET		334     // 독나방 독. 
//#define FILE_BULLET_WIDOWBULLET			335
//#define FILE_SMOKE_WAVEEFFECT				336
//#define FILE_INSECT						337
//#define FILE_WORRIORGHOST					338
//#define FILE_SINGERMGHOST1				339
//#define FILE_SEAHORSEBULLET				340
//#define FILE_ARCHERUPK					341
//#define FILE_HORSEARCHERUPK				342
//#define FILE_JUSTICEUPK					343
//#define FILE_SWORDUPK						344
//#define FILE_BUDAUPK						345
//#define FILE_SWORDUPJ						346
//#define FILE_NINJAUPJ						347
//#define FILE_GUNUPJ						348
//#define FILE_HORSESWORDUPJ				349
//#define FILE_ADVBUDAUPJ					350
//#define FILE_BEAR							351
//#define FILE_HORSESPEAR					352
//#define FILE_CAT							353
//#define FILE_DUCK							354
//#define FILE_HORNET						355
//#define FILE_MBANDIT3						356
//#define FILE_BULLET_POISONARROW			357     // 독침		(원주민무당) 
//#define FILE_BULLET_THROWSPEAR			358     // 던지는 창(원주민)
//#define FILE_BULLET_SCORPIONBULLET		359
//#define FILE_BULLET_CACTUSBULLET			360
//#define FILE_BIGTREEBULLET				361
//#define FILE_MAGIC4T						362
//#define FILE_MAGIC2T						363
//#define FILE_MAGICIANBULLET				364

// 중국용병
#define FILE_ON_CHAR_PORTER_C  				365		// 중국짐꾼
#define FILE_ON_CHAR_FAN_C		  			366		// 모험가
#define FILE_ON_CHAR_BUDAK_C		  		367		// 수도승
#define FILE_ON_CHAR_MUDOGA_C	  			368		// 무도가
#define FILE_ON_CHAR_SWORD_C		 		369		// 큰칼무사
#define FILE_ON_CHAR_FIREGUN_C		  		370		// 화포수

#define FILE_ON_CHAR_FANUP_C				371		// 고급 모험가 
#define FILE_ON_CHAR_BUDAKUP_C				372		// 고급 수도승
#define FILE_ON_CHAR_MUDOGAUP_C				373		// 고급 무도가
#define FILE_ON_CHAR_SWORDUP_C				374     // 고급 큰칼무사.
#define FILE_ON_CHAR_FIREGUNUP_C			375     // 고급 화포수.

#define FILE_GENERAL1C						376
#define FILE_GENERAL2C						377
#define FILE_GENERAL3C						378
#define FILE_GENERAL4C						379
#define FILE_GENERAL5C						380

#define FILE_RUNNERHAND						381
#define FILE_SANDGOD						382
#define FILE_SEAENVOY						383
#define FILE_VIPER							384
#define FILE_AMAKUSA						385
#define FILE_LAVAMONSTER					386
#define FILE_EVIL							387
#define FILE_BIRD							388
#define FILE_WEASEL							389
#define FILE_SOUL							390
#define FILE_COBRA							391
#define FILE_VOLCANO_FLAME					392
#define FILE_YELLOWSAND						393
#define FILE_VOLCANO_STONE					394
#define FILE_EVIL_BULLET					395
#define FILE_KOREAN_GHOST					396
#define FILE_SKELETON						397
#define FILE_SKELETONSOLDIER				398
#define FILE_PIRATECAPTAIN					399
#define FILE_ON_WANGSOUL					400
#define FILE_ON_PANDA						401
#define FILE_ON_DARKFIRE					402
#define FILE_ON_GHOST						403
#define FILE_ON_TAIWANGHOST					404
#define FILE_ON_JAPANGHOST					405
#define FILE_ON_SMALLTORNADO				406
#define FILE_ON_JAPANGHOST_ATTACK			407
#define FILE_BULLET_KUKET					408
#define FILE_CAP_BEAR						409
#define FILE_ON_AUTA						410
#define FILE_ON_JUCK						411
#define FILE_ON_FEMALESHAMAN				412
#define FILE_ON_QUEEN						413
#define FILE_ON_AUTA_ATTACK					414
#define FILE_ON_QUEEN_SUB1					415
#define FILE_ON_QUEEN_SUB2					416
#define FILE_ON_FIREATTACK					417
#define FILE_ON_RUNNINGFIRE					418
#define FILE_ON_ENERGYPA					419
#define FILE_ON_CHAR_DONKEY					420
#define FILE_ON_FIREENERGYPASTART			421
#define FILE_ON_FIREENERGYPA				422

#define FILE_ON_BUTCHER						423
#define FILE_ON_GENERAL1K					424
#define FILE_ON_DSWORD						425
#define FILE_ON_GENERAL1J					426
#define FILE_ON_BEASTWARRIOR				427
#define FILE_ON_GENERAL6T					428
#define FILE_ON_MAGIC_C						429
#define FILE_ON_GENERAL6_C					430
#define FILE_ON_AGILETIGER					431
#define FILE_ON_PAWNTIGER					432
#define FILE_ON_DOUBLE_SWORD				433			// 고니시 쌍검난무
#define FILE_ON_LONG_SPEAR					434			// 손유창 장창소환
#define FILE_ON_ICE_BARRIER					435			// 순비연 빙석술
#define FILE_ON_OWL							436			// 올빼미
#define FILE_ON_LEOPARD						437			// 표범
#define FILE_ON_HUNSWARRIOR					438			// 흉노전사
#define FILE_ON_HUNSHORSE					439			// 흉노기마병
#define FILE_ON_HUNSHORSEARCHER				440			// 흉노기마궁수
#define FILE_ON_LONG_SPEAR_HIT				441			// 장창소환 히트 이펙트
#define FILE_ON_GATEKEEPER					442			// 문지기
#define FILE_ON_CLAYFIGURE					443			// 기마병마용
#define FILE_ON_PHOENIX						444			// 불사조
#define FILE_ON_JINEMPEROR					445			// 진시황의혼
#define FILE_ON_JINEMPEROR_ATTACK			446			// 진시황의혼 공격
#define FILE_ON_JINEMPEROR_HIT				447			// 진시황의혼 HIT
#define FILE_ON_MEDICALMAN					448			// 의술사
#define FILE_ON_HEOJUN						449			// 허준
#define FILE_ON_COSMIC						450			// 음양사
#define FILE_ON_SEIMEI						451			// 세이메이
#define FILE_ON_YUMRUCK						452			// 염력사
#define FILE_ON_JANGSUNHWA					453			// 장선화
#define FILE_ON_LONGSPEAR					454			// 장창무사
#define FILE_ON_KOAKHU						455			// 곽후
#define FILE_ON_COSMIC_PAPERDOLL			456			// 음양사 종이인형
#define FILE_ON_COSMIC_PAPERDOLL_HIT		457			// 음양사 종이인형 히트

#define FILE_ON_NEEDLE						458			// 침공격
#define FILE_ON_FLOWER_NEEDLE_RAIN			459			// 만천화우
#define FILE_ON_SOUL_CHANGE					460			// 빙의술
#define FILE_ON_SOUL_CHANGE_HIT				461			// 빙의술 히트
#define FILE_ON_STONE_MAGIC_STONE			462			// 석괴술 돌
#define FILE_ON_STONE_MAGIC_SMOKE			463			// 석괴술 연기
#define FILE_ON_GOD_SPEAR					464			// 신창노도(일격필살)
#define FILE_ON_CLOUD_WIND					465			// 풍모술
#define FILE_ON_CLOUD_WIND_HIT				466			// 풍모술 히트
#define FILE_ON_MAGICC_ATTACK				467			// 도술사 공격
#define FILE_ON_SUNBIYOUN_ATTACK			468			// 순비연 공격
#define FILE_ON_SNOWMAN						469			// 눈사람
#define FILE_ON_SANTA_ARROW					470			// 화살쏘는 산타.
#define FILE_ON_SANTA_AXE					471			// 도끼든  산타.
#define FILE_ON_MOTHER_DEER					472			// 루돌이.
#define FILE_ON_SNOW						473			// 눈
#define FILE_ON_RAPTOR						474			// 랩터.
#define FILE_ON_TIRANO						475			// 티라노.
#define FILE_ON_FLYDRAGON					476			// 익룡.
#define FILE_ON_MONKEY						477			// 소환원숭이.
#define FILE_ON_FIREENERGYPA_HIT1			478			// 용장풍 히트1(불투명)
#define FILE_ON_FIREENERGYPA_HIT2			479			// 용장풍 히트2(투명)
#define FILE_ON_BASE_TIMER					480			
#define FILE_ON_ALLY_TITLE					481
#define FILE_ON_ALLY_WINDOW					482
#define FILE_ON_ALLY_CHECK					483
#define FILE_ON_ALLY_BUTTON_OPEN			484
#define FILE_ON_ALLY_BUTTON_CLOSE			485
#define FILE_ON_HELLKING					486
#define FILE_ON_HELLMESSENGER				487
#define FILE_ON_GHOSTSAMURAI				488
#define FILE_ON_PYTHON						489
#define FILE_ON_PYTHON_BULLET				490

#define FILE_ON_GUARDIAN_MOUSE_START		491
#define FILE_ON_GUARDIAN_CAW_START			492
#define FILE_ON_GUARDIAN_TIGER_START		493
#define FILE_ON_GUARDIAN_DRAGON_START		494	
#define FILE_ON_GUARDIAN_HORSE_START		495
#define FILE_ON_GUARDIAN_RABBIT_START		496

#define FILE_ON_GUARDIAN_MOUSE				497
#define FILE_ON_GUARDIAN_TIGER				498
#define FILE_ON_GUARDIAN_DRAGON				499
#define FILE_ON_GUARDIAN_HORSE				500
#define FILE_ON_GUARDIAN_RABBIT				501
#define FILE_ON_GUARDIAN_CAW1				502
#define FILE_ON_GUARDIAN_CAW2				503
#define FILE_ON_GUARDIAN_CAW3				504

#define FILE_ON_GUARDIAN_HORSE_USE			505
#define FILE_ON_GUARDIAN_RABBIT_USE			506

#define FILE_ON_BULLET_GUARDIAN_TIGER		507

#define FILE_ON_GUARDIAN_RABBIT_ATTACK		508

#define FILE_ON_GUARDIAN_CAW_SMOKE			509
#define FILE_ON_GUARDIAN_TIGER_SMOKE		510

#define FILE_ON_GUARDIAN_SHEEP_LIFEUP_USE		511
#define FILE_ON_GUARDIAN_SHEEP_MAGICRESIST_USE	512

#define FILE_ON_GUARDIAN_SNAKE_START		513
#define FILE_ON_GUARDIAN_PIG_START			514
#define FILE_ON_GUARDIAN_CHICKEN_START		515
#define FILE_ON_GUARDIAN_MONKEY_START		516
#define FILE_ON_GUARDIAN_SHEEP_START		517
#define FILE_ON_GUARDIAN_DOG_START			518

#define FILE_ON_GUARDIAN_CHICK				519
#define FILE_ON_GUARDIAN_MONKEY				520
#define FILE_ON_GUARDIAN_PIG				521

#define FILE_ON_GUARDIAN_CHICK_ATTACK		522
#define FILE_ON_GUARDIAN_CHICK_SMOKE		523

// robypark 2004-6-3 16:15
// 천지독살 냉기에 의한 마비 smoke, 흑룡차 기본 공격에 의한 마비 smoke
#define	FILE_ON_FOG_OF_ICE_HIT				555	

#define MAX_GAME_SPR_NUMBER					700



// 일반 그림들을 불러온다. 
void LoadFnt();
BOOL FreeFnt();

// 지도용 타일을 불러온다.
void LoadMapTile(SHORT maptype);
// 지도용 타일을 해제한다.
void FreeMapTile();
SI32 GetValueOfTileID(CHAR *TileID);
BOOL LoadImgFileList();

// 로딩된 시간을 근거로 오래된 이미지를 제거한다.
void FreeOldImageByTime();

SI32 GetValueOfGameSprNumID(CHAR *ID);
// 해당 이미지 파일을 불러온다.
BOOL LoadImgFile(SI32 num);
// 게임 중에 로딩된 이미지 파일에 해당하는 메모리를 모두 해제한다.
void FreeImgFile();

extern int FileTileGrss1;
extern int FileTileDiff1;
extern int FileTileHill0;
extern int FileTileHill1;
extern int FileTileNewBlkGate;
extern int FileTileSea0;
extern int FileTileCastle2;
extern int FileTileBlackTile;
extern int FileTileShallow;

class _FileVary{
public:
	SI32 OnlineLogoFile;
	SI32 LogoFile;
	SI32 TreeFile;

	void Init();
};

BOOL LoadPannel(SI32 sicountry);


#endif




#else

#ifndef _FILE_H
#define _FILE_H   1

#include <cltString.h>

#define MAX_MAP_TILE_SPR_NUMBER				17	// 지도 타일용 스프라이트의 갯수

// (주의) 추가할 때는 맨 끝에 추가할 것

//----------------------------------------------
// <임진록2> 위 define 중에서 사용되는 define
#define FILE_GAME_MOUSE         			0
#define FILE_ITEMBOX_SELECTED_LINE			1
#define FILE_GAME_MISSILE1					2
#define FILE_GAME_CANNON0       			3
//#define FILE_GAME_EXP0           			4
//#define FILE_GAME_EXP1           			5
//#define FILE_GAME_EXP2           			6
//#define FILE_GAME_EXP3           			7
//#define FILE_GAME_EXP4           			8
//#define FILE_GAME_EXP5           			9
//#define FILE_GAME_EXP6           			10
//#define FILE_GAME_EXP7           			11
//#define FILE_GAME_EXP8           			12
//#define FILE_GAME_EXP9           			13
//#define FILE_GAME_EXP10          			14
//#define FILE_GAME_EXP11          			15
//#define FILE_GAME_EXP12          			16
//#define FILE_GAME_EXP13          			17
//#define FILE_SINGERMGHOST3				18
#define FILE_CROP0              			19
#define FILE_CROP1              			20
//#define FILE_TREE0							21
#define FILE_MOVEMARK           			22
//#define FILE_IMJIN2						23
#define FILE_RAIN0              			24
#define FILE_RAIN1              			25
#define FILE_TEAMNUMBER         			26
#define FILE_SELECTBOX          			27
#define FILE_HEAL               			28
//#define FILE_BUDAFIRE						29
//#define FILE_BUDAFIRE2						30
//#define FILE_BUDAFIRE3						31
//#define FILE_ARROW							32
//#define FILE_NEEDLE							33
#define FILE_MONEYNUMBER        			34
#define FILE_SMALLNUMBER        			35
#define FILE_RESOURCE           			36
#define FILE_SMOKE1             			41
#define FILE_ELECTRON           			47
#define FILE_EARTHQUAKE         			50
//#define FILE_MAGICBALL           			51
#define FILE_LIGHTNING1         			52
//#define FILE_LIGHTNINGEXP                 53    
#define FILE_TREASURE           			54
#define FILE_PROTECTOR          			55
#define FILE_TEXT1              			56
#define FILE_CREW               			57
#define FILE_FIREHOUSESMOKE     			58
#define FILE_SHORTCUTKEY        			59
//#define FILE_PLAYERSTART        			60
#define FILE_MAGICK             			61
#define FILE_MAGICJ             			62
#define FILE_HELPRESOURCE       			63
#define FILE_LEVEL              			64
//#define FILE_BULLET          				65
#define FILE_BUILDEXP1          			66
#define FILE_STEALEXP           			67
#define FILE_CLOCK              			68
#define FILE_RUIN0              			69
#define FILE_RUIN1              			70
#define FILE_RUIN2              			71
#define FILE_RUIN3              			72
#define FILE_EVENTMARK          			73
#define FILE_BUILDRUIN          			74
#define FILE_SHIELD             			75
#define FILE_GENERAL2KFIRE      			76
#define FILE_SATFX							77
//#define FILE_BOMBEXP1						78
//#define FILE_BOMBEXP2						79
//#define FILE_BOMBEXP3						80
//#define FILE_TORNADO						81
#define FILE_TEAMFLAG           			82
#define FILE_PORTRAITTEAMNUMBER 			83

// 임진록2 사용되는 define			
//----------------------------------------------
			
	
// 캐릭터 화일 			
//#define FILE_CHAR_SWORD_K       			84
//#define FILE_CHAR_HARUBANG       			85
//#define FILE_CHAR_CRAB	      			86
//#define FILE_AXET			     			87
//#define FILE_AXEUPT		    			88
//#define FILE_CHAR_FARMERK       			89
//#define FILE_ICETREE           			90
//#define FILE_BADMERCHANT         			91
//#define FILE_CHAR_HORSEARCHERK  			92
//#define FILE_MADBULL		      			93
//#define FILE_BUDAUPT		       			94
//#define FILE_CHAR_BUDAK         			95
//#define FILE_CHAR_GUNJ1         			96
//#define FILE_CHAR_GUNJ2         			97
//#define FILE_CHAR_GUNJ3         			98
//#define FILE_CHAR_HORSESWORDJ  			99
//#define FILE_CHAR_GHOSTCAT				100		// 귀묘 
//#define FILE_SNOWLIZARD       			101
//#define FILE_CHAR_STABLEK       			102
//#define FILE_CHAR_HEAVYMAKERK   			103
//#define FILE_CHAR_HEAVYMAKERJ   			104
//#define FILE_SKYNINE	         			105
//#define FILE_CHAR_ADVBUDAJ      			106
//#define FILE_GUNUPT		     			107
//#define FILE_CHAR_AIRMAKERK     			108
//#define FILE_AMAZONUPT        			109
//#define FILE_CHAR_TOWERK        			110
//#define FILE_CHAR_TEMPLEK       			111
//#define FILE_BIGTREE		     			112
//#define FILE_CHAR_SHIPYARDK     			113
//#define FILE_CHAR_SHIPYARDK2    			114
//#define FILE_GENERAL4T	     			115
//#define FILE_CHAR_NINJAJ        			116
//#define FILE_CHAR_HQC           			117
//#define FILE_ELEBARRACKC	      			118
//#define FILE_CHAR_FIREHOUSEC    			119
//#define FILE_GENERAL5T		   			120
//#define FILE_CHAR_SHIPYARDC     			121
//#define FILE_CHAR_AIRMAKERC     			122
//#define FILE_CHAR_HQK           			123
//#define FILE_FARMERT      				124
//#define FILE_CHAR_MILLC         			125
//#define FILE_CHAR_FARMERJ       			126
//#define FILE_CHAR_MILLK         			127
//#define FILE_SWORDT		     			128
//#define FILE_HOOK		         			129
//#define FILE_GUNT          				130
//#define FILE_MBANDIT2         			131
//#define FILE_CHAR_GENERALK11    			132
//#define FILE_CHAR_GENERALK12    			133
//#define FILE_CHAR_GENERALK13    			134
//#define FILE_CHAR_GENERALK2    			135
//#define FILE_CHAR_GENERALK3    			136
//#define FILE_CHAR_GENERALK4     			137
//#define FILE_CHAR_GENERALK5    			138
//#define FILE_LEECH						139
//#define FILE_SEAHORSE						140
//#define FILE_CHAR_GENERALK6     			141
//#define FILE_CHAR_GENERALJ1    			142
//#define FILE_THIEF						143
//#define FILE_SEAKING						144
//#define FILE_CHAR_GENERALJ2    			145
//#define FILE_CHAR_GENERALJ3    			146
//#define FILE_CHAR_GENERALJ4    			147
//#define FILE_CHAR_GENERALJ5     			148
//#define FILE_CHAR_GENERALC11    			149                                                                                                                                                                                                                                                                                                                                                                                                                                             
//#define FILE_CHAR_GENERALC12    			150
//#define FILE_CHAR_GENERALC13    			151
//#define FILE_CHAR_GENERALC2     			152
//#define FILE_CHAR_GENERALC31    			153
//#define FILE_CHAR_GENERALC32    			154                          
//#define FILE_CHAR_GENERALC33    			155                          
//#define FILE_CHAR_GENERALC41    			156
//#define FILE_CHAR_GENERALC42    			157                           
//#define FILE_CHAR_GENERALC43    			158                           
//#define FILE_CHAR_GENERALC51    			159
//#define FILE_SWORDUPT		     			160
//#define FILE_CHAR_TANKC         			161
//#define FILE_CHAR_ADVTOWERK     			162
//#define FILE_PIRATES		    			163
//#define FILE_CHAWOO		     			164
//#define FILE_CHAR_ASHIPK        			165
//#define FILE_CHAR_TOWERC        			166
//#define FILE_BANARIANSHAMAN    			167
//#define FILE_AMAZONT          			168
//#define FILE_SCORPION		    			169
//#define FILE_CHAR_PSHIPK        			170
//#define FILE_SNAKE	         			171
//#define FILE_BABARIAN	        			172
//#define FILE_CHAR_CARJ          			173
//#define FILE_CHAR_TEMPLEC       			174
//#define FILE_CROCODILE         			175
//#define FILE_BUDAT			   			176
//#define FILE_MBANDIT1	        			177
//#define FILE_BIGSWORD	        			178
//#define FILE_GENERAL1T	       			179
//#define FILE_CHAR_ADVTEMPLEK    			180
//#define FILE_PANDER	        			181
//#define FILE_BADDEER			   			182
//#define FILE_GENERAL2T        			183
//#define FILE_CHAR_ADVTOWERJ     			184
//#define FILE_MAGICIAN		    			185
//#define FILE_GENERAL3T		   			186
//#define FILE_CHAR_MILLLABC      			187
//#define FILE_CHAR_KOREANKING    			188
//#define FILE_CHAR_FLAGMAN					189
//#define FILE_IMJIN2PLUS					190
//#define FILE_CHAR_GENERALK7				191
//#define FILE_CHAR_GENERALJ6				192
//#define FILE_CHAR_CANNON2K1				193
//#define FILE_CHAR_CANNON2K2				194
//#define FILE_SMALLGANG					195
//#define FILE_CHAR_GENERALK8     			196
//#define FILE_HORSEMAN						197
//#define FILE_CHAR_GENERALJ7				198
//#define FILE_CHAR_EAGLEC3					199
//#define FILE_TREE_PLUS					200
//#define FILE_CHAR_GENERALJ8				201
#define FILE_GROUNDFIRE  					202
//#define FILE_CHAR_GENERALC7				203
//#define FILE_CHAR_GENERALC6				204
//#define FILE_CHAR_HEAVYGUNC				205
//#define FILE_ADVTOWERKEXP					206
#define FILE_CLOCK_PLUS						207
#define FILE_GAME_CLOUD						208
#define FILE_GAME_WIND						209
//#define FILE_CHAR_JUSTICEK				210
//#define FILE_GAME_GENERAL7K_BOMB			211
#define FILE_GAME_ITEMLIGHT					212
//#define FILE_CHAR_NINJAJ2        			213
//#define FILE_GAME_DART        			214
//#define FILE_MAGIC8K						215
//#define FILE_MAGIC8K_EXP					216
//#define FILE_MAMMOTH						217
//#define FILE_WILDBOAR						218
//#define FILE_MAGIC6C						219
#define FILE_ITEMBOX_LIST					220
#define FILE_ITEMBOX_CANCEL					221
#define FILE_ITEMBOX_SCROLL					222
#define FILE_ITEMBOX_UPDOWN					223
//#define FILE_MAGIC4C						224
//#define FILE_MAGIC4CEXP					225
//#define FILE_MAGIC6CBULLET				226
#define FILE_MAGIC6CSTICK					227
#define FILE_GROUNDFIRESMOKE				228
#define FILE_ITEMBOX_GRAY_LINE				229
//#define FILE_SINGERMGHOSTEXP				230
//#define FILE_BURNARROW					231
//#define FILE_ARROWTRAIL					232
//#define FILE_SPECIAL						233
//#define FILE_HORSEARCHERK_PLUS			234
//#define FILE_WATEREFFECT					235
//#define FILE_DUST							236
//#define FILE_SINGERMGHOST2				237
//#define FILE_MAGIC5J						238
#define FILE_RUIN4              			239
//#define FILE_VOLCANO_BULLET             	240
//#define FILE_CHAR_HQK_PLUS				241
//#define FILE_CHAR_SWORD_K_PLUS			242
//#define FILE_CHAR_ARCHER_K_PLUS			243
//#define FILE_ON_CHAR_KEON_1HS				244
//#define FILE_IMJINONLINE					245
//#define FILE_ON_CHAR_EATERDEMON			246
//#define FILE_ON_CHAR_WEIRDPLANT			247
//#define FILE_ON_CHAR_HUNGRYDEMON			248
//#define FILE_ON_CHAR_TIGER				249
//#define FILE_ON_CHAR_DOGDEMON				250
//#define FILE_ON_CHAR_ONGHWA				251
//#define FILE_ON_CHAR_BANDIT1				252
#define FILE_BUTTON_PLUS           			253
//#define FILE_DETECTMINEEFFECT				254
//#define FILE_CHAR_TOWERK_PLUS      		255
//#define FILE_ON_CHAR_BANDIT2				256
//#define FILE_ON_CHAR_BANDIT3				257
//#define FILE_ON_CHAR_BANDIT4				258
//#define FILE_CACTUS						259
//#define FILE_FIREHOUSESMOKE_PLUS 			260
//#define FILE_CHAR_MILLK_PLUS     			261
//#define FILE_ON_CHAR_GACKDAGUI   			262
//#define FILE_ON_CHAR_GATPHA    			263
//#define FILE_ON_CHAR_PORTERK     			264
#define FILE_ON_CHARINFO					265
#define FILE_ON_LEVEL						266
#define FILE_ON_LEVEL_NUM					267
#define FILE_ON_BUTTMENU					268
#define FILE_ON_GAGEBAR						269
#define FILE_ON_BUTTON           			270
//#define FILE_ON_CHAR_BOGY					271
//#define FILE_ON_CHAR_FOX   				272
//#define FILE_ON_CHAR_STARFISH   			273
#define FILE_ON_ALLY_PANNEL        			274
#define FILE_ON_ALLY_BUTT          			275
//#define FILE_ON_CHAR_SHIP1				276
//#define FILE_ON_CHAR_ONI					277
#define FILE_ON_CHARINFO_ICON				278
#define FILE_SNOWMAGIC1						279
#define FILE_SNOWMAGIC2						280
#define FILE_ON_PORTRAIT					281
#define FILE_RUIN_CASTLETOWER     			282
#define FILE_RUIN_DESTROYEDCASTLE			283
#define FILE_RUIN_CASTLEGATE         		284
#define FILE_RUIN_CASTLETILE         		285
#define FILE_RUIN_CASTLELARGEGATE        	286
//#define FILE_ON_CHAR_SNOWDEMON			287
//#define FILE_ON_CHAR_FURHEADDEMON			288
//#define FILE_ON_CHAR_FIREDRAGON			289
//#define FILE_ON_CHAR_BAT					290
//#define FILE_ON_CHAR_TOAD					291
//#define FILE_ON_CHAR_CENTIPEDE			292
//#define FILE_ON_CHAR_DUEOKSINI			293
//#define FILE_ON_CHAR_GANGSI1   			294
//#define FILE_ON_CHAR_GANGSI2   			295
//#define FILE_ON_CHAR_GANGSI3   			296
//#define FILE_GATPHAPOISON					297
//#define FILE_ON_CHAR_RACCOONDOG  			298
//#define FILE_ON_CHAR_DEER   				299
//#define FILE_ON_CHAR_SWORDK  				300
//#define FILE_ON_CHAR_ARCHERK 				301
//#define FILE_ON_CHAR_SWORDJ  				302
//#define FILE_ON_CHAR_GUNJ  				303
//#define FILE_ON_CHAR_SWORDPIRATE			304
//#define FILE_ON_CHAR_GUNPIRATE			305
#define FILE_ON_EFFECT_HURT1				306		// 공격당했을 때 출혈 효과1
#define FILE_ON_EFFECT_HURT2				307		// 공격당했을 때 출혈 효과2
#define FILE_ON_EFFECT_FOXCLON_APPEAR		308		// 구미호 분신이 나타날 때 효과
#define FILE_ON_EFFECT_FOXCLON_DIE			309		// 구미호 분신이 죽을 때 효과
//#define FILE_ON_CHAR_SNOWWOMAN			310		// 설녀
#define FILE_ON_LEVELUP						311		// 레벨업시 효과
//#define FILE_ON_CHAR_BUDAK				312		// 조선 파계승
#define FILE_ON_POISONSMOKE					313		// 독무(두꺼비, 지네의 독에 당했을 때)
//#define FILE_ON_CHAR_GOLDDRAGON			314		// 금룡 제자리
//#define FILE_ON_CHAR_GOLDDRAGON2			315		// 금룡 공격1
//#define FILE_ON_CHAR_GOLDDRAGON3			316		// 금룡 공격2
//#define FILE_ON_CHAR_GOLDDRAGON4			317		// 금룡 죽기
#define FILE_ON_GOLDDRAGON_CLOUD			318		// 금룡 주변의 구름 효과
#define FILE_ON_GOLDDRAGON_BULLET			319		// 금룡 발사체 효과
#define FILE_ON_GOLDDRAGON_SMOKE			320		// 금룡에게 폭파당했을때 효과
#define FILE_ON_EFFECT_POISON				321		// 두꺼비, 지네의 정지 애니메이션시 독효과
#define FILE_ON_FURHEADDEMON_BULLET			322		// 털가면이 던지는  얼음
#define FILE_ON_FURHEADDEMON_SMOKE			323		// 털가면이 던진 얼음에 당했을 때
//#define FILE_BULLET_ONGHWA				324     // 온천 원숭이 해골. 	
//#define FILE_BULLET_GHOSTCAT				325     // 귀묘 음파 공격. 
//#define FILE_SMOKE_GHOSTCAT				326     // 귀묘 음파 공격 폭파. . 
//#define FILE_BULLET_DOLL1					327     // 무명인형. 
//#define FILE_BULLET_DOLL2					328     // 하카다인형. 
//#define FILE_MANAGER1						329     // 운영자1
//#define FILE_WHITETIGER					330     // 백호
//#define FILE_TUTTLE						331     // 바다거북이
//#define FILE_BUTTERFLY					332     // 독나방
//#define FILE_WIDOW						333     // 주귀
//#define FILE_BULLET_BUTTERFLYBULLET		334     // 독나방 독. 
//#define FILE_BULLET_WIDOWBULLET			335
//#define FILE_SMOKE_WAVEEFFECT				336
//#define FILE_INSECT						337
//#define FILE_WORRIORGHOST					338
//#define FILE_SINGERMGHOST1				339
//#define FILE_SEAHORSEBULLET				340
//#define FILE_ARCHERUPK					341
//#define FILE_HORSEARCHERUPK				342
//#define FILE_JUSTICEUPK					343
//#define FILE_SWORDUPK						344
//#define FILE_BUDAUPK						345
//#define FILE_SWORDUPJ						346
//#define FILE_NINJAUPJ						347
//#define FILE_GUNUPJ						348
//#define FILE_HORSESWORDUPJ				349
//#define FILE_ADVBUDAUPJ					350
//#define FILE_BEAR							351
//#define FILE_HORSESPEAR					352
//#define FILE_CAT							353
//#define FILE_DUCK							354
//#define FILE_HORNET						355
//#define FILE_MBANDIT3						356
//#define FILE_BULLET_POISONARROW			357     // 독침		(원주민무당) 
//#define FILE_BULLET_THROWSPEAR			358     // 던지는 창(원주민)
//#define FILE_BULLET_SCORPIONBULLET		359
//#define FILE_BULLET_CACTUSBULLET			360
//#define FILE_BIGTREEBULLET				361
//#define FILE_MAGIC4T						362
//#define FILE_MAGIC2T						363
//#define FILE_MAGICIANBULLET				364

// 중국용병
#define FILE_ON_CHAR_PORTER_C  				365		// 중국짐꾼
#define FILE_ON_CHAR_FAN_C		  			366		// 모험가
#define FILE_ON_CHAR_BUDAK_C		  		367		// 수도승
#define FILE_ON_CHAR_MUDOGA_C	  			368		// 무도가
#define FILE_ON_CHAR_SWORD_C		 		369		// 큰칼무사
#define FILE_ON_CHAR_FIREGUN_C		  		370		// 화포수

#define FILE_ON_CHAR_FANUP_C				371		// 고급 모험가 
#define FILE_ON_CHAR_BUDAKUP_C				372		// 고급 수도승
#define FILE_ON_CHAR_MUDOGAUP_C				373		// 고급 무도가
#define FILE_ON_CHAR_SWORDUP_C				374     // 고급 큰칼무사.
#define FILE_ON_CHAR_FIREGUNUP_C			375     // 고급 화포수.

#define FILE_GENERAL1C						376
#define FILE_GENERAL2C						377
#define FILE_GENERAL3C						378
#define FILE_GENERAL4C						379
#define FILE_GENERAL5C						380

#define FILE_RUNNERHAND						381
#define FILE_SANDGOD						382
#define FILE_SEAENVOY						383
#define FILE_VIPER							384
#define FILE_AMAKUSA						385
#define FILE_LAVAMONSTER					386
#define FILE_EVIL							387
#define FILE_BIRD							388
#define FILE_WEASEL							389
#define FILE_SOUL							390
#define FILE_COBRA							391
#define FILE_VOLCANO_FLAME					392
#define FILE_YELLOWSAND						393
#define FILE_VOLCANO_STONE					394
#define FILE_EVIL_BULLET					395
#define FILE_KOREAN_GHOST					396
#define FILE_SKELETON						397
#define FILE_SKELETONSOLDIER				398
#define FILE_PIRATECAPTAIN					399
#define FILE_ON_WANGSOUL					400
#define FILE_ON_PANDA						401
#define FILE_ON_DARKFIRE					402
#define FILE_ON_GHOST						403
#define FILE_ON_TAIWANGHOST					404
#define FILE_ON_JAPANGHOST					405
#define FILE_ON_SMALLTORNADO				406
#define FILE_ON_JAPANGHOST_ATTACK			407
#define FILE_BULLET_KUKET					408
#define FILE_CAP_BEAR						409
#define FILE_ON_AUTA						410
#define FILE_ON_JUCK						411
#define FILE_ON_FEMALESHAMAN				412
#define FILE_ON_QUEEN						413
#define FILE_ON_AUTA_ATTACK					414
#define FILE_ON_QUEEN_SUB1					415
#define FILE_ON_QUEEN_SUB2					416
#define FILE_ON_FIREATTACK					417
#define FILE_ON_RUNNINGFIRE					418
#define FILE_ON_ENERGYPA					419
#define FILE_ON_CHAR_DONKEY					420
#define FILE_ON_FIREENERGYPASTART			421
#define FILE_ON_FIREENERGYPA				422

#define FILE_ON_BUTCHER						423
#define FILE_ON_GENERAL1K					424
#define FILE_ON_DSWORD						425
#define FILE_ON_GENERAL1J					426
#define FILE_ON_BEASTWARRIOR				427
#define FILE_ON_GENERAL6T					428
#define FILE_ON_MAGIC_C						429
#define FILE_ON_GENERAL6_C					430
#define FILE_ON_AGILETIGER					431
#define FILE_ON_PAWNTIGER					432
#define FILE_ON_DOUBLE_SWORD				433			// 고니시 쌍검난무
#define FILE_ON_LONG_SPEAR					434			// 손유창 장창소환
#define FILE_ON_ICE_BARRIER					435			// 순비연 빙석술
#define FILE_ON_OWL							436			// 올빼미
#define FILE_ON_LEOPARD						437			// 표범
#define FILE_ON_HUNSWARRIOR					438			// 흉노전사
#define FILE_ON_HUNSHORSE					439			// 흉노기마병
#define FILE_ON_HUNSHORSEARCHER				440			// 흉노기마궁수
#define FILE_ON_LONG_SPEAR_HIT				441			// 장창소환 히트 이펙트
#define FILE_ON_GATEKEEPER					442			// 문지기
#define FILE_ON_CLAYFIGURE					443			// 기마병마용
#define FILE_ON_PHOENIX						444			// 불사조
#define FILE_ON_JINEMPEROR					445			// 진시황의혼
#define FILE_ON_JINEMPEROR_ATTACK			446			// 진시황의혼 공격
#define FILE_ON_JINEMPEROR_HIT				447			// 진시황의혼 HIT
#define FILE_ON_MEDICALMAN					448			// 의술사
#define FILE_ON_HEOJUN						449			// 허준
#define FILE_ON_COSMIC						450			// 음양사
#define FILE_ON_SEIMEI						451			// 세이메이
#define FILE_ON_YUMRUCK						452			// 염력사
#define FILE_ON_JANGSUNHWA					453			// 장선화
#define FILE_ON_LONGSPEAR					454			// 장창무사
#define FILE_ON_KOAKHU						455			// 곽후
#define FILE_ON_COSMIC_PAPERDOLL			456			// 음양사 종이인형
#define FILE_ON_COSMIC_PAPERDOLL_HIT		457			// 음양사 종이인형 히트

#define FILE_ON_NEEDLE						458			// 침공격
#define FILE_ON_FLOWER_NEEDLE_RAIN			459			// 만천화우
#define FILE_ON_SOUL_CHANGE					460			// 빙의술
#define FILE_ON_SOUL_CHANGE_HIT				461			// 빙의술 히트
#define FILE_ON_STONE_MAGIC_STONE			462			// 석괴술 돌
#define FILE_ON_STONE_MAGIC_SMOKE			463			// 석괴술 연기
#define FILE_ON_GOD_SPEAR					464			// 신창노도(일격필살)
#define FILE_ON_CLOUD_WIND					465			// 풍모술
#define FILE_ON_CLOUD_WIND_HIT				466			// 풍모술 히트
#define FILE_ON_MAGICC_ATTACK				467			// 도술사 공격
#define FILE_ON_SUNBIYOUN_ATTACK			468			// 순비연 공격
#define FILE_ON_SNOWMAN						469			// 눈사람
#define FILE_ON_SANTA_ARROW					470			// 화살쏘는 산타.
#define FILE_ON_SANTA_AXE					471			// 도끼든  산타.
#define FILE_ON_MOTHER_DEER					472			// 루돌이.
#define FILE_ON_SNOW						473			// 눈
#define FILE_ON_RAPTOR						474			// 랩터.
#define FILE_ON_TIRANO						475			// 티라노.
#define FILE_ON_FLYDRAGON					476			// 익룡.
#define FILE_ON_MONKEY						477			// 소환원숭이.
#define FILE_ON_FIREENERGYPA_HIT1			478			// 용장풍 히트1(불투명)
#define FILE_ON_FIREENERGYPA_HIT2			479			// 용장풍 히트2(투명)
#define FILE_ON_BASE_TIMER					480			
#define FILE_ON_ALLY_TITLE					481
#define FILE_ON_ALLY_WINDOW					482
#define FILE_ON_ALLY_CHECK					483
#define FILE_ON_ALLY_BUTTON_OPEN			484
#define FILE_ON_ALLY_BUTTON_CLOSE			485
#define FILE_ON_HELLKING					486
#define FILE_ON_HELLMESSENGER				487
#define FILE_ON_GHOSTSAMURAI				488
#define FILE_ON_PYTHON						489
#define FILE_ON_PYTHON_BULLET				490

#define FILE_ON_GUARDIAN_MOUSE_START		491
#define FILE_ON_GUARDIAN_CAW_START			492
#define FILE_ON_GUARDIAN_TIGER_START		493
#define FILE_ON_GUARDIAN_DRAGON_START		494	
#define FILE_ON_GUARDIAN_HORSE_START		495
#define FILE_ON_GUARDIAN_RABBIT_START		496

#define FILE_ON_GUARDIAN_MOUSE				497
#define FILE_ON_GUARDIAN_TIGER				498
#define FILE_ON_GUARDIAN_DRAGON				499
#define FILE_ON_GUARDIAN_HORSE				500
#define FILE_ON_GUARDIAN_RABBIT				501
#define FILE_ON_GUARDIAN_CAW1				502
#define FILE_ON_GUARDIAN_CAW2				503
#define FILE_ON_GUARDIAN_CAW3				504

#define FILE_ON_GUARDIAN_HORSE_USE			505
#define FILE_ON_GUARDIAN_RABBIT_USE			506

#define FILE_ON_BULLET_GUARDIAN_TIGER		507

#define FILE_ON_GUARDIAN_RABBIT_ATTACK		508

#define FILE_ON_GUARDIAN_CAW_SMOKE			509
#define FILE_ON_GUARDIAN_TIGER_SMOKE		510

#define FILE_ON_GUARDIAN_SHEEP_LIFEUP_USE		511
#define FILE_ON_GUARDIAN_SHEEP_MAGICRESIST_USE	512

#define FILE_ON_GUARDIAN_SNAKE_START		513
#define FILE_ON_GUARDIAN_PIG_START			514
#define FILE_ON_GUARDIAN_CHICKEN_START		515
#define FILE_ON_GUARDIAN_MONKEY_START		516
#define FILE_ON_GUARDIAN_SHEEP_START		517
#define FILE_ON_GUARDIAN_DOG_START			518

#define FILE_ON_GUARDIAN_CHICK				519
#define FILE_ON_GUARDIAN_MONKEY				520
#define FILE_ON_GUARDIAN_PIG				521

#define FILE_ON_GUARDIAN_CHICK_ATTACK		522
#define FILE_ON_GUARDIAN_CHICK_SMOKE		523

#define FILE_ON_BTMAP_TREE_1				524			//배틀맵 나무1
#define FILE_ON_BTMAP_TREE_2				525			//배틀맵 나무2
#define FILE_ON_BTMAP_TREE_3				526			//배틀맵 나무3
#define FILE_ON_BTMAP_TREE_4				527			//배틀맵 나무4
#define FILE_ON_BTMAP_TREE_5				528			//배틀맵 나무5
#define FILE_ON_BTMAP_TREE_6				529			//배틀맵 나무6
#define FILE_ON_BTMAP_TREE_7				530			//배틀맵 나무7

// robypark 2004-6-3 16:15
// 천지독살 냉기에 의한 마비 smoke, 흑룡차 기본 공격에 의한 마비 smoke
#define	FILE_ON_FOG_OF_ICE_HIT				555	

#define MAX_GAME_SPR_NUMBER					600



// 일반 그림들을 불러온다. 
void LoadFnt();
BOOL FreeFnt();

// 지도용 타일을 불러온다.
void LoadMapTile(SHORT maptype);
// 지도용 타일을 해제한다.
void FreeMapTile();
SI32 GetValueOfTileID(CHAR *TileID);
BOOL LoadImgFileList();

// 로딩된 시간을 근거로 오래된 이미지를 제거한다.
void FreeOldImageByTime();

SI32 GetValueOfGameSprNumID(CHAR *ID);
// 해당 이미지 파일을 불러온다.
BOOL LoadImgFile(SI32 num);
// 게임 중에 로딩된 이미지 파일에 해당하는 메모리를 모두 해제한다.
void FreeImgFile();

extern int FileTileGrss1;
extern int FileTileDiff1;
extern int FileTileHill0;
extern int FileTileHill1;
extern int FileTileNewBlkGate;
extern int FileTileSea0;
extern int FileTileCastle2;
extern int FileTileBlackTile;
extern int FileTileShallow;

struct _TreeFile
{	
	char FileName[32];
	SI32 BlockSize;
};


class _FileVary{
public:
	SI32 OnlineLogoFile;
	SI32 LogoFile;
	SI32 *m_TreeFiles;
	_TreeFile				*m_TreeImgFile;		//트리 타입이 저장된 파일
	SI32					m_siTreeFileNo;		//트리 타입 개수
	
	SI32					m_siSelectFile;		//선택한 파일의 인덱스..

	SI32	GetNowFile() { return m_TreeFiles[m_siSelectFile]; }
	void	SetNowFile( SI32 siSel) { m_siSelectFile = siSel;}

	void Init();

	//Tree Type이 들어있는 파일을 읽어온다.
	BOOL ReadTreeFileText_Daesang();

};

BOOL LoadPannel(SI32 sicountry);


#endif

#endif