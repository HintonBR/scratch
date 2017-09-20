#ifndef FAMILY
#define FAMILY
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include "mchild.h"
#include "xmltag.h"
#include "person.h"


string getLink(list<Person*>*, string);
class Family : public XMLTag {
    public:
        virtual string parse(string);
        string getMother() {return mother;}
        string getFather() {return father;}
        string parseTag(string, string);
        void generateWebPage(list<Person*>*, list<Family*>*, string);
        Event* getMarriageDate();
        Family();
        ~Family();

    private:
        string father;
        string mother;
        list<Child*> children;
};//end of Family class
#endif


