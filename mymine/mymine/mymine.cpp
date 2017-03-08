#include "mymine.hpp"

MyMine::MyMine(int row, int col, int mine_number)
{
	this->row = row ;
	this->col = col ;
	this->mine_number = mine_number ;
	mine_map.resize(row) ;
	for(int i = 0; i < row; i++)
		mine_map[i].resize(col) ;
	generateRandomMineMap() ;
}

MyMine::~MyMine()
{
}

void MyMine::generateRandomMineMap(void)
{
	static bool isSrand = false ;
	if(isSrand == false){
		srand((unsigned int)(time(NULL))) ;
		isSrand = true ;
	}
	for(int i = 0; i < mine_number; ){
		int row_id = getRandomNumber(0, row-1) ;
		int col_id = getRandomNumber(0, col-1) ; // get random position
		if(mine_map[row_id].at(col_id).cell_type == is_mine)
			continue ;
		i++ ; // current random position is not mine, set it to mine
		mine_map[row_id].at(col_id).cell_type = is_mine ;
		mine_map[row_id].at(col_id).center.x = row_id ;
		mine_map[row_id].at(col_id).center.y = col_id ;
	} // generate random mine map
}

void MyMine::freshMineMap(void)
{
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			mine_map[i].at(j).cell_type = not_mine ;
		}
	}
}

int MyMine::getRandomNumber(int begin, int end)
{
	return (rand()%(end-begin+1))+begin ;
}

/////////////////////////////////////////////////////////////////////////////////

DrawMineMap::DrawMineMap(int vas_width, int vas_height,const MyMine &mine_instance)
{
	mine_map = mine_instance.getMineMap() ;
	row = mine_instance.getRow() ;
	col = mine_instance.getCol() ;
	mine_num = mine_instance.getMineNumber() ;
	distance_map.resize(row) ;
	for(int i = 0; i < row; i++)
		distance_map[i].resize(col) ;
	this->vas_width = vas_width ;
	this->vas_height = vas_height ;
	bias_pos.x = 70 ;
	bias_pos.y = 60 ;
	side_len_mine = 20 ;
}

DrawMineMap::~DrawMineMap()
{
}

void DrawMineMap::drawFramework(void)
{
	setlinecolor(0xAA0000) ; // set line color
	int width = (col)*side_len_mine ;
	int height = (row)*side_len_mine ;
	int bias_y = 20 ;
	bias_pos.x = vas_width/2-width/2 ;
	bias_pos.y = vas_height/2-height/2+bias_y ;
	line(bias_pos.x, bias_pos.y, 
		bias_pos.x+width, bias_pos.y) ;
	line(bias_pos.x,bias_pos.y+height,
		bias_pos.x+width,bias_pos.y+height) ;
	line(bias_pos.x,bias_pos.y,
		bias_pos.x, bias_pos.y+height) ;
	line(bias_pos.x+width, bias_pos.y,
		bias_pos.x+width, bias_pos.y+height) ;
	//////////////////////////////////////////////////////////////////////////
	for(int i = 0; i <= row; i++){
		line(bias_pos.x, bias_pos.y+i*side_len_mine,
			bias_pos.x+width, bias_pos.y+i*side_len_mine) ;
	}
	for(int i = 0; i <= col; i++){
		line(bias_pos.x+i*side_len_mine, bias_pos.y,
			bias_pos.x+i*side_len_mine, bias_pos.y+height) ;
	}
}

void DrawMineMap::drawMine(Position pos)
{
	setlinecolor(0x0000AA) ;
	setfillcolor(0x0000AA) ;
	fillcircle(pos.x+bias_pos.x+side_len_mine/2, pos.y+bias_pos.y+side_len_mine/2,
			side_len_mine/2-2) ;
	outtextxy(pos.x+bias_pos.x+5, pos.y+bias_pos.y+2,
		      "M") ; // string represent Mine
}

void DrawMineMap::drawMine(int num_x, int num_y)
{
	Position pos_tmp ;
	pos_tmp.x = (num_y-1)*side_len_mine ;
	pos_tmp.y = (num_x-1)*side_len_mine ;
	drawMine(pos_tmp) ;
}

