#ifndef XMLTAG
#define XMLTAG
#include <iostream>
#include <string>
#include <ctype.h>
#include <list>
#include <algorithm>
#include "CS240.h"
#include "history.h"
#include "event.h"

class XMLTag : public CS240 {

    public:
      string getType() {return type;}
      string getID() {return id;}
      virtual string parse(string);
      string getValue() {return value;}
      XMLTag();
      ~XMLTag();
    protected:
      string id;
      History *history;
      list<Event*> events;
   private:
      string type;
      string value;
};//endof XMLTag class

#endif
