#include <iostream>
#include "Node2Vec.h"
#include "Word2Vec.h"

int main() {
    //std::cout << "Hello, World!" << std::endl;
    Node2Vec *node = new Node2Vec();
    string input_file = "";
    string output_file = "text8-sgns.txt";
    string save_vocab_file = "";
    string read_vocab_file = "";
    string model = "sg";
    string train_method = "ns";

    int table_size = 100000000;
    int word_dim = 200;
    float init_alpha = 0.025f;
    int window = 5;
    float subsample_threshold = 0.0001;
    float min_alpha = init_alpha * 0.0001;
    bool cbow_mean = true;
    int negative = 0;
    int num_threads = 1;
    int iter = 1;
    int min_count = 0;
    if(cbow_mean)
        init_alpha = 0.05;
    Word2Vec w2v(iter, window, min_count, table_size, word_dim, negative, subsample_threshold,
                 init_alpha, min_alpha, cbow_mean, num_threads, train_method, model);
    vector<vector<string>> sentences;
    /*
    vector<vector<string>> sentences;
    vector<string> sentence;
    string s = "this is a test";
    sentence.push_back(s);
    vector<string> sentence2;
    string s2 = "check";
    sentence2.push_back(s2);
    sentences.push_back(sentence);
    sentences.push_back(sentence2);


    w2v.build_vocab(sentences);
    w2v.init_weights(w2v.vocab.size());
    w2v.train(sentences);
    //cout<< w2v.C << endl;
    cout << w2v.W << endl;
    Node2Vec *node = new Node2Vec();
*/
    for(int i=0 ;i<node->walks->size(); i++){
        vector<string> sentence;
        for(int j=0; j< node->walks->at(i).size();j++){
            int tmp = node->walks->at(i).at(j);
            sentence.push_back(tmp + "");
        }
        sentences.push_back(sentence);

    }
    w2v.build_vocab(sentences);
    w2v.init_weights(w2v.vocab.size());
    w2v.train(sentences);
    //cout<< w2v.C << endl;
    cout << w2v.W << endl;
    return 0;
}