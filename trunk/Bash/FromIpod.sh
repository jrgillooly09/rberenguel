#!/bin/sh

# Copyright 2009 Rubén Berenguel

# ruben /at/ maia /dot/ ub /dot/ es

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# FromIpod: Will copy FILENAME from your iPod's MacOsClassic directory
# into DSKDESTINATIONDIRECTORY, mount it and copy all files into
# DATADESTINATIONDIRECTORY, setting all possible permissions (I had
# problems with permissions before). You can download the latest
# version (and read the #GPL license) version from
# http://rberenguel.googlecode.com/svn/trunk/Bash/FromIpod.sh

FILENAME=TeXFiles.dsk
DSKDESTINATIONDIRECTORY=~/Documents/
MOUNTPOINT=/mnt/hfs-disk/
DATADESTINATIONDIRECTORY=~/Documents/iPod-Data

scp mobile@$1:/var/mobile/Library/MacOSClassic/$FILENAME $DSKDESTINATIONDIRECTORY
sudo mount -oloop -t hfs $DSKDESTINATIONDIRECTORY$FILENAME $MOUNTPOINT
sudo cp $MOUNTPOINT*.*  $DATADESTINATIONDIRECTORY
sudo chmod u+rxw  $DATADESTINATIONDIRECTORY/*.* -v 
sudo umount $MOUNTPOINT