#include "Letter.h"

Letter::Letter(char cname) {
    #ifdef LetterDebugging
        cout << "Letter object created" << endl;
    #endif
    name = cname;
    wList = NULL;
}

//default constructor
Letter::Letter() {
}

Letter::~Letter() {
    delete wList;
}

WordList* Letter::insertWord(string wName) {

    string wordName = wName;
    #ifdef LetterDebugging
        cout << "Searching for a word object" << endl;
    #endif

    if (wList == NULL) {
        wList = new WordList(wordName);
        return wList;
    }
    if (wList->isInList(wordName) == NULL) {
        #ifdef LetterDebugging
            cout << "Inserting a word object" << endl;
        #endif
        return (wList->addWord(wordName));
    }
    wList->incrementCount();
    return (wList->isInList(wordName));

}//end of insertWord method

bool Letter::generatePage(string oDirectory) {
    string direct = oDirectory;
    string filePath = direct + '\\' + name + ".html";
    ofstream outFile(filePath, ios::out);
    WordList *tempList = wList;

    if (!outFile) {
        cerr << "File failed to open - not creating page!!" << endl;
        return false;
    }
    else {
        outFile << "<html><head><title>The " + name + " page</title>" << endl;
        outFile << "<h1>This is the index page for all words starting with the letter " + name + "</h1>" << endl;
        outFile << "<h3>Click <a href=\"" + direct + '\\' + "index.html\">here</a> to return to the home page</h3>" << endl;
        outFile << "<hr>" << endl;
        for(;tempList != NULL; tempList = tempList->nextWord) {
        outFile << "<a href=\"" + direct + '\\' + tempList->getWord() + ".html\">tempList->getWord()</a>" << endl;
        outFile << "Total number of times " + tempList->getWord() + " appeared in pages was " + tempList->getWordCount + " times" << endl;
        }
        outFile << "</body></html>" << endl;
        outFile.close();
    }
    if (wList->generateWordPages(direct)) {
        return true;
    }
    else {
        return false;
    }

}//end of generatePage method