void DrawMineMap::drawFlag(Position pos)
{
	setlinecolor(0xFF55FF) ;
	setfillcolor(0xFF55FF) ;
	fillcircle(pos.x+bias_pos.x+side_len_mine/2, pos.y+bias_pos.y+side_len_mine/2,
		side_len_mine/2-2) ;
	outtextxy(pos.x+bias_pos.x+5, pos.y+bias_pos.y+2,
		"F") ; // string represent Mine Flag
}

void DrawMineMap::drawFlag(int num_x, int num_y)
{
	Position pos_tmp ;
	pos_tmp.x = (num_y-1)*side_len_mine ;
	pos_tmp.y = (num_x-1)*side_len_mine ;
	drawFlag(pos_tmp) ;
}

void DrawMineMap::clearFlag(int num_x, int num_y)
{
	Position pos ;
	pos.x = (num_y-1)*side_len_mine ;
	pos.y = (num_x-1)*side_len_mine ;
	clearcircle(pos.x+bias_pos.x+side_len_mine/2, pos.y+bias_pos.y+side_len_mine/2,
				side_len_mine/2-1) ;
}

void DrawMineMap::freshMineMap(void)
{
	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++)
			mine_map[i].at(j).cell_type = not_mine ;
}

void DrawMineMap::computeNeighborMine(void)
{
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			distance_map[i].at(j) = 0 ;
		}
	}
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			if(mine_map[i].at(j).cell_type == is_mine){
				updateNeighborMap(i, j) ;
			} // if (i, j) is mine, then update the neighbor mine distance map
		}
	}
}

void DrawMineMap::updateNeighborMap(int num_x, int num_y)
{
	if(num_x == 0 && num_y == 0){
		distance_map[num_x].at(num_y+1)++ ;
		distance_map[num_x+1].at(num_y)++ ;
		distance_map[num_x+1].at(num_y+1)++ ;
		return ;
	}
	if(num_x == row-1 && num_y == col-1){
		distance_map[num_x].at(num_y-1)++ ;
		distance_map[num_x-1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y-1)++ ;
		return ;
	}
	if(num_x == 0 && num_y == col-1){
		distance_map[num_x].at(num_y-1)++ ;
		distance_map[num_x+1].at(num_y)++ ;
		distance_map[num_x+1].at(num_y-1)++ ;
		return ;
	}
	if(num_x == row-1 && num_y == 0){
		distance_map[num_x].at(num_y+1)++ ;
		distance_map[num_x-1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y+1)++ ;
		return ;
	} // update corner mines
	if(num_x == 0){
		distance_map[num_x].at(num_y+1)++ ;
		distance_map[num_x].at(num_y-1)++ ;
		distance_map[num_x+1].at(num_y)++ ;
		distance_map[num_x+1].at(num_y+1)++ ;
		distance_map[num_x+1].at(num_y-1)++ ;
		return ;
	}
	if(num_x == row-1){
		distance_map[num_x].at(num_y+1)++ ;
		distance_map[num_x].at(num_y-1)++ ;
		distance_map[num_x-1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y+1)++ ;
		distance_map[num_x-1].at(num_y-1)++ ;
		return ;
	}
	if(num_y == 0){
		distance_map[num_x+1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y+1)++ ;
		distance_map[num_x+1].at(num_y+1)++ ;
		distance_map[num_x].at(num_y+1)++ ;
		return ;
	}
	if(num_y == col-1){
		distance_map[num_x+1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y)++ ;
		distance_map[num_x-1].at(num_y-1)++ ;
		distance_map[num_x+1].at(num_y-1)++ ;
		distance_map[num_x].at(num_y-1)++ ;
		return ;
	}
	distance_map[num_x].at(num_y+1)++ ;
	distance_map[num_x].at(num_y-1)++ ;
	distance_map[num_x-1].at(num_y)++ ;
	distance_map[num_x-1].at(num_y+1)++ ;
	distance_map[num_x-1].at(num_y-1)++ ;
	distance_map[num_x+1].at(num_y)++ ;
	distance_map[num_x+1].at(num_y+1)++ ;
	distance_map[num_x+1].at(num_y-1)++ ;
	return ;
}

