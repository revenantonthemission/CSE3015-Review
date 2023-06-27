#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr(); // ncurses 초기화
	noecho(); // 키보드 입력을 출력하지 않는다.
	keypad(stdscr, TRUE);	// stdscr 윈도우에서 입력을 받을 때는 stdin에서 지원하지 않는 키패드와 특수문자의 입력을 가능하게 한다.

	srand((unsigned int)time(NULL)); // 무작위 수를 생성하기 위한 시드값을 설정한다.

	while(!exit){
		clear(); // 화면에 그려져 있는 모든 내용을 지운다.
		switch(menu()){
		case MENU_PLAY: play(); break; // Play 선택 -> play() 함수 호출
		case MENU_EXIT: exit=1; break; // Exit 선택 -> exit=1로 설정하여 while문을 빠져나온다.
		default: break;
		}
	}

	endwin(); // ncurses 종료
	system("clear"); // 터미널 화면을 지운다.
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
        break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){ // MENU_PLAY를 선택했을 때 실행되는 함수
	int command;
	clear(); // 화면을 지운다.
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact); // SIGALRM 시그널이 발생하면 BlockDown 함수를 실행한다.
	InitTetris(); // 테트리스 초기 화면 그리기
	do{
		if(timed_out==0){
			alarm(1); //지정한 시간이 되면 SIGARLM 전송
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0); // SIGALRM 시그널을 무시한다.
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);
    //gameOver가 1이 되면 게임이 종료된다.
	alarm(0); // SIGALRM 시그널을 무시한다.
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n"); // stdscr 위의 prinf()
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr); // win에서 받은 키보드 입력을 반환한다.
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int move_flag = 1;
    for(int i=0; i<BLOCK_HEIGHT; i++) {
        for(int j=0; j<BLOCK_WIDTH; j++) {
            if(block[currentBlock][blockRotate][i][j]==1) {
                if((i+blockY >= HEIGHT) || (j + blockY < 0) || (j+blockX >= WIDTH)) move_flag = 0;
                if(f[i+blockY][j+blockX]) move_flag = 0;
            }
        }
    }
    return move_flag;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    //1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
    int x = blockX, y = blockY, r = blockRotate;
    switch(command) {
        case KEY_LEFT:
            x++; //이전에는 오른쪽에 있었음
            break;
        case KEY_RIGHT:
            x--; //이전에는 왼쪽에 있었음
            break;
        case KEY_UP:
            r = (blockRotate+3) % 4;
            break;
        case KEY_DOWN:
            y--;
            break;
        default:
            break;
    }
    //2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
    for(int i=0; i<BLOCK_HEIGHT; i++) {
        for(int j=0; j<BLOCK_WIDTH; j++) {
            if(block[currentBlock][r][i][j] == 1 && i+y >= 0) {
                move(i+y+1, j+x+1);
                addch('.');
            }
        }
    }
    //3. 새로운 블록 정보를 그린다.
    DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
}

void BlockDown(int sig){
	// user code
    if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX) == 1) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    }
    else if(blockY == -1) gameOver = TRUE;
    else {
        AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        score += DeleteLine(field);
        PrintScore(score);
        nextBlock[0] = nextBlock[1];
        nextBlock[1] = rand()%7;
        blockRotate=0;
        blockY=-1;
	    blockX=WIDTH/2-2;
        DrawField();
        DrawNextBlock(nextBlock);
	//강의자료 p26-27의 플로우차트를 참고한다.
    }
    timed_out = 0;
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    for(int i=0; i<BLOCK_HEIGHT; i++) {
        for(int j=0; j<BLOCK_WIDTH; j++) {
            if(block[currentBlock][blockRotate][i][j] == 1) {
                    f[i+blockY][j+blockX]=1;
            }
        }
    }
	//Block이 추가된 영역의 필드값을 바꾼다.
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
    int full_flag = 0;
    int full_line = 0;
    for(int i=0; i<HEIGHT; i++) {
        full_flag = 1;
        for(int j=0; j<WIDTH; j++) {
            if(!f[i][j]) full_flag = 0;
        }
        if(full_flag == 1) {
            //2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
            for(int k=i; k>0; k--) {
                for(int j=0; j<WIDTH; j++) {
                    f[k][j] = f[k-1][j];
                }
            }
            for(int j=0; j<WIDTH; j++) f[i][j] = 0;
            full_line++;
        }
    }
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	return full_line*full_line*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}