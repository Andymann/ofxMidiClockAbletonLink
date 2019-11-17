#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxAbletonLink.h"
#include "ofxMidi.h"
#include "ofxMidiClock.h"
#include "ofxMidiTimecode.h"

class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
    
        
        void setup();
        void draw();
        void update();
        void exit();

        ofTrueTypeFont font;
        string TITLE = "MidiClock to Ableton Link v0.8a";
        string txtMsg = "www.Doktor-Andy.de                            #fcksbt";
    
        ofxAbletonLink link;
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
        Boolean bReact = true;
        Boolean bNewDownbeat = false;
        Boolean bWaitingForDownbeat = true;
        Boolean bResetNext = false;
        int iRetriggerDelay = 50;
        string txtMidiClockState  ="MIDI Clock:";
        string txtMidiClockBeats = "MIDI Beats:";
    
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
        void retriggerLink();
    
        void bpmChanged(double &bpm) {
            //        ofLogNotice("bpmChanged") << bpm;
        }
        
        void numPeersChanged(std::size_t &peers) {
            //        ofLogNotice("numPeersChanged") << peers;
        }
    
        void setMidiPort(string pPortName);
};

class myCustomTheme : public ofxDatGuiTheme{
public:
    myCustomTheme(){
        font.size = 13;
        //font.file = "path/to/font.ttf";
        init();
    }
};
