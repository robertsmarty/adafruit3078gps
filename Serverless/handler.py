import json
import boto3

def main(event, context):
	dynamodb = boto3.resource('dynamodb',"ap-southeast-2")
	table = dynamodb.Table('deviceLocation')
	result = table.scan(
		ProjectionExpression="deviceLocation.lat,deviceLocation.lng"
    )
	body={"deviceLocations":[]}
	for item in result["Items"]: body["deviceLocations"]+=[{"lat":float(item["deviceLocation"]["lat"]),"lng": float(item["deviceLocation"]["lng"])}]
	response = {
		"statusCode": 200,
		"body": json.dumps(body),
		"headers": {
			"Access-Control-Allow-Origin" : "*"
		},
	}
	return response