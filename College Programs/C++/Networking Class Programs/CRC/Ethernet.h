// ethernet.h
#include <ctype.h>
#include "protocol.h"
#include "message.h"
 
#ifndef ETHERNET_H
#define ETHERNET_H

#define ETH_ADDRESS_LEN 6

#define BASIC_FRAME_SIZE 	18   //bytes in a frame w/o message or sentinels
#define SENTINEL 0x7E

typedef unsigned char BYTE;



class ethernet: public protocol {
private:
	//The following are parts of the ethernet frame
	char* src;
	//vars for stuffing

	//FUNCTIONS
	BYTE* bitstuff(unsigned char *packet, int pSize, int* newPacketSize);
	BYTE getbit(BYTE curByte, int offset);
	BYTE setbit(int offset);
	BYTE unsetbit(int offset);
	BYTE* unbitstuff(unsigned char *packet, int pSize, int* newPacketSize);
	unsigned long computeCRC(BYTE* m, int length);
public:
	
	int init (char *name, protocol *higher, protocol *lower, char *addr, int port, int client);
  	int push (unsigned char *buf, int len, char *addr, message *mb);
  	int pop (unsigned char *buf, int len, message *mb);
};

#endif
