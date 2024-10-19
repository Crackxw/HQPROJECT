#include <stdlib.h>
#include "SORand.h"

unsigned long	lrand()
{
	unsigned long	lRandValue;
	unsigned char	*pcRandValue;	

	pcRandValue	=	( unsigned char* )&lRandValue;

	pcRandValue[ 0 ] = rand() % 255;
	pcRandValue[ 1 ] = rand() % 255;
	pcRandValue[ 2 ] = rand() % 255;
	pcRandValue[ 3 ] = rand() % 255;

	return	lRandValue;
}