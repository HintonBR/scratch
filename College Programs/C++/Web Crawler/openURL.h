////////////////////////////////////////////////////////////////////
// OpenURL - Code to open a URL
//
// Copyright (C) 1999 William Moyes
// Distribution prohibited without written permission.
// Modification of this notice prohibited.
#ifndef OPENURL_H
#define OPENURL_H
#include <stdio.h>
#include "cs240.h"
class OpenURL : CS240
{
   public:
      OpenURL();
      ~OpenURL();
      void OpenSocket(char *url);      // Pass a URL
      char ReadByte(void);             // Returns next byte of page
      bool Empty(void) {return !Open;} // True when no data left
      char getLookAhead() {return LookAhead;}

   private:
      void CloseSocket(void);
      bool FileMode;
      bool Open;
      FILE *finput;
      char Buffer[1024];
      char LookAhead;
      int sockHandle;
      void InitHTTP(char *url);
      void InitFile(char *filename);
};
#endif

