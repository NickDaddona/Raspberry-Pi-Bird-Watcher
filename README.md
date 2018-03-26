# Raspberry-Pi-Bird-Watcher
A smart birdwatcher using the raspberry pi written in C.

Developed by Nicholas Daddona and Tobias Rittgers as the class project for CS355-Systems Programming.

Compile Instructions 2018-03-25
This requires the grovepi c++ library to be installed and forces the C files to be
compiled for C and linked with the c++ wrapper functions via g++. As such, the 
option -x c will have to be used for each source file to ensure its compiled as C.