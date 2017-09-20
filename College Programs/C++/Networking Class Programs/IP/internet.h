// internet.h
#include <ctype.h>
#include "protocol.h"
 
#ifndef INTERNET_H
#define INTERNET_H

#define IP_ADDRESS_LEN 4
struct ipfragment {
	int id;
	int sum;
	int maxvalue;
	unsigned char packet[1000];
	bool inuse;
};

class internet: public protocol {
private:
	char myipaddr[4];
	ipfragment frag1, frag2, frag3;
public:
  int init (char *name, protocol *higher, protocol *lower, 
	    char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr, message *mb);
  int pop (unsigned char *buf, int len, message *mb);
};

#endif
