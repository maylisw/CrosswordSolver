# CrosswordSolver
Using [NYT_Crosswords](https://github.com/doshea/nyt_crosswords) we built an acessible database of past clues and answers for the NYT Crossword puzzle.

### JSON
We used the JSON Cpp and Curl libraries to make request to the crossword database and build local objects that paried the clue and answers from each puzzle.

### Files
We then stored the files into txt files ... (in progress, more info later)

### Usage
tbd

## Note:
To use the JSON-test program you need to install jsoncpp and curl. Make sure the include headers for the two libraries align with where the packages are installed.

For example: <jsoncpp/json/json.h> might change

### Json Cpp
sudo apt-get install libjsoncpp-dev
iasa
### Curl
sudo apt-get install libcurl4-openssl-dev
