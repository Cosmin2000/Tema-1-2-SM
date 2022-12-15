build: mpi 

mpi: mpi.c
	mpicc mpi.c -o mpi -lm

run:
	mpirun -np 4 mpi ../input1.in

clean:
	rm mpi 
