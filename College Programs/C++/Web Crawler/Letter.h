#ifndefine LETTER_H
#define LETTER_H

#include <string>
#include <iostream>
#include "WordList.h"


class Letter : CS240  {
    public:
        Letter(char);
        ~Letter();
        WordList* isInWordList(string);
        WordList* insertWord(string);
        bool generatePage(string);

    private:
        char name;
        WordList *wList;
};//end of class Letter
#endif


