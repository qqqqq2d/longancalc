#include <Arduino.h>       


// the setup function runs once when you press reset or power the board
void setup() {
pinMode(LED_RED, OUTPUT);  
pinMode(LED_GREEN, OUTPUT);
pinMode(LED_BLUE, OUTPUT);
}



// the loop function runs over and over again forever
void loop() {
    digitalWrite(LED_RED, LOW); //kui väljund on LOW, siis LED on sees
    delay(2000); 
    digitalWrite(LED_GREEN, LOW);
    delay(2000); 
    digitalWrite(LED_BLUE, LOW);
    delay(2000); 
    digitalWrite(LED_BLUE, HIGH); 
    delay(1000);  
    digitalWrite(LED_GREEN, HIGH); 
    delay(1000);
    digitalWrite(LED_RED, HIGH); 
    delay(1000);
}
