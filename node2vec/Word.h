//
// Created by Fu,Yanmei on 17/6/3.
//

#ifndef NODE2VEC_WORD_H
#define NODE2VEC_WORD_H

#endif //NODE2VEC_WORD_H



#include <vector>
#include <string>
#include <cstdint>
#include <memory>

using namespace std;

class Word
{
public:
    size_t index;
    size_t count;
    float sample_probability;
    string text;
    Word *left, *right;

    std::vector<size_t> codes;
    std::vector<size_t> points;

public:
    Word(void){};
    Word(size_t index, size_t count, string text, Word *left = nullptr, Word *right = nullptr):
            index(index), count(count), text(text), left(left), right(right) {}

    ~Word(void){};
};

typedef std::shared_ptr<Word> WordP;