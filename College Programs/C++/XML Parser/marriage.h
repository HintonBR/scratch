#ifndef MARRIAGE
#define MARRIAGE
#include <iostream>
#include <string>
#include "CS240.h"

class Marriage : public CS240 {

		public:
            string getID(){return id;}
            Marriage(){}
            ~Marriage(){}
            void parse(string);
            void setYear(int m) { year = m;}
            void setMonth(int m) { month = m;}
            void setDay(int m) { day = m;}
            int getYear() {return year;}
            int getMonth() {return month;}
            int getDay() {return day;}
        private:
            string id;
            int year;
            int month;
            int day;
};//end of Marriage class
#endif

