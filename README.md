# ofxMidiClockAbletonLink
</br>
</br>
</br>
<img src="https://github.com/Andymann/ofxMidiClockAbletonLink/blob/main/image.png" />
</br>
</br>
</br>
You'll also need:<br>
ofxDatGui       https://github.com/braitsch/ofxDatGui<br>
ofxAbletonLink  https://github.com/2bbb/ofxAbletonLink<br>
ofxMidi         https://github.com/danomatika/ofxMidi<br>
ofxXmlSettings (builtin)<br>

Please clone and process these addons according to their respective READMEs  
  
   
   
     
If you are new to openFrameworks and want to build the binaries yourself run the following commands in a terminal (MacOS).  
It might take ~10-15 minutes depending on your download speed etc.  
  
Please notice:  
These instructions are tested on MacOS Big Sur and Sonoma. If you are running an older version of MacOS the build might fail. On MacOS Mojave, for example, we successfully compiled the application with openFrameworks v0.10.1.  

  
curl -k -L -o ofx.zip https://github.com/openframeworks/openFrameworks/releases/download/0.12.0/of_v0.12.0_osx_release.zip   
unzip ofx.zip  
rm ofx.zip   
mv of_v* openFrameworks/   
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
