import json
import boto3
import decimal
import serial
import serial.tools.list_ports
import time
#Setup DynamoDB client and connect to table
dynamodb = boto3.resource('dynamodb','ap-southeast-2')
table = dynamodb.Table('deviceLocation')
#Open first available servial port assuming connection to Adafruit feather
ser = serial.Serial(serial.tools.list_ports.comports()[0].device, 9600, timeout=0, parity=serial.PARITY_EVEN, rtscts=1)
while True:
  s = ser.readline()
#Wait for data  
  if len(s) > 0:
    s=s.replace("\r\n","")
    l=s.split(",")
#Make sure the correct number of values are available
 if len(l) == 10:
#Process data if GPS is ready
      if l[1] == "A":
#Grab latitude, longitude, speed and direction. Latitude and longitude rounded to 4 decimal places which is about 10m.
	  lat= round(float(l[2][2:])/60+float(l[2][:2]),4)
        lng= round(float(l[4][3:])/60+float(l[4][:3]),4)
        if l[3] == "S":
          lat=lat*-1
        if l[5] == "W":
          lng=lng*-1
        spd=float(l[6])
        dir=float(l[7])
#Build time and date from data
        tmd=l[8][4:]+"-"+l[8][2:-4]+"-"+l[8][:2]+"T"+l[0][:2]+":"+l[0][2:-5]+":"+l[0][4:-3]+"Z"
        print "%f,%f,%f,%f,%s" % (lat,lng,dir,spd,tmd)
#Load location into DynamoDB. Possible that location already exists however will fail put_item and continue.
        response = table.put_item(
          Item={
            'location': "%s%s" %(lat,lng),
            'deviceLocation':{
              'lat': decimal.Decimal("%f" %(lat)),
              'lng': decimal.Decimal("%f" %(lng)),
            }
          }
        )
