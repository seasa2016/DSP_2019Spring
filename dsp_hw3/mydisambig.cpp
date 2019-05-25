#include <stdio.h>
#include "Ngram.h"
#include "Decode.h"
#include <string.h>




void format_warm(string arr)
{
    printf("%s\n",arr.c_str());
    printf("the format should be\n");
    printf("./mydisambig -text [input_file] -map [map_file] -lm [lang_file] -order 2");
    exit(-1);
}

int main(int argc, char *argv[])
{
    //check for input size
    //./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt;
    Decoder de;
    string map="",lm="",text="";
    int order=0;
    if(argc != 9)
    {
        format_warm("size error");
    }
    for(int i=1;i<argc;i+=2)
        if( !strcmp(argv[i],"-map"))
            map=argv[i+1];
        else if(!strcmp(argv[i],"-lm"))
            lm=argv[i+1];
        else if(!strcmp(argv[i],"-order"))
            sscanf(argv[i+1],"%d",&order);
        else if(!strcmp(argv[i],"-text"))
            text=argv[i+1];
    
    if(order==0 || map.size()==0||lm.size()==0||text.size()==0)
        format_warm("some term missing");


    de.init(lm.c_str(),map.c_str(),order);
    de.findbeststr(text.c_str());
}
