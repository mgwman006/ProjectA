
.RECIPEPREFIX +=
CC = g++
CFLAGS = -Wall -lm -lrt -lwiringPi -lpthread

PROG = bin/*
OBJS = obj/*

default:
    mkdir -p bin obj
    $(CC) $(CFLAGS) -c src/Main.cpp -o obj/Main
    $(CC) $(CFLAGS) obj/Main -o bin/Main
    
    $(CC) $(CFLAGS) -c src/Alarm.cpp -o obj/Alarm
    $(CC) $(CFLAGS) obj/Alarm -o bin/Alarm


run:
    sudo ./bin/Main

clean:
    rm $(PROG) $(OBJS)