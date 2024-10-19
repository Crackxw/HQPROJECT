#ifndef __ONLINETIP_PARSER_H
#define __ONLINETIP_PARSER_H


#include <windows.h>
#include <directives.h>

struct  OnlineTipContentsHeader
{
	UI32 uiSubject;
	UI32 uiContents;
	UI16 uiVariable;
	UI32 uicode;
};

struct	OnlineTipHeader
{
 
	UI32 uiMainSubjectNum;   //��ȣ
	UI32 uiMainSubjectIndex; //Text���� ������ Index Number]

	UI32 uiTotalTipHeader;   //Subject�� ����.
	UI32 uiDescription;      //����.
	OnlineTipContentsHeader* pOnlineTipContentsHeader;  //Contents ����ü.
};



class OnlineTipParser
{
private:
	OnlineTipHeader*			pOnlineMainTipHeader;
	OnlineTipHeader*			pOnlineTipHeader;
	OnlineTipHeader*			pOnlineSubContentsHeader;


public:
	SI16					   m_iMainNum;
	SI16					   m_iSubjectNum;
	SI16					   m_iContentsNum;
	OnlineTipParser();
	~OnlineTipParser();

	BOOL ReadText(char* filename);
	BOOL Free();
	OnlineTipContentsHeader*        GetTipMain(int con);
	OnlineTipContentsHeader*		GetTipSubject(int sub,int con);
	OnlineTipContentsHeader*		GetTipContents(int sub,int con);
	UI16							GetTipContentsSelect(UI16 code);
	UI16							GetSubjectTotal(UI16 code);
	UI16							GetContentsTotal(UI16 code);

	OnlineTipHeader*                GetTipHeader();
	OnlineTipHeader*                GetTipSubjectHeader(int sub);
	OnlineTipHeader*				GetTipContentsHeader(int sub);
};

#endif