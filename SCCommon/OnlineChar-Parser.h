#ifndef	__ONLINECHARPARSER_H
#define	__ONLINECHARPARSER_H

/********************************************************************************************
	���ϸ�	:	OnlineChar-Parser.h
	�ۼ���	:	�� �� ��
	�����  :   �� �� ��
	�ۼ���	:	2001.06.30
	������	:	2001.12.24
********************************************************************************************/


#define ON_ATTRI_PLAYER				1
#define ON_ATTRI_MONSTER			2
#define ON_ATTRI_MERCENARY 			4

#define ON_MONSTER_TYPE1			1
#define ON_MONSTER_TYPE2			2
#define ON_MONSTER_TYPE3			3
#define ON_MONSTER_HALT				4



#pragma warning ( disable : 4786 )

#include	"ParametaBox.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


struct	CharHead
{
	UI16				uiID;						// �ɸ��� ���̵�
	UI32				uiCharName;					// �ɸ��� �̸�
	SI16				siAttri;
	UI16				uiMyExp;					// ��� ���� ����
	SI16				siNation;					// ����
	SI16				siMoveSpeed;				// �̵� �ӵ�
	SI32				siManaMax;	
	SI32				siCost;
	SI16				siLv;
	SI16				siCon;
	SI16				siNeedLv;					// �뺴�� ��� �ִ� ��
	UI08				uiWeaponType;				// ��� �Ҽ� �ִ� ����
	SI16				siNeedCredit;				// �뺴�� �����Ҽ� �ִ� �ſ뵵
	SI16				siItemDrop[4];				// ���� ���� ������
	SI32				siDropPercent[4];			// ���� ���� Ȯ��
	SI32				siDescText;
	SI16				siGeneral;					// �Ϲݿ뺴(0), ���(1), ����(2)
	SI08				siMonsterType;				// �Ϲݸ���(1), ������(2)

	_ImportantParameta	Parameta;	

	const bool CharHead::operator < (const CharHead &lhs) const { return lhs.uiID < this->uiID ? true : false; };
	const bool CharHead::operator ==(const CharHead &lhs) const { return lhs.uiID == this->uiID ? true : false; };
};

class	OnlineCharParser
{
public:
	vector<CharHead>	m_CharHeader;
	typedef vector<CharHead>::iterator VWlter;
	typedef pair<VWlter, VWlter> VWlterPair;	

	SI16				siMonsterNum;																// ������ �� ��
	SI16				siTotalCharNum;	
public:
	OnlineCharParser()								   { siMonsterNum = 0; };
	~OnlineCharParser()								   { Free(); };

	VOID				Free()						   { m_CharHeader.clear(); };					// �ɸ��� ����ü�� ������.
	virtual BOOL		Init( CHAR *pCharFile );													// �ɸ��� ����ü�� ����Ÿ�� ����
	CharHead			*GetCharHeader( SI16 siIndex ) { return &m_CharHeader[siIndex]; };			// �ɸ��� ����� ���� �޴´�.
	CharHead			*GetCharHeaderID( UI16 uiID );												// �ɸ��� ����� ���� �޴´�.
	SI16				GetCharTotalNum()			   { return m_CharHeader.size(); };				// �� �ɸ��� ���� ���� �Ѵ�.
	SI16				GetMonsterTotalNum()		   { return siMonsterNum; };					// �� ���� ���� ���� �Ѵ�.
	UI16				GetID( char *pToken );														// �ɸ����� ���̵� ���� �޴´�.

protected:
	SI16				GetAttri( char *pToken );													// �ɸ��Ͱ� ���� �ΰ� �÷��̾� �ΰ� �˾� �´�.
};
#endif