void DrawMineMap::drawNeighborMineCount(void)
{
	char distance_char[2] = {0} ;
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			if(mine_map[i].at(j).cell_type == is_mine){
				drawMine(i+1 ,j+1) ;
				continue ;
			}
			itoa(distance_map[i].at(j), distance_char, 10) ;
			outtextxy((j)*side_len_mine+bias_pos.x+5, (i)*side_len_mine+bias_pos.y+2,
				distance_char) ; 
			memset(distance_char, 0x00, 2*sizeof(char)) ;
		}
	}
}

void DrawMineMap::drawCenterNeighborNumbers(int num_x, int num_y)
{
	if(num_x == 0 && num_y == 0){
		drawNumChar(num_x, num_y+1) ;
		drawNumChar(num_x+1, num_y) ;
		drawNumChar(num_x+1, num_y+1) ;
		return ;
	}
	if(num_x == row-1 && num_y == col-1){
		drawNumChar(num_x, num_y-1) ;
		drawNumChar(num_x-1, num_y) ;
		drawNumChar(num_x-1, num_y-1) ;
		return ;
	}
	if(num_x == 0 && num_y == col-1){
		drawNumChar(num_x, num_y-1) ;
		drawNumChar(num_x+1, num_y) ;
		drawNumChar(num_x+1, num_y-1) ;
		return ;
	}
	if(num_x == row-1 && num_y == 0){
		drawNumChar(num_x, num_y+1) ;
		drawNumChar(num_x-1, num_y) ;
		drawNumChar(num_x-1, num_y+1) ;
		return ;
	} // update corner mines
	if(num_x == 0){
		drawNumChar(num_x, num_y+1) ;
		drawNumChar(num_x, num_y-1) ;
		drawNumChar(num_x+1, num_y) ;
		drawNumChar(num_x+1, num_y+1) ;
		drawNumChar(num_x+1, num_y-1) ;
		return ;
	}
	if(num_x == row-1){
		drawNumChar(num_x, num_y+1) ;
		drawNumChar(num_x, num_y-1) ;
		drawNumChar(num_x-1, num_y) ;
		drawNumChar(num_x-1, num_y+1) ;
		drawNumChar(num_x-1, num_y-1) ;
		return ;
	}
	if(num_y == 0){
		drawNumChar(num_x+1, num_y) ;
		drawNumChar(num_x-1, num_y) ;
		drawNumChar(num_x-1, num_y+1) ;
		drawNumChar(num_x+1, num_y+1) ;
		drawNumChar(num_x, num_y+1) ;
		return ;
	}
	if(num_y == col-1){
		drawNumChar(num_x+1, num_y) ;
		drawNumChar(num_x-1, num_y) ;
		drawNumChar(num_x-1, num_y-1) ;
		drawNumChar(num_x+1, num_y-1) ;
		drawNumChar(num_x, num_y-1) ;
		return ;
	}
	drawNumChar(num_x, num_y+1) ;
	drawNumChar(num_x, num_y-1) ;
	drawNumChar(num_x-1, num_y) ;
	drawNumChar(num_x-1, num_y+1) ;
	drawNumChar(num_x-1, num_y-1) ;
	drawNumChar(num_x+1, num_y) ;
	drawNumChar(num_x+1, num_y+1) ;
	drawNumChar(num_x+1, num_y-1) ;
	return ;
}

void DrawMineMap::drawNumChar(int num_x, int num_y)
{
	char distance_char[2] = {0} ;
	itoa(distance_map[num_x].at(num_y), distance_char, 10) ;
	outtextxy(num_y*side_len_mine+bias_pos.x+5, num_x*side_len_mine+bias_pos.y+2,
		distance_char) ; 
}

void DrawMineMap::showAllMines(void)
{
	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++){
			if(mine_map[i].at(j).cell_type == is_mine){
				drawMine(i+1, j+1) ;
			}
		}
}


