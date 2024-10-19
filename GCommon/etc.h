#ifndef _ETC_H
#define _ETC_H   1

// NPC ���� ���� (�ʻ��� ��� ����)
#define NPC_KIND_GOV			1	// ��û
#define NPC_KIND_FARM			2	// ����
#define NPC_KIND_WEAPONSHOP		3	// ������
#define NPC_KIND_NORMALMARKET	4	// ����
#define NPC_KIND_CROPMARKET		5	// ����
#define NPC_KIND_DRUGMARKET		6	// ���
#define NPC_KIND_HARVOR			7	// �ε�
#define NPC_KIND_DOCTOR			8	// �ǿ�
#define NPC_KIND_BANK			9	// ����
#define NPC_KIND_BARRACK		10	// �뺴�Ʒü�
#define NPC_KIND_GBARRACK		11	// ����缺��
#define NPC_KIND_MBARRACK		12	// �������ü�


// box bar��� ���� definition
#define ENERGY_UNIT_SIZE				3
#define COLOR_BORDER_IN_BOX_BAR       246


// ĳ���Ͱ� ��� �ִ��� ���θ� �˷��ش�. 
BOOL IsAlive(int id);
// ĳ���Ͱ� ��� �ִ��� ���θ� �˷��ش�. 
BOOL IsAlive(_NewID id);


// ���� �ִ� ĳ�������� ���θ� �˷��ش�. (���� ���̵� )
BOOL IsSee(int id);
// ���� �ִ� ĳ�������� ���θ� �˷��ش�. ( ���̵� ����)
BOOL IsSee(_NewID id);

// ���õ� ��� ĳ���͸� �����Ѵ�. 
void Unselect();
// ĳ���͵��� ��� ��ġ�� ���Ѵ�. 
BOOL GetCenterXY(_NewID* plist, SHORT charnumber, short & centerx, short& centery);
// sx, sy�� dx,dy�� ������ ������ ã�´�. (8����)
SHORT DecideDirection8(DWORD sx, DWORD sy, DWORD dx, DWORD dy);
SHORT DecideDirection16(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
SHORT DecideDirectionChar16(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
SHORT DecideDirection32(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
// srcdir�� �������� destdir�� �ð�����ΰ�?
BOOL IsClockwiseDir16(SHORT srcdir, SHORT destdir);

// player_number1�� player_number2�� ģ���Ѱ� ?
BOOL IsFriendToByPlayerNumber(SHORT player_number1, SHORT player_number2);

BOOL IsFriendToByID(SHORT id1, SHORT id2);

// �÷��̾� ��ȣ�� ��ȿ���� �˻��Ѵ�. 
BOOL IsValidPlayerNumber(SHORT pnumber);
BOOL FindEmptyAreaGlobal(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize, SHORT dx, SHORT dy, SHORT dxsize, SHORT dysize);

// ���ڸ� ���ش�.
SHORT DrawNumber(SHORT startx, SHORT starty, SHORT file, SHORT font, SHORT number, SHORT option, BOOL clipmode = FALSE);
// 2ĳ���� ������ �Ÿ��� ���Ѵ�.
SHORT GetLengthBetweenChar(SHORT id1, SHORT id2);

// ���� ����� ���� ����ġ���� �������ش�. 
void KillOperation(SHORT myplayernumber, _NewID attakid);
// x, y�� ��ǥ�� �ǳ����� �ִ��� Ȯ���Ѵ�. 
BOOL IsPtInPannel(SHORT x, SHORT y);
// x, y�� ��ǥ�� ����â���� �ִ��� Ȯ���Ѵ�.
BOOL IsPtInAllyWindow(SHORT x,SHORT y);
// ĳ���� ���� �Ÿ��� ���Ѵ�. 
SHORT GetLegthBetweenChar(SHORT id1, SHORT id2);

// ������ ���׸� �����ش�. 
void GlobalBlackCheck(SI32 x, SI32 y, SHORT range, BOOL bMode, SI32 pnumber=-1);

// path�� ����Ͽ� ȭ�� �̸��� ���Ѵ�. 
void GetFileNamePath(char* src_filename, char* path, char* dst_filename);

void InitFrameClock();
void PushFrameClock(DWORD frame, DWORD clock);
void SetStartFrameClock(DWORD clock);
// ������ ��ǻ�Ϳ� ���Ͽ� �� ���� �ӵ��� �������� Ȯ���Ѵ�. 
void CompareSyncSpeed(SHORT id, DWORD frame, DWORD clock);
DWORD GetFrameClock();

/// ���� �� ������ �Ǻ��Ͽ� ������ ���Ѵ�. 
DWORD CalcurateScore(SHORT eplayernumber, _NewID died_enemy_id);

// �湫�� ź���ϰ� ���� Ư���� ���� ��ġ�Ǵ� ������ ó���Ѵ�. 
void SpecialDateOperation();

// ���� ���õ� ���� ����� �����ش�. 
void DrawTeamFlag(LPDIRECTDRAWSURFACE7 psurface, SHORT selectedteamnumber);

// �ؽ�Ʈ�� Ư�� ��ǥ�� �߽����� ��´�.(��� ����)
void DrawTextInCenter(HDC hdc, SI32 centerx, SI32 centery, char* text);

// �ؽ�Ʈ�� �־��� RECT�ȿ� ����Ѵ�. 
void DrawTextInRect(LPDIRECTDRAWSURFACE7 psurface, HFONT font, char* buffer, RECT rect);


// Box�� ����Ͽ� ��ġ�� �����·� ǥ���Ѵ�. 
SI32 DrawBoxBar(SI32 value, SI32 maxvalue, SI32 valueperbox, POINT start, SI32 xdotsize, BOOL bVillage = FALSE);

// robypark 2004/10/28 14:16
// Box�� ����Ͽ� ��ġ�� �����·� ǥ���Ѵ�. 
// ���� DrawBoxBar()�� �����ϳ� �� �׵θ� �� ���� ä������ ���� ���� �޾� ����Ѵ�.
SI32 DrawBoxBar(SI32 value, SI32 maxvalue, SI32 valueperbox, POINT start, SI32 xdotsize, SI08 clrBarEdge, SI08 clrBarFilled, BOOL bVillage = FALSE, SI08 clrBarEmpty = 0);

// robypark 2004/10/28 15:3
// ��ġ�� ������ ���·� �׷��ش�.
void DrawGauge(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight, R32 fRate, UI08 uiClrGaugeFilled, UI08 uiClrGaugeEmpty);

// robypark 2005/1/7 18:41
// ��ġ(10%������)�� ������ ���·� �׷��ش�.
// ������ ���� ü��, ���� ���������� ���
void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

// robypark 2005/1/7 18:41
// ��ġ�� ������ ���·� �׷��ش�.
// ���� ����, ���� ������ ��¿��� ���
void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage);

// ����� ���� ����ġ �ս��� ���Ѵ�.
SI32 GetExpReduceForDying(SI32 level);


// NPC �� �ʻ��� �׸���.
// npc_kind: NPC�� ����.
// x, y: ���� �� ���� ��ġ.
// psurface: ���� �� ���ǽ� 
// lock_switch: ���ǽ��� ���� �Ŀ� �׷��� �ϴ��� �׳� �׷��� �ϴ��� ����.(TRUE : �� �� �Ŀ� �׸���.)
void DrawNPCProtrait(SI32 npc_kind, SI16 nation, SI32 x, SI32 y, LPDIRECTDRAWSURFACE7 psurface, BOOL lock_switch) ;

#endif
