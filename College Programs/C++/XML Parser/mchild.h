#ifndef CHILD
#define CHILD
#include <iostream>
#include <string>
#include "CS240.h"

class Child : public CS240 {

        public:
            string getHTML();
            void parse(string);
            string getID() {return id;}
            Child(){}
            ~Child(){}


        private:
            string id;
};//end of Child class
#endif
