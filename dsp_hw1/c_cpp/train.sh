make all
iteration=100



for(( i=1;i<=5;i++))
do
	./train ${iteration} model_init.txt ./../seq_model_0${i}.txt ./model/model_0${i}.txt
	#./train ${iteration} model_init.txt ./../debug model_0${i}.txt > output
done
