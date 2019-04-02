make all
iteration=100



for(( i=1;i<=2;i++))
do
	 ./test modellist.txt ./../testing_data${i}.txt result${i}.txt
done
