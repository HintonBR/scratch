#include "URL.h"


void tolower(char *sPtr) {
    while (*sPtr != '\0') {
        if (isalpha(*sPtr)) *sPtr = tolower(*sPtr);
        ++sPtr;
    }
}//end of tolower method

//Method to determine whether a page has an opening <HTML> tag or not
bool validatePage(OpenURL *con) {
    string tString;
    char currentChar;
    while (con->Empty() == false) {
        currentChar = con->ReadByte();
        if (currentChar == '<') {
            tString = con->ReadByte();
            tString += con->ReadByte();
            tString += con->ReadByte();
            tString += con->ReadByte();
            if (tolower(tString.c_str()) == "html") {
                while (con->ReadByte() != '>');
                return true;
            }
        }
    }
    return false;


}//end of validatePage method

//Method that returns the next token in the HTML page
//tok is the type of token that is being returned
//val is the value of the token
bool getNextToken(string& tok, string& val, OpenURL *con) {
    char currentChar;
    val = "";
    tok = "";
    currentChar = con->ReadByte();
    while ((currentChar != '<' || isalpha(currentChar) == false) && con->Empty() == false) {
        currentChar = con->ReadByte();
    }
    //if it is a word - read the word and return
    if (isalpha(currentChar) == true) {
        val = currentChar;
        currentChar = con->ReadByte();
        while (isalnum(con->getLookAhead()) == true) {
            currentChar = con->ReadByte();
            val += currentChar;
        }
        tok = "Word";
        return true;
    }
    if (currentChar == '<') {
        while ((con->getLookAhead() != ' ') && (con->getLookAhead() != '>') {
            currentChar = con->ReadByte();
            val += currentChar;
        }
        if (val[0] == '/') {
            tok = "IgnoreToken"
            currentChar = con->ReadByte();
            while (currentChar != '>') {
                currentChar = con->ReadByte();
            }
            return true;
        }
        else if (tolower(val.c_str()) == "a") {
            tok = "Link";
            while (con->getLookAhead() != '>') {
                if (tolower(con->getLookAhead()) == 'h') {
                    con->ReadByte();
                    if (tolower(con->getLookAhead()) == 'r') {
                        con->ReadByte();
                        if (tolower(con->getLookAhead()) == 'e') {
                            con->ReadByte();
                            if (tolower(con->getLookAhead()) == 'f') {
                                con->ReadByte();
                                while (con->ReadByte() != '=');
                                while (con->ReadByte() != '"');
                                val = "";
                                while (con->getLookAhead() != '"') {
                                    val += con->ReadByte();
                                }
                                while (con->ReadByte() != '>');
                                return true;
                            }
                        }
                    }
                }
            con->ReadByte();
            }//end of while
            con->ReadByte();
            tok = "Ignore Link";
            return true;
        }//end of else if
        else if (val.length() == 2 && tolower(val[0]) == 'h' && isdigit(val[1])) {
            tok = "Header";
            val = "";
            while (con->ReadByte() != '>');
            while (con->getLookAhead() != '<') {
                val += con->ReadByte();
            }
            val = val.trim();
            return true;
        }
        else if (tolower(val.c_str()) == "title") {
            tok = "Title";
            while (con->ReadByte() != '>');
            while (con->getLookAhead() != '<') {
                val += con->ReadByte();
            }
            val = val.trim();
            return true;
        }
        else {
            tok = "Ignore Token";
            while (con->ReadByte() != '>');
            return true;
        }

    }//end of if on condition (currentChar == '<')_

    //This indicates that there is no matter data to be parsed so return false
    return false;
}//end of getNextToken method

URL::URL(string tname) {
    name = tname;
    searched = false;
 }//end of constructor
 URL::URL() {

 }//end of constructor

URL::~URL() {
}//end of destructor

void parsePage(Letter[] lArray, URLList *tList, string test) {

    OpenURL connectObj = new openURL();
	string tokType;
	string value;
    string tSummary;
    WordList *tList;
    int tSummaryCount = 0;
    char *tString = new char(100);
  	summaryInfoFound = false;
    searched = true;
    connectObj->OpenSocket(name.c_str);

    #ifdefURLDebugging
        cout << "We are parsing the page " << name << endl;
    #endif

    //if the openURL object is empty then don't parse it
    if (connectObj->Empty() == false) {
          #ifdefURLDebugging
            cout << "Connection object opened" << endl;
        #endif

        //if the page has an HTML tag then parse it
        if (validatePage(connectObj) == true) {

        //while there are still more tokens
		while (getNextToken(tokType&, value&, connectObj)) {
            if (toktype == "Link") {

                #ifdefURLDebugging
                    cout << "Link found" << endl;
                #endif

                if (value.find(test) != string::npos) {
                    if (value.find(".htm") != string::npos || value.find(".html") != string::npos || value.find(".HTM") != string::npos || value.find(".HTML") != string::npos) {
                        if (tList->inList(value) == false) {
                            tList->insert(value);
                        }
                    }
                }
            }
            if (toktype == "Title") {
                summaryInfoFound = true;
                summary = value;

                #ifdefURLDebugging
                    cout << "Title tag found" << endl;
                #endif

                tString = strtok(value.c_str, " ");
                while (tString != NULL) {

                    //if it is a valid word - add it to the list
                   if (isalpha(tString[0]) {
                      tList = lArray[((tolower(tString[0])) - 'a')].insertWord)(tString);
                      if (tList->urlIsInList(name) == false) {
                        tList->addURL(name);
                      }
                   }
                }

            }
            if (toktype == "Header") {
                if (summaryInfoFound != true) {
                    summaryInfoFound = true;
                    summary = value;

                    #ifdefURLDebugging
                        cout << "Header tag found" << endl;
                    #endif

                    tString = strtok(value.c_str, " ");
                    while (tString != NULL) {

                    //if it is a valid word - add it to the list
                       if (isalpha(tString[0]) {
                            lArray[((tolower(tString[0])) - 'a')].insertWord)(tString);
                            if (tList->urlIsInList(name) == false) {
                                tList->addURL(name);
                            }
                        }
                    }
                }
                else {
                    tString = strtok(value.c_str, " ");
                    while (tString != NULL) {
                        //if it is a valid word - add it to the list
                        if (isalpha(tString[0]) {
                            lArray[((tolower(tString[0])) - 'a')].insertWord)(tString);
                            if (tList->urlIsInList(name) == false) {
                                 tList->addURL(name);
                            }
                        }
                    }
                }
            }
            if (toktype == "Word") {

                //if it is a valid word - add it to the list
                if (isalpha(tString[0]) {
                            lArray[((tolower(tString[0])) - 'a')].insertWord)(tString);
                            #ifdefURLDebugging
                                cout << "Word found" << endl;
                            #endif

                            if (tSummaryCount < 100) {
                                tSummary = tSummary + " " + tString;
                                tSummaryCount++;
                            }
                            if (tList->urlIsInList(name) == false) {
                             tList->addURL(name);
                            }
                }
            }
		}
        if (summaryFound == false) {
            summary = tSummary;
        }
	   } //end of while
      }
      else {
          cout << "No <HTML> tag could be found - so not parsing this page" << endl;
      }
    }
	else {
		cout << "The page " << name << " could not be opened - Program continuing without parsing this page" << endl;
	}
	delete connectObj;
    delete tString;
}//end of parsePage method

bool URL::operator==(string tname) {
    return (name == tname);
}//end of operator== method

string URL::getURL() {
    return name;
}//end of getURL

string URL::getSummary() {
    return summary;
}//end of getSummary method

