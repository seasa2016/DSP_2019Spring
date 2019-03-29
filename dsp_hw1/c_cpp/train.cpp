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
		{
			data.push_back(vector<char>());
		}
		else
		{
			data[data.size()-1].push_back(temp);
		}
		
	}
	data.pop_back();	
	data.pop_back();	
	
	printf("%d\n",data.size());

	return data;
}

void update_alpha_beta(double **alpha,double **beta,double **gamma,double **epsilon,int max_len,HMM *hmm_model,vector< vector<char> > &data)
{
	double **temp_gamma,**temp_epsilon;

	temp_gamma = new double*[hmm_model->state_num];
	temp_epsilon = new double*[hmm_model->state_num];

	for(int j=0 ; j<hmm_model->state_num ; j++)
	{
		temp_gamma[j] = new double[max_len];
		temp_epsilon[j] = new double[max_len];
	}


	//iteerate over all data
	for(int i=0;i<data.size();i++)
	{
		//in each round re initial matrix
		for(int j=0 ; j<hmm_model->state_num ; j++)
			for(int k=0 ; k<data[i].size() ; k++)
			{
				alpha[j][k] = 0;
				beta[j][k] = 0;
				temp_gamma[j][k] = 0;
				temp_epsilon[j][k] = 0;
			}
		
		//do the forward algorithm
		//setting the initial probabilityi
		//alpha_{1}(i) = pi_{i}*b_{i}(o_1) 	1<=i<=N

		//update
		//alpha_{t+1}(j) = \sum_{i=1}^{N}(alpha_{t}(i)*a_{ij}) *b_{j}(o_{t+1})
		//1<=t<=T-1 1<=j<=N
		for(int j=0 ; j<hmm_model->state_num ; j++)
			for(int k=0 ; k<data[i].size()-1 ; k++)
			{
				if(k==0)
					for(int j=0 ; j<hmm_model->state_num ; j++)
						alpha[j][0] = hmm_model->initial[j] * hmm_model->observation[ data[i][0]-'A' ][ j ];
				else:
					for(int l=0 ; l<hmm_model->state_num ; l++)
						alpha[j][k+1] += alpha[l][k]*hmm_model->transition[l][j] * hmm_model->observation[ data[i][k+1] ][l];
			}

		//do the backward algorithm
		//we should notice that beta doesnt do with the current state observation probability
		//setting the initial probability
		//beta_{T}(i) = 1 1<=i<=N
		for(int j=0 ; j<hmm_model->state_num ; j++)
			beta[j][ data[i].size()-1 ] = 1;

		for(int j=0 ; j<hmm_model->state_num ; j++)
		{
			for(int k=data[i].size()-1 ; k>0 ; k--)
			{
				//update beta
				//beta_{t}(i) = \sum_{j=1}^{N}( a_{ij} * b_{j}(o_{t+1})*beta_{t+1}(j))
				//t=T-1,T-2,...,2,1 1<=i<=N
				for(int l=0 ; l<hmm_model->state_num ; l++)
					beta[j][k-1] += alpha[l][j]*beta[l][k]*hmm_model->observation[ data[i][k] ][l];
				

				//updata gamma epsilon
				
			}
		}
	
	}
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
