#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxAbletonLink.h"
#include "ofxMidi.h"
#include "ofxMidiClock.h"
#include "ofxMidiTimecode.h"

const string LBL_RETRIGGER_WAIT = "Retrigger wait";
const string LBL_BEATSTEP = "Beat Step:";
const string TITLE = "MidiClock to Ableton Link v1.03";

class MyThread : public ofThread {

    ofxAbletonLink link;

    bool bWaitingForStuff  =false;
    int iRetriggerDelay = 0;
    // the thread function
    void threadedFunction() {
        // start
        while(isThreadRunning()) {
            if(bWaitingForStuff==true){
                if(ofGetElapsedTimeMillis()>=iRetriggerDelay){
                    cout << "Thread:" << ofGetElapsedTimeMillis() << " retrigger link" << endl;
                    //bWaitingForRetriggerLink=false;
                    bWaitingForStuff=false;
                    //ofApp::retriggerLink();
                    startLink();
                }
            }else{
                sleep(100);
            }
        }
    }

    public:
        void resetTimer(){
            ofResetElapsedTimeCounter();
            bWaitingForStuff = true;
        }

        void startLink(){
            ofLog() << "start link";
            //link.setPhase(0);
            link.setQuantum(4);
            link.setBeatForce(0);
            link.play();
        }

        void stopLink(){
            //ofLog() << "stop link";
            link.stop();
        }

        void setRetriggerDelay(int pDelay){
            this->iRetriggerDelay  = pDelay;
        }

        void setLinkBPM(double pBPM){
            link.setBPM(pBPM);
        }

        void setLinkIsPlaying(bool p){
            link.setIsPlaying(p);
        }

        void setLinkBeatForce(int p){
            link.setBeatForce(p);
        }

        int getLinkNumPeers(){
            return link.getNumPeers();
        }

        int getLinkPhase(){
            return link.getPhase();
        }

        int getLinkQuantum(){
            return link.getQuantum();
        }
};


class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
    
        void setup();
        void draw();
        void update();
        void exit();

        ofTrueTypeFont font;
        string txtMsg = "www.Andyland.info";
    
        //ofxAbletonLink link;
        ofxMidiIn midiIn;
        ofxDatGui* gui;
    
        ofxMidiClock clock; //< clock message parser
        bool clockRunning = false; //< is the clock sync running?
        unsigned int beats = 0; //< song pos in beats
        unsigned int beatsOfficial = 0;
        double seconds = 0; //< song pos in seconds, computed from beats
        string txtBPM = "BPM:";
        string txtAbletonLinkPeers ="Link Peers:";
        double bpm = 120; //< song tempo in bpm, computed from clock length
        bool bReact = true;
        bool bNewDownbeat = false;
        bool bWaitingForDownbeat = true;
        //bool bResetNext = false;
        int iRetriggerDelay = 50;
        string txtMidiClockState  ="MIDI Clock:";
        //string txtMidiClockBeats = "MIDI Beats:";
    
        // MIDI TIMECODE
        ofxMidiTimecode timecode; //< timecode message parser
        bool timecodeRunning = false; //< is the timecode sync running?
        long timecodeTimestamp = 0; //< when last quarter frame message was received
        ofxMidiTimecodeFrame frame; //< timecode frame data, ie. H M S frame rate
    
    
        void keyPressed(int key);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
        void newMidiMessage(ofxMidiMessage& eventArgs);
        void retriggerDelayChanged(int & circleResolution);
        
        void bpmChanged(double &bpm) {
            //        ofLogNotice("bpmChanged") << bpm;
        }
        
        void numPeersChanged(std::size_t &peers) {
            //        ofLogNotice("numPeersChanged") << peers;
        }
    
        void setMidiPort(string pPortName);
    
        MyThread thread;
};

class myCustomTheme : public ofxDatGuiTheme{
public:
    myCustomTheme(){
        font.size = 13;
        //font.file = "path/to/font.ttf";
        init();
    }
};