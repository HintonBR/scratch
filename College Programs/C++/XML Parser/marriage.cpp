#include "marriage.h"

void Marriage::parse(string data) {
    int temp = 0;
    int temp2 = 0;
    cout << "Marriage object found: " << endl << "'" << data << "'" << endl;
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

}//end of marriage method

