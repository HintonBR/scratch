#ifndefine URLLIST_H
#define URLIST_H
#include "URL.h"
#include "Letter.h"

class URLList : CS240 {
    public:
        bool inList(string);
        bool insert(string);
        bool parse(Letter[], string);
        string getSummary();
        string getURL();
        URLList(string, string);
        ~URLList();


    private:
        URL *pageName;
        URLList *nextURL;



};//end of URLList class
#endif
