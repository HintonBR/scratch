// This is just a template for you to use in your coding

#include "internet.h"
#include "arp.h"
#include "nat.h"
#include <netinet/in.h>
static int id = 1;


u_short calculateCheckSum(u_short* buf, int count) {
	register u_long sum = 0;

	while (count--) {
		sum += *buf++;
		if (sum & 0xffff0000) {
			//carry occured so wrap around
			sum &= 0xffff;
			sum++;
		}
	}
	return ~(sum & 0xffff);
}

int  internet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  cout << "This is the internet init routine for " << name << endl;
  print_addr("Internet Address", (unsigned char *)addr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;
  memcpy(myipaddr, addr, 4);
  frag1.id = 0;
  frag2.id = 0;
  frag3.id = 0;
  frag1.sum = 0;
  frag2.sum = 0;
  frag3.sum = 0;
  frag1.maxvalue = -1;
  frag2.maxvalue = -1;
  frag3.maxvalue = -1;
  frag1.inuse = false;
  frag2.inuse = false;
  frag3.inuse = false;
  memset(frag1.packet, 0, 1000);
  memset(frag2.packet, 0, 1000);
  memset(frag3.packet, 0, 1000);
}

// Push the data to the next lower layer
int internet::push (unsigned char *buf, int len, char *addr, message *mb)
{
  //unsigned char* ippacket = new unsigned char[len + 20];
  cout << "This is the internet push routine" << endl;
  print_chars(prot_name, buf, len);
  u_short temp;
  
  if (len > 30) {
	int templen = 0;
	while (templen < len) {
		int plen;
		int flag = 0;
		if ((len - templen) > 30) {
			plen = 24;
			flag = 1;
		}
		else {
			plen = len - templen;
		}
		//Build packet
		unsigned char* ippacket = new unsigned char[plen + 20];
		
		memset(ippacket, 0, plen+20);
		ippacket[0] = 0x45;
		temp = htons(plen+20);
		memcpy(ippacket+2, &(temp), 2);
		temp = htons(id);
		memcpy(ippacket+4, &(temp), 2);
		if (flag == 1) {
			temp = htons((1 << 13) | templen);
			memcpy(ippacket+6, &(temp),2);
		}
		else {
			temp = htons(templen);
			memcpy(ippacket+6, &(temp),2);
		}
		ippacket[8] = 0x01;
		ippacket[9] = 0x06;
		memcpy(ippacket+12, myipaddr, 4);
		memcpy(ippacket+16, addr, 4);
		memcpy(ippacket+20, buf + templen, plen);
		temp = htons(calculateCheckSum((u_short*)ippacket, 10));
		memcpy(ippacket+10, &(temp), 2);
		if(prot_lower)
			prot_lower->push (ippacket, plen+20, addr, mb);	
		templen += plen;
	}
  }
  else { //no need to fragment just send
	//Build packet
    unsigned char* ippacket = new unsigned char[len + 20];
	memset(ippacket, 0, len+20);
	ippacket[0] = 0x45;
	temp = htons(len+20);
	memcpy(ippacket+2, &(temp), 2);
	temp = htons(id);
	memcpy(ippacket+4, &(temp), 2);
	ippacket[8] = 0x01;
	ippacket[9] = 0x06;
	memcpy(ippacket+12, myipaddr, 4);
	memcpy(ippacket+16, addr, 4);
	memcpy(ippacket+20, buf, len);
	temp = htons(calculateCheckSum((u_short*)ippacket, 10));
	memcpy(ippacket+10, &(temp), 2);
	if(prot_lower)
		prot_lower->push (ippacket, len+20, addr, mb);
	id++;
  }
}

// Pop the data from the next lower layer
int internet::pop (unsigned char *buf, int len, message *mb)
{
  
  print_chars("Popping in the internet Layer",buf,len);

  ipfragment* curFrag;
  u_short usID, usLen, usOffset, csum;
  int id, plen, offset;
  unsigned char destIP[4];
  memcpy(destIP, buf+16, 4);
  memcpy(&csum, buf+10, 2);
  csum = ntohs(csum);
  memset(buf+10, 0, 2);
  memcpy(&usLen, buf+2, 2);
  plen = (int)ntohs(usLen);
  if (memcmp(destIP, myipaddr,4) == 0) {
	  if (csum == calculateCheckSum((u_short*)buf,10)) {
		memcpy(&usOffset, buf+6, 2);
		offset = ntohs(usOffset);
		if (offset == 0) {
			if(prot_higher)
				prot_higher->pop (buf+20, plen-20, mb);
		}
		else {
			memcpy(&usID, buf+4, 2);
			id = (int) ntohs(usID);
			if (id == frag1.id) {
				curFrag = &frag1;
			}
			else if (id == frag2.id) {
				curFrag = &frag2;
			}
			else if (id == frag3.id) {
				curFrag = &frag3;
			}
			else if (frag1.inuse == false) {
				curFrag = &frag1;
			}
			else if (frag2.inuse == false) {
				curFrag = &frag2;
			}
			else {
				curFrag = &frag3;
			}
			curFrag->id = id;
			curFrag->inuse = true;
			cout << "Using fragment with id=" << id << endl;
			if ((offset&0x2000) == 0) {
				cout << "The M bit was 0" << endl;
				curFrag->maxvalue = (offset & 0x1fff) + plen-20;
				cout << "The max value was set to " << curFrag->maxvalue << endl;
				
			}
			else {
				cout << "The M bit was 1" << endl;
			}
			curFrag->sum += (plen-20);
			cout << "Current sum of packet is " << curFrag->sum << endl;
			cout << "Looking for a max value of " << curFrag->maxvalue << endl;
			memcpy(curFrag->packet+(offset&0x1fff), buf+20, plen-20);
			if(prot_higher && curFrag->sum == curFrag->maxvalue) {
				unsigned char* newpack = new unsigned char[curFrag->sum];
				memcpy(newpack, curFrag->packet, curFrag->sum);
				cout << "Passing up a fragmented packet" << endl;
				print_chars("Packet", newpack, curFrag->sum);
				prot_higher->pop (newpack, curFrag->sum, mb);
				curFrag->inuse = false;
				curFrag->maxvalue = -1;
				curFrag->sum = 0;
				memset(curFrag->packet, 0, 1000);
			}
		}
	  }
	  else {
		cout << "Checksum was wrong" << endl;
		print_chars("Calculating Checksum on this buffer",buf,len);
		u_short tempsum = calculateCheckSum((u_short*)buf,10);
		print_chars("Checksum calculated", (unsigned char*) &tempsum, 2);
		print_chars("Checksum found", (unsigned char*) &csum, 2);
	  }
  }
  else {
	  cout << "Packet was not for me" << endl;
	  print_addr("Target IP was", destIP, 4);
  }
}
