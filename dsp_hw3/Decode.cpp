#include"Decode.h"
const double LogP_PseudoZero = -100;

void Decoder::init(const char lm_filename[],const char map_filename[],int ngram_order)
{
    //init mapping
    this->readmap(map_filename);
    //init language model
    File lmFile( lm_filename, "r" );
    
    this->lm = new Ngram( this->voc, ngram_order );

    this->lm->read(lmFile);
    lmFile.close();
}

void Decoder::findbeststr(const char in_filename[])
{
    //read the input data
    this->readinput(in_filename);

    for(int i=0;i<this->data.size();i++)
        this->find(i);
}

void Decoder::readinput(const char in_filename[])
{
    this->data.clear();
    vector<string> line;

    FILE *fp = fopen(in_filename,"r");
      
    char temp,word[3]={0};
    line.clear();
    line.push_back("<s>");

    while(1)
    {
        fscanf(fp,"%c",&temp);
        if(feof(fp))
            break;
            
        if(temp==10) //for changeline
        {
            line.push_back("</s>");
            this->data.push_back(line);
            line.clear();
            line.push_back("<s>");
        }
        else if(temp!=' ')
        {
            word[0]=temp;
            fscanf(fp,"%c",&word[1]);
            line.push_back(word);
        }
    }
    fclose(fp);
}

void Decoder::readmap(const char map_filename[])
{
    FILE *fp = fopen(map_filename,"r");
    char arr[8192];
    char word[3]={0},ch[3]={0},t;
    int index=0;
    while(true)
	{
		fscanf(fp,"%c%c%c\n",&word[0],&word[1],&t);
		if(feof(fp))
			break;
		if(t==9)
		{
            ch[0] = word[0];
            ch[1] = word[1];
        }
		else
            (this->voc_map)[ch].insert( word ); 
	}
    fclose(fp);
}
VocabIndex Decoder::getIndex(const char *w)
{
    VocabIndex wid1 = this->voc.getIndex(w);

    if(wid1 == Vocab_None)  //OOV
        return this->voc.getIndex(Vocab_Unknown);
    return wid1;
}
double Decoder::getUniProb(const char *w1)
{
    VocabIndex wid1 = this->getIndex(w1);

    VocabIndex context[] = {Vocab_None };
    return this->lm->wordProb( wid1, context);
}

double Decoder::getBigramProb(const char *w1,const char *w2)
{
    VocabIndex wid1 = this->getIndex(w1);
    VocabIndex wid2 = this->getIndex(w2);

    VocabIndex context[] = { wid1, Vocab_None };
    return this->lm->wordProb( wid2, context);
}

double Decoder::getTrigramProb(const char *w1, const char *w2, const char *w3) 
{
    VocabIndex wid1 = this->getIndex(w1);
    VocabIndex wid2 = this->getIndex(w2);
    VocabIndex wid3 = this->getIndex(w3);

    VocabIndex context[] = { wid2, wid1, Vocab_None };
    return this->lm->wordProb( wid3, context );
}

void Decoder::find(int now)
{
    //first find all possible state
    vector<set<string>> state;
    state = this->build_state(this->data[now]);
    //use viterbi to find the probability
    this->viterbi(state);
}
void Decoder::print(vector<string> data)
{
    printf("%s",data[0].c_str());
    for(int i=1;i<data.size();i++)
        printf(" %s",data[i].c_str());
    printf("\n");
}
vector<set<string>> Decoder::build_state(vector<string> line)
{
    //using the vocab to findout all possible state for each timestep
    map<string,set<string> >::iterator it;

    vector< set<string> > state;

    for(int i=0;i<line.size();i++)
    {
        //check if the word in the vocab, or copy it
        it = this->voc_map.find(line[i]);
        if (it != this->voc_map.end())
        {
            state.push_back( it->second);
        }
        else
        {
            set<string> temp;
            temp.insert(line[i]);
            state.push_back(temp);
        }
    }
    return state;
}


void Decoder::max_prob(vector< pair< vector<string> ,double> > &p1, vector< pair< vector<string> ,double> > &p2, string word)
{
    //do some backoff on the prob
    int index=0,now=p2[0].first.size()-1;
    double temp,prob= p2[0].second + this->getBigramProb(p2[0].first[now].c_str(),word.c_str());
    
    for(int i=1;i<p2.size();i++)
    {
        temp = p2[i].second + this->getBigramProb(p2[i].first[now].c_str(),word.c_str());
        
        if(temp>prob)
        {
            index = i;
            prob = temp;
        }
    }
    // update the max one to the p1
    vector<string> line_temp(p2[index].first);
    line_temp.push_back(word);
    p1.push_back( make_pair( line_temp,prob) );
    //printf("%s %s %lf\t",p2[index].first[now].c_str(),word.c_str(),prob);
}

void Decoder::viterbi(vector<set<string>> state)
{
    //for compute the bigram probability, we only need to preserve the last one.
    vector< pair< vector<string> ,double> > path_prob[2]; //one for now, the other for next.
    //initial
    int i;
    double prob;
    for (set<string>::iterator it=state[0].begin(); it!=state[0].end(); it++)
    {
        vector<string> temp;
        temp.push_back(*it);

        prob = this->getUniProb(it->c_str());
        if(prob == LogP_Zero)
            prob = -100;

        path_prob[0].push_back( make_pair( temp,prob) );  //log prob
    }    
    
    //find the max path for each state    
    for(i=1;i<state.size();i++)
    {
        path_prob[i%2].clear();
        //printf("\n-----------------------------------------\n");
        //printf("%d %d %d\n",state[i].size(),path_prob[i%2].size(),path_prob[(i+1)%2].size());
        for (set<string>::iterator it=state[i].begin(); it!=state[i].end(); it++)
            this->max_prob(path_prob[i%2],path_prob[(i+1)%2],*it);
        
    }
    
    //find the largest path at the last timestep
    int min=0;
    i = (i-1)%2;
    /*
    for( int index=1;index<path_prob[i].size();index++)
        if(path_prob[i][min].second < path_prob[i][index].second)
            min = index;
    */
    //print out the largest probability one
    this->print( path_prob[i][min].first );
    
}