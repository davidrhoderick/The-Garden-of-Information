#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	lightArrayBlue = new Array( 0, 0, 0, 0, 0 );
	lightArrayRed = new Array( 0, 0, 0, 0, 0 );
	intensityBlue = 0;
	intensityRed = 0;
	temperature = 16f;
	water = FALSE;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

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