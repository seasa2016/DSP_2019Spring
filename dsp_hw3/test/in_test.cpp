#include<stdio.h>

int main(void)
{
    FILE *fp=fopen("./../testdata/seg_1.txt","r");
    char temp,word[3]={0};

    while(1)
    {
        fscanf(fp,"%c",&temp);
        if(feof(fp))
            break;
        printf("%d%c",temp,temp);
        if(temp==10) //for changeline
        {
            printf("\n");
        }
        else if(temp!=' ')
        {
            word[0]=temp;
            fscanf(fp,"%c",&word[1]);
            printf("%c",word[1]);
        }
    }
    fclose(fp);
    return 0;
}