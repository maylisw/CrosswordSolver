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
        ClueObj(const string c, const string a);

        // destructor
        ~ClueObj(){}

        // copy constructor
        ClueObj(const ClueObj& rhs);

        // copy assignment
        ClueObj& operator=(const ClueObj& rhs);

        bool operator<( const ClueObj & rhs) const;
        // put-to operator
        friend ostream& operator<<(ostream& os, const ClueObj& o);
        
        // GETTERS
        string getClue() const;

        string getAnswer() const;

        int length() const {return answer.length(); }

    private:
        string clue;
        string answer;
};

#endif
