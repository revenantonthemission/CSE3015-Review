#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct queue_node {
    int ID;
    struct queue_node *link;
} Node;

class Queue {
    private:
        Node *Front;
        Node *Back;

    public:
        Queue() {
        Front = Back = 0;
        };
        ~Queue() {
            while (!empty()) {
                pop_front();
            }
        };
        void push_back(int data) {
            Node *newNode = new Node;
            newNode->ID = data;
            newNode->link = 0;
            if (!empty()) {
                Back->link = newNode;
                Back = Back->link;
            }
            else {
                Back = newNode;
                Front = Back;
            }
        };
        void pop_front() {
            if (Front != Back) {
                Node *nextFront = Front->link;
                Front->link = 0;
                delete Front;
                Front = nextFront;
            }
            else if(!empty()) {
                Back = 0;
                delete Front;
                Front = 0;
            }
        };
        void print_queue() {
            cout << "current queue: ";
            for(Node* ptr = Front; ptr; ptr = ptr->link) {
                cout << ptr->ID << ' ';
            }
            cout << endl;
        };
        int get_front() {
            if(!empty())
                return Front->ID;
            else
                return -1;
        };
        int get_back() {
            if(!empty())
                return Back->ID;
            else
                return -1;
        };
        bool empty() {
            return (!Front && !Back);
        };
};

vector<vector<int> > adjlist;

// Iterative DFS
void dfs(int point, int target)
{
    vector<int> stack;
    vector<bool> visited(adjlist.size());
    vector<int>::iterator iter;

    // 초기 설정
    stack.push_back(point);
    cout << stack.back() << endl;
    iter = adjlist[stack.back()].begin();
    visited[stack.back()] = true;

    while (!stack.empty())
    {
        if (stack.back() == target) // 목적지를 찾으면 탐색 중단.
            break;
        if (iter == adjlist[stack.back()].end())
        { // 인접한 모든 노드를 방문했으므로 더 이상 갈 곳이 없다.
            // cout << "pop " << stack.back() << endl;
            stack.pop_back();
            iter = adjlist[stack.back()].begin();
        }
        else if (visited[*iter])
        { // 방문하지 않은 노드를 찾는다.
            // cout << "skip " << *iter << endl;
            iter++;
        }
        else
        {
            stack.push_back(*iter);
            cout << stack.back() << endl; // 목적지를 찾기 전까지의 전체 경로
            iter = adjlist[stack.back()].begin();
            visited[stack.back()] = true;
        }
    }
    while (!stack.empty())
    {
        cout << "pop " << stack.back() << endl; // 탈출 경로의 역순
        stack.pop_back();
    }
}

//Iterative BFS
void bfs(int point, int target)
{
    //vector<int> distance(adjlist.size());
    vector<int> parent(adjlist.size());
    vector<bool> visited(adjlist.size());
    vector<int>::iterator iter;
    Queue q;
    int cur;

    //distance[point] = 0;
    visited[point] = true;
    parent[point] = point;
    q.push_back(point);

    while(!q.empty())
    {
        cur = q.get_front();
        //cout << "stage " << cur << endl;
        //q.print_queue();
        for (iter = adjlist[cur].begin(); (iter != adjlist[cur].end()); iter++)
        {
            if (!visited[*iter])
            {
                //distance[*iter] = distance[cur] + 1;
                visited[*iter] = true;
                parent[*iter] = cur;
                //cout << *iter << ' ';
                q.push_back(*iter);
                if(*iter == target) {
                    break;
                }
            }
        }
        //cout << endl;
        q.pop_front();
        if(*iter == target) {
            break;
        }
    }
    if(*iter == target) { // 탈출 경로를 역순으로 출력
        cout << endl;
        cur = *iter;
        do {
            cout << cur << ' ';
            cur = parent[cur];
        } while(cur != parent[cur]);
        cout << cur << endl;
    }
}

int main()
{
    vector<string> maze;
    int mazewidth, mazeheight, width, height;
    string input;
    ifstream fin;
    fin.open("20190328.maz");
    if (fin.fail())
    {
        cerr << "Error!" << endl;
        exit(-1);
    }
    while (!fin.eof())
    {
        getline(fin, input);
        maze.push_back(input);
    }

    mazewidth = maze[0].length();
    mazeheight = maze.size() - 1;
    width = mazewidth / 2;
    height = mazeheight / 2;
    adjlist.assign(width * height, vector<int>(0));
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            if (maze[(a + 1) * 2][b * 2 + 1] == ' ')
            {
                if (find(adjlist[a * width + b].begin(), adjlist[a * width + b].end(), (a + 1) * width + b) == adjlist[a * width + b].end())
                    adjlist[a * width + b].push_back((a + 1) * width + b);
                if (find(adjlist[(a + 1) * width + b].begin(), adjlist[(a + 1) * width + b].end(), a * width + b) == adjlist[(a + 1) * width + b].end())
                    adjlist[(a + 1) * width + b].push_back(a * width + b);
            }
            if (maze[a * 2 + 1][(b + 1) * 2] == ' ')
            {
                if (find(adjlist[a * width + b].begin(), adjlist[a * width + b].end(), a * width + b + 1) == adjlist[a * width + b].end())
                    adjlist[a * width + b].push_back(a * width + b + 1);
                if (find(adjlist[a * width + b + 1].begin(), adjlist[a * width + b + 1].end(), a * width + b) == adjlist[a * width + b + 1].end())
                    adjlist[a * width + b + 1].push_back(a * width + b);
            }
        }
    }
    /*for (vector<vector<int> >::iterator iter_vec = adjlist.begin(); iter_vec != adjlist.end(); iter_vec++)
    {
        for (vector<int>::iterator iter = iter_vec->begin(); iter != iter_vec->end(); iter++)
        {
            cout << (*iter) << ' ';
        }
        cout << endl;
    }*/
    fin.close();
    dfs(0, 24);
    bfs(0, 24);
    return 0;
}
