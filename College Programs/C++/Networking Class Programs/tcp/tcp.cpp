
// This is just a template for you to use in your coding

#include "tcp.h"

static int snum = 0;
static int nfe = 0;
struct window {
	unsigned char buffer[100];
	unsigned char ipaddr[4];
	int time;
	int len;
	int seqnum;
	int ack;
};

window win[20];
sem_t count, locked;
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

void tcp::sendwin(unsigned char* buf, int len, unsigned char* ipdest) {
	
	sem_wait(&count);
	sem_wait(&locked);
		//copy segment into win[]
	for(int i = 0; i < windowsize; i++) {
			if (win[i].time == 100) {
				cout << "Using segment number " << i << endl;
				memcpy(win[i].buffer, buf, len);
				win[i].len = len;
				win[i].ack = 0;
				memcpy(win[i].ipaddr, ipdest, 4); 
				win[i].time = 5;
				win[i].seqnum = snum;
			}//end of time
		}//end of for	
	sem_post(&locked);
	message* mb = 0;
	prot_lower->push(buf, len, ipdest, mb);
}

// Start the thread
void *startthread (void *arg) {
  tcp *myps = (tcp*)arg;
  myps->slidewin();
  return NULL;
}

void tcp::slidewin() {
	int i;
	while(true) {
		sleep(5);
		for(i = 0; i < windowsize; i++) {
			sem_wait(&locked);
			if (win[i].time != 100) {
				if (win[i].ack == 1) {
					//clean up segment and remove the ack
					cout << "Acknowledgement received for seqnum " << win[i].seqnum << endl; 
					win[i].ack = 0;
					memset(win[i].buffer, 0, win[i].len);
					win[i].len = 0;
					memset(win[i].buffer, 0, 4);
					win[i].time = 100;
					win[i].seqnum = -1;
					sem_post(&count);
				}
				else {
					if (win[i].time <=0) {
						//push
						cout << "Resending seqnum " << win[i].seqnum << endl; 
						message* mb = 0;
						prot_lower->push(win[i].buffer, win[i].len, win[i].ipaddr, mb);
						win[i].time = 5;
					}
					else {
						time -= 1;
					}
				}//else on ack
			}//end of time
			sem_post(&locked);
		}//end of for
	}//end of while

}
int  tcp::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int wsize, int client)
{
  sem_init(locked, 0, 1);
  sem_init(count, 0, wsize);
  cout << "This is the tcp init routine for " << name << endl;
  memcpy(my_tcpaddr,addr,TCP_ADDRESS_LEN);
  memcpy(my_ipaddr,&addr[2],IP_ADDRESS_LEN);
  print_addr("tcp port Address", my_tcpaddr, TCP_ADDRESS_LEN);
  print_addr("tcp IP Address", my_ipaddr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  memcpy(my_tcpaddr,addr,2);
  memcpy(my_ipaddr,addr+2,4);
  prot_lower = lower;
  prot_higher = higher;
  windowsize = wsize;
	
  	for(int i = 0; i < windowsize; i++) {
		win[i].time = 100;
		win[i].seqnum = -1;
	}
  pthread_t threadId; 
  pthread_create(&threadId, NULL, startthread, this)
}

// Push the data to the next lower layer
int tcp::push (unsigned char *buf, int len, char *addr, message *mb)
{
  cout << "This is the tcp push routine" << endl;
  print_chars(prot_name, buf, len);

  unsigned char dest_ipaddr[IP_ADDRESS_LEN];
  unsigned char dest_tcpaddr[TCP_ADDRESS_LEN];
  unsigned char* packet; 	
  u_short temp;

  memcpy(dest_tcpaddr,addr,2);
  memcpy(dest_ipaddr,addr+2,4);
  print_addr("Destination tcp port Address", dest_tcpaddr, TCP_ADDRESS_LEN);
  print_addr("Destination tcp IP Address", dest_ipaddr, IP_ADDRESS_LEN);
  if (len <= 100) { 
	packet = new unsigned char[len + 24];
	memset(packet, 0, len+24);
	memcpy(packet, my_tcpaddr, 2);
	memcpy(packet+2, dest_tcpaddr, 2);
	memcpy(packet+4, snum, 4);
	packet[12] = 0x60;
	packet[13] = 0x08;
	packet[15] = windowsize;
	memcpy(packet+20, my_ipaddr,4);
	memcpy(packet+24, buf, len);
	temp = calculateCheckSum((u_short*)packet, (24+len)/2);
	memcpy(packet+16, &(temp), 2);
	
	cout << "Sending out a tcp packet" << endl;
	print_chars(prot_name, packet, len+24);

	sendwin(packet, 24+len, dest_ipaddr);
	snum += len;
  }
  else {
	  
	  int templen = 0;
	while (templen < len) {
		int plen;
		int flag = 0;
		if ((len - templen) > 100) {
			plen = 100;
			flag = 1;
		}
		else {
			plen = len - templen;
		}
		//Build packet
		packet = new unsigned char[plen + 24];
		memset(packet, 0, plen+24);
		memcpy(packet, my_tcpaddr, 2);
		memcpy(packet+2, dest_tcpaddr, 2);
		memcpy(packet+4, snum, 4);
		packet[12] = 0x60;
		packet[13] = 0x08;
		packet[15] = windowsize;
		memcpy(packet+20, my_ipaddr,4);
		memcpy(packet+24, buf + templen, plen);
		temp = calculateCheckSum((u_short*)packet, (plen+24)/2);
		memcpy(packet+16, &(temp), 2);
		
		cout << "Sending out a fragement tcp packet" << endl;
		print_chars(prot_name, packet, plen+24);

		sendwin(packet, 24+plen, dest_ipaddr);
		snum += plen;	
		templen += plen;
	}
  }
}

// Pop the data from the next lower layer
int tcp::pop (unsigned char *buf, int len, message *mb)
{
  cout << "This is the tcp pop routine" << endl;
  print_chars(prot_name, buf, len);

  int seqnum;
  memcpy(&seqnum, buf[4], 4);
  if (buf[13] == 0x10) {
	for(i = 0; i < windowsize; i++) {
			sem_wait(&locked);
			if (win[i].seqnum == seqnum) {
				win[i].ack = 1;
			}//end of time
			sem_post(&locked);
		}//end of for
  }
  else {
	  if (seqnum == nfe) {
			
		  unsigned char dest_ipaddr[IP_ADDRESS_LEN];
		  unsigned char dest_tcpaddr[TCP_ADDRESS_LEN];
		  unsigned char* packet; 	
		  u_short temp;

		  memcpy(dest_ipaddr, buf[24], 4);
		  memcpy(dest_tcpaddr, buf, 2);
		  packet = new unsigned char[24];
			memset(packet, 0, len+24);
			memcpy(packet, my_tcpaddr, 2);
			memcpy(packet+2, dest_tcpaddr, 2);
			memcpy(packet+4, seqnum, 4);
			packet[12] = 0x60;
			packet[13] = 0x10;
			packet[15] = windowsize;
			memcpy(packet+20, my_ipaddr,4);
						
			cout << "Sending out a ACK packet" << endl;
			print_chars(prot_name, packet, 24);
			prot_lower->push(packet, 24, dest_ipaddr, mb);

		  prot_higher->pop(buf+24, len - 24, mb);
		  nfe += (len - 24);
	  }
	  else {
		cout << "Sequence number did not match nfe" << endl;	
	  }
  }
}


