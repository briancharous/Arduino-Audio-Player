Arduino Audio Player
=====================

Overview
-------------
This is a simple Arduino program that is used to read a .wav file off an SD card and set the Arduino's pins to correspond to the bytes in the wav file. The ultimate goal is to play an audio file through a DAC (specifically, an R2R ladder). This program is written for the Mega 2560, but can be modified for an Uno (albeit with less bits).  

Limitations
------------
Given the limited resources of the Arduino, this program can play 8-bit mono audio at 22kHz.