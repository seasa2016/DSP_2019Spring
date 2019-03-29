.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library

TARGET=test_hmm train test

all: $(TARGET)
# type make/make all to compile test_hmm
	
test_hmm: test_hmm.c
	g++ test_hmm.c -o test_hmm
train: train.cpp
	g++ train.cpp -o train
test:
	#g++ test.c -o test

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
