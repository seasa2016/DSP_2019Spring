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

void update_alpha_beta(double **alpha,double **beta,double **gamma,double **epsilon,int max_len,HMM *hmm_model,vector< vector<char> > &data)
{
	double **temp_gamma,***temp_epsilon;
	
	//allocate dynamic memory
	//N*T
	temp_gamma = new double*[hmm_model->state_num];
	for(int j=0 ; j<hmm_model->state_num ; j++)
		temp_gamma[j] = new double[max_len];
	
	//T-1*N*N
	temp_epsilon = new double**[max_len-1];
	for(int j=0 ; j<hmm_model->max_len-1 ; j++)
	{
		temp_epsilon[j] = new double*[hmm_model->state_num];
		for(int k=0 ; k<hmm_model->state_num ; k++)
			temp_epsilon[j][k] = new double[hmm_model->state_num];
	}		
	


	//iteerate over all data
	for(int i=0;i<data.size();i++)
	{
		//in each round re initial matrix
		for(int t=0 ; t<data[i].size() ; t++)
			for(int j=0 ; k<hmm_model->state_num ; j++)
			{
				alpha[j][t] = 0;
				beta[j][t] = 0;
				temp_gamma[j][t] = 0;
			}
		for(int t=0 ; t<data[i].size()-1 ; t++)
			for(int j=0 ; k<hmm_model->state_num ; j++)
				for(int k=0 ; k<hmm_model->state_num ; k++)
					temp_epsilon[t][j][k] = 0;
		
		//do the forward algorithm
		//setting the initial probabilityi
		//alpha_{1}(j) = pi_{j}*b_{j}(o_1) 	1<=i<=N
		//update
		//alpha_{t+1}(j) = \sum_{k=1}^{N}(alpha_{t}(k)*a_{kj}) *b_{j}(o_{t+1})
		//1<=t<=T-1 1<=j<=N

		for(int j=0 ; j<hmm_model->state_num ; j++)
			alpha[j][0] = hmm_model->initial[j] * hmm_model->observation[ data[i][0]-'A' ][ j ];
			
		for(int t=0 ; t<data[i].size()-1 ; t++)
			for(int j=0 ; k<hmm_model->state_num ; j++)
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
		for(int j=0 ; j<hmm_model->state_num ; j++)
		{
			beta[j][ data[i].size()-1 ] = 1;
			
			temp_gamma[j][data[i].size()-1] = alpha[j][data[i].size()-1]*beta[j][data[i].size()-1];
		}

		for(int t=data[i].size()-2 ; t>=0 ; t--)
		{
			for(int j=0 ; j<hmm_model->state_num ; j++)
			{
				//beta
				for(int k=0 ; k<hmm_model->state_num ; k++)
				{
					beta[j][t] += hmm_model->transition[j][k]*beta[k][t+1]*hmm_model->observation[ data[i][t+1]-'A' ][k];
					temp_epsilon[j][k] = alpha[j][t]*hmm_model->transition[j][k]*beta[k][t+1]*hmm_model->observation[ data[i][t+1]-'A' ][k];
				}
				//gamma epsilon
				temp_gamma[j][t] = alpha[j][t]*beta[j][t];
			}
			//update global gamma epsilon
			temp_epsilon[j][k]
			temp_gamma[j][t]
		}
	}	
	//free dynamic memory
	//N*T
	for(int j=0 ; j<hmm_model->state_num ; j++)
		delete temp_gamma[j];
	delete temp_gamma;
	
	//T-1*N*N
	for(int j=0 ; j<hmm_model->max_len-1 ; j++)
	{
		for(int k=0 ; k<hmm_model->state_num ; k++)
			delete temp_epsilon[j][k];
		delete temp_epsilon[j];
	}		
	delete temp_epsilon = new double;
}

void update_pi_a_b(alpha,beta,hmm_model,data)
{

}

void updata(int iterate,HMM* hmm_model,vector< vector<char> >&data )
{
	//do the initial setting
	//check for the variable size and initial the matrix
	int max_len=0;

	for(int i=0;i<data.size();i++)
		if(max_len<data[i].size())
			max_len = data[i].size();

	double **alpha,**beta;
	alpha = new double*[hmm_model->state_num];
	for( int i=0 ; i<hmm_model->state_num;i++)
		alpha[i] = new double[max_len];

	beta = new double*[hmm_model->state_num];
	for( int i=0 ; i<hmm_model->state_num;i++)
		beta[i] = new double[max_len];

	for(int i=0;i<data[0].size();i++)
	{
		update_alpha_beta(alpha,beta,hmm_model,data);
		update_pi_a_b(alpha,beta,hmm_model,data);
	}
}

int main(int argv,char* argc[])
{
	int iteration;
	HMM hmm_initial;
	vector< vector<char> > data;
	loadHMM( &hmm_initial, argc[2]);
	data = load_data(argc[3]);

	update(iteration,&hmm_initial);


	return 0;
}
