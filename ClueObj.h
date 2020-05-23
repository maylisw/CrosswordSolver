#ifndef __CLUEOBJ_H__
#define __CLUEOBJ_H__

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

class ClueObj {
    public:
        // constructor
        ClueObj(const char *c, const char *a);

        // destructor
        ~ClueObj();

        // copy constructor
        ClueObj(const ClueObj& rhs);

        // copy assignment
        ClueObj& operator=(const ClueObj& rhs);

        int length() const {return strlen(answer); }

        // put-to operator
        friend ostream& operator<<(ostream& os, const ClueObj& o);

    private:
        char* clue;
        char* answer;
};

#endif
