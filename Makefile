CFLAGS=-O3
all: test_bitvector test_bloomfilter bloomfilter_benchmark

test_bitvector:	test_bitvector.o bitvector.o
	${CC} ${CFLAGS} -o test_bitvector test_bitvector.o bitvector.o

test_bloomfilter: test_bloomfilter.o bitvector.o bloomfilter.o murmur3.o
	${CC} ${CFLAGS} -o test_bloomfilter test_bloomfilter.o bitvector.o bloomfilter.o murmur3.o

bloomfilter_benchmark: bloomfilter_benchmark.o bitvector.o bloomfilter.o murmur3.o
	${CC} ${CFLAGS} -o bloomfilter_benchmark bloomfilter_benchmark.o bitvector.o bloomfilter.o murmur3.o

test_bitvector.o: test_bitvector.c
	${CC} ${CFLAGS} -c test_bitvector.c

test_bloomfilter.o: test_bloomfilter.c
	${CC} ${CFLAGS} -c test_bloomfilter.c

bloomfilter_benchmark.o: bloomfilter_benchmark.c
	${CC} ${CFLAGS} -c bloomfilter_benchmark.c

murmur3.o: murmur3.c
	${CC} ${CFLAGS} -c murmur3.c

bloomfilter.o: bloomfilter.c
	${CC} ${CFLAGS} -c bloomfilter.c

bitvector.o: bitvector.c
	${CC} ${CFLAGS} -c bitvector.c

clean:
	rm -rf *.o test_bitvector test_bloomfilter
