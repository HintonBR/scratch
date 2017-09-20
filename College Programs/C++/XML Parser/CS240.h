//
//CS 240 Base Class -- MUst be inherited by all other classes
//

#ifndef CS240_H
#define CS240_H

class CS240 {
    public :
        CS240();
        CS240(const CS240 &);

        ~CS240();

        static void print();
        static int get_creations();
        static int get_deletions();

    private:
        static int creations;
        static int deletions;

};

#endif
