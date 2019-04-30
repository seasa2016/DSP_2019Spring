for((i=2;$i<=2;i+=2))
do
    for((j=2;$j<=2;j+=2))
    do
        python gen_proto.py $i
        python gen_state.py $j
        ./02_run_HCompV.sh > ./record/$i_$j_2
        ./03_training.sh > ./record/$i_$j_3

        echo "$i $j" >> ./result/accuracy
        ./04_testing.sh > ./record/$i_$j_4
    done
done