#!/bin/bash

HOST="ec2-52-14-14-114.us-east-2.compute.amazonaws.com"
ENDPOINT="http://{$HOST}/medications"

if [ "$#" -ne 1 ];
	then echo "USAGE: $0 <product identifier>"
else
	ID=$1
	curl -H "Content-Type: application/json" \
	-X POST \
	-d "{ \"id\":\"$ID\" }" ${ENDPOINT}
fi
