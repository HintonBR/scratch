#include "WordList.h"


WordList::WordList(string name) {
    string cname = name;
    cWord = new Word(cname);
    nextWord = NULL;

}//end of constructor

WordList::WordList() {
    cWord = NULL;
    nextWord = NULL;
}

WordList::~WordList() {
    if (cWord != NULL) {
        delete cWord;
        cWord = 0;
    }
    if (nextWord != NULL) {
        delete nextWord;
        nextWord = 0;
    }
}//end of destructor

WordList* WordList::isInList(string name){
    WordList *nWord = *this;
    const string tWord = name;

    while (!(nWord->(*cWord) == tWord) && nWord != NULL) {
        nWord = nWord->nextWord;
    }

    return nWord;

}//end of isInList method

WordList* WordList::addWord(string name){
    string tWord = name;
    WordList *nWord = *this;
    WordList *trailingWordList = *this;
    Word *tempWord;

    #ifdef WordListDebugging
        cout << "Adding the word '" << name << "'" << endl;
    #endif

    while (nWord->nextWord != NULL  && nWord->getWord().compare(0, nWord->getWord().length(), tWord) < 0) {
        trailingWordList = nWord;
        nWord = nWord ->nextWord;
    }
    if (nWord->nextWord == NULL) {
        if (nWord->getWord().compare(0, nWord->getWord().length(), tWord) > 0) {
            //if there is only one word in the list and the word being inserted needs to
            //be inserted in front of it - switch the word objects and then create a new
            //wordlist object
            if (this == trailingWordList) {
                tempWord = trailingWordList->cWord;
                trailingWordList->cWord = new Word(tWord);
                trailingWordList->nextWord = new WordList();
                trailingWordList->nextWord->cWord = tempWord;
                return trailingWordList;
            }
            trailingWordList->nextWord = new WordList(tWord);
            trailingWordList->nextWord->nextWord = nWord;
            return trailingWordList->nextWord;
        }
        else {
            nWord->nextWord = new WordList(tWord);
            return nWord->nextWord;
        }
    }
    else {
        trailingWordList->nextWord = new WordList(tWord);
        trailingWordList->nextWord->nextWord = nWord;
        return trailingWordList->nextWord;
    }
}//end of addWord method

void WordList::generateWordPages(string oDirectory) {
    string direct = oDirectory;
    WordList *tempList = *this;

    #ifdef WordListDebugging
        cout << "Generating word pages" << endl;
    #endif

    while (tempList != NULL) {
        tempList->cWord->generatePage(direct);
        tempList = tempList->nextWord;
    }
}//end of generateWordPages

string WordList::getWord() {
    return (cWord->getWord());
}//end of getWord method


int WordList::getWordCount() {
   return (cWord->getWordCount());
}//end of getWordCount method

bool WordList::urlIsInList(string tname) {
    return (cWord->urlIsInList(tname));
}//end of urlIsInList
void WordList::addURL(string tname) {
    cWord->addURL(tname);
}//end of addURL method
void WordList::incrementCount() {
    cWord->incrementCount();
}


