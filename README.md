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

# Results
A drive around my area shows about a 400m range in residential areas with buildings and house. You’ll also see up to about 2kms range from the top of the Illawarra escarpment to my house.
![feather_3078_map](https://cloud.githubusercontent.com/assets/4920375/25830088/dec356e2-349d-11e7-9509-b785966f39fb.png)
