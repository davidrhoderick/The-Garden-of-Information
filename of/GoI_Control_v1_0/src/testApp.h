#pragma once

#include "ofMain.h"
#include "ofEvents.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		
		void setupArduino(const int & version);
		// readCSV reads the data from the CSV file and assigns it to the variables
		void readCSV();
		// convertAngle changes the angle of the sunlight into the values for the light array
		void convertAngle(int angle, string blueOrRed);
		// sendToArduino sends commands to the Arduino for controlling the grow box
		void sendToArduino();
		// receiveFromArduino receives data from the Arduino
		void receiveFromArduino();
		// saves data from the grow box to a file
		void saveToFile();
	
		void updateArduino();
	
		ofArduino ard;
		bool bSetupArduino;
		
		Array lightArrayBlue[];	// lightArrayBlue contains the 5 lights' values for the blue light array
		Array lightArrayRed[];	// lightArrayRed contains  the 5 lights' values for the red light array
		int intensityBlue;		// intensityBlue is the intensity of the blue light
		int intensityRed;		// intensityRed is the intensity of the red light
		float temperature;		// temperature is the target temperature for the grow box
		bool water;				// water is TRUE when a water command needs to be issued and FALSE otherwise
};