void DrawMineMap::generateRandomMineMap(void)
{
	static bool isSrand = false ;
	if(isSrand == false){
		srand((unsigned int)(time(NULL))) ;
		isSrand = true ;
	}
	for(int i = 0; i < mine_num; ){
		int row_id = getRandomNumber(0, row-1) ;
		int col_id = getRandomNumber(0, col-1) ; // get random position
		if(mine_map[row_id].at(col_id).cell_type == is_mine)
			continue ;
		i++ ; // current random position is not mine, set it to mine
		mine_map[row_id].at(col_id).cell_type = is_mine ;
		mine_map[row_id].at(col_id).center.x = row_id ;
		mine_map[row_id].at(col_id).center.y = col_id ;
	} // generate random mine map
}

int DrawMineMap::getRandomNumber(int begin, int end)
{
	return (rand()%(end-begin+1))+begin ;
}

/////////////////////////////////////////////////////////////////////////////////

void MineGame::setGodMode(bool isGod)
{
	this->isGod = isGod ;
}

void MineGame::begin(void)
{
	int score_pos_x = 20 ;
	int score_pos_y = 20 ;
	int restart_pos_x = 100 ;
	int restart_pos_y = 20 ;
	bool isInGameBox = true ;
	bool isInRestartBox = true ;
	bool continue_the_game = true ;
	while(1)
	{
		drawer.freshMineMap() ;
		drawer.generateRandomMineMap() ; // generate random mines map
		drawer.computeNeighborMine() ; // compute the map
		drawer.drawFramework() ;
		drawScoreBar(score_pos_x, score_pos_y) ;
		drawRestart(restart_pos_x, restart_pos_y) ;
		if(isGod == true){
			drawer.drawNeighborMineCount();
		}
		int isBoom = false ;
		continue_the_game = true ;
		while(continue_the_game){
			m_msg = GetMouseMsg() ;
			isInGameBox = isClickGameBoxInside() ;
			isInRestartBox = isClickRestartBoxInside(Position(restart_pos_x, restart_pos_y)) ;
			if(isInGameBox == false){
				if(isInRestartBox == true){

					cleardevice() ;
					continue_the_game = false ;
				} // restart the game, new a game
				continue ; // click not in the game box
			}
			if(isBoom == true)
				break;
			switch(m_msg.uMsg){
			case WM_LBUTTONDOWN: 
				isBoom = judge() ;
				if(isBoom == true){
					drawer.showAllMines() ;
					errorCorrect() ;
				} // boommmm!
				break ; // judge
			case WM_RBUTTONDOWN: 
				flag(); 
				clearrectangle(score_pos_x, score_pos_y, score_pos_x+score_width, score_pos_y+score_height) ;
				drawScoreBar(score_pos_x, score_pos_y) ;
				break ; // flag
			}
		} // a game loop
	} // the main loop 
}

bool MineGame::judge(void)
{
	int num_y = (m_msg.x-drawer.getBiasPos().x) / drawer.getSideLen() ;
	int num_x = (m_msg.y-drawer.getBiasPos().y) / drawer.getSideLen() ;
	if(drawer.getMineMap()[num_x].at(num_y).cell_type == is_mine){
		drawer.drawMine(num_x+1, num_y+1) ;
		return true ;
	}
	else{
		drawer.drawNumChar(num_x, num_y) ;
		return false ;
	}
}

void MineGame::flag(void)
{
	int num_y = (m_msg.x-drawer.getBiasPos().x) / drawer.getSideLen() ;
	int num_x = (m_msg.y-drawer.getBiasPos().y) / drawer.getSideLen() ;
	if(flag_map[num_x].at(num_y) == false){
		drawer.drawFlag(num_x+1, num_y+1) ;
		flag_map[num_x].at(num_y) = true ;
		remain_mine-- ;
	}
	else{
		drawer.clearFlag(num_x+1, num_y+1) ;
		flag_map[num_x].at(num_y) = false ;
		remain_mine++ ;
	}
}


