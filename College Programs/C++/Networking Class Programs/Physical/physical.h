// Physical.h
#ifndef PHYSICAL_H
#define PHYSICAL_H

#include "protocol.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>

#define BUFFERSIZE 100
#define PACKETSIZE 90
#define SOCKET_ERROR -1
#define QUEUE_SIZE 10
#define HOST_NAME_SIZE      255

 
class physical: public protocol {
private:
  int hostport;
  char *hostname;
  int hSocket;
  char strHostName[HOST_NAME_SIZE];
  unsigned char inbuffer[BUFFERSIZE], outbuffer[BUFFERSIZE];
  

public:

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr, message *mb);
	static void* readloop (void* arg);
  void createServerSocket();
    void createClientSocket();
};

#endif
