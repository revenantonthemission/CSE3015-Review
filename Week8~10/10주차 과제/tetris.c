#include "tetris.h"

static struct sigaction act, oact;
int B, count;

int main()
{
    int exit = 0;

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    createRankList();

    srand((unsigned int)time(NULL));

    while (!exit)
    {
        clear();
        switch (menu())
        {
        case MENU_PLAY:
            play();
            break;
        case MENU_RANK:
            rank();
            break;
        case MENU_EXIT:
            exit = 1;
            break;
        case MENU_RECOMMEND:
            recommendedPlay();
            break;
        default:
            break;
        }
    }
    while (ranklist->next)
    {
        User *node = ranklist;
        ranklist = ranklist->next;
        free(node);
    }
    endwin();
    system("clear");
    return 0;
}

void InitTetris()
{
    int i, j;

    for (j = 0; j < HEIGHT; j++)
        for (i = 0; i < WIDTH; i++)
            field[j][i] = 0;

    nextBlock[0] = rand() % 7;
    nextBlock[1] = rand() % 7;
    nextBlock[2] = rand() % 7;

    blockRotate = 0;
    blockY = -1;
    blockX = WIDTH / 2 - 2;
    score = 0;
    gameOver = 0;
    timed_out = 0;

    DrawOutline();
    DrawField();
    DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
    DrawNextBlock(nextBlock);
    PrintScore(score);
}

void DrawOutline()
{
    int i, j;
    /* 블럭이 떨어지는 공간의 태두리를 그린다.*/
    DrawBox(0, 0, HEIGHT, WIDTH);

    /* next block을 보여주는 공간의 태두리를 그린다.*/
    move(2, WIDTH + 10);
    printw("NEXT BLOCK");
    DrawBox(3, WIDTH + 10, 4, 8);

    move(9, WIDTH + 10);
    DrawBox(9, WIDTH + 10, 4, 8);

    /* score를 보여주는 공간의 태두리를 그린다.*/
    move(16, WIDTH + 10);
    printw("SCORE");
    DrawBox(17, WIDTH + 10, 1, 8);
}

