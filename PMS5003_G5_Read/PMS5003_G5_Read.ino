// http://lass-net.org/2016/03/implement-pms3003-g3-with-linkit-smart-7688-duo/
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define TXD 8 // 接G5的 Pin4 RX
#define RXD 9 // 接G5的 Pin5 TX

    long pmcf10=0;
    long pmcf25=0;
    long pmcf100=0;
    long pmat10=0;
    long pmat25=0;
    long pmat100=0;

    String fpmcf010;
    String fpmcf025;
    String fpmcf100;

    String fpmat010;
    String fpmat025;
    String fpmat100;

// instead of using Serial1 for the UART,
// using software serial TX/RX to read PMS3003 sensor:

SoftwareSerial SSerial(RXD, TXD); // RX, TX

void setup() {
// put your setup code here, to run once:

  Serial.begin(9600);
  SSerial.begin(9600);

  // Linkit Smart 7688 Duo use URAT1 which is Serial1 for data exchange between MCU & MPU 
  // to communicate with MPU, MCU side need to set baud rate to 57600 
  Serial1.begin(57600);          

  delay(3000);  
}

void loop() {

    int count = 0;
    unsigned char c;
    unsigned char high;

    while (SSerial.available()) {

      c = SSerial.read();
      Serial.println(c);
      if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
        Serial.println("check failed");
        break;
      }

      if(count > 15){
        /* Serial.println("complete"); */
        break;
      }
      else if(count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14) high = c;
      else if(count == 5){
        pmcf10 = 256*high + c;
      }
      else if(count == 7){
        pmcf25 = 256*high + c;
      }
      else if(count == 9){
        pmcf100 = 256*high + c;
      }
      else if(count == 11){
        pmat10 = 256*high + c;
      }
      else if(count == 13){
        pmat25 = 256*high + c;
      }
      else if(count == 15){
        pmat100 = 256*high + c;
      }

      count++;

   }

    fpmcf010 = FormatDigits(pmcf10); 
    fpmcf025 = FormatDigits(pmcf25); 
    fpmcf100 = FormatDigits(pmcf100); 

    fpmat010 = FormatDigits(pmat10); 
    fpmat025 = FormatDigits(pmat25); 
    fpmat100 = FormatDigits(pmat100); 

    StaticJsonBuffer<200> jsonBuffer;   // if your payload size is too big, please extend buffer to 300 or whatever.
    JsonObject& Jasonroot = jsonBuffer.createObject();
    Jasonroot["pm010_TSI"] = fpmcf010;
    Jasonroot["pm010_ATM"] = fpmat010;
    Jasonroot["pm025_TSI"] = fpmcf025;
    Jasonroot["pm025_ATM"] = fpmat025;
    Jasonroot["pm100_TSI"] = fpmcf100;
    Jasonroot["pm100_ATM"] = fpmat100;
  

//    Jasonroot.printTo(Serial1);
    Jasonroot.prettyPrintTo(Serial);
    /*Jasonroot.prettyPrintTo(Serial1);*/
    Serial.print("\r\n");
   
    while(SSerial.available()) SSerial.read();
    
    delay(10000);

}

String FormatDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  String StrInput = String(digits, DEC);

  if (digits >= 10) {
    return StrInput;
  } else {
    return "0" + StrInput;
  }

}

