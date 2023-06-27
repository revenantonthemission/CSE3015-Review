/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon

class Queue {
private:
	typedef struct queue_node {
		int ID;
		struct queue_node* link;
	} Node;
	Node* Front;
	Node* Back;

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
        Node* newNode = new Node;
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
            Node* nextFront = Front->link;
            Front->link = 0;
            delete Front;
            Front = nextFront;
        }
        else if (!empty()) {
            Back = 0;
            delete Front;
            Front = 0;
        }
    };
    void print_queue() {
        cout << "current queue: ";
        for (Node* ptr = Front; ptr; ptr = ptr->link) {
            cout << ptr->ID << ' ';
        }
        cout << endl;
    };
    int get_front() {
        if (!empty())
            return Front->ID;
        else
            return -1;
    };
    int get_back() {
        if (!empty())
            return Back->ID;
        else
            return -1;
    };
    bool empty() {
        return (!Front && !Back);
    };
};

class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key); // Traps escape key if exit disabled
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		bool readFile();
		void freeMemory();
		bool DFS();
		void dfsdraw();
		bool BFS();
		void bfsdraw();
		int HEIGHT;//미로의 높이
		int WIDTH;//미로의 너비
		int MAZE_HEIGHT;
		int MAZE_WIDTH;
		char** input;//텍스트 파일의 모든 정보를 담는 이차원 배열이다.
		int** visited;//방문여부를 저장할 포인
		int maze_col;//미로칸의 열의 인덱스를 가리킨다.
		int maze_row;//미로칸의 행의 인덱스를 가리킨다.
		int k;
		int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
		int isDFS;//DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
		int isBFS;//BFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
		vector<vector<int> > adjlist; //Adjacency List for maze 
		vector<int> search_route; // 탐색 경로를 담는 벡터
		vector<int> escape_route; //탈출 경로를 담는 배열
        vector<int> distance;
        vector<int> parent;
		// Menu
		ofxWinMenu * menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		// Used by example app
		ofTrueTypeFont myFont;
        ofImage myImage;
		float windowWidth, windowHeight;
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;
		bool isdfs;
		bool isbfs;
		// Example functions
 		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);

};

