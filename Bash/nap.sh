#!/bin/bash

BeepIt() {
    Result="1"
    while [ "$Result" -ne "0" ]; do
	Data=$(read -s -t 1 -n 1)
	Result=$?
	printf '\a';
    done
}

player="madplay -a -30" # Sample player. Examples:
                        #
                        # madplay for Linux (command line mp3 player) 
                        # aplay for Linux 
                        # open in Mac OS

file='./E.mp3' # Sample file: better in the same directory of the
	       # script and without spaces in the filename

minutes=$1
player2=$2
if [ "$player2" != "" ]; then
    player=$player2
fi
echo $minutes
if [ "$minutes" = 'test' ]; then
    if [ "$player" = "" ]; then
	BeepIt
    else
	echo $player \"$file\" > 31415.nap
	chmod +x 31415.nap
	./31415.nap
    fi
    exit
fi

if [ $# -lt 1 ]; then
    echo "Usage: ./nap.sh minutes player"
else
    let seconds=60*$minutes
    declare -i seconds
    echo "Nap for " $minutes " minutes ("$seconds " seconds)"
    echo "Init: time is" $(date +%H%M)
    Result="0"
    while [ "$Result" = "0" ]; do
	Data=$(read -s -t 1 -n 1)
	Result=$?
    #echo $Result "asdf"
    done
    echo "Nap: current time is" $(date +%H%M)
    echo "Sleeping for " $seconds " seconds"
    sleep $seconds
    echo "Done: current time is" $(date +%H%M)
    if [ "$player" = "" ]; then
	BeepIt
    else
	echo $player \"$file\" > 31415.nap
	chmod +x 31415.nap
	./31415.nap
    fi
fi
