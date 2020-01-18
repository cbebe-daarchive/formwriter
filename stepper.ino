#include <Arduino.h>

void setup() {  
  for (int i = 4; i < 8; ++i) {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
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
	} else {
    	return -1;
	}
    
}


void loop() {
  if (Serial.available() > 0) {
    char digit = Serial.read();
    int8_t hexNum = getHexValue(digit);
    
    for (int j = 0; j < 4; j++) {
      digitalWrite(j + 4, hexNum & (1 << j));
    }
  }
} 

