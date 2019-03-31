#include"hmm.h"
#include<stdio.h>
#include<math.h>
#include<vector>

using namespace std;

vector< vector<char> > load_data(char *file_name)
{
	FILE *f=fopen(file_name,"r");
	int i =0;
	char temp;
	vector< vector<char> > data;
	data.push_back(vector<char>());
	while(!feof(f))
	{
		fscanf(f,"%c",&temp);
		if(temp == '\n')
			data.push_back(vector<char>());
		
		else
			data[data.size()-1].push_back(temp);
	}
	data.pop_back();	
	data.pop_back();	
	
	printf("%d\n",data.size());

	return data;
}
void update_model(double **alpha,double **beta,double *temp_gamma,double *total_gamma,double **temp_epsilon,double *initial, double **transition,double **observation,HMM *hmm_model,vector< vector<char> > &data)
{	
	double norm_gamma,norm_epsilon;

	//iterate over all data
	for(int i=0;i<data.size();i++)
	{
		//in each round re initial matrix
		for(int j=0 ; j<hmm_model->state_num ; j++)
		{
			total_gamma[j] = 0;
			temp_gamma[j] = 0;
			initial[j] = 0;
			for(int k=0 ; k<hmm_model->state_num ; k++)
			{
				transition[j][k] = 0;
				temp_epsilon[j][k] = 0;
			}
		}
		for(int j=0 ; j<hmm_model->observ_num ; j++)
			for(int k=0 ; k<hmm_model->state_num ; k++)
				observation[j][k] = 0;
		
		for(int t=0 ; t<data[i].size() ; t++)
			for(int j=0 ; j<hmm_model->state_num ; j++)
			{
				alpha[j][t] = 0;
				beta[j][t] = 0;
			}
		
		//do the forward algorithm
		//setting the initial probabilityi
		//alpha_{1}(j) = pi_{j}*b_{j}(o_1) 	1<=i<=N
		//update
		//alpha_{t+1}(j) = \sum_{k=1}^{N}(alpha_{t}(k)*a_{kj}) *b_{j}(o_{t+1})
		//1<=t<=T-1 1<=j<=N

		for(int j=0 ; j<hmm_model->state_num ; j++)
			alpha[j][0] = hmm_model->initial[j] * hmm_model->observation[ data[i][0]-'A' ][ j ];
			
		for(int t=0 ; t<data[i].size()-2 ; t++)
			for(int j=0 ; j<hmm_model->state_num ; j++)
				for(int k=0 ; k<hmm_model->state_num ; k++)
					alpha[j][t+1] += alpha[k][t]*hmm_model->transition[k][j] * hmm_model->observation[ data[i][t+1]-'A' ][j];
			
		/*
		do the backward algorithm
		we should notice that beta doesnt do with the current state observation probability
		setting the initial probability
		beta_{T}(i) = 1 1<=i<=N
		update beta
		beta_{t}(j) = \sum_{k=1}^{N}( a_{jk} * b_{k}(o_{t+1})*beta_{t+1}(k))
		t=T-1,T-2,...,2,1 1<=i<=N
		*/

		/*
		updata temp_gamma temp_epsilon
		gamma_t(i) = alpha_t(i)*beta_t(i)
		epsilon_t(i,j) = alpha_t(i)*a_{ij}*b_{j}(O_{t+1})*beta_t(j)
		*/
		norm_gamma = 0;
		norm_epsilon = 0;
		for(int j=0 ; j<hmm_model->state_num ; j++)
		{
			beta[j][ data[i].size()-1 ] = 1;
			
			temp_gamma[j] = alpha[j][data[i].size()-1]*beta[j][data[i].size()-1];
			norm_gamma += temp_gamma[j];
		}
		for(int j=0 ; j<hmm_model->state_num ; j++)
		{
			observation[data[i][data[i].size()-1]-'A'][j] += temp_gamma[j]/norm_gamma;
			total_gamma[j] += temp_gamma[j]/norm_gamma;
		}
		for(int t=data[i].size()-2 ; t>=0 ; t--)
		{
			norm_gamma = 0;
			norm_epsilon = 0;
			for(int j=0 ; j<hmm_model->state_num ; j++)
			{
				//beta
				for(int k=0 ; k<hmm_model->state_num ; k++)
				{
					beta[j][t] += hmm_model->transition[j][k]*beta[k][t+1]*hmm_model->observation[ data[i][t+1]-'A' ][k];
					temp_epsilon[j][k] = alpha[j][t]*hmm_model->transition[j][k]*beta[k][t+1]*hmm_model->observation[ data[i][t+1]-'A' ][k];
										
					norm_epsilon += temp_epsilon[j][k];
				}
				//gamma epsilon
				temp_gamma[j] = alpha[j][t]*beta[j][t];
				norm_gamma += temp_gamma[j];
			}
			//update global gamma epsilon
			for(int j=0 ; j<hmm_model->state_num ; j++)
			{
				for(int k=0 ; k<hmm_model->state_num ; k++)
					transition[j][k] += temp_epsilon[j][k]/norm_epsilon;
				
				observation[data[i][t]-'A'][j] += temp_gamma[j]/norm_gamma;
				total_gamma[j] += temp_gamma[j]/norm_gamma;
			}
		}
		for(int j=0 ; j<hmm_model->state_num ; j++)
			initial[j] += temp_gamma[j]/norm_gamma;
	}	
	//calculate  pi A B

	for(int i=0;i<hmm_model->state_num;i++)
	{
		//update pi
		hmm_model->initial[i] = initial[i] / data.size();
		//update transition
		for(int j=0 ; j<hmm_model->state_num ; j++)
			hmm_model->transition[i][j] = transition[i][j] / total_gamma[i];
		//update observation
		for(int j=0 ; j<hmm_model->observ_num ; j++)
			hmm_model->observation[j][i] = observation[j][i] / total_gamma[i];
	}
}

