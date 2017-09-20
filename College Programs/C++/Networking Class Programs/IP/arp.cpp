
// This is just a template for you to use in your coding

#include "arp.h"
extern int blabby;

int  arp::init (char *name, protocol *higher, protocol *lower,
char *addr, int port, int client)
{
  cout << "This is the init routine for " << name << endl;
  strcpy(prot_name , name);
  memcpy(ipaddr,addr,4);
  memcpy(ethaddr,addr+4,6);
  prot_higher = higher;
  prot_lower = lower;
  arpsize = 0;
}
int arp::checkarptable(char* addr) {
	int foundip = -1;
	//Look up ip address in arpiptable
  for (int i=0; i < arpsize; i++) {
	  if (memcmp(arpiptable[i], addr, 4) == 0) {
		  foundip = i;
		  break;
	  }
  }
  return foundip;
}
int arp::push (unsigned char *buf, int len, char *addr, message *mb)
{
   unsigned char arppacket[28];
  if(blabby & 0x8) {
    cout << "This is the ARP push routine" << endl;
    print_chars(prot_name, buf, len & 0xffff);
  }
  int foundip = checkarptable(addr);	
  
  if (foundip == -1) {
	// Send an ARP packet out because we didn't find the address in the ARP table
	#define BCAST_ETHADDR {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
	char bcast_ethaddr[6]=BCAST_ETHADDR;

	//Create Packet
		arppacket[0] = 0x00;
		arppacket[1] = 0x01;
		arppacket[2] = 0x08;
		arppacket[3] = 0x00;
		arppacket[4] =  0x06;
		arppacket[5] = 0x04;
		arppacket[6] = 0x00;
		arppacket[7] =  0x01;
		memcpy(arppacket+8, ethaddr, 6);
		memcpy(arppacket+14, ipaddr, 4);
		//memcpy(arppacket+18, bcast_ethaddr, 6);
		memcpy(arppacket+24,addr, 4);
		//struct timespec delay;
		//delay.tv_sec = 10;
		//delay.tv_nsec = 0;
		//pthread_delay_np( &delay );
	
	if(prot_lower)
		prot_lower->push (arppacket, 28 | (0x806 << 16), bcast_ethaddr, mb);

	cout << "Going to sleep" << endl;
	sleep(4);
	cout << "Checking for address" << endl;
		foundip = checkarptable(addr);
		
	
	if (prot_lower && foundip != -1) {
		prot_lower->push (buf, len | (0x800 << 16), (char*)arpethertable[foundip], mb);
	}
	else {
		cout << "Throwing away packet" << endl;
	}
  }
  else {
	if(prot_lower)
		prot_lower->push (buf, len | (0x800 << 16), (char*) arpethertable[foundip], mb);
  }
}
// Pop the data from the next lower layer
int arp::pop (unsigned char *buf, int len, message *mb)
{
  int dlen = len & 0xffff; // The data length
  int tlen = len>>16;      // The type
  unsigned char taddr[4];
  unsigned char setheraddr[6];
  unsigned char sipaddr[4];
  int operation;
  unsigned char	arppacket[28];
  if(blabby & 0x8) {
    cout << "This is the arp pop routine" << endl;
    print_chars(prot_name, buf, dlen);
    printf("len %x tlen %x and dlen %x\n",len,tlen, dlen);
  }

  if (tlen == 0x800) {
   if(prot_higher)
      prot_higher->pop (buf, dlen, mb);
  }
  else { //this is an ARP packet
	  
	  memset(taddr, 0, 4);
	  memset(setheraddr, 0, 6);
	  memset(sipaddr, 0, 4);
	  
	  
	  memcpy(taddr, buf + 24, 4);
	  operation = 0;
	  operation = buf[7];
	  operation |= buf[8] << 8;
	  memcpy(setheraddr, buf + 8, 6);
	  memcpy(sipaddr, buf + 14, 4);

	  printf("Comparing the ip address to my ip address - the operation is %d", operation);
	  cout << endl;
	  print_addr("My IP address", ipaddr, 4);
	  cout << endl;
	  print_addr("The target protocol address", taddr, 4);
	  cout << endl;
	  if (memcmp(taddr,ipaddr,4) == 0 && operation == 1) { //we were the target

		printf("Preparing ARP response");
		cout << endl;
		memset(arppacket, 0, 28);
		

		printf("Inserting into table");
		cout << endl;
		print_addr("IP address", sipaddr, 4);
		cout << endl;
		print_addr("Ethernet Address", setheraddr, 6);
		cout << endl;
		memcpy(arpiptable[arpsize], sipaddr, 4);
		memcpy(arpethertable[arpsize], setheraddr, 6);
		arpsize++;

		//Create Packet
		arppacket[0] = 0x00;
		arppacket[1] = 0x01;
		arppacket[2] = 0x08;
		arppacket[3] = 0x00;
		arppacket[4] =  0x06;
		arppacket[5] = 0x04;
		arppacket[6] = 0x00;
		arppacket[7] =  0x02;
		memcpy(arppacket+8, ethaddr, 6);
		memcpy(arppacket+14, ipaddr, 4);
		memcpy(arppacket+18, setheraddr, 6);
		memcpy(arppacket+24, sipaddr, 4);

		if(prot_lower)
			printf("Sending Packet");
			prot_lower->push(arppacket, 28 | (0x806 << 16), (char*) setheraddr,mb);
	  }
	  else if (memcmp(taddr,ipaddr,4) == 0 && operation == 2) { //we were the target
		printf("Operation was a two so we are Inserting into table");
		cout << endl;
		print_addr("IP address", sipaddr, 4);
		cout << endl;
		print_addr("Ethernet Address", setheraddr, 6);
		cout << endl;
		memcpy(arpiptable[arpsize], sipaddr, 4);
		memcpy(arpethertable[arpsize], setheraddr, 6);
		arpsize++;
	  }
	  else {
		cout << "We were not the target of this ARP packet" << endl;
	  }
  }
}

