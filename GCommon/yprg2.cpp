//--------------------------------------------------------------------
//  ´ã´ç : ±èÅÂ°ï
//--------------------------------------------------------------------
#include <GSL.h>
#include <yprg2.h>

void FindStraightLine(SHORT sx, SHORT sy, SHORT ex, SHORT ey, SHORT &number, SHORT* lparray, SI32 siMaxNumber)
{
	int		t, distance;
	int		xerr=0, yerr=0, delta_x, delta_y;
	int		incx, incy;
	int		i;

	number=0;

	delta_x=ex-sx;
	delta_y=ey-sy;

	if(delta_x > 0)			incx = 1;
	else if(delta_x == 0)	incx = 0;
	else					incx =-1;

	if(delta_y > 0)			incy = 1;
	else if(delta_y == 0)	incy = 0;
	else					incy =-1;

	delta_x = abs(delta_x);
	delta_y = abs(delta_y);

	if(delta_x > delta_y) distance = delta_x;
	else distance=delta_y;

	for(t=0, i=0; t < distance+1; t++, i+=2)
	{

		xerr+=delta_x;
		yerr+=delta_y;

		if(xerr > distance)
		{			
			xerr-=distance;
			sx+=incx;
		}

		if(yerr > distance)
		{
			yerr-=distance;
			sy+=incy;
		}

		lparray[i]=sx;
		lparray[i+1]=sy;

		number++;

		if(siMaxNumber == number)
			break;
	}	
}