//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

// ���п� ���� 
unsigned char SqrtVary[MAX_SQRT_RANGE];           // ��Ʈ �� 
DWORD         SineDegree[32];                     // ���ΰ� 

void InitMath()
{
	
	SHORT i;
	double rad;
	double sine;
	
	// sqrt������ �ʱ�ȭ�Ѵ�. 
	for(int ai=0;ai<MAX_SQRT_RANGE;ai++)
	{
		SqrtVary[ai]=(unsigned char)sqrt(ai);
		
	}
	
	for(i=0;i<32;i++)
	{
		rad=(3.14159/180)*11.25*i;
		
		sine=(double)(sin(rad)*1000);
		SineDegree[i]=(DWORD)sine;
	}
	
}