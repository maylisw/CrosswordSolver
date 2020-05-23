/*
 * Tester file to pratice with JSON retrevial
 * 
 * Adapted from: 
 * https://gist.github.com/connormanning/41efa6075515019e499c
 *
 * - maylisw
*/
#include <iostream>
#include <string>
#include <memory>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

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

    cerr << httpCode << endl;

    if(httpCode == 200) {
        cerr << "Sucessful response from " << url << endl;

        Json::Value jsonData;
        Json::Reader jsonReader;

        if(jsonReader.parse(*httpData.get(), jsonData)) {
            cerr << "Sucessfully parsed JSON data" << endl;
            cerr << "\n JSON data recieved:" << endl;

            // NEED to strtok() by ,\n to create each one 
            cerr << "across" << jsonData["answers"]["across"].toStyledString() << endl;
            cerr << "down" << jsonData["answers"]["down"].toStyledString() << endl;
            cerr << "across" << jsonData["clues"]["across"].toStyledString() << endl;
            cerr << "down" << jsonData["clues"]["down"].toStyledString() << endl;

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
