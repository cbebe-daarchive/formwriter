#include <Arduino.h>

void setup() {  
  init();
  for (int i = 4; i < 8; ++i) {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
  Serial.begin(9600);
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

int main() {
  setup();
  while (true) {
    // if (Serial.available() > 0) {
    //   Serial.print("Read: ");
    //   char digit = Serial.read();
    //   int8_t hexNum = getHexValue(digit);
    //   Serial.println(hexNum);
    //   if (hexNum > 0) {
    //     for (int j = 0; j < 4; j++) {
    //       digitalWrite(j + 4, hexNum & (1 << j));
    //     }  
    //   }
      while (Serial.available() == 0) {
        digitalWrite(4, HIGH);
        digitalWrite(5, LOW);
        delay(1);
        // digitalWrite(6, HIGH);
        digitalWrite(7, LOW);
        delay(1);
        digitalWrite(5, HIGH);
        digitalWrite(4, LOW);  
        delay(1);      
        digitalWrite(7, HIGH);
        // digitalWrite(6, LOW);

        delay(1);
      }
    }
  // } 
  Serial.flush();
  return 0;
}
