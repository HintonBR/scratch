// Physical.h
#ifndef PHYSICAL_H
#define PHYSICAL_H

#include "protocol.h"
#include "StdAfx.h"

#define BUFFERSIZE 100
#define PACKETSIZE 90
#define QUEUE_SIZE 10
#define HOST_NAME_SIZE      255

 
class physical: public protocol {
private:
  

public:

	int hostport;
  char *hostname;
  SOCKET hSocket;
  char strHostName[HOST_NAME_SIZE];
  char inbuffer[BUFFERSIZE], outbuffer[BUFFERSIZE];

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr, message *mb);
	
  void createServerSocket();
    void createClientSocket();
};

void* readloop (void* arg);
#endif
