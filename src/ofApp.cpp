#include "ofApp.h"

/*
    All components instantiated within a gui
    https://github.com/braitsch/ofxDatGui @braitsch
*/

void ofApp::setup()
{
    ofSetWindowTitle(TITLE);
    ofSetBackgroundColor(0, 0, 0);
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofSetEscapeQuitsApp(false);
    
    font.load(OF_TTF_MONO, 23);
        
    ofAddListener(link.bpmChanged, this, &ofApp::bpmChanged);
    ofAddListener(link.numPeersChanged, this, &ofApp::numPeersChanged);
    
    
// instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    
    // add a dropdown menu //
    vector<string> opts = {midiIn.getInPortList()/*"option - 1", "option - 2", "option - 3", "option - 4"*/};
    gui->addDropdown("Select MidiPort", opts);
    gui->addLabel("---- Midi Clock ----");
    
    gui->addSlider("Retrigger wait", 0, 100, iRetriggerDelay);
    
    //----Midi Clock
    gui->addSlider("MIDI Step:",0,4);
    gui->addTextInput(txtMidiClockState, "stopped");
    gui->addTextInput(txtMidiClockBeats, "");
    //----BPM
    string nearest = ofToString(bpm, 2);
    gui->addTextInput(txtBPM, nearest );
    gui->addBreak();
    
    //----Ableton Link
    gui->addLabel("---- Ableton Link ----");
    gui->addTextInput(txtAbletonLinkPeers, "");
    gui->addSlider("Ableton Link",1, 5);
    
    
    
    
    
// once the gui has been assembled, register callbacks to listen for component specific events //
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    
    //gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->setTheme(new myCustomTheme() );
    gui->setWidth(1024);
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    //cout << "onSliderEvent: " << e.target->getLabel() << " " << e.target->getValue() << endl;
    //if (e.target->is("datgui opacity")) gui->setOpacity(e.scale);
    
    if (e.target->is("Retrigger Delay")){
        iRetriggerDelay = (int)e.value;
        //cout << "Trigger Delay:" << iRetriggerDelay;
    }
    
    
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
    setMidiPort( e.target->getLabel() );
}

