# File:   Makefile
# Author: robertomaldonado
# Created on November 30, 2017

# Declare variables
CC=g++ -std=c++11 -O2

#Declare processes
all: traffic aloha slotted_aloha csma

traffic: ALOHA/traffic_generator.o CSMA/traffic_generator.o
	$(CC) ALOHA/traffic_generator.cpp -o ALOHA/traffic_generator
	$(CC) CSMA/traffic_generator.cpp -o CSMA/traffic_generator

aloha: ALOHA/aloha.o
	$(CC) ALOHA/aloha.cpp -o ALOHA/aloha

slotted_aloha: ALOHA/slotted_aloha.o
	$(CC) ALOHA/slotted_aloha.cpp -o ALOHA/slotted_aloha

csma: CSMA/persist.o
	$(CC) CSMA/persist.cpp -o CSMA/persist

clean: 
	rm -rf ALOHA/*.o ALOHA/traffic_generator CSMA/traffic_generator CSMA/*.o
	rm -rf ALOHA/aloha ALOHA/slotted_aloha CSMA/persist
	rm -rf ALOHA/*.txt CSMA/*.txt ALOHA/*.dat CSMA/*.dat 