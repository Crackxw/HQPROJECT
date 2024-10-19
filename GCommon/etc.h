#ifndef _ETC_H
#define _ETC_H   1

// NPC 종류 정의 (초상을 찍기 위해)
#define NPC_KIND_GOV			1	// 관청
#define NPC_KIND_FARM			2	// 농장
#define NPC_KIND_WEAPONSHOP		3	// 무기점
#define NPC_KIND_NORMALMARKET	4	// 시전
#define NPC_KIND_CROPMARKET		5	// 싸전
#define NPC_KIND_DRUGMARKET		6	// 약방
#define NPC_KIND_HARVOR			7	// 부두
#define NPC_KIND_DOCTOR			8	// 의원
#define NPC_KIND_BANK			9	// 전장
#define NPC_KIND_BARRACK		10	// 용병훈련소
#define NPC_KIND_GBARRACK		11	// 장수양성소
#define NPC_KIND_MBARRACK		12	// 몬스터조련소


// box bar출력 관련 definition
#define ENERGY_UNIT_SIZE				3
#define COLOR_BORDER_IN_BOX_BAR       246


// 캐릭터가 살아 있는지 여부를 알려준다. 
BOOL IsAlive(int id);
// 캐릭터가 살아 있는지 여부를 알려준다. 
BOOL IsAlive(_NewID id);


// 볼수 있는 캐릭터인지 여부를 알려준다. (정수 아이디 )
BOOL IsSee(int id);
// 볼수 있는 캐릭터인지 여부를 알려준다. ( 아이디 포맷)
BOOL IsSee(_NewID id);

// 선택된 모든 캐릭터를 해제한다. 
void Unselect();
// 캐릭터들의 평균 위치를 구한다. 
BOOL GetCenterXY(_NewID* plist, SHORT charnumber, short & centerx, short& centery);
// sx, sy와 dx,dy를 가지고 방향을 찾는다. (8방향)
SHORT DecideDirection8(DWORD sx, DWORD sy, DWORD dx, DWORD dy);
SHORT DecideDirection16(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
SHORT DecideDirectionChar16(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
SHORT DecideDirection32(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
// srcdir을 기준으로 destdir이 시계방향인가?
BOOL IsClockwiseDir16(SHORT srcdir, SHORT destdir);

// player_number1가 player_number2에 친근한가 ?
BOOL IsFriendToByPlayerNumber(SHORT player_number1, SHORT player_number2);

BOOL IsFriendToByID(SHORT id1, SHORT id2);

// 플레이어 번호가 유효한지 검사한다. 
BOOL IsValidPlayerNumber(SHORT pnumber);
BOOL FindEmptyAreaGlobal(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize, SHORT dx, SHORT dy, SHORT dxsize, SHORT dysize);

// 숫자를 써준다.
SHORT DrawNumber(SHORT startx, SHORT starty, SHORT file, SHORT font, SHORT number, SHORT option, BOOL clipmode = FALSE);
// 2캐릭터 사이의 거리를 구한다.
SHORT GetLengthBetweenChar(SHORT id1, SHORT id2);

// 전투 결과에 따라 경험치등을 조정해준다. 
void KillOperation(SHORT myplayernumber, _NewID attakid);
// x, y의 좌표가 판넬위에 있는지 확인한다. 
BOOL IsPtInPannel(SHORT x, SHORT y);
// x, y의 좌표가 동맹창위에 있는지 확인한다.
BOOL IsPtInAllyWindow(SHORT x,SHORT y);
// 캐릭터 간의 거리를 구한다. 
SHORT GetLegthBetweenChar(SHORT id1, SHORT id2);

// 지도의 포그를 밝혀준다. 
void GlobalBlackCheck(SI32 x, SI32 y, SHORT range, BOOL bMode, SI32 pnumber=-1);

// path를 고려하여 화일 이름을 구한다. 
void GetFileNamePath(char* src_filename, char* path, char* dst_filename);

void InitFrameClock();
void PushFrameClock(DWORD frame, DWORD clock);
void SetStartFrameClock(DWORD clock);
// 원격지 컴퓨터와 비교하여 내 컴의 속도가 정상인지 확인한다. 
void CompareSyncSpeed(SHORT id, DWORD frame, DWORD clock);
DWORD GetFrameClock();

/// 죽은 적 유닛을 판별하여 점수를 구한다. 
DWORD CalcurateScore(SHORT eplayernumber, _NewID died_enemy_id);

// 충무공 탄신일과 같은 특별한 날에 조치되는 사항을 처리한다. 
void SpecialDateOperation();

// 현재 선택된 팀의 깃발을 보여준다. 
void DrawTeamFlag(LPDIRECTDRAWSURFACE7 psurface, SHORT selectedteamnumber);

// 텍스트를 특정 좌표를 중심으로 찍는다.(가운데 정렬)
void DrawTextInCenter(HDC hdc, SI32 centerx, SI32 centery, char* text);

// 텍스트를 주어진 RECT안에 출력한다. 
void DrawTextInRect(LPDIRECTDRAWSURFACE7 psurface, HFONT font, char* buffer, RECT rect);


// Box를 사용하여 수치를 바형태로 표현한다. 
SI32 DrawBoxBar(SI32 value, SI32 maxvalue, SI32 valueperbox, POINT start, SI32 xdotsize, BOOL bVillage = FALSE);

// robypark 2004/10/28 14:16
// Box를 사용하여 수치를 바형태로 표현한다. 
// 위의 DrawBoxBar()와 동일하나 바 테두리 선 색과 채워지는 색을 전달 받아 사용한다.
SI32 DrawBoxBar(SI32 value, SI32 maxvalue, SI32 valueperbox, POINT start, SI32 xdotsize, SI08 clrBarEdge, SI08 clrBarFilled, BOOL bVillage = FALSE, SI08 clrBarEmpty = 0);

// robypark 2004/10/28 15:3
// 수치를 게이지 형태로 그려준다.
void DrawGauge(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight, R32 fRate, UI08 uiClrGaugeFilled, UI08 uiClrGaugeEmpty);

// robypark 2005/1/7 18:41
// 수치(10%단위로)를 게이지 형태로 그려준다.
// 공성전 유닛 체력, 보급 게이지에서 사용
void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

// robypark 2005/1/7 18:41
// 수치를 게이지 형태로 그려준다.
// 마을 방어력, 수비병 게이지 출력에서 사용
void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage);

// 사망에 의한 경험치 손실을 구한다.
SI32 GetExpReduceForDying(SI32 level);


// NPC 의 초상을 그린다.
// npc_kind: NPC의 종류.
// x, y: 찍어야 할 시작 위치.
// psurface: 찍어야 할 서피스 
// lock_switch: 서피스를 락한 후에 그려야 하는지 그냥 그려야 하는지 여부.(TRUE : 락 한 후에 그린다.)
void DrawNPCProtrait(SI32 npc_kind, SI16 nation, SI32 x, SI32 y, LPDIRECTDRAWSURFACE7 psurface, BOOL lock_switch) ;

#endif
