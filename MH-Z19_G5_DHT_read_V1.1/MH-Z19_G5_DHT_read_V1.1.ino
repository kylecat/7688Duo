/*
 * 功能：整合溫溼度、懸浮微粒、二氧化碳感測資料，並輸出至序列埠(Serial)
 * 
 * Sensor種類：
 * 1.溫溼度：   DHT11
 * 2.懸浮微粒： PMS5003(G5)
 * 3.二氧化碳： MH-Z19
 * 
 * 資料傳輸方式：
 * 1.溫溼度：   未知
 * 2.懸浮微粒： UART (PWM, I2C方式未確認)
 * 3.二氧化碳： PWM  (UART可用，但似乎無法同時使用兩組)
 */
#include <SoftwareSerial.h>

// DHT libary
#include <DHT.h>

// DHT PIN set
#define DHTPIN 9
#define LED 13

// DHT Initialization
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// CO2 PIN set
#define pwmPin 10 // D6 for PWM

// CO2 Initialization
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char CO2response[9]; //接收資料用
unsigned long th, tl, ppm, ppm2, ppm3 = 0;

// PM Pin set
SoftwareSerial PMSerial(8, 7); // RX, TX
unsigned int PMresponse[63]; //接收資料用
int pmcf10, pmcf25, pmcf100, pmat10, pmat25, pmat100;


void setup() {
  Serial.begin(9600);
  pinMode(pwmPin, INPUT); // MH-Z19 CO2 PWM
  PMSerial.begin(9600);    // PMS5003 G5 UART
  
  pinMode(LED, OUTPUT);
  dht.begin();
}

// 把UART 指定位置的數值換算成濃度print out
long PM_UART(unsigned int index) {
  
  int high = PMresponse[index];
  // Serial.print("high: ");
  // Serial.println(high);
 
  int next = index + 1;
  int low = PMresponse[next];
  // Serial.print("low: ");
  // Serial.println(low);
  
  int concentraion = 256*high+low;
  return concentraion;
  //Serial.println(concentraion);
  }

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println(" *C ");
  Serial.println("==================================================");
  delay(100);
  
  //PM via UART
  /* 
  // print the len of PMSerial
  Serial.print("PM data :");
  Serial.print(PMSerial.available()); 
  Serial.println("byte.");
  */
  if (PMSerial.available()) {
    for (int i=0; i<63; i++){
      PMresponse[i] = PMSerial.read();
      }
    //data output
    //標準狀態濃度(CF=1)
    Serial.print("PM 1(cf):");
    Serial.print(PM_UART(4));
    Serial.print("PM2.5(cf):");
    Serial.print(PM_UART(6));
    Serial.print("PM10(cf):");
    Serial.println(PM_UART(8));
    //大氣環境濃度
    Serial.print("PM 1(atm):");
    Serial.print(PM_UART(10));
    Serial.print("PM2.5(atm):");
    Serial.print(PM_UART(12));
    Serial.print("PM10(atm):");
    Serial.println(PM_UART(14));
    // 0.3, 0.5, 1.0, 2.5, 5.0, 10.0粒徑的顆粒數輸出(每0.1L)
    Serial.print("PM 03(count):");
    Serial.print(PM_UART(16));
    Serial.print("PM 05(count):");
    Serial.print(PM_UART(18));
    Serial.print("PM 10(count):");
    Serial.println(PM_UART(20));
    Serial.print("PM 25(count):");
    Serial.print(PM_UART(22));
    Serial.print("PM 50(count):");
    Serial.print(PM_UART(24));
    Serial.print("PM100(count):");
    Serial.println(PM_UART(26));
    Serial.println("=================================================");
    delay(100);
  }
  
  //CO2 via pwm
  
  do {
    th = pulseIn(pwmPin, HIGH, 1004000) / 1000;
    tl = 1004 - th;
    ppm2 = 2000 * (th-2)/(th+tl-4);
  } while (th == 0);

  // data output
  Serial.print("CO2(PWM 2000):");
  Serial.println(ppm2);
  Serial.println("=================================================");

  delay(10000);
}
