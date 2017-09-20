#ifndef OCCUPATION
#define OCCUPATION
#include "CS240.h"
#include <iostream>
#include <string>

class Occupation : public CS240 {

    public:
        string getDesc(){return description;}
        void parse(string);
        Occupation(){}
        ~Occupation(){}

    private:
        string description;
     };//end of occupation class
#endif

