#ifndef URL_H
#define URL_H

#include "Letter.h"
#include "openURL.h"
#include "WordList.h"
#include <string.h>
#include <ctype.h>
#include "URLList.h"


class URL : CS240  {

    public:
        URL(string);
        ~URL();
        void parsePage(Letter[], URLList* string);
        bool operator==(string);
        string getSummary();
        string getURL();

    private:
            string name;
            bool searched;
            string summary;
};//end of URL class
#endif
