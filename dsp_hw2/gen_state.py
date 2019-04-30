import sys
num_state=int(sys.argv[1])

with open("./lib/mix2_10.hed",'w') as f:
    for n in ["liN","#i","#er","san","sy","#u","liou","qi","ba","jiou"]:
        f.write("MU {1} {{{0}.state[2-{1}].mix}}\n".format(n,num_state))
    f.write("MU {0} {{sil.state[2-4].mix}}".format(num_state))



