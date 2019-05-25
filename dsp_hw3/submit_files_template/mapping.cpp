//./mapping $(FROM) $(TO)
/*
1 一 ㄧˊ/ㄧˋ/ㄧˍ
2 乙 ㄧˇ
3 丁 ㄉㄧㄥˍ
4 七 ㄑㄧˍ
5 乃 ㄋㄞˇ
6 九 ㄐㄧㄡˇ
7 了 ㄌㄜ˙/ㄌㄧㄠˇ
8 二 ㄦˋ
9 人 ㄖㄣˊ
10 儿 ㄖㄣˊ
11 入 ㄖㄨˋ
12 八 ㄅㄚˍ
13 几 ㄐㄧˍ
14 刀 ㄉㄠˍ
*/
#include<stdio.h>
#include<vector>

using namespace std;

vector<char> convert()
{

}

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("You should input from data and output name\n");
        exit(-1);
    }
    data = convert(argv[1]);
    write(data,argv[2]);

    return 0;
}