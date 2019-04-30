make all
iteration=1000



for(( i=1;i<=5;i++))
do
	./train ${iteration} model_init.txt ./../seq_model_0${i}.txt ./model/model_0${i}.txt
	#./train ${iteration} model_init.txt ./../debug model_0${i}.txt > output
done
cd dsp_hw1/c_cpp/
./train 1000 model_init.txt ./../seq_model_01.txt ./model/model_01.txt
cd dsp_hw1/c_cpp/
./train 1000 model_init.txt ./../seq_model_02.txt ./model/model_02.txt
cd dsp_hw1/c_cpp/
./train 1000 model_init.txt ./../seq_model_03.txt ./model/model_03.txt
cd dsp_hw1/c_cpp/
./train 1000 model_init.txt ./../seq_model_04.txt ./model/model_04.txt
cd dsp_hw1/c_cpp/
./train 1000 model_init.txt ./../seq_model_05.txt ./model/model_05.txt