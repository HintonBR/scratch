#include "xmltag.h"

string XMLTag::parse(string data) {
    int temp = 0;
    int temp2 = 0;
    string tempString = "";

    while (!(isalpha(data[temp]))) {
        temp++;
    }
    temp = data.find("<");
    data = data.substr(temp);
    temp = 1;
    while (isalpha(data[temp])) {
        tempString += tolower(data[temp]);
        temp++;
    }
    if (tempString == "person") {
        type = "Person";
        temp = data.find(">");
        if (data[temp - 1] != '/') {
            temp = data.find("</person");
            temp = data.find(">", temp);
        }
        if (temp == string::npos) {
            cout << "Opening person tag without a closing person tag -- Program exiting" << endl;
            exit(1);
        }
        temp++;
        value = data.substr(0, temp);
        cout << endl;
        cout << "Person found--'" << endl;
        cout << "'" << value << "'" << endl;
        data = data.substr(temp);
        return data;

    }
    else if (tempString == "family") {
        type = "Family";
        temp = data.find(">");
        if (data[temp - 1] != '/') {
            temp = data.find("</family");
            temp = data.find(">", temp);
        }
        if (temp == string::npos) {
            cout << "Opening family tag without a closing family tag -- Program exiting" << endl;
            exit(1);
        }
        temp++;
        value = data.substr(0, temp);
        cout << endl << "Family found--'" << endl << value << "'" << endl;
        data = data.substr(temp);
        return data;
    }
}
XMLTag::XMLTag() {
    type = "";
    value = "";
    id = "";
    history = NULL;
}
XMLTag::~XMLTag() {
}

