//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

// 수학용 변수 
unsigned char SqrtVary[MAX_SQRT_RANGE];           // 루트 값 
DWORD         SineDegree[32];                     // 사인값 

void InitMath()
{
	
	SHORT i;
	double rad;
	double sine;
	
	// sqrt변수를 초기화한다. 
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