#include "place.h"

void Place::parse(string data) {
 int temp = 0;
    int temp2 = 0;
    cout << "Place found : '" << data << "'" << endl;
    temp = data.find(">");
    if (data[temp -1] != '/') {
        temp++;
        temp2 = data.find("<", temp);
        location = data.substr(temp, temp2 - temp);
    }
    else {
        location = "";
    }

}//end of parse method
