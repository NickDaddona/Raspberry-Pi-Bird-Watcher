# Raspberry-Pi-Bird-Watcher
A smart birdwatcher using the raspberry pi written in C.

Developed by Nicholas Daddona as the class project for CS355-Systems Programming.

The birdwatcher currently uses thingspeak to record temperature and humidity and
Imgur to store any pictures taken. An api key is required to upload any information
to thingspeak. Imgur requires an Oauth key to upload any pictures to someone's account.
These keys expire, but the one in use should be good for 10 years, at least according
to the result from a key request. I'm unsure if the lengths of these keys vary.
If they do, it would require reworking strings used in libcurl functions.