
// This is just a template for you to use in your coding

#include "ethernet.h"
#include <stdio.h>

#define CRC_POLY   0x04c11db7
#define OFFSET     14        // 6 (destAddr) + 6 (srcAddr) + 2 (type) = 14
#define BITMASK	   128
#define ARPTYPE	   0x0806
#define IPTYPE	   0x0800

extern int blabby;

int  ethernet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  cout << "This is the ethernet init routine for " << name << endl;
  print_addr("Ethernet Address", addr, ETH_ADDRESS_LEN);
  src = new char[ETH_ADDRESS_LEN];
  memcpy(src, addr, 6);
  print_addr("Ethernet Address", src, ETH_ADDRESS_LEN);
  strcpy(prot_name, name);
  prot_higher = higher;
  prot_lower = lower;
  return 0;
}
BYTE ethernet::getbit(BYTE curByte, int offset) {
	BYTE msg = 0x80 >> (offset % 8);
	return msg & curByte;
}
BYTE ethernet::setbit(int offset) {
	BYTE msg = 0x80 >> (offset % 8);
	return msg;
}
BYTE ethernet::unsetbit(int offset) {
	BYTE msg = 0x7F >> (offset % 8);
	msg = msg | (0xff << (8- (offset %8)));
	return msg;
}
// Push the data to the next lower layer
int ethernet::push (unsigned char *buf, int len, char *addr, message *mb)
{
  BYTE sentinel[1];
  BYTE* packet = new BYTE[18 + len];
  BYTE* stuffedPacket = new BYTE[100];
  BYTE* strTempPacket;
  int stuffedPacketSize = 0;
  memset(stuffedPacket, 0, 100);
	memset(packet, 0, 18 + len);
  if(blabby & 0x2)
    {
      cout << "This is the ethernet push routine len = " << len << endl;
      print_addr("Ethernet Address", addr, ETH_ADDRESS_LEN);
      print_chars(prot_name, buf, len);
    }
	
  //Insert the data into the packet
	memcpy(packet + 14, buf, len);
	
  //Insert the front header
	memcpy(packet, addr, ETH_ADDRESS_LEN);
	memcpy(packet + 6, src, ETH_ADDRESS_LEN);
  //Insert the back header
		//CRC is already set to zero
	//Bit Stuff
	print_chars(prot_name, packet, 18+len);
	strTempPacket = bitstuff(packet, len + 18, &stuffedPacketSize);
	memcpy(stuffedPacket + 1, strTempPacket, stuffedPacketSize/8);
  //Add Sentinels
  memset(sentinel, 0, 1);
  sentinel[0] = SENTINEL;
	memcpy(stuffedPacket, sentinel, 1);
	stuffedPacketSize += 8;
	//Set end Sentinel
	for (int i=0; i < 8; i++) {
		if (i == 0 || i == 7) {
			stuffedPacketSize++;
		}
		else {
			stuffedPacket[stuffedPacketSize/8] = stuffedPacket[stuffedPacketSize/8] | setbit(stuffedPacketSize);		
			stuffedPacketSize++;
		}
	}

print_chars(prot_name, stuffedPacket, stuffedPacketSize/8 + 1);
 if(prot_lower)
    prot_lower->push(stuffedPacket, 90, 0, NULL);
 return 0;
}
	

BYTE* ethernet::bitstuff(unsigned char *packet, int pSize, int* newPacketSize) {
	BYTE* newPacket = new BYTE[100];
	memset(newPacket, 0, 100);
	int newPacketLocation = 0;
	int state = 0;
	//Loop through each bit
	for (int i=0; i < (pSize *8);i++) {
		if (getbit(packet[(i/8)], i) > 0) {
			if (state == 5) {
 				newPacketLocation++;
				newPacket[newPacketLocation/8] = newPacket[newPacketLocation/8] | setbit(newPacketLocation);
				newPacketLocation++;
				state = 1;
			}
			else {
			  newPacket[newPacketLocation/8] = newPacket[newPacketLocation/8] | setbit(newPacketLocation);
				newPacketLocation++;
				state++;
			}
		}
		else {
			if (state == 5) {
				newPacketLocation += 2;
				state = 0;
			}
			else {
				newPacketLocation++;
				state = 0;
			}
		}
	}		
	(*newPacketSize) = newPacketLocation;
	return newPacket;
}

