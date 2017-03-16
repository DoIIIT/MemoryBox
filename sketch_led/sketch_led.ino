int led=12;
int led2=8;

void setup() {
  // put your setup code here, to run once:
  pinMode (led,OUTPUT);
  pinMode (led2,OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(3000);
  digitalWrite(led2, HIGH);
  delay(1000);
  digitalWrite(led2, LOW);
  delay (3000);
//    digitalWrite (LED_BUILTIN, HIGH);
//    delay (1000);
//    digitalWrite(LED_BUILTIN, LOW);
//    delay (1000);
}
