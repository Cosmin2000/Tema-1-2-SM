#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define min(x, y) (((x) < (y)) ? (x) : (y) )

int dim;
int no_threads;
int *pos;
int *result;
int *vec;

void print_to_file(int number) {
    FILE *f_out;
    
    char filename[50];
    sprintf(filename,"%s%d", "./out/pthreads/out", number);

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

// functia executata de fiecare thread
void *thread_func(void *arg)
{
	int thread_id = *(int *)arg;
	int start = thread_id * (double) dim / no_threads;
	int end = min(((thread_id + 1) * (double) dim / no_threads), dim);
	
	for (int i = start; i < end; i++) {
		for (int j = 0; j < dim; j++) {
			if (vec[i] > vec[j] || (vec[i] == vec[j] && j > i)) {
				pos[i]++;
			}
		}
		result[pos[i]] = vec[i];
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    
	
    if (argc < 2) {
        printf("Usage: ./pthreads <NO_THREADS> <input number>\n");
        exit(1);
    }

    read_input(atoi(argv[2]));
    no_threads = atoi(argv[1]);


    int i;
	pthread_t tid[no_threads];
	int threads_id[no_threads];
	
	// se creeaza thread-urile
	for (i = 0; i < no_threads; i++) {
		threads_id[i] = i;
		pthread_create(&tid[i], NULL, thread_func, &threads_id[i]);
	}
	// se asteapta thread-urile
	for (i = 0; i < no_threads; i++) {
		pthread_join(tid[i], NULL);
	}
    
    print_to_file(atoi(argv[2]));
	// eliberare memorie
    free(vec);
	free(pos);
	free(result);
	return 0;
}
