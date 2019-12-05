#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_PTHREADS 4
#define MATRIX_SIZE 5000



double** Generate_Random_Matrix()
{
  srand(time(NULL));
  double** matrix = (double**) malloc(MATRIX_SIZE * sizeof(double*));
  for(int i = 0; i < MATRIX_SIZE; i++)
      matrix[i]  = (double*) malloc(MATRIX_SIZE * sizeof(double));
  for(int i = 0; i < MATRIX_SIZE; i++)
  {
    for(int j = 0; j < MATRIX_SIZE; j++)
    {
      matrix[i][j] = rand()%4;
    }
  }

  // for(int i = 0; i < MATRIX_SIZE; i++)
  // {
  //   for(int j = 0; j < MATRIX_SIZE; j++)
  //   {
  //     printf("%lf ", matrix[j][i]);
  //   }
  //   printf("\n");
  // }

  return matrix;
}



typedef struct
{
  double** matrix1;
  double** matrix2;
  int npthread;
  double** result_matrix;
}Data;


void* thread_Function(void* data_void_ptr)
{
  Data* ptr_data = (Data*) data_void_ptr;
  for(int k = ptr_data->npthread; k < MATRIX_SIZE; k+= NUM_PTHREADS)
  {
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
      for(int j = 0; j < MATRIX_SIZE; j++)
        ptr_data->result_matrix[k][i] += ptr_data->matrix1[j][i]*ptr_data->matrix2[k][j];
    }
  }


}

double** start_threads(double** matrixA, double** matrixB)
{
  double** results = (double**) calloc(MATRIX_SIZE, sizeof(double*));
  for(int i = 0; i < MATRIX_SIZE; i++)
    results[i] = (double*) calloc(MATRIX_SIZE, sizeof(double));


  pthread_t* pThreads = (pthread_t*) calloc(NUM_PTHREADS, sizeof(pthread_t)); // array of threads
  Data* args = (Data*) calloc(NUM_PTHREADS, sizeof(Data));// array of arguments


  for(int i = 0; i < NUM_PTHREADS; i++)
  {
      args[i].matrix1 = matrixA;
      args[i].matrix2 = matrixB;
      args[i].npthread = i;
      args[i].result_matrix = results;
      pthread_create(pThreads + i, NULL, thread_Function, &args[i]); // creating threads and starting
  }

  for (int i = 0; i < NUM_PTHREADS; i++) {
    pthread_join(pThreads[i], NULL);// waiting end of threads

    // for(int i = 0; i < MATRIX_SIZE; i++)
    // {
    //   for(int j = 0; j < MATRIX_SIZE; j++)
    //   {
    //     printf("%lf ", results[j][i]);
    //   }
    //   printf("\n");
    // }


  for(int i = 0; i < MATRIX_SIZE; i++)
    free(results[i]);
  free(results);
  free(pThreads);
  free(args);

  return results;
}
}


int main()
{
  double** matrix1 = Generate_Random_Matrix();
  double** matrix2 = Generate_Random_Matrix();
  start_threads(matrix1, matrix2);
  
  return 0;
}
