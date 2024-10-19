#include <GSL.h>
#include <main.h>
#include "yoptionmenu.h"
#include "gamemenu.h"
#include "gmenu.h"
#include "winlose.h"



//--------------------------------------------------------------------------
// Name: AllMemoryFree() 
// Desc: 정모가 사용하는 모든것의 메모리 해제 
//--------------------------------------------------------------------------
VOID	AllMemoryFree()
{	
	FreeMainMenu();
	FreeInitMenu();	
	FreeWinLose();
	FreeOptionMenu();	
}
