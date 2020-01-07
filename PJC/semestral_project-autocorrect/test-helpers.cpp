#include "test-helpers.hpp"

#include <random>
#include <algorithm>
#include <iostream>
#include <random>
#include <omp.h>

size_t lines_in_file(const std::string& filename){
    size_t count = 0;
    std::string line;

    std::fstream infile;
    infile.open(filename);
    while (getline(infile, line))
        count++;

    infile.close();
    return count;
}

bool vector_contains(const std::vector<std::string>& words, const std::string word){
    for(const auto& w: words){
        if(w == word){
            return true;
        }
    }
    return false;
}

std::vector<std::string> generateWrongWords(std::string filename){
    // open file
    std::fstream infile;
    infile.open(filename);

    std::vector<std::string> dictionary;

    // read words
    std::string word;
    while(infile >> word)
        dictionary.push_back(word);

    // close file
    infile.close();

    // generate wrong words
    std::vector<std::string> misspelledWords;
    std::sample(dictionary.begin(), dictionary.end(), std::back_inserter(misspelledWords),
                16, std::mt19937_64{std::random_device{}()});

    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (auto &misspelledWord : misspelledWords) {
        switch (rand()%3) {
            case 0:
                misspelledWord += alphabet[rand() % alphabet.size()];
                break;
            case 1:
                misspelledWord[rand() % misspelledWord.size()] = alphabet[rand() % alphabet.size()];
                break;
            case 2:
                misspelledWord = alphabet[rand() % alphabet.size()] + misspelledWord;
                break;
            default:
                misspelledWord += alphabet[rand() % alphabet.size()];
                break;
        }
    }

    // exit
    return misspelledWords;
}