# Introduction
I discovered LoRa RF radio about 2 years ago. At that time, it was one of many different types of low power radio solutions and I didn't really take much notice however I did setup a Google Alert RSS feed and Slack subscription as I was intrigued about reports of over 10km in range and batteries lasting years.

Over the last 6 months (late 2016 onwards) I noticed a lot of media attention around the technology with many announcements of city, state and country wide networks being built so I thought it's about time I checked it out. The LoRa Alliance (https://www.lora-alliance.org/) provides some idea of how much support there is around this technology. Look at the member list. It’s obviously going places.

A search for prototype breakout boards revealed the Adafruit Feather 32u4 with a Semtech SX1276 LoRa Radio Module. This is a sweet piece of kit with an Arduino compatible micro controller. Arduino rocks. The Semtech LoRa radio can be tuned to frequencies between 137-1020MHz. Check out the size of it below next to an Aussie 20 cent coin…
![feather_3078](https://cloud.githubusercontent.com/assets/4920375/25772034/a2f2a50a-32a4-11e7-862f-2bc6c7d03c20.png)

Next step was to find a cheap GPS receiver and batteries. Eventually I settled on the VK2828U7G5LF GPS module and 3.7v LiPo batteries.

![vk2828u8g5lf](https://cloud.githubusercontent.com/assets/4920375/25771792/db6bf13a-329e-11e7-9a6e-a55ecd2b7c7f.png)

# Shopping List
- 2x Adafruit Feather 32u4 with LoRa Radio Modules $34.95 (USD)
- 1x VK2828U7G5LF GPS module ~$8 (USD)
- 2x 3.7v 350mAh LiPo battery $6.95 (USD)
- 2x Micro serial cables to debug Adafruit Feather and connect to Raspberry Pi ~$6.00 (USD)
- 1x Raspberry Pi (see later) ~$55 (USD) + ~$8 for power supply

If you doen't have them already, you'll also need the following...
- Soldering Iron
- Solder
- Wire (I used breadboard jumper wires)
- Pliers to cut and strip wire

All hardware should be less than $200 USD.

I encourage you to have a look around the Adafruit web site. I feel like a kid in a lolly shop when I'm there. The company was started by Limor “Ladyada” Fried, she is an inspiration. Read more about her and how she started the company in the About Us page (https://www.adafruit.com/about).

The Adafruit Feather 32u4 with LoRa Radio has comprehensive documentation on the Adafruit site and I have basically reused their code for these radios and added in some additional code for GPS and processing data.
You can find doco for the GPS module here…
https://github.com/CainZ/V.KEL-GPS/blob/master/VK2828U7G5LF%20Data%20Sheet%2020150902.pdf

It's pretty interesting if you're into that sort of thing, like me!

# Wiring
When I received the gear, I worked out how to connect the GPS receiver up to one of the breakout boards. Here is how it's connected...
- GPS EN -> 5
- GPS TX -> RX
- GPS RX -> TX
- GPS GND -> GND
- GPS VCC -> 3V
You'll also need to solder a whip antenna to both breakout boards. I'm using a public radio band with frequency 915Mhz. A search online returns 87mm as the best length antenna for this frequency.

# Solution Design
![gpstracker](https://cloud.githubusercontent.com/assets/4920375/25830080/d50b65f4-349d-11e7-8016-5bfe612977ad.png)

There are over 30 GPS satellites that contantly transmit their location and time. GPS receivers process data coming from multiple satellites and triangulate the current location, latitude and longitude. The GPS receiver I have used also determines speed and direction. It can also store about a day of data locally on the device which can then be uploaded later.


The GPS receiver has serial RX/TX pins which I've connected to the Arduino serial RX/TX pins on an Adafuit feather. Once the onboard LoRa radio is configured, you can simply read from the Arduino serial RX and send the data to the radio.

The receiving Adafruit feather is configured to listen to data recieved by the onboard LoRa radio and then write the data to another serial port. This second serial port can be connected to the USB of a computer for debugging. In this setup, I've connected the serial port to a Raspberry Pi which I'm using as an internet gateway, having connected it to my ADSL router. The Raspberry Pi reads the data and then forwards to an AWS DynamoDB table.

In order to provide anonymous access and simplify data processing in a browser, I have used the Serverless framework to deploy an API on AWS Gateway backed by AWS Lambda which retreives data from DynamoDB and formats the data so that it can be easily processed on the client side.
 
To visualise the data, I have configured an application in Google apps and obtained an API key which is required when developing applications utilising Google maps. A static html page stored in AWS S3 has some javascript which retrieves location data from the API hosted on AWS, renders a Google map centered on the first data point and then renders all the data points providing a map similar to the one below.

AWS Route53 is used to register and host the DNS zone that I'm using. It also provides friendly DNS names for other AWS services such as AWS CloudFront. AWS CloudFront is used to cache S3 content and allow registration of a domain specific SSL certificates managed by AWS Certificate Manager. IAM provides services to create identities and manage access to services such as AWS DynamoDB. 

#Tooling & Setup
##Serverless
Install node.js and pretty much follow instructions from Serverless web site. You'll need to setup AWS credentials in the ~/.aws folder.

You'll notice in the serverless yaml configuration that cors configuration allows access to the API from specific DNS names. There is also corresponding configuration in the HTTP response found in the handler python code. 

##Raspberry Pi
You'll need to install pyserial, boto3 and setup AWS credentials in the ~/.aws folder before using the script. 

##Arduino
Install the Arduino IDE and follow installation instructions for the Adafruit feather. Specifically, you need to install the drivers and libraries for the radio. "Adafruit Feather 32u4" will need to be selected as the target device prior to uploading code.

##Google API
Google provides free access to their maps API however you need an API key so that Google can manage and throttle access. This key is embedded in the HTML page. The key also has restrictions on where the Google API can be called from so that it cannot be hijacked.

##AWS Configuration
I have setup most of my environment manually however it is possible to deploy the whole of the AWS environment using the serverless framework or CloudFormation.

These are the things you'll need to setup...
- A DynamoDB table and API keys to write to the table
- An S3 public configured for web hosting
- A CloudFront distribution with the S3 bucket configured as the origin
- A certificate manager managed SSL certificate registerd against the CloudFront distribution
- A DNS zone and friendly name/alias to reference the CloudDistribution


# Results
A drive around my area shows about a 400m range in residential areas with buildings and house. You’ll also see up to about 2kms range from the top of the Illawarra escarpment to my house.
![feather_3078_map](https://cloud.githubusercontent.com/assets/4920375/25830088/dec356e2-349d-11e7-9509-b785966f39fb.png)
