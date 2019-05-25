"""
python mapping.py $(FROM) $(TO)

1 一 ㄧˊ/ㄧˋ/ㄧˍ
2 乙 ㄧˇ
3 丁 ㄉㄧㄥˍ
4 七 ㄑㄧˍ
"""
import sys
import os
import re

def convert(file_name):
    data = {}
    with open(file_name,encoding='BIG5-HKSCS') as f:
        for i,line in enumerate(f):
            #print([line])
            line = re.split('[/\s]',line.strip())
            
            #print(line)
            
            for char in line:
                if(not char):
                    continue
                try:
                    data[ char[0] ].add(line[0])
                except:
                    data[ char[0] ] = set()
                    data[ char[0] ].add(line[0])
            #if(i==10):
            #    break
    #print(data)
    return data

def write(data,file_name):
    with open(file_name,'w',encoding='BIG5-HKSCS') as f:
        for key in sorted(data.keys()):
            f.write('{0}\t'.format(key))
            for word in data[key]:
                f.write('{0} '.format(word))
            f.write('\n')
    


if(__name__ == '__main__'):
    if( len(sys.argv) != 3):
        print("You should input from data and output name\n")
        sys.exit(-1)

    data = convert(sys.argv[1])
    #print(data['ㄅ'])
    write(data,sys.argv[2])

