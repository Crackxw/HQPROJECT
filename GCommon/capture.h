#ifndef _CAPTURE_H
#define _CAPTURE_H

class _clCapture
{
private:
	SHORT ScreenNumber;


public:
	_clCapture();

	BOOL ScreenCapture();
	int SaveBIT24(BITMAPIMAGE &bit, char* filename);
	BOOL MapCapture(unsigned char* imagebuffer, int xsize, int ysize);

};


#endif