#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	lightArrayBlue = new Array( 0, 0, 0, 0, 0 );
	lightArrayRed = new Array( 0, 0, 0, 0, 0 );
	intensityBlue = 0;
	intensityRed = 0;
	temperature = 16f;
	water = FALSE;
	
	// replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
	ard.connect("/dev/tty.usbmodemfd121", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::setupArduinoSerial(const int & comPort) {
	
	// FROM SERIAL EXAMPLE
	/*ofSetVerticalSync(true);
	 
	 bSendSerialMessage = false;
	 ofBackground(255);	
	 ofSetLogLevel(OF_LOG_VERBOSE);
	 
	 font.loadFont("DIN.otf", 64);
	 
	 serial.listDevices();
	 vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	 
	 // this should be set to whatever com port your serial device is connected to.
	 // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	 // arduino users check in arduino app....		
	 serial.setup(0, 9600); //open the first device
	 //serial.setup("COM4"); // windows example
	 //serial.setup("/dev/tty.usbserial-A4001JEC",9600); // mac osx example
	 //serial.setup("/dev/ttyUSB0", 9600); //linux example
	 
	 nTimesRead = 0;
	 nBytesRead = 0;
	 readTime = 0;
	 memset(bytesReadString, 0, 4);*/
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// FROM SERIAL EXAMPLE
	/*if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
	 ofSetColor(0);
	 } else {
	 ofSetColor(220);
	 }
	 string msg;
	 msg += "click to test serial:\n";
	 msg += "nBytes read " + ofToString(nBytesRead) + "\n";
	 msg += "nTimes read " + ofToString(nTimesRead) + "\n";
	 msg += "read: " + ofToString(bytesReadString) + "\n";
	 msg += "(at time " + ofToString(readTime, 3) + ")";
	 font.drawString(msg, 50, 100);*/
}

//--------------------------------------------------------------
void testApp::readCSV(){
	
}

//--------------------------------------------------------------
void testApp::convertAngle(int angle, string blueOrRed){
	int intAngle = angle / 36;			// get the first active array
	float fractionAngle = angle / 36f;	// get the fraction between the first and second active arrays
	fractionAngle -= intAngle;
	
	if(blueOrRed == "BLUE")
	{
		lightArrayBlue[intAngle] = int((1f - fractionAngle) * float(intensityBlue));
		lightArrayBlue[intAngle+1] = int(fractionAngle * float(intensityBlue));
	}
	else
	{
		lightArrayRed[intAngle] = int((1f - fractionAngle) * float(intensityRed));
		lightArrayRed[intAngle+1] = int(fractionAngle * float(intensityRed));
	}
}

//--------------------------------------------------------------
void testApp::sendToArduino(){
	
}

//--------------------------------------------------------------
void testApp::receiveFromArduino(){
	
	// FROM SERIAL EXAMPLE
	/*if (bSendSerialMessage){
		
		// (1) write the letter "a" to serial:
		serial.writeByte('a');
		
		// (2) read
		// now we try to read 3 bytes
		// since we might not get them all the time 3 - but sometimes 0, 6, or something else,
		// we will try to read three bytes, as much as we can
		// otherwise, we may have a "lag" if we don't read fast enough
		// or just read three every time. now, we will be sure to 
		// read as much as we can in groups of three...
		
		nTimesRead = 0;
		nBytesRead = 0;
		int nRead  = 0;  // a temp variable to keep count per read
		
		unsigned char bytesReturned[3];
		
		memset(bytesReadString, 0, 4);
		memset(bytesReturned, 0, 3);
		
		while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
			nTimesRead++;	
			nBytesRead = nRead;
		};
		
		memcpy(bytesReadString, bytesReturned, 3);
		
		bSendSerialMessage = false;
		readTime = ofGetElapsedTimef();
	}*/
}

//--------------------------------------------------------------
void testApp::saveToFile(){
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}