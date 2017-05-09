#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

// TX must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13
 
void setup() 
{
// Configure onboard LED
  pinMode(LED, OUTPUT); 

// Configure reset pin for output
  pinMode(RFM95_RST, OUTPUT);

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  Serial1.begin(9600);

// Configure serial port for debugging
  Serial.begin(9600);
  delay(100);
 
// Reset the radio
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

// Initialize radio 
  rf95.init();

// Set frequency, power and slow-long range
  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);
}
 
void loop()
{
  if (rf95.available())
  {
// Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      Serial.print((char*)buf);
      Serial.print(",");
      Serial.println(rf95.lastRssi(), DEC);
      delay(10);

// Send a reply
//      uint8_t data[] = "TODO";
//      rf95.send(data, sizeof(data));
//      rf95.waitPacketSent();
      digitalWrite(LED, LOW);
    }
  }
}