void ofApp::draw(){
    
    int iHeight = ofGetWindowHeight();
    int iWidth = ofGetWindowWidth();
    
    //----Ableton Link indicator
    
    gui->getTextInput(txtAbletonLinkPeers)->setText(ofToString(link.getNumPeers()));
    gui->getSlider("Ableton Link")->setValue( 1+ 4*link.getPhase() / link.getQuantum() );
    gui->getTextInput(txtBPM)->setText( ofToString(bpm, 2) );
    
    /*
    std::stringstream ss("");
    ss
    << "bpm:   " << link.getBPM() << std::endl
    << "beat:  " << link.getBeat() << std::endl
    << "phase: " << link.getPhase() << std::endl
    << "peers: " << link.getNumPeers() << std::endl;
    */

    /*
     ofDrawBitmapString(ss.str(), 20, 20);
     ofDrawBitmapString("BPM: " + ofToString(bpmTapper.bpm()), 20, 120);
     ofDrawBitmapString("Hit any key to tap BPM", 50, 120);
     ofDrawBitmapString("BPM: " + ofToString(bpmTapper.bpm()), 50, 100);
     //bpmTapper.draw(50, 200, 10);
     */
    
    /*
    ofDrawBitmapString((clockRunning ? "MIDI clock: running" : "MIDI clock: stopped"), iWidth/2, 30);
    ofDrawBitmapString("pos MIDI beats: "+ofToString(beats), iWidth/2, 58);
    ofDrawBitmapString("pos seconds: "+ofToString(seconds), iWidth/2, 74);
    */
    
    // a MIDI beat is a 16th note, so do a little math to convert to a time signature:
    // 4/4 -> 4 notes per bar & quarter note = 1 beat, add 1 to count from 1 instead of 0
    int quarters = beats / 4; // convert total # beats to # quarters
    int bars = (quarters / 4) + 1; // compute # of bars
    int beatsInBar = (quarters % 4) + 1; // compute remainder as # notes within the current bar
    
    /*
    ofDrawBitmapString("4/4 bars: "+ofToString(bars)+" beat: "+ofToString(beatsInBar), iWidth/2, 90);
    ofDrawBitmapString("bpm: "+ofToString(round(bpm)), iWidth/2, 130);
    */
    
    if(beatsInBar==1){
        //ofLog() << "New DownBeat";
        if( bWaitingForDownbeat ){
            bWaitingForDownbeat = false;
            bNewDownbeat = true;
        }
    }else{
        bWaitingForDownbeat = true;
        if( bResetNext ){
            bResetNext = false;
            beatsInBar = 1;
            link.setBeatForce(0.0);
        }
    }
    
    /*
    int iBoxWidth = iWidth/4;
    ofSetColor(0, 0, 255 * (beatsInBar==1));
    ofDrawRectangle(0, 3*iHeight/5, iBoxWidth, iHeight/5);
    
    ofSetColor(0, 0, 255 * (beatsInBar==2));
    ofDrawRectangle(iBoxWidth, 3*iHeight/5, iBoxWidth, iHeight/5);
    
    ofSetColor(0, 0, 255 * (beatsInBar==3));
    ofDrawRectangle(2*iBoxWidth, 3*iHeight/5, iBoxWidth, iHeight/5);
    
    ofSetColor(0, 0, 255 * (beatsInBar==4));
    ofDrawRectangle(3*iBoxWidth, 3*iHeight/5, iBoxWidth, iHeight/5);
    */
    gui->getSlider("MIDI Step:")->setValue( beatsInBar );
    gui->getTextInput(txtMidiClockState)->setText( clockRunning ? "running" : "stopped" );
    gui->getTextInput(txtMidiClockBeats)->setText( ofToString(beats) );
    
    //ofPushMatrix();
    //ofTranslate(10, 5*ofGetHeight()/6);
    
    /*
    string text = "Growing and Shrinking!";
    float fontWidth = font.stringWidth(text);
    font.drawString(text, -fontWidth/2, 0);
    */
    ofSetColor(100, 100, 100);
    font.drawString(txtMsg, 10, ofGetHeight()-10);
    
    
    
    //ofPopMatrix();
}



void ofApp::update(){
    
    if(timecodeRunning && ofGetElapsedTimeMillis() - timecodeTimestamp > 100) {
        ofLog() << "timecode stopped";
        timecodeRunning = false;
    }
    
    link.setBPM( bpm );
    
    if( bNewDownbeat){
        bNewDownbeat=false;
        ofLog() << "New DownBeat";
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
                bpm += (clock.getBpm() - bpm) / 5; // average the last 5 bpm values
                // no break here so the next case statement is checked,
                // this way we can set clockRunning if we've missed a MIDI_START
                // ie. master was running before we started this example
            }
            // transport control
        case MIDI_START: case MIDI_CONTINUE:
            if(!clockRunning) {
                clockRunning = true;
                retriggerLink();
                ofLog() << "clock started";
            }
            break;
        case MIDI_STOP:
            if(clockRunning) {
                clockRunning = false;
                ofLog() << "clock stopped";
            }
            break;
            
        default:
            break;
    }
 /*
    // update the timecode pos
    if(timecode.update(message.bytes)) {
        
        // we got a new frame pos
        frame = timecode.getFrame();
        
        // if the last message was a timecode quarter frame message,
        // then assume the timecode master has started playback
        if(message.status == MIDI_TIME_CODE) {
            if(!timecodeRunning) {
                timecodeRunning = true;
                ofLog() << "timecode started";
            }
            timecodeTimestamp = ofGetElapsedTimeMillis();
        }
    }
*/
}

void ofApp::retriggerLink(){
    
    //float lastTime = ofGetElapsedTimef();
    //while (ofGetElapsedTimef()< lastTime + sliderVal){
    //    ofLog() << "Retrigger.Wait:" << sliderVal;
    //}
    //if(ofGetElapsedTimef() > starttime+desiredlengthofnote -> note off.
    bReact = false;
//    ofLog() << "Retrigger.Wait:" << sliderVal;
    bpm = bpm/2;
    ofSleepMillis(iRetriggerDelay);
    beats = 1;
    link.setBeatForce(0);
    bReact = true;
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
    midiIn.closePort();
    midiIn.removeListener(this);
}

