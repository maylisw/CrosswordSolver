

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "ClueObj.h"
#include <jsoncpp/json/json.h>

using namespace std;

class Crossword {
    public:
        // constructor
        Crossword(int year, int month, int day);

        // destructor
        ~Crossword();


        // put-to operator
        friend ostream& operator<<(ostream& os, const Crossword& o);

        void operator<<(istream & is);

        int getPuzzle(int year, int month, int day);

        void gridJSONInit(int w, int h, Json::Value & solution, Json::Value & startLocs);
        ostream & printGrid(ostream& os, char ** toPrint) const;
        
        // GETTERS
        //string getClue() const;

       // string getAnswer() const;

       // int length() const {return answer.length(); }

    private:
        char ** solGrid;
        char ** curGrid;
        char ** clueStartGrid;

        int height;
        int width;

        map<int, pair<int,int>> clueStartLocs;

        std::vector<ClueObj> aClues;
        std::vector<ClueObj> dClues;
};

