#include<stdio.h>

int main(void)
{
	FILE *fp = fopen("./../ZhuYin-Big5.map","r");
	char word[2],temp;

	while(true)
	{
		fscanf(fp,"%c%c%c\n",&word[0],&word[1],&temp);
		if(feof(fp))
			break;
		if(temp==9)
			printf("\n");
		
		printf("%c%c%d",word[0],word[1],temp);
	}
}


