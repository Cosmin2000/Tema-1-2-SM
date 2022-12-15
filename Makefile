build: openMP mpi pthreads hybrid



openMP: ./OPENMP/openMP.c
	gcc -Wall OPENMP/openMP.c -o openMP -fopenmp

mpi: ./MPI/mpi.c
	mpicc ./MPI/mpi.c -o mpi

pthreads: ./PTHREADS/pthreads.c
	gcc -Wall ./PTHREADS/pthreads.c -o pthreads -lpthread

hybrid: ./HYBRID/hybrid.c
	mpicc -fopenmp ./HYBRID/hybrid.c -o hybrid




run_pthreads: pthreads
	./pthreads 4 1

run_openMP:
	./openMP 4 1

run_mpi:
	mpirun --oversubscribe -np 4 mpi 1

run_hybrid:
	mpirun --oversubscribe -np 4 hybrid 2 




clean_openMP:
	rm -rf openMP

clean_mpi:
	rm -rf mpi

clean_pthreads:
	rm -rf pthreads

clean_hybrid:
	rm -rf pthreads
	
