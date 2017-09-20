#ifndef EVENT
#define EVENT
#include <iostream>
#include <string>
#include <ctype.h>
#include "history.h"
#include "place.h"
#include "CS240.h"


class Event : public CS240 {

        public:
            string getHTML();
            void formatDate();
            string getDate() {return date;}
            string getType() {return type;}
            int getYear() {return year;}
            int getMonth() {return month;}
            int getDay() {return day;}
            string getPlace();
            Event();
            ~Event();
            void parse(string);

        private:
            string type;
            string date;
            int day;
            int month;
            int year;
            History *history;
            Place *place;

            void getMonth(const char*, int*);
};//end of Event class
bool compareDate(Event*, Event*);
#endif
