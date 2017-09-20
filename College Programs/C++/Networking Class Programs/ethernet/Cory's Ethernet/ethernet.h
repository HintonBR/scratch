// ethernet.h
#include <ctype.h>
#include "protocol.h"
#include "message.h"
 
#ifndef ETHERNET_H
#define ETHERNET_H

#define ETH_ADDRESS_LEN 6

#define BASIC_FRAME_SIZE 	18   //bytes in a frame w/o message or sentinels
typedef unsigned char BYTE;


class ethernet: public protocol {
private:
	unsigned char address[6];
	unsigned char origBuffer[100];
	unsigned char newBuffer[100];
	//The following are parts of the ethernet frame

	//vars for stuffing

	//FUNCTIONS
	
public:
	int init (char *name, protocol *higher, protocol *lower, char *addr, int port, int client);
  	int push (unsigned char *buf, int len, char *addr, message *mb);
  	int pop (unsigned char *buf, int len, message *mb);
	int bitStuff(unsigned char* newMessage, int len, unsigned char* oldMessage);
	int getBit(unsigned char* message, int offset);
	void setBit(unsigned char* message, int offset);
	int unBitStuff(unsigned char* message, int offset, unsigned char *newMessage);
};

#endif
