#pragma once

#include "ofMain.h"
#include "CanonSDKWrapper.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyReleased(int key);
    
    private:
        CanonSDKWrapper wrapper;
        void onFileAdded(string &fileName);
        ofImage lastImage;
		
};
