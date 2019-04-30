import sys

t,f = 0,0
with open(sys.argv[1],'r') as f_1:
    with open(sys.argv[2],'r') as f_2:
        for line1,line2 in zip(f_1,f_2):
            line1 = line1.strip().split()[0]
            line2 = line2.strip()
                  
            if(line1[-6:] == line2[-6:]):
                t+=1
            f+=1
print(t/f)