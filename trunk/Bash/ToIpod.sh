#!/bin/sh
FILENAME=TeXFiles.dsk
DSKSOURCE=~/Documents/
MOUNTPOINT=/mnt/hfs-disk/
DATASOURCEDIRECTORY=~/Documents/iPod-Data

sudo mount -oloop -t hfs $DSKSOURCE$FILENAME  $MOUNTPOINT
sudo cp $DATASOURCEDIRECTORY/*.* $MOUNTPOINT
sudo chmod a+rxw  $MOUNTPOINT*.* 
sudo umount $MOUNTPOINT
scp $DSKSOURCE$FILENAME mobile@$1:/var/mobile/Library/MacOSClassic/$FILENAME