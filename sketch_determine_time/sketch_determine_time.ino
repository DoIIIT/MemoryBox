int ledPins[] = {12,8,4};
int pinCount = 3;
const int buttonPin = 2;
int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  for (int thisPin=0; thisPin<pinCount; thisPin++){
    pinMode(ledPins[thisPin],OUTPUT);
  }
  pinMode(buttonPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  if (buttonState ==LOW){
    if (millis() == 3000){
      digitalWrite(ledPins[0],HIGH);
      delay(2000);
      digitalWrite(ledPins[0],LOW);
    } 
    if (millis() ==5000){
      digitalWrite(ledPins[1],HIGH);
      delay(2000);
      digitalWrite(ledPins[1],LOW);
    }
    if (millis() ==7000){
      digitalWrite(ledPins[2],HIGH);
      delay(2000);
      digitalWrite(ledPins[2],LOW);
    }
    if (millis() >= 9000){
      digitalWrite(ledPins[0],HIGH);
      digitalWrite(ledPins[1],HIGH);
      digitalWrite(ledPins[2],HIGH);
    }
  } else {
    // turn LED off:
    for (int thisPin=0; thisPin<pinCount; thisPin++){
      digitalWrite(ledPins[thisPin], LOW);
    }
  }
  
}
