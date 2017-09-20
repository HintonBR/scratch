#ifndef HISTORY
#define HISTORY
#include <iostream>
#include <string>
#include "CS240.h"

class History : public CS240 {
        public:
            string getHTML();
            History(){}
            ~History(){}
            void parse(string);
        private:
            string text;
};//end of History class
#endif

