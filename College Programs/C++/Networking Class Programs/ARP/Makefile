# CS 460
# Lab 4

# macro definitions
CC=g++ -g
C_FLAGS=
LINK_FLAGS=-lnsl -lpthread
# for visual C++ -ML
# -g +eh
EXE_NAME=arp

# program executable *********
arp: main.o protocol.o message.o arp.o internet.o ethernet.o physical.o
	$(CC) -v -o $(EXE_NAME) main.o protocol.o message.o arp.o internet.o ethernet.o physical.o $(LINK_FLAGS)

client: main.o protocol.o message.o ethernet.o ip.o physical.o
	$(CC) -v -o client main.o protocol.o message.o ethernet.o ip.o physical.o $(LINK_FLAGS)

# .o files *****************

main.o:	main.cpp protocol.h message.h
	$(CC) $(C_FlAGS) -c main.cpp

protocol.o: protocol.cpp protocol.h message.h
	$(CC) $(C_FLAGS) -c protocol.cpp

physical.o: physical.cpp physical.h message.h
	$(CC) $(C_FLAGS) -c physical.cpp

ethernet.o: ethernet.cpp ethernet.h message.h
	$(CC) $(C_FLAGS) -c ethernet.cpp

internet.o: internet.cpp internet.h message.h
	$(CC) $(C_FLAGS) -c internet.cpp

ip.o: ip.cpp internet.h message.h
	$(CC) $(C_FLAGS) -c ip.cpp

message.o: message.cpp message.h
	$(CC) $(C_FLAGS) -c message.cpp
clean:
	rm *.o