int GetCommand()
{
    int command;
    command = wgetch(stdscr);
    switch (command)
    {
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_LEFT:
        break;
    case KEY_RIGHT:
        break;
    case ' ': /* space key*/
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

int ProcessCommand(int command)
{
    int ret = 1;
    int drawFlag = 0;
    switch (command)
    {
    case QUIT:
        ret = QUIT;
        break;
    case KEY_UP:
        if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
            blockRotate = (blockRotate + 1) % 4;
        break;
    case KEY_DOWN:
        if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
            blockY++;
        break;
    case KEY_RIGHT:
        if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
            blockX++;
        break;
    case KEY_LEFT:
        if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
            blockX--;
        break;
    default:
        break;
    }
    if (drawFlag)
        DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
    return ret;
}

void DrawField()
{
    int i, j;
    for (j = 0; j < HEIGHT; j++)
    {
        move(j + 1, 1);
        for (i = 0; i < WIDTH; i++)
        {
            if (field[j][i] == 1)
            {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else
                printw(".");
        }
    }
}

void PrintScore(int score)
{
    move(18, WIDTH + 11);
    printw("%8d", score);
}

void DrawNextBlock(int *nextBlock)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        move(4 + i, WIDTH + 13);
        for (j = 0; j < 4; j++)
        {
            if (block[nextBlock[1]][0][i][j] == 1)
            {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else
                printw(" ");
        }
        move(10 + i, WIDTH + 13);
        for (j = 0; j < 4; j++)
        {
            if (block[nextBlock[2]][0][i][j] == 1)
            {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else
                printw(" ");
        }
    }
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0)
            {
                move(i + y + 1, j + x + 1);
                attron(A_REVERSE);
                printw("%c", tile);
                attroff(A_REVERSE);
            }
        }

    move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width)
{
    int i, j;
    move(y, x);
    addch(ACS_ULCORNER);
    for (i = 0; i < width; i++)
        addch(ACS_HLINE);
    addch(ACS_URCORNER);
    for (j = 0; j < height; j++)
    {
        move(y + j + 1, x);
        addch(ACS_VLINE);
        move(y + j + 1, x + width + 1);
        addch(ACS_VLINE);
    }
    move(y + j + 1, x);
    addch(ACS_LLCORNER);
    for (i = 0; i < width; i++)
        addch(ACS_HLINE);
    addch(ACS_LRCORNER);
}

void play()
{
    int command;
    clear();
    act.sa_handler = BlockDown;
    sigaction(SIGALRM, &act, &oact);
    InitTetris();
    do
    {
        if (timed_out == 0)
        {
            alarm(1);
            timed_out = 1;
        }

        command = GetCommand();
        if (ProcessCommand(command) == QUIT)
        {
            alarm(0);
            DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
            move(HEIGHT / 2, WIDTH / 2 - 4);
            printw("Good-bye!!");
            refresh();
            getch();

            return;
        }
    } while (!gameOver);

    alarm(0);
    getch();
    DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
    move(HEIGHT / 2, WIDTH / 2 - 4);
    printw("GameOver!!");
    refresh();
    getch();
    newRank(score);
}

char menu()
{
    printw("1. play\n");
    printw("2. rank\n");
    printw("3. recommended play\n");
    printw("4. exit\n");
    return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX)
{
    // user code
    // 1. 블록이 필드랑 겹치는지 확인한다.
    // 2. 블록이 전체 필드를 벗어나지 않는지 확인한다.
    int move_flag = 1;
    for (int i = 0; i < BLOCK_HEIGHT; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (block[currentBlock][blockRotate][i][j] == 1)
            {
                if (i + blockY >= HEIGHT || i + blockY < 0)
                    move_flag = 0;
                if (j + blockX >= WIDTH || j + blockX < 0)
                    move_flag = 0;
                if (f[i + blockY][j + blockX])
                    move_flag = 0;
            }
        }
    }
    return move_flag;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX)
{
    // user code
    int prev[3] = {
        0,
    };
    int shadow_y = blockY;
    prev[0] = blockRotate;
    prev[1] = blockY;
    prev[2] = blockX;
    // 1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
    switch (command)
    {
    case KEY_UP:
        blockRotate = (prev[0] + 3) % 4;
        break;
    case KEY_DOWN:
        blockY = prev[1] - 1;
        break;
    case KEY_LEFT:
        blockX = prev[2] + 1;
        break;
    case KEY_RIGHT:
        blockX = prev[2] - 1;
        break;
    }
    // 2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
    while (CheckToMove(field, currentBlock, blockRotate, shadow_y + 1, blockX))
        shadow_y++;
    for (int i = 0; i < BLOCK_HEIGHT; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (block[currentBlock][blockRotate][i][j] == 1 && i + blockY >= 0)
            {
                move(i + blockY + 1, j + blockX + 1);
                printw(".");
                move(i + shadow_y + 1, j + blockX + 1);
                printw(".");
            }
        }
    }
    // 2-1. 이전 그림자도 같이 지운다.
    // 3. 새로운 블록 정보를 그린다.
    DrawBlockWithFeatures(prev[1], prev[2], currentBlock, prev[0]);
}

