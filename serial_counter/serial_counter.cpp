// ---------------------------------------------------
// Name : Charles Ancheta
// ID: 1581672
// CMPUT 274 , Fall 2019
//
// Weekly Assignment 6: Serial Counter
// ---------------------------------------------------
 
# include <Arduino.h>
 
const int LEDPins[5] = {9, 10, 11, 12, 13};
 
void setup() {
	init();
	Serial.begin(9600);
	for (int i = 0; i < 5; i++) {
    	pinMode(LEDPins[i], OUTPUT);
    	digitalWrite(LEDPins[i], LOW);
	}
}
 
int8_t getHexValue(char digit) {
	// converting char to their ASCII value without using int() function
	uint8_t hexNum = digit - '\0';
	// 0 to 9
	if (48 <= hexNum && hexNum <= 57) {
    	return hexNum - 48;
	// A to F
	} else if (65 <= hexNum && hexNum <= 70) {
    	return hexNum - 55;
	// a to F
	} else if (97 <= hexNum && hexNum <= 102) {
    	return hexNum - 87;
	// space or return
	} else if (hexNum == 32 || hexNum == 13) {
    	return -2;
	// any other value returns -1 and is ignored
	} else {
    	return -1;
	}
    
}
void updateLEDs(uint8_t total) {
	for (int i = 0; i < 5; i++) {
    	// uses bitwise operations to determine
    	// what to write on the LED
    	if (((1 << i) & total) == (1 << i)) {
        	digitalWrite(LEDPins[i], HIGH);
    	}
    	else {
        	digitalWrite(LEDPins[i], LOW);
    	}
	}
}
 
int main() {
	setup();
	uint8_t total = 0;
	while (true) {
    	if (Serial.available() > 0) {
        	char digit = Serial.read();
        	int8_t hexNum = getHexValue(digit);
        	if (hexNum == -2) {
            	total = 0;
        	} else if (hexNum > 0){
            	total += hexNum;
        	}
    	}
    	updateLEDs(total);
	}
    
	Serial.flush();
	return 0;
}