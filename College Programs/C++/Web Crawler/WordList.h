#ifndefine WORDLIST_H
#define WORDLIST_H

#include <string>
#include "Word.h"


class WordList : CS240  {
    public:
        WordList(string);
        ~WordList();
        WordList* isInList(string);
        WordList* addWord(string);
        void generateWordPages(string);
        string getWord();
        int getWordCount();
        void incrementCount();
        bool urlIsInList(string);
        void addURL(string);


    private:
        Word *cWord;
        WordList *nextWord;

};//end of class Letter
#endif
