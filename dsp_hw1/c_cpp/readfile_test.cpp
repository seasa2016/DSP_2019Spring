#include<stdio.h>


int main(void)
{
    //char file_name[] = "./../seq_model_02.txt";
    char file_name[] = "./../debug";
    FILE *f=fopen(file_name,"r");
	if(f==NULL)
	{
		printf("no this file\n");
		return 0;
	}
	int i =0;
	char temp;
	while(!feof(f))
	{
		fscanf(f,"%c",&temp);
        printf("%d ",temp);
        if(temp=='\n')
            printf("\n",temp);
	}
}
