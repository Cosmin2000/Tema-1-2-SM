#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MASTER 0


int *vec, *result;
int dim;

void print_to_file() {
    FILE *f_out;
    
    f_out = fopen("out1", "w+");
    
    if (!f_out) {
        printf("OUT FILE NOT FOUND\n");
        exit(1);
    }

    for (int i = 0; i < dim; i++) {
        fprintf(f_out,"%d ", result[i]);
    }
    fclose(f_out);
}

void read_input(char *filename) {
    FILE *fp;

    fp = fopen(filename, "r");
    if (!fp) {
        printf("FILE NOT FOUND\n");
        exit(1);
    }
    fscanf(fp, "%d", &dim);
    vec = malloc(sizeof(int) * dim);
    
    for (int i = 0; i < dim; i++) {
        fscanf(fp, "%d", &vec[i]);
        // printf("%d ", vec[i]);
    }
    fclose(fp);
}


 
int main(int argc, char * argv[]) {
	int rank, i, j;
	int nProcesses;
    int *process_arr;
    int *pos;
    int chunk_size;
    int *result_pos;
    int array_len;

	MPI_Init(&argc, &argv);
	MPI_Status status;
    
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    if (rank == MASTER) {
    	read_input(argv[1]);
        result = malloc(sizeof(int) * dim);
        result_pos = malloc(sizeof(int) * dim);

    }


	
    // printf("%d", rank);

    // send the vector to all processes
    MPI_Bcast(&dim, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank != MASTER) {
        vec = malloc(sizeof(int) * dim);
    }

    MPI_Bcast(vec, dim, MPI_INT, MASTER, MPI_COMM_WORLD);
  
    chunk_size = dim / nProcesses;

    if (dim % nProcesses && rank == nProcesses - 1) {
        array_len = chunk_size + 1;
    } else {
        array_len = chunk_size;
    }
    process_arr = calloc (array_len, sizeof(int));
    pos = calloc (array_len, sizeof(int));


    MPI_Scatter(vec, chunk_size, MPI_INT, process_arr, chunk_size, MPI_INT, MASTER, MPI_COMM_WORLD);
  
    if (dim % nProcesses != 0){
        if (rank == MASTER) {
            MPI_Send(&vec[dim-1], 1, MPI_INT, nProcesses - 1, 0, MPI_COMM_WORLD);
        } else if (rank == nProcesses - 1) {
            MPI_Recv(&process_arr[chunk_size], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        }
    }

    for (int i = 0; i < array_len; i++) {
        for (int j = 0; j < dim ; j++) {
            if (process_arr[i] > vec[j] || (process_arr[i] == vec[j] && (j > i + rank * chunk_size)) ) {
					pos[i]++;
				}
        }
    }
    
    MPI_Gather(pos, chunk_size, MPI_INT, result_pos, chunk_size, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (dim % nProcesses != 0){
        if (rank == MASTER) {
            MPI_Recv(&result_pos[dim-1], 1, MPI_INT, nProcesses - 1, 0, MPI_COMM_WORLD, NULL);
        } else if (rank == nProcesses - 1) {
            MPI_Send(&pos[chunk_size], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    
    if (rank == MASTER) {
        
        for (int i = 0; i < dim; i++) {
            result[result_pos[i]] = vec[i];
        }

    }


	// finalizare program
	MPI_Finalize();
	return 0;
}
