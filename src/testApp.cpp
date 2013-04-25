#include "testApp.h"
#include "ofxOscSender.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofBackground(0,0,0);
    
    w = 320;
    h = 240;
    
    movie.initGrabber(w, h, true);
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
    
    slowYourRoll = 0;
    velocitySmoothed = 0;
    
    sender.setup("127.0.0.1", 7400);
}



//--------------------------------------------------------------
void testApp::update(){
    
    movie.update();
    
    if (movie.isFrameNew()) {
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels(), w, h);
        
        //mirror horizontal
        rgb.mirror(false, true);
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++) {
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-5,findHue+5) ? 255 : 0;
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, 50, w*h/2, 1, false);
        
        
        //VELOCITY
        posDiffX = (v1.x - pos.x);
        posDiffY = (v1.y - pos.y);
        velocity = sqrt( powf(posDiffX,2) + powf(posDiffY,2));
        
        if (velocity < 50){
            
            velocitySmoothed = 0.98 * velocitySmoothed + 0.02 * velocity;
        }
        
        //velocitySmoothed = 0.98 * velocitySmoothed + 0.02 * velocity;
        
        
        v1.set(pos.x, pos.y);
        
        
        
    }
    
    
    
    //COLOR - Checking to see which color is active (change this, clicking shouldnt have to happen)
    if (findHue > 160 && findHue < 180){
        activeColor = "pink";
        cout << "velocity is " << velocity << endl;
        cout << "active color is " << activeColor << endl;
    
        //change velocity to absolute value
        if (velocity < 0){
            velocity = velocity * (-1);
        }
   
    //if the movement on the x axis of the colored blob is greater than 1 unit (in either direction), then:
        if(velocitySmoothed > 5){
            
            //SEND to OSC the activeColor and posDiffX (velocity on the x axis) (i know theres a way to calculate velocity of vx + vy)
            
            if (slowYourRoll % 20 == 0){
            
                ofxOscMessage x;
                x.setAddress("/playtone");
                x.addFloatArg(velocity);
                x.addStringArg(activeColor);
                sender.sendMessage(x);
                cout << "message sent" << endl;
                //ofSleepMillis(200);
            }
            slowYourRoll++;
        }
    }    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255);
    
    //draw all cv images
    rgb.draw(0,0);
    hsb.draw(640,0);
    hue.draw(0,240);
    sat.draw(320,240);
    bri.draw(640,240);
    filtered.draw(0,480);
    contours.draw(0,480);
    
    ofSetColor(255, 0, 0);
    ofFill();
    
    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        
        ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
        pos.x = contours.blobs[i].centroid.x;
        pos.y = contours.blobs[i].centroid.y;
        
        posSpeed = newPos.x - pos.x;
        
        //draw xPos
        string xposition = "xPos: "+ofToString(pos.x, 15);
        ofDrawBitmapString(xposition, 100, 100);
        
        //draw yPos
        string yposition = "yPos: "+ofToString(pos.y, 15);
        ofDrawBitmapString(yposition, 100, 125);
    }
    
    
    ofDrawBitmapStringHighlight("velocity:  " + ofToString(velocity), 800, ofGetHeight()-120);
    
    ofDrawBitmapStringHighlight("velocitySmoothed:  " + ofToString(velocitySmoothed), 800, ofGetHeight()-100);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //calculate local mouse x,y in image
    int mx = x % w; //whats the remainder of x/w ? 
    int my = y % h; //whats the remainder of y/h ?
    //**smart way of making each box its little world!!
    
    //get hue value on mouse position
    //get the pixel values (rgb) of the mouse position from the ofxCvGrayscaleImage hue
   
    //findHue = hue.getPixels()[my*w+mx];
    findHue = hue.getPixels()[my*w+mx];
    cout << "*******hueFound is " << findHue << endl;
    
    /* HERE ARE THE VALUES TO CONVERT findHue to an actual color:
    Red: 0 (wrapped round from 255)
    Orange: 25
    Yellow: 42
    Green: 85
    Blue: 170
    Purple: 205
    Red: 255 (wraps round to 0)
    */
}
