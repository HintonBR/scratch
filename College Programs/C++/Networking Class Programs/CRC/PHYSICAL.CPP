// This is just a template for you to use in your coding

#include "physical.h"

extern int blabby;

// Start the thread

int physical::init (char *name, protocol *higher, protocol *lower,
 char *address, int port, int client)
{

  DWORD threadid;
  HANDLE threadHandle;
  //int val;

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
	  threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &readloop, (void*) this, 0, &threadid);
		CloseHandle(threadHandle);
	 	//val = pthread_create(&mythread, NULL, physical::readloop, (void *) this);
	 	//printf("the create thread function returned a val of %d\n", val);
 	}
  return 0;
}

// Push the data to the next lower layer
int physical::push (unsigned char *buf, int len, char *addr, message *mb)
{
  memset(outbuffer, 0, 90);
  memcpy(outbuffer, buf, len);

  if(blabby & 0x1) {
    cout << "This is the physical push routine" << endl;
    print_chars(prot_name, buf, len);
  }

  /* number returned by read() and write() is the number of bytes
  ** read or written, with -1 being that an error occured
  ** write what we received back to the server */
  send(hSocket,outbuffer,90, 0);
	return 0;
}

void* readloop (void* arg) {
int total, rval;
unsigned char tempin[BUFFERSIZE];
physical* layer = (physical *) arg;
/* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    for (;;) {
			total=0;
			while (total < 90) {
				rval = recv(layer->hSocket, layer->inbuffer+total, 90-total, 0);
				if (rval > 0) {
					total += rval;				
				}
				else {
					exit(-1);				
				}			
			}
				memcpy(tempin, layer->inbuffer, total);
			    layer->pop(tempin, total, NULL);
    }

    /* close socket */
    if(closesocket(layer->hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
    return 0;
}//end of static readloop method

void physical::createServerSocket() {

	int nRet;
	SOCKET listenSocket;
	WORD 	versionRequested;
	WSADATA	wsaData;
	sockaddr_in clientName;
	sockaddr_in myserver;
	
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	
	ZeroMemory(&myserver, sizeof(SOCKADDR_IN));
	myserver.sin_family = AF_INET;
	myserver.sin_port = htons((u_short) hostport);
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Could not create listen socket" << WSAGetLastError() << endl;
		ExitThread(1);
	}

	nRet = bind(listenSocket, (SOCKADDR*) &myserver, sizeof(sockaddr_in));

	if (nRet == SOCKET_ERROR)
	{
		cout << "bind() error" << WSAGetLastError() << endl;
		closesocket(listenSocket);
		ExitThread(1);
	}

	nRet = listen(listenSocket, SOMAXCONN);
	
	if (nRet == SOCKET_ERROR)
	{
		cout << "listen() error" << WSAGetLastError();
		ExitThread(1);
	}
	
	printf("\nMaking a listen queue of %d elements",QUEUE_SIZE);
    /* establish listen queue */
    if(listen(listenSocket,QUEUE_SIZE) == SOCKET_ERROR)
    {
        printf("\nCould not listen\n");
        return;
    }

    printf("\nWaiting for a connection\n");
    /* get the connected socket */
    int temp=sizeof(SOCKADDR_IN);
	hSocket = accept(listenSocket, (SOCKADDR*) &clientName, &temp);

}//end of createServerSocket

void physical::createClientSocket() {

	sockaddr_in remote_server;
	HOSTENT *host;
	WORD 		versionRequested;
	WSADATA	wsaData;
    int nRet;
	char localhostname[100] = "";
	//HOSTENT FAR * namestruct;

	//Set up Winsock
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	
	
	//Get host and localhost information
	host=gethostbyname(strHostName);
	gethostname(localhostname, sizeof(localhostname));
	
	//Fill in Sock_addr_in structure
	ZeroMemory(&remote_server, sizeof(SOCKADDR_IN));
	CopyMemory(&remote_server.sin_addr, host->h_addr_list[0], sizeof(host->h_addr_list[0]));//host->h_length
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons((u_short) hostport);

	//Create Socket
    hSocket = socket(AF_INET, SOCK_STREAM, 0);

	//Connect
	nRet = connect(hSocket, (SOCKADDR*) &remote_server, sizeof(sockaddr_in));
	if (nRet == SOCKET_ERROR)
	{
		cout << "socket could not connet" << WSAGetLastError() << endl;
		closesocket(hSocket);
		ExitThread(0);
	}

 }	//end of createClientSocket
