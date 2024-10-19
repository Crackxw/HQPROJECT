#ifndef __ON_FIELDPARSER_H
#define __ON_FIELDPARSER_H


#include <iostream>
#include <map>

using namespace std;

/********************************************************************************************
	���ϸ�	:	OnlineFieldParser.h
	�ۼ���	:	�� �� ��
	�����  :   �� �� ��
	�ۼ���	:	2001.07.24
	������	:	2001.07.25
********************************************************************************************/


struct MilepostHeader
{
	UI32					uiID;																// ���̵�	
	SI16					siNowMapIndex;														// Now Map Index
	SI16					siFont;																// ���ҽ������� ���� �ε���
	SI16					siBGM;																// ��� ����
	SI16					siFile;																// ���ҽ��� �ε���	
	CHAR					szImageFileName[124];												// ���� �̸�
	SI32					siText;																// �ؽ�Ʈ������ �ε��� ��ȣ	
};


class OnlineFieldParser
{
private:
	SI16					siTotal;															// �� ǥ���Ǽ�
	MilepostHeader			*pMilepostHeader;													// ����ǥ ����ü
	typedef map< UI32, MilepostHeader, less<UI32> > Milepost;
	Milepost				_Milepost;

public:
	OnlineFieldParser();
	~OnlineFieldParser();

	BOOL					Init(CHAR *pFileName);												// �ʱ�ȭ 
	SI16					GetTotalNum();														// �� ǥ���Ǽ��� �����Ѵ�.
	
	MilepostHeader			*GetMilepost( SI16 siIndex );										// ����ǥ�� �ش� �ε����� �������ش�.
	VOID					Free();																// ������.
	BOOL					SetFile( CHAR *pFileName, SI16 siResFile );							// ���ҽ��� �ε����� �����Ѵ�.
	MilepostHeader			*GetMilePost(SI16 siX, SI16 siY);									// ����� ����( 2�� �˻� )
};

#endif