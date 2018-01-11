#pragma once

#include "ofMain.h"
#include "ofxWatchFile.h"
#include "../TriggerBuffer.h"

class ofApp : public ofBaseApp{


	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		string adi_path; // .bakファイルの絶対パス
		ofxWatchFile file_;
		void fileEvent(ofFile &file);
		void fileEvent2(const void *sender, ofFile &file);
		
};