void BlockDown(int sig)
{
    // user code
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX))
    {
        ++blockY;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    }
    else
    {
        if (blockY == -1)
            gameOver = TRUE;
        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        score += DeleteLine(field);
        PrintScore(score);
        nextBlock[0] = nextBlock[1];
        nextBlock[1] = nextBlock[2];
        nextBlock[2] = rand() % NUM_OF_SHAPE;
        // recommend 관련 호출이 들어갈 자리
        recommendR = 0;
        recommendX = WIDTH / 2 - 2;
        recommendY = 0;
        recommendS = -1;
        recRoot = (RecNode *)malloc(sizeof(RecNode));
        recRoot->lv = 0;
        // m_recRoot->score = recRoot->score = 0;
        recRoot->f = (char(*)[WIDTH])malloc(sizeof(char) * WIDTH * HEIGHT);
        // m_recRoot->f = (char(*)[WIDTH])malloc(sizeof(char) * WIDTH * HEIGHT);
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                *(*(recRoot->f + i) + j) = field[i][j];
                //    *(*(m_recRoot->f + i) + j) = field[i][j];
            }
        }
        recommend(recRoot);
        // modified_recommend(m_recRoot);
        //  메모리 해제
        for (int i = 0; recRoot->c[i]; i++)
        {
            free(recRoot->c[i]->f);
            free(recRoot->c[i]);
        }
        free(recRoot->f);
        free(recRoot);
        blockRotate = 0;
        blockY = -1;
        blockX = WIDTH / 2 - 2;
        DrawNextBlock(nextBlock);
        DrawField();
    }
    timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX)
{
    // user code
    int touched = 0;

    for (int i = 0; i < BLOCK_HEIGHT; i++)
    {
        for (int j = 0; j < BLOCK_WIDTH; j++)
        {
            if (block[currentBlock][blockRotate][i][j] == 1)
            {
                f[i + blockY][j + blockX] = 1;
                if (f[i + blockY + 1][j + blockX] == 1 || i + blockY == HEIGHT - 1)
                    touched++;
            }
        }
    }
    // Block이 추가된 영역의 필드값을 바꾼다.

    return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH])
{
    // user code
    int full_flag = 0;
    int full_line = 0;
    int floor = 0;
    for (int i = 0; i < HEIGHT; i++)
    {
        full_flag = 1;
        for (int j = 0; j < WIDTH; j++)
        {
            if (!f[i][j])
                full_flag = 0;
        }
        if (full_flag)
        {
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < WIDTH; j++)
                    f[k][j] = f[k - 1][j];
            }
            for (int j = 0; j < WIDTH; j++)
                f[0][j] = 0;
            full_line++;
        }
    }
    return full_line * full_line * 100;
    // 1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
    // 2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate)
{
    // user code
    while (CheckToMove(field, blockID, blockRotate, y + 1, x))
        y++;
    DrawBlock(y, x, blockID, blockRotate, '/');
}
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate)
{
    DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ');
    if (score)
        DrawRecommended(recommendY, recommendX, blockID, recommendR);
}