void update(int iterate,HMM* hmm_model,vector< vector<char> >&data )
{
	//do the initial setting
	//check for the variable size and initial the matrix
	int max_len=0;
	double **alpha,**beta,*temp_gamma,*total_gamma,**temp_epsilon,*initial,**transition,**observation;

	for(int i=0;i<data.size();i++)
		if(max_len<data[i].size())
			max_len = data[i].size();

	//allocate dynamic memory
	//N*T
	alpha = new double*[hmm_model->state_num];
	beta = new double*[hmm_model->state_num];
	total_gamma = new double[hmm_model->state_num];
	temp_gamma = new double[hmm_model->state_num];
	temp_epsilon = new double*[hmm_model->state_num];
	transition = new double*[hmm_model->state_num];
	initial = new double[hmm_model->state_num];
	for(int i=0 ; i<hmm_model->state_num ; i++)
	{
		alpha[i] = new double[max_len];
		beta[i] = new double[max_len];
		transition[i] = new double[hmm_model->state_num];
		temp_epsilon[i] = new double[hmm_model->state_num];
	}
	observation = new double*[hmm_model->observ_num];
	for(int i=0 ; i<hmm_model->observ_num ; i++)
		observation[i] = new double[hmm_model->state_num];	

	for(int i=0;i<iterate;i++)
		update_model(alpha,beta,temp_gamma,total_gamma,temp_epsilon,initial,transition,observation,hmm_model,data);
	printf("here\n");
	//free dynamic memory
	for(int i=0 ; i<hmm_model->state_num ; i++)
	{
		delete alpha[i];
		delete beta[i];
		delete temp_gamma;
		delete transition[i];
		delete temp_epsilon[i];
	}
	delete temp_epsilon;
	delete alpha;
	delete beta;
	delete total_gamma;
	delete temp_gamma;
	delete transition;
	delete initial;
	
	//T-1*N*N
	for(int i=0 ; i<hmm_model->observ_num ; i++)
		delete observation[i];	
	delete observation;
	printf("here2\n");
}

int main(int argv,char* argc[])
{
	int iteration;
	HMM hmm_initial;
	sscanf(argc[1],"%d",&iteration);
	vector< vector<char> > data;
	loadHMM( &hmm_initial, argc[2]);
	data = load_data(argc[3]);

	update(iteration,&hmm_initial,data);
	//dumpHMM(fopen(argc[4],"w"),&hmm_initial);
	FILE* f = fopen(argc[4],"w");
	fprintf(f,"test\n");

	return 0;
}
