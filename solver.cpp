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

#include "ClueObj.h"

const int years[] = {1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983,
                     1984, 1985, 1986, 1987, 1988, 1989, 1990, 1991,
                     1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
                     2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
                     2008, 2009, 2010, 2011, 2012, 2013, 2014};

// Days per month
const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 

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

int main(int argc, char **argv) {
    // NOTE: leap years all divide by 4
    // note - not portable for any new centuries lol due to leap year rules
    //TODO create a loop to request each puzzel for each day or each month of each year
    //KNOWN gaps, aug 10, 1978 trhough nov 5, 1978
    //inconsistant data starting in 2015

    // Create url
    char format_url[100];
    snprintf(format_url, sizeof(format_url), "https://raw.githubusercontent.com/doshea/nyt_crosswords/master/%d/%.2d/%.2d.json", 1980, 12, 01);
    
    cerr << format_url << endl;
    string url(format_url);
    
    // Create Curl
    CURL *curl = curl_easy_init();

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

    if(httpCode != CURLE_HTTP_RETURNED_ERROR) {
        cerr << "Sucessful response from " << url << endl;

        Json::Value jsonData;
        Json::Reader jsonReader;

        if(jsonReader.parse(*httpData.get(), jsonData)) {
            cerr << "Sucessfully parsed JSON data" << endl;

            int c_across_size = (int) jsonData["clues"]["across"].size();
            int c_down_size = (int) jsonData["clues"]["down"].size();
            int a_across_size = (int) jsonData["answers"]["across"].size();
            int a_down_size = (int) jsonData["answers"]["down"].size();

            Json::Value c_across = jsonData["clues"]["across"];
            Json::Value c_down = jsonData["clues"]["down"];
            Json::Value a_across = jsonData["answers"]["across"];
            Json::Value a_down = jsonData["answers"]["down"];
        
            if(c_across_size == a_across_size) {
                for(int i = 0; i < c_across_size; i ++) {
                    cerr << "clue: " << c_across[i];
                    cerr << " answer: " << a_across[i] << endl;
                    //TODO use .toStyledString and/or .c_str() to create ClueObjs 
                }
            } else {
                cerr << "c len: " << c_across_size << " a len: " << a_across_size << endl;
                cerr << c_across << endl;
                cerr << a_across << endl;
                cerr << "Across clues and answers are not the same len" << endl;
            }

            if(c_down_size == a_down_size) {
                for(int i = 0; i < c_down_size; i ++) {
                    cerr << "clue: " << c_down[i];
                    cerr << " answer: " << a_down[i] << endl;
                    //TODO use .toStyledString and/or .c_str() to create ClueObjs 
                }
            } else {
                cerr << "c len: " << c_down_size << " a len: " << a_down_size << endl;
                cerr << c_down << endl;
                cerr << a_down<< endl;
                cerr << "Down clues and answers are not the same len" << endl;
            }

       } else {
            cerr << "Could not parse HTTP data as JSON" << endl;
            cerr <<  "HTTP data was:\n" << *httpData.get() << endl;
            return 1;
        }
    } else {
        cerr << "Couldn't GET from " << url << endl;
        cerr << httpCode << " error" << endl;
        return 1;
    }
    return 0;
}