#ifndef _ROCKET_NEW_TILE_MAP
#ifndef _MAPEDIT_H
#define _MAPEDIT_H

#define MAPEDIT_MODE_LEVEL             0
#define MAPEDIT_MODE_SEA               1
#define MAPEDIT_MODE_SHALLOW           2
#define MAPEDIT_MODE_NEWBLK            3
#define MAPEDIT_MODE_NEWBLKGATE        4
#define MAPEDIT_MODE_CASTLE            5
#define MAPEDIT_MODE_CASTLE_GATE_LEFT  6
#define MAPEDIT_MODE_CASTLE_GATE_RIGHT 7
#define MAPEDIT_MODE_TILE              8
#define MAPEDIT_MODE_RICE              9
#define MAPEDIT_MODE_POTATO            10
#define MAPEDIT_MODE_TREE              11
#define MAPEDIT_MODE_BAMBOO            12
#define MAPEDIT_MODE_RUIN              13
#define MAPEDIT_MODE_SHIPPATROL        14
#define MAPEDIT_MODE_SHIPLANDING       15
#define MAPEDIT_MODE_UNIT              16

class _MapEdit {
public:

	SHORT PutSeaMode;
	SHORT PutWaterMode;
	SHORT PutNewBlkMode;
	SHORT PutLevelLevel;

	SHORT TerrainMode;
	SHORT Level;        // ������
	SHORT Tile;
	SHORT SelectedKind;
	SHORT Ruin;

	SHORT PlayerNumber;

	char MapFileName[256];
	char MapFileShortName[128];
	void Init();
	
	void Action();

	// ���̸� �����Ѵ�. 
	// x, y :2��� ��ǥ 
	// level: ���� 
	// pushmode: �ֺ��� �ִ� �ٸ� ������ Ÿ���� ���ִ°�? TRUE : ���ش�. 
	//                                                    FALSE:������ �ʴ´�.  
	void MapEdit_PutLevel(SHORT x, SHORT y, SHORT level, SHORT pushmode);

	void MapEdit_PutSea(SHORT x, SHORT y, SHORT mode);
	void MapEdit_PutWater(SHORT x, SHORT y);
	void MapEdit_PutNewBlk(SHORT x, SHORT y);
	void MapEdit_DeleteNewBlk(SHORT x, SHORT y);
	void MapEdit_PutNewBlkGate(SHORT x, SHORT y);
	void MapEdit_PutCastle(SHORT x, SHORT y);
	void MapEdit_DeleteCastle(SHORT x, SHORT y);


	// ���� ������ ǥ���Ѵ�. 
    void MapEdit_DrawInfo();

	// ����������� �߰��Ѵ�.
    void PushShipPatrol(SHORT x, SHORT y);
	// �� ��������� �߰��Ѵ�. 
    void PushShipLanding(SHORT x, SHORT y);


};

#endif

#else

#ifndef _MAPEDIT_H
#define _MAPEDIT_H

#define MAPEDIT_MODE_LEVEL             0
#define MAPEDIT_MODE_SEA               1
#define MAPEDIT_MODE_SHALLOW           2
#define MAPEDIT_MODE_NEWBLK            3
#define MAPEDIT_MODE_NEWBLKGATE        4
#define MAPEDIT_MODE_CASTLE            5
#define MAPEDIT_MODE_CASTLE_GATE_LEFT  6
#define MAPEDIT_MODE_CASTLE_GATE_RIGHT 7
#define MAPEDIT_MODE_TILE              8
#define MAPEDIT_MODE_RICE              9
#define MAPEDIT_MODE_POTATO            10
#define MAPEDIT_MODE_TREE              11
#define MAPEDIT_MODE_BAMBOO            12
#define MAPEDIT_MODE_RUIN              13
#define MAPEDIT_MODE_SHIPPATROL        14
#define MAPEDIT_MODE_SHIPLANDING       15
#define MAPEDIT_MODE_UNIT              16

class _MapEdit {
public:

	SHORT PutSeaMode;
	SHORT PutWaterMode;
	SHORT PutNewBlkMode;
	SHORT PutLevelLevel;

	SHORT TerrainMode;
	SHORT Level;        // ������
	SHORT Tile;
	SHORT SelectedKind;
	SHORT Ruin;	
	SHORT Tree;		//���� Ÿ��


	SHORT PlayerNumber;

	char MapFileName[256];
	char MapFileShortName[128];
	void Init();
	
	void Action();

	// ���̸� �����Ѵ�. 
	// x, y :2��� ��ǥ 
	// level: ���� 
	// pushmode: �ֺ��� �ִ� �ٸ� ������ Ÿ���� ���ִ°�? TRUE : ���ش�. 
	//                                                    FALSE:������ �ʴ´�.  
	void MapEdit_PutLevel(SHORT x, SHORT y, SHORT level, SHORT pushmode);

	void MapEdit_PutSea(SHORT x, SHORT y, SHORT mode);
	void MapEdit_PutWater(SHORT x, SHORT y);
	void MapEdit_PutNewBlk(SHORT x, SHORT y);
	void MapEdit_DeleteNewBlk(SHORT x, SHORT y);
	void MapEdit_PutNewBlkGate(SHORT x, SHORT y);
	void MapEdit_PutCastle(SHORT x, SHORT y);
	void MapEdit_DeleteCastle(SHORT x, SHORT y);


	// ���� ������ ǥ���Ѵ�. 
    void MapEdit_DrawInfo();

	// ����������� �߰��Ѵ�.
    void PushShipPatrol(SHORT x, SHORT y);
	// �� ��������� �߰��Ѵ�. 
    void PushShipLanding(SHORT x, SHORT y);


};

#endif
#endif