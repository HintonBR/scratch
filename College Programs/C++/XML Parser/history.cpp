#include "history.h"

string History::getHTML() {
 return ("<br>&nbsp;&nbsp;&nbsp;&nbsp;" + text + "<br>");
}//end of getHTML method

void History::parse(string data) {
     int temp = 0;
    int temp2 = 0;
    cout << "History object found '" << data << "'" << endl;
    temp = data.find(">");
    if (data[temp -1] != '/') {
        temp++;
        temp2 = data.find("<", temp) - 1;
        text = data.substr(temp, temp2);
    }
    else {
        text = "";
    }
}//end of parse method

