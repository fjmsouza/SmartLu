/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  https://heltec.org
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"
#include "imageLogo.h"
#include "imageSadMood.h"
#include "imageHappyMood.h"


#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
#define sensor 36
#define eletrovalve 0
#define threshold 35
#define dry 370
#define soaked 125

float moisture = 0.0;

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,smartlu_logo_width,smartlu_logo_height,smartlu_logo_bits);
  Heltec.display->display();
}

void sadMood()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,smartlu_sad_width,smartlu_sad_height,smartlu_sad_bits);
  Heltec.display->display();
}

void happyMood()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,smartlu_happy_width,smartlu_happy_height,smartlu_happy_bits);
  Heltec.display->display();
}

void setup()
{
//  Serial.begin(115200);
//  while (!Serial);
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
  analogReadResolution(9);             // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
  digitalWrite(eletrovalve, HIGH);
  pinMode(eletrovalve, OUTPUT);  
  
}

void displayMoisture(float moisture, unsigned int counter)
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 5, "Moisture: ");
  Heltec.display->drawString(90, 5, String(moisture)+"%");
  Heltec.display->drawString(0, 30, "Packet: ");
  Heltec.display->drawString(90, 30, String(counter));
  Heltec.display->display();
}

void displayMood(float moisture)
{
  if (moisture<threshold){
    sadMood();
  }
  else {
    happyMood();
  }
}

void LoRaSend(float moisture, unsigned int counter)
{
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("Moisture: ");
  LoRa.print(moisture);
  LoRa.print(counter);
  LoRa.endPacket();
}

void valveControl()
{
  if (moisture<threshold){
    digitalWrite(eletrovalve, LOW);
    delay(2000);
    digitalWrite(eletrovalve, HIGH);
  }
}

void loop()
{
  //
  moisture = analogRead(sensor);
  moisture = map(moisture, dry,soaked,0,100);
  
  //control the valve
  valveControl();

  // send packet
  LoRaSend(moisture, counter);

  //show plant's mood in Oled
  displayMood(moisture);
  delay(5000);

  //show moisture in OLed
  displayMoisture(moisture, counter);

  digitalWrite(LED, HIGH);   
  delay(1000);                       
  digitalWrite(LED, LOW);    
  delay(25000);                      
  counter++;
}
