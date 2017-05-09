#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define GPS_ENABLE 5

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

// Enable GPS
  pinMode(GPS_ENABLE, OUTPUT);
  digitalWrite(GPS_ENABLE, HIGH);

// Configure GPS serial port
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
  char radiodata[1000],c=0;
  String gpsdata,year;

// Read data from GPS module until end of line  
  c=0;
  while(c !=10) {
    if(Serial1.available()) {
      c=Serial1.read();
      gpsdata+=c;
    }
  }
    
  if(gpsdata.startsWith("$GPRMC,")) {
// Got the data we need, format prior to sending  
    digitalWrite(13, HIGH);

// Remove data that we don't need    
    gpsdata.replace("$GPRMC,","");
    gpsdata=gpsdata.substring(0,gpsdata.lastIndexOf(','));
    gpsdata=gpsdata.substring(0,gpsdata.lastIndexOf(','));
    gpsdata=gpsdata.substring(0,gpsdata.lastIndexOf(','));

// Assume speed 0 if no speed returned from GPS module    
    gpsdata.replace(",,",",0,");

// Update year to 4 digits    
    year = gpsdata.substring(gpsdata.length()-2,gpsdata.length());
    gpsdata=gpsdata.substring(0,gpsdata.length()-2)+"20"+year;

// rf95.send wants char array so convert
    gpsdata.toCharArray(radiodata, gpsdata.length()+1);

// Output radio data for debugging purposes      
    Serial.println(radiodata);

// Send radio data and wait until buffer cleared     
    rf95.send((uint8_t *)radiodata, gpsdata.length()+1);
    rf95.waitPacketSent();

    digitalWrite(13, LOW);

// Sleep for 10 seconds and then loop    
    delay(10000);
  }
}

