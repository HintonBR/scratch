#ifndef PARENTS
#define PARENTS
#include <iostream>
#include <string>
#include "CS240.h"

class Parents : public CS240 {

        public:
          string getHTML();
          string getID() {return id;}
          Parents(){}
          ~Parents(){}
          void parse(string);
        private:
            string id;
};//end of Parents class
#endif

