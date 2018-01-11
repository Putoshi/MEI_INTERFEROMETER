#include "ofApp.h"
#include "../TriggerBuffer.h"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <string.h>
#include <iterator>

using namespace std;


//--------------------------------------------------------------
void ofApp::setup(){

	unsigned short ushortValue2 = 0x017a;
	ofLogNotice() << "value: " << ushortValue2;

	//adi_path = ofFilePath::getAbsolutePath("frag.frag", true);
	adi_path = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";

	file_.addListener(this, &ofApp::fileEvent);
	file_.addListener(this, &ofApp::fileEvent2);
	file_.setTargetPath(adi_path);



	/*ofLogNotice() << "value: " << 1;
	string fileName = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";
	string text = string(ofBufferFromFile(fileName));
	ofLogNotice() << "value: " << text;*/

	/*std::ifstream ifs("C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak", std::ios_base::in | std::ios_base::binary);
	if (ifs.fail()) {
		std::cerr << "ファイルオープンに失敗" << std::endl;
		std::exit(1);
	}else{
		std::cerr << "ファイルオープンに成功" << std::endl;
	}
	std::string str((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());

	std::string destination;*/

	/*for (int i = 0; i < (int)str.size(); ++i) {
		char buffer[3];
		snprintf(buffer, sizeof(buffer), "%02x", (unsigned char)str[i]);
		destination = destination + buffer;

	}

	int len = (int)strlen(destination.c_str());*/
	/*unsigned char data[len / 2];

	ofstream wf;
	wf.open("output", ios::trunc);

	for (int i = 0; i < len; i += 2) {
		unsigned int x;
		sscanf((char *)(str + i), "%02x", &x);
		data[i / 2] = x;
		wf << data[i / 2];
	}*/

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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


void ofApp::fileEvent(ofFile &file)
{
	std::cout << "loaded event function called" << std::endl;
}
void ofApp::fileEvent2(const void *sender, ofFile &file)
{
	std::cout << "loaded event function (with sender) called" << std::endl;
}
