#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
    
    line_data = 0;
    dot_data = 0;
    chosen = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);
    
    ofSetLineWidth(5);
    if( draw_flag ){
        
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
        for(int i=0; i<num_of_line; i++) {
            ofDrawLine(line_data[i].start.x, line_data[i].start.y, line_data[i].finish.x, line_data[i].finish.y);
        }
        for(int i=0; i<num_of_dot; i++) {
            ofDrawCircle(dot_data[i].x, dot_data[i].y, 10);
        }
        ofSetColor(ofColor::red);
        ofDrawCircle(dot_data[chosen].x, dot_data[chosen].y, 10);
        
        // 2nd week portion.
        ofSetLineWidth(2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        
        // Free the dynamically allocated memory exits.
        free(line_data);
        cout << "Memory for line segment has been freed." << endl;
        free(dot_data);
        cout << "Memory for dot has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;
        
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
        draw_flag = 1;
        draw();
    }
    if (key == 's'){
        // 2nd week portion.
    }
    if (key == 'e'){
        // 2nd week portion.
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    ofSetColor(ofColor::red);
    if (key == OF_KEY_RIGHT){
        if(!load_flag) {
            cout << "No File!" << endl;
            _Exit(1);
        }
        chosen++;
        if(chosen >= num_of_dot) chosen=0;
        cout << "Selcted Dot Coordinate is (" << dot_data[chosen].x << ", " << dot_data[chosen].y << ")" << endl;
    }
    if (key == OF_KEY_LEFT){
        if(!load_flag) {
            cout << "No File!" << endl;
            _Exit(1);
        }
        chosen--;
        if(chosen < 0) chosen=num_of_dot-1;
        cout << "Selcted Dot Coordinate is (" << dot_data[chosen].x << ", " << dot_data[chosen].y << ")" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
    int last_word_size = 0;
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
    int idx_line = 0, idx_dot = 0;
    oneline l;
    dot d;
    
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if(last_word_size > words.size()) input_type = 1;
        if( words.size() == 1){
            if( input_type == 0){ // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                line_data = (oneline*)malloc(sizeof(oneline)*num_of_line);
                cout << "The number of line is: " << num_of_line << endl;
            }
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                dot_data = (dot*)malloc(sizeof(dot)*num_of_dot);
                cout << "The number of dot is: " << num_of_dot << endl;
            }
        }
        else if (words.size() >= 2){
            if( input_type == 0){ // Input for actual information of lines
                l.start.x = atoi(words[0].c_str());
                l.start.y = atoi(words[1].c_str());
                l.finish.x = atoi(words[2].c_str());
                l.finish.y = atoi(words[3].c_str());
                if(l.start.x >= ofGetScreenWidth()) {
                    cout << "Out of Screen" << endl;
                    _Exit(2);
                }
                else if(l.start.y >= ofGetScreenHeight()) {
                    cout << "Out of Screen" << endl;
                    _Exit(2);
                }
                else if(l.finish.x >= ofGetScreenWidth()) {
                    cout << "Out of Screen" << endl;
                    _Exit(2);
                }
                else if(l.finish.y >= ofGetScreenHeight()) {
                    cout << "Out of Screen" << endl;
                    _Exit(2);
                }
                line_data[idx_line] = l;
                idx_line++;
            }
            else { // Input for actual information of dots.
                d.x = atoi(words[0].c_str());
                d.y = atoi(words[1].c_str());
                if(d.x >= ofGetScreenWidth()) {
                    cout << "Out of Screen" << endl;
                    _Exit(2);
                }
                else if(d.y >= ofGetScreenHeight()) {
                    cout << "Out of Screen" << endl;
                    _Exit(2);
                }
                dot_data[idx_dot++] = d;
            }
        } // End of else if.
        last_word_size = words.size();
    } // End of for-loop (Read file line by line).
    
    //initializeWaterLines();
}

void ofApp::initializeWaterLines() {
    ;
}


