#!/bin/bash

HOST="ec2-52-14-14-114.us-east-2.compute.amazonaws.com"
ENDPOINT="http://{$HOST}/medications"

if [ "$#" -ne 3 ];
	then cat <<EOM

USAGE: $0 <product identifier> <event code> <date string>

  <event code>: USED | EXPIRED | STORAGE_EXCEPTION | COUNTDOWN_STARTED
  <date string>: Date, formatted as 2017-06-22T18:33:27.675Z

EOM
else
	ID=$1

	case $2 in
		USED)
			EVENT_CODE=2
			;;
		EXPIRED)
			EVENT_CODE=1
			;;
		STORAGE_EXCEPTION)
			EVENT_CODE=3
			;;
		COUNTDOWN_STARTED)
			EVENT_CODE=4
			;;
		*)
			echo "ERROR: Unknown event code [$2]"
			exit -1
			;;
	esac
	
	curl -H "Content-Type: application/json" \
	-X POST \
	-d "{ \"code\": \"$EVENT_CODE\", \"date\":\"$3\" }" ${ENDPOINT}/$ID/events
fi
