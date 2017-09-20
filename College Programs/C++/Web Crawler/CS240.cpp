//
//CS240 Base Class -- Must be inherited by other classes

#include <iostream>
#include "CS240.h"

int CS240::creations = 0;
int CS240::deletions = 0;

CS240::CS240() {
    ++creations;
}

CS240::CS240(const CS240 &cs) {
    ++creations;

}

CS240::~CS240() {

    ++deletions;
}

void CS240::print() {
    cout << "Created = " << creations << endl;
    cout << "Deleted = " << deletions << endl;

}

int CS240::get_creations() {
    return creations;
}

int CS240::get_deletions() {
    return deletions;
}
