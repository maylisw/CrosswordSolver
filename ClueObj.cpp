#include "ClueObj.h"

// constructor
ClueObj::ClueObj(const char *c, const char *a) {
    if(c) {
        clue = new char[strlen(c) + 1];
        strcpy(clue, c);
    } else {
        clue = new char[1];
        clue[0] = '\0';
    }
    
    if(a) {
        answer = new char[strlen(a) + 1];
        strcpy(answer, a);
    } else {
        answer = new char[1];
        strcpy(answer, a);
    }
}
// destructor
ClueObj::~ClueObj() {
    delete[] clue;
    delete[] answer;
}

// copy constructor
ClueObj::ClueObj(const ClueObj& rhs) {
    clue = new char[strlen(rhs.clue)];
    strcpy(clue, rhs.clue);

    answer = new char[strlen(rhs.answer)];
    strcpy(answer, rhs.answer);
}

// copy assignment
ClueObj& ClueObj::operator=(const ClueObj& rhs) {
    if(this == &rhs) {
        return *this;
    }

    // deallocate the objects in 'this'
    delete[] clue;
    delete[] answer;

    // now copy over
    clue = new char[strlen(rhs.clue)];
    strcpy(clue, rhs.clue);
    answer = new char[strlen(rhs.answer)];
    strcpy(answer, rhs.answer);

    return *this;
}

// put-to operator
ostream& operator<<(ostream& os, const ClueObj& o) {
    os << "{" << o.clue << ", " << o.answer << "}";
    return os;
}


