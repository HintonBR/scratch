#include "WordList.h"

URLList::URLList(string tname) {
    pageName = new URL(tname);
    nextURL = NULL;
}//end of constructor

URLList::URLList() {
}//end of constructor

URLList::~URLList() {
    delete pageName;
    if (nextURL != NULL) {
        delete nextURL;
    }
}//end of destructor

bool URLList::inList(string tname) {
    URLList *temp = *this;
    while (temp != NULL) {
        if ((*pageName) == tname) {
            return true;
        }
        temp = temp->nextURL;
    }
    return false;
}

bool URLList::insert(string tname) {
    URLList *temp = *this;
    while (temp->nextURL != NULL) {
        temp = temp->nextURL;
    }
    temp->nextURL = new URLList(tname);
    return true;
}//end of insert method

bool parse(Letter[] tObject, string tPath) {
    URLList *temp = *this;
    while (temp != NULL) {
        #ifdef URLListDebugging
            cout << "Parsing page found at '" << temp->pageName->getURL() << "'" << endl;
        #endif
        temp->pageName->parsePage(tObject, *this, tPath);
    }
    return true;
}//end of parse method

string URLList::getURL() {
    return pageName->getURL();
}//end of getURL method

string URLList::getSummary() {
    return pageName->getSummary();
}

