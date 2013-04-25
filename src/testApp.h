
#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBlobTracker.h"
#include "ofxOsc.h"


class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void mousePressed(int x, int y, int button);
    
    ofVideoGrabber movie;
    
    ofxCvColorImage rgb,hsb;
    ofxCvGrayscaleImage hue,sat,bri,filtered;
    ofxCvContourFinder contours;
    
    ofxBlobTracker  tracker;
    
    int w,h;
    int findHue;
    
    ofPoint pos;
    ofPoint newPos;
    
    float posSpeed;
    int posDiffX;
    int posDiffY;
    
    int slowYourRoll;
    
    ofVec3f v1;
    ofxOscSender sender;
    
    //isaac's variables for sequencer
    string activeColor; //which color is being tracked?
    int activeColorVel; //what is the velocity of the movement of color being tracked?
    float velocity;
    float velocitySmoothed;
    
    
};

#endif
