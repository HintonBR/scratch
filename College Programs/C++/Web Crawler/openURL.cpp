////////////////////////////////////////////////////////////////////
// OpenURL - Code to open a URL
//
// Copyright (C) 1999 William Moyes
// Distribution prohibited without written permission.
// Modification of this notice prohibited.
//
//
//
// This software supports file: and http://
// Notice that most servers want a slash at the end of the URL
// Example:
//    http://www.byu.edu/
//
// Also notice that files in the form 
//    file:/u1/ugrad/wmoyes/goodjob.html
// refer to the root directory and 
//    file:goodjob.html
// refers to the current directory.  This is the way Netscape does it
//
// This code will not time out if the server does not respond. If it apears
// to crash the sever might not be responding after the socket was opened.
// Also this code does not support redirected web-pages.  If you recieve
// a 301 or 302 error you don't have the real URL.  If you turn the
// SHOW_HEADER feature on you will see the header and where the real URL is.


#include <iostream.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "openURL.h"

// #define SHOW_HEADER // This will cause the program to display the HTTP header to cout

#define STRSIZE  256
#define HTTPPORT 80

OpenURL::OpenURL()
{
   Open = false;
   FileMode = true;
   finput = NULL;
   sockHandle = socket(AF_INET, SOCK_STREAM, 0);
}

OpenURL::~OpenURL()
{
   close(sockHandle);
   if (Open)
      CloseSocket();
}

void OpenURL::OpenSocket(char *url)
{
   if (Open)
   {
      cerr << "OpenURL: socket is still open, cannot reopen" << endl;
      return;
   }

 
   if (tolower(url[0]) == 'h' && tolower(url[1]) == 't' && tolower(url[2]) == 't' && tolower(url[3]) == 'p')
   {
      if (!(url[4] == ':' && url[5] == '/' && url[6] == '/'))
      {
         cerr << "OpenURL: Invalid URL" << endl;
         return;
      }
      // Http style
      InitHTTP(&url[7]);
   } 
   else if (tolower(url[0]) == 'f' && tolower(url[1]) == 'i' && tolower(url[2]) == 'l' && tolower(url[3]) == 'e' && url[4] == ':')
   {
      // file style
      InitFile(&url[5]);
   }
   else
   {
      cerr << "OpenURL: Invalid URL" << endl;
      return;
   }   
}

void OpenURL::CloseSocket(void)
{
   if (!Open)
   {
      cerr << "OpenURL: Cannot close a closed socket" << endl;
      return;
   }
   if (FileMode)
   {
      fclose(finput);
      Open = false;
   }
   else
   {
      Open = false;
   }
}

void OpenURL::InitHTTP(char *url)
{
   int i, j;
   char host[STRSIZE];
   int port;
   char location[STRSIZE];

   FileMode = false;

   // Grab the hosts name
   i = 0;
   while (url[i] != '/' && url[i] != '\0' && url[i] != ':')
   {
      host[i] = url[i];
      i++;
   }
   host[i] = '\0';

   // Grab the port number if present
   if (url[i] == ':')
   {
      i++;
      port = 0;
      while (url[i] >= '0' && url[i] <= '9')
      {
         port = port*10 + (url[i] - '0');
         i++;
      }
   }
   else
      port = HTTPPORT;

   // Now the rest should be a valid location
   j = 0;
   while (url[i] != '\0')
   {
      location[j] = url[i];
      i++;
      j++;
   }
   location[j] = '\0';

   // Now we open the socket;
   struct hostent *hostData = gethostbyname(host);
   if (hostData == NULL || *(hostData->h_addr_list) == NULL)
   {
      // Could not find the IP address of the host
      cerr << "OpenURL: Could not find host " << host << endl;
      return;
   }
  
   // I have double checked all this code, at it appears to be fine
   struct sockaddr_in SockAddress;
   memset(&SockAddress, 0, sizeof(SockAddress));
   memcpy(&SockAddress.sin_addr, hostData->h_addr, hostData->h_length);
   SockAddress.sin_port = htons(port);
   SockAddress.sin_family = hostData->h_addrtype;

   if (connect(sockHandle, (struct sockaddr*)&SockAddress, sizeof(SockAddress)) == -1)
   {
     cerr << "OpenURL: Could not connect to host " << host << endl;
     return;
   }   

   // Tell the server we want this URL   
   char OutMessage[STRSIZE];
   strcpy(OutMessage, "GET ");
   strcat(OutMessage, location);
   strcat(OutMessage, " HTTP/1.0\r\n");
   send(sockHandle, OutMessage, strlen(OutMessage), 0); // Send the GET command
   send(sockHandle, "\r\n", 2, 0); // Send a blank line

   Open = true;

   int len = recv(sockHandle, &LookAhead, 1, O_NDELAY);
   if (len != 1)
   {
      // Socket was closed
      Open = false;
   }

   // Parse off the HTTP header information if its present
   if (LookAhead == 'H')  // Make sure a header was sent
   {
   #ifndef SHOW_HEADER
      // Strip off the HTTP header
      while(Open)
      {
         char ch;
         ch = this->ReadByte();
         if (ch == '\n' && Open)
         {
 	    this->ReadByte();  // should be a '\r'
            if (this->ReadByte() == '\n')
               break;
         }
      }
   #else   
      cout << "********HEADER********" << endl;
      while(Open)


      {
         char ch, ch2;
         ch = this->ReadByte();
         cout << ch;
         if (ch == '\n' && Open)
         {
	    if (Open) cout << this->ReadByte();  // should be a '\r'
            if (Open) ch2 = this->ReadByte();
            cout << ch2;
            if (ch2  == '\n')
               break;
         }
      } 
      cout << "**********************" << endl;
   #endif
   }
   else
   {
   #ifdef SHOW_HEADER
      cout << "*******NO HEADER******" << endl;
   #endif
   }
}     

void OpenURL::InitFile(char *filename)
{
   FileMode = true;
   finput = fopen(filename, "rb");
   if (finput == NULL)
   {
      cerr << "OpenURL: Error opening file " << filename << endl;
      return;
   }
   Open = true;
   LookAhead = fgetc(finput);
   if (feof(finput))
   {
      fclose(finput);
      Open = false;
   }
} 

char OpenURL::ReadByte(void)
{
   char ch;
   char ReturnChar;

   if (!Open)
   {
     cerr << "OpenURL: socket not open" << endl;
     return 0;
   }
   
   if (FileMode)
   {
      ReturnChar = LookAhead;
      ch = fgetc(finput);
      if (feof(finput))
      {
         fclose(finput);
         Open = false;
      }      
      else
         LookAhead = ch;
   }
   else
   {
      int len;

      ReturnChar = LookAhead;    
      len = recv(sockHandle, &LookAhead, 1, O_NDELAY);
      if (len != 1)
      {
	 // Socket was closed
         Open = false;
      }
   }
   return ReturnChar;
}

bool OpenURL::Empty(void)
{
   if (!Open)
      return true;
   else
      return false;
}

/********************************************************
void main(void)
{
   OpenURL http;

   char str[STRSIZE];
   
   do
   {
     cout << "Enter URL: ";
     cin.getline(str, STRSIZE, '\n');
     if (str[0] != '\0')
     {
        http.OpenSocket(str);
        while (!http.Empty())
        {
	  cout << http.ReadByte();
        }
        cout << endl;
     }
   } while (str[0] != '\0'); 
}
 ***********************************************************/