BYTE* ethernet::unbitstuff(unsigned char *packet, int pSize, int* newPacketSize) {
	BYTE* newPacket = new BYTE[100];
	memset(newPacket, 0, 100);
	int newPacketLocation = 0;
	int state = 0;
	//Loop through each bit
	for (int i=0; i < pSize;i++) {
		if (getbit(packet[(i/8)], i) > 0) {
			  newPacket[newPacketLocation/8] = newPacket[newPacketLocation/8] | setbit(newPacketLocation);
			  newPacketLocation++;
			  state++;
		}
		else {
			if (state == 5) {
				state = 0;
			}
			else {
				newPacketLocation++;
				state = 0;
			}
		}
	}		
	(*newPacketSize) = newPacketLocation;
	return newPacket;
}

// Pop the data from the next lower layer
int ethernet::pop (unsigned char *buf, int len, message *mb)
{
   if(blabby &0x2)
     {
       cout << "This is the ethernet pop routine" << endl;
       print_chars(prot_name, buf, len);
     }

   int preamble = 0;
   int postamble = 0;
   int bit = 0;
   BYTE crc[4];
   BYTE validcrc[4] = {0x00, 0x00, 0x00, 0x00};
   BYTE dest[6];
   BYTE* body;
   BYTE* data = new BYTE[100];
   BYTE* unStuffed = new BYTE[100];
	memset(data, 0, 100);
	memset(unStuffed, 0, 100);
	int packetSize = 0;
	int unstuffedPacketSize = 0;
	int validData = 1;
	//Loop through each bit
	for (int i=0; i < (len *8);i++) {
		bit = getbit(buf[(i/8)], i);
		
		if (preamble != 8) {
			if (preamble == 0 || preamble == 7) {
				if (bit == 0) preamble++;
				else preamble = 0;
			}
			else {
				if (bit > 0) preamble++;
				else preamble = 1;
			}
		}
		else if (postamble != 8) {
			if (postamble == 0 || postamble == 7) {
				if (bit == 0) postamble++;
				else postamble = 0;
			}
			else {
				if (bit > 0) postamble++;
				else postamble = 1;
			}
			if (bit > 0 ) {
				data[packetSize/8] = data[packetSize/8] | setbit(packetSize);
				packetSize++;
			}
			else {
				packetSize++;
			}
		}
		if (postamble == 8) {
			preamble = 0;
			postamble = 0;
			
			if (packetSize > 160) {
			//remove postamble from data
			for (int i=packetSize-7; i<=packetSize; i++) {
				int tbit = getbit(data[i/8], i);
				if (tbit > 0) {
					data[i/8] = data[i/8] & unsetbit(i);
				}
			}
			packetSize = packetSize -8;
			//unbit stuff
			unStuffed = unbitstuff(data, packetSize, &unstuffedPacketSize);
			//validate data
			int bodySize = (unstuffedPacketSize/8) - 18;
			body = new BYTE[bodySize];
			memcpy(crc, unStuffed + ((unstuffedPacketSize/8) -4), 4);
			memcpy(dest, unStuffed, 6);
			memcpy(body, unStuffed + 14, bodySize);
			if (memcmp(validcrc, crc, 4) != 0) {
				validData = 0;
				cout << "Invalid CRC" << endl;
			}
			if (memcmp(dest, src, 6) != 0) {
				validData = 0;
				cout << "Packet is not for me" << endl;
			}
			if(prot_higher && validData == 1) {
				cout << "Good Packet" << endl;
				prot_higher->pop (body, bodySize, mb);
			}
			}
			else {
				cout << "Bad Packet - Using this postamble as new preamble" << endl;
				preamble = 8;
			}
			packetSize = 0;
			validData = 1;
			memset(data, 0, 100);
			memset(unStuffed, 0, 100);
		}
	}
   return 0;
}

