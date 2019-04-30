import sys
num_state=int(sys.argv[1])
with open("./lib/proto",'w') as f:
    f.write("~o <VECSIZE> 39 <MFCC_Z_E_D_A>\n")
    f.write('~h "proto"\n<BeginHMM>\n')
    f.write("<NumStates> {0}\n".format(num_state))

    for i in range(2,num_state):
        f.write("<State> {0}\n".format(i))
        f.write("<Mean> 39\n")
        f.write("0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0\n")
        f.write("<Variance> 39\n")
        f.write("1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0\n")

    f.write("<TransP> {0}\n".format(num_state))

    f.write('0.0 1.0 ')
    for i in range(num_state-2):
        f.write('0.0 ')
    f.write('\n')

    for i in range(1,num_state-1):
        for j in range(i):
            f.write('0.0 ')
        
        f.write('0.5 0.5 ')
        for j in range(i,num_state-2):
            f.write('0.0 ')
        f.write('\n')

    for i in range(num_state):
        f.write('0.0 ')
    f.write("\n<EndHMM>\n")


