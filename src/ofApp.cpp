#include "ofApp.h"
int iCounter=-1;


void ofApp::setup()
{
    ofSetWindowTitle(TITLE);
    ofSetBackgroundColor(0, 0, 0);
    //ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofSetEscapeQuitsApp(false);
    
    font.load(OF_TTF_MONO, 23);

    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    
    // add a dropdown menu //
    vector<string> opts = {midiIn.getInPortList()/*"option - 1", "option - 2", "option - 3", "option - 4"*/};
    gui->addDropdown("Select MidiPort", opts);
    //gui->addLabel("---- Midi Clock ----");
    
    gui->addSlider(LBL_RETRIGGER_WAIT, 0, 200, iRetriggerDelay);
    
    //----Midi Clock
    gui->addSlider(LBL_BEATSTEP,0,4);
    gui->addTextInput(txtMidiClockState, "stopped");
    //gui->addTextInput(txtMidiClockBeats, "");
    //----BPM
    string nearest = ofToString(bpm, 2);
    gui->addTextInput(txtBPM, nearest );
    gui->addBreak();
    
    //----Ableton Link
    //gui->addLabel("---- Ableton Link ----");
    gui->addTextInput(txtAbletonLinkPeers, "");
    //gui->addSlider("Ableton Link",0, 4);
    

// once the gui has been assembled, register callbacks to listen for component specific events //
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->setTheme(new myCustomTheme() );
    gui->setWidth(1024);

    thread.startThread(false); // NON-Blocking?
    thread.setRetriggerDelay(50);
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if (e.target->getLabel().compare(LBL_RETRIGGER_WAIT)==0){
        iRetriggerDelay = (int)e.value;
        thread.setRetriggerDelay(iRetriggerDelay);
    }   
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
    setMidiPort( e.target->getLabel() );
}

//int oldBeatsInBar=0;
//bool bForceNewBeat  = false;
//bool bWaitingForRetriggerLink=false;

void ofApp::draw(){

    //Just to save *some* drawing operations
    
    if(iCounter==0){
        thread.setLinkBPM( bpm );

        int iHeight = ofGetWindowHeight();
        int iWidth = ofGetWindowWidth();
        gui->getTextInput(txtMidiClockState)->setText( clockRunning ? "running" : "stopped" );
        gui->getTextInput(txtAbletonLinkPeers)->setText(ofToString(thread.getLinkNumPeers()));

        gui->getTextInput(txtBPM)->setText( ofToString(bpm, 2) );
        

    }else if(iCounter==30){
        iCounter=-1;
    }
    iCounter++;
    
    // a MIDI beat is a 16th note, so do a little math to convert to a time signature:
    // 4/4 -> 4 notes per bar & quarter note = 1 beat, add 1 to count from 1 instead of 0
    int quarters = beats / 4; // convert total # beats to # quarters
    int bars = (quarters / 4) + 1; // compute # of bars
    int beatsInBar = (quarters % 4) + 1; // compute remainder as # notes within the current bar
    
    if(beatsInBar==1){
        if( bWaitingForDownbeat ){
            bWaitingForDownbeat = false;
            bNewDownbeat = true;
        }
    }else{
        bWaitingForDownbeat = true;
    }
    
    if(clockRunning){
        gui->getSlider(LBL_BEATSTEP)->setValue( (int)beatsInBar );
    }else{
        gui->getSlider(LBL_BEATSTEP)->setValue( 0 );
    }

    ofSetColor(100, 100, 100);
    font.drawString(txtMsg, 10, ofGetHeight()-10);

}



void ofApp::update(){
    
    if(timecodeRunning && ofGetElapsedTimeMillis() - timecodeTimestamp > 100) {
        ofLog() << "timecode stopped";
        timecodeRunning = false;
    }
    
    //thread.setLinkBPM( bpm );
    
    if( bNewDownbeat){
        bNewDownbeat=false;
        //ofLog() << "New DownBeat";
    }
}

void ofApp::keyPressed(int key)
{

}



void ofApp::newMidiMessage(ofxMidiMessage& message) {
    
    // MIDI CLOCK
    
    // update the clock length and song pos in beats
    if(clock.update(message.bytes)) {
        // we got a new song pos
        if(clock.getBeats()!=beatsOfficial){
            beatsOfficial = clock.getBeats();
            //beats = clock.getBeats();
            beats++;
            //ofLog() << "clock Beats:" << beats << std::endl;
        }
        seconds = clock.getSeconds();
    }
    
    // compute the seconds and bpm
    switch(message.status) {
            // compute seconds and bpm live, you may or may not always need this
            // which is why it is not integrated into the ofxMidiClock parser class
        case MIDI_TIME_CLOCK:
            if( bReact ){
                seconds = clock.getSeconds();
                bpm += (clock.getBpm() - bpm) / 25; // average the last 25 bpm values
            }
            break;
        case MIDI_START:
            //ofLog() << "received clock start";
            if(!clockRunning) {
                thread.resetTimer();
                bNewDownbeat = true;
                clockRunning = true;
                beats = 1;                
            }
            iCounter = -1;
            break;
        case MIDI_STOP:
            //ofLog() << "received clock stopp";
            if(clockRunning) {
                clockRunning = false;
                thread.stopLink();
            }
            iCounter = -1;
            break;
            
        default:
            break;
    }
}


void bpmChanged(double &bpm) {
    //        ofLogNotice("bpmChanged") << bpm;
}

void numPeersChanged(std::size_t &peers) {
    //        ofLogNotice("numPeersChanged") << peers;
}


void ofApp::setMidiPort(string pPortName){
    midiIn.closePort();
    midiIn.removeListener(this);
    
    midiIn.openPort( pPortName ); // open a virtual port
    midiIn.ignoreTypes(false, // sysex  <-- don't ignore timecode messages!
                       false, // timing <-- don't ignore clock messages!
                       true); // sensing
    
    // add ofApp as a listener
    midiIn.addListener(this);
    cout << "setMidiPoprt: " << pPortName << " Selected" << endl;
}
    
void ofApp::exit(){
    // clean up
    thread.waitForThread(true, 1000);
    midiIn.closePort();
    midiIn.removeListener(this);
    //thread.stopThread();
    
}

