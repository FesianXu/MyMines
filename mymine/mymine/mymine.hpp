#ifndef _MY_MINE_HPP_
#define _MY_MINE_HPP_

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>

using namespace std ;
class Position
{
public:
	int x;
	int y;
	Position(void){
		x = -1 ;
		y = -1 ;
	}
	Position(int x, int y){ 
		this->x = x ; 
		this->y = y ;
	}
};

enum CellType
{
	is_mine = 1 ,
	not_mine = 2 
};

class MineCell
{

public:
	Position center ; // the position of the center point
	CellType cell_type ;

	MineCell(void){
		center.x = 0 ;
		center.y = 0 ;
		cell_type = not_mine ; // defaultly setting not mine
	}
};

class MyMine // generate mine map
{
public:
	MyMine(int row, int col, int mine_num) ;
	~MyMine() ;

	void generateRandomMineMap(void) ;
	void freshMineMap(void) ;
	vector<vector<MineCell> > getMineMap(void) const{
		return mine_map ;
	}
	int getRow(void) const{
		return row ;
	}
	int getCol(void) const{
		return col ;
	}
	int getMineNumber(void) const{
		return mine_number ;
	}

private:
	vector<vector<MineCell> > mine_map ; // mine_map
	int mine_number ;
	int row ;
	int col ;
	
	int getRandomNumber(int begin, int end) ;
};

class DrawMineMap
{
public:
	DrawMineMap(int vas_width, int vas_height,const MyMine &mine_instance) ;
	~DrawMineMap() ;

	void drawFramework(void) ;
	void drawMine(int num_x, int num_y) ;
	void drawFlag(int num_x, int num_y) ;
	void drawNeighborMineCount(void) ;
	void BeginMouseCapture(void) ;
	void computeNeighborMine(void) ;
	void drawCenterNeighborNumbers(int num_x, int num_y) ;
	void drawNumChar(int num_x, int num_y) ;
	void showAllMines(void) ;
	void generateRandomMineMap(void) ;
	void clearFlag(int num_x, int num_y) ;
	void freshMineMap(void) ;

	int getSideLen(void){ return side_len_mine ;}
	Position getBiasPos(void){ return bias_pos ;} 
	vector<vector<MineCell> > getMineMap(void) const{
		return mine_map ;
	}
private:
	int row ;
	int col ;
	int mine_num ;
	Position bias_pos ;
	int side_len_mine ;
	int vas_width ;
	int vas_height ;
	vector<vector<MineCell> > mine_map ;
	vector<vector<int> > distance_map ;

	void updateNeighborMap(int num_x, int num_y) ;
	void drawFlag(Position pos) ;
	void drawMine(Position pos) ;
	int getRandomNumber(int begin, int end) ;
};

class MineGame
{
public:
	MineGame(MyMine &mines, DrawMineMap draw):mine_map(mines),drawer(draw){
		flag_map.resize(mine_map.getRow()) ;
		for(int i = 0; i < mine_map.getRow(); i++)
			flag_map[i].resize(mine_map.getCol()) ;
		for(int i = 0; i < mine_map.getRow(); i++)
			for(int j = 0; j < mine_map.getCol(); j++)
				flag_map[i].at(j) = false ;
		remain_mine = mines.getMineNumber() ;
	} ;
	~MineGame(){} ;

	void begin(void) ;
	void setGodMode(bool isGod = false) ;
	void errorCorrect(void) ;

private:
	MOUSEMSG m_msg ; // mouse msg 
	MyMine mine_map ;
	int remain_mine ;
	DrawMineMap drawer ;
	vector<vector<bool> > flag_map ;
	bool isGod ;
	const static int score_width = 50 ;
	const static int score_height = 30 ;
	const static int restart_width = 60 ;
	const static int restart_height = 30 ;

	bool judge(void) ;
	void flag(void) ;
	void drawRightFlag(int num_x, int num_y) ;
	void drawWrongFlag(int num_x, int num_y) ;
	void drawScoreBar(int pos_x, int pos_y) ;
	void drawRestart(int pos_x, int pos_y) ;
	int computeScore(void) ;
	bool isClickGameBoxInside(void) ;
	bool isClickRestartBoxInside(const Position &pos) ;
	void freshFlagMap(void) ;
};


#endif