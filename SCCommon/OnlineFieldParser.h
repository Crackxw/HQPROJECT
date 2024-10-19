#ifndef __ON_FIELDPARSER_H
#define __ON_FIELDPARSER_H


#include <iostream>
#include <map>

using namespace std;

/********************************************************************************************
	파일명	:	OnlineFieldParser.h
	작성자	:	정 진 욱
	담당자  :   이 상 원
	작성일	:	2001.07.24
	수정일	:	2001.07.25
********************************************************************************************/


struct MilepostHeader
{
	UI32					uiID;																// 아이디	
	SI16					siNowMapIndex;														// Now Map Index
	SI16					siFont;																// 리소스에서의 파일 인덱스
	SI16					siBGM;																// 배경 음악
	SI16					siFile;																// 리소스의 인덱스	
	CHAR					szImageFileName[124];												// 파일 이름
	SI32					siText;																// 텍스트에서의 인덱스 번호	
};


class OnlineFieldParser
{
private:
	SI16					siTotal;															// 총 표지판수
	MilepostHeader			*pMilepostHeader;													// 이정표 구조체
	typedef map< UI32, MilepostHeader, less<UI32> > Milepost;
	Milepost				_Milepost;

public:
	OnlineFieldParser();
	~OnlineFieldParser();

	BOOL					Init(CHAR *pFileName);												// 초기화 
	SI16					GetTotalNum();														// 총 표지판수를 리턴한다.
	
	MilepostHeader			*GetMilepost( SI16 siIndex );										// 이정표의 해당 인덱스만 리턴해준다.
	VOID					Free();																// 날린다.
	BOOL					SetFile( CHAR *pFileName, SI16 siResFile );							// 리소스의 인덱스를 설정한다.
	MilepostHeader			*GetMilePost(SI16 siX, SI16 siY);									// 헤더를 리턴( 2진 검색 )
};

#endif