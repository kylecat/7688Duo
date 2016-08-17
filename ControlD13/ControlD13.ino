/*
   從 OpenWrt -> Arduino
   輸入0 or 1控制 LED13

   OpenWrt:
   control-led13.py

*/
void readStringFrom7688() {
  String rdata = "";
  if (Serial1.available() > 0) {
    rdata = Serial1.read();
  }
  if (rdata != "") {
    Serial.print("Received: ");
    Serial.print(rdata);
    if (rdata == "1") {
      Serial.println("led on");
      //led(true);
    }
    else {
      Serial.println("led off");
      //led(false);
    }
  }
  delay(100);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // open serial connection to USB Serial port (connected to computer)
  Serial1.begin(57600);  // open internal serial connection to MT7688AN
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read data from MT7688
  readStringFrom7688();
  // control-led13.py
  // control D13 led on or off
  int c = Serial1.read(); // read from MT7688AN
  if (c != -1) {
    Serial.println(c);
    switch (c) {
      case '0': // turn off D13 when receiving "0"
        digitalWrite(13, 0);
        break;
      case '1': // turn off D13 when receiving "1"
        digitalWrite(13, 1);
        break;
    }
  }
}
