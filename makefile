install: main.o sensors.o thingspeak.o util.o grovewrap.o # link object files
	g++ -o bird-watcher ./out/main.o ./out/sensors.o ./out/thingspeak.o ./out/util.o ./out/grovewrap.o -lgrovepicpp -lcurl -lrt

main.o: sensors.o ./src/main.c #build main.c
	g++ -o ./out/main.o -c -x c ./src/main.c -Wall

sensors.o: ./src/thingspeak.c ./src/util.c ./src/sensors.c ./includes/sensors.h # build sensors.c
	g++ -o ./out/sensors.o -c -x c ./src/sensors.c -Wall

thingspeak.o: ./src/thingspeak.c ./src/util.c ./includes/thingspeak.h #build thingspeak.c
	g++ -o ./out/thingspeak.o -c -x c ./src/thingspeak.c -Wall -lcurl

util.o: ./src/util.c ./includes/util.h # build util.c
	g++ -o ./out/util.o -c -x c ./src/util.c -Wall

grovewrap.o: ./lib/grovewrap.cpp ./lib/grovewrap.h # build wrapper library
	g++ -o ./out/grovewrap.o -c ./lib/grovewrap.cpp -Wall -lgrovepicpp

clean: # remove object files
	rm ./out/main.o ./out/sensors.o ./out/thingspeak.o ./out/util.o ./out/grovewrap.o