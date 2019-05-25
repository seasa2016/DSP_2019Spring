#include<vector>
#include"Ngram.h"
#include<map> 
#include<set> 
#include<utility>
#include<string>
#include<stdio.h>


using namespace std;
class Decoder
{
    public:
        void findbeststr(const char in_filename[]);
        void init(const char lm_filename[],const char map_filename[],int ngram_order);

    private:
        Vocab voc;
        Ngram *lm;
        map<string,set<string> > voc_map;
        vector< vector<string> > data;

        void find(int);
        void print(vector<string> data);
        VocabIndex getIndex(const char *w);
        double getUniProb(const char *w1);
        double getBigramProb(const char *w1, const char *w2);
        double getTrigramProb(const char *w1, const char *w2, const char *w3);
        void readmap(const char filename[]);
        void readinput(const char filename[]);
        void max_prob(vector< pair< vector<string> ,double> > &,vector< pair< vector<string> ,double> > &,string word);

        vector<set<string>> build_state(vector<string>);
        void viterbi(vector<set<string>> state);
};