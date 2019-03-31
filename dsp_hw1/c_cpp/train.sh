make all
iteration=1

for(( i=1;i<=5;i++))
do
	./train ${iteration} model_init.txt ./../seq_model_0${i}.txt model_0${i}.txt
done
