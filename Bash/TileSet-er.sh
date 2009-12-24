#/bin/bash

# Uses the triangle approximator file output to generate a tile set.
# It assumes the source image should be also there (as Something.jpg,
# with Something higher in lexicographical order than
# Generation%7d.jpg

FILES=24
COLUMNS=25
NUM=0
for i in `seq 1 $FILES`;
do
    let NUM=$i*$COLUMNS
    ls *.jpg | head -n $NUM | tail -n $COLUMNS > F$i
done    

for i in `seq 1 $FILES`;
do
    convert @F$i +append F$i.jpg
    echo F$i.jpg >> FINAL
done
convert @FINAL -append Mosaic.jpg
rm FINAL
for i in `seq 1 $FILES`;
do
    rm F$i.jpg F$i
done