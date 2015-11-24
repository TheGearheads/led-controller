#!/bin/bash
socat -,raw,echo=0 /dev/ttyUSB0,raw,echo=0,b9600
