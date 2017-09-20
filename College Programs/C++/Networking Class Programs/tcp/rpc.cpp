
// This is just a template for you to use in your coding

#include "rpc.h"

// This is the maximum packet size that will be sent.  It is also declared in main.cc
#define MAXLEN 40
#define CHECKSIZE 500

extern int gerrors;

int  rpc::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{

  errors = 0;
  packet_count = 0;
  remaining = CHECKSIZE;
  cout << "This is the rpc init routine for " << name << endl;
  memcpy(my_tcpaddr,addr,TCP_ADDRESS_LEN);
  memcpy(my_ipaddr,addr+2,IP_ADDRESS_LEN);
  print_addr("rpc port Address", my_tcpaddr, TCP_ADDRESS_LEN);
  print_addr("rpc IP Address", my_ipaddr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  if(client == 1)
    amclient = 1;
  else	
    amclient = 0;
  prot_higher = higher;
  prot_lower = lower;
}

// Push the data to the next lower layer
int rpc::push (unsigned char *buf, int len, char *addr, message *mb)
{
  cout << "This is the rpc push routine" << endl;
  print_chars(prot_name, buf, len);


  if(prot_lower)
    prot_lower->push (buf, len, addr, mb);
}

extern int crcerrors;

// Pop the data from the next lower layer
int rpc::pop (unsigned char *buf, int len, message *mb)
{
  int i;
  int curpos;

  cout << "This is the rpc pop routine" << endl;
  print_chars(prot_name, buf, len);

  if(packet_count == 0) {
      if(len != 4) {
	  cout << "--------------------------------------------------------------------------------------------------------Error, incorrect length expected 4" << endl;
	  errors++;
      }
      if(amclient) {
	packet_count++;
	if( (buf[0] == 0xfe) &&
	    (buf[1] == 0xed) &&
	  (buf[2] == 0xbe) &&
	  (buf[3] == 0xef) ) {
	   cout << "Received packet  " << packet_count << "!!!!!!!!!!!!" << endl;
	   return(0);
	} else {
	  cout << "-----------------------------------------------------------------------------------------------------------------------Error, incorrect data received, expected 0xfeedbeef" << endl;
	  errors++;
	}
      } else {
	packet_count++;
	if( (buf[0] == 0xde) &&
	    (buf[1] == 0xad) &&
	  (buf[2] == 0xbe) &&
	  (buf[3] == 0xef) ) {
	   cout << "Received packet  " << packet_count << "!!!!!!!!!!!!" << endl;
	   return(0);
	} else {
	  cout << "-----------------------------------------------------------------------------------------------------------------------Error, incorrect data received, expected 0xdeadbeef" << endl;
	  errors++;
	}
      }
      remaining = CHECKSIZE;
      return(0);
    }
    /* Now look for 500 bytes of good data.  It may come in several messages */
    if(len <= remaining) {
      curpos = CHECKSIZE - remaining;
      remaining -= len;
      for(i = 0; i < len; i++) {
	if(buf[i] != (unsigned char)((i+curpos) & 0xff)) {
	  printf("Error, incorrect data received got %x, expected %x\n", buf[i], (i + curpos)& 0xff);
	  errors++;
	}
      }
      packet_count++;
      cout << "Received message remaining= " << remaining << " !!!!!!!!!!!!" << endl;
    } else {
      printf("ERROR, incorrect message length received\n");
      errors++;
    }
    if(remaining == 0) {
      if(errors || gerrors)
	cout << "You didnt pass off this part of the TCP lab you had " <<
	  errors << " errors" << endl;
      else
	cout << "You passed off this part of the TCP lab  !!!!!!!!!!!!" << endl;
    }
}









