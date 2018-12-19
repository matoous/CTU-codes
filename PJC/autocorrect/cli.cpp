//
// Created by matouus on 26/11/18.
//
#include <iostream>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "autocorrect.hpp"

using namespace std;

void handle_corrections(pjc::autocorrect& ac, const bool parallel, const vector<string> &to_correct) {
    vector<vector<string>> corrections;
    if(parallel){
        corrections = ac.p_correct(to_correct);
    } else {
        corrections = ac.correct(to_correct);
    }
    // print corrections
    for(const auto& cs : corrections){
        for(const auto& c: cs)
            cout << c << " ";
        cout << endl;
    }
}

int main(int argc, char* argv[]){
    bool parallel = false;
    pjc::autocorrect ac;
    vector<string> to_correct = vector<string>();
    bool interactive = true;

    for(int i = 1; i < argc; i++){
        string argument(argv[i]);
        if(argument == "-p" || argument == "--parallel"){
            parallel = true;
        } else if(argument == "-d" || argument == "--dictionary"){
            i++;
            vector<string> dictionary = vector<string>();
            string word;
            // read all provided word until we find flag or end of words
            while((word = string(argv[i])).rfind('-', 0) != 0){
                dictionary.push_back(word);
            }
            // words were not terminated manually, next argument is flag
            if(word != "-")
                i--;
            // initialize autocorrect with dictionary
            ac = pjc::autocorrect(dictionary);
        } else if(argument == "-f" || argument == "--file"){
            // file flag without file name
            if(i+1 == argc)
                cerr << "missing dictionary file" << endl;

            string filename(argv[++i]);
            fstream dictionary(filename);
            ac = pjc::autocorrect(dictionary);
            if(!ac.size()){
                cerr << "provided file is empty or couldn't be open" << endl;
                return 1;
            }
        } else if((argument == "--help" || argument == "-h") && argc == 2){
            // print help
            cout << "Autoccorect command line tool (PJC)\n"
                         "\n"
                         "Autoccorect is command line tool for words correction.\n"
                         "It reads words from standard input and returns available corrections for each word on separate line.\n"
                         "\n"
                         "Usage: autocorrect [options] [words]\n"
                         "  options:\n"
                         "      -p, --parallel: use parallel version of the algorithm\n"
                         "      -d, --dictionary words... -: reads arguments until next flag or end and uses them as words for the dictionary\n"
                         "                                   End the words with '-' if you don't want to provide more flags\n"
                         "                                      and don't want to use autocorrect interactively.\n"
                         "      -f, --file file_name: uses provided file as source of words for the dictionary\n"
                         ;
        } else {
            // unsupported flag
            if(argument.rfind('-', 0) == 0){
                cerr << "unsupported argument" << endl;
                return 1;
            } else {
                // words are provided as command line arguments
                interactive=false;
                to_correct.push_back(argument);
            }
        }
    }

    // if the words are provided as arguments correct them and exit
    if(!interactive){
        handle_corrections(ac, parallel, to_correct);
        return 0;
    }

    string line;
    // read stdin line by line
    while(getline(cin, line)){
        // split line into words
        vector<string> vec;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(vec));

        handle_corrections(ac, parallel, vec);
    }

    return 0;
}