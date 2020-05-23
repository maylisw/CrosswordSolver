#include "ClueObj.h"

// constructor
ClueObj::ClueObj(const string c, const string a) {
    clue = c;
    answer = a;
}

// copy constructor
ClueObj::ClueObj(const ClueObj& rhs) {
    clue = rhs.clue;
    answer = rhs.answer;
}

// copy assignment
ClueObj& ClueObj::operator=(const ClueObj& rhs) {
    if(this == &rhs) {
        return *this;
    }

    // now copy over
    clue = rhs.clue;
    answer = rhs.answer;

    return *this;
}

// put-to operator
ostream& operator<<(ostream& os, const ClueObj& o) {
    os << "{" << o.clue.c_str() << ", " << o.answer.c_str() << "}";
    return os;
}


