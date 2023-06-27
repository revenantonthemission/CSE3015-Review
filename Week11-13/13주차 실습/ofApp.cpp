/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

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

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			isdfs = DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j, p, q;
	
	// TO DO : DRAW MAZE; 
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here	
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			if (input[i][j] == '|') {
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * (i + 1));
			}
			else if (input[i][j] == '-') {
				ofDrawLine(20 * (j - 1), 20 * i, 20 * (j + 1), 20 * i);
			}
		}
	}


	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		cout << "Open" << endl;
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
				isDFS = 0;
				isdfs = false;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;

			// TO DO
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
				cnt++;
			ofBuffer::Line it = buffer.getLines().begin();
			string line = *it;
			MAZE_WIDTH = line.length();
			MAZE_HEIGHT = cnt;
			input = new char*[MAZE_HEIGHT];
			for (int i = 0; i < MAZE_HEIGHT; i++) {
				input[i] = new char[MAZE_WIDTH];
			}
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				strcpy(input[idx++], line.c_str());
			}
			HEIGHT = MAZE_HEIGHT / 2;
			WIDTH = MAZE_WIDTH / 2;
			visited = new int*[HEIGHT];
			for (int i = 0; i < HEIGHT; i++) {
				visited[i] = new int [WIDTH];
			}
			for (vector< vector<int> >::iterator iter = adjlist.begin(); iter != adjlist.end(); iter++) {
				iter->clear();
			}
			adjlist.clear();
			escape_route.clear();
			search_route.clear();

			adjlist.assign(HEIGHT * WIDTH, vector<int>(0));
			for (maze_col = 0; maze_col < HEIGHT; maze_col++)
			{
				for (maze_row = 0; maze_row < WIDTH; maze_row++)
				{
					if (input[(maze_col + 1) * 2][maze_row * 2 + 1] == ' ')
					{
						if (find(adjlist[maze_col * WIDTH + maze_row].begin(), adjlist[maze_col * WIDTH + maze_row].end(), (maze_col + 1) * WIDTH + maze_row) == adjlist[maze_col * WIDTH + maze_row].end())
							adjlist[maze_col * WIDTH + maze_row].push_back((maze_col + 1) * WIDTH + maze_row);
						if (find(adjlist[(maze_col + 1) * WIDTH + maze_row].begin(), adjlist[(maze_col + 1) * WIDTH + maze_row].end(), maze_col * WIDTH + maze_row) == adjlist[(maze_col + 1) * WIDTH + maze_row].end())
							adjlist[(maze_col + 1) * WIDTH + maze_row].push_back(maze_col * WIDTH + maze_row);
					}
					if (input[maze_col * 2 + 1][(maze_row + 1) * 2] == ' ')
					{
						if (find(adjlist[maze_col * WIDTH + maze_row].begin(), adjlist[maze_col * WIDTH + maze_row].end(), maze_col * WIDTH + maze_row + 1) == adjlist[maze_col * WIDTH + maze_row].end())
							adjlist[maze_col * WIDTH + maze_row].push_back(maze_col * WIDTH + maze_row + 1);
						if (find(adjlist[maze_col * WIDTH + maze_row + 1].begin(), adjlist[maze_col * WIDTH + maze_row + 1].end(), maze_col *	WIDTH + maze_row) == adjlist[maze_col * WIDTH + maze_row + 1].end())
							adjlist[maze_col * WIDTH + maze_row + 1].push_back(maze_col * WIDTH + maze_row);
					}
				}
			}
		}
		else {
			cout << "  Needs a '.maz' extension" << endl;
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	for (int i = 0; i < maze_col; i++) {
		delete[] visited[i];
	}
	delete[] visited;
	for (int i = 0; i < HEIGHT; i++) {
		delete[] input[i];
	}
	delete[] input;
}

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	if (isdfs) 
		return true;
	isDFS = 1;
	//TO DO
	//DFSŽ���� �ϴ� �Լ� ( 3����)
	vector<int> stack;
	vector<bool> visited(adjlist.size());
	vector<int>::iterator iter;
	int target = HEIGHT * WIDTH - 1;

	// �ʱ� ����
	stack.push_back(0);
	cout << stack.back() << endl;
	iter = adjlist[stack.back()].begin();
	visited[stack.back()] = true;

	while (!stack.empty())
	{
		if (stack.back() == target) // �������� ã���� Ž�� �ߴ�.
			break;
		if (iter == adjlist[stack.back()].end())
		{ // ������ ��� ��带 �湮�����Ƿ� �� �̻� �� ���� ����.
			// cout << "pop " << stack.back() << endl;
			search_route.push_back(stack.back());
			stack.pop_back();
			iter = adjlist[stack.back()].begin();
		}
		else if (visited[*iter])
		{ // �湮���� ���� ��带 ã�´�.
			// cout << "skip " << *iter << endl;
			iter++;
		}
		else
		{
			stack.push_back(*iter);
			search_route.push_back(stack.back());
			iter = adjlist[stack.back()].begin();
			visited[stack.back()] = true;
		}
	}
	for (iter = stack.begin(); iter != stack.end(); iter++) {
		escape_route.push_back(*iter);
	}
	stack.clear();
	return visited[target];
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS�� ������ ����� �׸���. (3���� ����)
	vector<int>::iterator iter, iter_connect;
	int i, j, p, q;
	
	ofSetColor(ofColor::red);
	for (iter = search_route.begin(); iter != search_route.end() - 1; iter++) {
		i = ((*iter) / WIDTH) * 2 + 1;
		j = ((*iter) % WIDTH) * 2 + 1;
		p = (*(iter + 1) / WIDTH) * 2 + 1;
		q = (*(iter + 1) % WIDTH) * 2 + 1;
		if(find(adjlist[*(iter + 1)].begin(), adjlist[*(iter + 1)].end(), *iter) != adjlist[*(iter + 1)].end())
			ofDrawLine(20 * j, 20 * i, 20 * q, 20 * p);
	}

	ofSetColor(ofColor::green);
	for (iter = escape_route.begin(); iter != escape_route.end() - 1; iter++) {
		i = ((*iter) / WIDTH) * 2 + 1;
		j = ((*iter) % WIDTH) * 2 + 1;
		p = (*(iter + 1) / WIDTH) * 2 + 1;
		q = (*(iter + 1) % WIDTH) * 2 + 1;
		ofDrawLine(20 * j, 20 * i, 20 * q, 20 * p);
	}
}

