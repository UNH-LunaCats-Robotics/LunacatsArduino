#!/bin/bash

sftp pi@10.0.10.10 <<EOF
cd Documents/arduino/LunacatsArduino
put LunacatsArduino.ino .
exit

EOF