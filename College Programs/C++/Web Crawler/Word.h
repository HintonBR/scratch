#ifndefine WORD_H
#define WORD_H

#include <string>
#include <iostream>
#include "URLList.h"


class Word : CS240 {
    public:
        Word(string);
        ~Word();
        void incrementCount();
        bool operator==(string);
        void generatePage(string);
        bool urlIsInList(string);
        void addURL(string);
        string getWord();
        int getWordCount();

    private:
        string name;
        long wordCount;
        URLList *uList;

};//end of class Letter
#endif
