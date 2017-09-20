
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
	memcpy(address, addr, 6);//store the addr for later use
	cout << "This is the ethernet init routine for " << name << endl;
	print_addr("Ethernet Address", addr, ETH_ADDRESS_LEN);
	prot_lower = lower;
	prot_higher = higher;
	strcpy(prot_name, name);

	return 0;
}

// Push the data to the next lower layer
int ethernet::push (unsigned char *buf, int len, char *addr, message *mb)
{
	int newLength = 0;
	int origLength = 18 + len;
	memset(origBuffer, 0, 100);
	memset(newBuffer, 0, 100);
	memcpy(origBuffer, addr, 6);
	memcpy(origBuffer + 6, address, 6);
	memcpy(origBuffer + 14, buf, len);
	newLength = bitStuff(origBuffer, len + 18, newBuffer);
	if(blabby & 0x2)
    {
		cout << "This is the ethernet push routine len = " << len << endl;
		print_addr("Ethernet Address", addr, ETH_ADDRESS_LEN);
		print_chars(prot_name, buf, len);
    }

	if(prot_lower)
	{
		prot_lower->push (newBuffer, 90, 0, NULL);
	}
	return 0;
}




// Pop the data from the next lower layer
int ethernet::pop (unsigned char *buf, int len, message *mb)
{
	

   if(blabby &0x2)
   {
       cout << "This is the ethernet pop routine" << endl;
       print_chars(prot_name, buf, len);
   }
   if(prot_higher)
   {
     prot_higher->pop ((unsigned char *)buf, len, mb);
   }
   return 0;
}

int ethernet::bitStuff(unsigned char *oldMessage, int len, unsigned char *newMessage)
{
	int newCounter = 0;
	int oldCounter = 0;
	int oneCounter = 0;
	int bit = 0;
	print_chars(prot_name, oldMessage, len);
	memset(newMessage, 0, 100);
	newMessage[0] = 0x7e;
	for(newCounter = 8; oldCounter<(len *8); newCounter++)
	{
		bit = getBit(oldMessage, oldCounter);
		if(bit != 0)
		{
			oneCounter++;
			setBit(newMessage, newCounter);
			if(oneCounter == 5)
			{
				newCounter++;
				oneCounter = 0;
			}
		}//end if
		else
		{
			oneCounter = 0;
		}
		oldCounter++;
	}//end for
	//Set end Sentinel
	for (int i=0; i < 8; i++) {
		if (i == 0 || i == 7) {
			newCounter++;
		}
		else {
			//newMessage[newCounter/8] = newMessage[newCounter/8] | 
			setBit(newMessage, newCounter);		
			newCounter++;
		}
	}

	//newCounter++;// = 0
	//newMessage[newCounter] = 0x7e;
	print_chars("Ethernet - after bit stuff", newMessage, (newCounter/8)+1);
	return newCounter;
}

int ethernet::getBit(unsigned char* message, int offset)
{
	//message[offset/8];//gives you the byte
	unsigned char mask = 0x80 >> (offset % 8);
	if((message[offset/8] & mask) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void ethernet::setBit(unsigned char* message, int offset)
{
	unsigned char mask = (0x80 >> (offset % 8));
	message[offset/8] |= mask;
}

int ethernet::unBitStuff(unsigned char *oldMessage, int len, unsigned char *newMessage)
{
	int newCounter = 0;
	int oldCounter = 0;
	int oneCounter = 0;
	int bit = 0;
	memset(newMessage, 0, 100);
	for(newCounter = 8; oldCounter<(len *8); newCounter++)
	{
		bit = getBit(oldMessage, oldCounter);
		if(bit != 0)
		{
			oneCounter++;
			setBit(newMessage, newCounter);
			if(oneCounter == 5)
			{
				oldCounter++;
				oneCounter = 0;
			}
		}//end if
		else
		{
			oneCounter = 0;
		}
		oldCounter++;
	}//end for
	newCounter++;// = 0
	return newCounter;
}