void createRankList()
{
    // user code
    // 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
    // 1. "rank.txt"열기
    // 2. 파일에서 랭킹정보 읽어오기
    // 3. LinkedList로 저장
    // 4. 파일 닫기
    FILE *fp;
    User *rankdata = NULL, *pos = NULL;
    int i, j;
    int usrscore;
    char usrname[NAMELEN + 1];

    // 1. 파일 열기
    fp = fopen("rank.txt", "r");

    // 2. 정보읽어오기
    /* int fscanf(FILE* stream, const char* format, ...);
    stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
    format: 형식지정자 등등
    변수의 주소: 포인터
    return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
    // EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
    if (fscanf(fp, "%d", &score_number) != EOF)
    {
        // 초기 데이터 생성
        for (i = 0; i < score_number; i++)
        {
            fscanf(fp, "%s %d", usrname, &usrscore);
            rankdata = (User *)malloc(sizeof(User));
            strcpy(rankdata->name, usrname);
            rankdata->user_score = usrscore;
            rankdata->next = NULL;

            if (!pos)
            {
                pos = rankdata;
                ranklist = pos;
            }
            else
            {
                pos->next = rankdata;
                pos = pos->next;
            }
        }
    }
    else
    {
        printw("Your rank file is empty!");
    }
    // 4. 파일닫기
    fclose(fp);
}

void rank()
{
    // user code
    // 목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
    // 1. 문자열 초기화
    int X = 1, Y = score_number, ch, i, j;
    User *pos = NULL;
    clear();

    // 2. printw()로 3개의 메뉴출력
    printw("1. list ranks from X to Y\n");
    printw("2. list ranks by a specific name\n");
    printw("3. delete a specific rank\n");

    // 3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
    ch = wgetch(stdscr);

    // 4. 각 메뉴에 따라 입력받을 값을 변수에 저장
    // 4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
    if (ch == '1')
    {
        char input_x[50], input_y[50];
        int chr, idx = 0;
        printw("X: ");
        // 입력 보충 필요함.
        while ((chr = getch()) != '\n')
        {
            X = 0;
            if (chr == KEY_BACKSPACE)
            {
                if (idx)
                    idx--;
            }
            else
            {
                input_x[idx++] = chr;
            }
        }
        input_x[idx] = '\0';
        printw("%s\n", input_x);
        for (int fac, i = 0; idx && i < idx; i++)
        {
            fac = 1;
            for (int j = idx - 1; j > i; j--)
            {
                fac *= 10;
            }
            X += (input_x[i] - '0') * fac;
        }
        idx = 0;
        // 입력 보충 필요함.
        printw("Y: ");
        while ((chr = getch()) != '\n')
        {
            Y = 0;
            if (chr == KEY_BACKSPACE)
            {
                if (idx)
                    idx--;
            }
            else
            {
                input_y[idx++] = chr;
            }
        }
        input_y[idx] = '\0';
        printw("%s\n", input_y);
        for (int fac, i = 0; idx && i < idx; i++)
        {
            fac = 1;
            for (int j = idx - 1; j > i; j--)
            {
                fac *= 10;
            }
            Y += (input_y[i] - '0') * fac;
        }
        printw("\n");
        printw("            name            |   score   \n");
        printw("----------------------------------------\n");

        if (X > Y || X <= 0 || X > score_number || Y <= 0 || Y > score_number)
            printw("search faliure: no rank in the list");
        else
        {
            int count = 0;
            count++;
            for (pos = ranklist;; count++, pos = pos->next)
            {
                if (count >= X && count <= Y)
                {
                    printw("%-27s | %-10d\n", pos->name, pos->user_score);
                }
                else
                {
                    if (count > Y || !pos)
                        break;
                    else
                        continue;
                }
            }
        }
    }

    // 4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
    else if (ch == '2')
    {
        char str[NAMELEN + 1];
        int check = 0, idx = 0, chr;
        User *pos = NULL;
        printw("Input the name: ");
        while ((chr = getch()) != '\n')
        {
            Y = 0;
            if (chr == KEY_BACKSPACE)
            {
                if (idx)
                    idx--;
            }
            else
            {
                str[idx++] = chr;
            }
        }
        str[idx] = '\0';
        printw("%s\n", str);
        printw("            name            |   score   \n");
        printw("----------------------------------------\n");
        for (pos = ranklist; pos; pos = pos->next)
        {
            if (!strcmp(pos->name, str))
            {
                printw("%-27s | %-10d\n", pos->name, pos->user_score);
                check = 1;
            }
        }
        if (!check)
            printw("search faliure: no rank in the list");
    }

    // 4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
    else if (ch == '3')
    {
        int num = 0, idx = 0, chr;
        char str[NAMELEN + 1];
        User *pos = NULL, *prev = NULL;
        printw("Input the rank: ");
        while ((chr = getch()) != '\n')
        {
            Y = 0;
            if (chr == KEY_BACKSPACE)
            {
                if (idx)
                    idx--;
            }
            else
            {
                str[idx++] = chr;
            }
        }
        str[idx] = '\0';
        printw("%s\n", str);
        for (int fac, i = 0; idx && i < idx; i++)
        {
            fac = 1;
            for (int j = idx - 1; j > i; j--)
            {
                fac *= 10;
            }
            num += (str[i] - '0') * fac;
        }
        if (num <= 0 || num > score_number)
            printw("search failure: the rank not in the list\n");
        else
        {
            pos = ranklist;
            if (num == 1)
            {
                ranklist = pos->next;
                pos->next = NULL;
                free(pos);
            }
            else
            {
                for (int i = 1; i < num; i++)
                {
                    prev = pos;
                    pos = pos->next;
                }
                prev->next = pos->next;
                pos->next = NULL;
                free(pos);
            }
            score_number--;
            printw("result: the rank deleted\n");
            writeRankFile();
        }
    }
    getch();
}

void writeRankFile()
{
    // user code
    // 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
    int i;
    User *pos = NULL;
    // 1. "rank.txt" 연다
    FILE *fp = fopen("rank.txt", "w");

    // 2. 랭킹 정보들의 수를 "rank.txt"에 기록
    fprintf(fp, "%d\n", score_number);

    // 3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
    for (i = 0, pos = ranklist; i < score_number; i++, pos = pos->next)
    {
        fprintf(fp, "%s %d\n", pos->name, pos->user_score);
    }
    fclose(fp);
}

void newRank(int score)
{
    // user code
    // 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
    char str[NAMELEN + 1];
    int i = 0, j = 0, idx = 0, ch;
    clear();
    // 1. 사용자 이름을 입력받음
    printw("User name: ");
    while ((ch = wgetch(stdscr)) != '\n')
    {
        if (ch == KEY_BACKSPACE)
        {
            if (idx)
                idx--;
        }
        else
        {
            str[idx++] = ch;
        }
    }
    str[i] = '\0';
    printw("%s\n", str);
    // 2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
    User *pos = NULL;
    User *record = (User *)malloc(sizeof(User));
    strcpy(record->name, str);
    record->user_score = score;
    record->next = NULL;
    if (score_number)
    {
        for (pos = ranklist; pos->next && pos->next->user_score >= score; pos = pos->next)
            ;
        record->next = pos->next;
        pos->next = record;
    }
    else
    {
        ranklist = record;
    }
    score_number++;
    writeRankFile();
}

void DrawRecommended(int y, int x, int blockID, int blockRotate)
{
    // user code
    DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode *root)
{
    int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
    int temp = 0;
    int y, count = 0;
    for (int i = 0; i < NUM_OF_ROTATE; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            y = 0;
            if (CheckToMove(root->f, nextBlock[root->lv], i, y, j))
            {
                // make a new field
                root->c[count] = (RecNode *)malloc(sizeof(RecNode));
                root->c[count]->f = (char(*)[WIDTH])malloc(HEIGHT * WIDTH * sizeof(char));
                // store all the information
                root->c[count]->lv = root->lv + 1;
                root->c[count]->score = root->score;
                for (int p = 0; p < HEIGHT; p++)
                {
                    for (int q = 0; q < WIDTH; q++)
                    {
                        *(*(root->c[count]->f + p) + q) = *(*(root->f + p) + q);
                    }
                }
                if (root->lv < VISUAL_BLOCKS)
                {
                    while (CheckToMove(root->c[count]->f, nextBlock[root->lv], i, y + 1, j))
                        y++;
                    root->c[count]->score += AddBlockToField(root->c[count]->f, nextBlock[root->lv], i, y, j);
                    root->c[count]->score += DeleteLine(root->c[count]->f);
                    max = recommend(root->c[count]);
                    if (!root->lv && max > recommendS)
                    {
                        // if(max == recommendS && recommendY < y)
                        recommendX = j;
                        recommendY = y;
                        recommendR = i;
                        recommendS = max;
                    }
                }
                // 이때의 필드는 MAX_LEVEL만큼 시뮬레이션을 진행한 상태.
                // root->lv == MAX_LEVEL일 때 이곳에 도달하며, 이떄는 점수를 계산하지 않고 그대로 이어져 오며, 이 점수를 기준으로 기존의 최대값인 max와 비교한다.
                else if (root->score > max)
                {
                    max = root->score;
                }
                count++; // 다음 경우로 이동
            }
        }
    }
    return max;
}

void modified_recommend(M_RecNode *root)
{
    // 트리를 1차원으로 변환해 스택처럼 운용한다.
    // RecNode* root => field 복사 후 현재 블록에 대해 이중 for문 => 3개의 블록이 닿을 수 있는 모든 위치를 확인한다.
    // 점수 m_recommendS와 함께 m_recommendX, m_recommendY, m_recommendR을 갱신하고 스택 해제
    int y = 0, tmp = 0;
    tree_s = clock();
    root->x = (M_RecNode *)malloc(sizeof(M_RecNode) * VISUAL_BLOCKS);
    for (int i = 0; i < VISUAL_BLOCKS; i++)
    {
        root->x[i].f = (char(*)[WIDTH])malloc(HEIGHT * WIDTH * sizeof(char));
        for (int p = 0; p < HEIGHT; p++)
        {
            for (int q = 0; q < WIDTH; q++)
            {
                *(*(root->x[i].f + p) + q) = *(*(root->f + p) + q);
            }
        }
    }
    for (int a_i = 0; a_i < NUM_OF_ROTATE; a_i++)
    {
        for (int a_j = 0; a_j < WIDTH; a_j++)
        {
            y = 0;
            root->x[0].score = 0;
            if (CheckToMove(root->f, nextBlock[0], a_i, y, a_j))
            {
                while (CheckToMove(root->f, nextBlock[0], a_i, y + 1, a_j))
                    y++;
                root->x[0].score += AddBlockToField(root->x[0].f, nextBlock[0], a_i, y, a_j);
                root->x[0].score += DeleteLine(root->x[0].f);
                for (int p = 0; p < HEIGHT; p++)
                {
                    for (int q = 0; q < WIDTH; q++)
                    {
                        *(*(root->x[1].f + p) + q) = *(*(root->x[0].f + p) + q);
                    }
                }
            }
            tmp = y;
            for (int b_i = 0; b_i < NUM_OF_ROTATE; b_i++)
            {
                for (int b_j = 0; b_j < WIDTH; b_j++)
                {
                    root->x[1].score = root->x[0].score;
                    y = 0;
                    if (CheckToMove(root->x[0].f, nextBlock[1], b_i, y, b_j))
                    {
                        while (CheckToMove(root->x[0].f, nextBlock[1], b_i, y + 1, b_j))
                            y++;
                        root->x[1].score += AddBlockToField(root->x[1].f, nextBlock[1], b_i, y, b_j);
                        root->x[1].score += DeleteLine(root->x[1].f);
                        if(VISUAL_BLOCKS == 2) {
                            if (root->x[1].score > m_recommendS)
                            {
                                m_recommendX = a_j;
                                m_recommendY = tmp;
                                m_recommendR = a_i;
                                m_recommendS = root->x[1].score;
                            }
                            continue;
                        }
                        for (int p = 0; p < HEIGHT; p++)
                        {
                            for (int q = 0; q < WIDTH; q++)
                            {
                                *(*(root->x[2].f + p) + q) = *(*(root->x[1].f + p) + q);
                            }
                        }
                    }
                    for (int c_i = 0; c_i < NUM_OF_ROTATE && VISUAL_BLOCKS == 3; c_i++)
                    {
                        for (int c_j = 0; c_j < WIDTH; c_j++)
                        {
                            root->x[2].score = root->x[1].score;
                            y = 0;
                            if (CheckToMove(root->x[1].f, nextBlock[2], c_i, y, c_j))
                            {
                                while (CheckToMove(root->x[1].f, nextBlock[2], c_i, y + 1, c_j))
                                    y++;
                                root->x[2].score += AddBlockToField(root->x[2].f, nextBlock[2], c_i, y, c_j);
                                root->x[2].score += DeleteLine(root->x[2].f);
                            }

                            if (root->x[2].score > m_recommendS)
                            {
                                m_recommendX = a_j;
                                m_recommendY = tmp;
                                m_recommendR = a_i;
                                m_recommendS = root->x[2].score;
                            }
                        }
                    }
                }
            }
        }
    }
    tree_e = clock();
    treesize = VISUAL_BLOCKS * (sizeof(M_RecNode) + sizeof(char)*WIDTH*HEIGHT);
    for (int i = 0; i < VISUAL_BLOCKS; i++)
    {
        free(root->x[i].f);
    }
    free(root->x);
}

void recommendedPlay()
{
    int command;
    double diff;
    srand(time(NULL));
    play_s = clock();
    clear();
    act.sa_handler = recommendedBlockDown;
    sigaction(SIGALRM, &act, &oact);
    int i, j;

    for (j = 0; j < HEIGHT; j++)
        for (i = 0; i < WIDTH; i++)
            field[j][i] = 0;

    nextBlock[0] = rand() % 7;
    nextBlock[1] = rand() % 7;
    nextBlock[2] = rand() % 7;

    m_recommendR = 0;
    m_recommendY = -1;
    m_recommendX = WIDTH / 2 - 2;
    score = 0;
    gameOver = 0;
    timed_out = 0;

    DrawOutline();
    DrawField();
    DrawBlock(m_recommendY, m_recommendX, nextBlock[0], m_recommendR, ' ');
    DrawNextBlock(nextBlock);
    PrintScore(score);
    do
    {
        if (timed_out == 0)
        {
            alarm(1);
            timed_out = 1;
        }

        command = GetCommand();
        if (command == QUIT)
        {
            alarm(0);
            DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
            move(HEIGHT / 2, WIDTH / 2 - 4);
            printw("Good-bye!!");
            refresh();
            getch();

            return;
        }
        DrawBlock(m_recommendY, m_recommendX, nextBlock[0], m_recommendR, ' ');
    } while (!gameOver);
    play_e = clock();

    alarm(0);
    getch();
    DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
    move(HEIGHT / 2, WIDTH / 2 - 4);
    printw("GameOver!!");
    refresh();
    getch();
    // newRank(score);
    clear();
    move(0, WIDTH / 2 - 5);
    diff = (double)difftime(play_e, play_s)/CLOCKS_PER_SEC;
    printw("t : %.6lf second(s)\n", diff);
    printw("score(t) : %d\n", score);
    diff = (double)difftime(tree_e, tree_s)/CLOCKS_PER_SEC;
    printw("time(t) : %.6lf\n", diff);
    printw("space(t) : %ld bytes\n", treesize);
    printw("score(t)/time(t) : %.6lf\n", (double)score/diff);
    printw("score(t)/space(t) : %.6lf\n", (double)score/treesize);
    getch();
}

void recommendedBlockDown(int sig)
{
    m_recRoot = (M_RecNode *)malloc(sizeof(M_RecNode));
    m_recRoot->score = 0;
    m_recRoot->f = (char(*)[WIDTH])malloc(sizeof(char) * WIDTH * HEIGHT);
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            *(*(m_recRoot->f + i) + j) = field[i][j];
        }
    }
    modified_recommend(m_recRoot);
    if (!CheckToMove(field, nextBlock[0], m_recommendR, m_recommendY, m_recommendX))
        gameOver = TRUE;
    score += AddBlockToField(field, nextBlock[0], m_recommendR, m_recommendY, m_recommendX);
    score += DeleteLine(field);
    PrintScore(score);
    nextBlock[0] = nextBlock[1];
    nextBlock[1] = nextBlock[2];
    nextBlock[2] = rand() % NUM_OF_SHAPE;
    m_recommendR = 0;
    m_recommendX = WIDTH / 2 - 2;
    m_recommendY = -1;
    m_recommendS = 0;
    //  메모리 해제
    free(m_recRoot->f);
    free(m_recRoot);
    DrawNextBlock(nextBlock);
    DrawField();
    timed_out = 0;
}