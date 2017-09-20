#ifndef PERSON
#define PERSON
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <algorithm>
#include <ctype.h>
#include "marriage.h"
#include "occupation.h"
#include "parents.h"
#include "xmltag.h"

class Family;
bool marriagesort(Marriage*, Marriage*);
class Person : public XMLTag {

    public:
        Person();
        ~Person();
        virtual string parse(string);
        string getSurname() {return surname;}
        string getGivenNames() {return givenNames;}
        bool compareID(Person *);
        string getBirthDate();
        void printEventList();
        string getLink();
        void generateWebPage(list<Person*>*, list<Family*>*, string);
        void getMarriageDates(list<Family*>*);
        Event* getBirthEvent();
        string getBirthPlace();
        string getChildLink();

    private:
        string surname;
        string givenNames;
        char gender;
        Occupation *occupation;
        list<Marriage*> marriages;
        Parents *parents;

        string parseTag(string, string);
        string getParentLink(list<Person*>*, list<Family*>*, string);


};//end of Person class
bool childCompare(Person*, Person*);
#endif

