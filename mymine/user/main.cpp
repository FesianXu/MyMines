#include <graphics.h>
#include <conio.h>
#include <iostream>
#include "../mymine/mymine.hpp"
using namespace std ;

int main()
{
	int vas_width = 640 ;
	int vas_height = 480 ;
	initgraph(vas_width, vas_height, 1);   
	MyMine mine(10,20,40) ;
	DrawMineMap map(vas_width,vas_height,mine) ;
	MineGame game(mine, map) ;
	game.setGodMode(false) ;
	game.begin() ;

	getch();               
	closegraph() ;
}