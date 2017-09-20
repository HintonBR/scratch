#include "event.h"

string Event::getHTML() {
    string temp = "";

    temp = "<br><strong>Type: " + type + "<strong>&nbsp;&nbsp;&nbsp;&nbsp;Date: " + date + "&nbsp;&nbsp;&nbsp;&nbsp;" + (place != NULL ? "Place: " + place->getLocation() + "<br>" : "<br>") + "<strong>History<strong><br>" + (history != NULL ? history->getHTML() : "");
    return temp;
}//end of getHTML method

string Event::getPlace() {
       if (place != NULL) {
          return place->getLocation();
       }
       return "";
}
void Event::getMonth(const char *month, int *iMonth) {
    switch (month[0]) {
        case 'F':
        case 'f':
            *iMonth = 2;
            break;

        case 'S':
        case 's':
            *iMonth = 9;
            break;

        case 'O':
        case 'o':
            *iMonth = 10;
            break;

        case 'N':
        case 'n':
            *iMonth = 11;
            break;

        case 'D':
        case 'd':
            *iMonth = 12;
            break;

        case 'J':
        case 'j':
            switch (month[1]) {
                case 'A':
                case 'a':
                    *iMonth = 1;
                    break;

                case 'U':
                case 'u':
                    if (tolower(month[2]) == 'l') {
                        *iMonth = 7;
                    }
                    else {
                        *iMonth = 6;
                    }
                    break;

                default:
                    *iMonth = 0;
            }//end of switch
            break;

        case 'A':
        case 'a':
            if (tolower(month[1]) == 'p')  {
                *iMonth = 4;
            }
            else {
                *iMonth = 8;
            }
            break;

       case 'M':
       case 'm':
            if (tolower(month[2]) == 'r') {
                *iMonth = 3;
            }
            else {
                *iMonth = 5;
            }
            break;


       default:
            *iMonth = 0;
    }//end of switch
}//end of getMonth method

void Event::formatDate() {
    string tempString;
    int monthValue = 0;
    int temp = 0;
    int temp2 = 0;
    if (date != "") {
        if (date.find(" ", temp) != string::npos) {
            while (date.find(" ", temp) != string::npos) {
                  temp2 = date.find(" ", temp);
                  tempString = date.substr(temp, temp2 - temp);
                //the token is a month
                if (isalpha(tempString[0])) {
                    getMonth(tempString.c_str(), &monthValue);
                    month = monthValue;
                    for (int count = temp; count != temp2; count++) {
                        date[count] = toupper(date[count]);
                    }
                }
                else if (tempString.length() == 4) {
                    year = atoi(tempString.c_str());
                }
                else {
                    day = atoi(tempString.c_str());
                }
                temp2++;
                temp = temp2;
            }
       }
               tempString = date.substr(temp, date.length() - temp);
                //the token is a month
                if (isalpha(tempString[0])) {
                    getMonth(tempString.c_str(), &monthValue);
                    month = monthValue;
                    for (int count = temp; count != date.length(); count++) {
                        date[count] = toupper(date[count]);
                    }

                }
                else if (tempString.length() == 4) {
                    year = atoi(tempString.c_str());
                }
                else {
                    day = atoi(tempString.c_str());
                }
    }//end of if
}//end of formatDate Event


bool compareDate(Event * t1, Event *temp) {

    if (t1->getYear() < temp->getYear()) {
        return true;
    }
    else if (t1->getYear() == temp->getYear()) {
         if (t1->getMonth() < temp->getMonth()) {
            return true;
        }
        else if (t1->getMonth() == temp->getMonth()) {
             if (t1->getDay() < temp->getDay()) {
                return true;
            }
        }
    }
    return false;
}//end of compareDate method

Event::Event() {
    type = "" ;
    date = "";
    day = 0;
    month = 0;
    year = 0;
    history = NULL;
    place = NULL;
}

Event::~Event() {
    if (history != NULL) {
        delete history;
    }
    if (place != NULL) {
        delete place;
    }
}//end of destructor

void Event::parse(string data) {
    int temp = 0;
    int temp2 = 0;
    string tempstr = "";
    string tempString = "";
    cout << "Event found--'" << data << "'" << endl;
    //find and store the type attribute if it exists
    temp = data.find("type");
    if (temp != string::npos) {
        temp++;
        temp2 = data.find("\"", temp);
        temp = data.find("\"", temp2 + 1) - 1;
        type = data.substr(temp2 + 1, temp - temp2);
    }
    else {
        type = "";
    }
    //find date if there is one
    temp = data.find("date");
    if (temp != string::npos) {
        temp++;
        temp2 = data.find("\"", temp);
        temp = data.find("\"", temp2 + 1) - 1;
        date = data.substr(temp2 + 1, temp - temp2);
    }
    else {
        date = "";
    }
    formatDate();


    temp = 0;
    temp2 = 0;
    temp = data.find("<history");
    if (temp != string::npos) {
          temp2 = data.find(">", temp);
           if (data[temp2 - 1] != '/') {
              temp2++;
              temp2 = data.find("</history", temp2);
              temp2 = data.find(">", temp2);
              temp2++;
              tempstr = data.substr(temp, temp2 - temp);
          	}
        temp2++;
        tempstr = data.substr(temp, temp2 - temp);
    }

   if (tempstr != "") {
      history = new History();
      history->parse(tempstr);
	}

      temp = 0;
    temp2 = 0;
    tempstr = "";
    temp = data.find("<place");
    if (temp != string::npos) {
          temp2 = data.find(">", temp);
           if (data[temp2 - 1] != '/') {
              temp2++;
              temp2 = data.find("</place", temp2);
              temp2 = data.find(">", temp2);
              temp2++;
              tempstr = data.substr(temp, temp2 - temp);
          	}
        temp2++;
        tempstr = data.substr(temp, temp2 - temp);
    }

   if (tempstr != "") {
      place = new Place();
      place->parse(tempstr);
	}
}//end of parse method

