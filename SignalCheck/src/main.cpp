#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

  ofSetupOpenGL(1920,960, OF_WINDOW);      //1080 <-------- setup the GL context
  //ofSetupOpenGL(1024,768,OF_WINDOW);

  // this kicks off the running of my app
  // can be OF_WINDOW or OF_FULLSCREEN
  // pass in width and height too:
  ofRunApp(new ofApp());

}
