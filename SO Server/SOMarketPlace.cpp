#include "SOMarketPlace.h"
//����������������������������������������������������������������������������
// ���Ǵ� �����͸� ���´�.
//����������������������������������������������������������������������������
stBOOTH*	SOMarketPlace::GetBooth(UI16 BoothCode)
{
	for(int i=0; i<clBoothInfo.siMapStoreNum;i++)
	{
		if( pBooth[i].pHeader->siMapStoreCode	==	BoothCode )
		{
			return &pBooth[i];
		}
	}
	printf("Code��[%u]�� ���Ǵ븦 ���� �� ����.",BoothCode);
	return NULL;
}

//����������������������������������������������������������������������������
// ��ü ���Ǵ븦 �˻��ؼ� �÷��̾ ������ �Ѵ�.
//����������������������������������������������������������������������������
BOOL	SOMarketPlace::LeaveBooth(SOPlayer *pPlayer)
{
	for(int i=0; i<clBoothInfo.siMapStoreNum; i++ )	
	{
		if(pBooth[i].LeaveBooth(pPlayer) == TRUE )
			return TRUE;
	}
	return FALSE;
}

