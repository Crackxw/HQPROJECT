#include <GSL.h>
#include <main.h>
#include "yoptionmenu.h"
#include "gamemenu.h"
#include "gmenu.h"
#include "winlose.h"



//--------------------------------------------------------------------------
// Name: AllMemoryFree() 
// Desc: ���� ����ϴ� ������ �޸� ���� 
//--------------------------------------------------------------------------
VOID	AllMemoryFree()
{	
	FreeMainMenu();
	FreeInitMenu();	
	FreeWinLose();
	FreeOptionMenu();	
}
