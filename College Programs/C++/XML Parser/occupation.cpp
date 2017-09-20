#include "occupation.h"
void Occupation::parse(string desc) {
    int temp = 0;
    int temp2 = 0;
    cout << "Occupation object found:" << endl << "'" << desc << "'" << endl;
    temp = desc.find("desc");
    if (temp != string::npos) {
        temp++;
        temp2 = desc.find("\"", temp);
        temp = desc.find("\"", temp2 + 1) - 1;
        description = desc.substr(temp2 + 1, temp - temp2);
    }
    else {
        description = "";
    }
}//end of parse method

