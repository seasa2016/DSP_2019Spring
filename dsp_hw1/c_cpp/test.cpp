#include "hmm.h"
#include <math.h>
#include<vector>
#define max_num 5
using namespace std;

double viterbi(vector<char> &arr,HMM* hmm)
{
    double state_prob[2][hmm->state_num],max,prob;
    //initial
    for(int i=0 ; i<hmm->state_num ; i++)
    {
        state_prob[0][i] = hmm->initial[i] * hmm->observation[arr[0]-'A'][i];
    }

    for(int t=1;t<arr.size();t++)
    {
        for(int i=0 ; i<hmm->state_num ; i++)
        {
            max = 0;
            for(int j=0 ; j<hmm->state_num ; j++)
            {
                prob = state_prob[(t+1)%2][j]*hmm->transition[j][i];
                if(prob>max)
                    max = prob;
            }
            state_prob[t%2][i] = max*hmm->observation[arr[t]-'A'][i];
        }
    }
    max = -1;
    for(int i=0 ; i<hmm->state_num ; i++)
    {
        if(state_prob[(arr.size()-1)%2][i]>max)
            max = state_prob[(arr.size()-1)%2][i];
    }
    return max;
}
void compute(HMM* hmms,FILE* f_in,FILE* f_out)
{
    vector<char> arr;
    char temp;
    int max,count=0;
    if(f_in==NULL)
    {
        printf("no this file\n");
        exit(-1);
    }
    fscanf(f_in,"%c",&temp);
    
    double now=0,prob=0;
    int index=0;

    while(!feof(f_in))
    {
        if(temp =='\n')
        {
            now = -1;
            index=-1;
            count++;   
            for(int i=0;i<max_num;i++)
            {
                prob = viterbi(arr,&hmms[i]);
                if(prob>now)
                {
                    now = prob;
                    index = i;
                }
            }
            fprintf(f_out,"%s %e\n",hmms[index].model_name,now);
            arr.clear();
        }            
        else
            arr.push_back(temp);
        
        fscanf(f_in,"%c",&temp);
    }
}

int main(int argv,char* argc[])
{
	HMM hmms[5];
	load_models( argc[1], hmms, max_num);
	//dump_models( hmms, 5);

    compute(hmms,fopen(argc[2],"r"),fopen(argc[3],"w"));
	return 0;
}
