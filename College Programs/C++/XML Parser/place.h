#ifndef PLACE
#define PLACE
#include <iostream>
#include <string>
#include "CS240.h"

class Place : public CS240 {
    public:
        string getLocation() {return location;}
        void parse(string);
        Place(){}
        ~Place(){}
    private:
        string location;

};//end of Place class
#endif
