#include "parents.h"


string Parents::getHTML() {
    return ("<BR><strong>Parents:<strong>");
}//end of getHTML method
void Parents::parse(string data) {
    int temp = 0;
    int temp2 = 0;
    cout << "Parents object found:" << endl << "'" << data << "'" << endl;
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
}//end of parse method

