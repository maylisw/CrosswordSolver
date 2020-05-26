/*
 * Main class
 *
 * - maylisw
*/
#include <iostream>
#include <string>
#include <memory>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "ClueObj.h"
#include <set>

const int years[] = {1976}; 

/*    1977, 1978, 1979, 1980, 1981, 1982, 1983,
                     1984, 1985, 1986, 1987, 1988, 1989, 1990, 1991,
                     1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
                     2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
                     2008, 2009, 2010, 2011, 2012, 2013, 2014};
*/
// Days per month
const int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
const int leapMonths[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
const std::vector<string> outfileNames = {"len3.txt", "len4.txt", "len5.txt", "len6.txt", "len7.txt", "len8+.txt"};
const string delim = " ||| ";
const int MINWORDLENGTH = 3;
const int MAXWORDLENGTH = 8;
const int BUFFERSIZE = 75;

using namespace std;

int getAll(set<ClueObj>& pairs);
void printAll(set<ClueObj>& pairs);
string findInFile(string clue, int answerLength);
string binarySearchHelper( int left, int right, ifstream & ifile, string & clue);

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

int getPairs(int year, int month, int day, set<ClueObj>& pairs) {
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

            
                c_across = jsonData["clues"]["across"];
                c_down = jsonData["clues"]["down"];
                a_across = jsonData["answers"]["across"];
                a_down = jsonData["answers"]["down"];
            

            

                if(c_across.size() == a_across.size()) {
                    for(int i = 0; i < (int) c_across.size(); i ++) {
                        string clue = c_across[i].asString();
                        ClueObj obj(clue.substr(clue.find(". ") + 2), a_across[i].asString());
                        pairs.insert(obj);
                        //cerr << obj << endl;; 
                    }
                } else {
                    cerr << "c len: " << c_across.size() << " a len: " << a_across.size() << endl;
                    //cerr << c_across << endl;
                    //cerr << a_across << endl;
                    cerr << "Across clues and answers are not the same len" << endl;

                    delete httpData.get();
                    return 1;
                }

                if(c_down.size() == a_down.size()) {
                    for(int i = 0; i < (int) c_down.size(); i ++) {
                        string clue = c_down[i].asString();
                        ClueObj obj(clue.substr(clue.find(". ") + 2), a_down[i].asString());
                        pairs.insert(obj);
                        //cerr << obj << endl;
                    }
                } else {
                    cerr << "c len: " << c_down.size() << " a len: " << a_down.size() << endl;
                    //cerr << c_down << endl;
                    //cerr << a_down<< endl;
                    cerr << "Down clues and answers are not the same len" << endl;
                    delete  httpData.get();
                    return 1;
                }


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

int main(int argc, char **argv) {
    // Note: This code works for all leap years from 1904 to 2096

    //TODO create a loop to request each puzzel for each day or each month of each year
    // NOTE: leap years all divide by 4
    //KNOWN gaps, aug 10, 1978 trhough nov 5, 1978
    //inconsistant data starting in 2015


    set<ClueObj> pairs;


    //getPairs(1978, 06, 1, pairs);
    getAll(pairs);
    printAll(pairs);


    //cout<< findInFile("Pat" , 3) <<endl;


    return 0;
}






int getAll(set<ClueObj>& pairs){


    for(unsigned int ycount = 0 ; ycount < sizeof(years)/sizeof(years[0]); ++ycount){
        for(unsigned int mcount = 0; mcount < sizeof(months)/ sizeof(months[0]); ++mcount){

            if(years[ycount]%4==0){
                for(int day = 1; day <= leapMonths[mcount]; ++day){
                    getPairs(years[ycount], mcount +1, day, pairs);
                }
            }

            else{
                for(int day = 1; day <= months[mcount]; ++day){
                    getPairs(years[ycount], mcount +1, day, pairs);
                }
            }
        }
    }

    return 0;

}

void printAll(set<ClueObj>& pairs){
    ofstream ** ofiles = new ofstream * [outfileNames.size()];

    for(unsigned int fileNum =0; fileNum < outfileNames.size(); ++fileNum){

        ofiles[fileNum] = new ofstream(outfileNames[fileNum]); 
    }


    for(set<ClueObj>::const_iterator cit = pairs.begin(); cit != pairs.end(); ++cit){

        int length = min( MAXWORDLENGTH, (int)(cit -> getAnswer()).size() );
        length = max(MINWORDLENGTH, length);

        string in = cit -> getClue() + delim + cit -> getAnswer() + '\n';

        in.resize(BUFFERSIZE);


        *(ofiles[length- MINWORDLENGTH]) << in;
    }



    for(unsigned int fileNum =0; fileNum < outfileNames.size(); ++fileNum){
        delete ofiles[fileNum]; 
    }

    delete [] ofiles;
}




string findInFile(string clue, int answerLength){

    answerLength = max(MINWORDLENGTH, answerLength);
    answerLength = min(MAXWORDLENGTH, answerLength);

    ifstream ifile(outfileNames[answerLength - MINWORDLENGTH]);


    
    int fileSize;

    ifile.seekg(0, ifile.end);

    fileSize = ifile.tellg()/BUFFERSIZE;

    //cout << ifile.tellg() << endl;1

    return binarySearchHelper( 0 , fileSize, ifile, clue);
}


string binarySearchHelper( int left, int right, ifstream & ifile, string & clue){

    if(left >= right){
        return "";
    }

    int mid = (right + left)/2;
    //cout << left << " " << right << " " << mid << endl;

    char bufferline[BUFFERSIZE];

    ifile.seekg( (mid) * BUFFERSIZE);
    ifile.getline(bufferline, BUFFERSIZE);

    string bufferStr(bufferline);
    string bufferClue(bufferStr.substr(0 , bufferStr.find(delim)));



    if (clue == bufferClue){
        return bufferStr.substr(bufferStr.find(delim) +delim.size());
    }

    else if( clue < bufferClue){
        return binarySearchHelper(left, mid, ifile, clue);
    }
    else{
        return binarySearchHelper(mid + 1, right, ifile, clue);
    }


}

