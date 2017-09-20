// arp.h
#include <ctype.h>
#include "protocol.h"
#include <string.h>
#include <pthread.h>

#ifndef ARP_H
#define ARP_H

class arp: public protocol {
private:

public:
  unsigned char ipaddr[4];
  unsigned char ethaddr[6];
  unsigned char arpiptable[100][4];
  unsigned char arpethertable[100][6];
  int arpsize;
  int init (char *name, protocol *higher, protocol *lower,
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr, message *mb);
  int pop (unsigned char *buf, int len, message *mb);
  int checkarptable(char* addr);	
};

#endif

