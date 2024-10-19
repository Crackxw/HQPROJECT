
#pragma once

#include "OnlineItem.h"

class ItemParser : public CItem
{
public:
	ItemParser()	{}
	~ItemParser()	{}

	void	Load(OnlineText *Text, char *pFile);
	void	Save();	
	void	DeleteItem(int nIndex);

protected:
	BOOL	LoadItem(char *pItemFile, vector<CItemHeader> *v);
	BOOL	LoadWeapon( char *pWeaponFile );
};