#include "child.h"
string Child::getHTML() {
    return ("<br><strong>Children:<strong><br>");
}//end of getHTML method

void Child::parse(string data) {
    int temp = 0;
    int temp2 = 0;
    temp = data.find("id");
    if (temp != string::npos) {
        temp++;
        temp2 = data.find("\"", temp);
        temp = data.find("\"", temp2 + 1) - 1;
        id = data.substr(temp2 + 1, temp - temp2);
    }
    else {
        id = "";
    }
}//end of the parse method
