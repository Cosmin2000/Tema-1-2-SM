#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int dim;
int no_threads;
int *pos;
int *result;
int *vec;

void print_to_file(int number) {
    FILE *f_out;
    
    char filename[50];
    sprintf(filename,"%s%d", "./out/openMP/out", number);

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
    pos = malloc(sizeof(int) * dim);
    result = malloc(sizeof(int) * dim);
    vec = malloc(sizeof(int) * dim);
    
    for (int i = 0; i < dim; i++) {
        fscanf(fp, "%d", &vec[i]);
        // printf("%d ", vec[i]);
    }
    fclose(fp);
}

 
int main(int argc, char * argv[]) {

	if (argc < 2) {
        printf("Usage: ./openMP <NO_THREADS> <input number>\n");
        exit(1);
    }

    read_input(atoi(argv[2]));
    no_threads = atoi(argv[1]);


	int i, j;
	// setare numar thread-uri
	omp_set_num_threads(no_threads);
	// sortarea
	#pragma omp parallel shared(vec, pos, result)
	{
		// paralelizarea sortarii
		#pragma omp for private(i, j)
		for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++) {
				if (vec[i] > vec[j] || (vec[i] == vec[j] && j > i)) {
					pos[i]++;
				}
			}
			result[pos[i]] = vec[i];
		}
	}

	print_to_file(atoi(argv[2]));
	// eliberare memorie
	free(vec);
	free(pos);
	free(result);
	return 0;
}
