/************************************************************************************************
	파일명 : Josa.cpp
	담당자 : 이윤석

	한글중에서 조사를 검사하여 상황에 맞는 조사로 수정한다.
************************************************************************************************/
//#include "stdafx.h"
#include <GSL.h>
#include "JOSA.h"
#include "wansung2johap.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	DEFAULT_JOSA_FILTER	'^'
/*
은(0xC0BA) / 는(0xB4C2)
을(0xC0BB) / 를(0xB8A6)
과(0xB0FA) / 와(0xBFCD)
아(0xBEC6) / 야(0xBEDF)
이(0xC0CC) / 가(0xB0A1)
으로(0xC0B8,0xB7CE)/로(0xB7CE)

이는/는
이를/를
이와/와
이야/야
이가/가

으로/로

*/

wansung2johap	W2J;

JOSA::JOSA()
{

}

JOSA::~JOSA()
{

}
void JosaSelector(int pointer, char *Source, int Josa_No,char JongSung)
{
	int j=0;
	if (JongSung ==1)
	{
		switch(Josa_No)//Have jongSung
		{
		case 1:
			Source[pointer+1] = (unsigned char)0xc0;
			Source[pointer+2] = (unsigned char)0xba;
		break;

		case 2:
			Source[pointer+1] = (unsigned char)0xc0;
			Source[pointer+2] = (unsigned char)0xbb;
			break;

		case 3:
			Source[pointer+1] = (unsigned char)0xb0;
			Source[pointer+2] = (unsigned char)0xfa;
			break;

		case 4:
			Source[pointer+1] = (unsigned char)0xbe;
			Source[pointer+2] = (unsigned char)0xc6;
			break;

		case 5:
			Source[pointer+1] = (unsigned char)0xc0;
			Source[pointer+2] = (unsigned char)0xcc;
			break;
		default:
			break;
		}

		for(j=pointer+1;j<(int)strlen(Source);j++)//Wipe 1character ^
		{
			Source[j-1] = Source[j];
		}
		Source[j-1] = 0;
	}
	else
	{
		switch(Josa_No)//No JongSung
		{
		case 1:
			Source[pointer+1] = (unsigned char)0xb4;
			Source[pointer+2] = (unsigned char)0xc2;
			break;

		case 2:
			Source[pointer+1] = (unsigned char)0xb8;
			Source[pointer+2] = (unsigned char)0xa6;
			break;

		case 3:
			Source[pointer+1] = (unsigned char)0xbf;
			Source[pointer+2] = (unsigned char)0xcd;
			break;

		case 4:
			Source[pointer+1] = (unsigned char)0xbe;
			Source[pointer+2] = (unsigned char)0xdf;
			break;

		case 5:
			Source[pointer+1] = (unsigned char)0xb0;
			Source[pointer+2] = (unsigned char)0xa1;
			break;
		case 6:// Wipe 2nd character : 이,으
			for (j=pointer+3;j<(int)strlen(Source);j++)
			{
				Source[j-2] = Source[j];
			}
			Source[j-2] = 0;
			break;
		}
		for (j=pointer+1;j<(int)strlen(Source);j++)//Wipe 1 Character ^
		{
			Source[j-1] = Source[j];
		}
		Source[j-1] = (unsigned char)0;
	}
}

void ProcENIGR(int pointer,char *Source,int Josa_No)
{
	int i,j,l;
	unsigned char	a,b;
	unsigned char	*h_char;
	unsigned char	*hj_char;
	short			c;
	char No_Table[10] = {01,01,00,01,00,00,01,01,01,00};
	char temp[4]={0,0,0,0};

	int  Alp[] = {'p','n','l','m'};
	int  Alp_index=4;
	

	int hj_no=0;
	c=0;j=0;l=0;
	i = pointer-1;
	h_char = new unsigned char [10];
	a = (unsigned) Source[i];
	if (a>=0x30 && a<=0x39)//Check Number
	{
		if (No_Table[ (a-0x30) ] ==1)
			hj_no=1;
		else
			hj_no=0;
	}
	else
	if ( (a>=0x41 && a<=0x5a) || (a>=0x61 && a<=0x7a))
	{

		for (j=0;j<Alp_index;j++)
		{
			if (Alp[j] == a)
				hj_no=1;
			else
				hj_no=0;
		}
	}
	else
	if (a>=0xA1 && a<=0xFE)//Check Hangul 2nd Byte
	{
		b = (unsigned) Source[i-1];
		if (b==0xA4 || (b >=0xB0 && b<=0xC8 ))//Check Hangul 1st Byte
		{
			ZeroMemory(h_char,10);
			h_char[0]=b;
			h_char[1]=a;
			h_char[2]=0;
			hj_char = W2J.Input_Wansung(h_char);
			if ( (hj_char[1]&0x1F)^0x01 ==0x01)// have JongSung.
			{
				hj_no=1;
			}
			else
			{
				hj_no=0;
			}
		}
	}
	JosaSelector(pointer, Source, Josa_No,hj_no);
	delete [] h_char;
	return;
}

BOOL ProcEE(char *Source)
{
	unsigned char c=0,d=0;
	unsigned short a=0;
	c = Source[0];
	d = Source[1];
	a = c*0x100+d;
	if(Source[2] ==0 || Source[2] ==0x20)
		return TRUE;
	return FALSE;
}

void JOSA::JosaProc (char* Source )
{
	int	i,j,k;
	if (Source[0] == 0)
		return;

	for (i=0;i<(int)strlen(Source);i++)
	{
		switch((unsigned) Source[i])
		{
			case	'^':
				if ( (i+2)<(int)strlen(Source))
				{

					j = (unsigned)Source[i+1]&0xff;
					j = j*0x100;

					k = (unsigned)Source[i+2]&0xff;
					k = k+j;

					switch (k)
					{
						case	0xC0BA:// 은
						case	0xB4C2:// 는
							ProcENIGR(i,Source,1);
							k=0;
							break;
						case	0xC0BB:// 을
						case	0xB8A6:// 를
							ProcENIGR(i,Source,2);
							k=0;
							break;
						case	0xB0FA:// 과
						case	0xBFCD:// 와
							ProcENIGR(i,Source,3);
							k=0;
							break;
						case	0xBEC6:// 아
						case	0xBEDF:// 야
							ProcENIGR(i,Source,4);
							k=0;
							break;
						case	0xC0CC:// 이
							if( ((i+4)<(int)strlen(Source) )&& ProcEE(&Source[i+3]) )
							{
								ProcENIGR(i,Source,6);
								break;
							}
						case	0xB0A1:// 가
							ProcENIGR(i,Source,5);
							k=0;
							break;
						case 0xC0B8://으
							ProcENIGR(i,Source,6);
							break;

						default:
							k=0;
						break;
					}
				}
			break;
			default:
				k=0;
			break;
		}
	}
	return ;
}

void JOSA::JosaProc (char *Source , char *Filter )
{
	return ;
}
