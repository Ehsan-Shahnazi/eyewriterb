#ifndef THREADEDCLVIDEOGRABBER_H_INCLUDED
#define THREADEDCLVIDEOGRABBER_H_INCLUDED

#pragma once

#include "clEye.h"
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxThread.h"
#include "ofEvents.h"

class threadedClVideoGrabber : public clEye, public ofxThread {
protected:
	int pollRate;
	void threadedFunction() {
		while(threadRunning) {
			lock();
			update();						// we don't need lock here only if getThreadedPixels is called from "frameReceived"
			unlock();
			if(isFrameNew())
				ofNotifyEvent(frameReceived, *this);
			ofSleepMillis(pollRate);
		}
	}

public:

	ofEvent<clEye> frameReceived;
	threadedClVideoGrabber() {
		pollRate = 1;
	}
	template <class L>
	void setup(int width, int height, L* listener, bool useTexture = false, bool verbose = false) {
		initGrabber(width, height, useTexture);
		addListener(listener);
	}
	void startCapture(bool verbose = false){
		startThread(true, verbose);
	}
	void setPollRate(int pollRate) {
		this->pollRate = pollRate;
	}
	int getPollRate() {
		return pollRate;
	}
	template <class L>
	void addListener(L* listener) {
		ofAddListener(frameReceived, listener, &(L::frameReceived));
	}
	void getThreadedPixels(ofxCvColorImage & colorImg, bool & bNeedUpdate) {
		lock();
		colorImg.setFromPixels(this->getPixels(), width, height);
		bNeedUpdate = true;
		unlock();
	}
};


#endif // THREADEDCLVIDEOGRABBER_H_INCLUDED
