#include "Word.h"

Word::Word(string tname){
    name = tname;
    wordCount = 1;
    uList = NULL;
}//end of constructor

Word::Word() {
}

Word::~Word() {
    delete uList;
}//end of destructor

void Word::incrementCount() {
    wordCount += 1;
}//end of incrementCount method

bool Word::operator==(string tname) {
    return (tname == name);
}//end of operator == method

void Word::generatePage(string oDirectory) {
    string direct = oDirectory;
    string filePath = direct + '\\' + name + ".html";
    ofstream outFile(filePath, ios::out);
    WordList *tempList = uList;

    #ifdef WordDebugging
        cout << "Creating word page for '" << name << "'" << endl;
    #endif
    if (!outFile) {
        cerr << "File failed to open - not creating page!!" << endl;
        return false;
    }
    else {
        outFile << "<html><head><title>The " + name + " page</title>" << endl;
        outFile << "<h1>This is the index page for the word " + name + "</h1>" << endl;
        outFile << "<h3>Click <a href=\"" + direct + '\\' + name[0] + ".html\">here</a> to return to the home page</h3>" << endl;
        outFile << "<h3>Click <a href=\"" + direct + '\\' + "index.html\">here</a> to return to the home page</h3>" << endl;
        outFile << "<hr>" << endl;
        for(;tempList != NULL; tempList = tempList->nextURL) {
        outFile << "<a href=\"" +  tempList->getURL + "\">" + tempList->getURL() + "</a>" << endl;
        outFile << "Page Summary: " + tempList->getSummary() << endl;
        }
        outFile << "</body></html>" << endl;
        outFile.close();
    }
    return true;
}//end of generatePage method

bool Word::urlIsInList(string tname) {
    return (uList->inList(tname));
}//end of urlIsInList method

void Word::addURL(string tname) {
    string tString = tname;
    #ifdef WordDebugging
        cout << "Adding a URL to the list for " << name << endl;
    #endif
    if (uList == NULL) {
        uList = new URLList(tString);
    }
    else if (uList->inList(tString) == false) {
        uList->insert(tString);
    }
}//end of addURL method

string Word::getWord() {
    return name;
}//end of getWord

int Word::getWordCount() {
    return wordCount;
}//end of getWordCount


