#ifndef _EARTHQUAKE_H
#define _EARTHQUAKE_H

class _EarthQuake{
private:
   SHORT	IndexY;
   SHORT	EarthQuakeSwitch;
   SHORT	EarthQuakeStep;

public:
   void		Init(void);
   void		SetEarthQuake(void);
   void		Action(void);
   SHORT	GetOffsetY(void);
};


#endif