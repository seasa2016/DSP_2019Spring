for((i=1;$i<=10;i=i+1))
do
	./srilm-1.5.10/bin/i686-m64/disambig -text testdata/seg_${i}.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/${i}.txt &
done
wait
