#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MASTER 0
#define min(x, y) (((x) < (y)) ? (x) : (y) )


int *vec, *result;
int dim;

void print_to_file(int number) {
    FILE *f_out;
    char filename[50];

    sprintf(filename,"%s%d", "./out/mpi/out", number);
    f_out = fopen(filename, "w+");
    
    if (!f_out) {
        printf("OUT FILE NOT FOUND\n");
        exit(1);
    }

    for (int i = 0; i < dim; i++) {
        fprintf(f_out,"%d ", result[i]);
    }
    fclose(f_out);
}

void read_input(int number) {
    FILE *fp;
    char filename[50];
    
    sprintf(filename,"%s%d", "./input/input", number);
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
    int *pos;
    int start, end;
    int count = 0;


	MPI_Init(&argc, &argv);
	MPI_Status status;
    
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    if (rank == MASTER) {
    	read_input(atoi(argv[1]));
    }

    // send the vector to all processes
    MPI_Bcast(&dim, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    pos = calloc(dim, sizeof(int));
    if (rank != MASTER) {
        vec = malloc(sizeof(int) * dim);
    }

    MPI_Bcast(vec, dim, MPI_INT, MASTER, MPI_COMM_WORLD);
  
	start = rank * (double) dim / nProcesses;
	end = min(((rank + 1) * (double) dim / nProcesses), dim);
    
    for (int i = start; i < end; i++) {
        count = 0;
		for (int j = 0; j < dim; j++) {
			if (vec[i] > vec[j] || (vec[i] == vec[j] && j > i)) {
				count++;
			}
		}
        pos[i] = count;
	}

    if (rank == MASTER) {
        result = malloc(sizeof(int) * dim);
        for (int id = 1; id < nProcesses; id++) {
            start = id * (double) dim / nProcesses;
	        end = min(((id + 1) * (double) dim / nProcesses), dim);
            MPI_Recv(pos + start, end - start, MPI_INT, id, 0, MPI_COMM_WORLD, NULL);
        }

        for (int i = 0; i < dim; i++) {
            result[pos[i]] = vec[i];

        }

        print_to_file(atoi(argv[1]));

    } else {
       
        MPI_Send(pos + start, end - start, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    }


	// finalizare program
	MPI_Finalize();
	return 0;
}
