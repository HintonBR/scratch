// This is just a template for you to use in your coding

#include "physical.h"

extern int blabby;

// Start the thread

int physical::init (char *name, protocol *higher, protocol *lower,
 char *address, int port, int client)
{

  pthread_t mythread;
  int val;

  cout << "This is the physical init routine for " << name << endl;
  strcpy(prot_name , name);
  strcpy(strHostName, address);
  prot_higher = higher;
  prot_lower = lower;
  hostport = port;
  // You should put code to set up the socket here
        /* make a socket */
  if (client == 0) {	//Server
	  cout << "Creating a server socket" << endl;
		createServerSocket();
  }
  else {	//Client
    cout << "Creating a client socket" << endl;
		createClientSocket();		
  }

 	//spawn thread
	if(hSocket == SOCKET_ERROR) {
        printf("\nCould not make a socket\n");
        return 0;
  }
  else {
	 	val = pthread_create(&mythread, NULL, physical::readloop, (void *) this);
	 	printf("the create thread function returned a val of %d\n", val);
 	}
}

// Push the data to the next lower layer
int physical::push (unsigned char *buf, int len, char *addr, message *mb)
{
  int rval, total;
  memset(outbuffer, 0, 90);
  memcpy(outbuffer, buf, len);

  if(blabby & 0x1) {
    cout << "This is the physical push routine" << endl;
    print_chars(prot_name, buf, len);
  }

  /* number returned by read() and write() is the number of bytes
  ** read or written, with -1 being that an error occured
  ** write what we received back to the server */
  write(hSocket,outbuffer,90);

}

static void* physical::readloop (void* arg) {
int total, rval;
physical* layer = (physical *) arg;
/* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    for (;;) {
			total=0;
			while (total < 90) {
				rval = read(layer->hSocket, layer->inbuffer+total, 90-total);
				if (rval > 0) {
					total += rval;				
				}
				else {
					exit(-1);				
				}			
			}
			    layer->pop(layer->inbuffer, total, NULL);
    }

    /* close socket */
    if(close(layer->hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
    return 0;
}//end of static readloop method

void physical::createServerSocket() {


		int hServerSocket;  /* handle to socket */
    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address; /* Internet socket address stuct */
    int nAddressSize=sizeof(struct sockaddr_in);
    char pBuffer[BUFFERSIZE];
	printf("\nStarting server");

    printf("\nMaking socket");
    /* make a socket */
    hServerSocket=socket(AF_INET,SOCK_STREAM,0);

    if(hServerSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return;
    }

    /* fill address struct */
    Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(hostport);
    Address.sin_family=AF_INET;

    printf("\nBinding to port %d",hostport);

    /* bind to a port */
    if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address))
                        == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return;
    }
 /*  get port number */
    getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize);
    printf("opened socket as fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port) );

        printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %d\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );


    printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return;
    }

    printf("\nWaiting for a connection\n");
    /* get the connected socket */
    hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);

}//end of createServerSocket

void physical::createClientSocket() {

    struct hostent* pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;

		
    printf("\nMaking a socket");
    /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return;
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(hostport);
    Address.sin_family=AF_INET;

    printf("\nConnecting to %s on port %d",strHostName,hostport);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address))
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return;
    }
 }	//end of createClientSocket
