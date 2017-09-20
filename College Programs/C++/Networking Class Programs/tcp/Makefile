# CS 460
# Lab 4

# macro definitions
CC=g++ -g
C_FLAGS=
LINK_FLAGS= -lnsl -lpthread
# for visual C++ -ML
# -g +eh
EXE_NAME=tcp

# program executable *********
$(EXE_NAME): main.o protocol.o message.o tcp.o rpc.o arp.o ethernet.o internet.o physical.o
	$(CC) -v -o $(EXE_NAME) main.o tcp.o rpc.o arp.o protocol.o message.o ethernet.o internet.o physical.o $(LINK_FLAGS)
		
# .o files *****************

main.o:	main.cpp protocol.h message.h
	$(CC) $(C_FLAGS) -c main.cpp	

client:	
	$(CC) $(C_FLAGS) -DTESTING -c main.cpp	
	$(CC) -v -o server main.o tcp.o rpc.o arp.o protocol.o message.o ethernet.o internet.o physical.o $(LINK_FLAGS)
	rm main.o

protocol.o: protocol.cpp protocol.h message.h
	$(CC) $(C_FLAGS) -c protocol.cpp	

physical.o: physical.cpp physical.h message.h
	$(CC) $(C_FLAGS) -c physical.cpp	

ethernet.o: ethernet.cpp ethernet.h message.h
	$(CC) $(C_FLAGS) -c ethernet.cpp	

arp.o: arp.cpp arp.h message.h
	$(CC) $(C_FLAGS) -c arp.cpp	

internet.o: internet.cpp internet.h message.h
	$(CC) $(C_FLAGS) -c internet.cpp	

tcp.o: tcp.cpp tcp.h message.h
	$(CC) $(C_FLAGS) -c tcp.cpp	

rpc.o: rpc.cpp rpc.h message.h
	$(CC) $(C_FLAGS) -c rpc.cpp

message.o: message.cpp message.h
	$(CC) $(C_FLAGS) -c message.cpp

clean:
	rm *.o



