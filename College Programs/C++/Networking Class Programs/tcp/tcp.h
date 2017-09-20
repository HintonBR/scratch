// tcp.h
#include <ctype.h>
//#include <synch.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "internet.h"
#include <netinet/in.h>




#ifndef TCP_H
#define TCP_H

#define TCP_ADDRESS_LEN 2 // Holds the port number
#define IP_ADDRESS_LEN 4
class tcp: public protocol {
private:

public:
  unsigned char my_ipaddr[IP_ADDRESS_LEN];
  unsigned char my_tcpaddr[TCP_ADDRESS_LEN];
  int init (char *name, protocol *higher, protocol *lower, 
	    char *addr, int wsize, int client);
  int push (unsigned char *buf, int len, char *addr, message *mb);
  int pop (unsigned char *buf, int len, message *mb);
  void sendwin(unsigned char* buf, int len, unsigned char* ipdest);
  void slidewin();
  int windowsize;
};

#endif
