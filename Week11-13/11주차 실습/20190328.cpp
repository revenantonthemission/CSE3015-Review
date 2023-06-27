#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
#define East 1
#define West 2
#define South 3
#define North 4

class Maze
{
public:
    typedef struct wall
    {
        bool N;
        bool S;
        bool E;
        bool W;
    } wall;
    wall **maze;
    int *connection, *rank;
    int height, width;
    Maze(int m, int n);
    int find(int x);
    int isConnected(int x1, int x2, int y1, int y2);
    void Union(int x1, int x2, int y1, int y2, int dir);
    void printMaze();
    ~Maze();
};

Maze::Maze(int m, int n)
{
    height = m;
    width = n;
    connection = new int[height * width];
    rank = new int[height * width];
    maze = new wall *[height];
    for (int i = 0; i < height; i++)
    {
        maze[i] = new wall[width];
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            connection[i * width + j] = i * width + j;
            rank[i*width+j] = 1;
            maze[i][j].N = false;
            maze[i][j].S = false;
            maze[i][j].E = false;
            maze[i][j].W = false;
        }
    }
}
Maze::~Maze()
{
    for (int i = 0; i < height; i++)
    {
        delete[] maze[i];
    }
    delete[] connection;
    delete[] rank;
    delete[] maze;
}
int Maze::find(int x)
{
    if (x == connection[x])
        return x;
    connection[x] = find(connection[x]);
    return connection[x];
}
int Maze::isConnected(int x1, int x2, int y1, int y2)
{
    if (x1 == x2 && y1 == y2)
        return 0;
    else if (x1 < 0 || x2 < 0 || x1 >= width || x2 >= width)
        return 1;
    else if (y1 < 0 || y2 < 0 || y1 >= height || y2 >= height)
        return 2;
    else if (find(y1 * width + x1) != find(y2 * width + x2))
        return 3;
    else
        return 4;
}
void Maze::Union(int x1, int x2, int y1, int y2, int dir)
{
    int parent1 = find(y1 * width + x1), parent2 = find(y2 * width + x2);
    int x = rank[parent1] < rank[parent2]? parent1 : parent2;
    int y = rank[parent1] < rank[parent2]? parent2 : parent1;
    connection[x] = y;
    rank[x] += rank[y];
    // 추후에 결합 방향 고려
    switch (dir)
    {
    case East:
        maze[y1][x1].E = true;
        maze[y2][x2].W = true;
        break;
    case West:
        maze[y1][x1].W = true;
        maze[y2][x2].E = true;
        break;
    case South:
        maze[y1][x1].S = true;
        maze[y2][x2].N = true;
        break;
    case North:
        maze[y1][x1].N = true;
        maze[y2][x2].S = true;
        break;
    }
}
void Maze::printMaze()
{
    ofstream maz("20190328.maz");
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            if (!maze[j][i].N)
            {
                maz << "+ - ";
            }
            else
                maz << "+   ";
        }
        maz << "+";
        maz << endl;
        for (int i = 0; i < width; i++)
        {
            if (!maze[j][i].W)
            {
                maz << "|   ";
            }
            else
                maz << "    ";
        }
        maz << "|";
        maz << endl;
    }
    for (int i = 0; i < width; i++)
    {
        maz << "+ - ";
    }
    maz << "+";
    maz << endl;
    maz.close();
}

int main(void)
{
    int m, n, tmp;
    int x, y, dir, nx, ny;
    bool connectFlag = false;
    cin >> n >> m;
    srand(time(NULL));

    Maze maze = Maze(m, n);

    do
    {
        y = rand() % maze.height;
        x = rand() % maze.width;
        dir = rand() % 4 + 1;
        switch (dir)
        {
        case East:
            nx = x + 1;
            ny = y;
            break;
        case West:
            nx = x - 1;
            ny = y;
            break;
        case South:
            nx = x;
            ny = y + 1;
            break;
        case North:
            nx = x;
            ny = y - 1;
            break;
        default:
            continue;
        }
        if (maze.isConnected(x, nx, y, ny) == 3)
        {
            maze.Union(x, nx, y, ny, dir);
        }
        connectFlag = true;
        for (int i = 0; i < maze.height; i++)
        {
            for (int j = 0; j < maze.width; j++)
            {
                if (maze.isConnected(0, i, 0, j) == 3)
                    connectFlag = false;
            }
        }
    } while (!connectFlag);

    maze.printMaze();
    return 0;
}
