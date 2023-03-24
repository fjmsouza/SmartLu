/* 
  Check the new incoming messages, and print via serialin 115200 baud rate.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"
#include "UbidotsEsp32Mqtt.h"
#include "imageLogo.h"
#include "imageSadMood.h"
#include "imageHappyMood.h"
#include "WifiSettings.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
#define threshold 35
//const char *UBIDOTS_TOKEN = "*******";  // Put here your Ubidots TOKEN
//const char *WIFI_SSID = "*******";      // Put here your Wi-Fi SSID
//const char *WIFI_PASS = "*******";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "MyESP";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "Umidade"; // Put here your Variable label to which data  will be published
String packet;
float moisture = 0.0;
unsigned int counter = 0;

Ubidots ubidots(UBIDOTS_TOKEN);

void setup() {
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  displayLogo();
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  // put your setup code here, to run once:
  Serial.begin(115200);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setup();
  ubidots.reconnect();
}

void displayLogo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,smartlu_logo_width,smartlu_logo_height,smartlu_logo_bits);
  Heltec.display->display();
}

void displaySadMood()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,smartlu_sad_width,smartlu_sad_height,smartlu_sad_bits);
  Heltec.display->display();
}

void displayHappyMood()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,smartlu_happy_width,smartlu_happy_height,smartlu_happy_bits);
  Heltec.display->display();
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
    displaySadMood();
  }
  else {
    displayHappyMood();
  }
}

void loop() {
  // try to parse packet
  
//  int packetSize = LoRa.parsePacket();
  
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
//  if (packetSize) {
  if(true){
    
//    packet = "";
    packet = "teste wifi cin";
    // received a packet
    Serial.print("Received packet '");
    // read packet
//    while (LoRa.available()) {
//      packet += (char) LoRa.read();
//    }
    Serial.print(packet);
    // print RSSI of packet
//    Serial.print("' with RSSI ");
//    Serial.println(LoRa.packetRssi());

//    moisture = (packet.substring(10,12)).toInt();
    moisture = 55;
//    counter = (packet.substring(14,20)).toInt();
    counter = counter + 1;

    displayMood(moisture);
//    delay(500);
    delay(15000);
   
    ubidots.add(VARIABLE_LABEL, moisture ); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);

    displayMoisture(moisture, counter);
    delay(5000);
    
  }
  ubidots.loop();
}
