/*
  控制板上的 LED 每秒on/off一次
  LED pin 13
*/
void setup() {
  // put your setup code here, to run once:
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(2000);
}