void MineGame::errorCorrect(void)
{
	for(int i = 0; i < mine_map.getRow(); i++)
		for(int j = 0; j < mine_map.getCol(); j++){
			if(flag_map[i].at(j) == true && 
				drawer.getMineMap()[i].at(j).cell_type == is_mine){
				drawRightFlag(i+1, j+1) ;
			}
			if(flag_map[i].at(j) == true && 
				drawer.getMineMap()[i].at(j).cell_type == not_mine){
				drawWrongFlag(i+1, j+1) ;
			}
			
		}
}

void MineGame::drawRightFlag(int num_x, int num_y) //draw the flag which is a right mine
{
	Position pos ;
	setlinecolor(0x5555FF) ;
	pos.x = (num_y-1)*drawer.getSideLen()+drawer.getBiasPos().x ;
	pos.y = (num_x-1)*drawer.getSideLen()+drawer.getBiasPos().y ;
	clearcircle(pos.x+drawer.getSideLen()/2, pos.y+drawer.getSideLen()/2,
		drawer.getSideLen()/2-1) ; // clear the right flag
	line(pos.x, pos.y+drawer.getSideLen()/2,
		pos.x+drawer.getSideLen()/2, pos.y+drawer.getSideLen()) ;
	line(pos.x+drawer.getSideLen()/2, pos.y+drawer.getSideLen(),
		pos.x+drawer.getSideLen(), pos.y);
}

void MineGame::drawWrongFlag(int num_x, int num_y)
{
	Position pos ;
	setlinecolor(0x5555FF) ;
	pos.x = (num_y-1)*drawer.getSideLen()+drawer.getBiasPos().x ;
	pos.y = (num_x-1)*drawer.getSideLen()+drawer.getBiasPos().y ;
	clearcircle(pos.x+drawer.getSideLen()/2, pos.y+drawer.getSideLen()/2,
		drawer.getSideLen()/2-1) ; // clear the right flag
	line(pos.x, pos.y,
		pos.x+drawer.getSideLen(), pos.y+drawer.getSideLen()) ;
	line(pos.x, pos.y+drawer.getSideLen(),
		pos.x+drawer.getSideLen(), pos.y) ;
}

void MineGame::drawScoreBar(int pos_x, int pos_y)
{
	char str_score[4] = {0} ;
	setfillcolor(YELLOW) ;
	fillrectangle(pos_x, pos_y, pos_x+score_width, pos_y+score_height) ;
	settextcolor(RED) ;
	itoa(remain_mine, str_score, 10) ;
	outtextxy(pos_x, pos_y, str_score) ;
	settextcolor(WHITE) ;
}

void MineGame::drawRestart(int pos_x, int pos_y)
{
	setfillcolor(YELLOW) ;
	fillrectangle(pos_x, pos_y, pos_x+restart_width, pos_y+restart_height) ;
	settextcolor(RED) ;
	outtextxy(pos_x, pos_y+6, "RESTART") ;
	settextcolor(WHITE) ;
}

bool MineGame::isClickGameBoxInside(void)
{
	if(m_msg.uMsg == WM_LBUTTONDOWN || m_msg.uMsg == WM_RBUTTONDOWN){
		if(m_msg.x < drawer.getBiasPos().x)
			return false ;
		if(m_msg.y < drawer.getBiasPos().y)
			return false ;
		if(m_msg.x > drawer.getBiasPos().x+mine_map.getCol()*drawer.getSideLen()) 
			return false ;
		if(m_msg.y > drawer.getBiasPos().y+mine_map.getRow()*drawer.getSideLen())
			return false ;
		return true ;
	} // when click the mouse
	else
		return true ;

}

bool MineGame::isClickRestartBoxInside(const Position &pos)
{
	if(m_msg.uMsg == WM_LBUTTONDOWN || m_msg.uMsg == WM_RBUTTONDOWN){
		if(m_msg.x < pos.x)
			return false ;
		if(m_msg.y < pos.y)
			return false ;
		if(m_msg.x > pos.x+restart_width) 
			return false ;
		if(m_msg.y > pos.y+restart_height)
			return false ;
		return true ;
	}
	return false ;
}

void MineGame::freshFlagMap(void)
{
	for(int i = 0; i < mine_map.getRow(); i++){
		for(int j = 0; j < mine_map.getCol(); j++){
			flag_map[i].at(j) = false ;
		}
	}
}