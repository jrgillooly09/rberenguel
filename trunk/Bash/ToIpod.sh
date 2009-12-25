#!/bin/sh

# Copyright 2009 Ruben Berenguel

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

# ToIpod: will mount DSKSOURCE/FILENAME in MOUNTPOINT, and copy all
# files in DATASOURCEDIRECTORY to the mounted filesystem. It will set
# all permissions, Then will unmount #and use scp to copy FILENAME to
# your iPod/iPhone. You can download the latest version (and read the
# GPL #license) from

FILENAME=TeXFiles.dsk
DSKSOURCE=~/Documents/
MOUNTPOINT=/mnt/hfs-disk/
DATASOURCEDIRECTORY=~/Documents/iPod-Data

sudo mount -oloop -t hfs $DSKSOURCE$FILENAME  $MOUNTPOINT
sudo cp $DATASOURCEDIRECTORY/*.* $MOUNTPOINT
sudo chmod a+rxw  $MOUNTPOINT*.* 
sudo umount $MOUNTPOINT
scp $DSKSOURCE$FILENAME mobile@$1:/var/mobile/Library/MacOSClassic/$FILENAME