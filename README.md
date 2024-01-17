# ofxMidiClockAbletonLink

You'll also need:<br>
ofxDatGui       https://github.com/braitsch/ofxDatGui<br>
ofxAbletonLink  https://github.com/2bbb/ofxAbletonLink<br>
ofxMidi         https://github.com/danomatika/ofxMidi<br>

Please clone and process these addons according to their respective READMEs  
  


  
If you are new to openFrameworks and want to build the binaries yourself run the following commands in a terminal (MacOS).  
It might take ~10-15 minutes depending on your download speed etc:

curl -L -o ofx.zip https://github.com/openframeworks/openFrameworks/releases/download/0.12.0/of_v0.12.0_osx_release.zip   
unzip ofx.zip  
mv of_v0.12.0_osx_release openFrameworks/   
cd openFrameworks/   
export OF_ROOT=$(pwd)   
cd addons/   
git clone https://github.com/braitsch/ofxDatGui.git   
git clone https://github.com/danomatika/ofxMidi.git   
git clone --recursive https://github.com/2bbb/ofxAbletonLink.git   
cd ../apps/myApps/   
git clone https://github.com/Andymann/ofxMidiClockAbletonLink.git   
cd ofxMidiClockAbletonLink/   
echo y | rm -r ./bin/ofxMidiClockAbletonLink.app   
make clean && make  
open ./bin    
