

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "crossword.h"
#include "ClueObj.h"
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <memory>

using namespace std;

namespace {
    size_t callback(
        const char* in,
        size_t size,
        size_t num,
        string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}


 // constructor
Crossword::Crossword(int year, int month, int day){
	getPuzzle(year,month,day);
}

// destructor
Crossword::~Crossword(){

	for( int row = 0; row < height; row ++){
		delete curGrid[row];
		delete solGrid[row];
		
	}
	delete [] curGrid;
	delete [] solGrid;
}


// put-to operator
ostream& operator<<(ostream& os, const Crossword & o){
    return o.printGrid(os, o.curGrid);
}


int Crossword::getPuzzle(int year, int month, int day) {
    // Create url
    char format_url[100];
    snprintf(format_url, sizeof(format_url), "https://raw.githubusercontent.com"
            "/doshea/nyt_crosswords/master/%d/%.2d/%.2d.json", year, month, day);
    
    cerr << format_url << endl;
    string url(format_url);
    
    // Create Curl
    CURL *curl = curl_easy_init();
    if(curl) {
        // Set options: URL, IP Resolve, Timeout, & Redirects
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); //IPv4
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Response information
        long httpCode(0);

        
        unique_ptr<string> httpData(new string);

        // Data handling function and container
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

        // Run GET, capture resp codem, and clean up
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);

        if(httpCode == 200) {
            cerr << "Sucessful response from " << url << endl;
            Json::Value jsonData;
            Json::Reader jsonReader;

            if(jsonReader.parse(*httpData.get(), jsonData)) {
                cerr << "Sucessfully parsed JSON data" << endl;

                Json::Value c_across;
                Json::Value c_down ;
                Json::Value a_across;
                Json::Value a_down;
                Json::Value grid;
                Json::Value gridnums;
            
                c_across = jsonData["clues"]["across"];
                c_down = jsonData["clues"]["down"];
                a_across = jsonData["answers"]["across"];
                a_down = jsonData["answers"]["down"];
                grid = jsonData["grid"];
                gridnums = jsonData["gridnums"];
            

                if(c_across.size() == a_across.size()) {
                    for(int i = 0; i < (int) c_across.size(); i ++) {
                        string clue = c_across[i].asString();
                        ClueObj obj(clue.substr(clue.find(". ") + 2), a_across[i].asString());
                        aClues.push_back(obj);
                    }
                } else {
                    cerr << "c len: " << c_across.size() << " a len: " << a_across.size() << endl;
                    cerr << "Across clues and answers are not the same len" << endl;
                    delete httpData.get();
                    return 1;
                }

                if(c_down.size() == a_down.size()) {
                    for(int i = 0; i < (int) c_down.size(); i ++) {
                        string clue = c_down[i].asString();
                        ClueObj obj(clue.substr(clue.find(". ") + 2), a_down[i].asString());
                        dClues.push_back(obj);
                    }
                } else {
                    cerr << "c len: " << c_down.size() << " a len: " << a_down.size() << endl;
                    cerr << "Down clues and answers are not the same len" << endl;
                    delete  httpData.get();
                    return 1;
                }

                cout << grid.size() << endl;
                cout <<sqrt(grid.size())<< endl;
                gridJSONInit(sqrt(grid.size()), sqrt(grid.size()), grid, gridnums);


            } else {
                cerr << "Could not parse HTTP data as JSON" << endl;
                //cerr <<  "HTTP data was:\n" << *httpData.get() << endl;
                delete httpData.get();
                return 1;
            }
            //delete httpData.get();
        } else {
            cerr << "Couldn't GET from " << url << endl;
            cerr << httpCode << " error" << endl;
            delete httpData.get();
            return 1;
        }
    } else {
        cerr << "Curl failed" <<endl;
        //delete httpData.get();
        return 1;
    }
    //delete httpData.get();
    return 0;
}


void Crossword::gridJSONInit(int w, int h, Json::Value & solution, Json::Value & startLocs){
	width = w;
	height = h;

    int loc(0);

	curGrid = new char * [height];
	solGrid = new char * [height];
    clueStartGrid = new char * [height];


	for( int row = 0; row < height; ++row ){
		curGrid[row] = new char[width];
		solGrid[row] = new char[width];
        clueStartGrid[row]  = new char[width];
		
		for(int col = 0; col < width; ++col){
			curGrid[row][col] = ' ';
			solGrid[row][col] = ((solution[loc]).asString())[0];
            clueStartGrid[row][col] = (char)(startLocs[loc].asInt());

            if(clueStartGrid[row][col] != 0){
                clueStartLocs.insert( make_pair((int)clueStartGrid[row][col], make_pair(row,col)));
            }

            if((solution[loc]).asString() =="."){
                curGrid[row][col] = (solution[loc]).asString()[0];
            }

            ++loc;
		}
	}
}

ostream & Crossword::printGrid(ostream& os, char ** toPrint) const{

    for(int col =0; col < width; ++col){
        os << "______";
    }
    os << endl;

    for(int row =0; row < height; ++row){
        os << "|";
        for(int col =0; col < width; ++col){

            if(clueStartGrid[row][col] != 0){
                //cout << (int)o.clueStartGrid[row][col] << endl;
                if(clueStartGrid[row][col]/10 == 0){
                    os << (int) clueStartGrid[row][col]   << "    |";
                }
                else if(clueStartGrid[row][col]/100 == 0){
                    os << (int) clueStartGrid[row][col]   << "   |";
                }
                else{
                    os << (int) clueStartGrid[row][col]   << "  |";
                }                
            }
            else{
                os << "     |";
            }
        }
        os << endl;


        os << "|";
        for(int col =0; col < width; ++col){
            os << "  " << toPrint[row][col] << "  |";
        }
        os << endl;

        os << "|";
        for(int col =0; col < width; ++col){
            os << "_____|";
        }
        os << endl;

    }
    return os;
}